#include "QFont5x8.hpp"
#include <QPainter>
#include <QDebug>

QFont5x8::QFont5x8()
   : quadWidth( 2 )
   , quadHeight( 2 )
   , quadSpacingX( 0 )
   , quadSpacingY( 0 )
   , glyphSpacingX( 1 )
   , glyphSpacingY( 1 )
{
   createGlyphCache( m_GlyphCache );
}

uint64_t
QFont5x8::operator< ( QFont5x8 const & o ) const
{
   if ( quadWidth < o.quadWidth ) return true;
   if ( quadHeight < o.quadHeight ) return true;
   if ( quadSpacingX < o.quadSpacingX ) return true;
   if ( quadSpacingY < o.quadSpacingY ) return true;
   if ( glyphSpacingX < o.glyphSpacingX ) return true;
   if ( glyphSpacingY < o.glyphSpacingY ) return true;
   return false;
}

glm::ivec2
QFont5x8::getTextSize( QString const & msg ) const
{
   return getTextSize( msg, *this );
}

// static
void
QFont5x8::createGlyphCache( std::map< uint32_t, QFont5x8Matrix > & cache )
{
   auto addGlyph = [&] ( uint32_t unicode,
      std::string const & row0 = "", std::string const & row1 = "",
      std::string const & row2 = "", std::string const & row3 = "",
      std::string const & row4 = "", std::string const & row5 = "",
      std::string const & row6 = "", std::string const & row7 = "" )
   {
      //if ( cache.find( unicode ) != cache.end() ) { return; }
      QFont5x8Matrix glyph;
      glyph.setRow(0,row0); glyph.setRow(1,row1); glyph.setRow(2,row2); glyph.setRow(3,row3);
      glyph.setRow(4,row4); glyph.setRow(5,row5); glyph.setRow(6,row6); glyph.setRow(7,row7);
      cache[ unicode ] = glyph;
   };

   addGlyph(' ', "","","","","","","" );
   addGlyph(',', "","","","","","  ++","   +","  +" );
   addGlyph(';', "","","  ++","  ++","","  ++","   +","  +" );
   addGlyph('.', "","","","","","  ++","  ++" );
   addGlyph(':', "","  ++","  ++","","  ++","  ++","" );
   addGlyph('*', "","  +  ","+ + +"," +++ ","+ + +","  +  ","" );
   addGlyph('~', ""," +   ","+ + +","   + ","","","" );
   addGlyph(L'^',"  +"," + +","+   +","","","","" );
   //addGlyph(L"°","  ++"," +  +","  ++","","","","" );
   addGlyph('"'," + +"," + +","","","","","" );
   addGlyph('\'',"  +","  +","","","","","" );
   addGlyph('\\',"","+"," +","  +","   +","","" );
   addGlyph('/',"","   +","  +"," +","+","","" );
   addGlyph('#'," + +"," + +","+++++"," + +","+++++"," + +"," + +" );
   addGlyph('!',"  +","  +","  +","  +","","  +","  +" );
   addGlyph('?'," +++","+   +","    +","   +","  +","","  +" );
   addGlyph('_',"","","","","","","+++++" );
   addGlyph('$',"  +  "," ++++","+ +  "," +++ ","  + +","++++ ","  +  " );
   addGlyph('%', "++   ","++  +","   + ","  +  "," +   ","+  ++","   ++" );
   addGlyph('&', " +   ","+ +  ","+ +  "," +   ","+ + +","+  +"," ++ +" );
   addGlyph('@', " +++ ","+   +","+ + +","+ +++","+ +  ","+    "," ++++" );
   addGlyph('<', "   +","  +"," +","+"," +","  +","   +" );
   addGlyph('>', " +","  +","   +","    +","   +","  +"," +" );
   addGlyph('|', "  +","  +","  +","  +","  +","  +","  +" );
   addGlyph('=', "","","+++++","","+++++","","" );
   addGlyph('+', "","  +","  +","+++++","  +","  +","" );
   addGlyph('-', "","","","+++++","","","" );
   addGlyph('[', "  ++","  +","  +","  +","  +","  +","  ++" );
   addGlyph(']', " ++","  +","  +","  +","  +","  +"," ++" );
   addGlyph('(', "   +","  +"," +"," +"," +","  +","   +" );
   addGlyph(')', " +","  +","   +","   +","   +","  +"," +" );
   addGlyph('{', "   ++","  +","  +"," +","  +","  +","   ++" );
   addGlyph('}', "++","  +","  +","   +","  +","  +","++" );
   addGlyph('0', " +++ ","+   +","+   +","+ + +","+   +","+   +"," +++ " );
   addGlyph('1', "  +  "," ++  ","  +  ","  +  ","  +  ","  +  "," +++ " );
   addGlyph('2', "  ++ "," +  +","    +","   + ","  +  "," +   ","+++++" );
   addGlyph('3', "+++++","   + ","  +  ","   + ","    +","+   +"," +++ " );
   addGlyph('4', "   + ","  ++ "," + + ","+  + ","+++++","   + ","   + " );
   addGlyph('5', "+++++","+    ","++++ ","    +","    +","+   +"," +++ " );
   addGlyph('6', "  ++ "," +   ","+    ","++++ ","+   +","+   +"," +++ " );
   addGlyph('7', "+++++","    +","   + ","  +  "," +   "," +   "," +   " );
   addGlyph('8', " +++ ","+   +","+   +"," +++ ","+   +","+   +"," +++ " );
   addGlyph('9', " +++ ","+   +","+   +"," ++++","    +","   + "," ++  " );
   addGlyph('A', " +++ ","+   +","+   +","+++++","+   +","+   +","+   +" );
   addGlyph('B', "++++ ","+   +","+   +","++++ ","+   +","+   +","++++ " );
   addGlyph('C', " +++ ","+   +","+    ","+    ","+    ","+   +"," +++ " );
   addGlyph('D', "++++ ","+   +","+   +","+   +","+   +","+   +","++++ " );
   addGlyph('E', "+++++","+    ","+    ","++++ ","+    ","+    ","+++++" );
   addGlyph('F', "+++++","+    ","+    ","++++ ","+    ","+    ","+    " );
   addGlyph('G', " +++ ","+   +","+    ","+ +++","+   +","+   +"," +++ " );
   addGlyph('H', "+   +","+   +","+   +","+++++","+   +","+   +","+   +" );
   addGlyph('I', " +++ ","  +  ","  +  ","  +  ","  +  ","  +  "," +++ " );
   addGlyph('J', "  +++","   +" ,"   +" ,"   +" ,"   +" ,"+  +" ," ++ " );
   addGlyph('K', "+   +","+  + ","+ +  ","++   ","+ +  ","+  + ","+   +" );
   addGlyph('L', "+    ","+    ","+    ","+    ","+    ","+    ","+++++" );
   addGlyph('M', "+   +","++ ++","+ + +","+   +","+   +","+   +","+   +" );
   addGlyph('N', "+   +","++  +","+ + +","+  ++","+   +","+   +","+   +" );
   addGlyph('O', " +++ ","+   +","+   +","+   +","+   +","+   +"," +++ " );
   addGlyph('P', "++++","+   +","+   +","++++","+","+","+" );
   addGlyph('Q', " +++ ","+   +","+   +","+   +","+ + +","+  + "," ++ +" );
   addGlyph('R', "++++","+   +","+   +","++++","+ +","+  +","+   +" );
   addGlyph('S', " ++++","+    ","+    "," +++ ","    +","    +","++++" );
   addGlyph('T', "+++++","  +","  +","  +","  +","  +","  +" );
   addGlyph('U', "+   +","+   +","+   +","+   +","+   +","+   +"," +++ " );
   addGlyph('V', "+   +","+   +","+   +","+   +","+   +"," + +","  + " );
   addGlyph('W', "+   +","+   +","+   +","+ + +","+ + +","+ + +"," + + " );
   addGlyph('X', "+   +","+   +"," + + ","  +  "," + + ","+   +","+   +" );
   addGlyph('Y', "+   +","+   +","+   +"," + +","  +","  +","  +" );
   addGlyph('Z', "+++++","    +","   +","  +"," +","+","+++++" );
   addGlyph('a', "",""," +++ ","    +"," ++++","+   +"," ++++" );
   addGlyph('b', "+    ","+    ","+ ++ ","++  +","+   +","+   +","++++ " );
   addGlyph('c', "",""," +++ ","+    ","+    ","+   +"," +++ " );
   addGlyph('d', "    +","    +"," ++ +","+  ++","+   +","+   +"," ++++" );
   addGlyph('e', "",""," +++ ","+   +","+++++","+    "," +++ " );
   addGlyph('f', "  ++ "," +  +"," +   ","+++  "," +   "," +   "," +   " );
   addGlyph('g', "",""," +++ ","+   +","+   +"," ++++","    +"," +++ " );
   addGlyph('h', "+    ","+    ","+ ++ ","++  +","+   +","+   +","+   +" );
   addGlyph('i', "  +  ","     "," ++  ","  +  ","  +  ","  +  "," +++ " );
   addGlyph('j', "    +","     ","   ++","    +","    +","    +","+  +"," ++" );
   addGlyph('k', " +   "," +   "," +  +"," + + "," ++  "," + + "," +  +" );
   addGlyph('l', " ++","  +","  +","  +","  +","  +"," +++" );
   addGlyph('m', "","","++ + ","+ + +","+ + +","+   +","+   +" );
   addGlyph('n', "","","+ ++ ","++  +","+   +","+   +","+   +" );
   addGlyph('o', "",""," +++ ","+   +","+   +","+   +"," +++" );
   addGlyph('p', "","","++++ ","+   +","+   +","++++","+","+" );
   addGlyph('q', "",""," ++ +","+  ++","+   +"," ++++","    +","    +" );
   addGlyph('r', "","","+ ++ ","++  +","+    ","+    ","+    " );
   addGlyph('s', "     ","     "," +++ ","+    "," +++ ","    +","++++ " );
   addGlyph('t', " +   "," +   ","+++  "," +   "," +   "," +  +","  ++ " );
   addGlyph('u', "","","+   +","+   +","+   +","+  ++"," ++ +" );
   addGlyph('v', "","","+   +","+   +","+   +"," + +","  +" );
   addGlyph('w', "","","+   +","+   +","+ + +","+ + +"," + +" );
   addGlyph('x', "","","+   +"," + + ","  +  "," + + ","+   +" );
   addGlyph('y', "","","+   +","+   +","+   +"," ++++","    +"," +++" );
   addGlyph('z', "","","+++++","   +","  +"," +","+++++" );
}

