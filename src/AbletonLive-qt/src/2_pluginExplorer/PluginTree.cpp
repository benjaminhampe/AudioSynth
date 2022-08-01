#include "PluginTree.hpp"

PluginTree::PluginTree( App & app, QWidget* parent )
   : QTreeWidget( parent )
   , m_app( app )
   , m_selectedItem( nullptr )
   , m_isDragging( false )
{
   m_pixmapVST.load( "../../media-live/1_engine/vsti.png" );
   m_icoVST = QIcon( m_pixmapVST );

   setObjectName( "PluginTree" );
   setContentsMargins(0,0,0,0);
   //setAcceptDrops( true );
   setMouseTracking( true );

   auto header = headerItem();
   int k = 0;
   header->setText( k, "Name" ); k++;
   header->setText( k, "Edit" ); k++;
   header->setText( k, "Banks" ); k++;
   header->setText( k, "Params" ); k++;
   header->setText( k, "Out" ); k++;
   header->setText( k, "In" ); k++;
   header->setText( k, "f32" ); k++;
   header->setText( k, "f64" ); k++;
   header->setText( k, "entry" ); k++;
   header->setText( k, "uri" ); k++;

   k = 0;
   setColumnWidth( k, 150 ); k++;
   setColumnWidth( k, 20 ); k++;
   setColumnWidth( k, 20 ); k++;
   setColumnWidth( k, 20 ); k++;
   setColumnWidth( k, 20 ); k++;
   setColumnWidth( k, 20 ); k++;
   setColumnWidth( k, 20 ); k++;
   setColumnWidth( k, 20 ); k++;
   setColumnWidth( k, 100 ); k++;
   setColumnWidth( k, 450 ); k++;

   m_synthNode = new QTreeWidgetItem( this );
   m_synthNode->setText( 0, "Synths" );
   m_synthNode->setIcon( 0, m_icoVST );

   m_effectNode = new QTreeWidgetItem( this );
   m_effectNode->setText( 0, "Effects" );
   m_effectNode->setIcon( 0, m_icoVST );
//   connect( this, SIGNAL(itemActivated(QTreeWidgetItem*,int)),
//         this, SLOT(on_itemActivated(QTreeWidgetItem*,int)) );

   connect( this, SIGNAL(itemPressed(QTreeWidgetItem*,int)),
         this, SLOT(on_itemActivated(QTreeWidgetItem*,int)) );

   populateFromPluginDatabase();
}

PluginTree::~PluginTree()
{
}

void
PluginTree::populateFromPluginDatabase()
{
   auto const & pluginDb = m_app.m_pluginDb;

   clear();

   auto header = headerItem();
   int k = 0;
   header->setText( k, "Name" ); k++;
   header->setText( k, "Edit" ); k++;
   header->setText( k, "Banks" ); k++;
   header->setText( k, "Params" ); k++;
   header->setText( k, "Out" ); k++;
   header->setText( k, "In" ); k++;
   header->setText( k, "f32" ); k++;
   header->setText( k, "f64" ); k++;
   header->setText( k, "entry" ); k++;
   header->setText( k, "uri" ); k++;

   k = 0;
   setColumnWidth( k, 150 ); k++;
   setColumnWidth( k, 20 ); k++;
   setColumnWidth( k, 20 ); k++;
   setColumnWidth( k, 20 ); k++;
   setColumnWidth( k, 20 ); k++;
   setColumnWidth( k, 20 ); k++;
   setColumnWidth( k, 20 ); k++;
   setColumnWidth( k, 20 ); k++;
   setColumnWidth( k, 100 ); k++;
   setColumnWidth( k, 450 ); k++;

   m_synthNode = new QTreeWidgetItem( this );
   m_synthNode->setText( 0, "Synths" );
   m_synthNode->setIcon( 0, m_icoVST );

   m_effectNode = new QTreeWidgetItem( this );
   m_effectNode->setText( 0, "Effects" );
   m_effectNode->setIcon( 0, m_icoVST );

   addTopLevelItem( m_synthNode );
   addTopLevelItem( m_effectNode );
   for ( int i = 0; i < pluginDb.size(); ++i )
   {
      de::audio::PluginInfo const & pluginInfo = pluginDb[ i ];

      QTreeWidgetItem* parent = m_effectNode;
      if ( pluginInfo.m_isSynth )
      {
         parent = m_synthNode;
      }

      auto pin = new QTreeWidgetItem( parent );
      int k = 0;
      pin->setIcon( 0, m_icoVST );
      pin->setData( 0, Qt::UserRole, QString::fromStdWString( pluginInfo.m_uri ) );
      pin->setText( k, QString::fromStdWString( pluginInfo.m_name ) ); k++;
      //pin->setText( k, QString::number( pluginInfo.m_isSynth ) ); k++;
      pin->setText( k, QString::number( pluginInfo.m_hasEditor ) ); k++;
      pin->setText( k, QString::number( pluginInfo.m_numPrograms ) ); k++;
      pin->setText( k, QString::number( pluginInfo.m_numParams ) ); k++;
      pin->setText( k, QString::number( pluginInfo.m_numOutputs ) ); k++;
      pin->setText( k, QString::number( pluginInfo.m_numInputs ) ); k++;
      pin->setText( k, QString::number( pluginInfo.m_can32Bit ) ); k++;
      pin->setText( k, QString::number( pluginInfo.m_can64Bit ) ); k++;
      pin->setText( k, QString::fromStdString( pluginInfo.m_entry ) ); k++;
      pin->setText( k, QString::fromStdWString( pluginInfo.m_uri ) ); k++;
      //parent->addChild( pin );
   }
}


