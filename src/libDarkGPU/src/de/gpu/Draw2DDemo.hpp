#ifndef DE_GPU_DRAW2D_RENDERER_HPP
#define DE_GPU_DRAW2D_RENDERER_HPP

#include <de/gpu/IVideoDriver.hpp>
#include <de/gpu/Primitive2DRenderer.hpp>

namespace de {
namespace gpu {

// ===========================================================================
struct Draw2DDemo
// ===========================================================================
{
   DE_CREATE_LOGGER("de.gpu.Draw2DDemo")

   Draw2DDemo( IVideoDriver* driver = nullptr );
   ~Draw2DDemo();
   void setDriver( IVideoDriver* driver );
   void draw();

   IVideoDriver* m_Driver;

   Primitive2DRenderer m_2DRenderer;
};

} // end namespace gpu.
} // end namespace de.

#endif
