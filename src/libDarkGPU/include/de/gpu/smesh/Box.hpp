#ifndef DE_GPU_SMESH_BOX_HPP
#define DE_GPU_SMESH_BOX_HPP

#include <de/gpu/smesh/SMesh.hpp>

namespace de {
namespace gpu {
namespace smesh {


// =========================================================================
/// @brief CADCube = UnitCube * scale(x,y,z)
// =========================================================================
//
//        F-------G                 tex atlas infos         +----+----+---------+
//       /|      /|                                         |    |    |         |
//      / |  +Y / |                                         |    | +Z |         |
//     /  |    /  |                                         |    |    |         |
//    /   E---/---H   lookat = -X for plane PX              +----+----+----+----+
//   /-X /   / X /    <-------P  EyePos is +X for plane PX  |    |    |    |    |
//  /   /   / + /                                           | -X | -Y | +X | +Y |
// B-------C   /     +z                                     |    |    |    |    |
// |  /    |  /      |  +y                                  +----+----+----+----+
// | / -Y  | /       | /                                    |    |    |         |
// |/      |/        |/                                     |    | -Z |         |
// A-------D         0------> +x                            |    |    |         |
//
// =========================================================================
struct Box
// =========================================================================
{
   static void
   add( SMeshBuffer & o,
         glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d,
         glm::vec3 e, glm::vec3 f, glm::vec3 g, glm::vec3 h,
         uint32_t color = 0xFFFFFFFF )
   {
      { // Front quad ABCD + Back quad HGFE
      S3DVertex A( a.x, a.y, a.z, 0,0,-1, color, 0.0,1.0 );
      S3DVertex B( b.x, b.y, b.z, 0,0,-1, color, 0.0,0.0 );
      S3DVertex C( c.x, c.y, c.z, 0,0,-1, color, 1.0,0.0 );
      S3DVertex D( d.x, d.y, d.z, 0,0,-1, color, 1.0,1.0 );
      o.addQuad( A,B,C,D );
      S3DVertex E( e.x, e.y, e.z, 0,0,1, color, 0.0,1.0 );
      S3DVertex F( f.x, f.y, f.z, 0,0,1, color, 0.0,0.0 );
      S3DVertex G( g.x, g.y, g.z, 0,0,1, color, 1.0,0.0 );
      S3DVertex H( h.x, h.y, h.z, 0,0,1, color, 1.0,1.0 );
      o.addQuad( H,G,F,E );
      }

      { // Wall R+L quad DCGH + EFBA
      S3DVertex D( d.x, d.y, d.z, 0,0,1, color, 0.0,1.0 );
      S3DVertex C( c.x, c.y, c.z, 0,0,1, color, 0.0,0.0 );
      S3DVertex G( g.x, g.y, g.z, 0,0,1, color, 1.0,0.0 );
      S3DVertex H( h.x, h.y, h.z, 0,0,1, color, 1.0,1.0 );
      o.addQuad( D,C,G,H );
      S3DVertex E( e.x, e.y, e.z, 0,0,-1, color, 0.0,1.0 );
      S3DVertex F( f.x, f.y, f.z, 0,0,-1, color, 0.0,0.0 );
      S3DVertex B( b.x, b.y, b.z, 0,0,-1, color, 1.0,0.0 );
      S3DVertex A( a.x, a.y, a.z, 0,0,-1, color, 1.0,1.0 );
      o.addQuad( E,F,B,A );
      }

      { // Top quad BFGC + Bottom quad AEHD
      S3DVertex B( b.x, b.y, b.z, 0,1,0, color, 0.0,1.0 );
      S3DVertex F( f.x, f.y, f.z, 0,1,0, color, 0.0,0.0 );
      S3DVertex G( g.x, g.y, g.z, 0,1,0, color, 1.0,0.0 );
      S3DVertex C( c.x, c.y, c.z, 0,1,0, color, 1.0,1.0 );
      o.addQuad( B,F,G,C );
      S3DVertex A( a.x, a.y, a.z, 0,-1,0, color, 0.0,1.0 );
      S3DVertex E( e.x, e.y, e.z, 0,-1,0, color, 0.0,0.0 );
      S3DVertex H( h.x, h.y, h.z, 0,-1,0, color, 1.0,0.0 );
      S3DVertex D( d.x, d.y, d.z, 0,-1,0, color, 1.0,1.0 );
      o.addQuad( A,E,H,D );
      }
   }

   static void
   add( SMeshBuffer & o, glm::vec3 cubeSize, uint32_t color = 0xFFFFFFFF )
   {
      float dx = cubeSize.x * 0.5f;
      float dy = cubeSize.y * 0.5f;
      float dz = cubeSize.z * 0.5f;
      auto a = glm::vec3( -dx, -dy, -dz );
      auto b = glm::vec3( -dx,  dy, -dz );
      auto c = glm::vec3(  dx,  dy, -dz );
      auto d = glm::vec3(  dx, -dy, -dz );
      auto e = glm::vec3( -dx, -dy,  dz );
      auto f = glm::vec3( -dx,  dy,  dz );
      auto g = glm::vec3(  dx,  dy,  dz );
      auto h = glm::vec3(  dx, -dy,  dz );
      add( o, a,b,c,d, e,f,g,h, color );
   }
};

} // end namespace smesh.
} // end namespace gpu.
} // end namespace de.

#endif

