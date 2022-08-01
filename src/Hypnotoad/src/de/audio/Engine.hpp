#ifndef DE_AUDIO_ENGINE_IS_ONE_RTAUDIOSTREAM_HPP
#define DE_AUDIO_ENGINE_IS_ONE_RTAUDIOSTREAM_HPP

#include <thread>
#include <atomic>
#include <condition_variable>
#include <RtAudio/RtAudio.hpp>
#include <de/audio/IDspElement.hpp>

namespace de {
namespace audio {

// ===========================================================================
struct Engine
// ===========================================================================
{
protected:
   DE_CREATE_LOGGER("de.audio.Engine")

   std::atomic< bool > m_isPlaying; // std::atomic< bool >
   std::atomic< bool > m_isPaused; // std::atomic< bool >

   RtAudio* m_dac;
   double m_dacStreamTime;

   uint32_t m_outSampleRate;
   uint32_t m_outBufferFrames;
   int32_t m_outDeviceIndex;
   uint32_t m_outChannels;
   uint32_t m_outFirstChannel;

   uint32_t m_inSampleRate;
   uint32_t m_inBufferFrames;
   int32_t m_inDeviceIndex;
   uint32_t m_inChannels;
   uint32_t m_inFirstChannel;

   IDspElement* m_player;  // Single input, a real mixer is implemented as dsp element only
   IDspElement* m_recorder; // Single output, a real mixer is implemented as dsp element only
   std::vector< float > m_playerBuffer;   // Single input buffer?
   std::vector< float > m_recordBuffer;   // Record buffer for microphone
   //std::vector< float > m_mixBuffer;   // Mix buffers

protected:
   // AudioStream
   static int
   rt_audio_callback(
      void* outputBuffer,
      void* inputBuffer,
      unsigned int frameCount,
      double pts,
      RtAudioStreamStatus status,
      void* userData );

public:
   Engine();
   ~Engine();
   //bool isPlaying() const { return m_isPlaying; }
   void setPlayer( IDspElement* player ) { m_player = player; }
   void setRecorder( IDspElement* recorder ) { m_recorder = recorder; }   // Recorder, microphone loopback into a dsp chain

   void playAudio();
   void stopAudio();

};

} // end namespace gpu.
} // end namespace de.

#endif

