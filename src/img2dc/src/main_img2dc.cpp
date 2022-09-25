//////////////////////////////////////////////////////////////////////////////
/// @author Benjamin Hampe <benjamin.hampe@gmx.de>
//////////////////////////////////////////////////////////////////////////////

#include <de/img2dc.hpp>
#include <iostream>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <windowsx.h>
#include <time.h>

WNDCLASSW wc;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawPixels(HWND hwnd);
void DoDrawing(HWND hwnd);
void DrawRectangles(HWND hwnd);
void DrawHatchRectangles(HWND hwnd);

//int WINAPI WinMain(
//   HINSTANCE hInstance,
//   HINSTANCE hPrevInstance,
//   PWSTR lpCmdLine,
//   int nCmdShow )
//========================================================================
int main( int argc, char** argv )
//========================================================================
{
   //std::cout << argv[ 0 ] << " by (c) 2022 by <benjaminhampe@gmx.de>\n";

   de::Image img;

   // std::string uri;
   // if ( argc > 1 )
   // {
      // uri = argv[1];
   // }

   // if ( !dbExistFile( uri ) )
   // {
      // int dw = dbDesktopWidth() - 200;
      // int dh = dbDesktopHeight() - 200;
      // int dx = 100;
      // int dy = 100;
      // uri = dbOpenFileDlg("Save screenshot to image (bmp,png,jpg,gif,tga,dds,tif)",
         // dx,dy,dw,dh, "All Files (*.*)\0*.*\0"
                      // "Bitmap (*.bmp)\0*.bmp\0"
                      // "Portable Network Graphic (*.png)\0*.png\0"
                      // "JPEG (*.jpg)\0*.jpg\0"
                      // "Graphic interchange format (*.gif)\0*.gif\0"
                      // "TrueVision (*.tga)\0*.tga\0"
                      // "DDS (*.dds)\0*.dds\0"
                      // "Tif (*.tif)\0*.tif\0"
                      // "\0" );
   // }

   // if ( uri.empty() )
   // {
      // return 0;
   // }

   MSG msg;
   //WNDCLASSW
   wc = {0};
   HINSTANCE hInstance = nullptr;
   wc.style = CS_HREDRAW | CS_VREDRAW;
   wc.lpszClassName = L"Pixels";
   wc.hInstance = hInstance;
   wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
   wc.lpfnWndProc = WndProc;
   wc.hCursor = LoadCursor(0, IDC_ARROW);
   RegisterClassW(&wc);
   CreateWindowW(wc.lpszClassName, L"Pixels",
   WS_OVERLAPPEDWINDOW | WS_VISIBLE,
   100, 100, 300, 250, NULL, NULL, hInstance, NULL);

   while ( GetMessage(&msg, NULL, 0, 0) )
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }

   srand(time(NULL));
   return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   switch(msg)
   {
   case WM_CREATE:
   {
      //in Frame's constructor:
      //HWND hWnd = GetHandle();
      WINDOWINFO wi = {0};
      GetWindowInfo( hwnd, &wi );
      HINSTANCE hInstance = nullptr;

      HICON hIcon = LoadIcon( wc.hInstance, L"mspaint_xp.ico" );
      if ( !hIcon )
      {
         MessageBoxW(hwnd, L"Failed to load icon", L"Error", MB_OK);
      }
      SetClassLongPtr(hwnd, GCLP_HICONSM, (LONG_PTR)hIcon);
   }

   case WM_PAINT:
      {

      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd, &ps);

      // Draw Custom Brush (Bitmap)
      UINT bits[8] = {  0x111111ff,
                        0xffffffff,
                        0xffffffff,
                        0xffffffff,
                        0x00000000,
                        0x00000000,
                        0x00000000,
                        0x00000000 };
      HBITMAP hBtm = CreateBitmap(8, 8, 1, 1, (LPBYTE) bits);
      HBRUSH hCustomBrush = CreatePatternBrush(hBtm);
      HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hCustomBrush);
      SelectObject(hdc, GetStockObject(NULL_PEN));
      Rectangle(hdc, 20, 20, 250, 160);
      SelectObject(hdc, hOldBrush);
      DeleteObject(hCustomBrush);
      SelectObject(hdc, GetStockObject(BLACK_PEN));
      DeleteObject(hBtm);

