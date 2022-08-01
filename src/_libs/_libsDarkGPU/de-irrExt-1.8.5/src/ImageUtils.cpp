#include <irrExt/ImageUtils.hpp>
#include <iostream>

void printTextures( irr::video::IVideoDriver* driver )
{
    std::cout << "TexCount = " << driver->getTextureCount() << "\n";

    for ( uint32_t i = 0; i < driver->getTextureCount(); ++i )
    {
        irr::video::ITexture* tex = driver->getTextureByIndex( i );
        if ( tex )
        {
            std::cout << "Tex[" << i << "] = " << tex->getName().getPath().c_str() << "\n";
        }
    }
}

irr::video::IImage*
loadImage( irr::video::IVideoDriver* driver, std::string const & fileName )
{
    assert( driver );

    assert( !fileName.empty() );

    irr::video::IImage* img = driver->createImageFromFile( fileName.c_str() );

    if ( !img )
    {
        return nullptr;
    }

    return img;
}

irr::video::IImage*
createImage( irr::video::IVideoDriver* driver, int w, int h )
{
    assert( driver );

    assert( w > 0 );

    assert( h > 0 );

    irr::video::IImage * img = driver->createImage( irr::video::ECF_A8R8G8B8, irr::core::dimension2du( w, h ) );

    assert( img );

    assert( img->getDimension().Width > 0 );

    assert( img->getDimension().Height > 0 );

    img->fill( irr::video::SColor(0,0,0,0) );

    return img;
}


irr::video::ITexture*
createTextureFromImage(
    irr::video::IVideoDriver* driver,
    irr::video::IImage* img,
    std::string const & imgName )
{
    assert( driver );

    assert( img );

    const bool hasMipmaps = driver->getTextureCreationFlag( irr::video::ETCF_CREATE_MIP_MAPS );
    const bool hasAlways32 = driver->getTextureCreationFlag( irr::video::ETCF_ALWAYS_32_BIT );
    const bool hasNonPower2 = driver->getTextureCreationFlag( irr::video::ETCF_ALLOW_NON_POWER_2 );

    driver->setTextureCreationFlag( irr::video::ETCF_CREATE_MIP_MAPS, false );
    driver->setTextureCreationFlag( irr::video::ETCF_ALWAYS_32_BIT, true );
    driver->setTextureCreationFlag( irr::video::ETCF_ALLOW_NON_POWER_2, true );

    irr::video::ITexture * tex = driver->addTexture( imgName.empty() ? "" : imgName.c_str(), img, 0 );

    assert( tex );

    driver->setTextureCreationFlag( irr::video::ETCF_CREATE_MIP_MAPS, hasMipmaps );
    driver->setTextureCreationFlag( irr::video::ETCF_ALWAYS_32_BIT, hasAlways32 );
    driver->setTextureCreationFlag( irr::video::ETCF_ALLOW_NON_POWER_2, hasNonPower2 );

    return tex;
}