// static
glm::ivec2
QFont5x8::getTextSize( QString const & msg, QFont5x8 const & font )
{
   if ( msg.isEmpty() ) return glm::ivec2(0,0);

   int32_t lineCount = 1;
   int32_t lineWidth = 0;
   int32_t lineWidthMax = 0;

   for ( size_t i = 0; i < msg.size(); ++i )
   {
      uint32_t ch = msg.at( i ).unicode();
      if ( ch == '\r' )  // Mac or Windows line breaks.
      {
         lineCount++;
         lineWidthMax = std::max( lineWidthMax, lineWidth );
         lineWidth = 0;
         continue;
      }
      if ( ch == '\n' )	// Mac or Windows line breaks.
      {
         lineCount++;
         lineWidthMax = std::max( lineWidthMax, lineWidth );
         lineWidth = 0;
         continue;
      }

      lineWidth++;
   }
   lineWidthMax = std::max( lineWidthMax, lineWidth );

   int tx = 5*(font.quadWidth + font.quadSpacingX) * lineWidthMax
               + (lineWidthMax-1) * font.glyphSpacingX;
   int ty = 8*(font.quadHeight + font.quadSpacingY)* lineCount
               + (lineCount-1) * font.glyphSpacingY;

   return glm::ivec2( tx, ty );
}

