/// (c) 2017 - 20180 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef DE_LIVE5_EDITOR_PANEL_HPP
#define DE_LIVE5_EDITOR_PANEL_HPP

#include "Live0_Panel.hpp"
#include "HelpViewer.hpp"
#include "ClipEditor.hpp"
#include "FxPanel.hpp"

class Live5_Panel : public QWidget
{
   Q_OBJECT
   App & m_app;
public: // Connected by body
   Live0_Panel* m_helpPanel;
   HelpViewer* m_helpViewer;
   QWidget* m_helpWidget; // = helpPanel + 10px spacing integrated for better show/hide spacing aswell.

   Live0_Panel* m_clipPanel;
   ClipEditor* m_clipEditor;
   FxPanel* m_fxPanel;
public:
   Live5_Panel( App & app, QWidget* parent = nullptr );
   ~Live5_Panel() override;
};

#endif