//      HBITMAP hBitmap = (HBITMAP)LoadImageW(NULL, L"C:\\prog\\slovakia.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//      if (hBitmap == NULL)
//      {
//         MessageBoxW(hwnd, L"Failed to load image", L"Error", MB_OK);
//      }
//      HDC hdcMem = CreateCompatibleDC(hdc);
//      HBITMAP oldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);
//      GetObject(hBitmap, sizeof(bitmap), &bitmap);
//      BitBlt(hdc, 5, 5, bitmap.bmWidth, bitmap.bmHeight,hdcMem, 0, 0, SRCCOPY);
//      SelectObject(hdcMem, oldBitmap);
//      DeleteDC(hdcMem);

      // Draw Shapes
      Ellipse(hdc, 30, 30, 120, 90);
      RoundRect(hdc, 150, 30, 240, 90, 15, 20);
      Chord(hdc, 270, 30, 360, 90, 270, 45, 360, 45);
      POINT const polygon[10] = { 30, 145, 85, 165, 105, 110, 65, 125, 30, 105 };
      Polygon(hdc, polygon, 5);
      Rectangle(hdc, 150, 110, 230, 160);

      // Draw Texts
      DWORD color = GetSysColor(COLOR_BTNFACE);
      SetBkColor(hdc, color);
      HFONT hFont = CreateFontW(15, 0,0,0, FW_MEDIUM, 0,0,0,0, 0,0,0,0, L"Georgia");
      HFONT holdFont = (HFONT)SelectObject(hdc, hFont);

      wchar_t *ver1 = L"Not marble, nor the gilded monuments";
      wchar_t *ver2 = L"Of princes, shall outlive this powerful rhyme;";
      wchar_t *ver3 = L"But you shall shine more bright in these contents";
      wchar_t *ver4 = L"Than unswept stone, besmear'd with sluttish time.";
      wchar_t *ver5 = L"When wasteful war shall statues overturn,";
      wchar_t *ver6 = L"And broils root out the work of masonry,";
      wchar_t *ver7 = L"Nor Mars his sword, nor war's quick fire shall burn";
      wchar_t *ver8 = L"The living record of your memory.";
      wchar_t *ver9 = L"'Gainst death, and all oblivious enmity";
      wchar_t *ver10 = L"Shall you pace forth; your praise shall still find room";
      wchar_t *ver11 = L"Even in the eyes of all posterity";
      wchar_t *ver12 = L"That wear this world out to the ending doom.";
      wchar_t *ver13 = L"So, till the judgment that yourself arise,";
      wchar_t *ver14 = L"You live in this, and dwell in lovers' eyes.";

      TextOutW(hdc, 50, 20, ver1, lstrlenW(ver1));
      TextOutW(hdc, 50, 40, ver2, lstrlenW(ver2));
      TextOutW(hdc, 50, 60, ver3, lstrlenW(ver3));
      TextOutW(hdc, 50, 80, ver4, lstrlenW(ver4));
      TextOutW(hdc, 50, 100, ver5, lstrlenW(ver5));
      TextOutW(hdc, 50, 120, ver6, lstrlenW(ver6));
      TextOutW(hdc, 50, 140, ver7, lstrlenW(ver7));
      TextOutW(hdc, 50, 160, ver8, lstrlenW(ver8));
      TextOutW(hdc, 50, 180, ver9, lstrlenW(ver9));
      TextOutW(hdc, 50, 200, ver10, lstrlenW(ver10));
      TextOutW(hdc, 50, 220, ver11, lstrlenW(ver11));
      TextOutW(hdc, 50, 240, ver12, lstrlenW(ver12));
      TextOutW(hdc, 50, 260, ver13, lstrlenW(ver13));
      TextOutW(hdc, 50, 280, ver14, lstrlenW(ver14));
      SelectObject(hdc, holdFont);
      DeleteObject(hFont);


      Rectangle(hdc, 50, 50, 200, 100);

      MoveToEx(hdc, 50, 50, NULL);
      LineTo(hdc, 250, 50);
      HPEN hWhitePen = (HPEN)GetStockObject(WHITE_PEN);
      HPEN hOldPen = (HPEN)SelectObject(hdc, hWhitePen);
      MoveToEx(hdc, 50, 100, NULL);
      LineTo(hdc, 250, 100);

      DrawPixels(hwnd);

      POINT points[4] = { 20, 40, 320, 200, 330, 110, 450, 40 };
      PolyBezier(hdc, points, 4);



      HPEN hPen1 = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
      HPEN hPen2 = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
      HPEN hPen3 = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
      HPEN hPen4 = CreatePen(PS_DASHDOT, 1, RGB(0, 0, 0));
      HPEN hPen5 = CreatePen(PS_DASHDOTDOT, 1, RGB(0, 0, 0));
      HPEN holdPen = (HPEN)SelectObject(hdc, hPen1);

      MoveToEx(hdc, 50, 30, NULL);
      LineTo(hdc, 300, 30);
      SelectObject(hdc, hPen2);
      MoveToEx(hdc, 50, 50, NULL);
      LineTo(hdc, 300, 50);
      SelectObject(hdc, hPen2);
      MoveToEx(hdc, 50, 70, NULL);
      LineTo(hdc, 300, 70);
      SelectObject(hdc, hPen3);
      MoveToEx(hdc, 50, 90, NULL);
      LineTo(hdc, 300, 90);
      SelectObject(hdc, hPen4);
      MoveToEx(hdc, 50, 110, NULL);
      LineTo(hdc, 300, 110);
      SelectObject(hdc, holdPen);
      DeleteObject(hPen1);
      DeleteObject(hPen2);
      DeleteObject(hPen3);
      DeleteObject(hPen4);
      DeleteObject(hPen5);

      SelectObject(hdc, hOldPen);
      EndPaint(hwnd, &ps);

      DoDrawing(hwnd);
      DrawRectangles(hwnd);
      DrawHatchRectangles(hwnd);
      }
      break;
   case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
   }
   return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void DrawPixels(HWND hwnd)
{
   RECT r;
   GetClientRect( hwnd, &r );

   if ( r.bottom == 0 )
   {
      return;
   }

   PAINTSTRUCT ps;
   HDC hdc = BeginPaint(hwnd, &ps);

   for ( int i=0; i<1000; i++ )
   {
      int x = rand() % r.right;
      int y = rand() % r.bottom;
      SetPixel(hdc, x, y, RGB(255, 0, 0));
   }

   EndPaint(hwnd, &ps);

   DoDrawing(hwnd);
}


