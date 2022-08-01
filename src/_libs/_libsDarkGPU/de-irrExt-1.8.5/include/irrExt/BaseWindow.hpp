// Copyright (C) 2002-2018 Benjamin Hampe

#ifndef DE_ALPHASONIC_COMMON_BASE_WINDOW_HPP
#define DE_ALPHASONIC_COMMON_BASE_WINDOW_HPP

#include <irrExt/_common.hpp>
#include <irrExt/CGUIWindow.hpp>

class BaseWindow : public irr::gui::CGUIWindow
{
public:
    BaseWindow( irr::gui::IGUIEnvironment* env, irr::gui::IGUIElement* parent, int id, irr::core::recti const & pos );

    virtual ~BaseWindow();

    bool OnEvent( irr::SEvent const & event ) override;
};

BaseWindow*
createBaseWindow( std::string const & title, irr::core::recti const & pos, irr::gui::IGUIEnvironment* env, irr::gui::IGUIElement* parent, int id );

#endif // DE_ALPHASONIC_COMMON_BASE_WINDOW_HPP

