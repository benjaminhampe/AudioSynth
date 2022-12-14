#ifndef DE_GPU_SMESH_TRIANGLE_ROUNDRECT_HPP
#define DE_GPU_SMESH_TRIANGLE_ROUNDRECT_HPP

#include <de/gpu/smesh/SMesh.hpp>

namespace de {
namespace gpu {
namespace smesh {

//
// Vertices:   12 + 4*tess
//
//       x0  x1  x2  x3
// ry    |   |   |   |
//       |
// y0 ---+   G---H   tess=1   (CW) TopQuad:     BGHC(1,6,7,2)
//           |  /|  /         (CW) BottomQuad  LADK(11,0,3,10)
//         ry| / | /
//           |/  |/       A=0, B=1, C=2, D=3, E=4, F=5, G=6, H=7, I=8, J=9, K=10, L=11
// y1 -  F---B---C---I
//       |  /|  /|  /|
//       | / | M | / |    Round Corners as triangles:
//       |/  |/  |/  |
// y2 -  E---A---D---J        (CW) top-left = FGB
//        rx |  /|            (CW) top-right = HIC
//           | / |            (CW) bottom-right = JKD
//           |/  |            (CW) bottom-left = LEA
// y3 ---+   L---K
//
// =========================================================================
struct Generic2DTriangleRoundRect
// =========================================================================
{
   typedef std::function<uint32_t()> GET_VERTEXCOUNT;
   // void addVertex( float x, float y, uint32_t color )
   typedef std::function<void(float,float, uint32_t)> ADD_VERTEX;
   // void addVertex( float x, float y, float z, float nx, float ny, float nz, uint32_t color, float u, float v )
   //typedef std::function<void(float,float,float, float,float,float, uint32_t, float,float )> ADD_VERTEX;
   typedef std::function<void(uint32_t)> ADD_INDEX;

   DE_CREATE_LOGGER("de.gpu.Generic2DRoundRect")

   static void
   addTriangles(
      float w, float h, float rx, float ry,
      int32_t tess, uint32_t innerColor, uint32_t outerColor,
      GET_VERTEXCOUNT const & getVertexCount,
      ADD_VERTEX const & addVertex,
      ADD_INDEX const & addIndex )
   {
      uint32_t const vStart = getVertexCount();

      auto addIndexedTriangle = [&] ( uint32_t iA, uint32_t iB, uint32_t iC )
      {
         addIndex( vStart + iA );   // input ABC (cw), output ACB (ccw)
         addIndex( vStart + iC );
         addIndex( vStart + iB );
      };

      auto addIndexedQuad = [&] ( uint32_t iA, uint32_t iB, uint32_t iC, uint32_t iD )
      {
         addIndexedTriangle( iA,iB,iC );   // input ABC (cw)
         addIndexedTriangle( iA,iC,iD );   // input ACD (cw)
      };

      float const x0 = -0.5f * w;
      float const x1 = -0.5f * w + rx;
      float const x2 = 0.5f * w - rx;
      float const x3 = 0.5f * w;
      float const y0 = -0.5f * h;
      float const y1 = -0.5f * h + ry;
      float const y2 = 0.5f * h - ry;
      float const y3 = 0.5f * h;

      // Add 12 vertices for 5 main quads
      addVertex( x1, y2, innerColor ); // A=0
      addVertex( x1, y1, innerColor ); // B=1
      addVertex( x2, y1, innerColor ); // C=2
      addVertex( x2, y2, innerColor ); // D=3
      addVertex( x0, y2, outerColor ); // E=4
      addVertex( x0, y1, outerColor ); // F=5
      addVertex( x1, y0, outerColor ); // G=6
      addVertex( x2, y0, outerColor ); // H=7
      addVertex( x3, y1, outerColor ); // I=8
      addVertex( x3, y2, outerColor ); // J=9
      addVertex( x2, y3, outerColor ); // K=A
      addVertex( x1, y3, outerColor ); // L=B

      // Add 20 indices for 5 main quads
      uint32_t const INDEX_A = 0;
      uint32_t const INDEX_B = 1;
      uint32_t const INDEX_C = 2;
      uint32_t const INDEX_D = 3;
      uint32_t const INDEX_E = 4;
      uint32_t const INDEX_F = 5;
      uint32_t const INDEX_G = 6;
      uint32_t const INDEX_H = 7;
      uint32_t const INDEX_I = 8;
      uint32_t const INDEX_J = 9;
      uint32_t const INDEX_K = 10;
      uint32_t const INDEX_L = 11;
      uint32_t const INDEX_TOPLEFT = 12 + 0*tess;
      uint32_t const INDEX_TOPRIGHT = 12 + 1*tess;
      uint32_t const INDEX_BOTTOMRIGHT = 12 + 2*tess;
      uint32_t const INDEX_BOTTOMLEFT = 12 + 3*tess;
      //uint32_t const INDEX_END = 12 + 4*tess;
      addIndexedQuad( INDEX_B, INDEX_G, INDEX_H, INDEX_C ); // Top BGHC
      addIndexedQuad( INDEX_E, INDEX_F, INDEX_B, INDEX_A ); // Center EFBA
      addIndexedQuad( INDEX_A, INDEX_B, INDEX_C, INDEX_D ); // Center ABCD
      addIndexedQuad( INDEX_D, INDEX_C, INDEX_I, INDEX_J ); // Center DCIJ
      addIndexedQuad( INDEX_L, INDEX_A, INDEX_D, INDEX_K ); // Bottom LADK

      //
      // Add 4*tess corner vertices. They are between the main vertices.
      //
      struct SinCos
      {
         float s,c;
         SinCos() : s(0.f), c(0.f) {}
         SinCos(float _s,float _c) : s(_s), c(_c) {}
      };
      std::vector< SinCos > lookUpTable;
      lookUpTable.reserve( tess );

      float phi_step = float( 0.5 * M_PI ) / float( tess+1 );
      float phi = phi_step; // phi_start > 0?? and phi_end < 90??
      for ( uint32_t i = 0; i < tess; ++i )
      {
         lookUpTable.emplace_back( ::sinf( phi ), ::cosf( phi ) );
         phi += phi_step;
      }

      // 1. topLeft FGB (cw) quarter circle
      for ( uint32_t i = 0; i < tess; ++i )
      {
         float x = x1 - rx * lookUpTable[ i ].c;   // Vertices already in correct index order.
         float y = y1 - ry * lookUpTable[ i ].s;
         addVertex( x, y, outerColor );
      }
      // 2. topRight HIC (cw) quarter circle
      for ( int32_t i = tess - 1; i >= 0; --i )
      {
         float x = x2 + rx * lookUpTable[ i ].c;   // Add vertices in reverse (--i) order
         float y = y1 - ry * lookUpTable[ i ].s;   // to build indices in correct order.
         addVertex( x, y, outerColor );
      }
      // 3. bottomRight JKD (cw) quarter circle
      for ( uint32_t i = 0; i < tess; ++i )
      {
         float x = x2 + rx * lookUpTable[ i ].c;   // Vertices already in correct index order.
         float y = y2 + ry * lookUpTable[ i ].s;
         addVertex( x, y, outerColor );
      }
      // 4. bottomLeft LEA (cw) quarter circle
      for ( int32_t i = tess - 1; i >= 0; --i )
      {
         float x = x1 - rx * lookUpTable[ i ].c;   // Add vertices in reverse (--i) order
         float y = y2 + ry * lookUpTable[ i ].s;   // to build indices in correct order.
         addVertex( x, y, outerColor );
      }

      // add 4*tess CornerIndices:

      // 1. topLeft FGB (cw) quarter circle, glm::vec3 B( x1, 0.f, y2 );
      for ( uint32_t i = 0; i < tess; ++i )
      {
         uint32_t F = INDEX_TOPLEFT + i;
         uint32_t G = INDEX_TOPLEFT + i + 1;
         uint32_t B = INDEX_B;   // const (center)
         if ( i == 0 )             { F = INDEX_F; } // StartTriangle
         else if ( i == tess - 1 ) { G = INDEX_G; } // EndTriangle
         else                      {}               // MiddleTriangle
         addIndexedTriangle( F, G, B ); // all given in (cw)
      }

      // 2. topRight HIC (cw) quarter circle, glm::vec3 C( x2, 0.f, y2 );
      for ( uint32_t i = 0; i < tess; ++i )
      {
         uint32_t H = INDEX_TOPRIGHT + i;
         uint32_t I = INDEX_TOPRIGHT + i + 1;
         uint32_t C = INDEX_C;   // const (center)
         if ( i == 0 )             { H = INDEX_H; } // StartTriangle
         else if ( i == tess - 1 ) { I = INDEX_I; } // EndTriangle
         else                      {}               // MiddleTriangle
         addIndexedTriangle( H,I,C ); // all given in (cw)
      }

      // 3. bottomRight JKD (cw) quarter circle, glm::vec3 D( x2, 0.f, y1 );
      for ( uint32_t i = 0; i < tess; ++i )
      {
         uint32_t J = INDEX_BOTTOMRIGHT + i;
         uint32_t K = INDEX_BOTTOMRIGHT + i + 1;
         uint32_t D = INDEX_D;   // const (center)
         if ( i == 0 )             { J = INDEX_J;  } // StartTriangle
         else if ( i == tess - 1 ) { K = INDEX_K;  } // EndTriangle
         else                      {}                // MiddleTriangle
         addIndexedTriangle( J, K, D ); // all given in (cw)
      }

      // 4. bottomLeft LEA (cw) quarter circle, glm::vec3 A( x1, 0.f, y1 );
      for ( uint32_t i = 0; i < tess; ++i )
      {
         uint32_t L = INDEX_BOTTOMLEFT + i;
         uint32_t E = INDEX_BOTTOMLEFT + i + 1;
         uint32_t A = INDEX_A;   // const (center)
         if ( i == 0 )             { L = INDEX_L; } // StartTriangle
         else if ( i == tess - 1 ) { E = INDEX_E; } // EndTriangle
         else                      {}               // MiddleTriangle
         addIndexedTriangle( L, E, A ); // all given in (cw)
      }
   }


};


struct RoundRect
{
   DE_CREATE_LOGGER("de.gpu.smesh.RoundRect")

