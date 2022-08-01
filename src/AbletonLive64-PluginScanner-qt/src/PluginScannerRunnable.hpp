#ifndef DE_LIVE64_SCAN_PLUGININFO_RUNNABLE_HPP
#define DE_LIVE64_SCAN_PLUGININFO_RUNNABLE_HPP

#include "PluginInfo.hpp"

#include <QFileInfo>
#include <QDirIterator>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QThread>
#include <QThreadPool>
#include <QFuture>

struct PluginScanner;

// ===========================================================
struct PluginScannerRunnable : public QRunnable
// ===========================================================
{
   DE_CREATE_LOGGER("PluginScannerRunnable")

   PluginScanner* m_scanner;
   std::wstring m_uri;

   PluginScannerRunnable( PluginScanner* scanner, std::wstring const & uri );
   ~PluginScannerRunnable();
   void run();


};


#endif // G_LOPASS1_HPP
