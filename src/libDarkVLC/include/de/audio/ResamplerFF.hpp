#ifndef DE_AUDIO_RESAMPLER_FF_HPP
#define DE_AUDIO_RESAMPLER_FF_HPP

#include <de/Logger.hpp>
#include <de/audio/IAudioSource.hpp>

extern "C" {
   #include <libswresample/swresample.h>
}

namespace de {
namespace audio {

struct ResamplerFF // : public IAudioSource
{
   DE_CREATE_LOGGER("de.audio.ResamplerFF")

   SwrContext* m_swResampler;
   // SwrContext *swr = swr_alloc();
   // av_opt_set_channel_layout(swr, "in_channel_layout",  AV_CH_LAYOUT_5POINT1, 0);
   // av_opt_set_channel_layout(swr, "out_channel_layout", AV_CH_LAYOUT_STEREO,  0);
   // av_opt_set_int(swr, "in_sample_rate",     48000,                0);
   // av_opt_set_int(swr, "out_sample_rate",    44100,                0);
   // av_opt_set_sample_fmt(swr, "in_sample_fmt",  AV_SAMPLE_FMT_FLTP, 0);
   // av_opt_set_sample_fmt(swr, "out_sample_fmt", AV_SAMPLE_FMT_S16,  0);
   int m_srcLayout;
   int m_dstLayout;
   AVSampleFormat m_srcType;
   AVSampleFormat m_dstType;
   int m_srcBPS;
   int m_dstBPS;
   int m_srcRate;
   int m_dstRate;

   ResamplerFF()
   : m_swResampler( nullptr )
   , m_srcLayout( AV_CH_LAYOUT_STEREO )
   , m_dstLayout( AV_CH_LAYOUT_STEREO )
   , m_srcType( AV_SAMPLE_FMT_FLT ) // AV_SAMPLE_FMT_S16;
   , m_dstType( AV_SAMPLE_FMT_FLT )
   , m_srcBPS( 4 )
   , m_dstBPS( 4 )
   , m_srcRate( 44100 )
   , m_dstRate( 44100 )
   {

   }

   ~ResamplerFF()
   {
      if ( m_swResampler )
      {
         swr_free( &m_swResampler );
      }
   }


   // Processes the sound
   uint64_t
   resampleImpl( SampleBuffer const & srcBuffer, SampleBuffer & dstBuffer )
   {
      ESampleType srcType = srcBuffer.getSampleType();
      ESampleType dstType = dstBuffer.getSampleType();
      if ( srcType != ST_F32I )
      {
         DE_ERROR("Invalid srcType ", ST_toString(srcType))
         return 0;
      }

      if ( dstType != ST_F32I )
      {
         DE_ERROR("Set dstType(", ST_toString(dstType),") to dstType(ST_F32I)")
         dstType = ST_F32I;
         dstBuffer.setSampleType( ST_F32I );
      }

      int srcRate = srcBuffer.getSampleRate();
      int dstRate = dstBuffer.getSampleRate();
      m_srcRate = srcRate;
      m_dstRate = dstRate;

      m_swResampler = swr_alloc_set_opts( m_swResampler,
            m_dstLayout, // out_ch_layout
            m_dstType,   // out_sample_fmt
            m_dstRate,   // out_sample_rate
            m_srcLayout, // in_ch_layout
            m_srcType,   // in_sample_fmt
            m_srcRate,   // in_sample_rate
            0, nullptr ); // log_offset + log_ctx

      if ( !m_swResampler )
      {
         DE_ERROR("No m_swResampler")
         return 0;
      }

      //swr_init( m_swResampler );

      int64_t srcFrames = srcBuffer.getFrameCount();
      int64_t swrDelay = swr_get_delay( m_swResampler, srcRate );
      int64_t srcFrameDelay = swrDelay + srcFrames;
      int64_t dstFrames = av_rescale_rnd( srcFrameDelay, dstRate, srcRate, AV_ROUND_UP );

      DE_DEBUG("srcRate(",srcRate,"), srcFrames(",srcFrames,")")
      DE_DEBUG("dstRate(",dstRate,"), dstFrames(",dstFrames,")")
      DE_DEBUG("swrDelay(",swrDelay,")")
      DE_DEBUG("srcFrameDelay(",srcFrameDelay,")")

      dstBuffer.resize( dstFrames );

      uint8_t* dst = dstBuffer.data();
      uint8_t const* src = srcBuffer.data();
      int64_t doneFrames = swr_convert( m_swResampler, &dst,dstFrames, &src,srcFrames );

      if ( doneFrames != dstFrames )
      {
         DE_DEBUG("doneFrames(",doneFrames,") != dstFrames(",dstFrames,"), srcFrames(",srcFrames,")")
      }

      //uint8_t* output;
      //int dstSampleCount = av_rescale_rnd( swr_get_delay( swr, srcRate ) + sampleCount, dstRate, srcRate, AV_ROUND_UP );
      //outputVector.resize(
      //av_samples_alloc( &output, NULL, 2, dstSampleCount, AV_SAMPLE_FMT_S16, 0);
      //dstSampleCount = swr_convert(m_swResampler, &output, dstSampleCount, sampleVector.data(), sampleCount);

      // handle_output(output, dstSampleCount);

      //swr_close( m_swResampler );

      return doneFrames;
   }


