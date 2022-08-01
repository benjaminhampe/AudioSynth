#ifndef DE_GPU_SMESH_GEOMETRY_HPP
#define DE_GPU_SMESH_GEOMETRY_HPP

#include <de/gpu/smesh/SMesh.hpp>
/*
 *
namespace de {
namespace gpu {
namespace mesh {

// =========================================================================
/// @brief Line
// =========================================================================
//
//     B
//    /
//   /
//  /
// A
//
struct Line
{
   static void
   add( SMeshBuffer & o, S3DVertex a, S3DVertex b );

   static void
   add( SMeshBuffer & o, glm::vec3 a, glm::vec3 b, uint32_t color = 0xFFFFFFFF );

   static void
   add( SMeshBuffer & o, glm::vec3 a, glm::vec3 b, uint32_t colorA, uint32_t colorB );

   static void
   addBox( SMeshBuffer & o, Box3f const & box, uint32_t color = 0xFFFFFFFF );
};

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
struct LineTriangle
{
   static void
   add( SMeshBuffer & o, glm::vec3 a, glm::vec3 b, glm::vec3 c, uint32_t color = 0xFFFFFFFF );

   static void
   add( SMeshBuffer & o, glm::vec3 a, glm::vec3 b, glm::vec3 c, uint32_t colorA, uint32_t colorB, uint32_t colorC );
};

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
struct LineBox
{
   static void
   add( SMeshBuffer & o, Box3f const & box, uint32_t color = 0xFFFFFFFF );
};

// =========================================================================
/// @brief BKS Line mesh
// =========================================================================
struct BKS
{
   // As lines
   static SMeshBuffer
   create( float x = 0.0f, float y = 0.0f, float z = 0.0f );
};



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
struct Triangle
{
   static void
   add( SMeshBuffer & o, S3DVertex a, S3DVertex b, S3DVertex c, bool recalcNormal = false );

   static void
   addFront( SMeshBuffer & o, S3DVertex a, S3DVertex b, S3DVertex c );
};


// =========================================================================
/// @brief Quad
// =========================================================================
//
// B---C           Bottom Quad LADK: LDA + LKD
// |  /|
// | M |
// |/  |
// A---D
//
struct Quad
{
   // [1.] Generic by 4 raw vertices
   static void
   add( SMeshBuffer & o,
        S3DVertex const & a,
        S3DVertex const & b,
        S3DVertex const & c,
        S3DVertex const & d );

   // [2.] 4pos + 4color + 4uv
   static void
   add( SMeshBuffer & o,
      glm::vec3 const & a,
      glm::vec3 const & b,
      glm::vec3 const & c,
      glm::vec3 const & d,
      uint32_t colorA, uint32_t colorB, uint32_t colorC, uint32_t colorD,
      glm::vec2 const & uvA,
      glm::vec2 const & uvB,
      glm::vec2 const & uvC,
      glm::vec2 const & uvD,
      bool recomputeNormal = false );

   // [3.] 4pos + 1color + 2uv
   static void
   add( SMeshBuffer & o,
      glm::vec3 const & a,
      glm::vec3 const & b,
      glm::vec3 const & c,
      glm::vec3 const & d,
      uint32_t color = 0xFFFFFFFF,
      glm::vec2 const & uvStart = glm::vec2(0,0),
      glm::vec2 const & uvEnde = glm::vec2(1,1),
      bool recomputeNormal = false );

   // [4.] 4pos + 1color + 4uv
   static void
   add( SMeshBuffer & o,
      glm::vec3 const & a,
      glm::vec3 const & b,
      glm::vec3 const & c,
      glm::vec3 const & d,
      uint32_t color,
      glm::vec2 const & uvA,
      glm::vec2 const & uvB,
      glm::vec2 const & uvC,
      glm::vec2 const & uvD,
      bool recomputeNormal = false );

   // by 4 points, normal is computed.
   static void
   addFront( SMeshBuffer & o, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, uint32_t color = 0xFFFFFFFF  );

   static SMeshBuffer
   create( S3DVertex a, S3DVertex b, S3DVertex c, S3DVertex d );

   static SMeshBuffer
   create( glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, uint32_t color = 0xFFFFFFFF );

   // [5.] XY-Plane (w,h) centered at (x,y,z), 2 uv coords ( start + end into tex-atlas )
   static void
   addXY( SMeshBuffer & o, float w, float h, uint32_t color = 0xFFFFFFFF ,
         float x = 0.f, float y = 0.f, float z = 0.f,
         float u1 = 0.0f, float v1 = 0.0f,
         float u2 = 1.0f, float v2 = 1.0f,
         bool recomputeNormal = false );

   // XY plane
   static SMeshBuffer
   createXY( float w, float h, uint32_t color = 0xFFFFFFFF,
            float off_x = 0.f, float off_y = 0.f, float off_z = 0.f );

   // [6.] XZ-Plane (w,h) centered at (x,y,z), 2 uv coords ( start + end into tex-atlas )
   static void
   addXZ( SMeshBuffer & o, float w, float h, uint32_t color = 0xFFFFFFFF ,
         float x = 0.f, float y = 0.f, float z = 0.f,
         float u1 = 0.0f, float v1 = 0.0f,
         float u2 = 1.0f, float v2 = 1.0f,
         bool recomputeNormal = false );

   // XZ plane
   static SMeshBuffer
   createXZ( float w, float h, uint32_t color = 0xFFFFFFFF,
            float off_x = 0.f, float off_y = 0.f, float off_z = 0.f );

};

// =========================================================================
/// @brief RoundRect
// =========================================================================
struct RoundRect // = f(w,h,rx,ry)
{
   static SMeshBuffer*
   create( float w, float h, float rx, float ry, int32_t tesselation, uint32_t color = 0xFFFFFFFF,
           float off_x = 0.0f, float off_y = 0.0f, float off_z = 0.0f );

   /// @brief Add ROUND_RECT(w,h,rx,ry) geometry to meshbuffer ( asIndexedTriangles )
   static void
   add( SMeshBuffer & o,
        float w,
        float h,
        float rx,
        float ry,
        int32_t tesselation,
        uint32_t innerColor = 0xFFFFFFFF,
        uint32_t outerColor = 0xFFFFFFFF,
        float off_x = 0.0f,
        float off_y = 0.0f,
        float off_z = 0.0f );

   // static
   void
   add( SMeshBuffer & o,
         float w, float h,
         float rx, float ry,
         int32_t tesselation,
         uint32_t innerColor,
         uint32_t outerColor,
         float ox,
         float oy,
         float oz )
{

};

// =========================================================================
/// @brief Circle/Ellipse
// =========================================================================
struct Circle
{
   static std::vector< S3DVertex >
   createVerticesXY( int tess, float w, float h, uint32_t color = 0xFFFFFFFF,
            float off_x = 0.0f, float off_y = 0.0f, float off_z = 0.0f );

   static std::vector< S3DVertex >
   createVerticesXZ( int tess, float w, float h, uint32_t color = 0xFFFFFFFF,
            float off_x = 0.0f, float off_y = 0.0f, float off_z = 0.0f );

   static void
   addXY( SMeshBuffer & o, int tess, float w, float h, uint32_t color = 0xFFFFFFFF,
           float off_x = 0.0f, float off_y = 0.0f, float off_z = 0.0f );

   static void
   addXZ( SMeshBuffer & o, int tess, float w, float h, uint32_t color = 0xFFFFFFFF,
           float off_x = 0.0f, float off_y = 0.0f, float off_z = 0.0f );

};



// =========================================================================
/// @brief Ring
// =========================================================================
struct Ring
{
   static SMeshBuffer
   create(
      float outer_x,
      float outer_y,
      float inner_x,
      float inner_y,
      uint32_t tess,
      float off_x = 0.f,
      float off_y = 0.f,
      float off_z = 0.f );

   static void
   add( SMeshBuffer & o,
      float outer_x,
      float outer_y,
      float inner_x,
      float inner_y,
      uint32_t tess,
      float off_x = 0.f,
      float off_y = 0.f,
      float off_z = 0.f );
};

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
//                                                          +----+----+---------+
struct Cube
{
   typedef float T;
   typedef glm::tvec3< T > V3;

   // Add all 6 quads
   static void add( SMeshBuffer & o,
            float dx, float dy, float dz,
            uint32_t color = 0xFFFFFFFF,
            glm::vec2 uvStart1 = glm::vec2(0,0), glm::vec2 uvEnde1 = glm::vec2(1,1),
            glm::vec2 uvStart2 = glm::vec2(0,0), glm::vec2 uvEnde2 = glm::vec2(1,1),
            glm::vec2 uvStart3 = glm::vec2(0,0), glm::vec2 uvEnde3 = glm::vec2(1,1),
            glm::vec2 uvStart4 = glm::vec2(0,0), glm::vec2 uvEnde4 = glm::vec2(1,1),
            glm::vec2 uvStart5 = glm::vec2(0,0), glm::vec2 uvEnde5 = glm::vec2(1,1),
            glm::vec2 uvStart6 = glm::vec2(0,0), glm::vec2 uvEnde6 = glm::vec2(1,1) )
   {
      V3 r = V3( dx, dy, dz );
      V3 A = getA( r );
      V3 B = getB( r );
      V3 C = getC( r );
      V3 D = getD( r );
      V3 E = getE( r );
      V3 F = getF( r );
      V3 G = getG( r );
      V3 H = getH( r );
      Quad::add( o, A, B, C, D, color, uvStart1, uvEnde1 ); // -Y front plane ABCD
      Quad::add( o, H, G, F, E, color, uvStart2, uvEnde2 ); // +Y back plane HGFE
      Quad::add( o, E, F, B, A, color, uvStart3, uvEnde3 ); // -X left plane EFBA
      Quad::add( o, D, C, G, H, color, uvStart4, uvEnde4 ); // +X right plane DCGH
      Quad::add( o, D, H, E, A, color, uvStart5, uvEnde5 ); // -Z bottom plane HDAE
      Quad::add( o, C, B, F, G, color, uvStart6, uvEnde6 ); // +Z top plane BFGC
   }


   // -Y front plane ABCD
   static void getPlaneNY( V3 const & size, V3 & a, V3 & b, V3 & c, V3 & d )
   {
      a = getA( size ); b = getB( size ); c = getC( size ); d = getD( size );
   }

   // +Y back plane HGFE
   static void getPlanePY( V3 const & size, V3 & a, V3 & b, V3 & c, V3 & d )
   {
      a = getH( size ); b = getG( size ); c = getF( size ); d = getE( size );
   }

   // -X left plane EFBA
   static void getPlaneNX( V3 const & size, V3 & a, V3 & b, V3 & c, V3 & d )
   {
      a = getE( size ); b = getF( size ); c = getB( size ); d = getA( size );
   }

   // +X right plane DCGH
   static void getPlanePX( V3 const & size, V3 & a, V3 & b, V3 & c, V3 & d )
   {
      a = getD( size ); b = getC( size ); c = getG( size ); d = getH( size );
   }

   // -Z bottom plane HDAE
   static void getPlaneNZ( V3 const & size, V3 & a, V3 & b, V3 & c, V3 & d )
   {
      a = getH( size ); b = getD( size ); c = getA( size ); d = getE( size );
   }

   // +Z top plane BFGC
   static void getPlanePZ( V3 const & size, V3 & a, V3 & b, V3 & c, V3 & d )
   {
      a = getB( size ); b = getF( size ); c = getG( size ); d = getC( size );
   }

   static void addPlaneNX( SMeshBuffer & o, V3 const & size, uint32_t color  )
   {
      V3 a,b,c,d; getPlaneNX( size, a,b,c,d ); Quad::add( o, a,b,c,d, color );
   }
   static void addPlaneNY( SMeshBuffer & o, V3 const & size, uint32_t color  )
   {
      V3 a,b,c,d; getPlaneNY( size, a,b,c,d ); Quad::add( o, a,b,c,d, color );
   }
   static void addPlaneNZ( SMeshBuffer & o, V3 const & size, uint32_t color  )
   {
      V3 a,b,c,d; getPlaneNZ( size, a,b,c,d ); Quad::add( o, a,b,c,d, color );
   }
   static void addPlanePX( SMeshBuffer & o, V3 const & size, uint32_t color  )
   {
      V3 a,b,c,d; getPlanePX( size, a,b,c,d ); Quad::add( o, a,b,c,d, color );
   }
   static void addPlanePY( SMeshBuffer & o, V3 const & size, uint32_t color  )
   {
      V3 a,b,c,d; getPlanePY( size, a,b,c,d ); Quad::add( o, a,b,c,d, color );
   }
   static void addPlanePZ( SMeshBuffer & o, V3 const & size, uint32_t color  )
   {
      V3 a,b,c,d; getPlanePZ( size, a,b,c,d ); Quad::add( o, a,b,c,d, color );
   }

   static V3 getA() { return V3(-1,-1,-1 ); }
   static V3 getB() { return V3(-1,-1, 1 ); }
   static V3 getC() { return V3( 1,-1, 1 ); }
   static V3 getD() { return V3( 1,-1,-1 ); }
   static V3 getE() { return V3(-1, 1,-1 ); }
   static V3 getF() { return V3(-1, 1, 1 ); }
   static V3 getG() { return V3( 1, 1, 1 ); }
   static V3 getH() { return V3( 1, 1,-1 ); }
   static V3 getA( V3 const & size ) { return getA() * (size * 0.5f); }
   static V3 getB( V3 const & size ) { return getB() * (size * 0.5f); }
   static V3 getC( V3 const & size ) { return getC() * (size * 0.5f); }
   static V3 getD( V3 const & size ) { return getD() * (size * 0.5f); }
   static V3 getE( V3 const & size ) { return getE() * (size * 0.5f); }
   static V3 getF( V3 const & size ) { return getF() * (size * 0.5f); }
   static V3 getG( V3 const & size ) { return getG() * (size * 0.5f); }
   static V3 getH( V3 const & size ) { return getH() * (size * 0.5f); }

   // Baukasten for straight walls, control all 6 faces of a cube/box/quader.
   static SMeshBuffer create( float dx, float dy, float dz, uint32_t color = 0xFFFFFFFF );
   static SMeshBuffer createPlaneNX( float dx, float dy, float dz, uint32_t color = 0xFFFFFFFF );
   static SMeshBuffer createPlaneNY( float dx, float dy, float dz, uint32_t color = 0xFFFFFFFF );
   static SMeshBuffer createPlaneNZ( float dx, float dy, float dz, uint32_t color = 0xFFFFFFFF );
   static SMeshBuffer createPlanePX( float dx, float dy, float dz, uint32_t color = 0xFFFFFFFF );
   static SMeshBuffer createPlanePY( float dx, float dy, float dz, uint32_t color = 0xFFFFFFFF );
   static SMeshBuffer createPlanePZ( float dx, float dy, float dz, uint32_t color = 0xFFFFFFFF );
};



// =========================================================================
/// @brief Cube = UnitCube * scale(x,y,z)
// =========================================================================
//
//        F-------G
//       /|      /|     Bottom Quad
//      / |     / |
//     /  |    /  |
//    /   E---/---H
//   /   /   /   /
//  /   /   /   /
// B-------C   /     y
// |  /    |  /      |  Z
// | /     | /       | /
// |/      |/        |/
// A-------D         0------> x
//
struct CubeOld
{
   typedef float T;
   typedef glm::tvec3< T > V3;

   // Add all 6 quads
   static void add( SMeshBuffer & o, float dx, float dy, float dz, uint32_t color = 0xFFFFFFFF,
            glm::vec2 uvStart1 = glm::vec2(0,0), glm::vec2 uvEnde1 = glm::vec2(1,1),
            glm::vec2 uvStart2 = glm::vec2(0,0), glm::vec2 uvEnde2 = glm::vec2(1,1),
            glm::vec2 uvStart3 = glm::vec2(0,0), glm::vec2 uvEnde3 = glm::vec2(1,1),
            glm::vec2 uvStart4 = glm::vec2(0,0), glm::vec2 uvEnde4 = glm::vec2(1,1),
            glm::vec2 uvStart5 = glm::vec2(0,0), glm::vec2 uvEnde5 = glm::vec2(1,1),
            glm::vec2 uvStart6 = glm::vec2(0,0), glm::vec2 uvEnde6 = glm::vec2(1,1) )
   {
      V3 r = V3( dx, dy, dz ) * T(0.5);
      V3 A = V3(-1,-1, 1 ) * r;
      V3 B = V3(-1, 1, 1 ) * r;
      V3 C = V3( 1, 1, 1 ) * r;
      V3 D = V3( 1,-1, 1 ) * r;
      V3 E = V3(-1,-1,-1 ) * r;
      V3 F = V3(-1, 1,-1 ) * r;
      V3 G = V3( 1, 1,-1 ) * r;
      V3 H = V3( 1,-1,-1 ) * r;
      Quad::add( o, A, B, C, D, color, uvStart1, uvEnde1 ); // front
      Quad::add( o, H, G, F, E, color, uvStart2, uvEnde2 ); // back
      Quad::add( o, E, F, B, A, color, uvStart3, uvEnde3 ); // left
      Quad::add( o, D, C, G, H, color, uvStart4, uvEnde4 ); // right
      Quad::add( o, D, H, E, A, color, uvStart5, uvEnde5 ); // bottom
      Quad::add( o, C, B, F, G, color, uvStart6, uvEnde6 ); // top
   }

   static void addHullXY( SMeshBuffer & o, float dx, float dy, float dz, uint32_t color = 0xFFFFFFFF )
   {
      addQuadNX( o, dx, dy, dz, color );
      addQuadNY( o, dx, dy, dz, color );
      addQuadPX( o, dx, dy, dz, color );
      addQuadPY( o, dx, dy, dz, color );
   }

   static V3 getA() { return V3(-1,-1, 1 ); }
   static V3 getB() { return V3(-1, 1, 1 ); }
   static V3 getC() { return V3( 1, 1, 1 ); }
   static V3 getD() { return V3( 1,-1, 1 ); }
   static V3 getE() { return V3(-1,-1,-1 ); }
   static V3 getF() { return V3(-1, 1,-1 ); }
   static V3 getG() { return V3( 1, 1,-1 ); }
   static V3 getH() { return V3( 1,-1,-1 ); }

   static V3 getA( V3 const & size ) { return getA() * size; }
   static V3 getB( V3 const & size ) { return getB() * size; }
   static V3 getC( V3 const & size ) { return getC() * size; }
   static V3 getD( V3 const & size ) { return getD() * size; }
   static V3 getE( V3 const & size ) { return getE() * size; }
   static V3 getF( V3 const & size ) { return getF() * size; }
   static V3 getG( V3 const & size ) { return getG() * size; }
   static V3 getH( V3 const & size ) { return getH() * size; }


   // -X = Left = Quad(EFBA)
   static void getPlaneNX( V3 const & size, V3 & a, V3 & b, V3 & c, V3 & d )
   {
      V3 r = size * T(0.5);
      a = getE( r );
      b = getF( r );
      c = getB( r );
      d = getH( r );
   }

   // -Y = bottom = Quad(DHEA)
   static void getPlaneNY( V3 const & size, V3 & a, V3 & b, V3 & c, V3 & d )
   {
      V3 r = size * T(0.5);
      a = getD( r );
      b = getH( r );
      c = getE( r );
      d = getA( r );
   }

   // -Z = Back or Front = Quad(ABCD)
   static void getPlaneNZ( V3 const & size, V3 & a, V3 & b, V3 & c, V3 & d )
   {
      V3 r = size * T(0.5);
      a = getA( r );
      b = getB( r );
      c = getC( r );
      d = getD( r );
   }

   // +X = right = Quad(DCGH)
   static void getPlanePX( V3 const & size, V3 & a, V3 & b, V3 & c, V3 & d )
   {
      V3 r = size * T(0.5);
      a = getD( r );
      b = getC( r );
      c = getG( r );
      d = getH( r );
   }

   // +Y = Top = Quad(CBFG)
   static void getPlanePY( V3 const & size, V3 & a, V3 & b, V3 & c, V3 & d )
   {
      V3 r = size * T(0.5);
      a = getC( r );
      b = getB( r );
      c = getF( r );
      d = getG( r );
   }

   // +Z = Back or Front = Quad(HGFE)
   static void getPlanePZ( V3 const & size, V3 & a, V3 & b, V3 & c, V3 & d )
   {
      V3 r = size * T(0.5);
      a = getH( r );
      b = getG( r );
      c = getF( r );
      d = getE( r );
   }

   // -X = Left = Quad(EFBA)
   static void addQuadNX( SMeshBuffer & o, float dx, float dy, float dz, uint32_t color  )
   {
      V3 r = V3( dx, dy, dz ) * T(0.5); // Radius in x,y,z
      Quad::add( o, getE( r ), getF( r ), getB( r ), getH( r ), color );
   }
   // -Y = bottom = Quad(DHEA)
   static void addQuadNY( SMeshBuffer & o, float dx, float dy, float dz, uint32_t color  )
   {
      V3 r = V3( dx, dy, dz ) * T(0.5); // Radius in x,y,z
      Quad::add( o, getD( r ), getH( r ), getE( r ), getA( r ), color );
   }

   // -Z = Back or Front = Quad(ABCD)
   static void addQuadNZ( SMeshBuffer & o, float dx, float dy, float dz, uint32_t color  )
   {
      V3 r = V3( dx, dy, dz ) * T(0.5); // Radius in x,y,z
      Quad::add( o, getA( r ), getB( r ), getC( r ), getD( r ), color );
   }

   // +X = right = Quad(DCGH)
   static void addQuadPX( SMeshBuffer & o, float dx, float dy, float dz, uint32_t color  )
   {
      V3 r = V3( dx, dy, dz ) * T(0.5); // Radius in x,y,z
      Quad::add( o, getD( r ), getC( r ), getG( r ), getH( r ), color );
   }

   // +Y = Top = Quad(CBFG)
   static void addQuadPY( SMeshBuffer & o, float dx, float dy, float dz, uint32_t color  )
   {
      V3 r = V3( dx, dy, dz ) * T(0.5); // Radius in x,y,z
      Quad::add( o, getC( r ), getB( r ), getF( r ), getG( r ), color );
   }

   // +Z = Back or Front = Quad(HGFE)
   static void addQuadPZ( SMeshBuffer & o, float dx, float dy, float dz, uint32_t color  )
   {
      V3 r = V3( dx, dy, dz ) * T(0.5); // Radius in x,y,z
      Quad::add( o, getH( r ), getG( r ), getF( r ), getE( r ), color );
   }

   // Baukasten for straight walls, control all 6 faces of a cube/box/quader.
   static SMeshBuffer* create( float dx, float dy, float dz, uint32_t color = 0xFFFFFFFF );
   static SMeshBuffer* createPlaneNX( float dx, float dy, float dz, uint32_t color = 0xFFFFFFFF );
   static SMeshBuffer* createPlaneNY( float dx, float dy, float dz, uint32_t color = 0xFFFFFFFF );
   static SMeshBuffer* createPlaneNZ( float dx, float dy, float dz, uint32_t color = 0xFFFFFFFF );
   static SMeshBuffer* createPlanePX( float dx, float dy, float dz, uint32_t color = 0xFFFFFFFF );
   static SMeshBuffer* createPlanePY( float dx, float dy, float dz, uint32_t color = 0xFFFFFFFF );
   static SMeshBuffer* createPlanePZ( float dx, float dy, float dz, uint32_t color = 0xFFFFFFFF );

};


// =========================================================================
/// @brief SkyboxData
// =========================================================================
struct SkyboxData
{
   float dx;
   float dy;
   float dz;
   PrimitiveType::EType primType;
   std::string back;
   std::string front;
   std::string left;
   std::string right;
   std::string bottom;
   std::string top;
};

// =========================================================================
/// @brief Skybox
// =========================================================================
//
//        F-------G
//       /|      /|
//      / |     / |
//     /  |    /  |
//    /   E---/---H
//   /   /   /   /
//  /   /   /   /
// B-------C   /        Bottom Quad LADK: LDA + LKD
// |  /    |  /
// | /     | /
// |/      |/
// A-------D
//
struct SkyBox
{
   static SMesh*
   create( float dx, float dy, float dz,
           ITexture* back, ITexture* front, ITexture* left,
           ITexture* right, ITexture* bottom, ITexture* top,
           PrimitiveType::EType primType = PrimitiveType::Triangles );
};

// =========================================================================
/// @brief PolyLine
// =========================================================================
struct PolyLine
{
   static void
   add( SMeshBuffer & o, std::vector< glm::vec3 > const & points, float lineWidth, uint32_t color );
};

// =========================================================================
/// @brief PolyHull
// =========================================================================
struct PolyHull
{
   static void
   addLeftStripXY( SMeshBuffer & o, std::vector< glm::vec3 > const & points, float height, uint32_t color );

   static void
   addLeftStripXZ( SMeshBuffer & o, std::vector< glm::vec3 > const & points, float height, uint32_t color );

   static void
   addRightStrip( SMeshBuffer & o, std::vector< glm::vec3 > const & points, float height, uint32_t color );
};

// =========================================================================
/// @brief CylinderHull
// =========================================================================
struct CylinderHull
{
   static void
   addXY( SMeshBuffer & o,
        int tessCirc, float dx, float dy, float dz, uint32_t color = 0xFFFFFFFF,
            float off_x = 0.0f, float off_y = 0.0f, float off_z = 0.0f )
   {
      glm::vec3 offset( off_x, off_y, off_z );
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
         points.emplace_back( offset + glm::vec3( a * ps, b * pc, 0.0f ) );
      }

      PolyHull::addLeftStripXY( o, points, dz, color );
   }

   static void
   addXZ( SMeshBuffer & o,
        int tessCirc, float dx, float dy, float dz, uint32_t color = 0xFFFFFFFF,
            float off_x = 0.0f, float off_y = 0.0f, float off_z = 0.0f )
   {
      glm::vec3 offset( off_x, off_y, off_z );
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
         points.emplace_back( offset + glm::vec3( a * ps, 0.0f, b * pc ) );
      }

      PolyHull::addLeftStripXZ( o, points, dy, color );
   }
};

// =========================================================================
/// @brief Chip = CylinderTop [Ellipse] + CylinderHull, no bottom [Ellipse]
// =========================================================================
struct Cylinder
{
   static void
   addXY( SMeshBuffer & o, int tessCirc,
            float dx, float dy, float dz, uint32_t color = 0xFFFFFFFF,
            float off_x = 0.0f, float off_y = 0.0f, float off_z = 0.0f )
   {
      Circle::addXY( o, tessCirc, dx, dy, color, off_x, off_y, off_z + 0.5f * dz );
      CylinderHull::addXY( o, tessCirc, dx, dy, dz, color, off_x, off_y, off_z );
   }

   static void
   addXZ( SMeshBuffer & o, int tessCirc,
            float dx, float dy, float dz, uint32_t color = 0xFFFFFFFF,
            float off_x = 0.0f, float off_y = 0.0f, float off_z = 0.0f )
   {
      Circle::addXZ( o, tessCirc, dx, dz, color, off_x, off_y + 0.5f * dy, off_z );
      CylinderHull::addXZ( o, tessCirc, dx, dy, dz, color, off_x, off_y, off_z );
   }
};

typedef Cylinder Chip;

// ===========================================================================
// Sphere
// ===========================================================================
struct Sphere
{
   static S3DVertex
   computeVertex( float rx, float ry, float rz,
                  int32_t i, int32_t j,
                  int32_t tess_u, int32_t tess_v,
                  uint32_t color );

   static void
   add( SMeshBuffer & o,
        float rx, float ry, float rz,
        uint32_t color = 0xFFFFFFFF,
        int32_t tess_u = 33,
        int32_t tess_v = 33 );

   static SMeshBuffer
   create( float rx, float ry, float rz,
           int32_t tess_u, int32_t tess_v, uint32_t color = 0xFFFFFFFF );

// Old broken

   static SMeshBuffer
   createUpperHalf( float rx, float ry, float rz,
                  uint32_t tessCircle, uint32_t tessRows,
                  uint32_t color = 0xFFFFFFFF );

   static SMeshBuffer
   createLowerHalf( float rx, float ry, float rz,
                  uint32_t tessCircle, uint32_t tessRows,
                  uint32_t color = 0xFFFFFFFF );

};

struct PioHexagonOutline
{
   static SMeshBuffer
   create( float w, float h, float lineWidth, uint32_t color )
   {
      SMeshBuffer o( PrimitiveType::Triangles );
      auto points = PioHexagon::getCorners(w,h);
      PolyLine::add( o, points, lineWidth, color );
      return o;
   }
};

// =========================================================================
/// @brief The Hexagon ( ver_2018 ):
// =========================================================================
//
//               M| x=0 | y=0 | z=0 | u=.5| v=.5|
//       D       -|-----|-----|-----|-----|-----|
//      / \      A|   0 |  0  | -.5 | .5  | 0   |
//   C /   \ E   B| -.5 |  0  |-.25 |  0  | 0.25|
//    |-----|    C| -.5 |  0  | .25 |  0  | 0.75|
//    |  M  |    D| 0.0 |  0  |  .5 |  .5 | 1   |
//    |-----|    E| 0.5 |  0  | .25 |  1  | 0.75|
//   B \   / F   F| 0.5 |  0  |-.25 |  1  | 0.25|
//      \ /
//       A       triangles: ABF, BCE, BEF, CDE
//
struct PioHexagon
{
   DE_CREATE_LOGGER("PioHexagon")

   static int32_t
   safeMod( int32_t a, int32_t b );

   static float
   computeBoardPosX( float w, float h, int i, int j );

   static float
   computeBoardPosY( float w, float h, int i, int j );

   static glm::vec3
   lineCenter( glm::vec3 const & a, glm::vec3 const & b )
   {
      return a + ( b - a ) * 0.5f;
   }

//   static SMeshBuffer*
//   create( float w, float h, uint32_t color = 0xFFFFFFFF );

   static glm::vec3 cornerA( float w, float h ) { return glm::vec3( 0.0f,    0.0f, -.5f*h ); }
   static glm::vec3 cornerB( float w, float h ) { return glm::vec3( -0.5f*w, 0.0f,-.25f*h ); }
   static glm::vec3 cornerC( float w, float h ) { return glm::vec3( -0.5f*w, 0.0f, .25f*h ); }
   static glm::vec3 cornerD( float w, float h ) { return glm::vec3( 0.0f,   0.0f,   .5f*h ); }
   static glm::vec3 cornerE( float w, float h ) { return glm::vec3( 0.5f*w, 0.0f,  .25f*h ); }
   static glm::vec3 cornerF( float w, float h ) { return glm::vec3( 0.5f*w, 0.0f, -.25f*h ); }

   static glm::vec3 edgeAB( float w, float h ) { return lineCenter( cornerA(w,h), cornerB(w,h)); }
   static glm::vec3 edgeBC( float w, float h ) { return lineCenter( cornerB(w,h), cornerC(w,h)); }
   static glm::vec3 edgeCD( float w, float h ) { return lineCenter( cornerC(w,h), cornerD(w,h)); }
   static glm::vec3 edgeDE( float w, float h ) { return lineCenter( cornerD(w,h), cornerE(w,h)); }
   static glm::vec3 edgeEF( float w, float h ) { return lineCenter( cornerE(w,h), cornerF(w,h)); }
   static glm::vec3 edgeFA( float w, float h ) { return lineCenter( cornerF(w,h), cornerA(w,h)); }

   static glm::vec3
   getCorner( int i, float w, float h, glm::vec3 offset = glm::vec3() )
   {
      switch ( i )
      {
         case 0: return offset + cornerA( w,h );
         case 1: return offset + cornerB( w,h );
         case 2: return offset + cornerC( w,h );
         case 3: return offset + cornerD( w,h );
         case 4: return offset + cornerE( w,h );
         case 5: return offset + cornerF( w,h );
         default: return offset + cornerA( w,h );
      }
   }

   static std::vector< glm::vec3 >
   getCorners( float w, float h )
   {
      return std::vector< glm::vec3 > {
         cornerA( w,h ), cornerB( w,h ), cornerC( w,h ),
         cornerD( w,h ), cornerE( w,h ), cornerF( w,h ) };
   }

   static glm::vec3
   getEdge( int i, float w, float h, glm::vec3 offset = glm::vec3() )
   {
      switch ( i )
      {
         case 0: return offset + edgeAB( w,h );
         case 1: return offset + edgeBC( w,h );
         case 2: return offset + edgeCD( w,h );
         case 3: return offset + edgeDE( w,h );
         case 4: return offset + edgeEF( w,h );
         case 5: return offset + edgeFA( w,h );
         default: throw std::runtime_error("Invalid i in getEdge()");
      }
   }

   static std::vector< Triangle3f >
   getTriangles( float w, float h, glm::vec3 const & offset = glm::vec3() )
   {
      glm::vec3 A = offset + cornerA( w,h );
      glm::vec3 B = offset + cornerB( w,h );
      glm::vec3 C = offset + cornerC( w,h );
      glm::vec3 D = offset + cornerD( w,h );
      glm::vec3 E = offset + cornerE( w,h );
      glm::vec3 F = offset + cornerF( w,h );
      return { {B,A,F}, {B,F,E}, {B,E,C}, {C,E,D} };
   }

//   static float
//   computeAngle( glm::vec2 const & ab )
//   {
//      float phi = atan2( ab.y, ab.x ) * Math::RAD2DEG;
//      //if ( phi < 0.0f ) phi += 180.0f;
//      return phi;
//   }

   static float
   computeAngle( glm::vec2 const & v )
   {
      glm::vec2 const y(0,1);
      return acos( glm::dot(v,y) / glm::length(v) ) * Math::RAD2DEG;
   }
//
//               M| x=0 | y=0 | z=0 | u=.5| v=.5|
//       D       -|-----|-----|-----|-----|-----|
//      / \      A|   0 |  0  | -.5 | .5  | 0   |
//   C /   \ E   B| -.5 |  0  |-.25 |  0  | 0.25|
//    |-----|    C| -.5 |  0  | .25 |  0  | 0.75|
//    |  M  |    D| 0.0 |  0  |  .5 |  .5 | 1   |
//    |-----|    E| 0.5 |  0  | .25 |  1  | 0.75|
//   B \   / F   F| 0.5 |  0  |-.25 |  1  | 0.25|
//      \ /
//       A       triangles: ABF, BCE, BEF, CDE
//

   static float
   computeCornerAngle( int i, float w, float h )
   {
      switch ( i )
      {
         case 0: return 90.0f;
         case 1: return 0.0f;
         case 2: return 0.0f;
         case 3: return 0.0f;
         case 4: return 0.0f;
         case 5: return 90.0f;
         default: throw std::runtime_error("Invalid i in computeCornerAngle()");
      }
   }

   static float
   computeEdgeAngle( int i, float w, float h )
   {
      //i = i % 6;
      switch ( i )
      {
         case 0: return computeAngle( cornerB( w,h ) - cornerA( w,h ) );
         case 1: return computeAngle( cornerC( w,h ) - cornerB( w,h ) );
         case 2: return computeAngle( cornerD( w,h ) - cornerC( w,h ) );
         case 3: return computeAngle( cornerE( w,h ) - cornerD( w,h ) );
         case 4: return computeAngle( cornerF( w,h ) - cornerE( w,h ) );
         case 5: return computeAngle( cornerA( w,h ) - cornerF( w,h ) );
         default: throw std::runtime_error("Invalid i in computeEdgeAngle()");
      }
   }

   static void
   test( float w, float h )
   {
      DE_DEBUG("PioHexagon Angles for w(",w,"), h(",h,")")
      for ( int i = 0; i < 6; ++i )
      {
         DE_DEBUG("PioHexagon.CornerAngle[", i,"] = ", computeCornerAngle( i,w,h ) )
      }
      for ( int i = 0; i < 6; ++i )
      {
         DE_DEBUG("PioHexagon.EdgeAngle[", i,"] = ", computeEdgeAngle( i,w,h ) )
      }
   }

};




} // end namespace mesh.
} // end namespace gpu.
} // end namespace de.
*/

#endif

