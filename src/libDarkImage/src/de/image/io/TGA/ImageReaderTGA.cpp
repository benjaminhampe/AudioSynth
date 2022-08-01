#include "ImageReaderTGA.hpp"

#ifdef DE_IMAGE_READER_TGA_ENABLED

#include "TGA.hpp"
#include <fstream>

namespace de {
namespace image {

bool
ImageReaderTGA::load( Image & img, std::string const & uri )
{
   std::ifstream file( uri.c_str(), std::ios::in | std::ios::binary );
   if ( !file.is_open() )
   {
      DE_ERROR("File Not Found.")
      return false;
   }

   uint32_t width;
   uint32_t height;
   uint32_t bpp;  // bits per pixel
   //bool isCompressed;
   size_t size;
   std::vector< uint8_t > bytes;

   uint8_t header[18] = {0};
   static uint8_t DeCompressed[12] = {0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
   static uint8_t IsCompressed[12] = {0x0, 0x0, 0xA, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

   file.read( reinterpret_cast<char*>(&header), sizeof(header) );

   if ( std::memcmp( DeCompressed, &header, sizeof(DeCompressed) ) == 0 )
   {
      bpp = header[16];
      width  = header[13] * 256 + header[12];
      height = header[15] * 256 + header[14];
      size  = ((width * bpp + 31) / 32) * 4 * height;

      if ((bpp != 24) && (bpp != 32))
      {
         file.close();
         DE_ERROR("Invalid File Format. Required: 24 or 32 Bit Image.")
         return false;
      }

      bytes.resize( size);
      //isCompressed = false;
      file.read(reinterpret_cast<char*>( bytes.data() ), size);
   }
   else if ( std::memcmp( IsCompressed, &header, sizeof(IsCompressed) ) == 0 )
   {
      bpp = header[16];
      width  = header[13] * 256 + header[12];
      height = header[15] * 256 + header[14];
      size  = ((width * bpp + 31) / 32) * 4 * height;

      if ((bpp != 24) && (bpp != 32))
      {
         file.close();
         DE_ERROR("Invalid File Format. Required: 24 or 32 Bit Image.")
         return false;
      }

      image::tga::PixelInfo pixel = {0};
      size_t pixelIndex = 0;
      size_t byteIndex = 0;
      //isCompressed = true;
      uint8_t chunk = {0};
      int bytesPP = (bpp / 8);
      bytes.resize( width * height * sizeof(image::tga::PixelInfo) );

      do
      {
         file.read( reinterpret_cast< char* >( &chunk ), sizeof( chunk ) );

         if(chunk < 128) // A 'RAW' Chunk
         {
            ++chunk; // Add 1 To The Value To Get Total Number Of Raw Pixels
            for ( int i = 0; i < chunk; ++i, ++pixelIndex )
            {
               file.read(reinterpret_cast<char*>(&pixel), bytesPP);

               bytes[ byteIndex++ ] = pixel.B;
               bytes[ byteIndex++ ] = pixel.G;
               bytes[ byteIndex++ ] = pixel.R;
               if (bpp > 24)
               {
                  bytes[byteIndex++] = pixel.A;
               }
            }
         }
         else // A 'RLE' Chunk
         {
            chunk -= 127; // Subtract 127 To Get Rid Of The ID Bit
            file.read( reinterpret_cast<char*>( &pixel ), bytesPP );
            for ( int i = 0; i < chunk; ++i, ++pixelIndex )
            {
               bytes[ byteIndex++ ] = pixel.B;
               bytes[ byteIndex++ ] = pixel.G;
               bytes[ byteIndex++ ] = pixel.R;
               if ( bpp > 24 )
               {
                  bytes[ byteIndex++ ] = pixel.A;
               }
            }
         }
      }
      while( pixelIndex < (width * height) );

   }
   else
   {
      file.close();
      DE_ERROR("Invalid File Format. Required: 24 or 32 Bit TGA File.")
      return false;
   }

   file.close();

   // Todo: gather actually image.
   //img
   img = Image( width, height );
   img.setUri( uri );

   size_t byteIndex = 0;

   for ( uint32_t y = 0; y < height; ++y )
   {
      for ( uint32_t x = 0; x < width; ++x )
      {
         uint8_t b = bytes[ byteIndex++ ];
         uint8_t g = bytes[ byteIndex++ ];
         uint8_t r = bytes[ byteIndex++ ];
         uint8_t a = 0xFF;
         if ( bpp > 24 )
         {
            a = bytes[ byteIndex++ ];
         }
         img.setPixel( x,y, RGBA( r,g,b,a ) );
      }
   }
   return true;
}

// ===========================================================================

} // end namespace image.
} // end namespace de.

#endif // DE_IMAGE_READER_TGA_ENABLED

