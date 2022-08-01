#ifndef DE_GPU_MESH_HEXAGON_HPP
#define DE_GPU_MESH_HEXAGON_HPP

#include <de/gpu/smesh/SMesh.hpp>

namespace de {
namespace gpu {
namespace smesh {

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
struct Hexagon
{
   DE_CREATE_LOGGER("de.gpu.smesh.Hexagon")

   static void
   test( float w, float h )
   {
      DE_DEBUG("EdgeAngles for w(",w,"), h(",h,")")

      for ( int i = 0; i < 6; ++i )
      {
         DE_DEBUG("EdgeAngle[", i,"] get(", getEdgeAngle( i,w,h), "), computed(", computeEdgeAngle( i,w,h), ")" )
      }
   }

   static int32_t
   safeMod( int32_t a, int32_t b )
   {
      if ( a == 0 || b == 0 ) { return 0; }
      return std::abs( a ) % std::abs( b );
   }

   static float
   computeBoardPosX( float w, float h, int i, int j )
   {
      float x = 0.5f * float( safeMod( j, 2 ) == 1 ) + float( i );
      return x * w;
   }

   static float
   computeBoardPosY( float w, float h, int i, int j )
   {
      float y = 3.0f/4.0f * float( j );
      return y * h;
   }

//   static void
//   addXZ( SMeshBuffer & out,
//            float w, float h, uint32_t color = 0xFFFFFFFF,
//            float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f )
//      //, float u1, float v1, float u2, float v2 )
//   {
//      out.setPrimitiveType( PrimitiveType::Triangles );
//      float tx = u1;
//      float ty = v1;
//      float sx = u2 - u1;
//      float sy = v2 - v1;
//      S3DVertex const A( 0.0f,   0.0f, -.5f*h, 0,1,0, color, (0.5f * sx) + tx, ( 1.0f * sy) + ty );
//      S3DVertex const B( -0.5f*w,0.0f,-.25f*h, 0,1,0, color, (0.0f * sx) + tx, (0.75f * sy) + ty );
//      S3DVertex const C( -0.5f*w,0.0f, .25f*h, 0,1,0, color, (0.0f * sx) + tx, (0.25f * sy) + ty );
//      S3DVertex const D( 0.0f,   0.0f,  .5f*h, 0,1,0, color, (0.5f * sx) + tx, ( 0.0f * sy) + ty );
//      S3DVertex const E( 0.5f*w, 0.0f, .25f*h, 0,1,0, color, (1.0f * sx) + tx, (0.25f * sy) + ty );
//      S3DVertex const F( 0.5f*w, 0.0f,-.25f*h, 0,1,0, color, (1.0f * sx) + tx, (0.75f * sy) + ty );
//      out.addVertex( B ); out.addVertex( F ); out.addVertex( A ); // BAF
//      out.addVertex( B ); out.addVertex( E ); out.addVertex( F ); // BFE
//      out.addVertex( B ); out.addVertex( C ); out.addVertex( E ); // BEC
//      out.addVertex( C ); out.addVertex( D ); out.addVertex( E ); // CED
//      out.recalculateBoundingBox();
//   }

   static void
   addXZ( SMeshBuffer & out,
            float w, float h,
            uint32_t color = 0xFFFFFFFF,
            float x = .0f, float y = .0f, float z = .0f,
            Rectf const & coords = Rectf::identity() )
   {
      out.setPrimitiveType( PrimitiveType::Triangles );
      float const u1 = coords.u1();
      float const v1 = coords.v1();
      float const u2 = coords.u2();
      float const v2 = coords.v2();
      float sx = u2 - u1;
      float sy = v2 - v1;
      float hx = 0.5f * w;
      float hy = 0.5f * h;
      float qy = 0.25f * h;
      S3DVertex A( x,   y,z-hy, 0,1,0, color, (0.5f * sx) + u1, ( 1.0f * sy) + v1 );
      S3DVertex D( x,   y,z+hy, 0,1,0, color, (0.5f * sx) + u1, ( 0.0f * sy) + v1 );
      S3DVertex B( x-hx,y,z-qy, 0,1,0, color, (0.0f * sx) + u1, (0.75f * sy) + v1 );
      S3DVertex C( x-hx,y,z+qy, 0,1,0, color, (0.0f * sx) + u1, (0.25f * sy) + v1 );
      S3DVertex E( x+hx,y,z+qy, 0,1,0, color, (1.0f * sx) + u1, (0.25f * sy) + v1 );
      S3DVertex F( x+hx,y,z-qy, 0,1,0, color, (1.0f * sx) + u1, (0.75f * sy) + v1 );
      out.addTriangle( A,B,F ); // Bottom
      out.addTriangle( C,D,E ); // CED // Top
      out.addQuad( B,C,E,F ); // BCEF
   }

