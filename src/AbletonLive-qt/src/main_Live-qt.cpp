//////////////////////////////////////////////////////////////////////////////
/// @file main_AudioSynth-qt.cpp
/// @author Benjamin Hampe <benjamin.hampe@gmx.de>
//////////////////////////////////////////////////////////////////////////////
#include "Live0_Window.hpp"
#include <QApplication>
#include <QGuiApplication>

#include <FxPlugin.hpp>

int main( int argc, char** argv )
{
   volatile ComInit ole;

   QString appName = "AbletonLive64 Lite (c) 2022 <benjaminhampe@gmx.de>";
   QApplication::setStyle("windows");
   QApplication app( argc, argv );
   app.setApplicationName( appName );

//   QString const appPath = qApp->applicationDirPath() + "/";
//   QString const stylePath( appPath + "themes/darcula.css" );
//   app.setStyleSheet( "file:///" + stylePath );
//   dbAddFontFamily( "fontawesome.ttf" );
//   dbAddFontFamily( "la-regular-400.ttf" );
//   dbAddFontFamily( "la-brands-400.ttf" );
//   dbAddFontFamily( "la-solid-900.ttf" );
//   QString const appPath = qApp->applicationDirPath() + "/";
//   QString const cssPath( appPath + "themes/darcula.css" );
//   app.setStyleSheet( "file:///" + cssPath );

   Live0_Window win;
   win.setWindowTitle( appName );
   win.show();
   return app.exec();
}
