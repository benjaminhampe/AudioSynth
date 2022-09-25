/***************************************************************
 * Name:      Frame.cpp
 * Author:    BenjaminHampe@gmx.de (BenjaminHampe@gmx.de)
 * Created:   2011-07-12
 * License:   LGPL
 **************************************************************/

#include "Frame.h"
#include "../res/resource.h"
#include "res/xpm_desktop_64.h"
#include "res/xpm_CaptureScreen_64.h"
#include "res/xpm_cancel_64.h"
#include "res/xpm_opt_64.h"
#include "res/xpm_about_64.h"

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
#include <wx/msw/private.h> //for wxGetInstance()

#endif

#include <wx/button.h>
#include <wx/bmpbuttn.h>

BEGIN_EVENT_TABLE(Frame, wxFrame)
   EVT_CLOSE(Frame::OnClose)
   EVT_BUTTON(BTN_ID_SNIP, Frame::OnButtonClick_Snipping)
   EVT_BUTTON(BTN_ID_CANCEL, Frame::OnButtonClick_Cancel)
   EVT_BUTTON(BTN_ID_SETUP, Frame::OnButtonClick_Setup)
   EVT_BUTTON(BTN_ID_ABOUT, Frame::OnButtonClick_About)
   // EVT_MENU(ID_MENU_FILE_QUIT, Frame::OnQuit)
   // EVT_MENU(ID_MENU_FILE_OPEN, Frame::OnOpen)
   // EVT_MENU(ID_MENU_FILE_SAVE, Frame::OnSave)
   // EVT_MENU(ID_MENU_FILE_SAVE_AS, Frame::OnSaveAs)
   // EVT_MENU(ID_MENU_HELP_ABOUT, Frame::OnAbout)
   // EVT_MENU(ID_MENU_HELP_DOCS, Frame::OnDocumentation)
END_EVENT_TABLE()


Frame::Frame( 	wxWindow * parent,
               wxWindowID id,
               wxString const & title,
               wxPoint const & pos,
               wxSize const & size,
               long style )
   : wxFrame( parent, id, title, pos, size, style | wxSTAY_ON_TOP )
{
   //SetIcon( wxICON( CaptureScreen_32 ) ); // To Set App Icon
   //SetIcon( wxIcon( CaptureScreen_32_xpm ) ); // To Set App Icon
   SetIcon( wxICON(aaaa) ); // To Set App Icon

   wxSize btnSize = FromDIP( wxSize(64, 64) );
   wxSize dpiSize = wxSize( 5*btnSize.x, btnSize.y );

   wxSize dpi = GetDPI();
   std::cout << "dpi = (" << dpi.x << "," << dpi.y << ")\n";
   float dpiScale = GetDPIScaleFactor();
   std::cout << "dpiScaleFactor = " << dpiScale << "\n" << std::endl;

   std::cout << "dpiSize = (" << dpiSize.x << "," << dpiSize.y << ")\n";
   //SetSize( dpiSize );
   //SetMinSize( dpiSize );



/*
   //in Frame's constructor:
   HWND hWnd = this->GetHandle();
   HINSTANCE hInstance = wxGetInstance();
   HICON hIcon = ExtractIcon(hInstance, L"CaptureScreen.ico", 0);
   SetClassLongPtr(hWnd, GCLP_HICONSM, (LONG_PTR)hIcon);
*/

   //wxPanel* body = new wxPanel( this, wxID_ANY );
   wxButton* btn0 = new wxButton( this, BTN_ID_DESKTOP, L"", wxDefaultPosition, btnSize );
   wxButton* btn1 = new wxButton( this, BTN_ID_SNIP, L"", wxDefaultPosition, btnSize );
   wxButton* btn2 = new wxButton( this, BTN_ID_CANCEL, L"", wxDefaultPosition, btnSize );
   wxButton* btn3 = new wxButton( this, BTN_ID_SETUP, L"", wxDefaultPosition, btnSize );
   wxButton* btn4 = new wxButton( this, BTN_ID_ABOUT, L"", wxDefaultPosition, btnSize );

   //// Create the button widget
   //wxButton* button = new wxButton(panel, ButtonID, L"Button One");
   wxBitmap bmp0( desktop_64_xpm );
   wxBitmap bmp1( CaptureScreen_64_xpm );
   wxBitmap bmp2( cancel_64_xpm );
   wxBitmap bmp3( opt_64_xpm );
   wxBitmap bmp4( about_64_xpm );
   //bmp1.SetSize( btnSize - wxSize(16,16) );
   btn0->SetBitmap( bmp0 );
   btn1->SetBitmap( bmp1 );
   btn2->SetBitmap( bmp2 );
   btn3->SetBitmap( bmp3 );
   btn4->SetBitmap( bmp4 );

   wxGridSizer* sizer = new wxGridSizer( 1,4,0,0 );
   sizer->SetMinSize( dpiSize );
//   sizer->Add( btn1, wxSizerFlags() );
//   sizer->Add( btn2, wxSizerFlags() );
//   sizer->Add( btn3, wxSizerFlags() );
//   sizer->Add( btn4, wxSizerFlags() );

   sizer->Add( btn0, wxSizerFlags().Expand() );
   sizer->Add( btn1, wxSizerFlags().Expand() );
   sizer->Add( btn2, wxSizerFlags().Expand() );
   sizer->Add( btn3, wxSizerFlags().Expand() );
   sizer->Add( btn4, wxSizerFlags().Expand() );


   //body->SetSizerAndFit( sizer );

   SetSizerAndFit( sizer );

   //SetMinClientSize( dpiSize );
   Center();

   m_SnipWindow = new SnipWindow( nullptr, wxID_ANY );
   m_SnipWindow->Hide();
   //body->SetAutoLayout( true );
   //pnl->SetDoubleBuffered();
   //body->Update();
   //body->SetSize( dpiSize );
   //Layout();

   //wxMenuBar* menuBar = new wxMenuBar();
   //wxMenu* fileMenu = new wxMenu(_T(""));
   //fileMenu->Append(ID_MENU_FILE_OPEN, _("&Open\tAlt-O"), _("Laden von Dateien und Projekten"));
   //fileMenu->Append(ID_MENU_FILE_SAVE, _("&Save\tS"), _("Speichern von Dateien und Projekten"));
   //fileMenu->Append(ID_MENU_FILE_SAVE_AS, _("&Save As\tAlt-S"), _("Speichern von Dateien und Projekten"));
   //fileMenu->Append(ID_MENU_FILE_QUIT, _("&Exit Application\tAlt-F4"), _("Programm beenden"));
   //menuBar->Append(fileMenu, _("&File(s)"));

   //wxMenu* helpMenu = new wxMenu(_T(""));
   //helpMenu->Append(ID_MENU_HELP_ABOUT, _("&Über\tF1"), _("Infos"));
   //helpMenu->Append(ID_MENU_HELP_DOCS, _("&Dokumentation\tF2"), _("Dokumentation"));
   //menuBar->Append(helpMenu, _("&Hilfe"));
   //SetMenuBar(menuBar);

   //CreateStatusBar(1);
   //SetStatusText(_("wxIrrlicht (c) 2011"),0);

   //wxPanel* pnl=new wxPanel(this, wxID_ANY);

   //IrrlichtControl* irrlicht=new IrrlichtControl(pnl, wxID_ANY, false, wxPoint(0,0), GetClientSize());
   //		IrrlichtControl* irrlicht2=new IrrlichtControl(pnl, wxID_ANY, false, wxPoint(100,100), GetClientSize());
   //		IrrlichtControl* irrlicht3=new IrrlichtControl(pnl, wxID_ANY, false, wxPoint(200,200), GetClientSize());

   //wxBoxSizer* sizerH = new wxBoxSizer(wxHORIZONTAL);
   //wxBoxSizer* sizerV = new wxBoxSizer(wxVERTICAL);

   //wxGridSizer* sizer = new wxGridSizer(1,1,0,0);
   //sizer->Add(irrlicht, wxSizerFlags().Expand());
   //pnl->SetSizer(sizer);
   //pnl->SetAutoLayout(true);
   //      pnl->SetDoubleBuffered();
   //pnl->Update();
   //      Layout();
}//ctor

