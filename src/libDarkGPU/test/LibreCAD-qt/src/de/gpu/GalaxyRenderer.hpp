#ifndef DE_GPU_GALAXY_RENDERER_HPP
#define DE_GPU_GALAXY_RENDERER_HPP

#include <de/gpu/IVideoDriver.hpp>

namespace de {
namespace gpu {

// ===========================================================================
struct GalaxyRenderer
// ===========================================================================
{
   DE_CREATE_LOGGER("de.gpu.GalaxyRenderer")
   GalaxyRenderer();
   ~GalaxyRenderer();
   void render();
   void setDriver( IVideoDriver* driver ) { m_Driver = driver; }



   IVideoDriver* m_Driver;
   SMeshBuffer m_Mesh;

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

