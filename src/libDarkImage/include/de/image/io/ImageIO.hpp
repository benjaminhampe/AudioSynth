#ifndef DARKIMAGE_IMAGEIO_HPP
#define DARKIMAGE_IMAGEIO_HPP

#include <de/image/io/ImageReader.hpp>
#include <de/image/io/ImageWriter.hpp>

bool
dbLoadImage( de::Image & img, std::string uri, de::ImageLoadOptions const & options = de::ImageLoadOptions() );

bool
dbLoadImageFromMemory( de::Image & img, uint8_t const* data, uint64_t bytes, std::string uri, de::ImageLoadOptions const & options = de::ImageLoadOptions() );

bool
dbLoadImageXPM( de::Image & dst, std::vector< std::string > const & xpm_data );

bool
dbSaveImage( de::Image const & img, std::string uri, uint32_t quality = 0 );

bool
dbConvertFloatHeightmapToRGBA( de::Image const & src, de::Image & dst );

// bool
// dbConvertImageFile( std::string loadName, std::string saveName );

/*
// ===========================================================================
// Glyph API
// ===========================================================================
de::Image
dbCreateGlyphImage(
   uint32_t unicode,
   uint32_t txtColor,
   uint32_t fillColor,
   de::Font const & font = de::Font() );

// ===========================================================================
// Text API
// ===========================================================================
de::TextSize
dbGetTextSize(
   std::wstring const & msg,
   de::Font const & font = de::Font() );

void
dbDrawText(
   de::Image & img,
   int x, int y,
   std::wstring const & msg,
   uint32_t color = 0xFF000000,
   de::Align::EAlign align = de::Align::Default,
   de::Font const & font = de::Font() );

de::Image
dbCreateTextImage(
   std::wstring const & msg,
   uint32_t txtColor = 0xFF000000,
   uint32_t fillColor = 0xFFFFFFFF,
   de::Font const & font = de::Font() );

de::PreparedGlyphText
dbPrepareText( std::wstring const & msg, de::Font const & font = de::Font() );

// ===========================================================================
// Font API
// ===========================================================================

de::Image const*
dbGetFontAtlasImage( de::Font const & font = de::Font() );

void
dbSaveFonts();
*/

#endif

