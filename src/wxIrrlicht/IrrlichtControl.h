/* IrrlichtControl.h -- A wxWidgets control that embeds the 'Irrlicht Engine'.

  Copyright (C) 2006 Yiannis Mandravellos <mandrav@codeblocks.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  ChangeLog
  ---------
    2 Sep 2006:
      * Original version released.
*/

/*
  2007/12/12
  	* changed version by loptr (devy@sourceforge.net)
*/

#ifndef __LOPTRIRR_H__
#define __LOPTRIRR_H__

// #include <windows.h>
#include <irrlicht.h>
#include <wx/wx.h>

class IrrlichtControl : public wxControl {
	public:
		IrrlichtControl(wxWindow* parent, wxWindowID id, bool bs=true, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = wxPanelNameStr);
		~IrrlichtControl();

        void init();
        void render();
        void startTimer(int milliseconds = 10);
        void stopTimer();

        irr::IrrlichtDevice* GetDevice() const { return device; }
        irr::video::IVideoDriver* GetVideoDriver() const { return driver; }
        irr::scene::ISceneManager* GetSceneManager() const { return smgr; }
        irr::scene::ICameraSceneNode* GetCameraNode() const { return camera; }
        irr::gui::IGUIEnvironment* GetGuiEnvironment() const { return guienv; }

	protected:
        void OnPaint(wxPaintEvent& event);
        void OnSize(wxSizeEvent& event);
        void OnEraseBackground(wxEraseEvent& event) {}
        void OnTimer(wxTimerEvent& event);
        void OnMouse(wxMouseEvent& event);
        void OnKey(wxKeyEvent& event);

        irr::SIrrlichtCreationParameters params;
        irr::IrrlichtDevice* device;
        irr::video::IVideoDriver* driver;
        irr::scene::ISceneManager* smgr;
        irr::gui::IGUIEnvironment* guienv;
        irr::scene::ICameraSceneNode* camera;

		wxTimer m_Timer;
        irr::gui::IGUIStaticText* m_pFPSText;
        bool m_showFPS;

		enum {
			ID_IRR_TIMER=1000
		};

    private:
		DECLARE_EVENT_TABLE();

};//IrrlichtControl

#endif
