#ifndef DE_IMAGEREADER_TGA_HPP
#define DE_IMAGEREADER_TGA_HPP

#include <de/image/io/ImageReader.hpp>

#ifdef DE_IMAGE_READER_TGA_ENABLED

namespace de {
namespace image {

class ImageReaderTGA : public IImageReader
{
   DE_CREATE_LOGGER("de.ImageReaderTGA")
public:

   std::vector< std::string >
   getSupportedReadExtensions() const override
   {
      return std::vector< std::string >{ "tga" };
   }

   bool
   isSupportedReadExtension( std::string const & ext ) const override
   {
      if ( ext == "tga" ) return true;
      return false;
   }

   bool
   load( Image & img, std::string const & uri ) override;
};

} // end namespace image.
} // end namespace de.

#endif // DE_IMAGE_READER_TGA_ENABLED

#endif // DE_IMAGEREADER_TGA_HPP
