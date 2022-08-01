// Copyright (C) 2002-2012, Benjamin Hampe

#include "ImageWriterXPM.hpp"

#ifdef DE_IMAGE_WRITER_XPM_ENABLED

#include "XPM.hpp"
#include <fstream>
#include <iomanip>

namespace de {
namespace image {

bool
ImageWriterXPM::save( Image const & img, std::string const & uri, uint32_t param )
{
   if ( uri.empty() ) { DE_ERROR("Empty uri(", uri, ")") return false; }
   if ( img.empty() ) { DE_ERROR("Cant save empty image uri(", uri, ")") return false; }
   int32_t w = img.getWidth();
   int32_t h = img.getHeight();
   if ( w < 1 || h < 1 ) { DE_ERROR("Got empty image(", uri, ")") return false; }

   // calculate needed bytes to store one color
   std::string const valid_chars = ".+@#$%&*=-;>,')!~{]^/(_:<[}|"
                                   "1234567890"
                                   "abcdefghijklmnopqrstuvwxyz"
                                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ`";
#if 0
   typedef std::unordered_map< uint32_t, std::string > ColorMap;
   ColorMap colorMap;
   colorMap.reserve( img.getPixelCount() ); // worst case

   // find all unique colors
   for ( int32_t y = 0; y < h; ++y )
   {
      for ( int32_t x = 0; x < w; ++x )
      {
         uint32_t color = img.getPixel( x, y ); // get color
         if ( color == 0x00000000 )
         {
            colorMap[ color ] = "none";
         }
         else
         {
            color |= 0xFF000000; // set alpha const to 255
            colorMap[ color ] = "#FFFFFF";   // set to white, reserve eenough for every other color
         }
      }
   }

   uint64_t colorCount = colorMap.size();
   uint32_t digitCount = 1;
   double f = double( colorCount );
   double e = double( valid_chars.size() );
   while ( f >= e )
   {
      f /= e;
      ++digitCount;
   }

   DE_DEBUG("colorCount(",colorCount,") digitCount(",digitCount,"), img(", img.toString(),")")

#else
   // Allocate worst case ( each pixel has a different color )
   std::vector< uint32_t > colors;
   colors.reserve( img.getPixelCount() );

   // loop image and find all unique colors

   // std::ostringstream s;
   for ( int32_t y = 0; y < h; ++y )
   {
      for ( int32_t x = 0; x < w; ++x )
      {
         uint32_t color = img.getPixel( x, y ); // get color
         color |= 0xFF000000; // set alpha const to 255

         // find color -> add_unique
         bool found = false;
         for ( size_t i = 0; i < colors.size(); ++i )
         {
            if ( colors[ i ] == color )
            {
               found = true;
               break;
            }
         }

         // add_unique
         if ( !found )
         {
            colors.emplace_back( color );
         }
      }
   }

   uint32_t digitCount = 1;
   double f = double( colors.size() );
   double e = double( valid_chars.size() );
   while ( f >= e )
   {
      f /= e;
      ++digitCount;
   }

   DE_DEBUG("digitCount(",digitCount,"), img(", img.toString(),")")

   std::vector< std::string > names;
   names.reserve( colors.size() );

   // Unique color -> Unique name
   for ( size_t i = 0; i < colors.size(); ++i )
   {
      names.push_back( xpm::Utils::computeColorName(i,valid_chars,digitCount) );
   }


#endif
   // XPM-Header
   // static char const * const xpm_test1[] = {
   // "/* XPM */",
   // "16 16 6 1",
   // " 	c None",
   // ".	c #FFFFFF",
   // "+	c #000000",
   // "@	c #9A9A9A",
   // "#	c #FEFEFE",
   // "$	c #F7F7F7",
   // "................",

   std::string baseName = dbGetFileBaseName( uri );
   dbStrReplace( baseName, ".", "_");
   dbStrReplace( baseName, ",", "_");
   dbStrReplace( baseName, "-", "_");
   dbStrReplace( baseName, "+", "_");
   dbStrReplace( baseName, "*", "_");
   dbStrReplace( baseName, "/", "_");
   dbStrReplace( baseName, "\\","_");
   dbStrReplace( baseName, " ", "_");
   dbStrReplace( baseName, "#", "_");
   dbStrReplace( baseName, "<", "_");
   dbStrReplace( baseName, ">", "_");
   dbStrReplace( baseName, "$", "_");
   dbStrReplace( baseName, "!", "_");
   dbStrReplace( baseName, "?", "_");
   dbStrReplace( baseName, "(", "_");
   dbStrReplace( baseName, ")", "_");
   dbStrReplace( baseName, "{", "_");
   dbStrReplace( baseName, "}", "_");
   dbStrReplace( baseName, "[", "_");
   dbStrReplace( baseName, "]", "_");
   dbStrReplace( baseName, "%", "_");
   dbStrReplace( baseName, "'", "_");
   dbStrReplace( baseName, "~", "_");
   dbStrReplace( baseName, "|", "_");
   dbStrReplace( baseName, "'", "_");
   dbStrReplace( baseName, "^", "_");
   dbStrReplace( baseName, "°", "_");
   dbStrReplace( baseName, "&", "_");
   dbStrReplace( baseName, "€", "_");

   std::stringstream s;
   s <<
   "// XPM-Header\n"
   "// URI = " << uri << "\n"
   "// ValidChars = " << valid_chars.size() << "\n"
   "static std::vector< std::string > const " << baseName << "_xpm {\n"

   "\""<<w<<" "<<h<<" "<<
#if 0
   colorCount
#else
   colors.size()
#endif
   <<" "<<digitCount<<"\",\n";

   // ====================================
   // Create names
   // ====================================
#if 0
   ColorMap::iterator it = colorMap.begin();
   size_t i = 0;
   while ( it != colorMap.end() )
   {
      // The name depends on an index, not the color itself
      it->second = xpm::Utils::computeColorName(i,valid_chars,digitCount);
      ++it;
      ++i;
   }


   // ====================================
   // Write color-table to xpm
   // ====================================
   it = colorMap.begin();
   while ( it != colorMap.end() )
   {
      // The name depends on an index, not the color itself
      it->second = xpm::Utils::computeColorName(i,valid_chars,digitCount);
      ++it;
      ++i;
   }

   for ( size_t i = 0; i < colors.size(); ++i )
   {
      // format color #RRGGBB
      uint32_t color = colors[ i ];
      char buf[ 8 ];
      sprintf( buf, "#%02x%02x%02x",
                     RGBA_R( color ),
                     RGBA_G( color ),
                     RGBA_B( color ) );
      std::string tmp = buf;
      dbStrUpperCase( tmp );
      s << "\"" << names[ i ] << "\tc " << tmp << "\",\n";
   }
//   uint32_t color = colors[ i ];
//      char buf[ 8 ];
//      sprintf( buf, "#%02x%02x%02x",
//                     RGBA_R( color ),
//                     RGBA_G( color ),
//                     RGBA_B( color ) );
#else

   // ====================================
   // Write color-table to xpm
   // ====================================
   for ( size_t i = 0; i < colors.size(); ++i )
   {
      // format color #RRGGBB
      uint32_t color = colors[ i ];
//      std::string name = ;
//      char buf[ 8 ];
//      sprintf( buf, "#%02x%02x%02x",
//                     RGBA_R( color ),
//                     RGBA_G( color ),
//                     RGBA_B( color ) );
//      std::string tmp = buf;
//      dbStrUpperCase( tmp );
      s << "\"" << names[ i ] << " c " << xpm::Utils::toRGB( color ) << "\",\n";
   }

   // ====================================
   // Write pixels to xpm
   // ====================================
   std::string line;
   for ( int32_t y = 0; y < h; ++y )
   {
      line = "\"";
      for ( int32_t x = 0; x < w; ++x )
      {
         // current pixel
         uint32_t color = img.getPixel( x, y );
         //color.setAlpha(255);

         // search color in color-table
         if ( color != 0 )
         {
            color |= 0xFF000000; // Set alpha to opaque ( speed up compare )
         }

         auto cit = std::find_if( colors.begin(), colors.end(),
                        [&] ( uint32_t cached ) { return color == cached; }
         );
         if ( cit != colors.end() )
         {
            // write found color-table color
            line += names[ std::distance( colors.begin(), cit ) ];
         }

/*
         size_t found = 0;
         for ( size_t i = 0; i < colors.size(); ++i )
         {
            if ( colors[ i ] == color )
            {
               found = i;
               break;
            }
         }

         // write found color-table color
         line += names[ found ];
*/
      }

      line += "\"";
      if ( y < h-1 )
         line+=",";
      else
         line+="};";

      s << line << "\n";
   }

   // std::cout << s.str() << std::endl;
#endif

   std::ofstream fout( uri.c_str() );
   if ( fout.is_open() )
   {
      fout << s.str();
      fout.close();
   }

   return true;
}




} // end namespace image.
} // end namespace de.

#endif // DE_IMAGE_WRITER_XPM_ENABLED



#if DO_SOMETHING
   // create array to hold all unique RGB colors
   std::vector< XPM_Color > colors( img.getPixelCount() );

   std::ostringstream s;
   // loop image and find all unique colors
   for ( int32_t y = 0; y < size.y; ++y )
   {
   for ( int32_t x = 0; x < size.x; ++x )
   {
      // get color
      uint32_t color = img.getPixel( x, y );

      // set alpha const to 255
      color |= 0xFF000000;

      // loop color array to see if color already stored
      bool found = false;
      for ( size_t i = 0; i < colors.size(); ++i )
      {
         if ( colors[i].color == color )
         {
            found = true;
            break;
         }
      }

      // if not found in array, color is unique --> store
      if (!found)
      {
         XPM_Color data( color, "" );
         colors.push_back(data);
      }
   }
   }

   // calculate needed bytes to store one color
   uint32_t bytesPerColor = 1;
   //uint32_t k = colors.size();
   //while ( k >= valid_chars.size() )
   //{
      //k /= valid_chars.size();
      //++bytesPerColor;
   //}

   // now give every unique color a corresponding byte-combination out of valid_chars
   uint32_t c = 0;
   while ( c < colors.size() )
   {
      colors[ c ].color = 0; // colorIndexToChars(c, bytesPerColor);
      colors[ c ].key = "?"; // colorIndexToChars(c, bytesPerColor);
      ++c;
   }

   // XPM-Header
   // static const char *const wxwin32x32_xpm[] = {
   // "32 32 407 2",
   // "  	c None",

   // /* XPM */
   s << "/* XPM */\n"
   // static const char *const wxwin32x32_xpm[] = {
     << "static const char *const " << uri << "_xpm[] = {\n";
   // "32 32 407 2"
   // Breite des Bildes (in Pixel)
   // Höhe des Bildes (in Pixel)
   // Anzahl der Farben im Bild
   // Anzahl der Zeichen pro Pixelwert
   // (X-Position des „Hotspots") --> MausCursor
   // (Y-Position des „Hotspots") --> MausCursor
   s << "\"" << size.x << " " << size.y << " " << colors.size() << " " << bytesPerColor << "\"\n";
   // write color-table
   // "  	c None",
   // ". 	c #7373C1",
   // "+ 	c #6E6EBF",
   // "@ 	c #6B6BBF",
   // "# 	c #6868BF",
   // "$ 	c #6464BF",
   // "Z+	c #FF4A4A",
   // "`+	c #FF4545",
   // " @	c #E73535",
   // "`@	c #B8B819",
   // " #	c #B7B717",
   // "0#	c #C50000",
   for ( size_t i = 0; i < colors.size(); ++i )
   {
      // format color #RRGGBB
      char buf[ 7 ];
      uint32_t c = colors[ i ].color;
      sprintf( buf, "#%02x%02x%02x", RGBA_R( c ), RGBA_G( c ), RGBA_B( c ) );
      std::string tmp = buf;
      StringUtils::upperCase( tmp );

      s << "\"" << colors[ i ].key << "\tc " << colors[ i ].key.size() << " " << tmp << "\",\n";
   }

   // write pixels
   //	"$+%+&+*+R.S.=+-+;+>+,+'+)+A.X.Y.!+~+{+]+^+/+(+_+H.I.J.:+<+[+}+|+",

   // buffer for current line
   std::string line;

   // loop
   for ( int32_t y = 0; y < size.y; ++y )
   {
      line = "\"";
      for ( int32_t x = 0; x < size.x; ++x )
      {
         // current pixel
         uint32_t colorNow = img.getPixel( x, y );
         //colorNow.setAlpha(255);

         // search color in color-table
         uint32_t i = 0;
         while ( i < colors.size() )
         {
            if ( colors[ i ].color == colorNow )
            {
               break;
            }
            ++i;
         }

         // write found color-table color
         line += colors[ i ].key;
      }

      line += "\"";
      if ( y < size.y-1 )
         line+=",";
      else
         line+="};";

      s << line << "\n";
   }

   std::cout << s.str() << std::endl;
#endif
