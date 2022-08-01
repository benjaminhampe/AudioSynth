#ifndef DE_AUDIO_DSP_MIXER_STREAM_HPP
#define DE_AUDIO_DSP_MIXER_STREAM_HPP

#include <de/audio/dsp/IDspChainElement.hpp>

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
   bool isPlaying() const;

   void play();
   void stop();

protected:
   DE_CREATE_LOGGER("de.audio.MixerStream")
   void* m_impl;
};

} // end namespace gpu.
} // end namespace de.

#endif