Frame::~Frame()
{

}

void Frame::OnClose( wxCloseEvent & event )
{
   m_SnipWindow->Hide();
   delete m_SnipWindow;
   Destroy();
}

void Frame::OnButtonClick_Snipping( wxCommandEvent & evt )
{
   m_SnipWindow->Show();
   m_SnipWindow->Raise();
   this->Raise();
//    if (m_textCtrl)
//    {
//        std::wstringstream contents;
//        contents << L"Button has been clicked "
//            << m_buttonClicksCount << " times";
//        m_textCtrl->SetValue(contents.str());
//    }

    evt.Skip();
}

void Frame::OnButtonClick_Cancel( wxCommandEvent & evt )
{
   m_SnipWindow->Hide();
   this->Raise();
//    if (m_textCtrl)
//    {
//        std::wstringstream contents;
//        contents << L"Button has been clicked "
//            << m_buttonClicksCount << " times";
//        m_textCtrl->SetValue(contents.str());
//    }

    evt.Skip();
}

void Frame::OnButtonClick_Setup( wxCommandEvent & evt )
{
   wxMessageBox( "(c) 2022 by Benjamin Hampe <benjaminhampe@gmx.de>",
                 "About screen capture tool" );
//    if (m_textCtrl)
//    {
//        std::wstringstream contents;
//        contents << L"Button has been clicked "
//            << m_buttonClicksCount << " times";
//        m_textCtrl->SetValue(contents.str());
//    }

    evt.Skip();
}

void Frame::OnButtonClick_About( wxCommandEvent & evt )
{
   wxMessageBox( "(c) 2022 by Benjamin Hampe <benjaminhampe@gmx.de>",
                 "About screen capture tool" );
//    if (m_textCtrl)
//    {
//        std::wstringstream contents;
//        contents << L"Button has been clicked "
//            << m_buttonClicksCount << " times";
//        m_textCtrl->SetValue(contents.str());
//    }

    evt.Skip();
}

/*
void Frame::OnOpen( wxCommandEvent & event )
{

}

void Frame::OnSave( wxCommandEvent & event )
{

}

void Frame::OnSaveAs( wxCommandEvent & event )
{

}

void Frame::OnQuit( wxCommandEvent & event )
{
   Destroy();
}

void Frame::OnAbout( wxCommandEvent & event )
{
   wxMessageBox(wxT("wxIrrlicht (c) 2008-2011 by BenjaminHampe@gmx.de\n\nVersion 1.0\n"), wxT("About"));
}

void Frame::OnDocumentation( wxCommandEvent & event )
{
   wxMessageBox(wxT("wxIrrlicht (c) 2008-2011 by BenjaminHampe@gmx.de\n\nVersion 1.0\n"), wxT("Docs"));
}
*/
