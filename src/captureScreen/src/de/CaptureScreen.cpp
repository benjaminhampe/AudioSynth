// (C) 2002-2022 Benjamin Hampe, for free use

#include <de/CaptureScreen.hpp>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <windowsx.h>
#include <iostream>

namespace de {

/* Circumvent use of a manifest file
void InitializeDPIScale(HWND hwnd)
{
   HDC hdc = GetDC(hwnd);
   g_DPIScaleX = GetDeviceCaps(hdc, LOGPIXELSX) / 96.0f;
   g_DPIScaleY = GetDeviceCaps(hdc, LOGPIXELSY) / 96.0f;
   ReleaseDC(hwnd, hdc);
}
*/


void
CaptureScreen::copyBitmapToClipboard()
{
   //::SetProcessDpiAwarenessContext();

   HWND hWnd = GetConsoleWindow();
   ShowWindow( hWnd, SW_HIDE );
   Sleep( 1000 );

//   int x1 = GetSystemMetrics( SM_XVIRTUALSCREEN );
//   int y1 = GetSystemMetrics( SM_YVIRTUALSCREEN );
//   int x2 = GetSystemMetrics( SM_CXVIRTUALSCREEN );
//   int y2 = GetSystemMetrics( SM_CYVIRTUALSCREEN );

   int x1 = 0;
   int y1 = 0;
   int x2 = ::GetSystemMetrics( SM_CXSCREEN );
   int y2 = ::GetSystemMetrics( SM_CYSCREEN );

   int w = x2 - x1;
   int h = y2 - y1;

   HDC hScreen = GetDC( nullptr );
   HDC hDC = CreateCompatibleDC( hScreen );

   //HDC hdc = GetDC(hwnd);
   int logPixelX = GetDeviceCaps( hDC, LOGPIXELSX );
   int logPixelY = GetDeviceCaps( hDC, LOGPIXELSY );
   float dpiScaleX = float( logPixelX ) / 96.0f;
   float dpiScaleY = float( logPixelY ) / 96.0f;
   std::cout << "LOGPIXELSX = " << logPixelX << "\n";
   std::cout << "LOGPIXELSY = " << logPixelY << "\n";
   std::cout << "dpiScaleX = " << dpiScaleX << "\n";
   std::cout << "dpiScaleY = " << dpiScaleY << "\n";

   //ReleaseDC(hwnd, hdc);

   HBITMAP hBitmap = CreateCompatibleBitmap( hScreen, w, h );
   /*HGDIOBJ old_obj =*/ SelectObject( hDC, hBitmap );
   /*BOOL bRet =*/ BitBlt( hDC, 0, 0, w, h, hScreen, x1, y1, SRCCOPY );

   OpenClipboard( nullptr );
   EmptyClipboard();
   SetClipboardData( CF_BITMAP, hBitmap );

   CloseClipboard();

// Infos

   ShowWindow( hWnd, SW_SHOW );

   std::cout << "SM_XVIRTUALSCREEN = " << GetSystemMetrics( SM_XVIRTUALSCREEN ) << "\n";
   std::cout << "SM_YVIRTUALSCREEN = " << GetSystemMetrics( SM_YVIRTUALSCREEN ) << "\n";
   std::cout << "SM_CXVIRTUALSCREEN = " << GetSystemMetrics( SM_CXVIRTUALSCREEN ) << "\n";
   std::cout << "SM_CYVIRTUALSCREEN = " << GetSystemMetrics( SM_CYVIRTUALSCREEN ) << "\n";

   std::cout << "SM_CXSIZE = " << GetSystemMetrics( SM_CXSIZE ) << "\n";
   std::cout << "SM_CYSIZE = " << GetSystemMetrics( SM_CYSIZE ) << "\n";
   std::cout << "SM_CXSCREEN = " << GetSystemMetrics( SM_CXSCREEN ) << "\n";
   std::cout << "SM_CYSCREEN = " << GetSystemMetrics( SM_CYSCREEN ) << "\n";

   std::cout <<
      "\nWin10 returns wrong screen size!!! "
      "Due to bug ( my opinion ) using scaling over 100%, aka High-DPI bug!!!\n"
      "This is one of the most annoying bug ever\n"
      "Circumvent by setting exe properties manually ->"
      "Right click on exe -> select Properties -> Check lower box.\n"
      "Let app control dpi!!!\n\n";

   std::cout << "Copied screenshot to clipboard." << std::endl;
   //return 0;
}

} // END NAMSPACE de
