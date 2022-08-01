/// (c) 2017 - 20180 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef DE_QHEXEDIT_HPP
#define DE_QHEXEDIT_HPP

#include <sstream>
#include <fstream>
#include <chrono>
#include <cmath>

#include <QMainWindow>
#include <QScreen>
#include <QWidget>
#include <QDesktopWidget>

#include <QWidget>
#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QDial>
#include <QDebug>
#include <QThread>

#include <QWidget>
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QFileInfo>
#include <QScrollArea>

#include <DarkImage.hpp>
#include <QImageWidget.hpp>
#include <QFont5x8.hpp>

inline bool
loadBinary( std::string const & uri, std::vector< uint8_t > & bin )
{
   FILE* file = ::fopen64( uri.c_str(), "rb" );
   if ( !file )
   {
      return false;
   }

   ::fseeko64( file, 0, SEEK_END );
   auto siz = ::ftello64( file );
   ::fseeko64( file, 0, SEEK_SET );

   if ( siz < 1 )
   {
      return false;
   }

   bin.resize( siz );
   ::fread( bin.data(), 1, bin.size(), file );
   ::fclose( file );
   return true;
}

class QHexViewer : public QWidget
{
   Q_OBJECT

public:
   QHexViewer( QWidget* parent );
   ~QHexViewer() override;

public slots:
   bool openFile();
   bool openFile( QString uri );
   void closeFile();

protected:
   void resizeEvent( QResizeEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;

   DE_CREATE_LOGGER("QHexViewer")
   //QImageWidget* m_img;
   int m_lineStart;
   int m_lineCount;
   QImage m_img;
   QString m_uri;
   QFont5x8 m_font;
   std::vector< uint8_t > m_bytes;
};

#endif
