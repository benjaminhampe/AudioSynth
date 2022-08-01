#include <de/gpu/Font5x8Renderer.hpp>
#include <de/gpu/IVideoDriver.hpp>

namespace de {
namespace gpu {

// ===========================================================================
struct Font5x8Manager
// ===========================================================================
{
   DE_CREATE_LOGGER("de.gpu.Font5x8Manager")

   static Font5x8Manager &
   getInstance()
   {
      static Font5x8Manager s_Instance;
      return s_Instance;
   }

   Font5x8Manager() {}
   //~Font5x8Manager() {}

   Font5x8Face::SharedPtr
   getFont5x8( Font5x8 const & font )
   {
      auto it = m_FontCache.find( font );
      if ( it != m_FontCache.end() )
      {
         return it->second;
      }

      Font5x8Face::SharedPtr fontImpl = std::make_shared< Font5x8Face >();
      fontImpl->font = font;
      m_FontCache[ font ] = fontImpl;
      return m_FontCache[ font ];
   }

   std::map< Font5x8, Font5x8Face::SharedPtr > m_FontCache;
};


Font5x8Face::SharedPtr
getFont5x8( Font5x8 const & font )
{
   return Font5x8Manager::getInstance().getFont5x8( font );
}

// ===========================================================================

glm::ivec2
Font5x8Face::getTextSize( std::string const & msg ) const
{
   return Font5x8::getTextSize( msg, font );
}

void
Font5x8Face::cacheString( std::string const & msg )
{
   for ( size_t i = 0; i < msg.size(); ++i )
   {
      char ch = msg[ i ];
      auto it = m_GlyphCache.find( ch );
      if ( it == m_GlyphCache.end() )
      {
         m_GlyphCache[ ch ] = Font5x8::getGlyph( ch );
      }
   }
}

void
Font5x8Face::add2DText( SMeshBuffer & o, int x, int y, std::string const & msg, uint32_t color, Align::EAlign align )
{
   cacheString( msg );

   o.setPrimitiveType( PrimitiveType::Triangles );
   o.setLighting( 0 );
   //o.setBlend( Blend::disabled() );
   //o.setCulling( Culling::disabled() );
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
      else {}
           if ( align & Align::Middle ){ aligned_pos.y -= th/2; }
      else if ( align & Align::Bottom ){ aligned_pos.y -= th;   }
      else {}
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

// ===========================================================================


Font5x8Renderer::Font5x8Renderer()
   : m_Driver( nullptr )
{}

Font5x8Renderer::~Font5x8Renderer()
{}

glm::ivec2
Font5x8Renderer::getTextSize( std::string const & msg, Font5x8 const & font ) const
{
   return Font5x8::getTextSize( msg, font );
}

void
Font5x8Renderer::add2DText(
   SMeshBuffer & o,
   int x,
   int y,
   std::string const & msg,
   uint32_t color,
   Align::EAlign align,
   Font5x8 const & font )
{
   getFont5x8( font )->add2DText( o,x,y,msg,color,align );
}

void
Font5x8Renderer::draw2DText(
   int x,
   int y,
   std::string const & msg,
   uint32_t color,
   Align::EAlign align,
   Font5x8 const & font )
{
   if ( !m_Driver )
   {
      DE_ERROR("No driver")
      return;
   }

   SMeshBuffer o;
   getFont5x8( font )->add2DText( o, x, y, msg, color, align );
   m_Driver->draw2D( o );
}


// ===========================================================================


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
      char ch = msg[ i ];

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

// static
Font5x8Matrix
Font5x8::getGlyph( char ch )
{
   Font5x8Matrix retVal;

   auto addGlyph = [&] ( char unicode,
      std::string const & row0, std::string const & row1,
      std::string const & row2, std::string const & row3,
      std::string const & row4, std::string const & row5,
      std::string const & row6, std::string const & row7 = "" )
   {
      retVal.clear();
      retVal.setRow(0,row0); retVal.setRow(1,row1); retVal.setRow(2,row2); retVal.setRow(3,row3);
      retVal.setRow(4,row4); retVal.setRow(5,row5); retVal.setRow(6,row6); retVal.setRow(7,row7);
   };

   switch ( ch ) // 5x8
   {
   case ' ': addGlyph( ch, "",
                           "",
                           "",
                           "",
                           "",
                           "",
                           "" ); break;

   case ',': addGlyph( ch, "",
                           "",
                           "",
                           "",
                           "",
                           "  ++",
                           "   +",
                           "  +" ); break;

   case ';': addGlyph( ch, "",
                           "",
                           "  ++",
                           "  ++",
                           "",
                           "  ++",
                           "   +",
                           "  +" ); break;

   case '.': addGlyph( ch, "",
                           "",
                           "",
                           "",
                           "",
                           "  ++",
                           "  ++" ); break;

   case ':': addGlyph( ch, "",
                           "  ++",
                           "  ++",
                           "",
                           "  ++",
                           "  ++",
                           "" ); break;

   case '*': addGlyph( ch, "",
                           "  +  ",
                           "+ + +",
                           " +++ ",
                           "+ + +",
                           "  +  ",
                           "" ); break;

   case '~': addGlyph( ch, "",
                           " +   ",
                           "+ + +",
                           "   + ",
                           "",
                           "",
                           "" ); break;

   case L'^': addGlyph( ch,"  +",
                           " + +",
                           "+   +",
                           "",
                           "",
                           "",
                           "" ); break;

//      case L"°": addGlyph( ch,"  ++",
//                              " +  +",
//                              "  ++",
//                              "",
//                              "",
//                              "",
//                              "" ); break;

   case '"': addGlyph( ch, " + +",
                           " + +",
                           "",
                           "",
                           "",
                           "",
                           "" ); break;

   case '\'': addGlyph( ch,"  +",
                           "  +",
                           "",
                           "",
                           "",
                           "",
                           "" ); break;

   case '\\': addGlyph( ch,"",
                           "+",
                           " +",
                           "  +",
                           "   +",
                           "",
                           "" ); break;

   case '/': addGlyph( ch,"",
                           "   +",
                           "  +",
                           " +",
                           "+",
                           "",
                           "" ); break;

   case '#': addGlyph( ch, " + +",
                           " + +",
                           "+++++",
                           " + +",
                           "+++++",
                           " + +",
                           " + +" ); break;

   case '!': addGlyph( ch, "  +",
                           "  +",
                           "  +",
                           "  +",
                           "",
                           "  +",
                           "  +" ); break;

   case '?': addGlyph( ch, " +++",
                           "+   +",
                           "    +",
                           "   +",
                           "  +",
                           "",
                           "  +" ); break;

   case '_': addGlyph( ch, "",
                           "",
                           "",
                           "",
                           "",
                           "",
                           "+++++" ); break;

   case '$': addGlyph( ch, "  +  ",
                           " ++++",
                           "+ +  ",
                           " +++ ",
                           "  + +",
                           "++++ ",
                           "  +  " ); break;

   case '%': addGlyph( ch, "++   ",
                           "++  +",
                           "   + ",
                           "  +  ",
                           " +   ",
                           "+  ++",
                           "   ++" ); break;

   case '&': addGlyph( ch, " +   ",
                           "+ +  ",
                           "+ +  ",
                           " +   ",
                           "+ + +",
                           "+  +",
                           " ++ +" ); break;

   case '@': addGlyph( ch, " +++ ",
                           "+   +",
                           "+ + +",
                           "+ +++",
                           "+ +  ",
                           "+    ",
                           " ++++" ); break;

   case '<': addGlyph( ch, "   +",
                           "  +",
                           " +",
                           "+",
                           " +",
                           "  +",
                           "   +" ); break;

   case '>': addGlyph( ch, " +",
                           "  +",
                           "   +",
                           "    +",
                           "   +",
                           "  +",
                           " +" ); break;

   case '|': addGlyph( ch, "  +",
                           "  +",
                           "  +",
                           "  +",
                           "  +",
                           "  +",
                           "  +" ); break;

   case '=': addGlyph( ch, "",
                           "",
                           "+++++",
                           "",
                           "+++++",
                           "",
                           "" ); break;

   case '+': addGlyph( ch, "",
                           "  +",
                           "  +",
                           "+++++",
                           "  +",
                           "  +",
                           "" ); break;

   case '-': addGlyph( ch, "",
                           "",
                           "",
                           "+++++",
                           "",
                           "",
                           "" ); break;

   case '[': addGlyph( ch, "  ++",
                           "  +",
                           "  +",
                           "  +",
                           "  +",
                           "  +",
                           "  ++" ); break;

   case ']': addGlyph( ch, " ++",
                           "  +",
                           "  +",
                           "  +",
                           "  +",
                           "  +",
                           " ++" ); break;

   case '(': addGlyph( ch, "   +",
                           "  +",
                           " +",
                           " +",
                           " +",
                           "  +",
                           "   +" ); break;

   case ')': addGlyph( ch, " +",
                           "  +",
                           "   +",
                           "   +",
                           "   +",
                           "  +",
                           " +" ); break;

   case '{': addGlyph( ch, "   ++",
                           "  +",
                           "  +",
                           " +",
                           "  +",
                           "  +",
                           "   ++" ); break;

   case '}': addGlyph( ch, "++",
                           "  +",
                           "  +",
                           "   +",
                           "  +",
                           "  +",
                           "++" ); break;

   case '0': addGlyph( ch, " +++ ",
                           "+   +",
                           "+   +",
                           "+ + +",
                           "+   +",
                           "+   +",
                           " +++ " ); break;

   case '1': addGlyph( ch, "  +  ",
                           " ++  ",
                           "  +  ",
                           "  +  ",
                           "  +  ",
                           "  +  ",
                           " +++ " ); break;

   case '2': addGlyph( ch, "  ++ ",
                           " +  +",
                           "    +",
                           "   + ",
                           "  +  ",
                           " +   ",
                           "+++++" ); break;

   case '3': addGlyph( ch, "+++++",
                           "   + ",
                           "  +  ",
                           "   + ",
                           "    +",
                           "+   +",
                           " +++ " ); break;

   case '4': addGlyph( ch, "   + ",
                           "  ++ ",
                           " + + ",
                           "+  + ",
                           "+++++",
                           "   + ",
                           "   + " ); break;

   case '5': addGlyph( ch, "+++++",
                           "+    ",
                           "++++ ",
                           "    +",
                           "    +",
                           "+   +",
                           " +++ " ); break;

   case '6': addGlyph( ch, "  ++ ",
                           " +   ",
                           "+    ",
                           "++++ ",
                           "+   +",
                           "+   +",
                           " +++ " ); break;

   case '7': addGlyph( ch, "+++++",
                           "    +",
                           "   + ",
                           "  +  ",
                           " +   ",
                           " +   ",
                           " +   " ); break;

   case '8': addGlyph( ch, " +++ ",
                           "+   +",
                           "+   +",
                           " +++ ",
                           "+   +",
                           "+   +",
                           " +++ " ); break;

   case '9': addGlyph( ch, " +++ ",
                           "+   +",
                           "+   +",
                           " ++++",
                           "    +",
                           "   + ",
                           " ++  " ); break;


   case 'A': addGlyph( ch, " +++ ",
                           "+   +",
                           "+   +",
                           "+++++",
                           "+   +",
                           "+   +",
                           "+   +" ); break;

   case 'B': addGlyph( ch, "++++ ",
                           "+   +",
                           "+   +",
                           "++++ ",
                           "+   +",
                           "+   +",
                           "++++ " ); break;

   case 'C': addGlyph( ch, " +++ ",
                           "+   +",
                           "+    ",
                           "+    ",
                           "+    ",
                           "+   +",
                           " +++ " ); break;

   case 'D': addGlyph( ch, "++++ ",
                           "+   +",
                           "+   +",
                           "+   +",
                           "+   +",
                           "+   +",
                           "++++ " ); break;

   case 'E': addGlyph( ch, "+++++",
                           "+    ",
                           "+    ",
                           "++++ ",
                           "+    ",
                           "+    ",
                           "+++++" ); break;

   case 'F': addGlyph( ch, "+++++",
                           "+    ",
                           "+    ",
                           "++++ ",
                           "+    ",
                           "+    ",
                           "+    " ); break;

   case 'G': addGlyph( ch, " +++ ",
                           "+   +",
                           "+    ",
                           "+ +++",
                           "+   +",
                           "+   +",
                           " +++ " ); break;

   case 'H': addGlyph( ch, "+   +",
                           "+   +",
                           "+   +",
                           "+++++",
                           "+   +",
                           "+   +",
                           "+   +" ); break;

   case 'I': addGlyph( ch, " +++ ",
                           "  +  ",
                           "  +  ",
                           "  +  ",
                           "  +  ",
                           "  +  ",
                           " +++ " ); break;

   case 'J': addGlyph( ch, "  +++",
                           "   +",
                           "   +",
                           "   +",
                           "   +",
                           "+  +",
                           " ++ " ); break;

   case 'K': addGlyph( ch, "+   +",
                           "+  + ",
                           "+ +  ",
                           "++   ",
                           "+ +  ",
                           "+  + ",
                           "+   +" ); break;

   case 'L': addGlyph( ch, "+    ",
                           "+    ",
                           "+    ",
                           "+    ",
                           "+    ",
                           "+    ",
                           "+++++" ); break;

   case 'M': addGlyph( ch, "+   +",
                           "++ ++",
                           "+ + +",
                           "+   +",
                           "+   +",
                           "+   +",
                           "+   +" ); break;

   case 'N': addGlyph( ch, "+   +",
                           "++  +",
                           "+ + +",
                           "+  ++",
                           "+   +",
                           "+   +",
                           "+   +" ); break;

   case 'O': addGlyph( ch, " +++ ",
                           "+   +",
                           "+   +",
                           "+   +",
                           "+   +",
                           "+   +",
                           " +++ " ); break;

   case 'P': addGlyph( ch, "++++",
                           "+   +",
                           "+   +",
                           "++++",
                           "+",
                           "+",
                           "+" ); break;

   case 'Q': addGlyph( ch, " +++ ",
                           "+   +",
                           "+   +",
                           "+   +",
                           "+ + +",
                           "+  + ",
                           " ++ +" ); break;

   case 'R': addGlyph( ch, "++++",
                           "+   +",
                           "+   +",
                           "++++",
                           "+ +",
                           "+  +",
                           "+   +" ); break;

   case 'S': addGlyph( ch, " ++++",
                           "+    ",
                           "+    ",
                           " +++ ",
                           "    +",
                           "    +",
                           "++++" ); break;

   case 'T': addGlyph( ch, "+++++",
                           "  +",
                           "  +",
                           "  +",
                           "  +",
                           "  +",
                           "  +" ); break;

   case 'U': addGlyph( ch, "+   +",
                           "+   +",
                           "+   +",
                           "+   +",
                           "+   +",
                           "+   +",
                           " +++ " ); break;

   case 'V': addGlyph( ch, "+   +",
                           "+   +",
                           "+   +",
                           "+   +",
                           "+   +",
                           " + +",
                           "  + " ); break;

   case 'W': addGlyph( ch, "+   +",
                           "+   +",
                           "+   +",
                           "+ + +",
                           "+ + +",
                           "+ + +",
                           " + + " ); break;

   case 'X': addGlyph( ch, "+   +",
                           "+   +",
                           " + + ",
                           "  +  ",
                           " + + ",
                           "+   +",
                           "+   +" ); break;

   case 'Y': addGlyph( ch, "+   +",
                           "+   +",
                           "+   +",
                           " + +",
                           "  +",
                           "  +",
                           "  +" ); break;

   case 'Z': addGlyph( ch, "+++++",
                           "    +",
                           "   +",
                           "  +",
                           " +",
                           "+",
                           "+++++" ); break;

   case 'a': addGlyph( ch, "",
                           "",
                           " +++ ",
                           "    +",
                           " ++++",
                           "+   +",
                           " ++++" ); break;

   case 'b': addGlyph( ch, "+    ",
                           "+    ",
                           "+ ++ ",
                           "++  +",
                           "+   +",
                           "+   +",
                           "++++ " ); break;

   case 'c': addGlyph( ch, "",
                           "",
                           " +++ ",
                           "+    ",
                           "+    ",
                           "+   +",
                           " +++ " ); break;

   case 'd': addGlyph( ch, "    +",
                           "    +",
                           " ++ +",
                           "+  ++",
                           "+   +",
                           "+   +",
                           " ++++" ); break;

   case 'e': addGlyph( ch, "",
                           "",
                           " +++ ",
                           "+   +",
                           "+++++",
                           "+    ",
                           " +++ " ); break;

   case 'f': addGlyph( ch, "  ++ ",
                           " +  +",
                           " +   ",
                           "+++  ",
                           " +   ",
                           " +   ",
                           " +   " ); break;

   case 'g': addGlyph( ch, "",
                           "",
                           " +++ ",
                           "+   +",
                           "+   +",
                           " ++++",
                           "    +",
                           " +++ " ); break;

   case 'h': addGlyph( ch, "+    ",
                           "+    ",
                           "+ ++ ",
                           "++  +",
                           "+   +",
                           "+   +",
                           "+   +" ); break;

   case 'i': addGlyph( ch, "  +  ",
                           "     ",
                           " ++  ",
                           "  +  ",
                           "  +  ",
                           "  +  ",
                           " +++ " ); break;

   case 'j': addGlyph( ch, "    +",
                           "     ",
                           "   ++",
                           "    +",
                           "    +",
                           "    +",
                           "+  +",
                           " ++" ); break;

   case 'k': addGlyph( ch, " +   ",
                           " +   ",
                           " +  +",
                           " + + ",
                           " ++  ",
                           " + + ",
                           " +  +" ); break;

   case 'l': addGlyph( ch, " ++",
                           "  +",
                           "  +",
                           "  +",
                           "  +",
                           "  +",
                           " +++" ); break;

   case 'm': addGlyph( ch, "",
                           "",
                           "++ + ",
                           "+ + +",
                           "+ + +",
                           "+   +",
                           "+   +" ); break;

   case 'n': addGlyph( ch, "",
                           "",
                           "+ ++ ",
                           "++  +",
                           "+   +",
                           "+   +",
                           "+   +" ); break;

   case 'o': addGlyph( ch, "",
                           "",
                           " +++ ",
                           "+   +",
                           "+   +",
                           "+   +",
                           " +++" ); break;

   case 'p': addGlyph( ch, "",
                           "",
                           "++++ ",
                           "+   +",
                           "+   +",
                           "++++",
                           "+",
                           "+" ); break;

   case 'q': addGlyph( ch, "",
                           "",
                           " ++ +",
                           "+  ++",
                           "+   +",
                           " ++++",
                           "    +",
                           "    +" ); break;

   case 'r': addGlyph( ch, "",
                           "",
                           "+ ++ ",
                           "++  +",
                           "+    ",
                           "+    ",
                           "+    " ); break;

   case 's': addGlyph( ch, "     ",
                           "     ",
                           " +++ ",
                           "+    ",
                           " +++ ",
                           "    +",
                           "++++ " ); break;

   case 't': addGlyph( ch, " +   ",
                           " +   ",
                           "+++  ",
                           " +   ",
                           " +   ",
                           " +  +",
                           "  ++ " ); break;

   case 'u': addGlyph( ch, "",
                           "",
                           "+   +",
                           "+   +",
                           "+   +",
                           "+  ++",
                           " ++ +" ); break;

   case 'v': addGlyph( ch, "",
                           "",
                           "+   +",
                           "+   +",
                           "+   +",
                           " + +",
                           "  +" ); break;

   case 'w': addGlyph( ch, "",
                           "",
                           "+   +",
                           "+   +",
                           "+ + +",
                           "+ + +",
                           " + +" ); break;

   case 'x': addGlyph( ch, "",
                           "",
                           "+   +",
                           " + + ",
                           "  +  ",
                           " + + ",
                           "+   +" ); break;

   case 'y': addGlyph( ch, "",
                           "",
                           "+   +",
                           "+   +",
                           "+   +",
                           " ++++",
                           "    +",
                           " +++" ); break;

   case 'z': addGlyph( ch, "",
                           "",
                           "+++++",
                           "   +",
                           "  +",
                           " +",
                           "+++++" ); break;

   default: break;
   }

   return retVal;
}

} // end namespace gpu.
} // end namespace de.


/*
 *
 *
// static
void
Font5x8::add2DText( SMeshBuffer & o, int x, int y,
            std::string const & msg,
            uint32_t color,
            Align::EAlign align,
            Font5x8 const & font )
{
   o.setPrimitiveType( PrimitiveType::Triangles );

   float cx = font.quadWidth + font.quadSpacingX;
   float cy = font.quadHeight + font.quadSpacingY;
   float gw = 5.f * cx + font.glyphSpacingX;
   float gh = 8.f * cy + font.glyphSpacingY;

   glm::ivec2 ts = getTextSize( msg, font );
   float tw = ts.x;
   float th = ts.y;

   // Align
   glm::ivec2 aligned_pos;
        if ( align & Align::Left )  { aligned_pos.x = x;       }
   else if ( align & Align::Center ){ aligned_pos.x = x - 0.5f * tw; }
   else if ( align & Align::Right ) { aligned_pos.x = x - tw;   }
   else { DE_ERROR("Invalid horizontal text align") return; }
        if ( align & Align::Top )   { aligned_pos.y = y;       }
   else if ( align & Align::Middle ){ aligned_pos.y = y - 0.5f * th; }
   else if ( align & Align::Bottom ){ aligned_pos.y = y - th;   }
   else { DE_ERROR("Invalid vertical text align") return; }

   float gx = aligned_pos.x;
   float gy = aligned_pos.y;

   for ( size_t u = 0; u < msg.size(); ++u )
   {
      char ch = msg[ u ];

      if ( ch == '\r' )  // Mac or Windows line breaks.
      {
         gx = aligned_pos.x;
         gy += gh;
         continue;
      }
      if ( ch == '\n' )	// Mac or Windows line breaks.
      {
         gx = aligned_pos.x;
         gy += gh;
         continue;
      }

      //char ch = char( std::clamp( unicode, uint32_t(30), uint32_t(127) ) );
      Font5x8Matrix m = getGlyph( ch );

      // Add dot matrix 5mal8 as max 49 quads
      for ( int j = 0; j < 8; ++j ) {
      for ( int i = 0; i < 5; ++i ) {

         int x1 = gx + i * cx;
         int y1 = gy + j * cy;

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

      gx += gw;
   }
}

*/








