//////////////////////////////////////////////////////////////////////////////
/// @file main_captureScreen.cpp
/// @author Benjamin Hampe <benjamin.hampe@gmx.de>
//////////////////////////////////////////////////////////////////////////////

#include <de/CaptureScreen.hpp>
#include <iostream>


#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>

#include <FL/Fl_Image.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_BMP_Image.H>

//#include <FL/Fl_Output.H>
//#include <FL/Fl_Progress.H>

// Global pointers for the GUI objects
Fl_Window* win;
Fl_Button* m_btn1;
//Fl_Output* myoutputbox;
//Fl_Progress* progbar;

// Callback function.  This represents a time-consuming operation, during which
// we want to update the progress bar to show that something is happening.
// The flush() calls are needed, otherwise the GUI will not be updated until
// the callback has finished, so the progress bar updates will not be seen until
// too late.
void start_cb(Fl_Widget* w, long int data)
{
   // myoutputbox->value("Started");
   Fl::flush();
   // for (int i = 0; i < 1000000; i++)
   // {
      // if (i % 100000 == 0)
      // {
      // progbar->value(i);  // every so often, bump the progbar
      // Fl::flush();
      // }
      // // waste some time
      // for (volatile int j=0; j<1000; j++) {}
   // }
   // myoutputbox->value("Finished");
   // progbar->value(1000000);
}

int main( int argc, char** argv )
{
   win = new Fl_Window(256,64,"CaptureScreen");

   Fl_Image* ico = new Fl_BMP_Image("");

   win->icon( ico );
   m_btn1 = new Fl_Button( 0, 10, 50, 30, "Start" );
   m_btn1->callback( start_cb );

   // myoutputbox = new Fl_Output(50, 50, 200, 30);
   // myoutputbox->value("Waiting");

   // progbar = new Fl_Progress(50, 100, 200, 25);
   // progbar->selection_color(FL_BLUE);
   // progbar->minimum(0.0);
   // progbar->maximum(1000000.0);

   win->end();
   win->show();
   return Fl::run();
}

/*
//========================================================================
int main( int argc, char** argv )
//========================================================================
{
   std::cout << argv[ 0 ] << " by (c) 2022 by <benjaminhampe@gmx.de>\n";

   //de::CaptureScreen::copyBitmapToClipboard();
   de::Image img = de::CaptureScreen::createImageFromDesktop();

   // saveImageWithDialog( img );


   std::string uri;
   if ( argc > 1 )
   {
      uri = argv[1];
   }

   if ( !dbExistFile( uri ) )
   {
      int dw = dbDesktopWidth() - 200;
      int dh = dbDesktopHeight() - 200;
      int dx = 100;
      int dy = 100;
      uri = dbSaveFileDlg("Save screenshot to image (bmp,png,jpg,gif,tga,dds,tif)",
         dx,dy,dw,dh, "All Files (*.*)\0*.*\0"
                      "Bitmap (*.bmp)\0*.bmp\0"
                      "Portable Network Graphic (*.png)\0*.png\0"
                      "JPEG (*.jpg)\0*.jpg\0"
                      "Graphic interchange format (*.gif)\0*.gif\0"
                      "TrueVision (*.tga)\0*.tga\0"
                      "DDS (*.dds)\0*.dds\0"
                      "Tif (*.tif)\0*.tif\0"
                      "\0" );
   }

   if ( uri.empty() )
   {
      uri = "unnamed_screenshot.bmp";
   }

   if ( !dbSaveImage( img, uri ) )
   {
      dbMessageDlg( uri, "Cant save screenshot uri");
   }

   return 0;
}
*/