void DoDrawing(HWND hwnd)
{
   LOGBRUSH brush;
   COLORREF col = RGB(0, 0, 0);
   DWORD pen_style = PS_SOLID | PS_JOIN_MITER | PS_GEOMETRIC;
   brush.lbStyle = BS_SOLID;
   brush.lbColor = col;
   brush.lbHatch = 0;
   PAINTSTRUCT ps;
   HDC hdc = BeginPaint(hwnd, &ps);
   HPEN hPen1 = ExtCreatePen(pen_style, 8, &brush, 0, NULL);
   HPEN holdPen = (HPEN)SelectObject(hdc, hPen1);
   POINT points[5] = {  { 30, 30 }, { 130, 30 }, { 130, 100 },
                        { 30, 100 }, { 30, 30}  };
   Polygon(hdc, points, 5);
   pen_style = PS_SOLID | PS_GEOMETRIC | PS_JOIN_BEVEL;
   HPEN hPen2 = ExtCreatePen(pen_style, 8, &brush, 0, NULL);
   SelectObject(hdc, hPen2);
   DeleteObject(hPen1);
   POINT points2[5] = { { 160, 30 }, { 260, 30 }, { 260, 100 },
                        { 160, 100 }, {160, 30 }};
   MoveToEx(hdc, 130, 30, NULL);
   Polygon(hdc, points2, 5);
   pen_style = PS_SOLID | PS_GEOMETRIC | PS_JOIN_ROUND;
   HPEN hPen3 = ExtCreatePen(pen_style, 8, &brush, 0, NULL);
   SelectObject(hdc, hPen3);
   DeleteObject(hPen2);
   POINT points3[5] = { { 290, 30 }, { 390, 30 }, { 390, 100 },
                        { 290, 100 }, {290, 30 }   };
   MoveToEx(hdc, 260, 30, NULL);
   Polygon(hdc, points3, 5);
   SelectObject(hdc, holdPen);
   DeleteObject(hPen3);
   EndPaint(hwnd, &ps);
}

