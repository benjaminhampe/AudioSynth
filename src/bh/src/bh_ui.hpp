// (c) 2022 by Benjamin Hampe <benjaminhampe@gmx.de>
// All code is for free use

#pragma once

#include <cstdint>
#include <sstream>
#include <fstream>
#include <iostream>
#include <DarkImage.hpp>

void createApp();
int runApp();

void setWindowTitle( wchar_t const* title );

void loadFile( std::wstring const & uri );
void saveFile( std::wstring const & uri );


void refreshScreen();
void resizeWindow( int w, int h );
