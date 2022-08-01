// (C) 2002-2022 Benjamin Hampe, for free use

#include <de/CaptureScreen.hpp>

#include <Windows.h>

namespace de {

void
CaptureScreen::copyBitmapToClipboard()
{
   HWND hWnd = GetConsoleWindow();
   ShowWindow( hWnd, SW_HIDE );
   Sleep( 1000 );

   int x1 = GetSystemMetrics( SM_XVIRTUALSCREEN );
   int y1 = GetSystemMetrics( SM_YVIRTUALSCREEN );
   int x2 = GetSystemMetrics( SM_CXVIRTUALSCREEN );
   int y2 = GetSystemMetrics( SM_CYVIRTUALSCREEN );

   int w = x2 - x1;
   int h = y2 - y1;

   HDC hScreen = GetDC( nullptr );
   HDC hDC = CreateCompatibleDC( hScreen );
   HBITMAP hBitmap = CreateCompatibleBitmap( hScreen, w, h );
   /*HGDIOBJ old_obj =*/ SelectObject( hDC, hBitmap );
   /*BOOL bRet =*/ BitBlt( hDC, 0, 0, w, h, hScreen, x1, y1, SRCCOPY );

   OpenClipboard( nullptr );
   EmptyClipboard();
   SetClipboardData( CF_BITMAP, hBitmap );

   CloseClipboard();

   //return 0;
}

} // END NAMSPACE de
