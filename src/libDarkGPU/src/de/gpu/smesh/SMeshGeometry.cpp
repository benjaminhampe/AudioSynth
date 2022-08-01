#include <de/gpu/smesh/Geometry.hpp>

/*
namespace de {
namespace gpu {
namespace mesh {
} // end namespace smesh.
} // end namespace gpu.
} // end namespace de.
void
Line::add( SMeshBuffer & o, S3DVertex a, S3DVertex b )
{
   o.addVertex( a );
   o.addVertex( b );
}

void
Line::add( SMeshBuffer & o, glm::vec3 a, glm::vec3 b, uint32_t color )
{
   o.addVertex( S3DVertex( a, glm::vec3(0,0,1), color, glm::vec2(0,0) ) );
   o.addVertex( S3DVertex( b, glm::vec3(0,0,1), color, glm::vec2(1,0) ) );
}

void
Line::add( SMeshBuffer & o, glm::vec3 a, glm::vec3 b, uint32_t colorA, uint32_t colorB )
{
   o.addVertex( S3DVertex( a, glm::vec3(0,0,1), colorA, glm::vec2(0,0) ) );
   o.addVertex( S3DVertex( b, glm::vec3(0,0,1), colorB, glm::vec2(1,0) ) );
}

// =========================================================================
/// @brief LineTriangle
// =========================================================================
//
//     B
//    /|
//   / |
//  /  |
// A---C
//
void
LineTriangle::add( SMeshBuffer & o, glm::vec3 a, glm::vec3 b, glm::vec3 c, uint32_t color )
{
   Line::add( o, a,b, color ); // AB
   Line::add( o, b,c, color ); // BC
   Line::add( o, c,a, color ); // CA
}

void
LineTriangle::add( SMeshBuffer & o, glm::vec3 a, glm::vec3 b, glm::vec3 c, uint32_t colorA, uint32_t colorB, uint32_t colorC )
{
   Line::add( o, a,b, colorA ); // AB
   Line::add( o, b,c, colorB ); // BC
   Line::add( o, c,a, colorC ); // CA
}

// =========================================================================
/// @brief LineBox
// =========================================================================
//
//     B
//    /
//   /
//  /
// A
//

void
LineBox::add( SMeshBuffer & o, Box3f const & box, uint32_t color )
{
   Line::add( o, box.getA(), box.getB(), color ); // AB
   Line::add( o, box.getB(), box.getC(), color ); // BC
   Line::add( o, box.getC(), box.getD(), color ); // CD
   Line::add( o, box.getD(), box.getA(), color ); // DA

   Line::add( o, box.getA(), box.getE(), color ); // AE
   Line::add( o, box.getB(), box.getF(), color ); // BF
   Line::add( o, box.getC(), box.getG(), color ); // CG
   Line::add( o, box.getD(), box.getH(), color ); // DH

   Line::add( o, box.getE(), box.getF(), color ); // EF
   Line::add( o, box.getF(), box.getG(), color ); // FG
   Line::add( o, box.getG(), box.getH(), color ); // GH
   Line::add( o, box.getH(), box.getE(), color ); // HE
}


// =========================================================================
/// @brief BKS Line mesh
// =========================================================================
SMeshBuffer
BKS::create( float x, float y, float z )
{
   SMeshBuffer o( PrimitiveType::Lines );
   float d = 1000.0f;
   glm::vec3 pos( x,y,z );
   SMeshBufferTool::addLine( o, pos, pos+glm::vec3( d,0,0 ), 0xFF0000FF );
   SMeshBufferTool::addLine( o, pos, pos+glm::vec3( 0,d,0 ), 0xFF00FF00 );
   SMeshBufferTool::addLine( o, pos, pos+glm::vec3( 0,0,d ), 0xFFFF0000 );

   //SMeshBufferTool::addLineBox( *o, Box3f( 0,0,0, d, d, d ), 0xFFFFFFFF );

   o.recalculateBoundingBox();
   return o;
}


// =========================================================================
/// @brief Triangle
// =========================================================================
//
// B---C
// |  /
// | /
// |/
// A
//

// static
void
Triangle::add( SMeshBuffer & o, S3DVertex a, S3DVertex b, S3DVertex c, bool recalcNormal )
{
   o.setPrimitiveType( PrimitiveType::Triangles );
   if ( recalcNormal )
   {
      glm::vec3 n = Math::getNormal3D( a.pos, b.pos, c.pos );
      a.normal = n;
      b.normal = n;
      c.normal = n;
   }
   o.addVertex( a );
   o.addVertex( c );
   o.addVertex( b );
}

// static
void
Triangle::addFront( SMeshBuffer & o, S3DVertex a, S3DVertex b, S3DVertex c )
{
   o.setPrimitiveType( PrimitiveType::Triangles );
   glm::vec3 n = Math::getNormal3D( a.pos, b.pos, c.pos );
   if ( glm::dot( n, glm::vec3( 0,0,1 ) ) < 0.0f )
   {
      a.normal = n;
      b.normal = n;
      c.normal = n;
      o.addVertex( a );
      o.addVertex( c );
      o.addVertex( b );
   }
   else
   {
      a.normal = -n;
      b.normal = -n;
      c.normal = -n;
      o.addVertex( a );
      o.addVertex( b );
      o.addVertex( c );
   }
}



// =========================================================================
/// @brief Quad
// =========================================================================
///
/// B---C           Bottom Quad LADK: LDA + LKD
/// |  /|
/// | M |
/// |/  |
/// A---D
///

// static
// [1.] Generic with 4 vertices.
void
Quad::add(
   SMeshBuffer & o,
   S3DVertex const & a,
   S3DVertex const & b,
   S3DVertex const & c,
   S3DVertex const & d )
{
   o.Vertices.reserve( o.Vertices.size() + 6 );
   Triangle::add( o,a,b,c );
   Triangle::add( o,a,c,d );
}

// static
// [2.] 4pos + 4color + 4uv
void
Quad::add(
   SMeshBuffer & o,
   glm::vec3 const & a,
   glm::vec3 const & b,
   glm::vec3 const & c,
   glm::vec3 const & d,
   uint32_t colorA, uint32_t colorB, uint32_t colorC, uint32_t colorD,
   glm::vec2 const & uvA,
   glm::vec2 const & uvB,
   glm::vec2 const & uvC,
   glm::vec2 const & uvD,
   bool recomputeNormal )
{
   glm::vec3 n( 0,0,1 );
   S3DVertex A( a, n, colorA, uvA ); // A
   S3DVertex B( b, n, colorB, uvB ); // B
   S3DVertex C( c, n, colorC, uvC ); // C
   S3DVertex D( d, n, colorD, uvD ); // D
   Triangle::add( o,A,B,C, recomputeNormal );
   Triangle::add( o,A,C,D, recomputeNormal );
}

// static
// [3.] 4pos + 1color + 2uv (start + end coords into a texture-atlas)
void
Quad::add(
   SMeshBuffer & o,
   glm::vec3 const & a,
   glm::vec3 const & b,
   glm::vec3 const & c,
   glm::vec3 const & d,
   uint32_t color,
   glm::vec2 const & uvStart,
   glm::vec2 const & uvEnde,
   bool recomputeNormal )
{
   add( o, a,b,c,d, color,color,color,color,
      glm::vec2( uvStart.x, uvEnde.y ),
      glm::vec2( uvStart.x, uvStart.y ),
      glm::vec2( uvEnde.x, uvStart.y ),
      glm::vec2( uvEnde.x, uvEnde.y ), recomputeNormal );
}

// static
// [4.] 4pos + 1color + 4uv
void
Quad::add(
   SMeshBuffer & o,
   glm::vec3 const & a,
   glm::vec3 const & b,
   glm::vec3 const & c,
   glm::vec3 const & d,
   uint32_t color,
   glm::vec2 const & uvA,
   glm::vec2 const & uvB,
   glm::vec2 const & uvC,
   glm::vec2 const & uvD,
   bool recomputeNormal )
{
   add( o, a,b,c,d,color,color,color,color,uvA,uvB,uvC,uvD, recomputeNormal );
}

void
Quad::addFront( SMeshBuffer & o, glm::vec3 a, glm::vec3 b,
                           glm::vec3 c, glm::vec3 d, uint32_t color )
{
   S3DVertex A( a.x, a.y, a.z, 0,0,1, color, 0,1 ); // A
   S3DVertex B( b.x, b.y, b.z, 0,0,1, color, 0,0 ); // B
   S3DVertex C( c.x, c.y, c.z, 0,0,1, color, 1,0 ); // C
   S3DVertex D( d.x, d.y, d.z, 0,0,1, color, 1,1 ); // D
   Triangle::addFront( o, A,B,C );
   Triangle::addFront( o, A,C,D );
}

SMeshBuffer
Quad::create( S3DVertex a, S3DVertex b, S3DVertex c, S3DVertex d )
{
   SMeshBuffer o( PrimitiveType::Triangles );
   add( o, a, b, c, d );
   o.recalculateBoundingBox();
   return o;
}


// by 4 points.
SMeshBuffer
Quad::create( glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, uint32_t color )
{
   SMeshBuffer o( PrimitiveType::Triangles );
   add( o, a, b, c, d, color );
   o.recalculateBoundingBox();
   return o;
}

// [5.] XY-Plane (w,h) centered at (x,y,z), 2 uv coords ( start + end into tex-atlas )
// static
void
Quad::addXY(
   SMeshBuffer & o,
   float w, float h,
   uint32_t color,
   float ox, float oy, float oz,
   float u1, float v1,
   float u2, float v2,
   bool recomputeNormal )
{
   // glm::vec3 n = Math::getNormal( a,b,c );
   float a = 0.5f * w;
   float b = 0.5f * h;
   S3DVertex A( ox-a, oy-b, oz, 0,0,-1, color, u1,v2 );
   S3DVertex B( ox-a, oy+b, oz, 0,0,-1, color, u1,v1 );
   S3DVertex C( ox+a, oy+b, oz, 0,0,-1, color, u2,v1 );
   S3DVertex D( ox+a, oy-b, oz, 0,0,-1, color, u2,v2 );
   Triangle::add( o,A,B,C, recomputeNormal );
   Triangle::add( o,A,C,D, recomputeNormal );
   // add( o, a, b, c, d, indexed );
}

SMeshBuffer
Quad::createXY( float w, float h, uint32_t color,
                         float ox, float oy, float oz )
{
   SMeshBuffer o( PrimitiveType::Triangles );
   addXY( o, w, h, color, ox, oy, oz );
   o.recalculateBoundingBox();
   return o;
}


// [6.] XZ-Plane (w,h) centered at (x,y,z), 2 uv coords ( start + end into tex-atlas )
// static
void
Quad::addXZ(
   SMeshBuffer & o,
   float w, float h,
   uint32_t color,
   float ox, float oy, float oz,
   float u1, float v1,
   float u2, float v2,
   bool recomputeNormal )
{
   // glm::vec3 n = Math::getNormal( a,b,c );
   float a = 0.5f * w;
   float b = 0.5f * h;
   S3DVertex A( ox-a, oy, oz-b, 0,1,0, color, u1,v2 );
   S3DVertex B( ox-a, oy, oz+b, 0,1,0, color, u1,v1 );
   S3DVertex C( ox+a, oy, oz+b, 0,1,0, color, u2,v1 );
   S3DVertex D( ox+a, oy, oz-b, 0,1,0, color, u2,v2 );
   Triangle::add( o,A,B,C, recomputeNormal );
   Triangle::add( o,A,C,D, recomputeNormal );
   // add( o, a, b, c, d, indexed );
}


// =========================================================================
/// @brief RoundRect
// =========================================================================
///
/// B---C           Bottom Quad LADK: LDA + LKD
/// |  /|
/// | M |
/// |/  |
/// A---D
///

// static
SMeshBuffer*
RoundRect::create( float w, float h, float rx, float ry, int32_t tesselation,
                             ITexture* tex, uint32_t color,
                             float ox, float oy, float oz )
{
   SMeshBuffer* o = SMeshBufferTool::createTriangles( tex );
   add( *o, w, h, rx, ry, tesselation, color, color, ox, oy, oz );
   // o.recalculateBoundingBox();
   return o;
}

// static
void
RoundRect::add( SMeshBuffer & o,
                  float w, float h,
                  float rx, float ry,
                  int32_t tesselation,
                  uint32_t innerColor,
                  uint32_t outerColor,
                  float ox,
                  float oy,
                  float oz )
{
   //moreVertices( 12 + 4 * tesselation );
   //moreIndices( 30 + 4 * 3 * ( tesselation + 1 ) );

   std::vector< S3DVertex > vertices;
   vertices.reserve( (4*tesselation + 20) * 3 );

//   auto addVertex = [&] ( float x, float y, float u, float v, uint32_t color )
//   {
//      vertices.push_back( S3DVertex( ox + x,oy + y,oz, 0,0,1, color, u,v ) );
//   };

//   auto addIndexedQuad = [&] ( uint32_t iA, uint32_t iB, uint32_t iC, uint32_t iD )
//   {
//      Quad::add( o, vertices[ iA ],vertices[ iB ],vertices[ iC ],vertices[ iD ] );
//   };

     auto addTriangle = [&] ( S3DV, )

Triangle::add( o, vertices[ INDEX_C ], c,d );

   // vertices = 12 + 4*tesselation
   ///
   /// +> rx G---H         CCW:  TopQuad     BGHC(1,6,7,2)
   /// v     |  /|               CenterQuad  EFIJ(4,5,8,9)
   /// ry    | / |               BottomQuad  LADK(11,0,3,10)
   ///       |/  |
   ///   F---B---C---I
   ///   |  /|  /|  /|
   ///   | / | M | / |     A=0, B=1, C=2, D=3, E=4, F=5, G=6, H=7, I=8, J=9, K=10, L=11
   ///   |/  |/  |/  |
   ///   E---A---D---J     End-Triangles CCW: south-west = LAE (11,0,4)
   ///       |  /|                            north-west = FBG (5,1,6)
   ///       | / |                            north-east = HCI (7,2,8)
   ///       |/  |                            south-east = JDK (9,3,10)
   ///       L---K
   ///
   float const x0 = -0.5f * w;
   float const x1 = -0.5f * w + rx;
   float const x2 =  0.5f * w - rx;
   float const x3 =  0.5f * w;
   float const y0 = -0.5f * h;
   float const y1 = -0.5f * h + ry;
   float const y2 =  0.5f * h - ry;
   float const y3 =  0.5f * h;
   float const inv_w = 1.0f / w;
   float const inv_h = 1.0f / h;
   float const u0 = 0.0f;
   float const u1 = rx * inv_w;
   float const u2 = ( w - rx ) * inv_w;
   float const u3 = 1.0f;
   float const v0 = 1.0f;
   float const v1 = 1.0f - ry * inv_h;
   float const v2 = 1.0f - ( h - ry ) * inv_h;
   float const v3 = 0.0f;

   addVertex( x1, y1, u1, v2, innerColor ); // A=0
   addVertex( x1, y2, u1, v1, innerColor ); // B=1
   addVertex( x2, y2, u2, v1, innerColor ); // C=2
   addVertex( x2, y1, u2, v2, innerColor ); // D=3
   addVertex( x0, y1, u0, v2, outerColor ); // E=4
   addVertex( x0, y2, u0, v1, outerColor ); // F=5
   addVertex( x1, y3, u1, v0, outerColor ); // G=6
   addVertex( x2, y3, u2, v0, outerColor ); // H=7
   addVertex( x3, y2, u3, v1, outerColor ); // I=8
   addVertex( x3, y1, u3, v2, outerColor ); // J=9
   addVertex( x2, y0, u2, v3, outerColor ); // K=A
   addVertex( x1, y0, u1, v3, outerColor ); // L=B

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
   addIndexedQuad( INDEX_B, INDEX_G, INDEX_H, INDEX_C ); // Top BGHC
   addIndexedQuad( INDEX_E, INDEX_F, INDEX_B, INDEX_A ); // Center EFBA
   addIndexedQuad( INDEX_A, INDEX_B, INDEX_C, INDEX_D ); // Center ABCD
   addIndexedQuad( INDEX_D, INDEX_C, INDEX_I, INDEX_J ); // Center DCIJ
   addIndexedQuad( INDEX_L, INDEX_A, INDEX_D, INDEX_K ); // Bottom LADK

   struct SinCos { float s,c,u,v; };
   std::vector< SinCos > lookUpTable;
   lookUpTable.reserve( tesselation + 1 );
   float phi_step = float( 0.5 * M_PI ) / float( tesselation-1 );
   for ( uint32_t i = 0; i <= tesselation; ++i )
   {
      float const phi = phi_step * float( i );
      SinCos item;
      item.s = ::sinf( phi );
      item.c = ::cosf( phi );
      item.u = item.s * rx / w;
      item.v = item.c * ry / h;
      lookUpTable.emplace_back( std::move( item ) );
   }

   for ( uint32_t i = 1; i < tesselation; ++i )
   {
      SinCos const & l1 = lookUpTable[ i-1 ];
      SinCos const & l2 = lookUpTable[ i ];

      // 1. topLeft BGF quarter circle, glm::vec3 B( x1, 0.f, y2 );
      float ax = x1 - rx * l1.s;
      float ay = y2 + ry * l1.c;
      float au = u1 - rx * l1.s * inv_w;
      float av = v1 + ry * l1.c * inv_h;
      S3DVertex a( ox + ax, oy + ay, oz, 0,0,1, outerColor, au, av );
      float bx = x1 - rx * l2.s;
      float by = y2 + ry * l2.c;
      float bu = u1 - rx * l2.s * inv_w;
      float bv = v1 + ry * l2.c * inv_h;
      S3DVertex b( ox + bx, oy + by, oz, 0,0,1, outerColor, bu, bv );
      Triangle::add( o, vertices[ INDEX_B ], a,b );

      // 2. topRight CHK CIH quarter circle, glm::vec3 C( x2, 0.f, y2 );
      float cx = x2 + rx * l1.s;
      float cy = y2 + ry * l1.c;
      float cu = u2 + rx * l1.s * inv_w;
      float cv = v1 + ry * l1.c * inv_h;
      S3DVertex c( ox + cx, oy + cy, oz, 0,0,1, outerColor, cu, cv );
      float dx = x2 + rx * l2.s;
      float dy = y2 + ry * l2.c;
      float du = u2 + rx * l2.s * inv_w;
      float dv = v1 + ry * l2.c * inv_h;
      S3DVertex d( ox + dx, oy + dy, oz, 0,0,1, outerColor, du, dv );
      Triangle::add( o, vertices[ INDEX_C ], c,d );

      // 3. bottomRight DLF DKJ quarter circle, glm::vec3 D( x2, 0.f, y1 );
      float ex = x2 + rx * l1.s;
      float ey = y1 - ry * l1.c;
      float eu = u2 + rx * l1.s * inv_w;
      float ev = v2 - ry * l1.c * inv_h;
      S3DVertex e( ox + ex, oy + ey, oz, 0,0,1, outerColor, eu, ev );
      float fx = x2 + rx * l2.s;
      float fy = y1 - ry * l2.c;
      float fu = u2 + rx * l2.s * inv_w;
      float fv = v2 - ry * l1.c * inv_h;
      S3DVertex f( ox + fx, oy + fy, oz, 0,0,1, outerColor, fu, fv );
      Triangle::add( o, vertices[ INDEX_D ], e,f );

      // 4. bottomLeft AEL quarter circle, glm::vec3 A( x1, 0.f, y1 );
      float gx = x1 - rx * l1.s;
      float gy = y1 - ry * l1.c;
      float gu = u1 - rx * l1.s * inv_w;
      float gv = v2 - ry * l1.c * inv_h;
      S3DVertex g( ox + gx, oy + gy, oz, 0,0,1, outerColor, gu, gv );
      float hx = x1 - rx * l2.s;
      float hy = y1 - ry * l2.c;
      float hu = u1 - rx * l2.s * inv_w;
      float hv = v2 - ry * l2.c * inv_h;
      S3DVertex h( ox + hx, oy + hy, oz, 0,0,1, outerColor, hu, hv );
      Triangle::add( o, vertices[ INDEX_A ], g,h );

   }

}

// =========================================================================
/// @brief Circle/Ellipse
// =========================================================================

std::vector< S3DVertex >
Circle::createVerticesXY( int tess, float w, float h,
                                   uint32_t color,
                                   float ox, float oy, float oz )
 {
   std::vector< S3DVertex > vertices;
   vertices.reserve( tess + 2 );

   S3DVertex M( ox, oy, oz, 0,0,1, color, 0.5f, 0.5f );
   vertices.emplace_back( M );

   float const angleStep = float( ( 2.0 * M_PI ) / double( tess ) );
   for ( uint32_t i = 0; i <= tess; ++i )
   {
      float const phi = angleStep * float( i );
      float const s = ::sinf( phi );
      float const c = ::cosf( phi );
      float const x = -0.5f*w * s;   // uses sin()
      float const y =  0.5f*h * c;    // uses cos()
      float const u =  0.5f - 0.5f * s; // uses sin()
      float const v =  0.5f - 0.5f * c; // uses cos()
      S3DVertex V( ox + x, oy + y, oz, 0,0,1, color, u, v );
      vertices.emplace_back( V );
   }

   return vertices;
}

void
Circle::addXY( SMeshBuffer & o, int tess,
                        float w, float h, uint32_t color,
                        float ox, float oy, float oz )
{
   std::vector< S3DVertex > vertices = createVerticesXY( tess, w, h, color, ox, oy, oz );

   for ( uint32_t i = 2; i < vertices.size(); ++i )
   {
      S3DVertex const & M = vertices[ 0 ];
      S3DVertex const & A = vertices[ i-1 ];
      S3DVertex const & B = vertices[ i ];
      o.addTriangle( M, A, B );
   }
}

std::vector< S3DVertex >
Circle::createVerticesXZ( int tess, float w, float h,
                                   uint32_t color,
                                   float ox, float oy, float oz )
 {
   std::vector< S3DVertex > vertices;
   vertices.reserve( tess + 2 );

   S3DVertex M( ox, oy, oz, 0,1,0, color, 0.5f, 0.5f );
   vertices.emplace_back( M );

   float const angleStep = float( ( 2.0 * M_PI ) / double( tess ) );
   for ( uint32_t i = 0; i <= tess; ++i )
   {
      float const phi = angleStep * float( i );
      float const s = ::sinf( phi );
      float const c = ::cosf( phi );
      float const x = -0.5f*w * s;   // uses sin()
      float const z =  0.5f*h * c;    // uses cos()
      float const u =  0.5f - 0.5f * s; // uses sin()
      float const v =  0.5f - 0.5f * c; // uses cos()
      S3DVertex V( ox + x, oy, oz + z, 0,1,0, color, u, v );
      vertices.emplace_back( V );
   }

   return vertices;
}

void
Circle::addXZ( SMeshBuffer & o, int tess,
                        float w, float h, uint32_t color,
                        float ox, float oy, float oz )
{
   std::vector< S3DVertex > vertices = createVerticesXZ( tess, w, h, color, ox, oy, oz );

   for ( uint32_t i = 2; i < vertices.size(); ++i )
   {
      S3DVertex const & M = vertices[ 0 ];
      S3DVertex const & A = vertices[ i-1 ];
      S3DVertex const & B = vertices[ i ];
      o.addTriangle( M, A, B );
   }
}

SMeshBuffer*
SMeshBufferEllipse::create( int tess, float w, float h,
      float ox, float oy, float oz )
{
   SMeshBuffer* o = createSolidTriangleBuffer( tex );

   auto addVertex = [&] ( float x, float y, float z, float u, float v )
   {
      o->addVertex( S3DVertex( ox + x, oy + y, oz + z, 0,0,1, 0xFFFFFFFF, u,v ) );
   };

   auto addIndexedTriangleCW = [&] ( uint32_t iA, uint32_t iB, uint32_t iC )
   {
      o->addIndexedTriangle( iA, iB, iC );
   };

   uint32_t const v0 = o->getVertexCount();
   // o.moreVertices( tess + 1 );
   // o.moreIndices( 3 * tess );

   // Add center vertex.
   addVertex( 0.f, 0.f, -0.78f, 0.5f, 0.5f );

   // Add radial vertices and connect them by indexed triangles
   float const angleStep = float( ( 2.0 * M_PI ) / double( tess ) );

   for ( uint32_t i = 0; i < tess; ++i )
   {
      float const phi = angleStep * float( i );
      float const s = ::sinf( phi );
      float const c = ::cosf( phi );
      float const x = -0.5f*w * s;   // uses sin()
      float const y = 0.5f*h * c;    // uses cos()
      float const u = 0.5f - 0.5f * s; // uses sin()
      float const v = 0.5f - 0.5f * c; // uses cos()
      addVertex( x, y, 0.0f, u, v );

      uint32_t j = i + 2;

      if ( j >= tess + 1 )
      {
         j = 1;
      }

      addIndexedTriangleCW( v0, v0 + j, v0 + i + 1 );
   }

   o->recalculateBoundingBox();
   return o;
}



// =========================================================================
/// @brief Ring
// =========================================================================
// static
SMeshBuffer
Ring::create(float outer_x, float outer_y,
                        float inner_x, float inner_y, uint32_t tess,
                        float ox, float oy, float oz )
{
   SMeshBuffer o( PrimitiveType::Triangles );
   add( o, outer_x, outer_y, inner_x, inner_y, tess, ox, oy, oz );
   o.recalculateBoundingBox();
   return o;
}

/// @brief Add geometry to meshbuffer ( asIndexedTriangles )
/// @param [o] o The output meshbuffer
/// @param [i] rr The input ring
/// @param [i] offset Another position offset to render same roundrect at different positions.
// static
void
Ring::add( SMeshBuffer & o,
                     float outer_x, float outer_y,
                     float inner_x, float inner_y,
                     uint32_t tess,
                     float ox, float oy, float oz )
{
   auto addVertex = [&] ( float x, float y, float u, float v )
   {
      o.addVertex( S3DVertex( ox+x, oy+y, oz, 0,0,1, 0xFFFFFFFF, u,v ) );
   };

   // Ask for more memory
   uint32_t const v = o.getVertexCount();
   o.moreVertices( 2 * ( tess + 1 ) );
   o.moreIndices( 6 * tess );

   // ring data
   float const & ao = 0.5f * outer_x; // a,b are radia, not sizes.
   float const & bo = 0.5f * outer_y;
   float const & ai = 0.5f * inner_x;
   float const & bi = 0.5f * inner_y;
   float const a_ratio = ai / ao; // ratio in x dir ( ellipse a )
   float const b_ratio = bi / bo; // ratio in y dir ( ellipse b )
   float const step = float( ( 2.0 * M_PI ) / double( tess ) ); // 0 to 2pi
   // ring inner vertices
   for ( uint32_t i = 0; i <= tess; ++i )
   {
      float const phi = step * float( i );
      float const s = -::sinf( phi );
      float const c =  ::cosf( phi );
      addVertex( ai * s,
                 bi * c,
                 .5f + .5f * a_ratio * s,
                 .5f - .5f * b_ratio * c ); // x,y,u,v
   }

   // ring outer vertices
   for ( uint32_t i = 0; i <= tess; ++i )
   {
      float const phi = step * float( i );
      float const s = -::sinf( phi );
      float const c =  ::cosf( phi );
      addVertex( ao * s,
                 bo * c,
                 .5f + .5f * s,
                 .5f - .5f * c ); // x,y,u,v
   }

   // ring indices
   for ( uint32_t i = 0; i < tess; ++i )
   {
      uint32_t A = v + i;
      uint32_t B = v + i + tess + 1;
      uint32_t C = v + i + tess + 2;
      uint32_t D = v + i + 1;
      o.addIndexedQuad( B, A, D, C );
   }

}

// =========================================================================
/// @brief Quad/Rectangle/Box
// =========================================================================
///
///        F-------G
///       /|      /|
///      / |     / |
///     /  |    /  |
///    /   E---/---H
///   /   /   /   /
///  /   /   /   /
/// B-------C   /        Bottom Quad LADK: LDA + LKD
/// |  /    |  /
/// | /     | /
/// |/      |/
/// A-------D
///
SMeshBuffer
Cube::create( float dx, float dy, float dz, uint32_t color )
{
   SMeshBuffer o( PrimitiveType::Triangles );
   add( o, dx, dy, dz, color );
   return o;
}

SMeshBuffer
Cube::createPlaneNX( float dx, float dy, float dz, uint32_t color  )
{
   SMeshBuffer o( PrimitiveType::Triangles );
   addPlaneNX( o, V3{dx, dy, dz} * 0.5f, color );
   return o;
}

SMeshBuffer
Cube::createPlaneNY( float dx, float dy, float dz, uint32_t color  )
{
   SMeshBuffer o( PrimitiveType::Triangles );
   addPlaneNY( o, V3{dx, dy, dz} * 0.5f, color );
   return o;
}

SMeshBuffer
Cube::createPlaneNZ( float dx, float dy, float dz, uint32_t color  )
{
   SMeshBuffer o( PrimitiveType::Triangles );
   addPlaneNZ( o, V3{dx, dy, dz} * 0.5f, color );
   return o;
}

SMeshBuffer
Cube::createPlanePX( float dx, float dy, float dz, uint32_t color  )
{
   SMeshBuffer o( PrimitiveType::Triangles );
   addPlanePX( o, V3{dx, dy, dz} * 0.5f, color );
   return o;
}

SMeshBuffer
Cube::createPlanePY( float dx, float dy, float dz, uint32_t color  )
{
   SMeshBuffer o( PrimitiveType::Triangles );
   addPlanePY( o, V3{dx, dy, dz} * 0.5f, color );
   return o;
}

SMeshBuffer
Cube::createPlanePZ( float dx, float dy, float dz, uint32_t color )
{
   SMeshBuffer o( PrimitiveType::Triangles );
   addPlanePZ( o, V3{dx, dy, dz} * 0.5f, color );
   return o;
}


// =========================================================================
/// @brief SkyboxMesh
// =========================================================================
SMesh*
SkyBox::create( float dx, float dy, float dz,
            ITexture* back, ITexture* front, ITexture* left,
            ITexture* right, ITexture* bottom, ITexture* top, PrimitiveType::EType primType )
{
   SMesh* mesh = new SMesh( "SkyBox" );
   typedef glm::vec3 V3;
   V3 size = V3( dx, dy, dz ) * 0.5f;
   V3 A = V3(-1,-1, 1 ) * size;
   V3 B = V3(-1, 1, 1 ) * size;
   V3 C = V3( 1, 1, 1 ) * size;
   V3 D = V3( 1,-1, 1 ) * size;
   V3 E = V3(-1,-1,-1 ) * size;
   V3 F = V3(-1, 1,-1 ) * size;
   V3 G = V3( 1, 1,-1 ) * size;
   V3 H = V3( 1,-1,-1 ) * size;

   SMeshBuffer buffer;
   // back
   Quad::add( buffer, B, A, D, C );
   buffer.setTexture( 0, back );
   mesh->addMeshBuffer( buffer );
   // front
   Quad::add( buffer, G, H, E, F );
   buffer.setTexture( 0, front );
   mesh->addMeshBuffer( buffer );
   // left
   Quad::add( buffer, F, E, A, B );
   buffer.setTexture( 0, left );
   mesh->addMeshBuffer( buffer );
   // right
   Quad::add( buffer, C, D, H, G );
   buffer.setTexture( 0, right );
   mesh->addMeshBuffer( buffer );
   // bottom
   Quad::add( buffer, H, D, A, E );
   buffer.setTexture( 0, bottom );
   mesh->addMeshBuffer( buffer );
   // top
   Quad::add( buffer, B, C, G, F );
   buffer.setTexture( 0, top );
   mesh->addMeshBuffer( buffer );
   mesh->recalculateBoundingBox();
   return mesh;
}

// =========================================================================
/// @brief The Hexagon ( ver_2018 ):
// =========================================================================
///
///               M| x=0 | y=0 | z=0 | u=.5| v=.5|
///       D       -|-----|-----|-----|-----|-----|
///      / \      A|   0 |  0  | -.5 | .5  | 0   |
///   C /   \ E   B| -.5 |  0  |-.25 |  0  | 0.25|
///    |-----|    C| -.5 |  0  | .25 |  0  | 0.75|
///    |  M  |    D| 0.0 |  0  |  .5 |  .5 | 1   |
///    |-----|    E| 0.5 |  0  | .25 |  1  | 0.75|
///   B \   / F   F| 0.5 |  0  |-.25 |  1  | 0.25|
///      \ /
///       A       triangles: ABF, BCE, BEF, CDE
///
// static
int32_t
PioHexagon::safeMod( int32_t a, int32_t b )
{
    if ( a == 0 || b == 0 ) { return 0; }
    return std::abs( a ) % std::abs( b );
}

// static
float
PioHexagon::computeBoardPosX( float w, float h, int i, int j )
{
   float x = 0.5f * float( safeMod( j, 2 ) == 1 ) + float( i );
   return x * w;
}

// static
float
PioHexagon::computeBoardPosY( float w, float h, int i, int j )
{
   float y = 3.0f/4.0f * float( j );
   return y * h;
}

//// static
//SMeshBuffer*
//PioHexagon::create( float w, float h, uint32_t color )
//{
//   SMeshBuffer* o = SMeshBufferTool::createTriangles();
//   S3DVertex const A( 0.0f,    -.5f*h, 0.0f, 0,0,1, color, 0.5f, 1.0f  );
//   S3DVertex const B( -0.5f*w,-.25f*h, 0.0f, 0,0,1, color, 0.0f, 0.75f );
//   S3DVertex const C( -0.5f*w, .25f*h, 0.0f, 0,0,1, color, 0.0f, 0.25f );
//   S3DVertex const D( 0.0f,     .5f*h, 0.0f, 0,0,1, color, 0.5f, 0.0f  );
//   S3DVertex const E( 0.5f*w,  .25f*h, 0.0f, 0,0,1, color, 1.0f, 0.25f );
//   S3DVertex const F( 0.5f*w, -.25f*h, 0.0f, 0,0,1, color, 1.0f, 0.75f );
//   Triangle::addFront( *o, B, A, F );
//   Triangle::addFront( *o, B, F, E );
//   Triangle::addFront( *o, B, E, C );
//   Triangle::addFront( *o, C, E, D );
//   o.recalculateBoundingBox();
//   return o;
//}

// =========================================================================
/// @brief Home = Box + 2 roof quads + 2 roof triangles.
// =========================================================================
void
PioneerHome::add( SMesh & out, float dx, float dy, float dz, uint32_t color )
{
   out.setName( "PioHouse" );

   SMeshBuffer walls;
   Cube::add( walls, dx, dy * 0.6f, dz, color );
   SMeshBufferTool::translateVertices( walls, 0,dy * 0.3f, 0.0f );
   out.addMeshBuffer( walls );

   SMeshBuffer roof1;
   Cube::add( roof1, 0.7*dx, 0.1*dy, dz, color );
   SMeshBufferTool::rotateVertices( roof1, 0,0,45 );
   SMeshBufferTool::translateVertices( roof1, -dx * 0.25f, dy * 0.8f, 0.0f );
   out.addMeshBuffer( roof1 );

   SMeshBuffer roof2;
   Cube::add( roof2, 0.7*dx, 0.1*dy, dz, color );
   SMeshBufferTool::rotateVertices( roof2, 0,0,-45 );
   SMeshBufferTool::translateVertices( roof2, dx * 0.25f, dy * 0.8f, 0.0f);
   out.addMeshBuffer( roof2 );
}

// =========================================================================
/// @brief City = Box + House(s)
// =========================================================================
void
PioneerCity::add( SMesh & out, float dx, float dy, float dz, uint32_t color )
{
   out.setName( "PioCity" );

   SMeshBuffer ground;
   Cube::add( ground, dx, 0.5f * dy, dz, color );
   SMeshBufferTool::translateVertices( ground, 0, 0.25f*dy, 0 );
   out.addMeshBuffer( ground );

   SMesh house;
   PioneerHome::add( house, 0.5f * dx, 0.5f * dy, dz, color );
   //house->rotateVertices( 0, 20, 0 );
   SMeshTool::translateVertices( house, 0.25f * dx, 0.5f*dy, 0 );
   out.addMesh( house );
}

// =========================================================================
/// @brief PolyLine
// =========================================================================
// static
void
PolyLine::add( SMeshBuffer & o, std::vector< glm::vec3 > const & points, float lineWidth, uint32_t color )
{
   if ( points.size() < 2 )
   {
      return;
   }

   auto addSegment = [&] ( glm::vec3 a, glm::vec3 b )
   {
      glm::vec2 m = Math::getNormal2D( glm::vec2( b ) - glm::vec2( a ) );
      glm::vec3 n( 0.5f * lineWidth * m.x, 0.5f * lineWidth * m.y, 0.0f );
      Quad::add( o, a - n, b - n, b + n, a + n, color  );
   };

   glm::vec3 a = points[ 0 ];
   for ( size_t i = 1; i < points.size(); ++i )
   {
      glm::vec3 b = points[ i ];
      addSegment( a, b );
      a = b;
   }
}

// =========================================================================
/// @brief PolyHull
// =========================================================================
// static
void
PolyHull::addLeftStripXY( SMeshBuffer & o, std::vector< glm::vec3 > const & points, float height, uint32_t color )
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

void
PolyHull::addLeftStripXZ( SMeshBuffer & o, std::vector< glm::vec3 > const & points, float height, uint32_t color )
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


// static
void
PolyHull::addRightStrip( SMeshBuffer & o, std::vector< glm::vec3 > const & points, float height, uint32_t color )
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
















































SMeshBuffer
Sphere::create( float rx, float ry, float rz, int32_t tess_u,
                        int32_t tess_v, uint32_t color )
{
   SMeshBuffer o( PrimitiveType::Triangles );
   o.Material.setLighting( true );
   o.Material.setCulling( false );
   add( o, rx, ry, rz, tess_u, tess_v, color );
   // SMeshBufferTool::computeNormals( o );
   o.recalculateBoundingBox();
   return o;
}

S3DVertex
Sphere::computeVertex( float rx, float ry, float rz,
                       int32_t i, int32_t j,
                       int32_t tess_u, int32_t tess_v, uint32_t color )
{
   float const UV_MIN = 0.000001f;
   float const UV_MAX = 1.0f - UV_MIN;
   float const fa = float( 2.0 * M_PI ) / float( tess_u );
   float const fb = float( M_PI ) / float( tess_v );

   float a = fa * float( i );
   float b = fb * float( j );
   float sa = std::sin( a );
   float ca = std::cos( a );
   float sb = std::sin( b );
   float cb = std::cos( b );
   float nx = ca * sb;
   float ny = cb;
   float nz = sa * sb;
   float u = glm::clamp( Math::fPart(.75f - .5f*ca * sb) , UV_MIN, UV_MAX );
   float v = glm::clamp( .5f - .5f*cb, UV_MIN, UV_MAX );
   return S3DVertex( nx*rx, ny*ry, nz*rz, nx,ny,nz, color, u, v );

//   float const wa = float( 2.0 * M_PI ) / float( tessCircle ); // azimutale angle between r_xz and +z axis [0°..360°]
//   float const wb = float( 0.5 * M_PI ) / float( tessRows );  // polar angle between r_xz and +y axis [0°..90°] ( begin at top )
//   uint32_t const k = getVertexCount();
//   float const UV_MIN = 0.000001f;
//   float const UV_MAX = 1.0f - UV_MIN;
//   float const sinA = ::sinf( wa * float( i ) );
//   float const cosA = ::cosf( wa * float( i ) );
//   float const sinB = ::sinf( wb * float( j ) );
//   float const cosB = ::cosf( wb * float( j ) );
//   float const x = -sinA * cosB;				// equals the normal at point radius*(x,y,z)
//   float const y =  cosA * cosB;				// equals the normal at point radius*(x,y,z)
//   float const z =  sinB;					// equals the normal at point radius*(x,y,z)
//   float const u = glm::clamp( .5f - .5f*cosB * sinA, UV_MIN, UV_MAX );
//   float const v = glm::clamp( .5f - .5f*cosB * cosA, UV_MIN, UV_MAX );
//   addVertex( radius*x, radius*y, radius*z, u, v, x, y, z );

//   float const du = float( 2.0 * M_PI );
//   float const dv = float( M_PI );
   //float const p90 = 0.5f * float( M_PI );
//   float us = std::sin( du * u );
//   float uc = std::cos( du * u );
//   float vs = std::sin( dv * v );
//   float vc = std::cos( dv * v );
//   glm::vec3 n = glm::vec3( uc * vs, vc, us * vs);

//   float su = std::sin( du * u );
//   float cu = std::cos( du * u );
//   float sv = std::sin( dv * v );
//   float cv = std::cos( dv * v );

//   float nx = -su * cv;				// equals the normal at point radius*(x,y,z)
//   float ny =  cu * cv;				// equals the normal at point radius*(x,y,z)
//   float nz =  sv;					// equals the normal at point radius*(x,y,z)

//   float nx = cu * sv;
//   float ny = cv;
//   float nz = su * sv;


//   u += 0.25f;
//   if ( u >= 1.0f ) u -= 1.0f;
//   return S3DVertex( nx*r,ny*r,nz*r, nx,ny,nz, color, u, v );
}

void
Sphere::add( SMeshBuffer & o,
             float rx, float ry, float rz, uint32_t color,
             int32_t tess_u, int32_t tess_v )
{
   if( tess_u < 3 ) { tess_u = 3; }
   if( tess_v < 3 ) { tess_v = 3; }

   for ( int32_t j = 0; j < tess_v; ++j )
   {
      for ( int32_t i = 0; i < tess_u; ++i )
      {
         S3DVertex A = computeVertex( rx,ry,rz, i,j,tess_u,tess_v, color );
         S3DVertex B = computeVertex( rx,ry,rz, i,j+1,tess_u,tess_v, color );
         S3DVertex C = computeVertex( rx,ry,rz, i+1,j+1,tess_u,tess_v, color );
         S3DVertex D = computeVertex( rx,ry,rz, i+1,j,tess_u,tess_v, color );
         Quad::add( o, A, B, C, D );
      }
   }
}




SMeshBuffer
Sphere::createUpperHalf( float rx, float ry, float rz,
                        uint32_t tessCircle, uint32_t tessRows,
                        uint32_t color )
{
   SMeshBuffer o( PrimitiveType::Triangles );

   // bbox
   o.BoundingBox.setMin( -rx, 0, -rz );
   o.BoundingBox.setMax( rx, ry, rz);

   float const sxz = 1.f / (float)tessCircle;
   float const sy = 1.f / (float)tessRows;

   // tables
   SinCosTablef sincosA(tessCircle);
   SinCosTablef sincosB(tessRows, float(0.0), float( 0.5*M_PI ) );

//   // material
//   if (color.getAlpha()<255)
//      o.Material.MaterialType = video::EMT_TRANSPARENT_VERTEX_ALPHA;
//   else
//      o.Material.MaterialType = video::EMT_SOLID;

   o.Vertices.clear();
   o.Indices.clear();
   // o.Vertices.reallocate( (tessCircle+1)*(tessHull+1) );
   // o.Indices.reallocate( 6*tessCircle*tessHull );
   // o.Vertices.set_used( 0 );
   // o.Indices.set_used( 0 );

   for (uint32_t j=0; j<tessRows; ++j)
   {
      for (uint32_t i=0; i<tessCircle; ++i)
      {
         float const sinA = sincosA[i].s;
         float const cosA = sincosA[i].c;
         float const sinB = sincosB[j].s;
         float const cosB = sincosB[j].c;
         float const x = -sinA * cosB;				// equals the normal at point radius*(x,y,z)
         float const y =  sinB;					// equals the normal at point radius*(x,y,z)
         float const z =  cosA * cosB;				// equals the normal at point radius*(x,y,z)
         float const u = 0.5f-0.5f*cosB * sinA; // not finished
         float const v = 0.5f-0.5f*cosB * cosA; // not finished
         o.Vertices.push_back(
               S3DVertex( rx*x,ry*y,rz*z, x,y,z, color, u,v ) );
      }
   }

   // push top center vertex
   o.Vertices.push_back( S3DVertex(0,ry,0, 0,1,0, color, 0.5f,0.5f) );

   // push indices
   for (uint32_t j=0; j<tessRows; j++)
   {
      for (uint32_t i=0; i<tessCircle; i++)
      {
         // top row contains just triangles, no quads
         if (j==tessRows-1)
         {
            uint32_t i0; // A
            uint32_t i1; // B
            uint32_t i2 = o.Vertices.size()-1; // C - top center

            // border
            if (i==tessCircle-1)
            {
               i0 = (j)*tessCircle + (i);	// A
               i1 = (j)*tessCircle + (0);	// B
            }
            // normal
            else
            {
               i0 = (j)*tessCircle + (i);	// A
               i1 = (j)*tessCircle + (i+1);	// B
            }
            o.Indices.push_back( i0 ); // A
            o.Indices.push_back( i2 ); // C
            o.Indices.push_back( i1 ); // B
         }

         // bottom to top-1 rows consist of quad segments (each of 2 triangles)
         else
         {
            uint32_t i0;	// A
            uint32_t i1;	// B
            uint32_t i2;	// C
            uint32_t i3;	// D

            // border
            if (i==tessCircle-1)
            {
               i0 = (j)*tessCircle + (i);	// A
               i1 = (j+1)*tessCircle + (i);	// B
               i2 = (j+1)*tessCircle + (0);	// C
               i3 = (j)*tessCircle + (0);	// D
            }
            // normal
            else
            {
               i0 = (j)*tessCircle + (i);	// A
               i1 = (j+1)*tessCircle + (i);	// B
               i2 = (j+1)*tessCircle + (i+1);// C
               i3 = (j)*tessCircle + (i+1);	// D
            }
            o.Indices.push_back( i0 ); // A
            o.Indices.push_back( i1 ); // B
            o.Indices.push_back( i2 ); // C
            o.Indices.push_back( i0 ); // A
            o.Indices.push_back( i2 ); // C
            o.Indices.push_back( i3 ); // D
         }
      }
   }

   return o;
}

SMeshBuffer
Sphere::createLowerHalf( float rx, float ry, float rz,
                         uint32_t tessCircle, uint32_t tessRows,
                         uint32_t color )
{
   SMeshBuffer o( PrimitiveType::Triangles );

   // bbox
   o.BoundingBox.setMin( -rx, -ry, -rz );
   o.BoundingBox.setMax( rx, 0, rz);

   float const sxz = 1.f / (float)tessCircle;
   float const sy = 1.f / (float)tessRows;

   // tables
   SinCosTablef sincosA(tessCircle);
   SinCosTablef sincosB(tessRows, float(0.0), float( 0.5*M_PI ) );

//   // material
//   if (color.getAlpha()<255)
//      o.Material.MaterialType = video::EMT_TRANSPARENT_VERTEX_ALPHA;
//   else
//      o.Material.MaterialType = video::EMT_SOLID;

   o.Vertices.clear();
   o.Indices.clear();
   // o.Vertices.reallocate( (tessCircle+1)*(tessHull+1) );
   // o.Indices.reallocate( 6*tessCircle*tessHull );
   // o.Vertices.set_used( 0 );
   // o.Indices.set_used( 0 );

   for (uint32_t j=0; j<tessRows; ++j)
   {
      for (uint32_t i=0; i<tessCircle; ++i)
      {
         float const sinA = sincosA[i].s;
         float const cosA = sincosA[i].c;
         float const sinB = sincosB[j].s;
         float const cosB = sincosB[j].c;
         float const x = -sinA * cosB;				// equals the normal at point radius*(x,y,z)
         float const y = -sinB;					// equals the normal at point radius*(x,y,z)
         float const z =  cosA * cosB;				// equals the normal at point radius*(x,y,z)
         float const u = 0.5f-0.5f*cosB * sinA; // not finished
         float const v = 0.5f-0.5f*cosB * cosA; // not finished
         o.Vertices.push_back(
               S3DVertex( rx*x,ry*y,rz*z, x,y,z, color, u,v ) );
      }
   }

   // push bottom center vertex
   o.Vertices.push_back( S3DVertex(0,-ry,0, 0,1,0, color, 0.5f,0.5f) );

   // push indices
   for (uint32_t j=0; j<tessRows; j++)
   {
      for (uint32_t i=0; i<tessCircle; i++)
      {
         // top row contains just triangles, no quads
         if (j==tessRows-1)
         {
            uint32_t i0; // A
            uint32_t i1; // B
            uint32_t i2 = o.Vertices.size()-1; // C - top center

            // border
            if (i==tessCircle-1)
            {
               i0 = (j)*tessCircle + (i);	// A
               i1 = (j)*tessCircle + (0);	// B
            }
            // normal
            else
            {
               i0 = (j)*tessCircle + (i);	// A
               i1 = (j)*tessCircle + (i+1);	// B
            }
            o.Indices.push_back( i0 ); // A
            o.Indices.push_back( i2 ); // C
            o.Indices.push_back( i1 ); // B
         }

         // bottom to top-1 rows consist of quad segments (each of 2 triangles)
         else
         {
            uint32_t i0;	// A
            uint32_t i1;	// B
            uint32_t i2;	// C
            uint32_t i3;	// D

            // border
            if (i==tessCircle-1)
            {
               i0 = (j)*tessCircle + (i);	// A
               i1 = (j+1)*tessCircle + (i);	// B
               i2 = (j+1)*tessCircle + (0);	// C
               i3 = (j)*tessCircle + (0);	// D
            }
            // normal
            else
            {
               i0 = (j)*tessCircle + (i);	// A
               i1 = (j+1)*tessCircle + (i);	// B
               i2 = (j+1)*tessCircle + (i+1);// C
               i3 = (j)*tessCircle + (i+1);	// D
            }
            o.Indices.push_back( i0 ); // A
            o.Indices.push_back( i1 ); // B
            o.Indices.push_back( i2 ); // C
            o.Indices.push_back( i0 ); // A
            o.Indices.push_back( i2 ); // C
            o.Indices.push_back( i3 ); // D
         }
      }
   }

   return o;
}

IMesh* CGeometryCreator::createUpperSphereHalfMesh(
      float radius,
      uint32_t tessCircle,
      uint32_t tessRows,
      const video::SColor& color,
      bool closeBottom) const
{
   SMesh* mesh = new SMesh();
   if (!mesh)
      return 0;

   float const sxz = 1.f / (float)tessCircle;
   float const sy = 1.f / (float)tessRows;

   // tables
   core::CSinTablef sinTableA(tessCircle);
   core::CCosTablef cosTableA(tessCircle);
   core::CSinTablef sinTableB(tessRows, 0.0f, 0.5f*core::PI);
   core::CCosTablef cosTableB(tessRows, 0.0f, 0.5f*core::PI);

   //! upper sphere half
   SMeshBuffer* buffer = new SMeshBuffer();
   if (buffer)
   {
      // bbox
      o.BoundingBox = core::aabbox3df( -radius, 0, -radius, radius, radius, radius);

      // material
      if (color.getAlpha()<255)
         o.Material.MaterialType = video::EMT_TRANSPARENT_VERTEX_ALPHA;
      else
         o.Material.MaterialType = video::EMT_SOLID;

      o.Vertices.clear();
      o.Indices.clear();
      // o.Vertices.reallocate( (tessCircle+1)*(tessHull+1) );
      // o.Indices.reallocate( 6*tessCircle*tessHull );
      // o.Vertices.set_used( 0 );
      // o.Indices.set_used( 0 );

      for (uint32_t j=0; j<tessRows; ++j)
      {
         for (uint32_t i=0; i<tessCircle; ++i)
         {
            float const sinA = sinTableA[i];
            float const cosA = cosTableA[i];
            float const sinB = sinTableB[j];
            float const cosB = cosTableB[j];
            float const x = -sinA * cosB;				// equals the normal at point radius*(x,y,z)
            float const y =  sinB;					// equals the normal at point radius*(x,y,z)
            float const z =  cosA * cosB;				// equals the normal at point radius*(x,y,z)
            float const u = 0.5f-0.5f*cosB * sinA; // not finished
            float const v = 0.5f-0.5f*cosB * cosA; // not finished
            o.Vertices.push_back(
               video::S3DVertex( radius*x,radius*y,radius*z, x,y,z, color, u,v ) );
         }
      }

      // push top center vertex
      o.Vertices.push_back(video::S3DVertex(0,radius,0, 0,1,0, color, 0.5f,0.5f) );

      // push indices
      for (uint32_t j=0; j<tessRows; j++)
      {
         for (uint32_t i=0; i<tessCircle; i++)
         {
            // top row contains just triangles, no quads
            if (j==tessRows-1)
            {
               int32_t i0; // A
               int32_t i1; // B
               int32_t i2 = o.Vertices.size()-1; // C - top center

               // border
               if (i==tessCircle-1)
               {
                  i0 = (j)*tessCircle + (i);	// A
                  i1 = (j)*tessCircle + (0);	// B
               }
               // normal
               else
               {
                  i0 = (j)*tessCircle + (i);	// A
                  i1 = (j)*tessCircle + (i+1);	// B
               }
               o.Indices.push_back((u16)i0); // A
               o.Indices.push_back((u16)i2); // C
               o.Indices.push_back((u16)i1); // B
            }

            // bottom to top-1 rows consist of quad segments (each of 2 triangles)
            else
            {
               int32_t i0;	// A
               int32_t i1;	// B
               int32_t i2;	// C
               int32_t i3;	// D

               // border
               if (i==tessCircle-1)
               {
                  i0 = (j)*tessCircle + (i);	// A
                  i1 = (j+1)*tessCircle + (i);	// B
                  i2 = (j+1)*tessCircle + (0);	// C
                  i3 = (j)*tessCircle + (0);	// D
               }
               // normal
               else
               {
                  i0 = (j)*tessCircle + (i);	// A
                  i1 = (j+1)*tessCircle + (i);	// B
                  i2 = (j+1)*tessCircle + (i+1);// C
                  i3 = (j)*tessCircle + (i+1);	// D
               }
               o.Indices.push_back((u16)i0); // A
               o.Indices.push_back((u16)i1); // B
               o.Indices.push_back((u16)i2); // C
               o.Indices.push_back((u16)i0); // A
               o.Indices.push_back((u16)i2); // C
               o.Indices.push_back((u16)i3); // D
            }
         }
      }

      // add to mesh
      mesh->addMeshBuffer(buffer);
      core::aabbox3df bbox = mesh->getBoundingBox();
      bbox.addInternalBox( o.getBoundingBox() );
      mesh->setBoundingBox( bbox );
      o.drop();
      buffer = 0;
   }

   //! bottom circle - cannot share vertices with hull, due to texturing and lighting
   if (closeBottom)
   {
      buffer = new SMeshBuffer();
      if (buffer)
      {
         // bbox
         o.BoundingBox.reset( core::aabbox3df(-radius,0,-radius, radius,0,radius) );

         // material
         if (color.getAlpha()<255)
            o.Material.MaterialType = video::EMT_TRANSPARENT_VERTEX_ALPHA;
         else
            o.Material.MaterialType = video::EMT_SOLID;

         // memory
         o.Vertices.reallocate( tessCircle+1 );
         o.Vertices.set_used( 0 );
         o.Indices.reallocate( 3*tessCircle );
         o.Indices.set_used( 0 );

         // center vertex
         o.Vertices.push_back( video::S3DVertex(0,0,0, 0,-1,0,color,0.5f,0.5f) );

         // vertices + indices
         for (uint32_t i=0; i<tessCircle; i++)
         {
            float const x = radius * sinTableA[i];
            float const z = radius * cosTableA[i];
            float const u = 0.5f+0.5f*sinTableA[i];
            float const v = 0.5f+0.5f*cosTableA[i];
            o.Vertices.push_back( video::S3DVertex(x,0,z,0,-1,0,color,u,v) );

            o.Indices.push_back(0);	// push always center vertex
            uint32_t j=i+2;
            if (j>=tessCircle+1)
               j=1;
            o.Indices.push_back(j);
            o.Indices.push_back(i+1);
         }

         mesh->addMeshBuffer(buffer);
         core::aabbox3df bbox = mesh->getBoundingBox();
         bbox.addInternalBox( o.getBoundingBox() );
         mesh->setBoundingBox( bbox );
         o.drop();
         buffer = 0;
      }
   }

   // exit
   mesh->setHardwareMappingHint(EHM_STATIC);
   return mesh;
}

//!----------------------------------------------------------------------------
//! lower-HalfSphere
//!----------------------------------------------------------------------------

IMesh* CGeometryCreator::createLowerSphereHalfMesh(
      float radius,
      uint32_t tessCircle,
      uint32_t tessRows,
      const video::SColor& color,
      bool closeTop) const
{
   SMesh* mesh = new SMesh();
   if (!mesh)
      return 0;

   // tables
   core::CSinTablef sinTableA(tessCircle);
   core::CCosTablef cosTableA(tessCircle);
   core::CSinTablef sinTableB(tessRows, 0.0f, 0.5f*core::PI);
   core::CCosTablef cosTableB(tessRows, 0.0f, 0.5f*core::PI);

   float const sxz = 1.f / (float)tessCircle;
   float const sy = 1.f / (float)tessRows;

   //!----------------------------------------------------------------------------
   //! lower-HalfSphere
   //!----------------------------------------------------------------------------

   SMeshBuffer* buffer = new SMeshBuffer();
   if (buffer)
   {
      // bbox
      o.BoundingBox = core::aabbox3df( -radius, -radius, -radius, radius, 0, radius);

      // material
      if (color.getAlpha()<255)
         o.Material.MaterialType = video::EMT_TRANSPARENT_VERTEX_ALPHA;
      else
         o.Material.MaterialType = video::EMT_SOLID;

      o.Vertices.clear();
      o.Indices.clear();
      // o.Vertices.reallocate( (tessCircle+1)*(tessHull+1) );
      // o.Indices.reallocate( 6*tessCircle*tessHull );
      // o.Vertices.set_used( 0 );
      // o.Indices.set_used( 0 );

      for (uint32_t j=0; j<tessRows; ++j)
      {
         for (uint32_t i=0; i<tessCircle; ++i)
         {
            float const sinA = sinTableA[i];
            float const cosA = cosTableA[i];
            float const sinB = sinTableB[j];
            float const cosB = cosTableB[j];
            float const x = -sinA * cosB;				// equals the normal at point radius*(x,y,z)
            float const y = -sinB;					// equals the normal at point radius*(x,y,z)
            float const z =  cosA * cosB;				// equals the normal at point radius*(x,y,z)
            float const u = 0.5f-0.5f*cosB * sinA; // not finished
            float const v = 0.5f+0.5f*cosB * cosA; // not finished
            o.Vertices.push_back(
               video::S3DVertex( radius*x,radius*y,radius*z, x,y,z, color, u,v ) );
         }
      }

      // lower-sphere-half bottom center vertex
      o.Vertices.push_back( video::S3DVertex(0.f,-radius,0.f, 0.f,-1.f,0.f, color, .5f,.5f) );

      // lower-sphere-half Indices
      for (uint32_t j=0; j<tessRows; j++)
      {
         for (uint32_t i=0; i<tessCircle; i++)
         {
            // Top just triangles
            if (j==tessRows-1)
            {
               int32_t i0; // A
               int32_t i1; // B
               int32_t i2 = o.Vertices.size()-1; // C - top center

               // border
               if (i==tessCircle-1)
               {
                  i0 = (j)*tessCircle + (i);	// A
                  i1 = (j)*tessCircle + (0);	// B
               }
               // normal
               else
               {
                  i0 = (j)*tessCircle + (i);	// A
                  i1 = (j)*tessCircle + (i+1);	// B
               }
               o.Indices.push_back((u16)i1); // A
               o.Indices.push_back((u16)i2); // C
               o.Indices.push_back((u16)i0); // B
            }
            // Bottom to Top-1 consist each of 2 triangles
            else
            {
               int32_t i0;	// A
               int32_t i1;	// B
               int32_t i2;	// C
               int32_t i3;	// D

               // border
               if (i==tessCircle-1)
               {
                  i0 = (j)*tessCircle + (i);	// A
                  i1 = (j+1)*tessCircle + (i);	// B
                  i2 = (j+1)*tessCircle + (0);	// C
                  i3 = (j)*tessCircle + (0);	// D
               }
               // normal
               else
               {
                  i0 = (j)*tessCircle + (i);	// A
                  i1 = (j+1)*tessCircle + (i);	// B
                  i2 = (j+1)*tessCircle + (i+1);// C
                  i3 = (j)*tessCircle + (i+1);	// D
               }
               o.Indices.push_back((u16)i2); // A
               o.Indices.push_back((u16)i1); // B
               o.Indices.push_back((u16)i0); // C
               o.Indices.push_back((u16)i3); // A
               o.Indices.push_back((u16)i2); // C
               o.Indices.push_back((u16)i0); // D
            }
         }
      }

      // add to mesh
      mesh->addMeshBuffer(buffer);
      core::aabbox3df bbox = mesh->getBoundingBox();
      bbox.addInternalBox( o.getBoundingBox() );
      mesh->setBoundingBox( bbox );
      o.drop();
   }

   //!----------------------------------------------------------------------------
   //! top circle
   //!----------------------------------------------------------------------------

   if (closeTop)
   {
      buffer = new SMeshBuffer();
      if (buffer)
      {
         // bbox
         o.setBoundingBox( core::aabbox3df( -radius,0,-radius, radius,0,radius ) );

         // memory
         o.Vertices.reallocate( tessCircle+1 );
         o.Vertices.set_used( 0 );
         o.Indices.reallocate( 3*tessCircle );
         o.Indices.set_used( 0 );

         // material
         if (color.getAlpha()<255)
            o.Material.MaterialType = video::EMT_TRANSPARENT_VERTEX_ALPHA;
         else
            o.Material.MaterialType = video::EMT_SOLID;

         // center vertex
         o.Vertices.push_back( video::S3DVertex(0,0,0,0,1,0,color,0.5f,0.5f) );

         // fill buffer
         for (uint32_t i=0; i<tessCircle; ++i)
         {
            float const x = -radius * sinTableA[i];
            float const z =  radius * cosTableA[i];
            float const u = 0.5f-0.5f*sinTableA[i];
            float const v = 0.5f-0.5f*cosTableA[i];
            o.Vertices.push_back( video::S3DVertex( x,0,z, 0,1,0, color, u,v ) );

            o.Indices.push_back(0);
            uint32_t j=i+2;
            if (j>=tessCircle+1)
               j=1;
            o.Indices.push_back(j);
            o.Indices.push_back(i+1);
         }

         // add to mesh
         mesh->addMeshBuffer(buffer);
         core::aabbox3df bbox = mesh->getBoundingBox();
         bbox.addInternalBox( o.getBoundingBox() );
         mesh->setBoundingBox( bbox );
         o.drop();
      }
   }

   mesh->setHardwareMappingHint(EHM_STATIC);
   return mesh;
}
*/


