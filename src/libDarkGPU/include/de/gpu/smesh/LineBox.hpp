#ifndef DE_GPU_SMESH_LINEBOX_HPP
#define DE_GPU_SMESH_LINEBOX_HPP

#include <de/gpu/smesh/Line.hpp>

namespace de {
namespace gpu {
namespace smesh {

// =========================================================================
struct LineBox
// =========================================================================
{
   static void
   add( SMeshBuffer & o, Box3f const & box, uint32_t color = 0xFFFFFFFF )
   {
      Line::add( o, box.getA(), box.getB(), color ); // AB
      Line::add( o, box.getB(), box.getC(), color ); // BC
      Line::add( o, box.getC(), box.getD(), color ); // CD
      Line::add( o, box.getD(), box.getA(), color ); // DA

      Line::add( o, box.getA(), box.getE(), color ); // AE
      Line::add( o, box.getB(), box.getF(), color ); // BF
      Line::add( o, box.getC(), box.getG(), color ); // CG
      Line::add( o, box.getD(), box.getH(), color ); // DH

      Line::add( o, box.getE(), box.getF(), color ); // EF
      Line::add( o, box.getF(), box.getG(), color ); // FG
      Line::add( o, box.getG(), box.getH(), color ); // GH
      Line::add( o, box.getH(), box.getE(), color ); // HE
   }
};

} // end namespace smesh.
} // end namespace gpu.
} // end namespace de.

#endif

