#ifndef DE_GPU_SMESH_POLYHULL_HPP
#define DE_GPU_SMESH_POLYHULL_HPP

#include <de/gpu/smesh/Rect.hpp>

namespace de {
namespace gpu {
namespace smesh {

// =========================================================================
struct PolyHull
// =======================================================================
{
   DE_CREATE_LOGGER("de.gpu.smesh.PolyHull")

   static void
   addLeftStripXZ( SMeshBuffer & o, std::vector< glm::vec3 > const & points, float height, uint32_t color )
   {
      if ( points.size() < 2 )
      {
         return;
      }

      auto addHullSegment = [&] ( glm::vec3 a, glm::vec3 b )
      {
         glm::vec3 A = glm::vec3( b.x, b.y - 0.5f * height, b.z );
         glm::vec3 B = glm::vec3( b.x, b.y + 0.5f * height, b.z );
         glm::vec3 C = glm::vec3( a.x, a.y + 0.5f * height, a.z );
         glm::vec3 D = glm::vec3( a.x, a.y - 0.5f * height, a.z );
         Quad::add( o, A,B,C,D, color );
      };

      glm::vec3 a = points[ 0 ];
      for ( size_t i = 1; i < points.size(); ++i )
      {
         glm::vec3 b = points[ i ];
         addHullSegment( a, b );
         a = b;
      }

      addHullSegment( points[ points.size() - 1 ], points[ 0 ] );
   }

   static void
   addLeftStripXY( SMeshBuffer & o, std::vector< glm::vec3 > const & points, float height, uint32_t color )
   {
      if ( points.size() < 2 )
      {
         return;
      }

      auto addHullSegment = [&] ( glm::vec3 a, glm::vec3 b )
      {
         glm::vec3 A = glm::vec3( b.x, b.y, b.z - 0.5f * height );
         glm::vec3 B = glm::vec3( b.x, b.y, b.z + 0.5f * height );
         glm::vec3 C = glm::vec3( a.x, a.y, a.z + 0.5f * height );
         glm::vec3 D = glm::vec3( a.x, a.y, a.z - 0.5f * height );
         Quad::add( o, A,B,C,D, color );
      };

      glm::vec3 a = points[ 0 ];
      for ( size_t i = 1; i < points.size(); ++i )
      {
         glm::vec3 b = points[ i ];
         addHullSegment( a, b );
         a = b;
      }

      addHullSegment( points[ points.size() - 1 ], points[ 0 ] );
   }

   /*
   static void
   addRightStrip( SMeshBuffer & o, std::vector< glm::vec3 > const & points, float height, uint32_t color )
   {
      if ( points.size() < 2 )
      {
         return;
      }

      auto addHullSegment = [&] ( glm::vec3 a, glm::vec3 b )
      {
         glm::vec3 A = glm::vec3( a.x, a.y, a.z - 0.5f * height );
         glm::vec3 B = glm::vec3( a.x, a.y, a.z + 0.5f * height );
         glm::vec3 C = glm::vec3( b.x, b.y, b.z + 0.5f * height );
         glm::vec3 D = glm::vec3( b.x, b.y, b.z - 0.5f * height );
         Quad::add( o, A,B,C,D, color );
      };

      glm::vec3 a = points[ 0 ];
      for ( size_t i = 1; i < points.size(); ++i )
      {
         glm::vec3 b = points[ i ];
         addHullSegment( a, b );
         a = b;
      }

      addHullSegment( points[ points.size() - 1 ], points[ 0 ] );
   }

*/
};

} // end namespace smesh.
} // end namespace gpu.
} // end namespace de.

#endif // DE_SCENEMANAGER_HPP