void
PluginTree::on_itemActivated( QTreeWidgetItem* item, int col )
{
   std::cout << "on_itemActivated " << col << std::endl;
   //m_selectedItem = item;
}

void
PluginTree::mousePressEvent( QMouseEvent* event )
{
   QTreeWidget::mousePressEvent( event );

   int x = event->x();
   int y = event->y();
   auto selectedItem = itemAt(x,y) ;
   if ( m_isDragging )
   {
      if ( selectedItem != m_selectedItem )
      {
         m_isDragging = false;
      }
   }
   else
   {
      if ( selectedItem != m_synthNode  // dont Drag synth root
          && selectedItem != m_effectNode ) // dont Drag effect root
      m_isDragging = true;
      m_dragStartX = x;
      m_dragStartY = y;
   }

   m_selectedItem = selectedItem;
   //std::cout << "mousePressEvent" << std::endl;
   update();
}

void
PluginTree::mouseReleaseEvent( QMouseEvent* event )
{
   QTreeWidget::mouseReleaseEvent( event );

   m_isDragging = false;
   //std::cout << "mouseReleaseEvent" << std::endl;
   update();
}

void
PluginTree::mouseMoveEvent( QMouseEvent* event )
{
   QTreeWidget::mouseMoveEvent( event );

   //std::cout << "mouseMoveEvent" << std::endl;
   if ( m_isDragging && m_selectedItem )
   {
      std::cout << "Dragging" << std::endl;
      int dx = event->x() - m_dragStartX;
      int dy = event->y() - m_dragStartY;
      float len = sqrtf( float( (dx*dx) + (dy*dy) ) );
      if ( len > 13.5f )
      {
         QDrag* drag = new QDrag( this );
         QMimeData *mime = new QMimeData;
         mime->setText( m_selectedItem->data(0,Qt::UserRole).toString() );

         drag->setMimeData( mime );

         //QPixmap pixmap(34, 34);
         //pixmap.fill( Qt::blue );
         /*
         QPainter dc( &pixmap );
         dc.setRenderHint(QPainter::Antialiasing);
         dc.translate(15, 15);
         paint( &dc, 0, 0 );
         dc.end();

         pixmap.setMask(pixmap.createHeuristicMask());
         */

         drag->setPixmap( m_pixmapVST );
         drag->setHotSpot( QPoint( 15, 20 ) );


         drag->exec( Qt::MoveAction | Qt::CopyAction );

         update();
      }

   }
//   m_mouseX = event->x();
//   m_mouseY = event->y();
}

void
PluginTree::wheelEvent( QWheelEvent* event )
{
//   int wheel = event->angleDelta().y();
//   DE_DEBUG("MouseWheel = ",wheel )

//   if ( wheel >= 1 )
//   {
//      m_keyStart = std::clamp( m_keyStart - 3, 0, 125 );
//      update();
//   }
//   else if ( wheel <= 1 )
//   {
//      m_keyStart = std::clamp( m_keyStart + 3, 0, 125 );
//      update();
//   }

   QTreeWidget::wheelEvent( event );
}

//void
//PluginTree::resizeEvent( QResizeEvent* event )
//{
//   update();
//   QWidget::resizeEvent( event );
//}

//void
//PluginTree::paintEvent( QPaintEvent* event )
//{
//   int w = width();
//   int h = height();

//   if ( w > 1 && h > 1 )
//   {
//      LiveSkin skin;

//      QPainter dc( this );
//      dc.setRenderHint( QPainter::NonCosmeticDefaultPen );

//      // Draw background
//      dc.fillRect( rect(), skin.contentColor );

//      // Draw title rect
//      dc.fillRect( QRect(0, 0, w-1, skin.titleH ), skin.titleColor );

//      // Draw table header
//      dc.fillRect( QRect(0, skin.titleH, w-1, skin.tdH), skin.tdColor );

//   }

//   QWidget::paintEvent( event );
//}
