#include <de/audio/buffer/io/BufferIO_FFMPEG.hpp>
#include <de/FileSystem.hpp>
#include <de/audio/buffer/BufferList.hpp>

#if 1 // v2
   #include <de/audio/decoder/DecoderFFMPEG.hpp>
#else
   #include <de/ffmpeg/Utils.hpp>
   #include <de/ffmpeg/File.hpp>
   //#include <de/ffmpeg/ResamplingFile.hpp>
#endif

namespace de {
namespace audio {
namespace io {

bool
BufferFFMPEG::load( Buffer & dst, std::string uri,
                       int streamIndex, ESampleType sampleType, int sampleRate )
{
   //DE_DEBUG("[LoadMP3] uri = ", uri )

   //if ( uri.empty() ) { DE_ERROR("Empty uri") return false; }
   //dst.setUri( uri );

   PerformanceTimer perf;
   perf.start();

#if 1
   decoder::DecoderFFMPEG file;
   if ( !file.open( uri, streamIndex ) )
   {
      //DE_ERROR("No open file ",uri)
      return false;
   }

   size_t totalChunks = 0;
   size_t totalBytes = 0;
   size_t totalProcessedBytes = 0;
   double totalDuration = 0.0;

   BufferList bufferList; // list of chunks -> join()ed at the end.

   auto handlePayload =
      [&]( decoder::AudioPayload const & payload )
      {
         //DE_INFO("Chunk[",chunks,"].payload->streamIndex = ",payload->streamIndex)
         //DE_INFO("Chunk[",chunks,"].payload->typ = ", ffUtils::toString( payload->typ ) )
         totalProcessedBytes += payload.byteCount();
         totalBytes += payload.byteCount();
         totalDuration += payload.duration();
         totalChunks++;

         Buffer buffer( ST_F32I,
                        payload.channelCount,
                        payload.sampleRate,
                        payload.frameCount );

         memcpy( (void*)buffer.data(), (void*)payload.samples.data(), payload.byteCount() );
         //std::swap( buffer.m_Samples._M_impl._M_start, payload.samples._M_impl._M_start );
         //std::swap( buffer.m_Samples._M_impl._M_start, payload.samples._M_impl._M_start );
         //std::swap( buffer.m_Samples._M_impl._M_start, payload.samples._M_impl._M_start );
         bufferList.append( buffer );
      };

   while ( AVERROR_EOF != file.readFrame( handlePayload ) )
   {
      // while loop body
      if ( totalProcessedBytes >= 256 * 1024 * 1024 )
      {
         totalProcessedBytes -= 256 * 1024 * 1024;

         std::cout << "[FFMPEG] "
               "Bytes(" << dbStrByteCount( totalBytes ) << "), "
               "Duration(" << dbSecondsToString( totalDuration ) << "), "
               "Chunks(" << totalChunks << ")" << std::endl;
      }
   };

   bool ok = bufferList.join( dst );
   perf.stop();

   //DE_DEBUG("[FFMPEG] needed ", perf.ms(), " ms, uri = ", uri )

//   if ( !ok )
//   {
//      DE_ERROR("[FFMPEG] Cant join bufferList = ", bufferList.toString() )
//   }

   //bufferList.clear();
#else
   ffmpeg::File file;
   if ( !file.open( uri, streamIndex ) )
   {
      DE_ERROR("No open file ",uri)
      return false;
   }

   file.setAudioOutFormat( sampleType );
   //file.setOutSampleRate( sampleRate );

   if ( streamIndex < 0 )
   {
      streamIndex = file.getBestAudioStreamIndex();
      //DE_DEBUG("LOAD stream index = ", streamIndex, " of ", file.getStreamCount() )
   }

   if ( streamIndex < 0 )
   {
      DE_ERROR("No audio stream")
      return false;
   }

   file.enableStream( streamIndex ); // enable processing
   auto audioStream = file.getStream( streamIndex );
   if ( !audioStream )
   {
      DE_ERROR("No audio stream")
      return false;
   }

   BufferList bufferList; // list of chunks -> join()ed at the end.
   size_t totalChunks = 0;
   size_t totalBytes = 0;
   size_t totalProcessedBytes = 0;
   double totalDuration = 0.0;

   auto appendBuffer =
      [&]( std::unique_ptr< ffmpeg::FilePayload > payload )
      {
         //DE_INFO("Chunk[",chunks,"].payload->streamIndex = ",payload->streamIndex)
         //DE_INFO("Chunk[",chunks,"].payload->typ = ", ffUtils::toString( payload->typ ) )
         size_t localBytes = 0;
         double localDuration = 0.0;

         if ( payload->streamIndex == streamIndex &&
              payload->typ == AVMEDIA_TYPE_AUDIO &&
              payload->buf )
         {
            if ( totalChunks == 0 )
            {
               DE_INFO("[",totalChunks,"] ",payload->buf->toString())
            }

            localBytes = payload->buf->getByteCount();
            localDuration = payload->buf->getDuration();
            bufferList.append( *payload->buf );
         }
         totalProcessedBytes += localBytes;
         totalBytes += localBytes;
         totalDuration += localDuration;
         totalChunks++;
      };

   while ( AVERROR_EOF != file.readFrame( appendBuffer ) )
   {
      // while loop body
      if ( totalProcessedBytes >= 256 * 1024 * 1024 )
      {
         totalProcessedBytes -= 256 * 1024 * 1024;

         std::cout << "[BufferReaderFFMPEG] "
               "Bytes(" << dbStrByteCount( totalBytes ) << "), "
               "Duration(" << dbSecondsToString( totalDuration ) << "), "
               "Chunks(" << totalChunks << ")" << std::endl;
      }
   };

   perf.stop();
   DE_DEBUG("[ReadMP3] needed ", perf.ms(), " ms, uri = ", uri )

   perf.start();
   bool ok = bufferList.join( dst );
   perf.stop();

   DE_DEBUG("[JoinMP3] needed ", perf.ms(), " ms, uri = ", uri )

   if ( !ok )
   {
      DE_ERROR("[LoadMP3] Cant join bufferList = ", bufferList.toString() )
   }

   // if ( debug )
   // {
      // DE_DEBUG("[Load] needed ", perf.ms(), " ms, uri = ", uri )
   // }
#endif

   return ok;
}


bool
isAudioFormatSupportedByCodec(const AVCodec *codec, enum AVSampleFormat sample_fmt)
{
   AVSampleFormat const* formatList = codec->sample_fmts;
   while (*formatList != AV_SAMPLE_FMT_NONE)
   {
      if (*formatList == sample_fmt) return true;
      formatList++;
   }
   return false;
}

//static int
//chooseSampleRate( AVCodec const* codec )
//{
//   if ( !codec )
//      return 44100;

//   if ( !codec->supported_samplerates )
//      return 44100;

//   int best_samplerate = 0;

//   int const* p = codec->supported_samplerates;
//   while (*p)
//   {
//      if ( !best_samplerate ||
//           abs(44100 - *p) < abs(44100 - best_samplerate))
//      best_samplerate = *p;
//      p++;
//   }
//   return best_samplerate;
//}
struct EncoderTools
{
   DE_CREATE_LOGGER("de.audio.MP3.EncoderTools")

