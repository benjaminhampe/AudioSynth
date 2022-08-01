#include <de/gpu/PerfOverlay.hpp>

namespace de {
namespace gpu {

namespace
{
   constexpr float const CONST_Z_INIT = 0.90f; // At 1.0 it disappears, not inside frustum.
   constexpr float const CONST_Z_STEP = -0.00001f; // enough for 200.000 elements, TODO: test more.
}

PerfOverlay::PerfOverlay( IVideoDriver* driver )
   : m_Driver( driver )
{
}

void
PerfOverlay::draw2DPerfOverlay5x8()
{
   if (!m_Driver) return;
   int w = m_Driver->getScreenWidth();
   int h = m_Driver->getScreenHeight();
   int mx = m_Driver->getMouseX();
   int my = m_Driver->getMouseY();

   int p = 2;
   int x = p;
   int y = p;
   int ln = m_Driver->getTextSize5x8( "W" ).y + p;
   Align::EAlign align = Align::TopLeft;

   auto s1 = dbStrJoin("Frames(",m_Driver->getFrameCount(),")");
   //DE_DEBUG("w(",w,"), h(",h,"), s1(",s1,")")
   m_Driver->draw2DText5x8( x,y, s1, 0xFF00AEFE, align ); y += ln;
   m_Driver->draw2DText5x8( x,y, dbStrJoin("Time(",m_Driver->getTimer(),")"), 0xFF0030FF, align ); y += ln;
   m_Driver->draw2DText5x8( x,y, dbStrJoin("FPS(",m_Driver->getFPS(),")"), 0xFF10BB10, align ); y += ln;
   m_Driver->draw2DText5x8( x,y, dbStrJoin("Mouse(",mx,",",my,")"), 0xFF10BB10, align ); y += ln;

   de::gpu::ICamera* camera = m_Driver->getActiveCamera();
   if ( camera )
   {
      m_Driver->draw2DText5x8( x,y, dbStrJoin("Camera-Pos(",glm::ivec3(camera->getPos()),")"), 0xFFFFEEEE, align ); y += ln;
      {
         std::ostringstream s;
         s << "Camera-Angle(" << int(camera->getAng().x) << ","
                              << int(camera->getAng().y) << ","
                              << int(camera->getAng().z) << ")";
         m_Driver->draw2DText5x8( x,y, s.str(), 0xFFFFBBBB, align ); y += ln;
      }
      m_Driver->draw2DText5x8( x,y, dbStrJoin("Camera-Dir(",camera->getDir(),")"), 0xFFDD6A6A, align ); y += ln;
      m_Driver->draw2DText5x8( x,y, dbStrJoin("Camera-Right(",camera->getRight(),")"), 0xFFCC1212, align ); y += ln;

      m_Driver->draw2DText5x8( x,y, dbStrJoin("Camera-FOV(",camera->getFOV(),")"), 0xFFaa5252, align ); y += ln;
      m_Driver->draw2DText5x8( x,y, dbStrJoin("Camera-Near(",camera->getNearValue(),")"), 0xFFaa52aa, align ); y += ln;
      m_Driver->draw2DText5x8( x,y, dbStrJoin("Camera-Far(",camera->getFarValue(),")"), 0xFFaa72CC, align ); y += ln;
   }

   m_Driver->draw2DText5x8( x,y, dbStrJoin("MSAA(",m_Driver->getMSAA(),")"), 0xFFee9292, align ); y += ln;
   auto smgr = m_Driver->getSceneManager();
   if ( smgr )
   {
      m_Driver->draw2DText5x8( x,y, dbStrJoin("Buffers(",smgr->getBufferCount(),")"), 0xFFaa72CC, align ); y += ln;
      m_Driver->draw2DText5x8( x,y, dbStrJoin("Vertices(",smgr->getVertexCount(),")"), 0xFFaa72CC, align ); y += ln;
      m_Driver->draw2DText5x8( x,y, dbStrJoin("Indices(",smgr->getIndexCount(),")"), 0xFFaa72CC, align ); y += ln;
      m_Driver->draw2DText5x8( x,y, dbStrJoin("Bytes(",dbStrByteCount( smgr->getByteCount() ),")"), 0xFFaa72CC, align ); y += ln;
      //m_Driver->draw2DText5x8( x,y, dbStrJoin("Primitives(",smgr->getByteCount(3),")"), 0xFFaa72CC, align ); y += ln;
   }

   align = Align::TopRight;
   x = w - 1 - p;
   y = p;

   {  std::ostringstream s;
      s << "Screen(" << m_Driver->getScreenWidth() << ","
                     << m_Driver->getScreenHeight()<< ","
                     << m_Driver->getScreenFormat().toString().c_str() << ")";
      m_Driver->draw2DText5x8( x,y, s.str(), 0xFF00EEEE, align ); y += ln;
   }

   align = Align::BottomRight;
   x = w - p;
   y = h - p;
   m_Driver->draw2DText5x8( x,y, "(c) 2021 <benjaminhampe@gmx.de>", 0xFFEEEEEE, align ); y += ln;

   if ( camera )
   {
      align = Align::BottomLeft;
      x = p;
      y = h - p - 4*ln;
      m_Driver->draw2DText5x8( x,y, dbStrJoin("ViewMatrix(",camera->getViewMatrix(),")"), 0xFF1BAB1B, align ); y += ln;
      m_Driver->draw2DText5x8( x,y, dbStrJoin("ProjectionMatrix(",camera->getProjectionMatrix(),")"), 0xFF3CBB3B, align ); y += ln;
      m_Driver->draw2DText5x8( x,y, dbStrJoin("ViewProjectionMatrix(",camera->getViewProjectionMatrix(),")"), 0xFF2BAB2B, align ); y += ln;
      m_Driver->draw2DText5x8( x,y, dbStrJoin("ModelMatrix(",m_Driver->getModelMatrix(),")"), 0xFF1B9BAB, align ); y += ln;
   }
}

/*
void
draw2DShadowText( IVideoDriver* driver,
   int x, int y, std::wstring const & msg, uint32_t fg_color, uint32_t bg_color,
   Align::EAlign align, Font const & font )
{
   Font small = font;
   small.pixelSize -= 4;

//   TextSize ts_font = getFontAtlas( font )->getTextSize( msg );
//   TextSize ts_small = getFontAtlas( small )->getTextSize( msg );

   //driver->draw2DText( x, y, msg, bg_color, align, font );
   driver->draw2DText( x-1, y-1, msg, bg_color, align, font );
   driver->draw2DText( x-1, y, msg, bg_color, align, font );
   driver->draw2DText( x-1, y+1, msg, bg_color, align, font );
   driver->draw2DText( x, y-1, msg, bg_color, align, font );
   driver->draw2DText( x, y, msg, bg_color, align, font );
   driver->draw2DText( x, y+1, msg, bg_color, align, font );
   driver->draw2DText( x+1, y-1, msg, bg_color, align, font );
   driver->draw2DText( x+1, y, msg, bg_color, align, font );
   driver->draw2DText( x+1, y+1, msg, bg_color, align, font );

   driver->draw2DText( x, y, msg, fg_color, align, font );
}

void
PerfOverlay::draw2DPerfOverlay()
{
   if (!m_Driver) return;
   int w = m_Driver->getScreenWidth();
   int h = m_Driver->getScreenHeight();
   de::gpu::ICamera* camera = m_Driver->getActiveCamera();

   int mx = m_Driver->getMouseX();
   int my = m_Driver->getMouseY();

   m_Driver->draw2DLine( mx, 0, mx, h, 0x2FFFFFFF );
   m_Driver->draw2DLine( 0, my, w, my, 0x2FFFFFFF );

   Font font( "arial", 24 );
   Font font2( "arial", 14 );
   TextSize ts = dbGetTextSize( L"WM_01°!", font );
   int p = 10;
   int x = p;
   int y = p;
   int ln = ts.lineHeight + p; // line-height
   Align::EAlign align = Align::TopLeft;

   auto drawFancy = [&] ( int px, int py, std::wstring const & msg,
      uint32_t fg_color, uint32_t bg_color, Align::EAlign align, Font const & font )
   {
      draw2DShadowText( m_Driver, px,py,msg,fg_color,bg_color, align,font );
   };

   uint32_t grey = 0xFF505050;

   align = Align::TopLeft;
   ln = dbGetTextSize( L"WM_01°!", font ).lineHeight + p;
   x = p;
   y = p;
   {  std::wostringstream w;
      w << L"Frames(" << m_Driver->getFrameCount() << L")";
      drawFancy( x,y, w.str(), 0xFF00AEFE, grey, align, font ); y += ln;
   }
   {
      std::wstringstream w;
      w << L"Time(" << m_Driver->getTimer() << L")";
      drawFancy( x,y, w.str(), 0xFF0030FF, grey, align, font ); y += ln;
   }
   {
      std::wstringstream w;
      w << L"FPS(" << m_Driver->getFPS() << L")";
      drawFancy( x,y, w.str(), 0xFF10BB10, grey, align, font ); y += ln;
   }
   {
      std::wstringstream w;
      w << L"Mouse(" << m_Driver->getMouseX() << L"," << m_Driver->getMouseY() << L")";
      drawFancy( x,y, w.str(), 0xFF10BB10, grey, align, font ); y += ln;
   }
   if ( camera )
   {
      {
         std::wostringstream w;
         w << L"Pos(" << glm::ivec3(camera->getPos()) << ")";
         drawFancy( x,y, w.str(), 0xFFFFEEEE, grey, align, font ); y += ln;
      }
      {
         std::wostringstream w;
         w << L"Ang(" << int(camera->getAng().x) << ""
           << L"," << int(camera->getAng().y) << ""
           << L"," << int(camera->getAng().z) << ")";
         drawFancy( x,y, w.str(), 0xFFFFBBBB, grey, align, font ); y += ln;
      }
      {
         std::wostringstream w;
         w << L"Dir(" << camera->getDir() << ")";
         drawFancy( x,y, w.str(), 0xFFDD6A6A, grey, align, font ); y += ln;
      }
      {
         std::wostringstream w;
         w << L"Right(" << camera->getRight() << ")";
         drawFancy( x,y, w.str(), 0xFFCC1212, grey, align, font ); y += ln;
      }
      {
         std::wostringstream w;
         w << L"FOV(" << camera->getFOV() << ")";
         drawFancy( x,y, w.str(), 0xFFaa5252, 0xFFFFFFFF, align, font ); y += ln;
      }
      {
         std::wostringstream w;
         w << L"Near(" << camera->getNearValue() << ")";
         drawFancy( x,y, w.str(), 0xFFaa52aa, grey, align, font ); y += ln;
      }
      {
         std::wostringstream w;
         w << L"Far(" << camera->getFarValue() << ")";
         drawFancy( x,y, w.str(), 0xFFaa72CC, grey, align, font ); y += ln;
      }
   }

   {
      std::wostringstream w;
      w << L"MSAA(" << m_Driver->getMSAA() << ")";
      drawFancy( x,y, w.str(), 0xFFee9292, 0xFFFFFFFF, align, font ); y += ln;
   }

   {
      std::wostringstream w;
      w << L"Buffers(" << m_Driver->getRenderStat(0) << ")";
      drawFancy( x,y, w.str(), 0xFFaa72CC, grey, align, font ); y += ln;
   }

   {
      std::wostringstream w;
      w << L"Vertices(" << m_Driver->getRenderStat(1) << ")";
      drawFancy( x,y, w.str(), 0xFFaa72CC, grey, align, font ); y += ln;
   }

   {
      std::wostringstream w;
      w << L"Indices(" << m_Driver->getRenderStat(2) << ")";
      drawFancy( x,y, w.str(), 0xFFaa72CC, grey, align, font ); y += ln;
   }

   {
      std::wostringstream w;
      w << L"Primitves(" << m_Driver->getRenderStat(3) << ")";
      drawFancy( x,y, w.str(), 0xFFaa72CC, grey, align, font ); y += ln;
   }

   align = Align::TopRight;
   ln = dbGetTextSize( L"WM_01°!", font ).lineHeight + p;
   x = w - p;
   y = p;

   {  std::wostringstream w;
      w << L"Screen(" << m_Driver->getScreenWidth() << L","
              << m_Driver->getScreenHeight()<< L","
              << m_Driver->getScreenFormat().toString().c_str() << L")";
      drawFancy( x,y, w.str(), 0xFF00EEEE, grey, align, font ); y += ln;
   }

   align = Align::BottomRight;
   ln = dbGetTextSize( L"WM_01°!").lineHeight + p;
   x = w - p;
   y = h - p;
   {
      std::wostringstream w; w << L"(c) 2021 <benjaminhampe@gmx.de>";
      drawFancy( x,y, w.str(), 0xFFEEEEEE, grey, align, font ); y += ln;
   }

   if ( camera )
   {
      align = Align::BottomLeft;
      ln = dbGetTextSize( L"WM_01°!", font ).lineHeight + p;
      x = p;
      y = h - p - 3*ln;
      {
         std::wostringstream w;
         w << L"ViewMatrix(" << camera->getViewMatrix() << ")";
         drawFancy( x,y, w.str(), 0xFF1BAB1B, grey, align, font2 ); y += ln;
      }
      {
         std::wostringstream w;
         w << L"ProjectionMatrix(" << camera->getProjectionMatrix() << ")";
         drawFancy( x,y, w.str(), 0xFF3CBB3B, grey, align, font2 ); y += ln;
      }
      {
         std::wostringstream w;
         w << L"ViewProjectionMatrix(" << camera->getViewProjectionMatrix() << ")";
         drawFancy( x,y, w.str(), 0xFF2BAB2B, grey, align, font2 ); y += ln;
      }
//      {
//         std::wostringstream w;
//         w << L"ModelMatrix(" << m_Driver->getModelMatrix() << ")";
//         drawFancy( x,y, w.str(), 0xFFAB3B3B, align, font2 ); y += ln;
//      }
   }
}
*/

} // end namespace gpu.
} // end namespace de.
