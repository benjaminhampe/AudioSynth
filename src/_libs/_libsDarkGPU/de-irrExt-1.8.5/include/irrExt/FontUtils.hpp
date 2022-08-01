#ifndef FONTTOOLS_HPP
#define FONTTOOLS_HPP

#include <irrExt/CGUITTFont.hpp>

irr::core::dimension2du
Font_getPixelSize( irr::gui::IGUIFont* font, std::string txt );

irr::gui::CGUITTFont*
Font_create(
        irr::gui::IGUIEnvironment* env,
        std::string fileName,
        int32_t pxSize,
        bool aa,
        bool transparent = true );

void
Font_draw(
        irr::gui::IGUIFont* font,
        std::string txt,
        int32_t x,
        int32_t y,
        uint32_t color );

void
Font_draw(
        irr::gui::IGUIFont* font,
        std::string txt,
        irr::core::recti const & pos,
        uint32_t color );

struct Text_t
{
    std::string Text;
    uint32_t Color;
    irr::gui::IGUIFont * Font;
    Text_t() : Text( "X" ), Color( 0xFFFFFFFF ), Font( nullptr ) {}
    Text_t( std::string txt, uint32_t color, irr::gui::IGUIFont* font ) : Text( txt ), Color( color ), Font( font ) {}
    bool canDraw() const
    {
        if ( !Font ) return false;
        if ( Text.empty() ) return false;
        return true;
    }
};

void Text_draw( Text_t * txt, int32_t x, int32_t y );
void Text_draw( Text_t * txt, irr::core::recti const & pos );

#endif // FONTTOOLS_HPP
