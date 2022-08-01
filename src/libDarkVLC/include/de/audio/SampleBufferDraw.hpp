#ifndef DE_AUDIO_SAMPLEBUFFER_DRAW_TO_IMAGE_HPP
#define DE_AUDIO_SAMPLEBUFFER_DRAW_TO_IMAGE_HPP

#include <de/audio/SampleBuffer.hpp>
#include <vector>
#include <list>
#include <deque>
#include <mutex>
#include <atomic>

namespace de {
namespace audio {

// Draw de.audio.SampleBuffer to de.Image.

// ===========================================================================
struct SampleBufferDraw
// ===========================================================================
{
   DE_CREATE_LOGGER("de.audio.SampleBufferDraw")

   static bool
   saveImage( SampleBuffer const & src, std::string uri, int w, int h, uint32_t color = 0xFFFFFFFF );

   static Image
   createImage( SampleBuffer const & src, int w, int h, uint32_t color = 0xFFFFFFFF );

   static void
   drawChannel( SampleBuffer const & src, uint32_t channel,
                Image & img, Recti pos, uint32_t color = 0xFFFFFFFF );

   static void
   draw( SampleBuffer const & src, Image & img, Recti pos, uint32_t color = 0xFFFFFFFF );


   // bool saveWAV_noperf( std::string uri, ESampleType dstType = ST_S16I ) const;
   // bool saveWAV( std::string uri, ESampleType dstType = ST_S16I ) const;
   // bool loadWAV_noperf( std::string uri, ESampleType dstType = ST_S16I ) const;
   // bool loadWAV( std::string uri, ESampleType dstType = ST_S16I ) const;
};


} // end namespace audio
} // end namespace de

#endif

