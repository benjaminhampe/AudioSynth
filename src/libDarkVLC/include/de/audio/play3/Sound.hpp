#ifndef DE_AUDIO_SOUND_HPP
#define DE_AUDIO_SOUND_HPP

#include <de/audio/IAudioSource.hpp>

namespace de {
namespace audio {
namespace play3 {

// =========================================================
struct Sound : public IAudioSource
// =========================================================
{
   Sound();
   ~Sound() override;

   uint64_t
   readSamples( SampleBuffer & dst, double pts ) override;

   bool
   is_loaded() const override;

   bool
   is_playing() const override;

   bool
   is_paused() const override;

   void
   play() override;

   void
   stop() override;

   void
   pause() override;

   int
   getVolume() const override;

   void
   setVolume( int vol_in_percent ) override;

   int
   getId() const { return m_Id; }

protected:
   DE_CREATE_LOGGER("de.audio.play3.Sound")

public:
   SampleBuffer m_SampleBuffer;
   uint64_t m_FrameIndex;
   std::string m_Uri;
   int m_Id;
   int m_Volume;   // between 0,1
   int m_StereoMode; // 0 - Stereo, 1 - Mono
   bool m_IsLoaded;
   bool m_IsPlaying;
   bool m_IsPaused;
   bool m_IsLooping;

};

} // end namespace play3
} // end namespace audio
} // end namespace de

#endif