   static void
   addXY( SMeshBuffer & o,
         glm::vec3 const & pos,
         float w, float h, float rx, float ry,
         int32_t tess = 13,
         uint32_t innerColor = 0xFFFFFFFF,
         uint32_t outerColor = 0xFFFFFFFF )
   {
      auto getVertexCount = [&] () { return o.getVertexCount(); };
      auto addVertex = [&] ( float x, float y, uint32_t color )
      {
         float px = pos.x + x;
         float py = pos.y + y;
         float pz = pos.z;
         float u = 0.5f + x / w;
         float v = 0.5f + y / h;
         o.addVertex( S3DVertex( px,py,pz, 0,0,-1, color, u,v ) );
      };

      auto addIndex = [&] ( uint32_t index ) { o.addIndex( index ); };

      Generic2DTriangleRoundRect::addTriangles( w, h, rx, ry, tess, innerColor, outerColor,
         getVertexCount, addVertex, addIndex );

      o.recalculateBoundingBox();
   }

   static void
   addXZ( SMeshBuffer & o,
         glm::vec3 const & pos,
         float w, float h, float rx, float ry,
         int32_t tess = 13,
         uint32_t innerColor = 0xFFFFFFFF,
         uint32_t outerColor = 0xFFFFFFFF )
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

      Generic2DTriangleRoundRect::addTriangles( w, h, rx, ry, tess, innerColor, outerColor,
         getVertexCount, addVertex, addIndex );

