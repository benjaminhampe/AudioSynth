#include "PluginDirTree.hpp"
#include "App.hpp"

PluginDirTree::PluginDirTree( App & app, QWidget* parent )
   : QTreeWidget( parent )
   , m_app( app )
{
   setObjectName( "PluginDirTree" );
   setContentsMargins(0,0,0,0);
   //setAcceptDrops( true );
   setMouseTracking( true );

   createLiveTreeHeader();

//   connect( this, SIGNAL(itemActivated(QTreeWidgetItem*,int)),
//         this, SLOT(on_itemActivated(QTreeWidgetItem*,int)) );

   connect( this, SIGNAL(itemPressed(QTreeWidgetItem*,int)),
         this, SLOT(on_itemActivated(QTreeWidgetItem*,int)) );

   updateFromPluginDb();
}

void
PluginDirTree::createLiveTreeHeader()
{
   auto header = headerItem();
   int k = 0;
   header->setText( k, "Directory" ); k++;
//   header->setText( k, "Recursive" ); k++;
   header->setText( k, "Synths" ); k++;
   header->setText( k, "Effects" ); k++;

   header->setText( k, "Dirs" ); k++;
   header->setText( k, "Files" ); k++;
   header->setText( k, "Size" ); k++;
//   header->setText( k, "Used" ); k++;
//   header->setText( k, "Karat" ); k++;

   k = 0;
   setColumnWidth( k, 250 ); k++;
   //setColumnWidth( k, 20 ); k++;
   setColumnWidth( k, 20 ); k++;
   setColumnWidth( k, 20 ); k++;

   setColumnWidth( k, 20 ); k++;
   setColumnWidth( k, 20 ); k++;
   setColumnWidth( k, 20 ); k++;

//   setColumnWidth( k, 20 ); k++;
}
void
PluginDirTree::updateFromPluginDb()
{
   clear();
   createLiveTreeHeader();

   blockSignals( true );

   de::audio::PluginInfoDb & pluginDb = m_app.pluginDb();

   std::lock_guard< std::mutex > lg( pluginDb.getMutex() );

   for ( de::audio::PluginInfoDbDir const * const pi : pluginDb.m_dirs )
   {
      if ( !pi ) continue;
      de::audio::PluginInfoDbDir const & dir = *pi;

      auto item = new QTreeWidgetItem( this );
      int k = 0;
      item->setIcon( 0, m_app.m_effectIcon );

      item->setText( k, QString::fromStdWString( dir.m_uri ) );
      item->setData( k, Qt::UserRole, QString::fromStdWString( dir.m_uri ) );
      k++;
//      item->setText( k, QString::number( int( dir.m_recursive ) ) );
//      item->setData( k, Qt::UserRole, int( dir.m_recursive ) );
//      k++;
      item->setText( k, QString::number( int( dir.m_numSynths ) ) );
      item->setData( k, Qt::UserRole, int( dir.m_numSynths ) );
      k++;
      item->setText( k, QString::number( int( dir.m_numEffects ) ) );
      item->setData( k, Qt::UserRole, int( dir.m_numEffects ) );
      k++;
      item->setText( k, QString::number( int( dir.m_numDirs ) ) );
      item->setData( k, Qt::UserRole, int( dir.m_numDirs ) );
      k++;
      item->setText( k, QString::number( int( dir.m_numFiles ) ) );
      item->setData( k, Qt::UserRole, int( dir.m_numFiles ) );
      k++;
      item->setText( k, QString::number( qulonglong(dir.m_numBytes ) ) );
      item->setData( k, Qt::UserRole, qulonglong( dir.m_numBytes ) );
      k++;
   }

   blockSignals( false );
}


void
PluginDirTree::on_itemActivated( QTreeWidgetItem* item, int col )
{
   std::cout << "PluginDirTree::on_itemActivated " << col << std::endl;
   //m_selectedItem = item;
}

