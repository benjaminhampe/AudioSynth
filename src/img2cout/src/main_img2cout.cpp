//////////////////////////////////////////////////////////////////////////////
/// @file main_img2cout.cpp
/// @author Benjamin Hampe <benjamin.hampe@gmx.de>
//////////////////////////////////////////////////////////////////////////////

#include <cstdint>
#include <sstream>
#include <fstream>
#include <iostream>

#include <DarkImage.hpp>

inline char
hexStrNibble( uint8_t byte )
{
   size_t lowbyteNibble = byte & 0x0F;
   if ( lowbyteNibble < 10 )
   {
      return '0' + lowbyteNibble;
   }
   else
   {
      return 'A' + (lowbyteNibble-10);
   }
}

inline std::string
hexStrByte( uint8_t byte )
{
   std::ostringstream s;
   s << hexStrNibble( byte >> 4 ) << hexStrNibble( byte );
   return s.str();
}

inline std::string
hexStr( uint8_t byte )
{
   std::ostringstream s;
   s << "0x" << hexStrByte( byte );
   return s.str();
}

inline bool
img2cout( std::string const & uri )
{
   //de::CaptureScreen::copyBitmapToClipboard();
   de::Image img;
   bool ok = dbLoadImage( img, uri );

   size_t w = img.getWidth();
   size_t h = img.getHeight();

   std::cout << "uri = " << uri << "\n";
   std::cout << "w = " << w << "\n";
   std::cout << "h = " << h << "\n";

   if ( ok )
   {
      std::cout << "static const char * const image_data_bytes[] = {\n";

      for ( size_t y = 0; y < h; ++y )
      {
         std::cout << "\t";

         for ( size_t x = 0; x < w; ++x )
         {
            uint32_t color = img.getPixel( x, y );
            int r = de::RGBA_R( color );
            int g = de::RGBA_G( color );
            int b = de::RGBA_B( color );

            std::cout
               << hexStr( r ) << ", "
               << hexStr( g ) << ", "
               << hexStr( b ) << ", ";
         }
         std::cout << "\n";
      }

      std::cout << "};\n";
   }
   else
   {
      std::cout << "ERROR: Cant load image file.\n";
   }

   return ok;
}


//========================================================================
int main( int argc, char** argv )
//========================================================================
{
   std::cout << argv[ 0 ] << " by (c) 2022 by <benjaminhampe@gmx.de>\n";

   std::wstring uri;

   if ( argc > 1 )
   {
      uri = dbStrW( std::string( argv[ 1 ] ) );
   }

   if ( !dbExistFile( uri ) )
   {
      uri = dbOpenFileDlgW( L"Load/open any image file (bmp,xpm,png,tga,jpg,tif,gif,dds,etc..)" );
   }

   if ( !dbExistFile( uri ) )
   {
      DEM_ERROR("Nothing todo")
      return 0;
   }

   img2cout( dbStr( uri ) );

   return 0;
}
