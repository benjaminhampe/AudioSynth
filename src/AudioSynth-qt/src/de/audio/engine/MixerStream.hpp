#ifndef DE_AUDIO_DSP_MIXER_STREAM_HPP
#define DE_AUDIO_DSP_MIXER_STREAM_HPP

#include <de/audio/dsp/IDspChainElement.hpp>
#include <de/audio/engine/RtUtils.hpp>
#include <thread>
#include <atomic>
#include <condition_variable>

namespace de {
namespace audio {

// ===========================================================================
struct MixerStream
// ===========================================================================
{
   MixerStream();
   ~MixerStream();

   void clearInputSignals();
   void setInputSignal( int i, IDspChainElement* input );

   int getVolume() const;
   void setVolume( int vol_in_percent );
   bool isPlaying() const { return m_IsPlaying; }

   void play();
   void stop();

protected:
   DE_CREATE_LOGGER("de.audio.MixerStream")

   // AudioStream
   std::atomic< bool > m_IsPlaying; // std::atomic< bool >
   std::atomic< bool > m_IsPaused; // std::atomic< bool >
   int m_Volume;
   RtAudio* m_dac;
   uint32_t m_dacSampleRate;
   uint32_t m_dacFrames;
   double m_dacStreamTime;
   int32_t m_outDevIndex;
   uint32_t m_outDevChannels;
   uint32_t m_outDevFirstChannel;

   // DSP inputs
   IDspChainElement* m_inputSignal;

   // AudioStream
   static int
   rt_audio_callback(
      void* outputBuffer,
      void* inputBuffer,
      unsigned int frameCount,
      double pts,
      RtAudioStreamStatus status,
      void* userData );
};

} // end namespace gpu.
} // end namespace de.

#endif

