/// (c) 2017 - 20180 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef DE_LIVE_MAINWINDOW_HPP
#define DE_LIVE_MAINWINDOW_HPP

#include "Live0_Body.hpp"

class Live0_Window : public QMainWindow
{
   DE_CREATE_LOGGER("Live0_Window")
   Q_OBJECT
   Live0_Body* m_body;

public:
   Live0_Window();
   ~Live0_Window() override;
   void createMenuBar();
public slots:
   void onMenuFileExit();
   void onMenuFileOpen();
   void onShowAboutDialog();
protected:

};

#endif
