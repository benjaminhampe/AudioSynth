#ifndef DE_GPU_SMESH_SPHERE_HPP
#define DE_GPU_SMESH_SPHERE_HPP

#include <de/gpu/smesh/SMesh.hpp>

namespace de {
namespace gpu {
namespace smesh {

// =========================================================================
struct Sphere
// =========================================================================
{
   // @brief Benni book example 2021
   DE_CREATE_LOGGER("de.gpu.smesh.Sphere")

   // [Longitude] Rotation um die y-Achse in degrees, equatorial xz plane is ground plane +Y is north pole
   // [Latitude] Rotation um die x/z-Achse in degrees: meridian plane (yx or yz)
   //
   static glm::vec3
   computeSphereNormal( float lon, float lat ) // in degrees
   {
      if ( lat >= 89.999f ) { return glm::vec3( 0,1,0 ); } // NorthPole
      else if ( lat <= -89.999f ) { return glm::vec3( 0,-1,0 ); } // SouthPole
      float lon_rad = lon * Math::DEG2RAD;
      float lat_rad = lat * Math::DEG2RAD;
      float x = ::cosf( lon_rad ) * ::cosf( lat_rad );
      float y =                     ::sinf( lat_rad );
      float z = ::sinf( lon_rad ) * ::cosf( lat_rad );
      //DE_DEBUG("f(",lon,"'E,",lat,"'N) = {x:",x,", y:",y,", z:",z,"}")
      return glm::vec3( x,y,z );
   }

   static void
   add( SMeshBuffer & o,
         glm::vec3 const & siz = glm::vec3(100,100,100),
         uint32_t color = 0xFFFFFFFF,
         uint32_t tessLon = 3,
         uint32_t tessLat = 3 )
   {
      o.setPrimitiveType( PrimitiveType::Triangles );
      if ( tessLat < 3 ) tessLat = 3;
      if ( tessLon < 3 ) tessLon = 3;

      glm::vec3 radius = siz * 0.5f;

      // u-step
      float lonStep = 360.0f / float( tessLon );
      float lonStart = 0.0f; // Greenwich
      //float lonEnd = 360.0f;
      float lonUV = 1.0f / float( tessLon );

      // v-step
      float latStep = -180.0f / float( tessLat );
      float latStart = 90.0f;
      //float latEnd = -90.0f;
      float latUV = 1.0f / float( tessLat );

      // Predict and reserve memory, indexed quads.
      o.moreVertices( 4 * tessLon * tessLat );
      o.moreIndices( 6 * tessLon * tessLat );

      auto addQuad = [&] ( int32_t ix, int32_t iy )
      {
         //DE_DEBUG("addQuad( i=",ix,", j=", iy, " )")
         float lon1 = lonStart + lonStep * ix;
         float lon2 = lonStart + lonStep * ( ix + 1 );
         float lat1 = latStart + latStep * iy;
         float lat2 = latStart + latStep * ( iy + 1 );
         glm::vec3 na = computeSphereNormal( lon1, lat2 );
         glm::vec3 nb = computeSphereNormal( lon1, lat1 );
         glm::vec3 nc = computeSphereNormal( lon2, lat1 );
         glm::vec3 nd = computeSphereNormal( lon2, lat2 );
         glm::vec3 a = radius * na;
         glm::vec3 b = radius * nb;
         glm::vec3 c = radius * nc;
         glm::vec3 d = radius * nd;
         uint32_t colorA = color;
         uint32_t colorB = color;
         uint32_t colorC = color;
         uint32_t colorD = color;
         S3DVertex A( a.x,a.y,a.z, na.x,na.y,na.z, colorA, lonUV * ix, latUV * (iy+1) );
         S3DVertex B( b.x,b.y,b.z, nb.x,nb.y,nb.z, colorB, lonUV * ix, latUV * (iy) );
         S3DVertex C( c.x,c.y,c.z, nc.x,nc.y,nc.z, colorC, lonUV * (ix+1), latUV * (iy) );
         S3DVertex D( d.x,d.y,d.z, nd.x,nd.y,nd.z, colorD, lonUV * (ix+1), latUV * (iy+1) );
         o.addQuad( A,B,C,D );
      };

      for ( int32_t j = 0; j < tessLat; ++j )
      {
         for ( int32_t i = 0; i < tessLon; ++i )
         {
            addQuad( i,j );
         }
      }
   }
};

} // end namespace smesh.
} // end namespace gpu.
} // end namespace de.

#endif
