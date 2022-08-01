#include <de/ffmpeg/FileStream.hpp>

namespace de {
namespace ffmpeg {

std::string
FileStream::getDetailName() const
{
   if ( !m_avCodec ) { return "Unknown"; }
   std::stringstream s;

   if ( getMediaType() == AVMEDIA_TYPE_AUDIO )
   {
      s << "ch:" << m_avCodecParams->channels << ", ";
      s << "fmt:" << Utils::toString( m_avCodecCtx->sample_fmt ) << ", ";
   }
   else if ( getMediaType() == AVMEDIA_TYPE_VIDEO )
   {
      s << "w:" << m_avCodecParams->width << ", ";
      s << "h:" << m_avCodecParams->height << ", ";
   }

   s << m_avCodec->long_name << ", ";
   s << "CodecId(" << int( m_avCodecParams->codec_id ) << ")";
   return s.str();
}

// Owned and used only by the File, so its actually a FileStream
FileStream::FileStream( AVFormatContext* avFormatCtx, int avStreamIndex )
   : m_avFormatCtx( avFormatCtx )
   , m_avStream( nullptr )
   , m_avCodecParams( nullptr )
   , m_avCodec( nullptr )
   , m_avCodecCtx( nullptr )
   , m_swResampler( nullptr )
   , m_swScaler( nullptr )
   , m_IsEnabled( false )
   , m_IsSeekable( false )
   , m_avStreamIndex( avStreamIndex )
   , m_Position( 0.0 )
   , m_Duration( 0.0 )
{
   if ( m_avFormatCtx )
   {
      m_avStream = m_avFormatCtx->streams[ m_avStreamIndex ];
      if ( m_avStream )
      {
         if ( m_avStream->duration == AV_NOPTS_VALUE )
         {
            m_avTimeBase.num = 0;
            m_avTimeBase.den = 1;
            m_avDuration = 0;
            m_IsSeekable = false;
            m_Duration = 0.0;
         }
         else
         {
            m_avTimeBase = m_avStream->time_base;
            m_avDuration = m_avStream->duration;
            m_IsSeekable = true;
            m_Duration = Utils::toDouble( m_avTimeBase ) * m_Duration;
         }

         m_avCodecParams = m_avStream->codecpar;
         m_avCodecId = m_avCodecParams->codec_id;
         m_avCodec = avcodec_find_decoder( m_avCodecParams->codec_id );
         if ( m_avCodec )
         {
            m_avCodecCtx = avcodec_alloc_context3( m_avCodec );
            if ( m_avCodecCtx )
            {
               int e = avcodec_parameters_to_context( m_avCodecCtx, m_avCodecParams );
               if (e < 0)
               {
                  DE_ERROR("No avcodec_parameters_to_context, e = ", e)
               }

               // int e = avcodec_parameters_to_context( m_avCodecCtx, m_avCodecParams );
               // Explicitly request interleaved ( non planar ) data.
               if ( m_avCodecCtx->codec_type == AVMEDIA_TYPE_AUDIO )
               {
                  AVSampleFormat fmt = m_avCodecCtx->sample_fmt;
                  m_avCodecCtx->request_sample_fmt = av_get_alt_sample_fmt( fmt, 0 ); // 0 = interleaved, 1 = planar
               }

               e = avcodec_open2( m_avCodecCtx, m_avCodec, nullptr );
               if (e >= 0)
               {

                  m_Duration = Utils::getDuration( m_avFormatCtx, m_avStreamIndex );

               }
               else { DE_ERROR("No avcodec_open2()") }
            }
            else { DE_ERROR("No avcodec_alloc_context3()") }
         }
         else { DE_ERROR("No avcodec_find_decoder(), streamIndex(",avStreamIndex,")") }
      }
      else { DE_ERROR("No m_avStream") }
   }
   else { DE_ERROR("No m_avFormatCtx") }
}

FileStream::~FileStream()
{
   //drainDecoder(m_avCodecCtx, m_avFrame);
   avcodec_close( m_avCodecCtx );
   avcodec_free_context( &m_avCodecCtx );
   swr_free( &m_swResampler );
   sws_freeContext( m_swScaler );
}


bool
FileStream::isSeekable() const { return m_IsSeekable; }

bool
FileStream::seek( double pts )
{
   if ( m_IsEnabled )
   {
      m_Position = pts;
      return Utils::seek( m_avFormatCtx, m_avStreamIndex, pts );
   }
   else
   {
      return false;
   }
}

double
FileStream::tell() const
{
   return m_Position;
}

//bool
//FileStream::hasDuration() const { return m_HasDuration; }
//double
//FileStream::getDuration() const { return m_Duration; }
//double
//FileStream::getPosition() const { return m_Position; }

//bool
//FileStream::setPosition( double pts )
//{
//   return seek( pts );
//}

} // end namespace ffmpeg
} // end namespace de

