#ifndef DE_AUDIO_FILESTREAM_HPP
#define DE_AUDIO_FILESTREAM_HPP

/*
#include <de/audio/BufferQueue.hpp>
#include <de/audio/DecoderFFMPEG.hpp>
#include <de/audio/IDspElement.hpp>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace de {
namespace audio {

// Uses libAVCodec/ffmpeg to decode any known file format to normal humans.
// Creates a background thread opening the file solely to load audio from it.
// It blocks until data is cached for playing and then signals the condition_var.
// All but one audio stream is enabled, all other packets are discarded.
// So far not seekable but also works for files with broken index.
// So it works for any audio/movie file

struct FileStream : public IDspElement
{
   typedef std::vector< float > SampleBuffer;

   DE_CREATE_LOGGER("de.audio.FileStream")
   DecoderFFMPEG m_file;
   std::atomic< bool > m_isPlaying;
   uint32_t m_channels;
   uint32_t m_sampleRate;

   std::mutex m_cacheMutex;
   SampleBuffer m_buffer1;       // 2 buffers making up a double buffer
   float* m_buffer;
   uint32_t m_bufferCount;
   uint32_t m_bufferIndex;
   uint32_t m_precacheMin;
   uint32_t m_precacheMax;
   uint32_t m_preloadChunk;

   std::string m_uri;

   FileStream()
      : m_threadPtr( nullptr )
      , m_isRunning( false )
      , m_didRun( false )
      , m_streamIndex( -1 )
      , m_channels( 0 )
      , m_sampleRate( 0 )
      , m_readBuffer( nullptr )
      , m_writeBuffer( nullptr )
      , m_readCount( 0 )
      , m_writeCount( 0 )
      , m_precacheMin( 10000 )
      , m_precacheMax( 40000 )
      , m_preloadChunk( 8000 )
   {
      DE_DEBUG("Constructor")
      m_buffer1.resize( 65536 );
      m_buffer2.resize( 65536 );
      m_readBuffer = m_buffer1.data();
      m_writeBuffer = m_buffer2.data();
   }

   ~FileStream()
   {
      DE_DEBUG("Destructor")
      close();
   }

   uint64_t
   readSamples( double pts,
                float* dst,
                uint32_t dstFrames,
                uint32_t dstChannels,
                uint32_t dstRate ) override
   {
      size_t dstSamples = dstFrames * dstChannels;

      if ( m_readCount - m_readIndex < dstSamples )
      {
         if ( m_writeCount < dstSamples )
         {
            fillCache( 2*dstSamples ); // Emergency caching
            DE_ERROR("Emergency caching")
         }
         swapCacheProtected();
      }

      if ( m_readCount - m_readIndex < dstSamples )
      {
         DE_ERROR("m_readCount - m_readIndex < dstSamples")
         DSP_FILLZERO( dst, dstSamples );
         return dstSamples;
      }

      DSP_COPY( m_readBuffer + m_readIndex, dst, dstSamples );
      m_readIndex += dstSamples;
      if ( m_readIndex >= m_readCount )
      {
         fillCache( 2*dstSamples );
         DE_ERROR("Emergency II caching")
      }

      return dstSamples;
   }

   void swapCacheProtected()
   {
      std::unique_lock< std::mutex > lg( m_cacheMutex );
      std::swap( m_readBuffer, m_writeBuffer );
      std::swap( m_readCount, m_writeCount );
      m_readIndex = 0;
   }

   void fillCache( uint32_t sampleCount )
   {
      auto handlePayload = [&]( Payload const & payload )
      {
         uint64_t dstSamples = uint64_t( payload.frameCount ) * payload.channelCount;
         DSP_COPY( payload.samples.data(), m_writeBuffer + m_writeCount, dstSamples );
         m_writeCount += dstSamples;
      };

      size_t runCounter = 0;
      while ( m_writeCount < sampleCount )
      {
         DE_DEBUG("Run[",runCounter,"] cachedSamples = ",m_writeCount)
         runCounter++;

         int e = m_file->readFrame( handlePayload );
         if ( e == AVERROR_EOF )
         {
            DE_DEBUG("AVERROR_EOF")
            break;
         }
      }

      DE_DEBUG("Caching done after ", runCounter, " runs. precacheMin = ", m_precacheMin)
   }

   //uint64_t getFrameCount() const { return m_bufferQueue.getFrameCount(); }
   //uint64_t getFrameIndex() const { return m_bufferQueue.getFrameIndex(); }
   bool isRunning() const { return m_isRunning; }
   bool isPlaying() const { return m_isPlaying; }

   void close()
   {
      m_isRunning = false;
      DE_DEBUG("Close")
      if ( m_threadPtr ) { m_threadPtr->join(); delete m_threadPtr; m_threadPtr = nullptr; }
      m_file.reset();
   }

   bool open( std::string uri, int streamIndex = -1 )
   {
      close();
      m_uri = uri;
      m_streamIndex = streamIndex;
      m_threadPtr = new std::thread( &FileStream::run, this );

      std::mutex m;
      std::unique_lock< std::mutex > guard( m );
      m_runFence.wait( guard, [&] () { return m_didRun; } );

      if ( m_isRunning )
      {
         DE_DEBUG("Loader running ...")
      }
      else
      {
         DE_ERROR("Loader finished")
      }

      return m_isRunning;
   }

   void run()
   {
      m_didRun = true;
      m_file.reset( new DecoderFFMPEG() );
      m_isRunning = m_file->open( m_uri, m_streamIndex, true );
      if ( !m_isRunning )
      {
         DE_ERROR("Cant open uri ", m_uri )
         m_runFence.notify_all();      // Abort for videos only
         return;
      }
      m_streamIndex = m_file->m_data.streamIndex;

//      size_t totalChunks = 0;
//      size_t totalBytes = 0;
//      size_t totalProcessedBytes = 0;
//      double totalDuration = 0.0;
//      size_t frameCounter = 0;

      DE_DEBUG("Opened uri ", m_uri )
      DE_DEBUG("Opened audio stream ", m_streamIndex )
      DE_DEBUG("Start caching ...")

      fillCache( m_precacheMin );

      m_runFence.notify_all();


      DE_DEBUG("Enter loop.")
      while ( m_isRunning )
      {
         if ( m_writeCount < m_precacheMin )
         {
            fillCache( m_preloadChunk );
         }
         else
         {
            std::this_thread::sleep_for( std::chrono::milliseconds(100) );
         }

//         // MemoryStats
//         if ( totalProcessedBytes >= 256 * 1024 * 1024 )
//         {
//            totalProcessedBytes -= 256 * 1024 * 1024;

//            std::cout << "[FFMPEG] "
//                  "Bytes(" << dbStrByteCount( totalBytes ) << "), "
//                  "Duration(" << dbSecondsToString( totalDuration ) << "), "
//                  "Chunks(" << totalChunks << ")" << std::endl;
//         }
      }

      DE_DEBUG("End loading audio file stream threadId = ", std::this_thread::get_id())
      m_isRunning = false;
   }
};
*/

} // end namespace audio
} // end namespace de

#endif

