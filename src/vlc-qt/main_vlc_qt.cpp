/// @author (♥) Benjamin Hampe <benjamin.hampe@gmx.de>

#include "VLC_Window.hpp"
#include <QApplication>
#include <QGuiApplication>
#include <QFontDatabase>
#include "embedded_fontawesome463_ttf.hpp"
#include "embedded_arial_ttf.hpp"
#include <iostream>

int main( int argc, char** argv )
{
   QString appName = "VLC Player (c) 2021 Benjamin Hampe using DarkVLC, DarkGPU and DarkImage";
   //QApplication::setStyle("windows");
   QApplication app( argc, argv );
   app.setApplicationName( appName );

   if ( !dbExistFile("fontawesome463.ttf") )
   {
      de::FileToHeader::saveRAMtoHDD( fontawesome463_ttf, 152796, "fontawesome463.ttf");
   }

   int familyID = QFontDatabase::addApplicationFont( "fontawesome463.ttf" );

   QStringList lst = QFontDatabase::applicationFontFamilies( familyID );
   std::cout << "QStringList.size() = " << lst.size() << "\n";
   for ( size_t i = 0; i < lst.size(); ++i )
   {
      std::cout << "FamilyId["<<familyID<<"].FontName["<<i<<"] "<< lst.at(i).toStdString() << "\n";
   }

   //QString const appPath = qApp->applicationDirPath() + "/";
//   QString const stylePath( appPath + "themes/darcula.css" );
//   app.setStyleSheet( "file:///" + stylePath );
//   dbAddFontFamily( "fontawesome.ttf" );
//   dbAddFontFamily( "la-regular-400.ttf" );
//   dbAddFontFamily( "la-brands-400.ttf" );
//   dbAddFontFamily( "la-solid-900.ttf" );
//   QString const appPath = qApp->applicationDirPath() + "/";
//   QString const cssPath( appPath + "themes/darcula.css" );
//   app.setStyleSheet( "file:///" + cssPath );

   VLC_Window win;
   win.setWindowTitle( appName );
   win.show();
   return app.exec();
}

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

