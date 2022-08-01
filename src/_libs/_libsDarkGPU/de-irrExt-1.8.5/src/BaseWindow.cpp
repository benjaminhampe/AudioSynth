#include <irrExt/BaseWindow.hpp>

BaseWindow*
createBaseWindow( std::string const & title, irr::core::recti const & pos, irr::gui::IGUIEnvironment* env, irr::gui::IGUIElement* parent, int id )
{
    BaseWindow* win = new BaseWindow( env, parent, id, pos );
    win->setText( irr::core::stringw( title.c_str() ).c_str() );
    win->setDrawTitlebar( true );
    return win;
}

BaseWindow::BaseWindow( irr::gui::IGUIEnvironment* env, irr::gui::IGUIElement* parent, int id, irr::core::recti const & pos )
    : irr::gui::CGUIWindow( env, parent, id, pos )
{}

BaseWindow::~BaseWindow()
{}

bool
BaseWindow::OnEvent( irr::SEvent const & event )
{
    if ( event.EventType == irr::EET_GUI_EVENT )
    {
        irr::SEvent::SGUIEvent guiEvent = event.GUIEvent;

        if ( guiEvent.Caller == getCloseButton() &&
             guiEvent.EventType == irr::gui::EGET_BUTTON_CLICKED )
        {
            this->setVisible( false );
            return true;
        }

    }

    return irr::gui::CGUIWindow::OnEvent( event );
}