void
QFont5x8::drawText( QPainter & dc, int x, int y, QString const & msg,
            uint32_t color, de::Align::EAlign align )
{
   int cx = quadWidth + quadSpacingX;
   int cy = quadHeight + quadSpacingY;
   int gw = 5 * cx + glyphSpacingX;
   int gh = 8 * cy + glyphSpacingY;

   // Align: default is TopLeft
   glm::ivec2 aligned_pos(x,y);

   if ( align != de::Align::TopLeft )
   {
      glm::ivec2 ts = getTextSize( msg ); // Optimize here for single lines, default: multiline but slower.
      float tw = ts.x;
      float th = ts.y;

           if ( align & de::Align::Center ){ aligned_pos.x -= tw/2; }
      else if ( align & de::Align::Right ) { aligned_pos.x -= tw;   }
           if ( align & de::Align::Middle ){ aligned_pos.y -= th/2; }
      else if ( align & de::Align::Bottom ){ aligned_pos.y -= th;   }
   }

   dc.setBrush( Qt::NoBrush );
   uint32_t colorQt = de::RGBA( de::RGBA_B(color), de::RGBA_G(color), de::RGBA_R(color), de::RGBA_A(color) );
   dc.setPen( QPen( QColor( colorQt ) ) );

   auto g = aligned_pos;

   for ( size_t u = 0; u < msg.size(); ++u )
   {
      uint32_t ch = msg.at( u ).unicode();
      if ( ch == '\r' )  // Mac or Windows line breaks.
      {
         g.x = aligned_pos.x;
         g.y += gh;
         continue;
      }
      if ( ch == '\n' )	// Mac or Windows line breaks.
      {
         g.x = aligned_pos.x;
         g.y += gh;
         continue;
      }

      //char ch = char( std::clamp( unicode, uint32_t(30), uint32_t(127) ) );
      QFont5x8Matrix const & m = m_GlyphCache[ ch ];

      // Add dot matrix 5mal8 as max 49 quads
      for ( int j = 0; j < 8; ++j )
      {
         for ( int i = 0; i < 5; ++i )
         {
            if ( !m.get( i,j ) ) continue; // is white or black.

            int x1 = g.x + i * cx;
            int y1 = g.y + j * cy;
//            dc.drawPoint( x1,y1 );

            for ( int ky = 0; ky < quadHeight; ++ky )
            {
               for ( int kx = 0; kx < quadWidth; ++kx )
               {
                  int x2 = x1 + kx;
                  int y2 = y1 + ky;
                  dc.drawPoint( x2,y2 );
               }
            }
         }
      }

      g.x += gw;
   }
}



