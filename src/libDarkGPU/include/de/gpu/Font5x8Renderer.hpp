#ifndef DE_GPU_FONT5X8_HPP
#define DE_GPU_FONT5X8_HPP

#include <de/gpu/smesh/SMesh.hpp>
#include <map>

namespace de {
namespace gpu {

// A glyph is a 5x8 pixel image with black and white ( bool )
struct Font5x8Matrix
{
   Font5x8Matrix() {}

   void clear()
   {
      for ( size_t i = 0; i < dots.size(); ++i )
      {
         dots[ i ] = false;
      }
   }

   void set( int x, int y, bool state )
   {
      size_t i = size_t(x) + 5*size_t(y);
      if ( i >= dots.size() )
      {
         return;
      }
      dots[ i ] = state;
   }


   void off( int x, int y ) { set( x,y,false ); }
   void on( int x, int y ) { set( x,y,true ); }

   void clearRow( int y )
   {
      for ( size_t i = 0; i < 8; ++i )
      {
         set( i,y, false );
      }
   }

   void setRow( int y, std::string cc )
   {
      clearRow( y );
      for ( size_t i = 0; i < std::min( cc.size(), size_t(5) ); ++i )
      {
         if ( cc[ i ] != ' ' )
         {
            set( i,y, true );
         }
      }
   }

   bool get( int x, int y ) const
   {
      size_t i = size_t(x) + 5*size_t(y);
      if ( i >= dots.size() )
      {
         return false;
      }
      return dots[ i ];
   }

   std::array< bool, 40 > dots; // 5x8

};

// ===========================================================================
struct Font5x8
// ===========================================================================
{
   DE_CREATE_LOGGER("de.gpu.Font5x8")

   int quadWidth;
   int quadHeight;
   int quadSpacingX;
   int quadSpacingY;
   int glyphSpacingX;
   int glyphSpacingY;

   Font5x8()
      : quadWidth( 1 )
      , quadHeight( 1 )
      , quadSpacingX( 0 )
      , quadSpacingY( 0 )
      , glyphSpacingX( 1 )
      , glyphSpacingY( 1 )
   {

   }

   // Hash me
   uint64_t operator< ( Font5x8 const & o ) const
   {
      if ( quadWidth < o.quadWidth ) return true;
      if ( quadHeight < o.quadHeight ) return true;
      if ( quadSpacingX < o.quadSpacingX ) return true;
      if ( quadSpacingY < o.quadSpacingY ) return true;
      if ( glyphSpacingX < o.glyphSpacingX ) return true;
      if ( glyphSpacingY < o.glyphSpacingY ) return true;
      return false;
   }

   // Hash me
   uint64_t hash() const {
      return   ( uint64_t( quadWidth & 0x0F ) )
            |  ( uint64_t( quadHeight & 0x0F ) << 4 )
            |  ( uint64_t( quadSpacingX & 0x0F ) << 8 )
            |  ( uint64_t( quadSpacingY & 0x0F ) << 12 )
            |  ( uint64_t( glyphSpacingX & 0x0F ) << 16 )
            |  ( uint64_t( glyphSpacingY & 0x0F ) << 20 );
   }

   static Font5x8Matrix
   getGlyph( char ch );

   static glm::ivec2
   getTextSize( std::string const & msg, Font5x8 const & font );

   glm::ivec2
   getTextSize( std::string const & msg ) const { return getTextSize( msg, *this ); }
};

// ===========================================================================
struct Font5x8Face
// ===========================================================================
{
   DE_CREATE_LOGGER("de.gpu.Font5x8GlyphCache")
   Font5x8 font;

   typedef std::shared_ptr< Font5x8Face > SharedPtr;

   Font5x8Face()
      : font()
   {}

   Font5x8Face( Font5x8 font5x8 )
      : font( font5x8 )
   {}

   void
   cacheString( std::string const & msg );

   glm::ivec2
   getTextSize( std::string const & msg ) const;

   void
   add2DText( SMeshBuffer & o, int x, int y,
            std::string const & msg,
            uint32_t color,
            Align::EAlign align );

   std::map< uint32_t, Font5x8Matrix > m_GlyphCache;

};

Font5x8Face::SharedPtr
getFont5x8( Font5x8 const & font );


struct IVideoDriver;

// ===========================================================================
struct Font5x8Renderer
// ===========================================================================
{
   DE_CREATE_LOGGER("de.gpu.Font5x8Renderer")

   Font5x8Renderer();
   ~Font5x8Renderer();
   void setDriver( IVideoDriver* driver ) { m_Driver = driver; }

   glm::ivec2
   getTextSize( std::string const & msg, Font5x8 const & font = Font5x8() ) const;

   void
   add2DText( SMeshBuffer & o, int x, int y,
            std::string const & msg,
            uint32_t color,
            Align::EAlign align,
            Font5x8 const & font );

   void
   draw2DText( int x, int y,
               std::string const & msg,
               uint32_t color = 0xFFFFFFFF,
               Align::EAlign align = Align::Default,
               Font5x8 const & font = Font5x8() );

   IVideoDriver* m_Driver;
};

} // end namespace gpu.
} // end namespace de.

#endif

