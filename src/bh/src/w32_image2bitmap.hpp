// (c) 2022 by Benjamin Hampe <benjaminhampe@gmx.de>
// All code is for free use

#pragma once

#include <cstdint>
#include <sstream>
#include <DarkImage.hpp>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <windowsx.h>
#include <iostream>

inline
HBITMAP createHBitmap( de::Image const & img )
{
   size_t w = img.getWidth();
   size_t h = img.getHeight();

   std::vector< uint8_t > tmp( w * h * size_t( 4 ) );

   uint8_t const* src = img.data();
   uint8_t* dst = tmp.data();

   for ( size_t y = 0; y < h; ++y )
   {
      for ( size_t x = 0; x < w; ++x )
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

   HBITMAP hBitmap = CreateBitmap( w, h, 4, 8, (LPBYTE)tmp.data() );
   return hBitmap;
}

inline
void drawHBitmap( HDC dc, HBITMAP hBitmap )
{
   if ( !dc )
   {
      std::cout << "No dc" << std::endl;
      return;
   }

   if ( !hBitmap )
   {
      std::cout << "No hBitmap" << std::endl;
      return;
   }

   HDC mem = CreateCompatibleDC( dc );
   if ( !mem )
   {
      std::cout << "No mem dc" << std::endl;
      return;
   }

   HBITMAP old = (HBITMAP)SelectObject( mem, hBitmap );

   BITMAP bm;
   GetObject( hBitmap, sizeof( bm ), &bm );
   BitBlt( dc, 0, 0, bm.bmWidth, bm.bmHeight, mem, 0, 0, SRCCOPY );
   SelectObject( mem, old );
   DeleteDC( mem );
}

/*

struct Bitmap
{
   int w = 0;
   int h = 0;
   HBITMAP handle = nullptr;
};

Bitmap createBitmap( de::Image const & img )
{
   int w = img.getWidth();
   int h = img.getHeight();
   HBITMAP hBitmap = CreateBitmap( w, h, 4, 32, (LPBYTE) img.data() );
   Bitmap bmp;
   bmp.w = w;
   bmp.h = h;
   bmp.handle = hBitmap;
   return bmp;
}

void drawBitmap( HDC dc, Bitmap const & bmp )
{
//   PAINTSTRUCT ps;
//   HDC hdc = BeginPaint(hwnd, &ps);
   HDC cdc = CreateCompatibleDC( dc );
   //HBITMAP hbmOld = SelectObject(hdcMem, g_hbmBall);
   HBITMAP old = (HBITMAP)SelectObject( dc, bmp.handle );

   BITMAP bm;
   GetObject( bmp.handle, sizeof(bm), &bm );

   BitBlt( dc, 0, 0, bm.bmWidth, bm.bmHeight, cdc, 0, 0, SRCCOPY );
   SelectObject( cdc, old );
   DeleteDC( cdc );
}

*/
