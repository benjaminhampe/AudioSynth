#include "PluginList.hpp"

PluginList::PluginList( App & app, QWidget* parent )
   : QWidget( parent )
   , m_app( app )
   , m_selectedItem( nullptr )
   , m_isDragging( false )
{
   setObjectName( "PluginList" );
   setContentsMargins(0,0,0,0);
   setAcceptDrops( true );
   setMouseTracking( true );

   m_pluginTree = new PluginTree( m_app, this );

   auto v = createVBox();
   v->setContentsMargins(10,30,10,20);
   v->addWidget( m_pluginTree );
   setLayout( v );
}

PluginList::~PluginList()
{
}

void
PluginList::on_itemActivated( QTreeWidgetItem* item, int col )
{
   std::cout << "on_itemActivated " << col << std::endl;
//   m_selectedItem = item;
}


void
PluginList::mousePressEvent( QMouseEvent* event )
{
//   m_isDragging = true;
//   m_dragStartX = event->screenPos().x();
//   m_dragStartY = event->screenPos().y();
//   std::cout << "mousePressEvent" << std::endl;
//   update();
   QWidget::mousePressEvent( event );
}

void
PluginList::mouseReleaseEvent( QMouseEvent* event )
{
//   m_isDragging = false;
//   std::cout << "mouseReleaseEvent" << std::endl;
//   update();
   QWidget::mouseReleaseEvent( event );
}

void
PluginList::mouseMoveEvent( QMouseEvent* event )
{
//   std::cout << "mouseMoveEvent" << std::endl;
//   if ( m_isDragging && m_selectedItem )
//   {
//      std::cout << "Dragging" << std::endl;
//      int dx = event->screenPos().x() - m_dragStartX;
//      int dy = event->screenPos().y() - m_dragStartY;
//      float len = sqrtf( float( (dx*dx) + (dy*dy) ) );
//      if ( len > QApplication::startDragDistance() )
//      {
//         QDrag* drag = new QDrag( this );
//         QMimeData *mime = new QMimeData;
//         mime->setText( m_selectedItem->data(0,Qt::UserRole).toString() );

//         drag->setMimeData( mime );

//         QPixmap pixmap(34, 34);
//         pixmap.fill( Qt::blue );
//         /*
//         QPainter dc( &pixmap );
//         dc.setRenderHint(QPainter::Antialiasing);
//         dc.translate(15, 15);
//         paint( &dc, 0, 0 );
//         dc.end();

//         pixmap.setMask(pixmap.createHeuristicMask());
//         */

//         drag->setPixmap( pixmap );
//         drag->setHotSpot( QPoint( 15, 20 ) );


//         drag->exec( Qt::MoveAction | Qt::CopyAction );

//         update();
//      }

//   }
//   m_mouseX = event->x();
//   m_mouseY = event->y();

   QWidget::mouseMoveEvent( event );
}

void
PluginList::wheelEvent( QWheelEvent* event )
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

   QWidget::wheelEvent( event );
}

//void
//PluginList::resizeEvent( QResizeEvent* event )
//{
//   update();
//   QWidget::resizeEvent( event );
//}

void
PluginList::paintEvent( QPaintEvent* event )
{
   int w = width();
   int h = height();

   if ( w > 1 && h > 1 )
   {
      LiveSkin const & skin = m_app.m_skin;
      auto const & pluginDb = m_app.m_pluginDb;

      QPainter dc( this );
      dc.setRenderHint( QPainter::NonCosmeticDefaultPen );

      // Draw background
      dc.fillRect( rect(), skin.contentColor );

      // Draw title rect
      dc.fillRect( QRect(0, 0, w-1, skin.searchBarH ), skin.titleColor );

      // Draw table header
      dc.fillRect( QRect(0, skin.searchBarH, w-1, skin.tdH), skin.tdColor );

      QFont searchBarFont( "Arial Black", 10, 900, true );
      dc.setFont( searchBarFont );
      auto msg = QString::fromStdString( pluginDb.m_vstDirMB );
//      auto ts = dc.fontMetrics().tightBoundingRect( msg ).size();
//      int x = ( w - ts.width() ) / 2;
//      int y = ( h + ts.height() ) / 2;
//      dc.drawText( x,y,msg );
      dc.drawText( 10,15,msg );
   }

   QWidget::paintEvent( event );
}
