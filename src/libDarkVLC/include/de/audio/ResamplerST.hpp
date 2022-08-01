#ifndef DE_AUDIO_RESAMPLER_SOUNDTOUCH_HPP
#define DE_AUDIO_RESAMPLER_SOUNDTOUCH_HPP

#if 0
#include <de/Logger.hpp>
#include <de/audio/IAudioSource.hpp>

//extern "C" {
#include <SoundTouch/SoundTouch.h>
#include <SoundTouch/BPMDetect.h>

namespace de {
namespace audio {

struct ResamplerST // : public IAudioSource
{
   DE_CREATE_LOGGER("de.audio.ResamplerST")

/*
   uint64_t
   readSamples( SampleBuffer & dst, double pts )
   {
      if ( !m_IsLoaded ) { DE_ERROR("Not loaded") return 0; }
      if ( m_IsPaused ) { DE_ERROR("Paused soundId") return 0; }

      uint64_t n = dst.getFrameCount();
      uint64_t m = m_SampleBuffer.getFrameCount();
      if ( m_FrameIndex + n >= m )
      {
         DE_DEBUG( "m_FrameIndex(",m_FrameIndex,") + n(",n,") >= m(",m,")" )
         m_FrameIndex = 0; // rewind dirty hack
         m_IsPlaying = false;
         m_IsPaused = false;
         //return 0;
      }
      uint64_t k = m_SampleBuffer.read( m_FrameIndex,
                                   dst.getFrameCount(),
                                   dst.getChannelCount(),
                                   dst.data(),
                                   dst.getSampleType() );

      //DE_DEBUG( "k = ",k,", dst.getChannelCount() = ",dst.getChannelCount() )
      m_FrameIndex += k;
      return k * dst.getChannelCount();
   }
*/
   // Processes the sound
   static void
   resample( SampleBuffer const & in, SampleBuffer & out, bool speechConfig = false )
   {
      if ( in.getSampleType() != ST_F32I )
      {
         DE_ERROR("No ST_F32I in")
         //return;
      }

      if ( out.getSampleType() != ST_F32I )
      {
         DE_ERROR("No ST_F32I out")
         //return;
      }

      float rateA = in.getSampleRate();
      float rateB = out.getSampleRate();
      float newRate = rateB / rateA;

      soundtouch::SoundTouch m_SoundTouch;
      m_SoundTouch.setSampleRate( in.getSampleRate() );
      m_SoundTouch.setChannels( out.getChannelCount() );
      m_SoundTouch.setRate( newRate );
      //m_SoundTouch.setTempoChange(params->tempoDelta);
      //m_SoundTouch.setPitchSemiTones(params->pitchDelta);
      m_SoundTouch.setSetting(SETTING_USE_QUICKSEEK, true );
      m_SoundTouch.setSetting(SETTING_USE_AA_FILTER, true );

      if (speechConfig)
      {
         // use settings for speech processing
         m_SoundTouch.setSetting(SETTING_SEQUENCE_MS, 40);
         m_SoundTouch.setSetting(SETTING_SEEKWINDOW_MS, 15);
         m_SoundTouch.setSetting(SETTING_OVERLAP_MS, 8);
         fprintf(stderr, "Tune processing parameters for speech processing.\n");
      }

      if ( in.getSampleType() != ST_F32I )
      {
         DE_DEBUG("Convert to ST_F32I")
         SampleBuffer tmp;
         in.convert( tmp, ST_F32I );
         DE_DEBUG("I. putSamples(",(void*)tmp.data(),"), frameCount(", tmp.getFrameCount(), ")")
         DE_FLUSH
         m_SoundTouch.putSamples( reinterpret_cast< float const* >( tmp.data() ), tmp.getFrameCount() );
      }
      else
      {
         DE_DEBUG("II. putSamples(",(void*)in.data(),"), frameCount(", in.getFrameCount(), ")")
         DE_FLUSH
         m_SoundTouch.putSamples( reinterpret_cast< float const* >( in.data() ), in.getFrameCount() );
      }

      //in.getSampleType(), in.getChannelCount(), in.getSampleRate(),
      //out.resize( uint64_t( double( in.getFrameCount() ) * newRate ) + 1 );

      DE_DEBUG("Resized to ",out.getFrameCount()," frames")
      uint64_t m_FrameIndex = 0;

      SampleBufferList liste;
      // Read ready samples from SoundTouch processor & write them output file.
      // NOTES:
      // - 'receiveSamples' doesn't necessarily return any samples at all
      //   during some rounds!
      // - On the other hand, during some round 'receiveSamples' may have more
      //   ready samples than would fit into 'sampleBuffer', and for this reason
      //   the 'receiveSamples' call is iterated for as many times as it
      //   outputs samples.
      SampleBuffer chunk( ST_F32I, out.getChannelCount(), in.getSampleRate(), 1024 );

      //outFile->write(sampleBuffer, nSamples * nChannels); TODO: Are samples = frames?

      int recvFrames = 0;
      do
      {
         recvFrames = m_SoundTouch.receiveSamples( reinterpret_cast< float* >( chunk.data() ), chunk.getFrameCount() );
         m_FrameIndex += recvFrames;
         if (recvFrames < in.getFrameCount() )
         {
            DE_DEBUG("Before Flush: recvFrames(",recvFrames,") < in.getFrameCount(",chunk.getFrameCount(),")")
            chunk.m_FrameCount = recvFrames;
         }
         liste.append( chunk );
      }
      while (recvFrames != 0);

      // Now the input file is processed, yet 'flush' few last samples that are
      // hiding in the SoundTouch's internal processing pipeline.
      m_SoundTouch.flush();
      do
      {
         recvFrames = m_SoundTouch.receiveSamples( reinterpret_cast< float* >( chunk.data() ), chunk.getFrameCount() );
         m_FrameIndex += recvFrames;
         if (recvFrames < in.getFrameCount() )
         {
            DE_DEBUG("After Flush: recvFrames(",recvFrames,") < in.getFrameCount(",chunk.getFrameCount(),")")
            chunk.m_FrameCount = recvFrames;
         }
         liste.append( chunk );
      }
      while (recvFrames != 0);

      if ( !liste.join( out ) )
      {
         DE_ERROR("No join")
      }
   }

};


} // end namespace audio
} // end namespace de

#endif // 0
#endif










