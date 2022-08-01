#ifndef DE_CPU_COLOR_PICKING_HPP
#define DE_CPU_COLOR_PICKING_HPP

#include <de/ImagePainter.hpp>

namespace de {
namespace cpu {

   struct ColorPicking
   {
      Image m_img;

      ColorPicking()
      {

      }

      //~ColorPicking()
      //{}

      void resize( int w, int h )
      {
         m_img.resize( w, h );
      }

      void clear()
      {
         m_img.fill( 0x00000000 );
      }

      void draw( Recti pos, uint32_t colorId )
      {
         ImagePainter::drawRect( m_img, pos, colorId );
      }

      uint32_t pick( int mx, int my )
      {
         return m_img.getPixel(mx,my);
      }


   };

} // end namespace cpu.
} // end namespace de.


#endif

