#include "SnipWindow.h"
#include "../res/resource.h"

#include <DarkImage.hpp>

#ifdef WX_PRECOMP
    #include "wx_pch.h"
#else
    #include <wx/wx.h>
    #include <wx/panel.h>
#endif

#ifdef __WXMSW__
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <shellapi.h>
#include "wx/msw/private.h" //for wxGetInstance()

#endif

BEGIN_EVENT_TABLE(SnipWindow, wxFrame)
   EVT_CLOSE(SnipWindow::closeEvent)
   EVT_LEFT_DOWN(SnipWindow::mousePressEventLeft)
   EVT_LEFT_UP(SnipWindow::mouseReleaseEventLeft)
   EVT_PAINT(SnipWindow::paintEvent)
   // EVT_MENU(ID_MENU_FILE_QUIT, SnipWindow::OnQuit)
END_EVENT_TABLE()

SnipWindow::SnipWindow(
   wxWindow* parent,
   wxWindowID id,
   wxString const & title,
   wxPoint const & pos,
   wxSize const & size,
   long style )
   : wxFrame(parent, id, title, pos, size, wxFRAME_NO_TASKBAR | wxSTAY_ON_TOP )
{
   int w = dbDesktopWidth();
   int h = dbDesktopHeight();
   SetSize( wxSize( w, h ) );
   SetMinSize( wxSize( w, h ) );
   Center();
}//ctor

SnipWindow::~SnipWindow()
{

}

void SnipWindow::closeEvent( wxCloseEvent & event )
{
   Destroy();
}


void SnipWindow::mousePressEventLeft( wxMouseEvent & event )
{
   std::cout << "Left Down" << std::endl;
}

void SnipWindow::mouseReleaseEventLeft( wxMouseEvent & event )
{
   std::cout << "Left Up" << std::endl;
}

void SnipWindow::paintEvent( wxPaintEvent & event )
{
   std::cout << "Paint" << std::endl;
}
