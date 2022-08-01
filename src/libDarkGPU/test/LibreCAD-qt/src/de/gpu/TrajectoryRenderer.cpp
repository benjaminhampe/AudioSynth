#include "TrajectoryRenderer.hpp"
//#include <de/gpu/TrajectoryRenderer.hpp>
#include <de/gpu/IVideoDriver.hpp>

namespace de {
namespace gpu {

TrajectoryRenderer::TrajectoryRenderer()
   : m_Driver(nullptr)
{
   addStop( glm::vec3( 0,0,0 ), glm::vec3( 0,0,1 ), 1.0f );
   addStop( glm::vec3( 0,0,100 ), glm::vec3( 0,0,1 ), 4*360.0f );
   addStop( glm::vec3( 0,100,100 ), glm::vec3( 1,0,0 ), 1.0f );
   //addStop( glm::vec3( 0,100,90 ), glm::vec3( -1,0,-1 ), 360.0f );
   addStop( glm::vec3( 0,100,0 ), glm::vec3( 0,0,-1 ), 4*360.0f );
}

TrajectoryRenderer::~TrajectoryRenderer()
{}

void
TrajectoryRenderer::render()
{
   if ( !m_Driver ) { DE_ERROR("No driver") return; }

   for ( size_t i = 0; i < m_Stops.size(); ++i )
   {
      Stop const & stop = m_Stops[ i ];
      auto modelMat = glm::mat4( stop.quat.toMat4() );
      auto m = glm::value_ptr( modelMat );
      // ()
      auto right = glm::vec3( m[0], m[4], m[8] );
      auto up    = glm::vec3( m[1], m[5], m[9] );
      auto front = glm::vec3( m[2], m[6], m[10] );

      m_Driver->draw3DLine( stop.pos, stop.pos + right * 20.0f, 0xFF0000FF );
      m_Driver->draw3DLine( stop.pos, stop.pos + up * 20.0f, 0xFF00FF00 );
      m_Driver->draw3DLine( stop.pos, stop.pos + front * 20.0f, 0xFFFF0000 );
   }

   for ( size_t i = 1; i < m_Stops.size(); ++i )
   {
      Stop const & s1 = m_Stops[ i-1 ];
      Stop const & s2 = m_Stops[ i ];

      DE_DEBUG("s1[",i,"] = ",s1.quat.toString())
      DE_DEBUG("s2[",i,"] = ",s2.quat.toString())

      size_t n = 30;
      float f = 1.0f / float(n-1);
      for ( size_t k = 0; k < n; ++k )
      {
         Stop s3 = interpolate( s1, s2, f * k );
         DE_DEBUG("s3[",i,"][",k,"] = ",s3.quat.toString())
         auto mat = glm::mat4( s3.quat.toMat4() );
         auto m = glm::value_ptr( mat );
         auto r = glm::vec3( m[0], m[4], m[8] );
         auto u = glm::vec3( m[1], m[5], m[9] );
         auto f = glm::vec3( m[2], m[6], m[10] );

         m_Driver->draw3DLine( s3.pos, s3.pos + r * 7.0f, 0xFF8080FF );
         m_Driver->draw3DLine( s3.pos, s3.pos + u * 7.0f, 0xFF80FF80 );
         m_Driver->draw3DLine( s3.pos, s3.pos + f * 7.0f, 0xFFFF8080 );
      }
   }
}


} // end namespace gpu.
} // end namespace de.



