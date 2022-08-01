#ifndef DE_AUDIO_SAMPLEBUFFER_HPP
#define DE_AUDIO_SAMPLEBUFFER_HPP

#include <de/audio/SampleType.hpp>
#include <vector>
#include <list>
#include <deque>
#include <mutex>
#include <atomic>

namespace de {
namespace audio {

// A SampleBuffer can hold audio data of arbitrary format ( stored as vector of bytes ).
// It can convert() to another formats using SampleTypeConverter.
// Can read() from and write() to other buffers, get() and set() sample data.
// Can interleave() and deinterleave with the same function.
// Can create a sub() buffer like a substr() for dsp and handy calls.
//
// SampleBufferLoadWAV: Can load WAV,OGG,FLAC,etc using SampleBufferLoadWAV using libsndfile ( 1MB )
// SampleBufferLoadMP3: Can load WAV,OGG,FLAC,MP3,MP4,MKV,AVI,MOV,3GP,RM using libAVCodec ( 105MB )
//
// A SampleBufferQueue can handle input and output buffer of different size.
// Can be resampled to other sampleRate using libSoundTouch.
// Will resample to other sampleRate using swResample from ffmpeg later.

// ===========================================================================
struct SampleBuffer
// ===========================================================================
{
   typedef std::unique_ptr< SampleBuffer > Ptr;
   typedef std::unique_ptr< SampleBuffer > UniquePtr;
   typedef std::shared_ptr< SampleBuffer > SharedPtr;

   DE_CREATE_LOGGER("de.audio.SampleBuffer")

   // The data member fields:
   ESampleType m_SampleType; // 2B, bitflags composing the sample type. ( e.g. Interleaved or planar )
   uint16_t m_ChannelCount;  // 2B, num channels [0..65535]
   uint32_t m_BytesPerSample; // 4B, precomputed in setSampleType() to have fast getter
   float m_SampleRate;     // 4B, framerate in [Hz]
   uint64_t m_FrameCount;  // 8B, computed num frames in m_Samples, precomputed to have fast getter.
   double m_PTS;           // 8B, presentation time stamp in [s] to sync with videos and other audios.
   std::string m_Uri;      // XB, name or filename, used by a buffer manager or media player
   std::vector< uint8_t > m_Samples; // XB, actual data in bytes to be most generic and flexible.

public:
   SampleBuffer();
   SampleBuffer( ESampleType sampleType, int channels, float sampleRate, uint64_t frames = 0 );
   ~SampleBuffer();

   static Ptr createPtr( ESampleType sampleType, int channels, float sampleRate, uint64_t frames = 0 );
   static SharedPtr createShared( ESampleType sampleType, int channels, float sampleRate, uint64_t frames = 0 );
   static SampleBuffer create( ESampleType sampleType, int channels, float sampleRate, uint64_t frames = 0 );
   static SampleBuffer* createNew( ESampleType sampleType, int channels, float sampleRate, uint64_t frames = 0 );

   //SampleBuffer& operator= ( SampleBuffer const & other );

   void dump();
   void setUri( std::string const & uri );
   void setChannelCount( int32_t channels );
   void setSampleRate( float rateInHz );
   void setSampleType( ESampleType sampleType );
   void setFormat( ESampleType sampleType, int channels, float sampleRate );
   bool isValid() const;
   // WaveFormat64 const & getFormat() const { return m_Header; }
   void clear();
   void resize( uint64_t frameCount );
   void shrink_to_fit();

   std::string toString( bool printEnergy = false ) const;
   double getPTS() const;
   void setPTS( double pts );
   std::string getSampleTypeStr() const;
   ESampleType getSampleType() const;
   uint32_t getBytesPerSample() const;
   uint32_t getBitsPerSample() const;
   uint32_t getChannelCount() const;
   uint32_t getFrameSize() const;
   bool isInterleaved() const;
   float getSampleRate() const;
   uint64_t getFrameCount() const;
   uint64_t getSampleCount() const;
   //uint64_t capacity() const;
   uint64_t getByteCount() const;
   uint64_t size() const;
   uint8_t* data();
   uint8_t const* data() const;
   template < typename T > T const*
   readPtr() const { return reinterpret_cast< T const* >( data() ); }
   template < typename T > T*
   writePtr() { return reinterpret_cast< T* >( data() ); }
   double getDuration() const;
   std::string const & getUri() const;
   uint64_t getStride() const;
   // but ffmpeg want an lumpy signed int 32bit,
   // so i dont know how future prove this can be, i guess never.
   int32_t getStridei() const;

   uint64_t computeByteIndex( uint64_t frame, uint32_t channel = 0 ) const;
   // if planar (ch0,ch0,ch1,ch1): get full frameCount times bps, which is ( in worst case ) already 64bit
   // if interleaved (ch0,ch1,ch0,ch1) : get number of channels times bps.
   // Frames are x-axis, channels are on y-axis, buffer is like an image.
   uint8_t* getSamples( uint64_t frame = 0, uint32_t channel = 0 );
   uint8_t const* getSamples( uint64_t frame = 0, uint32_t channel = 0 ) const;
   // Access samplebuffer like an image with frames x channels samples.
   // This way you dont need to know if the data is interleaved or planar, but is a bit slower if accessing in wrong order.
   // Still safer and easier to use and leads to more general algos, that can be specialized/optimized very easily afterwards.
   bool getSample( uint64_t frame, uint32_t channel, void * dst ) const;
   bool getSample( uint64_t frame, uint32_t channel, void * dst, ESampleType dstType ) const;
   bool getSample( uint64_t frame, uint32_t channel, void * dst, SampleTypeConverter::Converter_t const & converter ) const;
   bool setSample( uint64_t frame, uint32_t channel, void const * src );
   bool setSample( uint64_t frame, uint32_t channel, void const * src, ESampleType srcType );

   bool setSampleS16( uint64_t frame, uint32_t channel, int16_t value );
   int16_t getSampleS16( uint64_t frame, uint32_t channel ) const;
   bool setSampleF32( uint64_t frame, uint32_t channel, float value );
   float getSampleF32( uint64_t frame, uint32_t channel ) const;

   // bidirectional operation, like a butterfly op, relative expensive but often needed and handy.
   void interleave();
   bool convert( SampleBuffer & dst, ESampleType dstType = ST_Unknown ) const;
   bool sub( SampleBuffer & dst,
             uint64_t frameStart,
             uint64_t frameCount,
             ESampleType dstType = ST_Unknown ) const;

   //SampleBuffer sub( uint64_t frameStart, uint64_t frameCount, ESampleType dstType = ST_Unknown ) const;
   uint64_t write( uint64_t dstFrameStart, SampleBuffer src, uint64_t srcFrameStart, uint64_t srcFrameCount );
   uint64_t write( uint64_t frameStart, SampleBuffer src );
   uint64_t read( uint64_t frameStart, uint64_t frameCount, uint32_t dst_channels, void* dst, ESampleType dstType = ST_Unknown ) const;

   uint64_t read( uint64_t srcFrameStart, SampleBuffer & dst, uint64_t dstFrameStart = 0 ) const;

   uint64_t zeroMemory( uint64_t frameStart = 0 );


   double getChannelAverageEnergyPerSample( int channel ) const;
// bool saveWAV_noperf( std::string uri, ESampleType dstType = ST_S16I ) const;
// bool saveWAV( std::string uri, ESampleType dstType = ST_S16I ) const;
// bool loadWAV_noperf( std::string uri, ESampleType dstType = ST_S16I ) const;
// bool loadWAV( std::string uri, ESampleType dstType = ST_S16I ) const;
};



// ===========================================================================
struct SampleBufferList
// ===========================================================================
{
   typedef std::unique_ptr< SampleBufferList > UniquePtr;
   typedef std::shared_ptr< SampleBufferList > SharedPtr;

   DE_CREATE_LOGGER("de.audio.SampleBufferList")
   std::vector< SampleBuffer > m_Chunks;
   uint64_t m_TotalBytes;
   uint64_t m_TotalFrames;
   double m_TotalDuration;

   std::vector< uint64_t > m_FrameStarts;
   std::vector< double > m_DurationStarts;

   SampleBufferList()
      : m_TotalBytes( 0 )
      , m_TotalFrames( 0 )
      , m_TotalDuration( 0.0 )
   {
      DE_DEBUG("Constructor")
   }

   ~SampleBufferList()
   {
      DE_DEBUG("Destructor")
      clear();
   }

   void clear()
   {
      m_Chunks.clear();
      m_FrameStarts.clear();
      m_DurationStarts.clear();
      m_TotalBytes = 0;
      m_TotalFrames = 0;
      m_TotalDuration = 0.0;
   }
   uint32_t size() const { return m_Chunks.size(); }
   uint64_t totalFrames() const { return m_TotalFrames; }
   uint64_t totalBytes() const { return m_TotalBytes; }
   double totalDuration() const { return m_TotalDuration; }

   std::string
   toString() const
   {
      std::stringstream s;
      s << "chunks:" << m_Chunks.size();
      s << ", dur:" << dbSecondsToString( totalDuration() );
      s << ", frames:" << totalFrames();
      s << ", bytes:" << dbStrByteCount( totalBytes() );
      return s.str();
   }

   void
   append( SampleBuffer const & chunk )
   {
      if ( chunk.getFrameCount() < 1 )
      {
         //DE_ERROR("Does not add empty")
         return;
      }

      //DE_DEBUG("Append chunk [",m_Chunks.size(),"] ", chunk.toString())
      auto duration = chunk.getDuration();
      auto bytes = chunk.getByteCount();
      auto frames = chunk.getFrameCount();
      m_TotalFrames += frames;
      m_TotalDuration += duration;
      m_TotalBytes += bytes;
      m_Chunks.push_back( chunk );

      // Build frame index
      if ( m_FrameStarts.empty() )
      {
         m_FrameStarts.push_back( 0 );
      }
      else
      {
         m_FrameStarts.push_back( m_FrameStarts.back() + frames );
      }

      // Build duration index
      if ( m_DurationStarts.empty() )
      {
         m_DurationStarts.push_back( 0.0 );
      }
      else
      {
         m_DurationStarts.push_back( m_DurationStarts.back() + duration );
      }
   }

   // All conversion depends on first buffer format pushed to list.
   bool
   join( SampleBuffer & dst ) const
   {
      if ( m_Chunks.size() < 1 )
      {
         // dst.reset(); // Make invalid
         return false; // Indicate nothing done.
      }

      if ( m_Chunks.size() < 2 )
      {
         dst = m_Chunks.back(); // Copy last element ( = first and only element )
         return true;
      }

      ESampleType dstType = m_Chunks.front().getSampleType();
      int dstChannels = m_Chunks.front().getChannelCount();
      float dstRate = m_Chunks.front().getSampleRate();

      DE_DEBUG("dstType(", ST_toString(dstType), "), "
               "dstChannels(", dstChannels, "), "
               "dstRate(", dstRate, ")")
      try
      {
         dst.clear();
         dst.setFormat( dstType, dstChannels, dstRate );
         dst.resize( m_TotalFrames );
      }
      catch( ... )
      {
         DE_ERROR("OOM for frames(", m_TotalFrames, "), "
                        "dstType(", ST_toString(dstType), "), "
                        "dstChannels(", dstChannels, "), "
                        "dstRate(", dstRate, ")")
         return false;
      }

      size_t frameIndex = 0;

      for ( size_t i = 0; i < m_Chunks.size(); ++i )
      {
         frameIndex += dst.write( frameIndex, m_Chunks[ i ] );
      }

      DE_DEBUG("frameIndex(", frameIndex, "), JOINED ", dst.toString() )
      return true;
   }

};


// ===========================================================================
struct UniqueBufferList
// ===========================================================================
{
   DE_CREATE_LOGGER("de.audio.UniqueBufferList")

   std::vector< SampleBuffer::UniquePtr > m_Chunks;
   uint64_t m_TotalBytes;
   uint64_t m_TotalFrames;
   double m_TotalDuration;

   uint64_t m_FrameIndex;

   std::vector< uint64_t > m_FrameStarts;
   std::vector< double > m_DurationStarts;

   UniqueBufferList()
      : m_TotalBytes( 0 )
      , m_TotalFrames( 0 )
      , m_TotalDuration( 0.0 )
      , m_FrameIndex( 0 )
   {
      DE_DEBUG("Constructor")
   }

   ~UniqueBufferList()
   {
      DE_DEBUG("Destructor")
      clear();
   }

   void clear()
   {
      m_Chunks.clear();
      m_FrameStarts.clear();
      m_DurationStarts.clear();
      m_TotalBytes = 0;
      m_TotalFrames = 0;
      m_TotalDuration = 0.0;
   }
   uint32_t size() const { return m_Chunks.size(); }
   uint64_t totalFrames() const { return m_TotalFrames; }
   uint64_t totalBytes() const { return m_TotalBytes; }
   double totalDuration() const { return m_TotalDuration; }

   std::string
   toString() const
   {
      std::stringstream s;
      s << "chunks:" << m_Chunks.size();
      s << ", dur:" << dbSecondsToString( totalDuration() );
      s << ", frames:" << totalFrames();
      s << ", bytes:" << dbStrByteCount( totalBytes() );
      return s.str();
   }

   uint64_t
   read( SampleBuffer & dst )
   {
      uint64_t dstFrames = dst.getFrameCount();

//      DE_DEBUG("m_FrameIndex = ", m_FrameIndex )
//      DE_DEBUG("m_TotalFrames = ", m_TotalFrames )
//      DE_DEBUG("dstFrames = ", dstFrames )

      if ( m_FrameIndex + dstFrames > m_TotalFrames )
      {
         DE_ERROR("Data too low")
         DE_ERROR("m_FrameIndex = ", m_FrameIndex)
         DE_DEBUG("m_TotalFrames = ", m_TotalFrames )
         DE_DEBUG("dstFrames = ", dstFrames )
         //dst.fill_zero();
         return 0;
      }

      uint64_t frameCounter = 0;

      // find start chunk
      int32_t chunkIndex = -1;
      uint64_t frameIndex = 0;

      for ( size_t i = 0; i < m_Chunks.size(); ++i )
      {
         SampleBuffer const * const chunkPtr = m_Chunks[ i ].get();
         if ( !chunkPtr ) continue;
         SampleBuffer const & chunk = *chunkPtr;

         uint64_t chunkFrames = chunk.getFrameCount();

         if ( m_FrameIndex >= frameCounter &&
              m_FrameIndex < frameCounter + chunkFrames )
         {
            chunkIndex = int32_t( i );
            frameIndex = m_FrameIndex - frameCounter;
            break;
         }

         frameCounter += chunkFrames;
      }

      if ( chunkIndex < 0 )
      {
         DE_ERROR("No chunk found")
         DE_ERROR("m_FrameIndex = ", m_FrameIndex)
         DE_DEBUG("m_TotalFrames = ", m_TotalFrames )
         DE_DEBUG("dstFrames = ", dstFrames )
         //dst.fill_zero();
         return 0;
      }
      uint64_t writeCounter = 0;
      uint64_t writeIndex = 0;

      while ( dstFrames > 0 )
      {
         SampleBuffer const * const chunkPtr = m_Chunks[ chunkIndex ].get();
         if ( !chunkPtr ) continue;
         SampleBuffer const & chunk = *chunkPtr;
         uint64_t availFrames = chunk.getFrameCount() - frameIndex;
         if ( availFrames > dstFrames )
         {
            availFrames = dstFrames;
         }
         uint64_t writeCount = dst.write( writeIndex, chunk, frameIndex, availFrames );
         frameIndex = 0;
         dstFrames -= writeCount;
         chunkIndex++;
         writeIndex += writeCount;
         writeCounter += writeCount;
      }

      m_FrameIndex += writeCounter;

      //DE_DEBUG("Read ",dst.getFrameCount()," frames, got ", writeCounter)
      return writeCounter;
   }

   void
   append( SampleBuffer::UniquePtr && chunk )
   {
      if ( !chunk || chunk->getFrameCount() < 1 )
      {
         DE_ERROR("Does not add empty")
         return;
      }

      //DE_DEBUG("Append chunk [",m_Chunks.size(),"] ", chunk->toString())
      auto duration = chunk->getDuration();
      auto bytes = chunk->getByteCount();
      auto frames = chunk->getFrameCount();
      m_TotalFrames += frames;
      m_TotalDuration += duration;
      m_TotalBytes += bytes;
      m_Chunks.emplace_back( std::move( chunk ) );

      // Build frame index
      if ( m_FrameStarts.empty() )
      {
         m_FrameStarts.push_back( 0 );
      }
      else
      {
         m_FrameStarts.push_back( m_FrameStarts.back() + frames );
      }

      // Build duration index
      if ( m_DurationStarts.empty() )
      {
         m_DurationStarts.push_back( 0.0 );
      }
      else
      {
         m_DurationStarts.push_back( m_DurationStarts.back() + duration );
      }
   }

   // All conversion depends on first buffer format pushed to list.
   bool
   join( SampleBuffer & dst ) const
   {
      if ( m_Chunks.size() < 1 )
      {
         // dst.reset(); // Make invalid
         return false; // Indicate nothing done.
      }

      if ( m_Chunks.size() < 2 )
      {
         dst = *m_Chunks.back(); // Copy last element ( = first and only element )
         return true;
      }

      ESampleType dstType = m_Chunks.front()->getSampleType();
      int dstChannels = m_Chunks.front()->getChannelCount();
      float dstRate = m_Chunks.front()->getSampleRate();

      DE_DEBUG("dstType(", ST_toString(dstType), "), "
               "dstChannels(", dstChannels, "), "
               "dstRate(", dstRate, ")")
      try
      {
         dst.clear();
         dst.setFormat( dstType, dstChannels, dstRate );
         dst.resize( m_TotalFrames );
      }
      catch( ... )
      {
         DE_ERROR("OOM for frames(", m_TotalFrames, "), "
                        "dstType(", ST_toString(dstType), "), "
                        "dstChannels(", dstChannels, "), "
                        "dstRate(", dstRate, ")")
         return false;
      }

      size_t frameIndex = 0;

      for ( size_t i = 0; i < m_Chunks.size(); ++i )
      {
         if ( !m_Chunks[ i ] ) continue;
         frameIndex += dst.write( frameIndex, *m_Chunks[ i ] );
      }

      DE_DEBUG("frameIndex(", frameIndex, "), JOINED ", dst.toString() )
      return true;
   }

};

// ===========================================================================
struct SampleBufferQueue
// ===========================================================================
{
   typedef std::unique_ptr< SampleBufferQueue > Ptr;
   typedef std::unique_ptr< SampleBufferQueue > UniquePtr;
   typedef std::shared_ptr< SampleBufferQueue > SharedPtr;

   DE_CREATE_LOGGER("de.audio.SampleBufferQueue")
   std::deque< SampleBuffer::Ptr > m_Queue;
   uint64_t m_FrameCount;
   uint64_t m_FrameIndex;
   uint64_t m_ChunkIndex;
   uint64_t m_FrameOffset;
   mutable std::mutex m_Mutex;

   SampleBuffer m_WorkBuffer;

   SampleBufferQueue();
   ~SampleBufferQueue();

   void
   push_back( SampleBuffer::Ptr && src, double pts );

   uint64_t
   pop_front( SampleBuffer & dst, double pts );

   void clear();
   void dump();
   std::string toString() const;
   uint64_t getChunkIndex() const { return m_ChunkIndex; }
   uint64_t getChunkOffset() const { return m_FrameOffset; }
   uint64_t getChunkCount() const { return m_Queue.size(); }
   uint64_t getFrameIndex() const { return m_FrameIndex; }
   uint64_t getFrameCount() const { return m_FrameCount; }
   uint64_t getAvailCount() const { return m_FrameCount - (m_FrameIndex + m_FrameOffset ); }
};

// ===========================================================================
struct SampleBufferQueueTest
// ===========================================================================
{
   DE_CREATE_LOGGER("de.audio.SampleBufferQueueTest")

   static void
   test();
};

} // end namespace audio
} // end namespace de

#endif