   static int
   encode( AVCodecContext* ctx, AVFrame* frame, AVPacket* pkt, FILE* file )
   {
      // send the frame for encoding
      int ret = avcodec_send_frame(ctx, frame);
      if (ret < 0)
      {
         DE_ERROR("Error sending the frame to the encoder")
         DE_FLUSH
         return ret;
      }

      int loopCounter = 0;

      // read all the available packets (in general there may be any number of them
      while (ret >= 0)
      {
         ret = avcodec_receive_packet( ctx, pkt );
         if ( ret >= 0 )
         {
            DE_DEBUG("Write ", pkt->size, " bytes")
            fwrite( pkt->data, 1, pkt->size, file );
            av_packet_unref( pkt );
         }
         else if (ret == AVERROR(EAGAIN))
         {
            DE_ERROR("AVERROR(EAGAIN)")
            DE_FLUSH
            return ret;
         }
         else if (ret == AVERROR_EOF)
         {
            DE_ERROR("AVERROR_EOF")
            DE_FLUSH
            return ret;
         }
         else // if (ret < 0)
         {
            DE_ERROR("MP3 Error(",ret,") while encoding audio frame")
            DE_FLUSH
            return ret;
         }

         loopCounter++;
      }

      DE_DEBUG("Exit loopCounter = ",loopCounter)
      return ret;
   }
};

bool
BufferFFMPEG::save( Buffer const & src, std::string uri )
{
   DE_DEBUG("uri ",uri)
   DE_FLUSH

   if ( src.getSampleType() != ST_F32I )
   {
      fprintf(stderr, "src.getSampleType() != ST_F32I\n");
      return false; //exit(1);
   }

   DE_DEBUG("MP3 <- ",src.toString())
   DE_FLUSH

   /// find the MP2 encoder
   AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_MP3);
   if ( !codec )
   {
      fprintf(stderr, "AV_CODEC_ID_MP3 not found\n");
   }

