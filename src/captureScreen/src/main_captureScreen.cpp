//////////////////////////////////////////////////////////////////////////////
/// @file main_captureScreen.cpp
/// @author Benjamin Hampe <benjamin.hampe@gmx.de>
//////////////////////////////////////////////////////////////////////////////

#include <de/CaptureScreen.hpp>
#include <iostream>

/*
int saveImageWithDialog( de::Image const & img )
{
   std::string uri;
   if ( argc > 1 )
   {
   loadUri = argv[1];
   }

   if ( !dbExistFile(loadUri) )
   {
   loadUri = dbOpenFileDlg("Load/open any file that contains an audio stream (mp3,wav,mkv,avi,mp4,etc..) that is supported by libAVCodec ( vlc )");
   }
}
*/

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