void DrawRectangles(HWND hwnd)
{
   PAINTSTRUCT ps;
   HDC hdc = BeginPaint(hwnd, &ps);
   HPEN hPen = CreatePen(PS_NULL, 1, RGB(0, 0, 0));
   HPEN holdPen = (HPEN)SelectObject(hdc, hPen);
   HBRUSH hBrush1 = CreateSolidBrush(RGB(121, 90, 0));
   HBRUSH hBrush2 = CreateSolidBrush(RGB(240, 63, 19));
   HBRUSH hBrush3 = CreateSolidBrush(RGB(240, 210, 18));
   HBRUSH hBrush4 = CreateSolidBrush(RGB(9, 189, 21));
   HBRUSH holdBrush = (HBRUSH)SelectObject(hdc, hBrush1);
   Rectangle(hdc, 30, 30, 100, 100);
   SelectObject(hdc, hBrush2);
   Rectangle(hdc, 110, 30, 180, 100);
   SelectObject(hdc, hBrush3);
   Rectangle(hdc, 30, 110, 100, 180);
   SelectObject(hdc, hBrush4);
   Rectangle(hdc, 110, 110, 180, 180);
   SelectObject(hdc, holdPen);
   SelectObject(hdc, holdBrush);
   DeleteObject(hPen);
   DeleteObject(hBrush1);
   DeleteObject(hBrush2);
   DeleteObject(hBrush3);
   DeleteObject(hBrush4);
   EndPaint(hwnd, &ps);
}

void DrawHatchRectangles(HWND hwnd)
{
   PAINTSTRUCT ps;
   HDC hdc = BeginPaint(hwnd, &ps);
   HPEN hPen = CreatePen(PS_NULL, 1, RGB(0, 0, 0));
   HPEN holdPen = (HPEN)SelectObject(hdc, hPen);
   HBRUSH hBrush1 = CreateHatchBrush(HS_BDIAGONAL, RGB(0, 0, 0));
   HBRUSH hBrush2 = CreateHatchBrush(HS_FDIAGONAL, RGB(0, 0, 0));
   HBRUSH hBrush3 = CreateHatchBrush(HS_CROSS, RGB(0, 0, 0));
   HBRUSH hBrush4 = CreateHatchBrush(HS_HORIZONTAL, RGB(0, 0, 0));
   HBRUSH hBrush5 = CreateHatchBrush(HS_DIAGCROSS, RGB(0, 0, 0));
   HBRUSH hBrush6 = CreateHatchBrush(HS_VERTICAL, RGB(0, 0, 0));
   HBRUSH holdBrush = (HBRUSH)SelectObject(hdc, hBrush1);
   DWORD col = GetSysColor(COLOR_BTNFACE);
   SetBkColor(hdc, col);
   Rectangle(hdc, 30, 30, 100, 80);
   SelectObject(hdc, hBrush2);
   Rectangle(hdc, 110, 30, 180, 80);
   SelectObject(hdc, hBrush3);
   Rectangle(hdc, 190, 30, 260, 80);
   SelectObject(hdc, hBrush4);
   Rectangle(hdc, 30, 110, 100, 160);
   SelectObject(hdc, hBrush5);
   Rectangle(hdc, 110, 110, 180, 160);
   SelectObject(hdc, hBrush6);
   Rectangle(hdc, 190, 110, 260, 160);
   SelectObject(hdc, holdPen);
   SelectObject(hdc, holdBrush);
   DeleteObject(hPen);
   DeleteObject(hBrush1);
   DeleteObject(hBrush2);
   DeleteObject(hBrush3);
   DeleteObject(hBrush4);
   DeleteObject(hBrush5);
   DeleteObject(hBrush6);
   EndPaint(hwnd, &ps);
}