   // Processes the sound
   static uint64_t
   resample( SampleBuffer const & srcBuffer, SampleBuffer & dstBuffer )
   {
      ResamplerFF resampler;
      return resampler.resampleImpl( srcBuffer, dstBuffer );
   }

/*
   // Processes the sound
   static void
   resample( SampleBuffer const & srcBuffer, SampleBuffer & dstBuffer )
   {
      int srcRate = srcBuffer.getSampleRate();
      int dstRate = dstBuffer.getSampleRate();
      //float newRate = float( dstRate / srcRate );

      // SwrContext *swr = swr_alloc();
      // av_opt_set_channel_layout(swr, "in_channel_layout",  AV_CH_LAYOUT_5POINT1, 0);
      // av_opt_set_channel_layout(swr, "out_channel_layout", AV_CH_LAYOUT_STEREO,  0);
      // av_opt_set_int(swr, "in_sample_rate",     48000,                0);
      // av_opt_set_int(swr, "out_sample_rate",    44100,                0);
      // av_opt_set_sample_fmt(swr, "in_sample_fmt",  AV_SAMPLE_FMT_FLTP, 0);
      // av_opt_set_sample_fmt(swr, "out_sample_fmt", AV_SAMPLE_FMT_S16,  0);

      int srcLayout = AV_CH_LAYOUT_STEREO; // AV_CH_LAYOUT_5POINT1;
      int dstLayout = AV_CH_LAYOUT_STEREO;
      int srcType = AV_SAMPLE_FMT_FLTP; // AV_SAMPLE_FMT_S16;
      int dstType = AV_SAMPLE_FMT_FLTP;
      int srcBPS = 4; // BytesPerSample
      int dstBPS = 4; // BytesPerSample

      SwrContext* m_swResampler = swr_alloc_set_opts( nullptr, // create mode
         dstLayout, // out_ch_layout
         dstType,   // out_sample_fmt
         dstRate,   // out_sample_rate
         srcLayout, // in_ch_layout
         srcType,   // in_sample_fmt
         srcRate,   // in_sample_rate
         0, nullptr ); // log_offset + log_ctx

      swr_init( );
      //@ref lavu_sampmanip "samples manipulation" API, including av_samples_alloc()
      //swr_get_delay().


      uint8_t* output;
      int dstSampleCount = av_rescale_rnd( swr_get_delay( swr, srcRate ) + sampleCount, dstRate, srcRate, AV_ROUND_UP );
      outputVector.resize(
      av_samples_alloc( &output, NULL, 2, dstSampleCount, AV_SAMPLE_FMT_S16, 0);
      dstSampleCount = swr_convert(m_swResampler, &output, dstSampleCount, sampleVector.data(), sampleCount);

      handle_output(output, dstSampleCount);

      swr_close( m_swResampler );
      swr_free( &m_swResampler );

   }
*/
};


} // end namespace audio
} // end namespace de

#endif










