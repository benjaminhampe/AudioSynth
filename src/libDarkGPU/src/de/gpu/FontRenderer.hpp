#ifndef DE_GPU_FONTRENDERER_HPP
#define DE_GPU_FONTRENDERER_HPP

#include <de/gpu/IVideoDriver.hpp>

#ifdef USE_LIB_DARKFONT

namespace de {
namespace gpu {

// ===========================================================================
struct FontRenderer
// ===========================================================================
{
   DE_CREATE_LOGGER("de.gpu.FontRenderer")
   IVideoDriver* m_Driver;
   IShader* m_Shader;

   FontRenderer( IVideoDriver* driver );
   ~FontRenderer();

   TextSize
   getTextSize( std::wstring const & msg, Font const & font = Font() );

   void
   draw2DText( int x, int y, std::wstring const & msg, uint32_t color = 0xFF000000,
               Align::EAlign align = Align::Default, Font const & font = Font() );

   void
   draw2DTextDebug( int x, int y, std::wstring const & msg,
               uint32_t color = 0xFF000000,
               Align::EAlign align = Align::Default, Font const & font = Font() );

   void
   draw2DText( Text const & text )
   {
      draw2DText( text.x, text.y, text.msg, text.brush.color, text.align, text.font );
   }

   void clear();
};

} // end namespace gpu.
} // end namespace de.

#endif // #ifdef USE_LIB_DARKFONT

#endif

