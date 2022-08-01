#include <de/gpu/smesh/S2DShapes.hpp>

namespace de {
namespace gpu {
namespace smesh {

void
S2DCircle::add( SMeshBuffer & o, Recti const & pos,
      uint32_t outerColor, uint32_t innerColor, TexRef const & ref, int tess )
{
   float rx = 0.5f * pos.getWidth();
   float ry = 0.5f * pos.getWidth();

   o.setPrimitiveType( PrimitiveType::Triangles );
   o.setBlend( Blend::alphaBlend() );
   if ( ref.tex )
   {
      o.setTexture( 0, ref );
   }

   // Add center point
   uint32_t v = o.getVertexCount();
   o.addVertex( S3DVertex( 0,0,0, 0,0,-1, innerColor, 0.5f, 0.5f ) );

   float const angleStep = float( ( 2.0 * M_PI ) / double( tess ) );
//      float const outwardX = float( radius.x ) / float( tessOutwards );
//      float const outwardY = float( radius.y ) / float( tessOutwards );

   for ( uint32_t i = 0; i <= tess; ++i )
   {
      float const phi = angleStep * float( i );
      float const s = ::sinf( phi );
      float const c = ::cosf( phi );
//   for ( uint32_t j = 0; j <= tessOutwards; ++j )
//   {
      float x = rx * c; // * float( j ) * outwardX);
      float y = ry * s; // * float( j ) * outwardY);
      float u = std::clamp( 0.5f + 0.5f * c, 0.0001f, 0.9999f );
      float v = std::clamp( 0.5f - 0.5f * s, 0.0001f, 0.9999f );
      o.addVertex( S3DVertex( x,y,0, 0,0,-1, outerColor, u, v ) );
//         }
   }

   for ( uint32_t i = 0; i < tess; ++i )
   {
//   for ( uint32_t j = 0; j < tessOutwards; ++j )
//   {
      uint32_t M = v;
      uint32_t A = v + 1 + i;
      uint32_t B = v + 2 + i;
      o.addIndexedTriangle( M, A, B );
//   }
   }
}

void
S2DCircleOutline::add( SMeshBuffer & o, Recti const & pos,
      uint32_t color, TexRef const & ref, int borderWidth, int tess )
{

   float rx = 0.5f * pos.getWidth();
   float ry = 0.5f * pos.getWidth();

   std::vector< glm::vec2 > points;
   float const angleStep = float( ( 2.0 * M_PI ) / double( tess ) );
   for ( uint32_t i = 0; i <= tess; ++i )
   {
      float const phi = angleStep * float( i );
      float const x = rx * ::sinf( phi );
      float const y = ry * ::cosf( phi );
      points.emplace_back( x, y );
   }
   points.emplace_back( points.front() );

   S2DPolyLine::addTriangleStrip( o, points, color, borderWidth, LineCap::Square, LineJoin::Bevel );
   o.setBlend( Blend::alphaBlend() );
   if ( ref.tex )
   {
      o.setTexture( 0, ref );
   }

}

void
S2DLine::add( SMeshBuffer & o, int ax, int ay, int bx, int by,
              uint32_t colorA, uint32_t colorB )
{
   o.setPrimitiveType( PrimitiveType::Lines );
   o.Vertices.reserve( 2 );
   o.Vertices.push_back( S3DVertex( ax, ay, -1, 0,0,0, colorA, 0,0 ) ); // A
   o.Vertices.push_back( S3DVertex( bx, by, -1, 0,0,0, colorB, 0,0 ) ); // B
}

//
//  B +-----+ C --> +x, +u
//    |   / |
//    | /   |     FrontQuad ABCD (cw) = ABC (cw) + ACD (cw)
//  A +-----+ D
//    |           Normal -z shows towards viewer
//   +v = -y      Pos.z is always -1, so its at near plane.
//
void
S2DRect::add( SMeshBuffer & o, Recti const & pos,
              uint32_t colorA, uint32_t colorB, uint32_t colorC, uint32_t colorD,
              TexRef const & ref )
{
   int w = pos.w > 0 ? pos.w : ref.getWidth();
   int h = pos.h > 0 ? pos.h : ref.getHeight();
   if ( w < 1 || w > 10000 ) { DE_DEBUG( "Invalid w ",w ) return; }
   if ( h < 1 || h > 10000 ) { DE_DEBUG( "Invalid h ",h ) return; }

   o.setPrimitiveType( PrimitiveType::Triangles );
   o.setBlend( Blend::alphaBlend() );
   if ( ref.tex )
   {
      o.setTexture( 0, ref );
   }

   int x = pos.x;
   int y = pos.y;
   Rectf const & coords = ref.coords; // Is identity(0,0,1,1) if ref.tex is empty/nullptr.
   auto u1 = coords.u1();
   auto v1 = coords.v1();
   auto u2 = coords.u2();
   auto v2 = coords.v2();
   S3DVertex A( x,   y-1,   1.f, 0,0,0, colorA, u1,v1 );
   S3DVertex B( x,   y-1+h, 1.f, 0,0,0, colorB, u1,v2 );
   S3DVertex C( x+w, y-1+h, 1.f, 0,0,0, colorC, u2,v2 );
   S3DVertex D( x+w, y-1,   1.f, 0,0,0, colorD, u2,v1 );
   o.addTriangle( A, B, C );
   o.addTriangle( A, C, D );
}

//
//  B +-----+ C --> +x, +u
//    |   / |
//    | /   |     FrontQuad ABCD (cw) = ABC (cw) + ACD (cw)
//  A +-----+ D
//    |           Normal -z shows towards viewer
//   +v = -y      Pos.z is always -1, so its at near plane.
//
void
S2DRectOutline::add( SMeshBuffer & o, Recti const & pos,
                     uint32_t color, TexRef const & ref, int borderWidth )
{
   int w = pos.w > 0 ? pos.w : ref.getWidth();
   int h = pos.h > 0 ? pos.h : ref.getHeight();
   if ( w < 1 || w > 10000 ) { DE_DEBUG( "Invalid w ",w ) return; }
   if ( h < 1 || h > 10000 ) { DE_DEBUG( "Invalid h ",h ) return; }

   o.setPrimitiveType( PrimitiveType::TriangleStrip );
   o.setLighting( 0 );
   if ( ref.tex )
   {
      o.setTexture( 0, ref );
   }
   o.setBlend( Blend::alphaBlend() );

   int x = pos.x;
   int y = pos.y;
   Rectf const & coords = ref.coords;
   auto const & u1 = coords.u1();
   auto const & v1 = coords.v1();
   auto const & u2 = coords.u2();
   auto const & v2 = coords.v2();

   std::vector< glm::vec2 > points;
   points.reserve( 5 );
   points.emplace_back( x, y );
   points.emplace_back( x, y+h );
   points.emplace_back( x+w, y+h );
   points.emplace_back( x+w, y );
   points.emplace_back( x, y );
   S2DPolyLine::addTriangleStrip( o, points, color, borderWidth, LineCap::None, LineJoin::Bevel );
}


void
S2DRectOutline::add( SMeshBuffer & o, Recti const & pos,
                     uint32_t colorA, uint32_t colorB, uint32_t colorC, uint32_t colorD,
                     TexRef const & ref, int borderWidth )
{
   int w = pos.w > 0 ? pos.w : ref.getWidth();
   int h = pos.h > 0 ? pos.h : ref.getHeight();
   if ( w < 1 || w > 10000 ) { DE_DEBUG( "Invalid w ",w ) return; }
   if ( h < 1 || h > 10000 ) { DE_DEBUG( "Invalid h ",h ) return; }

   o.setPrimitiveType( PrimitiveType::Lines );
   o.setLighting( 0 );
   if ( ref.tex )
   {
      o.setTexture( 0, ref );
   }
   o.setBlend( Blend::alphaBlend() );

   int x = pos.x;
   int y = pos.y;
   Rectf const & coords = ref.coords;
   auto const & u1 = coords.u1();
   auto const & v1 = coords.v1();
   auto const & u2 = coords.u2();
   auto const & v2 = coords.v2();
   //
   //  B +-----+ C --> +x, +u
   //    |   / |
   //    | /   |     FrontQuad ABCD (cw) = ABC (cw) + ACD (cw)
   //  A +-----+ D
   //    |           Normal -z shows towards viewer
   //   +v = -y      Pos.z is always -1, so its at near plane.
   //
//      int x1 = x + 1;
//      int y1 = y - 1;
//      int x2 = x + w;
//      int y2 = y + h;

   S3DVertex A1( x+1, y-1+h,  -1, 0,0,-1, colorA, u1,v2 );
   S3DVertex B1( x+1, y-1,    -1, 0,0,-1, colorB, u1,v1 );
   o.addLine( A1, B1 );

   S3DVertex B2( x,   y-1,    -1, 0,0,-1, colorB, u1,v1 );
   S3DVertex C2( x+w, y-1,    -1, 0,0,-1, colorC, u2,v1 );
   o.addLine( B2, C2 );

   S3DVertex C3( x+w, y-1,    -1, 0,0,-1, colorC, u2,v1 );
   S3DVertex D3( x+w, y-1+h,  -1, 0,0,-1, colorD, u2,v2 );
   o.addLine( C3, D3 );

   S3DVertex D4( x+w, y-2+h,  -1, 0,0,-1, colorD, u2,v2 );
   S3DVertex A4( x,   y-2+h,  -1, 0,0,-1, colorA, u1,v2 );
   o.addLine( D4, A4 );
}

void
S2DRoundRect::add( SMeshBuffer & o, Recti const & pos, glm::ivec2 const & radius,
      uint32_t outerColor, uint32_t innerColor, TexRef const & ref, int tess )
{
   int w = pos.getWidth();
   int h = pos.getHeight();
   if ( w < 3 || w > 10000 ) { DE_DEBUG( "Invalid w ",w ) return; }
   if ( h < 3 || h > 10000 ) { DE_DEBUG( "Invalid h ",h ) return; }

   o.setPrimitiveType( PrimitiveType::Triangles );
   o.setCulling( false );
   o.setBlend( Blend::alphaBlend() );
   if ( ref.tex )
   {
      o.setTexture( 0, ref );
   }

   glm::vec3 off( w/2 + 1 + pos.x, h/2 + pos.y, 0 );
   RoundRect::addXY( o, off, w, h, radius.x, radius.y, tess, innerColor, outerColor );
}

// Vertices = 8 + 4*(tess)
// Indices = 2 * Primitives
// Primitives = 4 + 4*(tess+1) lines
//
//       x0  x1  x2  x3
// ry    |   |   |   |
//       |
// y0 ---+   G---H
//        (         )     A,B,C,D are not used for the outline
//    ry (           )
//       (           )    E=0, F=1, G=2, H=3, I=4, J=5, K=6, L=7, n = 8
// y1 -  F   B   C   I
//       |           |     Lines: EF, GH, IJ, KL
//       |     M     |            + lineFG[tess] + lineHI[tess] + lineJK[tess] + lineLE[tess]
//       |           |
// y2 -  E   A   D   J        (CW) top-left = FGB
//       (           )        (CW) top-right = HIC
//        (         )         (CW) bottom-right = JKD
//         (       )          (CW) bottom-left = LEA
// y3 ---+   L---K
//
void
S2DRoundRectOutline::add( SMeshBuffer & o, Recti const & pos, glm::ivec2 const & radius,
      uint32_t color, TexRef const & ref, int borderWidth, int tess )
{
   int w = pos.getWidth();
   int h = pos.getHeight();
   if ( w < 4 || w > 1000000 ) { DE_DEBUG( "Invalid w ",w ) return; }
   if ( h < 4 || h > 1000000 ) { DE_DEBUG( "Invalid h ",h ) return; }
   o.setBlend( Blend::alphaBlend() );
   if ( ref.tex )
   {
      o.setTexture( 0, ref );
   }

   glm::vec2 off( w/2 + 1 + pos.x, h/2 + pos.y );
   float const rx = radius.x;
   float const ry = radius.y;
   float const x0 = -0.5f * w;
   float const x1 = -0.5f * w + rx;
   float const x2 = 0.5f * w - rx;
   float const x3 = 0.5f * w;
   float const y0 = -0.5f * h;
   float const y1 = -0.5f * h + ry;
   float const y2 = 0.5f * h - ry;
   float const y3 = 0.5f * h;

// [SinCosLookUpTable] for a quarter circle
   struct SinCos
   {
      float s,c;
      SinCos() : s(0.f), c(0.f) {}
      SinCos(float _s,float _c) : s(_s), c(_c) {}
   };
   std::vector< SinCos > lookUpTable;
   lookUpTable.reserve( tess );

   float phi_step = float( 0.5 * M_PI ) / float( tess+1 );
   float phi = phi_step; // phi_start > 0° and phi_end < 90°
   for ( uint32_t i = 0; i < tess; ++i )
   {
      lookUpTable.emplace_back( ::sinf( phi ), ::cosf( phi ) );
      phi += phi_step;
   }
// [SinCosLookUpTable] End.

   std::vector< glm::vec2 > points;

   auto addPoint = [&] ( float x, float y )
   {
      points.emplace_back( off.x + x, off.y + y );
   };

   if ( h > 2*radius.y )
   {
      addPoint( x0, y2 ); // E=0
      addPoint( x0, y1 ); // F=1
   }

   // 1. topLeft FGB (cw) quarter circle
   for ( uint32_t i = 0; i < tess; ++i )
   {
      float x = x1 - rx * lookUpTable[ i ].c;   // Vertices already in correct index order.
      float y = y1 - ry * lookUpTable[ i ].s;
      addPoint( x, y );
   }

   if ( w > 2*radius.x )
   {
      addPoint( x1, y0 ); // G=2
      addPoint( x2, y0 ); // H=3
   }

   // 2. topRight HIC (cw) quarter circle
   for ( int32_t i = tess - 1; i >= 0; --i )
   {
      float x = x2 + rx * lookUpTable[ i ].c;   // Add vertices in reverse (--i) order
      float y = y1 - ry * lookUpTable[ i ].s;   // to build indices in correct order.
      addPoint( x, y );
   }

   if ( h > 2*radius.y )
   {
      addPoint( x3, y1 ); // I=4
      addPoint( x3, y2 ); // J=5
   }


   // 3. bottomRight JKD (cw) quarter circle
   for ( uint32_t i = 0; i < tess; ++i )
   {
      float x = x2 + rx * lookUpTable[ i ].c;   // Vertices already in correct index order.
      float y = y2 + ry * lookUpTable[ i ].s;
      addPoint( x, y );
   }

   if ( w > 2*radius.x )
   {
      addPoint( x2, y3 ); // K=6
      addPoint( x1, y3 ); // L=7
   }

   // 4. bottomLeft LEA (cw) quarter circle
   for ( int32_t i = tess - 1; i >= 0; --i )
   {
      float x = x1 - rx * lookUpTable[ i ].c;   // Add vertices in reverse (--i) order
      float y = y2 + ry * lookUpTable[ i ].s;   // to build indices in correct order.
      addPoint( x, y );
   }

   addPoint( x0, y2 ); // E=0

   S2DPolyLine::addTriangleStrip( o, points, color, borderWidth, LineCap::None, LineJoin::Bevel );

   // Set color between black and white to all vertices depending on their u value.
   float uMax = o.Vertices.back().tex.x;
   for ( size_t i = 0; i < o.Vertices.size(); ++i )
   {
      float u = o.Vertices[ i ].tex.x;
      uint8_t r = clampByte( Math::round32( 255.0f * u / uMax ) );
      uint32_t color = RGBA( r,r,r );
      o.Vertices[ i ].color = color;
   }
}
/*
// Vertices = 8 + 4*(tess)
// Indices = 2 * Primitives
// Primitives = 4 + 4*(tess+1) lines
//
//       x0  x1  x2  x3
// ry    |   |   |   |
//       |
// y0 ---+   G---H
//        (         )     A,B,C,D are not used for the outline
//    ry (           )
//       (           )    E=0, F=1, G=2, H=3, I=4, J=5, K=6, L=7, n = 8
// y1 -  F   B   C   I
//       |           |     Lines: EF, GH, IJ, KL
//       |     M     |            + lineFG[tess] + lineHI[tess] + lineJK[tess] + lineLE[tess]
//       |           |
// y2 -  E   A   D   J        (CW) top-left = FGB
//       (           )        (CW) top-right = HIC
//        (         )         (CW) bottom-right = JKD
//         (       )          (CW) bottom-left = LEA
// y3 ---+   L---K
//
void
S2DRoundRectOutline::add( SMeshBuffer & o, Recti const & pos, glm::ivec2 const & radius,
      uint32_t color, TexRef const & ref, int borderWidth, int tess )
{
   S2DPolyLine::
   int w = pos.getWidth();
   int h = pos.getHeight();
   if ( w < 4 || w > 1000000 ) { DE_DEBUG( "Invalid w ",w ) return; }
   if ( h < 4 || h > 1000000 ) { DE_DEBUG( "Invalid h ",h ) return; }
   o.setPrimitiveType( PrimitiveType::Lines );
   o.setBlend( Blend::alphaBlend() );
   if ( ref.tex )
   {
      o.setTexture( 0, ref );
   }

   glm::vec3 off( w/2 + 1 + pos.x, h/2 + pos.y, 0.0f );
   //o.moreVertices( 8 + 4*tess );
   //o.moreIndices( 8 + 8*(tess+1) );
   float const rx = radius.x;
   float const ry = radius.y;
   float const x0 = -0.5f * w;
   float const x1 = -0.5f * w + rx;
   float const x2 = 0.5f * w - rx;
   float const x3 = 0.5f * w;
   float const y0 = -0.5f * h;
   float const y1 = -0.5f * h + ry;
   float const y2 = 0.5f * h - ry;
   float const y3 = 0.5f * h;

   auto addVertex = [&] ( float x, float y )
   {
      float uCoord = 0.5f + x / w;
      float vCoord = 0.5f + y / h;
      o.addVertex( S3DVertex( off.x + x, off.y + y,-1, 0,0,-1, color, uCoord,vCoord ) );   // input ABC (cw), output ACB (ccw)
   };

   addVertex( x0, y2 ); // E=0
   addVertex( x0, y1 ); // F=1
   addVertex( x1, y0 ); // G=2
   addVertex( x2, y0 ); // H=3
   addVertex( x3, y1 ); // I=4
   addVertex( x3, y2 ); // J=5
   addVertex( x2, y3 ); // K=6
   addVertex( x1, y3 ); // L=7

   uint32_t const INDEX_E = 0;
   uint32_t const INDEX_F = 1;
   uint32_t const INDEX_G = 2;
   uint32_t const INDEX_H = 3;
   uint32_t const INDEX_I = 4;
   uint32_t const INDEX_J = 5;
   uint32_t const INDEX_K = 6;
   uint32_t const INDEX_L = 7;
   uint32_t const INDEX_TOPLEFT = 8 + 0*tess;
   uint32_t const INDEX_TOPRIGHT = 8 + 1*tess;
   uint32_t const INDEX_BOTTOMRIGHT = 8 + 2*tess;
   uint32_t const INDEX_BOTTOMLEFT = 8 + 3*tess;
   o.addIndexedLine( INDEX_E, INDEX_F );
   o.addIndexedLine( INDEX_G, INDEX_H );
   o.addIndexedLine( INDEX_I, INDEX_J );
   o.addIndexedLine( INDEX_K, INDEX_L );
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
   float phi = phi_step; // phi_start > 0° and phi_end < 90°
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
      addVertex( x, y );
   }
   // 2. topRight HIC (cw) quarter circle
   for ( int32_t i = tess - 1; i >= 0; --i )
   {
      float x = x2 + rx * lookUpTable[ i ].c;   // Add vertices in reverse (--i) order
      float y = y1 - ry * lookUpTable[ i ].s;   // to build indices in correct order.
      addVertex( x, y );
   }
   // 3. bottomRight JKD (cw) quarter circle
   for ( uint32_t i = 0; i < tess; ++i )
   {
      float x = x2 + rx * lookUpTable[ i ].c;   // Vertices already in correct index order.
      float y = y2 + ry * lookUpTable[ i ].s;
      addVertex( x, y );
   }
   // 4. bottomLeft LEA (cw) quarter circle
   for ( int32_t i = tess - 1; i >= 0; --i )
   {
      float x = x1 - rx * lookUpTable[ i ].c;   // Add vertices in reverse (--i) order
      float y = y2 + ry * lookUpTable[ i ].s;   // to build indices in correct order.
      addVertex( x, y );
   }

   // add 4*tess CornerIndices:

   // 1. topLeft FGB (cw) quarter circle, glm::vec3 B( x1, 0.f, y2 );
   for ( uint32_t i = 0; i < tess; ++i )
   {
      uint32_t iF = INDEX_TOPLEFT + i;
      uint32_t iG = INDEX_TOPLEFT + i + 1;
      if ( i == 0 )             { iF = INDEX_F; } // StartTriangle
      else if ( i == tess - 1 ) { iG = INDEX_G; } // EndTriangle
      else                      {}                // MiddleTriangle
      o.addIndexedLine( iF, iG ); // all given in (cw)
   }

   // 2. topRight HIC (cw) quarter circle, glm::vec3 C( x2, 0.f, y2 );
   for ( uint32_t i = 0; i < tess; ++i )
   {
      uint32_t iH = INDEX_TOPRIGHT + i;
      uint32_t iI = INDEX_TOPRIGHT + i + 1;
      if ( i == 0 )             { iH = INDEX_H; } // StartTriangle
      else if ( i == tess - 1 ) { iI = INDEX_I; } // EndTriangle
      else                      {}                // MiddleTriangle
      o.addIndexedLine( iH,iI ); // all given in (cw)
   }

   // 3. bottomRight JKD (cw) quarter circle, glm::vec3 D( x2, 0.f, y1 );
   for ( uint32_t i = 0; i < tess; ++i )
   {
      uint32_t iJ = INDEX_BOTTOMRIGHT + i;
      uint32_t iK = INDEX_BOTTOMRIGHT + i + 1;
      if ( i == 0 )             { iJ = INDEX_J;  } // StartTriangle
      else if ( i == tess - 1 ) { iK = INDEX_K;  } // EndTriangle
      else                      {}                 // MiddleTriangle
      o.addIndexedLine( iJ, iK ); // all given in (cw)
   }

   // 4. bottomLeft LEA (cw) quarter circle, glm::vec3 A( x1, 0.f, y1 );
   for ( uint32_t i = 0; i < tess; ++i )
   {
      uint32_t iL = INDEX_BOTTOMLEFT + i;
      uint32_t iE = INDEX_BOTTOMLEFT + i + 1;
      if ( i == 0 )             { iL = INDEX_L; } // StartTriangle
      else if ( i == tess - 1 ) { iE = INDEX_E; } // EndTriangle
      else                      {}               // MiddleTriangle
      o.addIndexedLine( iL, iE ); // all given in (cw)
   }
}
*/

} // end namespace smesh.
} // end namespace gpu.
} // end namespace de.
