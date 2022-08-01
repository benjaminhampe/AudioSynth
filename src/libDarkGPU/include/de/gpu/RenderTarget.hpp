/// ==========================================================================
/// @file FBO.hpp
/// @author 2018 Benjamin Hampe <benjaminhampe@gmx.de>
/// @copyright Free open source software
/// ==========================================================================

#ifndef DE_GPU_RENDERTARGET_HPP
#define DE_GPU_RENDERTARGET_HPP

#include <de/gpu/IVideoDriver.hpp>

namespace de {
namespace gpu {

// ===========================================================================
struct RT
// ===========================================================================
{
   DE_CREATE_LOGGER("de.gpu.RT")
   int w;        // screen width
   int h;       // screen height
   uint32_t fbo;      // GL fbo id
   uint32_t colorTex;
   uint32_t depthTex;
   TexInternalFormat colorFormat;
   TexInternalFormat depthFormat;
   int getWidth() const { return w; }
   int getHeight() const { return h; }
};

bool
createRT( RT & rt, int w, int h, IVideoDriver* driver );

void
destroyRT( RT & rt, IVideoDriver* driver );


} // end namespace gpu.
} // end namespace de.

#endif

