#ifndef DE_GPU_SMESH_LINE_HPP
#define DE_GPU_SMESH_LINE_HPP

#include <de/gpu/smesh/SMesh.hpp>

namespace de {
namespace gpu {
namespace smesh {

// =========================================================================
struct Line
// =========================================================================
{
   //     B
   //    /
   //   /
   //  /
   // A
   //

   static void
   add( SMeshBuffer & o, S3DVertex a, S3DVertex b )
   {
      o.addVertex( a );
      o.addVertex( b );
   }

   static void
   add( SMeshBuffer & o, glm::vec3 a, glm::vec3 b, uint32_t color )
   {
      o.addVertex( S3DVertex( a, glm::vec3(0,0,1), color, glm::vec2(0,0) ) );
      o.addVertex( S3DVertex( b, glm::vec3(0,0,1), color, glm::vec2(1,0) ) );
   }

   static void
   add( SMeshBuffer & o, glm::vec3 a, glm::vec3 b, uint32_t colorA, uint32_t colorB )
   {
      o.addVertex( S3DVertex( a, glm::vec3(0,0,1), colorA, glm::vec2(0,0) ) );
      o.addVertex( S3DVertex( b, glm::vec3(0,0,1), colorB, glm::vec2(1,0) ) );
   }

};


// =========================================================================
struct Triangle
// =========================================================================
{
   //     B
   //    /|
   //   / |   ABC cw inputted
   //  /--|
   // A   C
   //

   static void
   add(
      SMeshBuffer & o,
      glm::vec3 a,
      glm::vec3 b,
      glm::vec3 c,
      uint32_t colorA,
      uint32_t colorB,
      uint32_t colorC,
      glm::vec2 uvA = glm::vec2(0,0),
      glm::vec2 uvB = glm::vec2(1,0),
      glm::vec2 uvC = glm::vec2(1,1) )
   {
      auto n = Math::getNormal3D( a,b,c );
      S3DVertex A( a, n, colorA, uvA );
      S3DVertex B( b, n, colorB, uvB );
      S3DVertex C( c, n, colorC, uvC );
      o.addTriangle( A,B,C );
   }

   static void
   add( SMeshBuffer & o, glm::vec3 a, glm::vec3 b, glm::vec3 c, uint32_t color = 0xFFFFFFFF,
                     glm::vec2 uvA = glm::vec2(0,0),
                     glm::vec2 uvB = glm::vec2(1,0),
                     glm::vec2 uvC = glm::vec2(1,1) )
   {
      add( o, a,b,c, color, color, color, uvA, uvB, uvC );
   }
};

} // end namespace smesh.
} // end namespace gpu.
} // end namespace de.

#endif

