#ifndef DE_IMAGE_HPP
#define DE_IMAGE_HPP

#include <de/Color.hpp>
#include <de/Recti.hpp>
#include <de/Align.hpp>
#include <de/FileSystem.hpp>

namespace de {

// ===========================================================================
struct Image
// ===========================================================================
{
   DE_CREATE_LOGGER("de.Image")

   typedef std::unique_ptr< Image > Ptr;
   typedef std::shared_ptr< Image > SharedPtr;

   int32_t m_Width;           // same as (min) col count that has meaningful data
   int32_t m_Height;          // same as (min) row count
   ColorFormat::EColorFormat m_ColorFormat; // ColorBits A + R + G + B, if any
   uint32_t m_BytesPerPixel;
   double m_pts;
   std::vector< uint8_t > m_Data;
   std::string m_FileName;
public:
   Image();
   Image( int32_t w, int32_t h, ColorFormat::EColorFormat format = ColorFormat::RGBA8888 );
   Image( int32_t w, int32_t h, uint32_t fillColor, ColorFormat::EColorFormat format );
   // ~Image() = default;

   static Image*
   create( int32_t w, int32_t h, ColorFormat::EColorFormat format = ColorFormat::RGBA8888 );

   static Ptr
   createPtr( int32_t w, int32_t h, ColorFormat::EColorFormat format = ColorFormat::RGBA8888 );

   bool
   empty() const;

   bool
   equals( Image const & other ) const;

//   Image&
//   operator= ( Image const & other );

   double getPTS() const { return m_pts; }
   void setPTS( double pts ) { m_pts = pts; }
   int32_t getRedBits() const { return ColorFormat::getBitsPerChannel( 0, getFormat() ); }
   int32_t getGreenBits() const { return ColorFormat::getBitsPerChannel( 1, getFormat() ); }
   int32_t getBlueBits() const { return ColorFormat::getBitsPerChannel( 2, getFormat() ); }
   int32_t getAlphaBits() const { return ColorFormat::getBitsPerChannel( 3, getFormat() ); }
   glm::ivec2 getSize() const { return { getWidth(), getHeight() }; }
   int32_t getWidth() const;
   int32_t getHeight() const;
   Recti getRect() const { return Recti(0,0,getWidth(),getHeight()); }
   void setWidth( int w ) { m_Width = w; }
   void setHeight( int h ) { m_Height = h; }
   void clear( bool forceShrink = false );
   void resize( int32_t w, int32_t h );
   void shrink_to_fit();
   std::string const & getUri() const;
   void setUri( std::string uri );
   std::string toString( bool withUri = true, bool withBytes = false ) const;
   void fill( uint32_t color );
   void fillZero();
   void flipVertical();
   void flipHorizontal();
   void floodFill( int32_t x, int32_t y, uint32_t newColor, uint32_t oldColor );
   void switchRB();
   void setFormat( ColorFormat::EColorFormat colorFormat );
   ColorFormat::EColorFormat const & getFormat() const;
   std::string getFormatStr() const;

   void setPixel( int32_t x, int32_t y, uint32_t color, bool blend = false );
   uint32_t getPixel( int32_t x, int32_t y, uint32_t colorKey = 0x00000000 ) const;

//   float getPixelR32F( int32_t x, int32_t y ) const
//   {
//      return float( getPixel(x,y) );
//      //return *reinterpret_cast< float const * const >( &color );
//   }

   uint64_t getPixelCount() const;
   uint32_t getBitsPerPixel() const;
   uint32_t getBytesPerPixel() const;
   uint32_t getStride() const; // bytes per row - bytes per scanline
   uint8_t* getRow( int32_t y );
   uint8_t const* getRow( int32_t y ) const;

   Image copy( Recti const & pos ) const;
   void copyRowFrom( std::vector< uint8_t > & out, int32_t y );
   void copyRowTo( std::vector< uint8_t > const & row, int32_t y );

   template < typename T > T*
   writePtr() { return reinterpret_cast< T* >( m_Data.data() ); }
   template < typename T > T const*
   readPtr() const { return reinterpret_cast< T const * >( m_Data.data() ); }

   uint64_t getByteCount() const;
   uint8_t const* data() const;
   uint8_t* data();
   uint64_t size() const;
   uint64_t capacity() const;

   std::vector< uint8_t > &
   dataVector();
   std::vector< uint8_t > const &
   dataVector() const;

};

} // end namespace de.


inline std::ostream&
// ===========================================================================
operator<< ( std::ostream & o, de::Image const & img )
// ===========================================================================
{
   o << img.toString();
   return o;
}

#endif // DE_IMAGE_HPP
