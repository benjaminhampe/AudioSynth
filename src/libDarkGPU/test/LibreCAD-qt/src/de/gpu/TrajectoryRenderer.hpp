#ifndef DE_GPU_TRAJECTORY_RENDERER_HPP
#define DE_GPU_TRAJECTORY_RENDERER_HPP

#include <de/gpu/IVideoDriver.hpp>

namespace de {
namespace gpu {

// ===========================================================================
struct TrajectoryRenderer
// ===========================================================================
{
   DE_CREATE_LOGGER("de.gpu.TrajectoryRenderer")
   TrajectoryRenderer();
   ~TrajectoryRenderer();
   void render();
   void setDriver( IVideoDriver* driver ) { m_Driver = driver; }

   struct Stop
   {
      glm::vec3 pos;
      glm::vec3 dir;
      float deg_angle = 0.0f;
      Quatf quat;
   };

   void clear()
   {
      m_Stops.clear();
   }


   void addStop( glm::vec3 pos, glm::vec3 dir, float deg_angle = 0.0f )
   {
      m_Stops.emplace_back();
      Stop & stop = m_Stops.back();
      stop.pos = pos;
      stop.dir = dir;
      stop.deg_angle = deg_angle;
      stop.quat = Quatf::fromAngleAxis( glm::radians( deg_angle ), dir );
   }

   Stop
   interpolate( Stop const & s1, Stop const & s2, float t )
   {
      Stop s3 = s1;
      s3.quat = Quatf::slerp( s1.quat, s2.quat, t );
      s3.pos = s1.pos + (s2.pos - s1.pos)*t;
      auto aa = s3.quat.toAngleAxis();
      s3.dir = aa.axis;
      s3.deg_angle = glm::degrees( aa.angle );
      return s3;
   }


   IVideoDriver* m_Driver;
   SMeshBuffer m_Mesh;


   std::vector< Stop > m_Stops;

   std::vector< glm::mat4 > m_modelInstances;
   std::vector< uint32_t > m_colorInstances;

   // // Handle to a program object
   // GLuint programObject;

   // // VBOs
   // GLuint positionVBO;
   // GLuint colorVBO;
   // GLuint mvpVBO;
   // GLuint indicesIBO;

   // // Number of indices
   // int       numIndices;

   // // Rotation angle
   // GLfloat   angle[NUM_INSTANCES];
};



} // end namespace gpu.
} // end namespace de.

#endif

