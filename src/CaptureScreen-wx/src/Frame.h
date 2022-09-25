/***************************************************************
 * Name:      Frame.h
 * Author:    BenjaminHampe@gmx.de (BenjaminHampe@gmx.de)
 * Created:   2011-07-12
 * License:   LGPL
 **************************************************************/

#ifndef Frame_H
#define Frame_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "SnipWindow.h"

class Frame : public wxFrame
{
public:
   enum
   {
      ID_MENU_FILE_QUIT = 1000,
      ID_MENU_FILE_OPEN,
      ID_MENU_FILE_SAVE,
      ID_MENU_FILE_SAVE_AS,
      ID_MENU_HELP_ABOUT,
      ID_MENU_HELP_DOCS
   };

   Frame( 	wxWindow * parent,
            wxWindowID id = 1,
            wxString const & title = wxT("Project2"),
            wxPoint const & pos = wxDefaultPosition,
            wxSize const & size = wxDefaultSize,
            long style = wxDEFAULT_FRAME_STYLE );
   ~Frame();

   int w(float pct) { return (int) (GetClientSize().x * pct / 100); }
   int h(float pct) { return (int) (GetClientSize().y * pct / 100); }

private:
   void OnClose(wxCloseEvent& event);

   void OnButtonClick_Snipping( wxCommandEvent & evt );
   void OnButtonClick_Cancel( wxCommandEvent & evt );
   void OnButtonClick_Setup( wxCommandEvent & evt );
   void OnButtonClick_About( wxCommandEvent & evt );

//   void OnOpen(wxCommandEvent& event);
//   void OnSave(wxCommandEvent& event);
//   void OnSaveAs(wxCommandEvent& event);
//   void OnQuit(wxCommandEvent& event);
//   void OnAbout(wxCommandEvent& event);
//   void OnDocumentation(wxCommandEvent& event);

   DECLARE_EVENT_TABLE()

   SnipWindow* m_SnipWindow;
};

#endif // Frame_H
