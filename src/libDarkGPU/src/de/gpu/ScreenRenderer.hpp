#ifndef DE_GPU_SCREENRENDERER_HPP
#define DE_GPU_SCREENRENDERER_HPP

#include <de/gpu/IVideoDriver.hpp>

namespace de {
namespace gpu {

// ===========================================================================
struct ScreenRenderer
// ===========================================================================
{
   DE_CREATE_LOGGER("de.gpu.ScreenRenderer")

   ScreenRenderer( IVideoDriver* driver );
   ~ScreenRenderer() {}
   bool setMaterial( SMaterial const & material );
   void unsetMaterial( SMaterial const & material );
   void animate( double pts ) {}

protected:
   static std::string createShaderName( SMaterial const & material );
   static std::string createVS( SMaterial const & material );
   static std::string createFS( SMaterial const & material );
   IVideoDriver* m_Driver;
   IShader* m_Shader[ 2 ];  // color or texture shader
   float m_ZIndex;
   bool m_IsDirty;
   AnimTimer m_AnimTime;
   SMaterial m_Material;
};

} // end namespace gpu.
} // end namespace de.

#endif
