// (C) 2002-2022 Benjamin Hampe, for free use

#include <de/CaptureScreen.hpp>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <windowsx.h>
#include <iostream>

namespace de {

std::vector< uint8_t >
ToPixels(HBITMAP BitmapHandle, int &width, int &height)
{
    BITMAP Bmp = {0};
    BITMAPINFO Info = {0};
    std::vector< uint8_t > Pixels;

    HDC DC = CreateCompatibleDC(NULL);
    std::memset(&Info, 0, sizeof(BITMAPINFO)); //not necessary really..
    HBITMAP OldBitmap = (HBITMAP)SelectObject(DC, BitmapHandle);
    GetObject(BitmapHandle, sizeof(Bmp), &Bmp);

    Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    Info.bmiHeader.biWidth = width = Bmp.bmWidth;
    Info.bmiHeader.biHeight = height = Bmp.bmHeight;
    Info.bmiHeader.biPlanes = 1;
    Info.bmiHeader.biBitCount = Bmp.bmBitsPixel;
    Info.bmiHeader.biCompression = BI_RGB;
    Info.bmiHeader.biSizeImage = ((width * Bmp.bmBitsPixel + 31) / 32) * 4 * height;

    Pixels.resize(Info.bmiHeader.biSizeImage);
    GetDIBits(DC, BitmapHandle, 0, height, &Pixels[0], &Info, DIB_RGB_COLORS);
    SelectObject(DC, OldBitmap);

    height = std::abs(height);
    DeleteDC(DC);

    return Pixels;
}


de::Image
toImage( HBITMAP hBitmap )
{
   int w = 0;
   int h = 0;
   std::vector< uint8_t > pixels = ToPixels( hBitmap, w, h );
   de::Image img( w,h );
   //img.fill( 0xFFFFFFFF );

   size_t rs = size_t( w ) * 4; // scanline size = rowsize
   size_t is = rs * size_t( h ); // image data size = rowsize * rowcount
   //size_t n = size_t( w ) * size_t( h );

   uint8_t* dst = img.data();
   for ( size_t y = 0; y < size_t( h ); ++y )
   {
      uint8_t const* src = pixels.data() + is - ((y+1)*rs);
      for ( size_t x = 0; x < size_t( w ); ++x )
      {
         uint8_t b = *src++;
         uint8_t g = *src++;
         uint8_t r = *src++;
         uint8_t a = *src++;
         *dst++ = r;
         *dst++ = g;
         *dst++ = b;
         *dst++ = a;
      }
   }

   return img;
}

de::Image
CaptureScreen::createImageFromDesktop()
{
   // Hide console window
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
   HBITMAP hBitmap = CreateCompatibleBitmap( hScreen, w, h );
   /*HGDIOBJ old_obj =*/ SelectObject( hDC, hBitmap );
   /*BOOL bRet =*/ BitBlt( hDC, 0, 0, w, h, hScreen, x1, y1, SRCCOPY );
   DeleteDC( hDC );

   de::Image img = toImage( hBitmap );
   DeleteObject( hBitmap );

   ShowWindow( hWnd, SW_SHOW );

   w = img.getWidth();
   h = img.getHeight();
   std::cout << "Created image(" << w << "," << h << ") from desktop." << std::endl;
   return img;
}

/*
 *
std::vector<unsigned char>
ToPixels(HBITMAP orig_bmp, int &width, int &height)
{
    BITMAP Bmp = {0};
    BITMAPINFO Info = {0};
    std::vector<unsigned char> Pixels = std::vector<unsigned char>();

    HDC DC = CreateCompatibleDC(NULL);
    std::memset(&Info, 0, sizeof(BITMAPINFO)); //not necessary really..
    HBITMAP OldBitmap = (HBITMAP)SelectObject(DC, orig_bmp);
    GetObject(orig_bmp, sizeof(Bmp), &Bmp);

    Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    Info.bmiHeader.biWidth = width = Bmp.bmWidth;
    Info.bmiHeader.biHeight = height = Bmp.bmHeight;
    Info.bmiHeader.biPlanes = 1;
    Info.bmiHeader.biBitCount = Bmp.bmBitsPixel;
    Info.bmiHeader.biCompression = BI_RGB;
    Info.bmiHeader.biSizeImage = ((width * Bmp.bmBitsPixel + 31) / 32) * 4 * height;

    Pixels.resize(Info.bmiHeader.biSizeImage);
    GetDIBits(DC, orig_bmp, 0, height, &Pixels[0], &Info, DIB_RGB_COLORS);
    SelectObject(DC, OldBitmap);

    height = std::abs(height);
    DeleteDC(DC);
    return Pixels;
}

// Circumvent use of a manifest file
void InitializeDPIScale(HWND hwnd)
{
   HDC hdc = GetDC(hwnd);
   g_DPIScaleX = GetDeviceCaps(hdc, LOGPIXELSX) / 96.0f;
   g_DPIScaleY = GetDeviceCaps(hdc, LOGPIXELSY) / 96.0f;
   ReleaseDC(hwnd, hdc);
}



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
   // HGDIOBJ old_obj = SelectObject( hDC, hBitmap );
   // BOOL bRet = BitBlt( hDC, 0, 0, w, h, hScreen, x1, y1, SRCCOPY );

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

*/


} // END NAMSPACE de
