#ifndef DE_GPU_SMESH_LINECIRCLE_HPP
#define DE_GPU_SMESH_LINECIRCLE_HPP

#include <de/gpu/smesh/Line.hpp>

namespace de {
namespace gpu {
namespace smesh {

// =========================================================================
struct LineCircle
// =========================================================================
{
   DE_CREATE_LOGGER("de.gpu.smesh.LineCircle")

   static void
   addXY(
         SMeshBuffer & o,
         glm::vec2 const & radius,
         uint32_t color = 0xFFFFFFFF,
         int32_t tessRadial = 33 )
   {
      uint32_t const v = o.getVertexCount();

      float const angleStep = float( ( 2.0 * M_PI ) / double( tessRadial ) );
//      float const outwardX = float( radius.x ) / float( tessOutwards );
//      float const outwardY = float( radius.y ) / float( tessOutwards );

      for ( uint32_t i = 0; i <= tessRadial; ++i )
      {
         float const phi = angleStep * float( i );
         float const s = ::sinf( phi );
         float const c = ::cosf( phi );

//         for ( uint32_t j = 0; j <= tessOutwards; ++j )
//         {
         float x = (radius.x * c); // * float( j ) * outwardX);
         float y = (radius.y * s); // * float( j ) * outwardY);
         float u = std::clamp( 0.5f + 0.5f * c, 0.0001f, 0.9999f );
         float v = std::clamp( 0.5f - 0.5f * s, 0.0001f, 0.9999f );
         o.addVertex( S3DVertex( x, y, 0.0f, 0.f, 0.f, -1.f, color, u, v ) );
//         }
      }

      for ( uint32_t i = 0; i < tessRadial; ++i )
      {
//         for ( uint32_t j = 0; j < tessOutwards; ++j )
//         {
         uint32_t M = v;
         uint32_t A = v + 1 + i;
         uint32_t B = v + 1 + i + 1;
         o.addIndexedLineTriangle( M, A, B );
//         }
      }
   }

};

} // end namespace smesh.
} // end namespace gpu.
} // end namespace de.

#endif