void
QFont5x8::drawText( QImage & o, int x, int y, QString const & msg, uint32_t color, de::Align::EAlign align )
{
   int cx = quadWidth + quadSpacingX;
   int cy = quadHeight + quadSpacingY;
   int gw = 5 * cx + glyphSpacingX;
   int gh = 8 * cy + glyphSpacingY;

   // Align: default is TopLeft
   glm::ivec2 aligned_pos(x,y);

   if ( align != de::Align::TopLeft )
   {
      glm::ivec2 ts = getTextSize( msg ); // Optimize here for single lines, default: multiline but slower.
      float tw = ts.x;
      float th = ts.y;

           if ( align & de::Align::Center ){ aligned_pos.x -= tw/2; }
      else if ( align & de::Align::Right ) { aligned_pos.x -= tw;   }
           if ( align & de::Align::Middle ){ aligned_pos.y -= th/2; }
      else if ( align & de::Align::Bottom ){ aligned_pos.y -= th;   }
   }

   auto g = aligned_pos;

   for ( size_t u = 0; u < msg.size(); ++u )
   {
      uint32_t ch = msg.at( u ).unicode();

      if ( ch == '\r' )  // Mac or Windows line breaks.
      {
         g.x = aligned_pos.x;
         g.y += gh;
         continue;
      }
      if ( ch == '\n' )	// Mac or Windows line breaks.
      {
         g.x = aligned_pos.x;
         g.y += gh;
         continue;
      }

      //char ch = char( std::clamp( unicode, uint32_t(30), uint32_t(127) ) );
      QFont5x8Matrix & m = m_GlyphCache[ ch ];

      // Add dot matrix 5mal8 as max 49 quads
      for ( int j = 0; j < 8; ++j )
      {
         for ( int i = 0; i < 5; ++i )
         {
            int x1 = g.x + i * cx;
            int y1 = g.y + j * cy;

            if ( m.get( i,j ) )
            {
               for ( int ky = 0; ky < quadHeight; ++ky )
               {
                  for ( int kx = 0; kx < quadWidth; ++kx )
                  {
                     int x2 = x1 + kx;
                     int y2 = y1 + ky;
                     o.setPixel( x2,y2, color );
                  }
               }
            }
         }
      }
      g.x += gw;
   }
}

