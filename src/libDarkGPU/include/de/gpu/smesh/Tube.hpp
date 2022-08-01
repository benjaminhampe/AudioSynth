#ifndef DE_GPU_SMESH_RING_TUBE_HPP
#define DE_GPU_SMESH_RING_TUBE_HPP

#include <de/gpu/smesh/Ring.hpp>

namespace de {
namespace gpu {
namespace smesh {

// =========================================================================
struct Tube
// =========================================================================
{
   static void
   add( SMeshBuffer & o,
         // Describes outer cylinder
         float dx, // (outer) cylinder width
         float dz, // (outer) cylinder depth
         float hy, // (outer) cylinder height
         // Describes inner cylinder, rx,ry define the extrusion relative inside to dx,dy.
         // They define the inside border width and are not absolute values like dx,dy.
         float rx, // inside going border-width ( inside cylinder width cx = dx - rx )
         float rz, // inside going border-depth ( inside cylinder depth cz = dz - rz )
         uint32_t color = 0xFFFFFFFF, // one color to rule them all (vertices).
         int32_t tessCircle = 13,// tesselation of top and bottom ring = num ring segments
         int32_t tessHeight = 1 )// tesselation of cylinder hull height ( y-direction )
   {
      Ring::addXZ( o, dx, dz, rx, rz, color, color, tessCircle );

   }
};

} // end namespace smesh.
} // end namespace gpu.
} // end namespace de.

#endif

