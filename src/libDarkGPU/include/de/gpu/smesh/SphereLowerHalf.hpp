#ifndef DE_GPU_SMESH_SPHERE_LOWERHALF_HPP
#define DE_GPU_SMESH_SPHERE_LOWERHALF_HPP

#include <de/gpu/smesh/SMesh.hpp>

namespace de {
namespace gpu {
namespace smesh {

/*
// =========================================================================
struct SLowerHalfSphere
// =========================================================================
{
   // @brief Benni book example 2021
   static void
   add( SMeshBuffer & o,
         float rx = 50, float ry = 50, float rz = 50,
         uint32_t tessCircle = 15, uint32_t tessRows = 15,
         float x = 0.f, float y = 0.f, float z = 0.f,
         uint32_t color = 0xAFFFFFFF )
   {
      uint32_t const vStart = o.getVertexCount();

      // Vertices[vStart] = top center vertex
      o.addVertex( S3DVertex( 0,-ry,0, 0,1,0, color, 0.5f,0.5f ) );

      // Vertices[vStart+1 to vStart+tessCircle*tessRows ] = other vertices
      SinCosTablef sincosA(tessCircle);
      SinCosTablef sincosB(tessRows, float(0.0), float( 0.5*M_PI ) );

      for (uint32_t j=0; j<tessRows; ++j)
      {
         for (uint32_t i=0; i<tessCircle; ++i)
         {
            float const sinA = sincosA[i].s;
            float const cosA = sincosA[i].c;
            float const sinB = sincosB[j].s;
            float const cosB = sincosB[j].c;
            float const nx = -sinA * cosB;				// equals the normal at point radius*(x,y,z)
            float const ny = -sinB;					// equals the normal at point radius*(x,y,z)
            float const nz =  cosA * cosB;				// equals the normal at point radius*(x,y,z)
            float const u = 0.5f-0.5f*cosB * sinA; // not finished
            float const v = 0.5f-0.5f*cosB * cosA; // not finished
            o.addVertex( S3DVertex( rx*nx,ry*ny,rz*nz, nx,ny,nz, color, u,v ) );
         }
      }

      // push indices
      for ( uint32_t j = 0; j < tessRows; ++j )
      {
         for ( uint32_t i = 0; i < tessCircle; ++i )
         {
            // top row contains just triangles, no quads
            if ( j == tessRows - 1 )
            {
               uint32_t i0 = vStart + 1 + (j)*tessCircle + (i);    // A
               uint32_t i1 = vStart + 1 + (j)*tessCircle + (i+1);  // B
               uint32_t i2 = vStart;    // C - top center
               if ( i == tessCircle - 1 )
               {
                  i0 = vStart + 1 + (j)*tessCircle + (i);	// A
                  i1 = vStart + 1 + (j)*tessCircle + (0);	// B
               }
               o.addIndexedTriangle( i0, i1, i2 );
            }

            // bottom to top-1 rows consist of quad segments (each of 2 triangles)
            else
            {
               uint32_t i0 = vStart + 1 + (j)*tessCircle + (i);	// A
               uint32_t i1 = vStart + 1 + (j+1)*tessCircle + (i);	// B
               uint32_t i2 = vStart + 1 + (j+1)*tessCircle + (i+1);// C
               uint32_t i3 = vStart + 1 + (j)*tessCircle + (i+1);	// D
               if ( i == tessCircle - 1 )
               {
                  i0 = vStart + 1 + (j)*tessCircle + (i);	// A
                  i1 = vStart + 1 + (j+1)*tessCircle + (i);	// B
                  i2 = vStart + 1 + (j+1)*tessCircle + (0);	// C
                  i3 = vStart + 1 + (j)*tessCircle + (0);	// D
               }
               o.addIndexedQuad( i0, i1, i2, i3 );
            }
         }
      }
   }
};
*/

} // end namespace smesh.
} // end namespace gpu.
} // end namespace de.

#endif

