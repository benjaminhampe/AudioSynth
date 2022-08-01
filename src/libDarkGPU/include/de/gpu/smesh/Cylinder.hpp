#ifndef DE_GPU_SMESH_CYLINDER_HPP
#define DE_GPU_SMESH_CYLINDER_HPP

#include <de/gpu/smesh/Circle.hpp>
#include <de/gpu/smesh/PolyHull.hpp>

namespace de {
namespace gpu {
namespace smesh {

// =========================================================================
struct CylinderHull
// =========================================================================
{
   static void
   addXY( SMeshBuffer & o,
        float dx, float dy, float h,
        uint32_t color = 0xFFFFFFFF, int tessCirc = 33 )
   {
      std::vector< glm::vec3 > points;
      points.reserve( tessCirc + 1 );
      float angleStart = 0.0f;     // from 0 degrees
      float angleEnd = float( 2.0 * M_PI ); // to 90 degrees
      float angleStep = ( angleEnd - angleStart) / float( tessCirc );
      float a = 0.5f * dx;
      float b = 0.5f * dy;

      for ( int i = 0; i <= tessCirc; ++i )
      {
         float phi = angleStart + angleStep * float( i );
         float ps = ::sinf( phi );
         float pc = ::cosf( phi );
         points.emplace_back( a * ps, b * pc, 0.0f );
      }

      PolyHull::addLeftStripXY( o, points, h, color );
   }

   static void
   addXZ( SMeshBuffer & o,
        float dx, float h, float dz,
        uint32_t color = 0xFFFFFFFF,int tessCirc = 33 )
   {
      std::vector< glm::vec3 > points;
      points.reserve( tessCirc + 1 );
      float angleStart = 0.0f;     // from 0 degrees
      float angleEnd = float( 2.0 * M_PI ); // to 90 degrees
      float angleStep = ( angleEnd - angleStart) / float( tessCirc );
      float a = 0.5f * dx;
      float b = 0.5f * dz;

      for ( int i = 0; i <= tessCirc; ++i )
      {
         float phi = angleStart + angleStep * float( i );
         float ps = ::sinf( phi );
         float pc = ::cosf( phi );
         points.emplace_back( a * ps, 0.0f, b * pc );
      }

      PolyHull::addLeftStripXZ( o, points, h, color );
   }
};

// =========================================================================
struct Cylinder
// =========================================================================
{
   static void
   addXY( SMeshBuffer & o,
          float dx, float dy, float h,
          uint32_t color = 0xFFFFFFFF,
          int tessCirc = 33 )
   {
      glm::vec3 circleOffset( 0, 0, 0.5f * h );
      glm::vec2 circleSize( dx, dy );
      uint32_t vStart = o.getVertexCount();
      Circle::addXY( o, circleSize, color, tessCirc );
      SMeshBufferTool::translateVertices( o, circleOffset, vStart );
      vStart = o.getVertexCount();
      CylinderHull::addXY( o, dx,dy,h, color, tessCirc );
   }

   static void
   addXZ( SMeshBuffer & o,
          float dx, float h, float dz,
          uint32_t color = 0xFFFFFFFF,
          int tessCirc = 33 )
   {
      glm::vec3 circleOffset( 0, 0.5f * h, 0 );
      glm::vec2 circleSize( dx, dz );
      uint32_t vStart = o.getVertexCount();
      Circle::addXZ( o, circleSize, color, tessCirc );
      SMeshBufferTool::translateVertices( o, circleOffset, vStart );
      vStart = o.getVertexCount();
      CylinderHull::addXZ( o, dx,h,dz, color, tessCirc );
   }

};

} // end namespace smesh.
} // end namespace gpu.
} // end namespace de.

#endif

