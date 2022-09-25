// (C) 2002-2022 Benjamin Hampe, for free use

#include <bh_ui.hpp>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <windowsx.h>

#include <time.h>

#include <w32_FileNameDlg.hpp>
#include <w32_image2bitmap.hpp>
#include <w32_AboutDlg.hpp>

#include "../res/resource.h"

struct W32_App
{
   int32_t x = 0;
   int32_t y = 0;
   int32_t w = 0;
   int32_t h = 0;
   HWND hWindow = nullptr;
   HINSTANCE hInstance = nullptr;

   de::Image m_Image;
   HBITMAP m_Bitmap = nullptr;
};

W32_App app;

LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

void createWindow();
void createMenus();
void paintEvent();

void createWindow()
{
   WNDCLASSW wc = {0};
   HINSTANCE hInstance = nullptr;
   wc.style = CS_HREDRAW | CS_VREDRAW;
   wc.lpszClassName = L"VLC";
   wc.hInstance = hInstance;
   wc.hbrBackground = GetSysColorBrush( COLOR_3DFACE );
   wc.lpfnWndProc = WndProc;
   wc.hCursor = LoadCursor(0, IDC_ARROW);
   RegisterClassW( &wc );

   CreateWindowW(
      wc.lpszClassName,
      L"VLC",
      WS_OVERLAPPEDWINDOW | WS_VISIBLE,
      app.x=100,
      app.y=100,
      app.w=640,
      app.h=480,
      nullptr,
      nullptr,
      hInstance,
      nullptr
   );

   app.hInstance = hInstance;
}

void createApp()
{
   srand( time(nullptr) );
   createWindow();
}

int runApp()
{
   MSG msg;
   while ( GetMessage( &msg, nullptr, 0, 0 ) )
   {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
   }

   return int(msg.wParam);
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
   #ifndef WM_MOUSEWHEEL
   #define WM_MOUSEWHEEL 0x020A
   #endif
   #ifndef WHEEL_DELTA
   #define WHEEL_DELTA 120
   #endif

   switch(msg)
   {
   case WM_CREATE:
      {
         app.hWindow = hwnd;
         //in Frame's constructor:
         //HWND hWnd = GetHandle();
         HINSTANCE hInstance = GetModuleHandle( nullptr );

//         SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(NULL,
//                     MAKEINTRESOURCE(IDI_APPLICATION)));
//         SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(NULL,
//                     MAKEINTRESOURCE(IDI_APPLICATION)));

         HICON hIcon = LoadIcon( hInstance, MAKEINTRESOURCE(aaaa) );
         if ( hIcon )
         {
            SetClassLongPtr( hwnd, GCLP_HICON, (LONG_PTR)hIcon );
            SetClassLongPtr( hwnd, GCLP_HICONSM, (LONG_PTR)hIcon );
         }
         else
         {
            //MessageBoxW(hwnd, L"Failed to load icon", L"Error", MB_OK);
         }

         createMenus();
      }
      break;
   case WM_COMMAND:
      {
         switch( wParam )
         {
         case ID_FILE_EXIT:
            {
               DestroyWindow( hwnd );
            }
            break;
         case ID_FILE_LOAD:
            {
               std::wstring uri = loadFileNameDlgW();
               loadFile( uri );
            }
            break;
         case ID_FILE_SAVE:
            {
               std::wstring uri = saveFileNameDlgW();
               saveFile( uri );
            }
            break;
         case ID_HELP_ABOUT:
            {
               //std::cout << "About" << std::endl;
               doAboutDialog( hwnd );
            }
            break;
         default:
            break;
         }
      } break;
   case WM_SIZE:
      {
         int w = int( LOWORD(lParam) );
         int h = int( HIWORD(lParam) );
         app.w = w;
         app.h = h;
         std::cout << "WM_SIZE("<< w << "," << h << ")" << std::endl;

         //         RECT r_cl;
         //         GetClientRect( hwnd, &r_cl );
         //         int cw = r_cl.right - r_cl.left;
         //         int ch = r_cl.bottom - r_cl.top;

         int dw = GetSystemMetrics( SM_CXSCREEN );
         int dh = GetSystemMetrics( SM_CYSCREEN );

         std::wostringstream s;
         s << "BH Viewer 2022 | Desktop(" << dw << "," << dh << "), " <<
            "Window(" << w << "," << h << ")";
            //", ClientRect(" << cw << "," << ch << ")";

         setWindowTitle( s.str().c_str() );
      }
      break;
   case WM_PAINT:
      {
         //std::cout << "WM_PAINT\n";
         paintEvent();
      }
      break;

   case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
   default:
      //std::cout << "Unhandled Event " << msg << "\n";
      break;
   }
   return DefWindowProcW( hwnd, msg, wParam, lParam );
}

