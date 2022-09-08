//////////////////////////////////////////////////////////////////////////////
/// @file main_captureScreen.cpp
/// @author Benjamin Hampe <benjamin.hampe@gmx.de>
//////////////////////////////////////////////////////////////////////////////

#include <de/CaptureScreen.hpp>
#include <iostream>

//========================================================================
int main( int argc, char** argv )
//========================================================================
{
   std::cout << argv[ 0 ] << " by (c) 2022 by <benjaminhampe@gmx.de>\n";

   de::CaptureScreen::copyBitmapToClipboard();

   return 0;
}