   static glm::vec3 lineCenter( glm::vec3 const & a, glm::vec3 const & b ) { return a + ( b - a ) * 0.5f; }
   static glm::vec3 cornerA( float w, float h ) { return glm::vec3( 0.0f, -.5f*h, 0.0f ); }
   static glm::vec3 cornerB( float w, float h ) { return glm::vec3( -0.5f*w,-.25f*h, 0.0f ); }
   static glm::vec3 cornerC( float w, float h ) { return glm::vec3( -0.5f*w, .25f*h, 0.0f ); }
   static glm::vec3 cornerD( float w, float h ) { return glm::vec3( 0.0f,     .5f*h, 0.0f ); }
   static glm::vec3 cornerE( float w, float h ) { return glm::vec3( 0.5f*w,  .25f*h, 0.0f ); }
   static glm::vec3 cornerF( float w, float h ) { return glm::vec3( 0.5f*w, -.25f*h, 0.0f ); }
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

   static float
   computeAngle( glm::vec2 const & ab )
   {
      float phi = atan2( ab.y, ab.x ) * Math::RAD2DEG;
      //if ( phi < 0.0f ) phi += 180.0f;
      return phi;
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

   static float
   getEdgeAngle( int i, float w, float h )
   {
      // i = i % 6;
      switch ( i )
      {
         case 0: return -30.0f;
         case 1: return 90.0f;
         case 2: return 30.0f;
         case 3: return -30.0f;
         case 4: return 90.0f;
         case 5: return 30.0f;
         default: throw std::runtime_error("Invalid i in getEdgeAngle()");
      }
   }

};

/*
struct PioneerHexagonOutline
{
   static SMeshBuffer*
   create( float w, float h, float lineWidth, uint32_t color )
   {
      SMeshBuffer* o = SMeshBufferTool::createTriangles();
      auto points = PioneerHexagon::getCorners(w,h);
      PolyLine::add( *o, points, lineWidth, color );
      return o;
   }
};

// =========================================================================
/// @brief HexagonHull
// =========================================================================
struct PioneerHexagonHull
{
   static SMeshBuffer*
   create( float w, float h, float height, uint32_t color )
   {
      SMeshBuffer* o = SMeshBufferTool::createTriangles();
      o->Material.setCulling( false );
      o->Material.setBlend( Blend::alphaBlend() );
      auto points = PioneerHexagon::getCorners(w,h);
      PolyHull::addLeftStrip( *o, points, height, color );
      return o;
   }
};


// =========================================================================
/// @brief House = Box + 2 roof quads + 2 roof triangles.
// =========================================================================
struct PioneerHome
{
   // As triangles
   static SMesh*
   create( float dx, float dy, float dz, uint32_t color = 0xFFFFFFFF )
   {
      SMesh * mesh = new SMesh( "House" );

      SMeshBuffer* walls = Cube::create( dx, dy * 0.6f, dz, color );
      SMeshBufferTool::translateVertices( *walls, 0,dy * 0.3f, 0.0f );
      mesh->addMeshBuffer( walls );

      SMeshBuffer* roof1 = Cube::create( 0.7*dx, 0.1*dy, dz, color );
      SMeshBufferTool::rotateVertices( *roof1, 0,0,45 );
      SMeshBufferTool::translateVertices( *roof1, -dx * 0.25f, dy * 0.8f, 0.0f );
      mesh->addMeshBuffer( roof1 );

      SMeshBuffer* roof2 = Cube::create( 0.7*dx, 0.1*dy, dz, color );
      SMeshBufferTool::rotateVertices( *roof2, 0,0,-45 );
      SMeshBufferTool::translateVertices( *roof2, dx * 0.25f, dy * 0.8f, 0.0f);
      mesh->addMeshBuffer( roof2 );

      return mesh;
   }
};

// =========================================================================
/// @brief City = Box + House
// =========================================================================
struct PioneerCity
{
   // As triangles
   static SMesh*
   create( float dx, float dy, float dz, uint32_t color = 0xFFFFFFFF )
   {
      SMesh* mesh = new SMesh( "City" );

      SMeshBuffer* ground = Cube::create( dx, 0.5f * dy, dz, color );
      SMeshBufferTool::translateVertices( *ground, 0, 0.25f*dy, 0 );
      mesh->addMeshBuffer( ground );

      SMesh* house = PioneerHome::create( 0.5f * dx, 0.5f * dy, dz, color );
      //house->rotateVertices( 0, 20, 0 );
      house->translateVertices( 0.25f * dx, 0.5f*dy, 0 );
      mesh->addMesh( *house );
      delete house;

      return mesh;
   }

};
*/

} // end namespace smesh.
} // end namespace gpu.
} // end namespace de.


#endif

