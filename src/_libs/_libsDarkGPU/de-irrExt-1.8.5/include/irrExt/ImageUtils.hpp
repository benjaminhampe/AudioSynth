#ifndef DE_ALPHASONIC_CREATE_IMAGE_UTILS_HPP
#define DE_ALPHASONIC_CREATE_IMAGE_UTILS_HPP

#include <irrExt/_common.hpp>

void
printTextures( irr::video::IVideoDriver* driver );

irr::video::IImage*
loadImage( irr::video::IVideoDriver* driver, std::string const & fileName );

irr::video::IImage*
createImage( irr::video::IVideoDriver* driver, int w, int h );

irr::video::ITexture*
createTextureFromImage(
    irr::video::IVideoDriver* driver,
    irr::video::IImage* img,
    std::string const & imgName = "" );

#endif // DE_ALPHASONIC_CREATE_IMAGE_UTILS_HPP
