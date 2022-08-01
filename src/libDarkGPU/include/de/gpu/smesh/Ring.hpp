#ifndef DE_GPU_SMESH_TRIANGLE_RING_HPP
#define DE_GPU_SMESH_TRIANGLE_RING_HPP

#include <de/gpu/smesh/SMesh.hpp>

namespace de {
namespace gpu {
namespace smesh {

// o.moreVertices( 2 * ( tess + 1 ) );
// o.moreIndices( 6 * tess );

// =========================================================================
struct Generic2DTriangleRing
// =========================================================================
{
   // uint32_t getVertexCount();
   typedef std::function<uint32_t()> GET_VERTEXCOUNT;
   // void addVertex( float x, float y, uint32_t color );
   typedef std::function<void(float, float, uint32_t)> ADD_VERTEX;
   // void addIndex( uint32_t index );
   typedef std::function<void(uint32_t)> ADD_INDEX;

   static void
   addIndexedTriangles(
      float outer_x, float outer_y,
      float inner_x, float inner_y,
      int32_t tess, uint32_t innerColor, uint32_t outerColor,
      GET_VERTEXCOUNT const & getVertexCount,
      ADD_VERTEX const & addVertex,
      ADD_INDEX const & addIndex )
   {
      uint32_t const vStart = getVertexCount();

      auto addIndexedQuad = [&] ( uint32_t a, uint32_t b, uint32_t c, uint32_t d )
      {
         addIndex( a ); addIndex( c ); addIndex( b ); // ABC (cw) = ACB (ccw)
         addIndex( a ); addIndex( d ); addIndex( c ); // ACD (cw) = ADC (ccw)
      };

      float const ix = 0.5f * inner_x;
      float const iy = 0.5f * inner_y;
      float const ox = 0.5f * outer_x; // a,b are radia, not sizes.
      float const oy = 0.5f * outer_y;
      float const phi_step = float( ( 2.0 * M_PI ) / double( tess ) ); // 0 to 2pi

      // inner ring vertices
      for ( uint32_t i = 0; i <= tess; ++i )
      {
         float const phi = phi_step * float( i );
         float const s = -::sinf( phi );
         float const c =  ::cosf( phi );
         addVertex( ix * s, iy * c, innerColor );
      }

      // outer ring vertices
      for ( uint32_t i = 0; i <= tess; ++i )
      {
         float const phi = phi_step * float( i );
         float const s = -::sinf( phi );
         float const c =  ::cosf( phi );
         addVertex( ox * s, oy * c, outerColor );
      }

      // ring quad indices
      for ( uint32_t i = 0; i < tess; ++i )
      {
         uint32_t A = vStart + i;
         uint32_t B = vStart + i + tess + 1;
         uint32_t C = vStart + i + tess + 2;
         uint32_t D = vStart + i + 1;
         addIndexedQuad( B, A, D, C );
      }
   }
};

// =========================================================================
struct Ring
// =========================================================================
{
   static void
   addXY( SMeshBuffer & o,
         float w,
         float h,
         float rx,
         float ry,
         uint32_t innerColor = 0xFFFFFFFF,
         uint32_t outerColor = 0xFFFFFFFF,
         int32_t tess = 13,
         glm::vec3 const & pos = glm::vec3(0,0,0) )
   {
      auto getVertexCount = [&] () { return o.getVertexCount(); };
      auto addIndex = [&] ( uint32_t index ) { o.addIndex( index ); };
      auto addVertex = [&] ( float px, float py, uint32_t color )
      {
         float x = pos.x + px;
         float y = pos.y + py;
         float z = pos.z;
         float u = 0.5f + px / w;
         float v = 0.5f - py / h;
         o.addVertex( S3DVertex( x,y,z, 0,0,-1, color, u,v ) );
      };

      Generic2DTriangleRing::addIndexedTriangles( w, h, rx, ry, tess, innerColor, outerColor,
         getVertexCount, addVertex, addIndex );
   }

   static void
   addXZ(
      SMeshBuffer & o,
      float w,
      float h,
      float rx,
      float rz,
      uint32_t innerColor = 0xFFFFFFFF,
      uint32_t outerColor = 0xFFFFFFFF,
      int32_t tess = 13,
      glm::vec3 const & pos = glm::vec3(0,0,0) )
   {
      uint32_t const vStart = o.getVertexCount();

      float const ix = 0.5f * (w-rx);
      float const iz = 0.5f * (h-rz);
      float const ox = 0.5f * w; // a,b are radia, not sizes.
      float const oz = 0.5f * h;
      float const phi_step = float( ( 2.0 * M_PI ) / double( tess ) ); // 0 to 2pi

      // Inner ring vertices
      for ( uint32_t i = 0; i <= tess; ++i )
      {
         float const phi = phi_step * float( i );
         float const s = -::sinf( phi );
         float const c =  ::cosf( phi );
         float const u = 0.5f + 0.5f * s;
         float const v = 0.5f - 0.5f * c;
         o.addVertex( S3DVertex( pos.x + ix * s, pos.y, pos.z + iz * c, 0,0,-1, innerColor, u,v ) );
      }

      // Outer ring vertices
      for ( uint32_t i = 0; i <= tess; ++i )
      {
         float const phi = phi_step * float( i );
         float const s = -::sinf( phi );
         float const c =  ::cosf( phi );
         float const u = 0.5f + 0.5f * s;
         float const v = 0.5f - 0.5f * c;
         o.addVertex( S3DVertex( pos.x + ox * s, pos.y, pos.z + oz * c, 0,0,-1, outerColor, u,v ) );
      }

      // Build quads
      for ( uint32_t i = 0; i < tess; ++i )
      {
         uint32_t A = vStart + i;
         uint32_t B = vStart + i + tess + 1;
         uint32_t C = vStart + i + tess + 2;
         uint32_t D = vStart + i + 1;
         o.addIndexedQuad( B, A, D, C );
      }

   }
};

} // end namespace smesh.
} // end namespace gpu.
} // end namespace de.


