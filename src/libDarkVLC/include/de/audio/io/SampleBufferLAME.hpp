#ifndef DE_AUDIO_SAMPLEBUFFER_LAME_HPP
#define DE_AUDIO_SAMPLEBUFFER_LAME_HPP

#include <de/Logger.hpp>
#include <de/audio/SampleBuffer.hpp>

namespace de {
namespace audio {
namespace io {

struct SB_SaveOptions
{
   bool debug;

   SB_SaveOptions()
      : debug(false)
   {}
};

struct SampleBufferLAME
{
   DE_CREATE_LOGGER("de.audio.SampleBufferLAME")

   static bool
   save( SampleBuffer const & dst, std::string uri, SB_SaveOptions const & options = SB_SaveOptions() );
};

} // end namespace io
} // end namespace audio
} // end namespace de

#endif










