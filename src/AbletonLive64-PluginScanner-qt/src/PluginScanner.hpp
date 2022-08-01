/// (c) 2017 - 20180 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef DE_LIVE64_PLUGIN_DIR_SCANNER_WORKER_DISPLAY_WINDOW_HPP
#define DE_LIVE64_PLUGIN_DIR_SCANNER_WORKER_DISPLAY_WINDOW_HPP

#include <iostream>
#include <QWidget>
#include <QLabel>
#include <QThread>
#include <QThreadPool>
#include <QFuture>



#include <de/Logger.hpp>

#include <sstream>
//#include "GetPluginInfo.hpp"
#include "PluginDir.hpp"
#include "PluginScannerRunnable.hpp"
#include <mutex>

// Shown to user while scanning for VST AudioPlugins
// Shows a big image covering the window
// Shows a label at bottom of window displaying currently scanned filename
// ===========================================================
class PluginScanner : public QWidget
// ===========================================================
{
   DE_CREATE_LOGGER("PluginScanner")
   Q_OBJECT
public:
   PluginScanner( QWidget* parent = nullptr );
   ~PluginScanner() override;

public slots:
   // Called by user to start a scan dir action
   void scanPluginDir( std::wstring vstDir, bool recursive = true );

   // Called by runnable(s) to add new plugins found in current dir
   void addPluginInfo( de::audio::PluginInfo pi );
   void addBadPluginInfo( de::audio::PluginInfo pi );

   // Called by runnable
   //void setLabelText( QString const & text );

   // Called by system ( app start )
   void setImage( QImage const & img, bool useAsMinSize = false );
   //void setRect( QRect const & rect )

protected:
   void updateImage();
   void resizeEvent( QResizeEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;
   void enterEvent( QEvent* event ) override;
   void leaveEvent( QEvent* event ) override;

protected:

public:
   QThreadPool m_threadPool;

   mutable std::mutex m_pluginDirMutex;
   de::audio::PluginDir m_pluginDir;

   QImage m_imgSource;
   QImage m_imgDisplay;
   bool m_isDisplayDirty;
   QLabel* m_lblPluginDir; // Current dir(ectory) name
   QLabel* m_lblPlugin;    // Current file name
};

#endif