      o.recalculateBoundingBox();
   }

      // static SMeshBuffer*
   // create( float w, float h, float rx, float ry,
            // int32_t tess, uint32_t innerColor, uint32_t outerColor )
   // {
      // SMeshBuffer* p = new SMeshBuffer( PrimitiveType::Triangles );
      // p->setTexture( 0, tex );
      // p->setLighting( false );
      // //sky->setCulling( false );
      // //sky->setDepth( de::gpu::Depth::alwaysPass() );
      // return sky;
   // }
};

} // end namespace smesh.
} // end namespace gpu.
} // end namespace de.





//
// Vertices:   12 + 4*tess
//
//       x0  x1  x2  x3
// ry    |   |   |   |
//       |
// y0 ---+   G---H   tess=1   (CW) TopQuad:     BGHC(1,6,7,2)
//           |  /|  /         (CW) BottomQuad  LADK(11,0,3,10)
//         ry| / | /
//           |/  |/       A=0, B=1, C=2, D=3, E=4, F=5, G=6, H=7, I=8, J=9, K=10, L=11
// y1 -  F---B---C---I
//       |  /|  /|  /|
//       | / | M | / |    Round Corners as triangles:
//       |/  |/  |/  |
// y2 -  E---A---D---J        (CW) top-left = FGB
//        rx |  /|            (CW) top-right = HIC
//           | / |            (CW) bottom-right = JKD
//           |/  |            (CW) bottom-left = LEA
// y3 ---+   L---K
//
/*
// =========================================================================
struct Generic2DRoundRect
// =========================================================================
{
   typedef std::function<uint32_t()> GET_VERTEXCOUNT;
   // void addVertex( float x, float y, uint32_t color )
   typedef std::function<void(float,float, uint32_t)> ADD_VERTEX;
   // void addVertex( float x, float y, float z, float nx, float ny, float nz, uint32_t color, float u, float v )
   //typedef std::function<void(float,float,float, float,float,float, uint32_t, float,float )> ADD_VERTEX;
   typedef std::function<void(uint32_t)> ADD_INDEX;

   DE_CREATE_LOGGER("de.gpu.Generic2DRoundRect")

   static void
   addIndexed(
      float w, float h, float rx, float ry,
      int32_t tess, uint32_t innerColor, uint32_t outerColor,
      GET_VERTEXCOUNT const & getVertexCount,
      ADD_VERTEX const & addVertex,
      ADD_INDEX const & addIndex )
   {
      uint32_t const vStart = getVertexCount();

      auto addIndexedTriangle = [&] ( uint32_t iA, uint32_t iB, uint32_t iC )
      {
         addIndex( vStart + iA );   // input ABC (cw), output ACB (ccw)
         addIndex( vStart + iC );
         addIndex( vStart + iB );
      };

      auto addIndexedQuad = [&] ( uint32_t iA, uint32_t iB, uint32_t iC, uint32_t iD )
      {
         addIndexedTriangle( iA,iB,iC );   // input ABC (cw)
         addIndexedTriangle( iA,iC,iD );   // input ACD (cw)
      };

      float const x0 = -0.5f * w;
      float const x1 = -0.5f * w + rx;
      float const x2 = 0.5f * w - rx;
      float const x3 = 0.5f * w;
      float const y0 = -0.5f * h;
      float const y1 = -0.5f * h + ry;
      float const y2 = 0.5f * h - ry;
      float const y3 = 0.5f * h;

      // Add 12 vertices for 5 main quads
      addVertex( x1, y2, innerColor ); // A=0
      addVertex( x1, y1, innerColor ); // B=1
      addVertex( x2, y1, innerColor ); // C=2
      addVertex( x2, y2, innerColor ); // D=3
      addVertex( x0, y2, outerColor ); // E=4
      addVertex( x0, y1, outerColor ); // F=5
      addVertex( x1, y0, outerColor ); // G=6
      addVertex( x2, y0, outerColor ); // H=7
      addVertex( x3, y1, outerColor ); // I=8
      addVertex( x3, y2, outerColor ); // J=9
      addVertex( x2, y3, outerColor ); // K=A
      addVertex( x1, y3, outerColor ); // L=B

      // Add 20 indices for 5 main quads
      uint32_t const INDEX_A = 0;
      uint32_t const INDEX_B = 1;
      uint32_t const INDEX_C = 2;
      uint32_t const INDEX_D = 3;
      uint32_t const INDEX_E = 4;
      uint32_t const INDEX_F = 5;
      uint32_t const INDEX_G = 6;
      uint32_t const INDEX_H = 7;
      uint32_t const INDEX_I = 8;
      uint32_t const INDEX_J = 9;
      uint32_t const INDEX_K = 10;
      uint32_t const INDEX_L = 11;
      uint32_t const INDEX_TOPLEFT = 12 + 0*tess;
      uint32_t const INDEX_TOPRIGHT = 12 + 1*tess;
      uint32_t const INDEX_BOTTOMRIGHT = 12 + 2*tess;
      uint32_t const INDEX_BOTTOMLEFT = 12 + 3*tess;
      //uint32_t const INDEX_END = 12 + 4*tess;
      addIndexedQuad( INDEX_B, INDEX_G, INDEX_H, INDEX_C ); // Top BGHC
      addIndexedQuad( INDEX_E, INDEX_F, INDEX_B, INDEX_A ); // Center EFBA
      addIndexedQuad( INDEX_A, INDEX_B, INDEX_C, INDEX_D ); // Center ABCD
      addIndexedQuad( INDEX_D, INDEX_C, INDEX_I, INDEX_J ); // Center DCIJ
      addIndexedQuad( INDEX_L, INDEX_A, INDEX_D, INDEX_K ); // Bottom LADK

      //
      // Add 4*tess corner vertices. They are between the main vertices.
      //
      struct SinCos
      {
         float s,c;
         SinCos() : s(0.f), c(0.f) {}
         SinCos(float _s,float _c) : s(_s), c(_c) {}
      };
      std::vector< SinCos > lookUpTable;
      lookUpTable.reserve( tess );

      float phi_step = float( 0.5 * M_PI ) / float( tess+1 );
      float phi = phi_step; // phi_start > 0?? and phi_end < 90??
      for ( uint32_t i = 0; i < tess; ++i )
      {
         lookUpTable.emplace_back( ::sinf( phi ), ::cosf( phi ) );
         phi += phi_step;
      }

      // 1. topLeft FGB (cw) quarter circle
      for ( uint32_t i = 0; i < tess; ++i )
      {
         float x = x1 - rx * lookUpTable[ i ].c;   // Vertices already in correct index order.
         float y = y1 - ry * lookUpTable[ i ].s;
         addVertex( x, y, outerColor );
      }
      // 2. topRight HIC (cw) quarter circle
      for ( int32_t i = tess - 1; i >= 0; --i )
      {
         float x = x2 + rx * lookUpTable[ i ].c;   // Add vertices in reverse (--i) order
         float y = y1 - ry * lookUpTable[ i ].s;   // to build indices in correct order.
         addVertex( x, y, outerColor );
      }
      // 3. bottomRight JKD (cw) quarter circle
      for ( uint32_t i = 0; i < tess; ++i )
      {
         float x = x2 + rx * lookUpTable[ i ].c;   // Vertices already in correct index order.
         float y = y2 + ry * lookUpTable[ i ].s;
         addVertex( x, y, outerColor );
      }
      // 4. bottomLeft LEA (cw) quarter circle
      for ( int32_t i = tess - 1; i >= 0; --i )
      {
         float x = x1 - rx * lookUpTable[ i ].c;   // Add vertices in reverse (--i) order
         float y = y2 + ry * lookUpTable[ i ].s;   // to build indices in correct order.
         addVertex( x, y, outerColor );
      }

      // add 4*tess CornerIndices:

      // 1. topLeft FGB (cw) quarter circle, glm::vec3 B( x1, 0.f, y2 );
      for ( uint32_t i = 0; i < tess; ++i )
      {
         uint32_t F = INDEX_TOPLEFT + i;
         uint32_t G = INDEX_TOPLEFT + i + 1;
         uint32_t B = INDEX_B;   // const (center)
         if ( i == 0 )             { F = INDEX_F; } // StartTriangle
         else if ( i == tess - 1 ) { G = INDEX_G; } // EndTriangle
         else                      {}               // MiddleTriangle
         addIndexedTriangle( F, G, B ); // all given in (cw)
      }

      // 2. topRight HIC (cw) quarter circle, glm::vec3 C( x2, 0.f, y2 );
      for ( uint32_t i = 0; i < tess; ++i )
      {
         uint32_t H = INDEX_TOPRIGHT + i;
         uint32_t I = INDEX_TOPRIGHT + i + 1;
         uint32_t C = INDEX_C;   // const (center)
         if ( i == 0 )             { H = INDEX_H; } // StartTriangle
         else if ( i == tess - 1 ) { I = INDEX_I; } // EndTriangle
         else                      {}               // MiddleTriangle
         addIndexedTriangle( H,I,C ); // all given in (cw)
      }

      // 3. bottomRight JKD (cw) quarter circle, glm::vec3 D( x2, 0.f, y1 );
      for ( uint32_t i = 0; i < tess; ++i )
      {
         uint32_t J = INDEX_BOTTOMRIGHT + i;
         uint32_t K = INDEX_BOTTOMRIGHT + i + 1;
         uint32_t D = INDEX_D;   // const (center)
         if ( i == 0 )             { J = INDEX_J;  } // StartTriangle
         else if ( i == tess - 1 ) { K = INDEX_K;  } // EndTriangle
         else                      {}                // MiddleTriangle
         addIndexedTriangle( J, K, D ); // all given in (cw)
      }

      // 4. bottomLeft LEA (cw) quarter circle, glm::vec3 A( x1, 0.f, y1 );
      for ( uint32_t i = 0; i < tess; ++i )
      {
         uint32_t L = INDEX_BOTTOMLEFT + i;
         uint32_t E = INDEX_BOTTOMLEFT + i + 1;
         uint32_t A = INDEX_A;   // const (center)
         if ( i == 0 )             { L = INDEX_L; } // StartTriangle
         else if ( i == tess - 1 ) { E = INDEX_E; } // EndTriangle
         else                      {}               // MiddleTriangle
         addIndexedTriangle( L, E, A ); // all given in (cw)
      }
   }


};

struct RoundRect
{
   DE_CREATE_LOGGER("de.gpu.SMeshRoundRect")

   static void
   addXY( SMeshBuffer & o,
         glm::vec3 const & pos,
         float w, float h, float rx, float ry,
         int32_t tess = 13,
         uint32_t innerColor = 0xFFFFFFFF,
         uint32_t outerColor = 0xFFFFFFFF )
   {
      auto getVertexCount = [&] () { return o.getVertexCount(); };
      auto addVertex = [&] ( float x, float y, uint32_t color )
      {
         float px = pos.x + x;
         float py = pos.y + y;
         float pz = pos.z;
         float u = 0.5f + x / w;
         float v = 0.5f + y / h;
         o.addVertex( S3DVertex( px,py,pz, 0,0,-1, color, u,v ) );
      };

      auto addIndex = [&] ( uint32_t index ) { o.addIndex( index ); };

      Generic2DRoundRect::addIndexed( w, h, rx, ry, tess, innerColor, outerColor,
         getVertexCount, addVertex, addIndex );

      o.recalculateBoundingBox();
   }

   static void
   addXZ( SMeshBuffer & o,
         glm::vec3 const & pos,
         float w, float h, float rx, float ry,
         int32_t tess = 13,
         uint32_t innerColor = 0xFFFFFFFF,
         uint32_t outerColor = 0xFFFFFFFF )
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

      Generic2DRoundRect::addIndexed( w, h, rx, ry, tess, innerColor, outerColor,
         getVertexCount, addVertex, addIndex );

      o.recalculateBoundingBox();
   }

      // static SMeshBuffer*
   // create( float w, float h, float rx, float ry,
            // int32_t tess, uint32_t innerColor, uint32_t outerColor )
   // {
      // SMeshBuffer* p = new SMeshBuffer( PrimitiveType::Triangles );
      // p->setTexture( 0, tex );
      // p->setLighting( false );
      // //sky->setCulling( false );
      // //sky->setDepth( de::gpu::Depth::alwaysPass() );
      // return sky;
   // }
};
*/

#endif

