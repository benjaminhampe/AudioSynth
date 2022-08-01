#include <de/Font5x8.hpp>

namespace de {

Font5x8::Font5x8()
   : quadWidth( 1 )
   , quadHeight( 1 )
   , quadSpacingX( 0 )
   , quadSpacingY( 0 )
   , glyphSpacingX( 1 )
   , glyphSpacingY( 1 )
{
   createGlyphCache( m_GlyphCache );
}

uint64_t
Font5x8::operator< ( Font5x8 const & o ) const
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
Font5x8::getTextSize( std::string const & msg ) const
{
   return getTextSize( msg, *this );
}

// static
glm::ivec2
Font5x8::getTextSize( std::string const & msg, Font5x8 const & font )
{
   if ( msg.empty() ) return glm::ivec2(0,0);

   int32_t lineCount = 1;
   int32_t lineWidth = 0;
   int32_t lineWidthMax = 0;

   for ( size_t i = 0; i < msg.size(); ++i )
   {
      uint32_t ch = msg[ i ];
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
Font5x8::drawText( Image & o, int x, int y, std::string const & msg, uint32_t color, de::Align::EAlign align )
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
      else {}
           if ( align & de::Align::Middle ){ aligned_pos.y -= th/2; }
      else if ( align & de::Align::Bottom ){ aligned_pos.y -= th;   }
      else {}
   }

   auto g = aligned_pos;

   int w = o.getWidth();
   int h = o.getHeight();

   for ( size_t u = 0; u < msg.size(); ++u )
   {
      uint32_t ch = msg[ u ];

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
                     if ( x2 < 0 || x2 >= w )
                     {
                        //DE_DEBUG("Invalid x(",x2,") of w(",w,")")
                        continue;
                     }

                     int y2 = y1 + ky;
                     if ( y2 < 0 || y2 >= h )
                     {
                        //DE_DEBUG("Invalid y(",y2,") of h(",h,")")
                        continue;
                     }

                     o.setPixel( x2,y2, color );
                  }
               }
            }
         }
      }
      g.x += gw;
   }
}


// static
void
Font5x8::createGlyphCache( std::map< uint32_t, Font5x8Matrix > & cache )
{
   auto addGlyph = [&] ( uint32_t unicode,
      std::string const & row0 = "", std::string const & row1 = "",
      std::string const & row2 = "", std::string const & row3 = "",
      std::string const & row4 = "", std::string const & row5 = "",
      std::string const & row6 = "", std::string const & row7 = "" )
   {
      //if ( cache.find( unicode ) != cache.end() ) { return; }
      Font5x8Matrix glyph;
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

} // end namespace de