#ifndef DE_GPU_SMESH_DEBUG_HPP
#define DE_GPU_SMESH_DEBUG_HPP

#include <de/gpu/smesh/SMesh.hpp>

namespace de {
namespace gpu {

// =======================================================================
struct VisualDebugData
// =======================================================================
{
   enum EDebugFlag
   {
      NONE = 0, BBOX = 1, BBOXES = 2, NORMALS = 4,
      WIREFRAME = 16, TANGENTS = 32, BITANGENTS = 64,
      ALL = BBOX | BBOXES | NORMALS | WIREFRAME,
      EDebugFlagCount = 7
   };

   uint32_t flags;
   float normalLength;
   uint32_t normalColor;
   uint32_t bboxColor;
   uint32_t bboxesColor;
   float wireframeScale;
   uint32_t wireframeColor;

   VisualDebugData( uint32_t visualFlags = NONE );

   void
   toggle( EDebugFlag flag )
   {
      uint32_t uflag = flag;
      if ( flags & uflag ) { flags &= ~uflag; } // ... disable bit flag,
      else                 { flags |= uflag; }  // ... enable bit flag
   }
   bool
   operator== ( VisualDebugData const & other ) const
   {
      if ( flags != other.flags ) return false;
      if ( normalLength != other.normalLength ) return false;
      if ( normalColor != other.normalColor ) return false;
      if ( bboxColor != other.bboxColor ) return false;
      if ( bboxesColor != other.bboxesColor ) return false;
      if ( wireframeScale != other.wireframeScale ) return false;
      if ( wireframeColor != other.wireframeColor ) return false;
      return true;
   }
};

namespace smesh {

// =======================================================================
struct Debug
// =======================================================================
{
   VisualDebugData debugData;
   SMesh debugMesh;

   void clear();

   void add( SMeshBuffer const & dbgMesh,
               VisualDebugData const & dbgData = VisualDebugData::ALL );

   void add( SMesh const & dbgMesh,
               VisualDebugData const & dbgData = VisualDebugData::ALL  );

   Debug();
};

} // end namespace smesh.
} // end namespace gpu.
} // end namespace de.

#endif

