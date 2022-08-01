#if 0

#ifndef DE_VIDEO_DECODER_FFMPEG_HPP
#define DE_VIDEO_DECODER_FFMPEG_HPP

#include <cstdint>
#include <cstdlib>
#include <vector>
#include <functional>
#include <thread>
#include <mutex>
#include <de/Logger.hpp>


#ifdef __cplusplus
extern "C"
{
#endif

   #include <libavcodec/avcodec.h>
   #include <libavformat/avformat.h>
   #include <libavutil/avutil.h>
   #include <libavutil/opt.h>
   #include <libavutil/channel_layout.h>
   #include <libavutil/common.h>
   #include <libavutil/frame.h>
   #include <libavutil/imgutils.h>
   #include <libavutil/mathematics.h>
   #include <libavutil/samplefmt.h>
   #include <libavutil/mem.h>
   #include <libswscale/swscale.h>
   #include <libswresample/swresample.h>
   #include <inttypes.h>

#ifdef __cplusplus
}
#endif


namespace de {
namespace video {
namespace decoder {

// Payload is always float 32 bit
// Payload is always interleaved channels
struct VideoPayload
{
   double pts;
   uint64_t frameCount = 0;
   uint32_t channelCount = 0;
   uint32_t sampleRate = 0;
   std::vector< float > samples;

   uint64_t byteCount() const { return samples.size() * sizeof(float); }

   double duration() const {
      if ( sampleRate < 1 ) return 0.0;
      return double( frameCount ) / double( sampleRate );
   }

   std::string toString() const
   {
      std::stringstream s;
      s << ", pts:" << pts
      << "cc:" << channelCount
         << ", fc:" << frameCount
         << ", sr:" << sampleRate
         << ", samples:" << samples.size();
      return s.str();
   }
};

typedef std::function< void(VideoPayload const &) > ON_VIDEO_PAYLOAD;


// ===========================================================================
struct VideoDecoderCtx
// ===========================================================================
{
   AVFormatContext* formatCtx;
   AVPacket* packet;
   AVFrame* frame;
   bool isOpen;
   bool isDebug;
   bool isSeekable;
   bool hasFrameCount;
   bool hasDuration;
   std::string uri;

   int streamIndex;
   int64_t durationTicks;
   AVRational timeBase;
   AVStream* stream;
   AVCodecParameters* codecParams;
   AVCodecID codecId;
   AVCodec* codec;
   AVCodecContext* codecCtx;
   double position;
   double duration;

   uint32_t channelCount;
   uint32_t sampleRate;
   uint64_t frameCount;    // Can be 0 for bad files

   SwrContext* swResampler;
};

// ===========================================================================
struct VideoDecoderTools
// ===========================================================================
{
   DE_CREATE_LOGGER("VideoDecoderTools")

   static double
   toDouble( AVRational const & r );

   // Extract a single sample and convert to float.
   static float
   getSample( AVSampleFormat const & fmt, uint8_t const* src, uint32_t srcBPS );

   static std::string
   getErrorStr( int errnum );

   static bool
   isInterleaved( AVSampleFormat fmt );

   static void
   initDecoder( VideoDecoderCtx & ds );

   static void
   closeDecoder( VideoDecoderCtx & ds );

   static bool
   openDecoder( VideoDecoderCtx & ds, std::string uri, int streamIndex = -1, bool debug = false );

   static uint64_t
   extractAudio( AVFrame* avFrame, VideoPayload & payload, bool debug = false );

   static int32_t
   readFrame( VideoDecoderCtx & ds, ON_VIDEO_PAYLOAD const & onPayload );
};

// ===========================================================================
struct VideoDecoderFFMPEG
// ===========================================================================
{
   DE_CREATE_LOGGER("de.audio.VideoDecoderFFMPEG")
   VideoDecoderCtx m_data;

   VideoDecoderFFMPEG() { VideoDecoderTools::initDecoder( m_data ); }
   ~VideoDecoderFFMPEG() { close(); }

   bool
   open( std::string uri, int streamIndex = -1, bool debug = false )
   {
      return VideoDecoderTools::openDecoder( m_data, uri, streamIndex, debug );
   }

   void
   close() { VideoDecoderTools::closeDecoder( m_data ); }

   bool
   is_open() const { return m_data.isOpen; }

   bool
   is_seekable() const { return m_data.isSeekable; }

   bool
   hasDuration() const { return m_data.hasDuration; }
   double
   getDuration() const { return m_data.hasDuration; }

   std::string
   getUri() const { return m_data.uri; }

   int32_t
   readFrame( ON_VIDEO_PAYLOAD const & handlePayload )
   {
      return VideoDecoderTools::readFrame( m_data, handlePayload );
   }

//   double getPosition() const;
//   void setPosition( double positionInSeconds );

//   bool getCoverArt( Image & img );
//   void fillCache();

//protected:
//   void fillCacheUnguarded();
};

} // end namespace decoder
} // end namespace audio
} // end namespace de


#endif

#endif // 0
