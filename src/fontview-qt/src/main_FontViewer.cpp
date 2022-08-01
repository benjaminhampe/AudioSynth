#include <QApplication>
#include <QCommandLineParser>
#include "MainWindow.hpp"

int main( int argc, char * argv[] )
{
   ::srand( (unsigned int)time( nullptr ) );

   char const * appName = "Font and Glyph Viewer (c) 2019 by Benjamin Hampe <benjaminhampeatgmx.de>";

   QApplication::setStyle("windows");
   QApplication app( argc, argv );
   app.setApplicationName( appName );

//   QString const appPath = qApp->applicationDirPath() + "/";
//   QString const styleFilePath( appPath + "themes/darcula.css" );
//   app.setStyleSheet( "file:///" + styleFilePath );

   addFontAwesome463();


   MainWindow* test_wigdet = new MainWindow( nullptr );
   test_wigdet->setWindowTitle( appName );
   test_wigdet->show();

   return app.exec();
}
