#ifndef DE_MEDIA_FILE_FFMPEG_HPP
#define DE_MEDIA_FILE_FFMPEG_HPP

#include <de/ffmpeg/FileStream.hpp>
// #include <de/audio/IBufferFiller.hpp>
// #include <de/audio/BufferQueue.hpp>
// #include <de/ffmpeg/IImageFiller.hpp>
// #include <de/ffmpeg/ImageQueue.hpp>

namespace de {
namespace ffmpeg {

// ===========================================================================
struct File //: public audio::IBufferFiller, public IImageFiller
// ===========================================================================
{
   DE_CREATE_LOGGER("de.ffmpeg.File")
   std::string m_Uri;
   std::vector< FileStream* > m_Streams;
   bool m_IsOpen;
   bool m_IsDebug;
   bool m_IsSeekable;
   double m_Position;
   double m_Duration;

   AVFormatContext* m_avFormatCtx;
   AVPacket* m_avPacket;
   AVFrame* m_avFrame;
//   double m_VideoPTS; // ?
//   double m_AudioPTS; // ?

   audio::ESampleType m_AudioOutputType; // output format of audio decoder

//   double m_AudioFrameRate = 0.0;
//   uint32_t m_AudioQueueThresholdLoadStart;
//   uint32_t m_AudioQueueThresholdLoadStop;
//   audio::BufferQueue m_AudioQueue;
//   ImageQueue m_ImageQueue;
//   Image m_CoverArt;
//   std::thread* m_ThreadPtr;
//   mutable std::mutex m_Mutex;

//   double m_PrecacheDuration; // 300ms

   File();
   ~File();

//   uint64_t fillAudioBuffer( audio::Buffer & dst, double pts ) override;
//   bool fillImageBuffer( Image & dst, double pts ) override;

   typedef std::function< void(std::unique_ptr< FilePayload >&&) > ON_GET_DATA;

   std::string getUri() const { return m_Uri; }
   bool is_open() const;
   bool open( std::string uri, bool debug = false );
   void close();
   int32_t readFrame( ON_GET_DATA const & onGetData );

   int32_t getBestAudioStreamIndex() const { return Utils::findBestAudio( m_avFormatCtx ); }
   int32_t getBestVideoStreamIndex() const { return Utils::findBestVideo( m_avFormatCtx ); }

   bool isStream( int i ) const;
   uint32_t getStreamCount() const;
   FileStream* getStream( int i );
   FileStream const* getStream( int i ) const;

   void enableStream( int i );
   void disableStream( int i );
   void enableStreams();
   void disableStreams();

   bool isSeekable() const;

   void seek( double pts );
   double tell() const;

//   double getPosition() const;
//   void setPosition( double positionInSeconds );

   //bool hasDuration() const;
//   double getDuration() const;

//   bool getCoverArt( Image & img );


//   uint32_t getAudioStreamCount() const;
//   uint32_t getVideoStreamCount() const;
//   FileStream* getVideoStream( int i ) const;
//   FileStream* getAudioStream( int i ) const;

//   int selectedAudioStreamIndex() const { return m_AudioStreamIndex; }
//   int selectedVideoStreamIndex() const { return m_VideoStreamIndex; }

   void setAudioOutFormat( audio::ESampleType sampleType = audio::ST_Unknown );

//   void selectAudioStream( int i )
//   {
//      FileStream* stream = getStream( i );
//      if ( !stream ) { DE_ERROR("No stream i = ",i) return; }
//      if ( !stream->isAudio() ){ DE_ERROR("No audio stream i = ",i) return; }
//      stream->enable();
//      m_AudioStreamIndex = i;
//   }

//   void selectVideoStream( int i )
//   {
//      FileStream* stream = getStream( i );
//      if ( !stream ) { DE_ERROR("No stream i = ",i) return; }
//      if ( !stream->isVideo() ){ DE_ERROR("No video stream i = ",i) return; }
//      stream->enable();
//      m_VideoStreamIndex = i;
//   }

//   void fillCache();

//protected:
//   void fillCacheUnguarded();



};

} // end namespace ffmpeg
} // end namespace de


#endif
