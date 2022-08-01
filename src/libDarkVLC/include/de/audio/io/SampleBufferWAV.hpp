#ifndef DE_AUDIO_SAMPLEBUFFER_WAV_HPP
#define DE_AUDIO_SAMPLEBUFFER_WAV_HPP

#include <de/audio/SampleBuffer.hpp>

namespace de {
namespace audio {
namespace io {

struct SampleBufferWAV
{
   DE_CREATE_LOGGER("de.audio.SampleBufferWAV")
   static bool
   load( SampleBuffer & dst, std::string uri );

   static bool
   save( SampleBuffer const & src, std::string uri );
};

} // end namespace io
} // end namespace audio
} // end namespace de

#endif










