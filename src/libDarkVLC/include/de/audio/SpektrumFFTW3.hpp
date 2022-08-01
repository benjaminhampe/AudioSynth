#ifndef DE_AUDIO_SAMPLEBUFFER_SPEKTRUM_HPP
#define DE_AUDIO_SAMPLEBUFFER_SPEKTRUM_HPP

#include <de/audio/SampleBuffer.hpp>

namespace de {
namespace audio {

// ===========================================================================
struct SpektrumTools
// ===========================================================================
{
   DE_CREATE_LOGGER("de.audio.SpektrumTools")

   static bool
   draw( SampleBuffer const & src, int channel, Image & dst, Recti pos );
};




} // end namespace audio
} // end namespace de

#endif

