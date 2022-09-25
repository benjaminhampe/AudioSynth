/***************************************************************
 * Name:      SnipWindow.h
 * Author:    BenjaminHampe@gmx.de (BenjaminHampe@gmx.de)
 * Created:   2022-09-12
 * License:   LGPL
 **************************************************************/

#ifndef DE_SNIPWINDOW_H
#define DE_SNIPWINDOW_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class SnipWindow : public wxFrame
{
public:
   SnipWindow( 	wxWindow * parent,
            wxWindowID id = 1,
            wxString const & title = wxT("SnippingTool-RectSelection"),
            wxPoint const & pos = wxDefaultPosition,
            wxSize const & size = wxDefaultSize,
            long style = wxDEFAULT_FRAME_STYLE );
   ~SnipWindow();

   int w(float pct) { return (int) (GetClientSize().x * pct / 100); }
   int h(float pct) { return (int) (GetClientSize().y * pct / 100); }

private:
   void paintEvent( wxPaintEvent & event );

   void mousePressEventLeft( wxMouseEvent & event );
   void mouseReleaseEventLeft( wxMouseEvent & event );

   void closeEvent( wxCloseEvent & event );
//	void OnOpen(wxCommandEvent& event);
//	void OnSave(wxCommandEvent& event);
//	void OnSaveAs(wxCommandEvent& event);
//	void OnQuit(wxCommandEvent& event);
//	void OnAbout(wxCommandEvent& event);
//	void OnDocumentation(wxCommandEvent& event);

   DECLARE_EVENT_TABLE()
};

#endif // Frame_H
