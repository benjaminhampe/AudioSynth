#include "PluginScanner.hpp"

#include <QFileInfo>
#include <QDirIterator>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>

// ===========================================================
PluginScanner::PluginScanner( QWidget* parent )
// ===========================================================
   : QWidget( parent )
   , m_isDisplayDirty( true )
{
   setObjectName( "PluginScanner" );
   setContentsMargins( 0,0,0,0 );

   m_lblPluginDir = new QLabel( "PluginDir", this );
   m_lblPlugin = new QLabel( "Hello Plugin Scanner", this );

   m_lblPluginDir->setStyleSheet("font-size:10pt; color:white; background-color:none;");
   m_lblPlugin->setStyleSheet("font-size:10pt; color:white; background-color:none;");

   auto v = new QVBoxLayout();
   v->setContentsMargins( 20,10,10,10 );
   v->addWidget( m_lblPluginDir );
   v->addStretch( 1 );
   v->addWidget( m_lblPlugin );
   setLayout( v );

   m_imgSource.load( "../../media/abenton.png");
   setMinimumSize( m_imgSource.size() );
   setMaximumSize( m_imgSource.size() );
   //updateImage();
}

PluginScanner::~PluginScanner()
{}

void
PluginScanner::scanPluginDir( std::wstring vstDir, bool searchSubFolders )
{
   m_lblPluginDir->setText( QString::fromStdWString( vstDir ) );

   auto qtdir = QString::fromStdWString( vstDir );
   QDirIterator dirIt( qtdir, searchSubFolders ?
                              QDirIterator::Subdirectories :
                              QDirIterator::NoIteratorFlags );

   m_pluginDir.m_vstDir = vstDir;
   m_pluginDir.m_vstDirMB = qtdir.toStdString();

   int nCpu = QThread::idealThreadCount();
   DE_DEBUG("nCpu (ideal thread count) = ", nCpu)
   m_threadPool.setMaxThreadCount( nCpu );

   //std::vector< std::wstring > uris;
   //uris.reserve( 4000 );

   while ( dirIt.hasNext() )
   {
      QString elem = dirIt.next();
      QFileInfo fi = dirIt.fileInfo();
      if ( fi.isFile() )
      {
         auto ext = fi.suffix().toLower();
         if ( ext == "dll" )
         {
            try {
               std::wstring uri = elem.toStdWString();
               PluginScannerRunnable* runnable = new PluginScannerRunnable( this, uri );
               m_threadPool.start( runnable );

               //uris.emplace_back( std::move( uri ) );
            }
            catch (...)
            {
            }

         }
      }
   }

   //DE_DEBUG("nDLLs = ", uris.size() )

//   for ( size_t i = 0; i < uris.size(); ++i )
//   {
//      std::wstring const & uri = uris[ i ];
//      DE_DEBUG("DLL[",i,"] = ", dbStr(uri) )
//      PluginScannerRunnable* runnable = new PluginScannerRunnable( this, uri );
//      m_threadPool.start( runnable );
//   }

   //m_lblPluginDir->setText( QString("Finished - ") + QString::fromStdWString( vstDir ) );
}

// Called by runnable(s) to add new plugins found in current dir
void
PluginScanner::addPluginInfo( de::audio::PluginInfo pi )
{
   QString s;
   size_t k = 0;

   {
      // Threadsafe needle ear for all runnables to add items to db/dir.m_list.
      std::lock_guard< std::mutex > lg( m_pluginDirMutex );
      // Replace current shown label text ( visual feedback to user indicating working )

      s = QString::fromStdWString( pi.m_name );

      // Add item to db/dir.m_list
      m_pluginDir.m_list.emplace_back( std::move( pi ) );
      k = m_pluginDir.m_list.size();

   }

   if ( m_threadPool.activeThreadCount() < 2 )
   {
      m_lblPlugin->setText( QString("Finished Scan :: (%1) %2").arg(k).arg(s) );
   }
   else
   {
      m_lblPlugin->setText( QString("(%1) %2").arg(k).arg(s) );
   }
}

// Called by runnable(s) to add new plugins found in current dir
void
PluginScanner::addBadPluginInfo( de::audio::PluginInfo pi )
{
//   // Threadsafe needle ear for all runnables to add items to db/dir.m_list.
//   std::lock_guard< std::mutex > lg( m_pluginDirMutex );
//   // Replace current shown label text ( visual feedback to user indicating working )
//   m_lblPlugin->setText( QString::fromStdWString( pi.m_name ) );
//   // Add item to db/dir.m_list
//   m_pluginDir.m_list.emplace_back( std::move( pi ) );
}

/*
void
PluginScanner::setLabelText( QString const & text )
{
   m_lblPlugin->setText( text );
   update();
}

void setRect( QRect const & rect )
{
   move( rect.topLeft() );
   setMinimumSize( rect.size() );
   setMaximumSize( rect.size() );
}*/

void
PluginScanner::setImage( QImage const & img, bool useAsMinSize )
{
   m_imgSource = img;
   int w = m_imgSource.width();
   int h = m_imgSource.height();
   if ( w < 1 || h < 1 )
   {
      setVisible( false );
   }
   else
   {
      setVisible( true );
      if ( useAsMinSize )
      {
         setMinimumSize( w,h );
      }
   }
}

void
PluginScanner::updateImage()
{
   if ( !m_isDisplayDirty ) return;

   if ( m_imgSource.isNull() )
   {
      m_imgDisplay = QImage();
   }
   else
   {
      int w = width();
      int h = height();
      // if ( m_imgScaling )
      // {
         // if ( m_imgPreserveAspectWoH )
         // {
            // m_imgDisplay = m_imgSource.scaled( w,h, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation );
         // }
         // else
         // {
            m_imgDisplay = m_imgSource.scaled( w,h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
         // }
      // }
      // else
      // {
         // m_imgDisplay = m_imgSource;
      // }
   }

   m_isDisplayDirty = false;
}

void PluginScanner::resizeEvent( QResizeEvent* event )
{
   QWidget::resizeEvent( event );
   m_isDisplayDirty = true;
   update();
}

void PluginScanner::paintEvent( QPaintEvent* event )
{
   updateImage();

   int w = width();
   int h = height();

   if ( w > 1 && h > 1 )
   {
      int img_w = m_imgDisplay.width();
      int img_h = m_imgDisplay.height();
      if ( img_w > 0 && img_h > 0 )
      {
         QPainter dc( this );
         dc.setRenderHint( QPainter::NonCosmeticDefaultPen );
         //dc.setCompositionMode( QPainter::CompositionMode_SourceOver );
         int x = (w - img_w)/2;
         int y = (h - img_h)/2;
         dc.drawImage( x,y, m_imgDisplay );
      }
   }

   QWidget::paintEvent( event );
}


void
PluginScanner::enterEvent( QEvent* event )
{
   QWidget::enterEvent( event );
}

void
PluginScanner::leaveEvent( QEvent* event )
{
   QWidget::leaveEvent( event );
}

//void
//PluginScanner::focusInEvent( QFocusEvent* event )
//{
//   m_trackInfo.m_hasFocus = true;
//   update();
//   QWidget::focusInEvent( event );
//}

//void
//PluginScanner::focusOutEvent( QFocusEvent* event )
//{
//   m_trackInfo.m_hasFocus = true;
//   update();
//   QWidget::focusOutEvent( event );
//}