void paintEvent()
{
   HWND hwnd = app.hWindow;
   PAINTSTRUCT ps;
   HDC hdc = BeginPaint( hwnd, &ps );

   HBRUSH hBrush1 = CreateSolidBrush( RGB( 250, 100, 10 ) );
   HBRUSH hBrush0 = (HBRUSH)SelectObject( hdc, hBrush1 );
   Rectangle( hdc, 0, 0, app.w, app.h );
   SelectObject( hdc, hBrush0 );

   drawHBitmap( hdc, app.m_Bitmap );

   EndPaint( hwnd, &ps );
}

void createMenus()
{
   HWND hwnd = app.hWindow;
   HMENU hMenu = CreateMenu();
   HMENU hMenuFile = CreateMenu();
   HMENU hMenuHelp = CreateMenu();

   AppendMenu( hMenu, MF_POPUP, (UINT_PTR)hMenuFile, TEXT("Start") );
   AppendMenu( hMenuFile, MF_STRING, ID_FILE_LOAD, TEXT("&Load file") );
   AppendMenu( hMenuFile, MF_STRING, ID_FILE_SAVE, TEXT("&Save file") );
   AppendMenu( hMenuFile, MF_SEPARATOR, 0, 0 );
   AppendMenu( hMenuFile, MF_STRING, ID_FILE_EXIT, TEXT("&Exit application") );

   AppendMenu( hMenu, MF_POPUP, (UINT_PTR)hMenuHelp, TEXT("Help") );
   AppendMenu( hMenuHelp, MF_SEPARATOR, 0, 0 );
   AppendMenu( hMenuHelp, MF_STRING, ID_HELP_ABOUT, TEXT("About") );

   SetMenu( hwnd, hMenu );
}

void refreshScreen()
{
   HWND hwnd = app.hWindow;
   RECT r;
   GetClientRect( hwnd, &r );
   InvalidateRect( hwnd, &r, FALSE );
}

void resizeWindow( int w, int h )
{

}

void setWindowTitle( wchar_t const* title )
{
   HWND hwnd = app.hWindow;
   // SendMessage instead of SetText for cases where HWND was created in a different thread
   DWORD_PTR dwResult;
   SendMessageTimeoutW(
      hwnd,
      WM_SETTEXT,
      0,
      reinterpret_cast< LPARAM >( title ),
      SMTO_ABORTIFHUNG,
      2000,
      &dwResult
   );
}

void loadFile( std::wstring const & uri )
{
   HWND hwnd = app.hWindow;

   bool ok = dbLoadImage( app.m_Image, dbStr( uri ) );

   DeleteObject( app.m_Bitmap );

   if ( ok )
   {
      std::cout << "Create Bitmap" << std::endl;
      app.m_Bitmap = createHBitmap( app.m_Image );
      if ( !app.m_Bitmap )
      {
         std::cout << "No Bitmap created" << std::endl;
      }
      else
      {
         std::cout << "Bitmap created" << std::endl;
      }
   }
   else
   {
      app.m_Image.clear( true );
   }

   refreshScreen();

}

void saveFile( std::wstring const & uri )
{
   HWND hwnd = app.hWindow;
}

/*
 *
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

*/
