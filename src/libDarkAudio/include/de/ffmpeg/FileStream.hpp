#ifndef DE_MEDIA_FILESTREAM_FFMPEG_HPP
#define DE_MEDIA_FILESTREAM_FFMPEG_HPP

#include <de/ffmpeg/FilePayload.hpp>

namespace de {
namespace ffmpeg {

// ============================================================
struct FileStream
// ============================================================
{
   FileStream( AVFormatContext* avFormatCtx, int avStreamIndex );
   ~FileStream();
   int32_t getMediaType() const { return m_avCodecParams ? int32_t( m_avCodecParams->codec_type ) : int32_t( AVMEDIA_TYPE_UNKNOWN ); }
   bool isAudio() const { return getMediaType() == AVMEDIA_TYPE_AUDIO; }
   bool isVideo() const { return getMediaType() == AVMEDIA_TYPE_VIDEO; }
   bool isEnabled() const { return m_IsEnabled; }
   void enable() { m_IsEnabled = true; }
   void disable() { m_IsEnabled = false; }
   void toggleEnabled() { m_IsEnabled = !m_IsEnabled; }

   bool isSeekable() const;
   bool seek( double pts );
   double tell() const;

   double getDuration() const { return m_Duration; }
   //double getPosition() const;
   //bool setPosition( double pts );

   std::string getDetailName() const;

   DE_CREATE_LOGGER("de.FileStream")
   AVFormatContext* m_avFormatCtx;
   AVStream* m_avStream;
   AVRational m_avTimeBase;
   int64_t m_avDuration;
   AVCodecParameters* m_avCodecParams;
   AVCodecID m_avCodecId;
   AVCodec* m_avCodec;
   AVCodecContext* m_avCodecCtx;
   SwrContext* m_swResampler;
   SwsContext* m_swScaler;
   bool m_IsEnabled;
   bool m_IsSeekable;
   //bool m_HasDuration;
   int m_avStreamIndex;
   double m_Position;
   double m_Duration;
};

} // end namespace ffmpeg
} // end namespace de


#endif