   DE_DEBUG("MP3 -> found codec")
   DE_FLUSH

//   codec = avcodec_find_encoder(AV_CODEC_ID_MP3ADU);
//   if ( !codec )
//   {
//      fprintf(stderr, "AV_CODEC_ID_MP3ADU not found\n");
//      return false; //exit(1);
//   }

   AVCodecContext* c = avcodec_alloc_context3( codec );
   if ( !c )
   {
      fprintf(stderr, "Could not allocate audio codec context\n");
      return false; //exit(1);
   }

   DE_DEBUG("MP3 -> created codec context")
   DE_FLUSH

   c->bit_rate = 192000; // 64000;
   c->sample_fmt = AV_SAMPLE_FMT_FLTP;

   if (!isAudioFormatSupportedByCodec(codec, c->sample_fmt))
   {
      fprintf(stderr, "Encoder does not support sample format %s",
      av_get_sample_fmt_name(c->sample_fmt));
      return false; //exit(1);
   }

   DE_DEBUG("MP3 -> supported bit_rate = ", c->bit_rate)
   DE_DEBUG("MP3 -> supported sample_fmt = ", int(c->sample_fmt))
   DE_FLUSH

   // select other audio parameters supported by the encoder
   c->sample_rate    = src.getSampleRate(); // select_sample_rate(codec);
   c->channel_layout = AV_CH_LAYOUT_STEREO; // select_channel_layout(codec);
   c->channels       = av_get_channel_layout_nb_channels(c->channel_layout);

   // open it
   if ( avcodec_open2( c, codec, NULL) < 0 )
   {
      DE_ERROR("Could not open codec")
      return false;
   }

   DE_DEBUG("MP3 -> Codec opened...")
   DE_DEBUG("MP3 -> Codec sample_rate = ", c->sample_rate)
   DE_FLUSH

   FILE* f = fopen( uri.c_str(), "wb");
   if (!f)
   {
      DE_ERROR("Could not open ", uri)
      return false;
   }

   DE_DEBUG("MP3 -> File opened ", uri)
   DE_FLUSH


   // packet for holding encoded output
   AVPacket* pkt = av_packet_alloc();
   if ( !pkt )
   {
      DE_ERROR("Could not allocate the packet ", uri)
      return false;
   }

   // frame containing input raw audio
   AVFrame* frame = av_frame_alloc();
   if ( !frame )
   {
      DE_ERROR("Could not allocate the audio frame ", uri)
      return false;
   }