/*

void
Font5x8Face::add2DText( SMeshBuffer & o, int x, int y, std::string const & msg, uint32_t color, Align::EAlign align )
{
   cacheString( msg );

   o.setPrimitiveType( PrimitiveType::Triangles );
   o.setBlend( Blend::alphaBlend() );
   o.moreVertices( 4*msg.size() );
   o.moreIndices( 6*msg.size() );

   int cx = font.quadWidth + font.quadSpacingX;
   int cy = font.quadHeight + font.quadSpacingY;
   int gw = 5 * cx + font.glyphSpacingX;
   int gh = 8 * cy + font.glyphSpacingY;

   // Align: default is TopLeft
   glm::ivec2 aligned_pos(x,y);

   if ( align != Align::TopLeft )
   {
      glm::ivec2 ts = getTextSize( msg ); // Optimize here for single lines, default: multiline but slower.
      float tw = ts.x;
      float th = ts.y;

           if ( align & Align::Center ){ aligned_pos.x -= tw/2; }
      else if ( align & Align::Right ) { aligned_pos.x -= tw;   }
           if ( align & Align::Middle ){ aligned_pos.y -= th/2; }
      else if ( align & Align::Bottom ){ aligned_pos.y -= th;   }
   }

   auto g = aligned_pos;

   for ( size_t u = 0; u < msg.size(); ++u )
   {
      char ch = msg[ u ];

      if ( ch == '\r' )  // Mac or Windows line breaks.
      {
         g.x = aligned_pos.x;
         g.y += gh;
         continue;
      }
      if ( ch == '\n' )	// Mac or Windows line breaks.
      {
         g.x = aligned_pos.x;
         g.y += gh;
         continue;
      }

      //char ch = char( std::clamp( unicode, uint32_t(30), uint32_t(127) ) );
      Font5x8Matrix & m = m_GlyphCache[ ch ];

      // Add dot matrix 5mal8 as max 49 quads
      for ( int j = 0; j < 8; ++j ) {
      for ( int i = 0; i < 5; ++i ) {

         int x1 = g.x + i * cx;
         int y1 = g.y + j * cy;

         int x2 = x1 + font.quadWidth;
         int y2 = y1 + font.quadHeight;

         if ( m.get( i,j ) )
         {
            S3DVertex A( x1, y1, -1, 0,0,-1, color, 0,1 );
            S3DVertex B( x1, y2, -1, 0,0,-1, color, 0,0 );
            S3DVertex C( x2, y2, -1, 0,0,-1, color, 1,0 );
            S3DVertex D( x2, y1, -1, 0,0,-1, color, 1,1 );
            o.addQuad( A,B,C,D );
         }
      }
      }

      g.x += gw;
   }
}

*/
