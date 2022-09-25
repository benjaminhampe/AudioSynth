// (C) 2002-2022 Benjamin Hampe, for free use

#include <de/img2dc.hpp>

/*
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
   // HGDIOBJ old_obj =
	SelectObject( hDC, hBitmap );
   // BOOL bRet =
	BitBlt( hDC, 0, 0, w, h, hScreen, x1, y1, SRCCOPY );
   DeleteDC( hDC );

   de::Image img = toImage( hBitmap );
   DeleteObject( hBitmap );

   ShowWindow( hWnd, SW_SHOW );

   w = img.getWidth();
   h = img.getHeight();
   std::cout << "Created image(" << w << "," << h << ") from desktop." << std::endl;
   return img;
}

} // END NAMSPACE de

*/