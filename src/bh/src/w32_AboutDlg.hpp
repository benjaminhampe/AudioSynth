// (c) 2022 by Benjamin Hampe <benjaminhampe@gmx.de>
// All code is for free use

#pragma once

#include <cstdint>
#include <sstream>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <windowsx.h>
#include <iostream>

#include "../res/resource.h"

// DLGPROC a;

inline INT_PTR CALLBACK
AboutDlgProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
   switch(Message)
   {
   case WM_INITDIALOG:
      return TRUE;
   case WM_COMMAND:
      switch(LOWORD(wParam))
      {
      case IDOK:
         EndDialog(hwnd, IDOK);
      break;
      case IDCANCEL:
         EndDialog(hwnd, IDCANCEL);
      break;
      }
      break;
   default:
      return FALSE;
   }
   return TRUE;
}

inline void
doAboutDialog( HWND hwnd )
{
   int ret = DialogBoxParamW( GetModuleHandle(NULL),
                        MAKEINTRESOURCE(IDD_ABOUTDIALOG),
                        hwnd, AboutDlgProc, (LPARAM)0 );
   if ( ret == IDOK )
   {
//      MessageBox( hwnd,
//                  TEXT("Dialog exited with IDOK."),
//                  TEXT("Notice"),
//                  MB_OK | MB_ICONINFORMATION);
   }
   else if( ret == IDCANCEL )
   {
//      MessageBox( hwnd,
//                  TEXT("Dialog exited with IDCANCEL."),
//                  TEXT("Notice"), MB_OK | MB_ICONINFORMATION);
   }
   else if( ret == -1 )
   {
//      MessageBox( hwnd,
//                  TEXT("Dialog failed!"),
//                  TEXT("Error"), MB_OK | MB_ICONINFORMATION);
   }

}