   DE_DEBUG("MP3 -> Packet allocated")
   DE_DEBUG("MP3 -> Frame allocated")
   DE_FLUSH

   frame->nb_samples     = c->frame_size;
   frame->format         = c->sample_fmt;
   frame->channel_layout = c->channel_layout;

   // allocate the data buffers
   int ret = av_frame_get_buffer(frame, 0);
   if (ret < 0)
   {
      DE_ERROR("Could not allocate audio data buffers")
      return false;
   }

   DE_DEBUG("MP3 -> Frame buffer accessed")
   DE_FLUSH
   // encode a single tone sound
   //float t, tincr;
   //t = 0;
   //tincr = 2 * M_PI * 440.0 / c->sample_rate;

   int64_t chunkIndex = 0;
   int64_t frameIndex = 0;
   int64_t totalFrames = src.getFrameCount();
   while ( 1 )
   {
      // make sure the frame is writable -- makes a copy if the encoder kept a reference internally
      ret = av_frame_make_writable(frame);
      if (ret < 0)
      {
         DE_ERROR("Frame not writable")
         DE_FLUSH
         break;
      }

      if (!frame->data)
      {
         DE_ERROR("!frame->data")
         DE_FLUSH
         break;
      }

      float* dst = (float*)frame->data[0];
      if (!dst)
      {
         DE_ERROR("!dst")
         DE_FLUSH
         break;
      }

      // EndChunk
      if ( frameIndex + c->frame_size >= totalFrames )
      {
         DE_ERROR("EndChunk(",chunkIndex,"): frameIndex(",frameIndex,") + c->frame_size(",int(c->frame_size),") >= totalFrames(",totalFrames,")")
         DE_FLUSH

         for ( uint32_t channel = 0; channel < c->channels; channel++ ) {
         for ( uint32_t frame = 0; frame < c->frame_size; frame++ ) {
            float sample = 0.0f;
            if ( src.getSample( frame, channel, &sample, ST_F32I ) )
            {
               *dst = sample;
            }
            else
            {
               DE_ERROR("No sample frame(",frame,"), channel(",channel,"), frameIndex(",frameIndex,") + c->frame_size(",int(c->frame_size),") >= totalFrames(",totalFrames,")")
               *dst = 0.0f;
            }
            dst++;
         }
         }
         EncoderTools::encode(c, frame, pkt, f);
         break;
      }
      // Start/Normal
      else
      {
         DE_DEBUG("Chunk(",chunkIndex,"): frameIndex(",frameIndex,") + c->frame_size(",int(c->frame_size),") >= totalFrames(",totalFrames,")")

         for ( uint32_t frame = 0; frame < c->frame_size; frame++ ) {
         for ( uint32_t channel = 0; channel < c->channels; channel++ ) {
            float sample = 0.0f;
            if ( src.getSample( frame, channel, &sample, ST_F32I ) )
            {
               *dst = sample;
            }
            else
            {
               DE_ERROR("No sample frame(",frame,"), channel(",channel,"), frameIndex(",frameIndex,") + c->frame_size(",int(c->frame_size),") >= totalFrames(",totalFrames,")")
               *dst = 0.0f;
            }
            dst++;
         }
         }
         EncoderTools::encode(c, frame, pkt, f);
         frameIndex += c->frame_size;
      }

      chunkIndex++;
   }

   DE_DEBUG("MP3 -> Flushing encoder...")
   DE_FLUSH

   // EndChunk, flush the encoder
   EncoderTools::encode( c, NULL, pkt, f);

   DE_DEBUG("MP3 -> Close encoder...")
   DE_FLUSH

   fclose(f);
   av_frame_free(&frame);
   av_packet_free(&pkt);
   avcodec_free_context(&c);


   return true;
}


} // end namespace io
} // end namespace audio
} // end namespace de











