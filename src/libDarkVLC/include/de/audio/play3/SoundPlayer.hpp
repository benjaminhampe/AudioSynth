#ifndef DE_AUDIO_SOUND_PLAYER_HPP
#define DE_AUDIO_SOUND_PLAYER_HPP

#include <de/audio/play3/Sound.hpp>
#include <de/audio/RtUtils.hpp>
#include <thread>
#include <condition_variable>

namespace de {
namespace audio {
namespace play3 {

// =========================================================
struct SoundPlayer
// =========================================================
{
   DE_CREATE_LOGGER("de.audio.play3.SoundPlayer")

   SoundPlayer();
   ~SoundPlayer();

   static SoundPlayer &
   getInstance();

   bool is_playing() const;
   void play();
   void stop();

   int
   getVolume() const;

   void
   setVolume( int vol_in_percent );

   // SoundManager:
   void
   clearSounds();

   int
   addSound( std::string uri, SampleBuffer const & buffer );

   int
   loadSound( std::string uri );

   void
   deleteSound( int soundId );

   void
   playSound( int soundId, int loops = 0 );

   void
   pauseSound( int soundId );

   void
   stopSound( int soundId );

   int
   getSoundVolume( int soundId ) const;
   void
   setSoundVolume( int soundId, int vol_in_percent );


protected:

   void
   run();

   static int
   rt_audio_callback(
      void* outputBuffer,
      void* inputBuffer,
      unsigned int frameCount,
      double pts,
      RtAudioStreamStatus status,
      void* userData );

   //std::thread* m_ThreadPtr;
   std::atomic< bool > m_IsPlaying;
   std::atomic< bool > m_IsPaused;
   int m_Volume;
   //mutable std::condition_variable m_BlockSignalReady;

   std::vector< Sound > m_Sounds;
   //std::mutex m_SoundsMutex;

   RtAudio* m_dac;
   int32_t m_dacSampleRate;
   int32_t m_dacFrames;
   int32_t m_outDevIndex;
   int32_t m_outDevChannels;
   int32_t m_outDevFirstChannel;
//   uint64_t m_FrameIndex;
//   uint64_t m_FrameCount;
   SampleBuffer m_MixBuffer; // float32 accumulation ( sum of all sources )
   SampleBuffer m_SrcBuffer; // float32 buffer for reading single source ( before mix )

};


} // end namespace play3
} // end namespace audio
} // end namespace de

#endif

