#ifndef DE_IMAGE_TGA_HPP
#define DE_IMAGE_TGA_HPP

#include <de/Image.hpp>

#if defined(DE_IMAGE_READER_TGA_ENABLED) || defined(DE_IMAGE_WRITER_TGA_ENABLED)

namespace de {
namespace image {
namespace tga {

#pragma pack( push )
#pragma pack( 1 )

// these structs are also used in the TGA writer
struct TGAHeader
{
   uint8_t IdLength;
   uint8_t ColorMapType;
   uint8_t ImageType;
   uint8_t FirstEntryIndex[2];
   uint16_t ColorMapLength;
   uint8_t ColorMapEntrySize;
   uint8_t XOrigin[2];
   uint8_t YOrigin[2];
   uint16_t Width;
   uint16_t Height;
   uint8_t BitsPerPixel;
   uint8_t ImageDescriptor;

   size_t getPixelCount() const {
      return size_t( Height ) * size_t( Width );
   }

   size_t getByteCount() const {
      return getPixelCount() * size_t( BitsPerPixel ) / 8;
   }

   size_t getBitsPerPixel() const {
      return size_t( BitsPerPixel );
   }

   size_t getBytesPerPixel() const {
      return size_t( BitsPerPixel ) / 8;
   }

   int32_t getWidth() const { return int32_t( Width ); }
   int32_t getHeight() const { return int32_t( Height ); }
};

struct TGAFooter
{
   uint32_t ExtensionOffset;
   uint32_t DeveloperOffset;
   char  Signature[18];
};

typedef union PixelInfo
{
    uint32_t ABGR;
    struct { uint8_t R, G, B, A; };
} *PPixelInfo;


#pragma pack( pop )

class TGA
{
private:
   uint32_t width;
   uint32_t height;
   uint32_t bpp;  // bits per pixel
   bool isCompressed;
   size_t size;
   std::vector< uint8_t > bytes;
   std::string uri;
public:
   TGA()
      : width(0), height(0), bpp(0), isCompressed(false), size(0)
   {}
   bool hasAlphaChannel() const { return bpp == 32; }
};

} // end namespace tga
} // end namespace image.
} // end namespace de.

#endif // DE_IMAGE_CODEC_TGA_ENABLED

#endif // DE_IMAGE_TGA_HPP
