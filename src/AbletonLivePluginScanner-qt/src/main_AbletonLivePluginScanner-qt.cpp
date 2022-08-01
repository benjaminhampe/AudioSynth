//////////////////////////////////////////////////////////////////////////////
/// @file main_AudioSynth-qt.cpp
/// @author Benjamin Hampe <benjamin.hampe@gmx.de>
//////////////////////////////////////////////////////////////////////////////
#include <QApplication>
#include <QGuiApplication>

#include <ComInit.hpp>
#include <GetPluginInfoList.hpp>
#include <iostream>

int main( int argc, char** argv )
{
   //volatile ComInit ole;

   QString appName = "AbletonLive64 Lite - VST Plugin Database Scanner (c) 2022 <benjaminhampe@gmx.de>";
   QApplication::setStyle("windows");
   QApplication app( argc, argv );
   app.setApplicationName( appName );

   auto pluginList = de::audio::getPluginInfoList( L"G:/iso/2_dev/_vst_x64", true );
   std::wcout << pluginList.toWString() << std::endl;

   pluginList.saveXml( "data/plugin_db.xml" );
   pluginList.loadXml( "data/plugin_db.xml" );
   pluginList.saveXml( "data/plugin_db2.xml" );
   return 0;
}
