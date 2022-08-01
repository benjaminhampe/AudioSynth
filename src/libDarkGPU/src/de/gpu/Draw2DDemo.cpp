#include <de/gpu/Draw2DDemo.hpp>

namespace de {
namespace gpu {

namespace
{
   constexpr float const CONST_Z_INIT = 0.90f; // At 1.0 it disappears, not inside frustum.
   constexpr float const CONST_Z_STEP = -0.00001f; // enough for 200.000 elements, TODO: test more.
}

Draw2DDemo::Draw2DDemo( IVideoDriver* driver )
   : m_Driver( driver )
{
}

Draw2DDemo::~Draw2DDemo()
{

}

void
Draw2DDemo::setDriver( IVideoDriver* driver ) { m_Driver = driver; }

void
Draw2DDemo::draw()
{
   if (!m_Driver)
   {
      DE_ERROR("No driver") return;
   }

   int w = m_Driver->getScreenWidth();
   int h = m_Driver->getScreenHeight();

   TexRef ref = m_Driver->getTexture( "../../screenshots/gpu6.png" );
   if ( !ref.tex )
   {
      ref = m_Driver->addTexture2D( "../../screenshots/gpu6.png", false );
   }

   m_2DRenderer.clear();
   m_2DRenderer.setDriver( m_Driver );

//   Font5x8 font;
//   font.quadWidth = 1;
//   font.quadHeight = 1;
//   std::string name = "Hello Font5x8 can display much text per frame!?#_WURSTBROT";
//   glm::ivec2 ts = font.getTextSize( name );

//   int x = 0;
//   int y = 0;
//   int a = 10;
//   for ( int i = 0; i < 6; ++i )
//   {
//      for ( int j = 0; j < 150; ++j )
//      {
//         m_2DRenderer.add2DText( x,y, name, randomRGBA( 10 + 20*i ), Align::TopLeft, font );
//         y += ts.y;
//      }
//      x += ts.x+10;
//      y = 0;
//   }


//   int cx = 32;
//   int cy = 32;
//   int k = w/32;
//   int m = h/32;
//   float dk = 255.0f / float( k );
//   float dm = 255.0f / float( m );

//   for ( int j = 0; j <= 20; ++j )
//   {
//      for ( int i = 0; i <= 20; ++i )
//      {
//         //uint8_t c = clampByte( Math::round32( dk * i ) );
//         uint8_t r = randomByte();
//         uint8_t g = randomByte();
//         uint8_t b = randomByte();
//         uint8_t a = clampByte( Math::round32( dm * j ) );
//         if ( j % 2 == 1 )
//         {
//            a = clampByte( 255 - int(a) + 40 );
//         }
//         uint32_t colorA = RGBA( r,g,b,a );
//         r = randomByte();
//         g = randomByte();
//         b = randomByte();
//         uint32_t colorB = RGBA( r,g,b,a );
//         r = randomByte();
//         g = randomByte();
//         b = randomByte();
//         uint32_t colorC = RGBA( r,g,b,a );
//         r = randomByte();
//         g = randomByte();
//         b = randomByte();
//         uint32_t colorD = RGBA( r,g,b,a );

//         int x = i*cx;
//         int y = j*cy;

//         m_2DRenderer.add2DRect( Recti(x,y,cx,cy), colorA, colorB, colorC, colorD );
//         m_2DRenderer.add2DRectOutline( Recti(x,y,cx,cy), colorC, colorD, colorA, colorB, TexRef(), 5 );
//      }
//   }

//   DE_DEBUG("w(",w,"), h(",h,")")

   auto d = Recti(w-300-3,2,300,200);
   auto r = glm::ivec2(32,32);
   m_2DRenderer.add2DRoundRect( d,r, 0x40FFFFFF, ref, 5 );
   m_2DRenderer.add2DRoundRectOutline( d,r, 0xFFFFFFFF, TexRef(), 5, 5 );

   d = Recti(w-64-1-5,201,64,32);
   r = glm::ivec2(10,10);
   m_2DRenderer.add2DRoundRect( d,r, 0x40FFFFFF, ref, 5 );
   m_2DRenderer.add2DRoundRectOutline( d,r, 0xFFFFFFFF, TexRef(), 5, 5 );

   d = Recti(w-200-1-5,201,64,64);
   r = glm::ivec2(16,16);
   m_2DRenderer.add2DRoundRect( d,r, 0x40FFFFFF, ref, 16 );
   m_2DRenderer.add2DRoundRectOutline( d,r, 0xFFFFFFFF, TexRef(), 5, 16 );

   d = Recti(w-320,201,64,64);
   r = glm::ivec2(32,32);
   m_2DRenderer.add2DRoundRect( d,r, 0x40FFFFFF, ref, 32 );
   m_2DRenderer.add2DRoundRectOutline( d,r, 0xFFFFFFFF, TexRef(), 5, 32 );

//   d = Recti(w-64-1,250,64,64);
//   r = glm::ivec2(32,32);
//   m_2DRenderer.add2DRoundRect( d,r, 0x40FFFFFF, ref, 1 );
//   m_2DRenderer.add2DRoundRectOutline( d,r, 0xA000FFFF, TexRef(), 1 );

   d = Recti(w-2*70-1,250,64,64);
   r = glm::ivec2(32,32);
   m_2DRenderer.add2DRoundRect( d,r, 0x40FFFFFF, ref, 2 );
   m_2DRenderer.add2DRoundRectOutline( d,r, 0xFF00FFFF, TexRef(), 5, 2 );

   d = Recti(w-3*70-1,250,64,64);
   r = glm::ivec2(32,32);
   m_2DRenderer.add2DRoundRect( d,r, 0x40FFFFFF, ref, 3 );
   m_2DRenderer.add2DRoundRectOutline( d,r, 0xFF00FFFF, TexRef(), 5, 3 );

   d = Recti(w-4*70-1,250,64,64);
   r = glm::ivec2(32,32);
   m_2DRenderer.add2DRoundRect( d,r, 0x40FFFFFF, ref, 4 );
   m_2DRenderer.add2DRoundRectOutline( d,r, 0xFF00FFFF, TexRef(), 5, 4 );

   d = Recti(w-5*70-1,250,64,64);
   r = glm::ivec2(32,32);
   m_2DRenderer.add2DRoundRect( d,r, 0x40FFFFFF, ref, 4 );
   m_2DRenderer.add2DRoundRectOutline( d,r, 0xFF00FFFF, TexRef(), 5, 4 );

   m_2DRenderer.render();
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
Draw2DDemo::drawImmediate()
{
   if (!m_Driver) return;

   int w = m_Driver->getScreenWidth();
   int h = m_Driver->getScreenHeight();

   TexRef ref = m_Driver->getTexture( "../../screenshots/gpu6.png" );
   if ( !ref.tex )
   {
      ref = m_Driver->addTexture2D( "../../screenshots/gpu6.png", false );
   }

   m_Driver->draw2DRect( Recti(0,0,w,h ), 0xFFFFFFFF, ref );

   int cx = 32;
   int cy = 32;
   int k = w/32;
   int m = h/32;
   float dk = 255.0f / float( k );
   float dm = 255.0f / float( m );

   for ( int j = 0; j <= m; ++j )
   {
      for ( int i = 0; i <= k; ++i )
      {
         //uint8_t c = clampByte( Math::round32( dk * i ) );
         uint8_t r = randomByte();
         uint8_t g = randomByte();
         uint8_t b = randomByte();
         uint8_t a = clampByte( Math::round32( dm * j ) );
         uint32_t colorA = RGBA( r,g,b,a );
         r = randomByte();
         g = randomByte();
         b = randomByte();
         uint32_t colorB = RGBA( r,g,b,a );
         r = randomByte();
         g = randomByte();
         b = randomByte();
         uint32_t colorC = RGBA( r,g,b,a );
         r = randomByte();
         g = randomByte();
         b = randomByte();
         uint32_t colorD = RGBA( r,g,b,a );

         int x = i*cx;
         if ( j % 2 == 1 )
         {
            x = (k-i)*cx;
         }
         int y = j*cy;
         //uint32_t color2 = RGBA( clampByte(r+50),clampByte(g+50),clampByte(b+50),clampByte(a+50) );
         m_Driver->draw2DRect( Recti(x,y,cx,cy), colorA, colorB, colorC, colorD );
         m_Driver->draw2DRectOutline( Recti(x,y,cx,cy), colorC, colorD, colorA, colorB );
      }
   }

//   m_Driver->draw2DRect( Recti(0,0,32,32), 0xFFFFFFFF );
//   m_Driver->draw2DRect( Recti(16,16,8,8), 0xFF111111 );
//   m_Driver->draw2DRectOutline( Recti(16,16,8,8), 0xFF2288FF, 0xFF4488FF, 0xFF3388FF, 0xFF5588FF );

//   m_Driver->draw2DRect( Recti(32,0,32,32), 0xFF888888 );
//   m_Driver->draw2DRect( Recti(64,0,32,32), 0xFFCCCCCC );
//   m_Driver->draw2DRect( Recti(96,0,4,100), 0xFFAAAAAA );

}
*/


} // end namespace gpu.
} // end namespace de.
