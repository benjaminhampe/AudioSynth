#ifndef DE_GPU_PERFOVERLAY_RENDERER_HPP
#define DE_GPU_PERFOVERLAY_RENDERER_HPP

#include <de/gpu/IVideoDriver.hpp>

namespace de {
namespace gpu {

// ===========================================================================
struct PerfOverlay
// ===========================================================================
{
   DE_CREATE_LOGGER("de.gpu.PerfOverlay")

   PerfOverlay( IVideoDriver* driver );
   ~PerfOverlay() {}
   void draw2DPerfOverlay5x8();
   IVideoDriver* m_Driver;
};

} // end namespace gpu.
} // end namespace de.

#endif