/*

// o.moreVertices( 2 * ( tess + 1 ) );
// o.moreIndices( 6 * tess );

// =========================================================================
struct Generic2DRing
// =========================================================================
{
   // uint32_t getVertexCount();
   typedef std::function<uint32_t()> GET_VERTEXCOUNT;
   // void addVertex( float x, float y, uint32_t color );
   typedef std::function<void(float, float, uint32_t)> ADD_VERTEX;
   // void addIndex( uint32_t index );
   typedef std::function<void(uint32_t)> ADD_INDEX;

   static void
   addIndexedTriangles(
      float outer_x, float outer_y,
      float inner_x, float inner_y,
      int32_t tess, uint32_t innerColor, uint32_t outerColor,
      GET_VERTEXCOUNT const & getVertexCount,
      ADD_VERTEX const & addVertex,
      ADD_INDEX const & addIndex )
   {
      uint32_t const vStart = getVertexCount();

      auto addIndexedQuad = [&] ( uint32_t a, uint32_t b, uint32_t c, uint32_t d )
      {
         addIndex( a ); addIndex( c ); addIndex( b ); // ABC (cw) = ACB (ccw)
         addIndex( a ); addIndex( d ); addIndex( c ); // ACD (cw) = ADC (ccw)
      };

      float const ix = 0.5f * inner_x;
      float const iy = 0.5f * inner_y;
      float const ox = 0.5f * outer_x; // a,b are radia, not sizes.
      float const oy = 0.5f * outer_y;
      float const phi_step = float( ( 2.0 * M_PI ) / double( tess ) ); // 0 to 2pi

      // inner ring vertices
      for ( uint32_t i = 0; i <= tess; ++i )
      {
         float const phi = phi_step * float( i );
         float const s = -::sinf( phi );
         float const c =  ::cosf( phi );
         addVertex( ix * s, iy * c, innerColor );
      }

      // outer ring vertices
      for ( uint32_t i = 0; i <= tess; ++i )
      {
         float const phi = phi_step * float( i );
         float const s = -::sinf( phi );
         float const c =  ::cosf( phi );
         addVertex( ox * s, oy * c, outerColor );
      }

      // ring quad indices
      for ( uint32_t i = 0; i < tess; ++i )
      {
         uint32_t A = vStart + i;
         uint32_t B = vStart + i + tess + 1;
         uint32_t C = vStart + i + tess + 2;
         uint32_t D = vStart + i + 1;
         addIndexedQuad( B, A, D, C );
      }
   }
};

// =========================================================================
struct Ring
// =========================================================================
{
   static void
   add( SMeshBuffer & o,
         glm::vec3 const & pos,
         float w, float h,
         float rx, float ry,
         int32_t tess = 13,
         uint32_t innerColor = 0xFFFFFFFF, uint32_t outerColor = 0xFFFFFFFF )
   {
      auto getVertexCount = [&] () { return o.getVertexCount(); };
      auto addVertex = [&] ( float x, float y, uint32_t color )
      {
         float px = pos.x + x;
         float py = pos.y + y;
         float pz = pos.z;
         float u = 0.5f + x / w;
         float v = 0.5f - y / h;
         o.addVertex( S3DVertex( px,py,pz, 0,0,-1, color, u,v ) );
      };

      auto addIndex = [&] ( uint32_t index ) { o.addIndex( index ); };

      Generic2DTriangleRing::addIndexedTriangles( w, h, rx, ry, tess, innerColor, outerColor,
         getVertexCount, addVertex, addIndex );
   }

   static void
   addTrianglesXZ( SMeshBuffer & o,
         glm::vec3 const & pos,
         float w, float h,
         float rx, float ry,
         int32_t tess = 13,
         uint32_t innerColor = 0xFFFFFFFF, uint32_t outerColor = 0xFFFFFFFF )
   {
      auto getVertexCount = [&] () { return o.getVertexCount(); };
      auto addVertex = [&] ( float x, float y, uint32_t color )
      {
         float px = pos.x + x;
         float py = pos.y;
         float pz = pos.z + y;
         float u = 0.5f + x / w;
         float v = 0.5f - y / h;
         o.addVertex( S3DVertex( px,py,pz, 0,0,-1, color, u,v ) );
      };

      auto addIndex = [&] ( uint32_t index ) { o.addIndex( index ); };

      Generic2DTriangleRing::addIndexedTriangles( w, h, rx, ry, tess, innerColor, outerColor,
         getVertexCount, addVertex, addIndex );
   }
};
*/

#endif

