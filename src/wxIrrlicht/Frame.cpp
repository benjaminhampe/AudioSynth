/***************************************************************
 * Name:      Frame.cpp
 * Author:    BenjaminHampe@gmx.de (BenjaminHampe@gmx.de)
 * Created:   2011-07-12
 * License:   LGPL
 **************************************************************/

#include "Frame.h"
#include "IrrlichtControl.h"


#ifdef WX_PRECOMP
    #include "wx_pch.h"
#else
    #include <wx/wx.h>
    #include <wx/panel.h>
#endif

#ifdef __BORLANDC__
    #pragma hdrstop
#endif //__BORLANDC__

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

////helper functions
//enum wxbuildinfoformat {
//    short_f, long_f };
//
//wxString wxbuildinfo(wxbuildinfoformat format)
//{
//    wxString wxbuild(wxVERSION_STRING);
//
//    if (format == long_f )
//    {
//#if defined(__WXMSW__)
//        wxbuild << _T("-Windows");
//#elif defined(__WXMAC__)
//        wxbuild << _T("-Mac");
//#elif defined(__UNIX__)
//        wxbuild << _T("-Linux");
//#endif
//
//#if wxUSE_UNICODE
//        wxbuild << _T("-Unicode build");
//#else
//        wxbuild << _T("-ANSI build");
//#endif // wxUSE_UNICODE
//    }
//
//    return wxbuild;
//}

BEGIN_EVENT_TABLE(Frame, wxFrame)
    EVT_CLOSE(Frame::OnClose)
    EVT_MENU(ID_MENU_FILE_QUIT, Frame::OnQuit)
    EVT_MENU(ID_MENU_FILE_OPEN, Frame::OnOpen)
    EVT_MENU(ID_MENU_FILE_SAVE, Frame::OnSave)
    EVT_MENU(ID_MENU_FILE_SAVE_AS, Frame::OnSaveAs)
    EVT_MENU(ID_MENU_HELP_ABOUT, Frame::OnAbout)
    EVT_MENU(ID_MENU_HELP_DOCS, Frame::OnDocumentation)
END_EVENT_TABLE()

Frame::Frame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame(parent, id, title, pos, size, style)
{
    SetSize(800, 600);
	Center();

    wxMenuBar* menuBar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append(ID_MENU_FILE_OPEN, _("&Open\tAlt-O"), _("Laden von Dateien und Projekten"));
    fileMenu->Append(ID_MENU_FILE_SAVE, _("&Save\tS"), _("Speichern von Dateien und Projekten"));
    fileMenu->Append(ID_MENU_FILE_SAVE_AS, _("&Save As\tAlt-S"), _("Speichern von Dateien und Projekten"));
    fileMenu->Append(ID_MENU_FILE_QUIT, _("&Exit Application\tAlt-F4"), _("Programm beenden"));
    menuBar->Append(fileMenu, _("&File(s)"));

    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(ID_MENU_HELP_ABOUT, _("&Über\tF1"), _("Infos"));
    helpMenu->Append(ID_MENU_HELP_DOCS, _("&Dokumentation\tF2"), _("Dokumentation"));
    menuBar->Append(helpMenu, _("&Hilfe"));

    SetMenuBar(menuBar);

    CreateStatusBar(1);
    SetStatusText(_("wxIrrlicht (c) 2011"),0);

		wxPanel* pnl=new wxPanel(this, wxID_ANY);

		IrrlichtControl* irrlicht=new IrrlichtControl(pnl, wxID_ANY, false, wxPoint(0,0), GetClientSize());
//		IrrlichtControl* irrlicht2=new IrrlichtControl(pnl, wxID_ANY, false, wxPoint(100,100), GetClientSize());
//		IrrlichtControl* irrlicht3=new IrrlichtControl(pnl, wxID_ANY, false, wxPoint(200,200), GetClientSize());

		wxBoxSizer* sizerH = new wxBoxSizer(wxHORIZONTAL);
		wxBoxSizer* sizerV = new wxBoxSizer(wxVERTICAL);

		wxGridSizer* sizer = new wxGridSizer(1,1,0,0);
		sizer->Add(irrlicht, wxSizerFlags().Expand());
        pnl->SetSizer(sizer);
        pnl->SetAutoLayout(true);
//      pnl->SetDoubleBuffered();
        pnl->Update();
//      Layout();
}//ctor

Frame::~Frame()
{
}

void Frame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void Frame::OnOpen(wxCommandEvent &event)
{

}

void Frame::OnSave(wxCommandEvent &event)
{

}

void Frame::OnSaveAs(wxCommandEvent &event)
{

}

void Frame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void Frame::OnAbout(wxCommandEvent &event)
{
    wxMessageBox(wxT("wxIrrlicht (c) 2008-2011 by BenjaminHampe@gmx.de\n\nVersion 1.0\n"), wxT("About"));
}

void Frame::OnDocumentation(wxCommandEvent &event)
{
    wxMessageBox(wxT("wxIrrlicht (c) 2008-2011 by BenjaminHampe@gmx.de\n\nVersion 1.0\n"), wxT("Docs"));
}
