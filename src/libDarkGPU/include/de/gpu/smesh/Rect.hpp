#ifndef DE_GPU_SMESH_QUAD_HPP
#define DE_GPU_SMESH_QUAD_HPP

#include <de/gpu/smesh/SMesh.hpp>

namespace de {
namespace gpu {
namespace smesh {

// =========================================================================
struct Rect
// =========================================================================
{
   //
   // B---C           Bottom Quad LADK: LDA + LKD
   // |  /|
   // | M |
   // |/  |
   // A---D
   //

   static void
   add( SMeshBuffer & o, S3DVertex a, S3DVertex b, S3DVertex c, S3DVertex d )
   {
      o.addQuad( a, b, c, d );
   }

   //
   // B---C           Bottom Quad LADK: LDA + LKD
   // |  /|
   // | M |
   // |/  |
   // A---D
   //

   static void
   addXY( SMeshBuffer & o, glm::vec2 siz, uint32_t color = 0xFFFFFFFF,
        glm::vec3 pos = glm::vec3(0,0,0),
        float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f )
   {
      float x = pos.x;
      float y = pos.y;
      float z = pos.z;
      float a = 0.5f * siz.x;
      float b = 0.5f * siz.y;
      add( o,
         S3DVertex( x-a, y-b, z, 0,0,-1, color, u1,v2 ),
         S3DVertex( x-a, y+b, z, 0,0,-1, color, u1,v1 ),
         S3DVertex( x+a, y+b, z, 0,0,-1, color, u2,v1 ),
         S3DVertex( x+a, y-b, z, 0,0,-1, color, u2,v2 ) );
   }

   static void
   add( SMeshBuffer & o,
         glm::vec3 const & a,
         glm::vec3 const & b,
         glm::vec3 const & c,
         glm::vec3 const & d,
         uint32_t color = 0xFFFFFFFF,
         float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f )
   {
      auto n = Math::getNormal3D( a,b,c );
      o.addQuad(
         S3DVertex( a.x, a.y, a.z, n.x,n.y,n.z, color, u1,v2 ),
         S3DVertex( b.x, b.y, b.z, n.x,n.y,n.z, color, u1,v1 ),
         S3DVertex( c.x, c.y, c.z, n.x,n.y,n.z, color, u2,v1 ),
         S3DVertex( d.x, d.y, d.z, n.x,n.y,n.z, color, u2,v2 ) );
   }

};

typedef Rect Quad;

} // end namespace smesh.
} // end namespace gpu.
} // end namespace de.

#endif

