#ifndef DE_IMAGE_IMAGELOADER_JPG_HPP
#define DE_IMAGE_IMAGELOADER_JPG_HPP

#include <de/image/io/ImageReader.hpp>

#ifdef DE_IMAGE_READER_JPG_ENABLED

namespace de {
namespace image {

class ImageReaderJPG : public IImageReader
{
   DE_CREATE_LOGGER("de.ImageReaderJPG")
public:

   std::vector< std::string >
   getSupportedReadExtensions() const override
   {
      return { "jpg", "jpeg" };
   }

   bool
   isSupportedReadExtension( std::string const & ext ) const override
   {
      if ( ext == "jpg" ) return true;
      if ( ext == "jpeg" ) return true;
      return false;
   }

   bool
   load( Image & img, Binary & file ) override;

   bool
   load( Image & img, std::string const & fileName ) override;
};

} // end namespace image.
} // end namespace de.

#endif // DE_IMAGE_READER_JPG_ENABLED

#endif // DE_IMAGE_IMAGELOADER_JPG_HPP
