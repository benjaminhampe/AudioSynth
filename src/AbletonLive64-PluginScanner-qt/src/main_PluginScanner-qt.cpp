//////////////////////////////////////////////////////////////////////////////
/// @file main_AudioSynth-qt.cpp
/// @author Benjamin Hampe <benjamin.hampe@gmx.de>
//////////////////////////////////////////////////////////////////////////////
#include <QApplication>
#include <QGuiApplication>

//#include <ComInit.hpp>
#include "PluginScanner.hpp"
#include <iostream>

int main( int argc, char** argv )
{
   //volatile ComInit ole;

   QString appName = "AbletonLive64 Lite - VST Plugin Database Scanner (c) 2022 <benjaminhampe@gmx.de>";
   QApplication::setStyle("windows");
   QApplication app( argc, argv );
   app.setApplicationName( appName );

   PluginScanner* scanner = new PluginScanner( nullptr );
   scanner->show();

   scanner->scanPluginDir( L"G:/iso/3_audio/_vst_x64" );

   // std::wcout << pluginList.toWString() << std::endl;

   int retVal = app.exec();

   scanner->m_pluginDir.saveXml( "data/plugin_db5.xml" );
   scanner->m_pluginDir.loadXml( "data/plugin_db5.xml" );
   scanner->m_pluginDir.saveXml( "data/plugin_db6.xml" );

   // auto pluginList = de::audio::getPluginInfoList( L"G:/iso/2_dev/_vst_x64", true );
   // std::wcout << pluginList.toWString() << std::endl;

   // pluginList.saveXml( "data/plugin_db.xml" );
   // pluginList.loadXml( "data/plugin_db.xml" );
   // pluginList.saveXml( "data/plugin_db2.xml" );

   delete scanner;

   return retVal;
}
