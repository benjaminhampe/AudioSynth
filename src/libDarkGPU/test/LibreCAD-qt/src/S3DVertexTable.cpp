#include "S3DVertexTable.hpp"
#include <QPainter>
#include <QDebug>

S3DVertexTable::S3DVertexTable( QWidget* parent )
   : QWidget( parent ) // , Qt::NoDropShadowWindowHint
   , m_Data( nullptr )
   , m_TimerId( 0 )
   , m_RowStart( 0 )
   , m_RowCount( 10 )
   , m_SelectStart( 0 )
   , m_SelectCount( 0 )
   , m_HeaderHeight( 0 )
   , m_HoverCol( -1 )
   , m_HoverRow( -1 )
   , m_IsFocused( false )
   , m_IsHovered( false )
{
   m_Font.quadWidth = 1;
   m_Font.quadHeight = 1;
   m_Font.quadSpacingX = 0;
   m_Font.quadSpacingY = 0;
   m_Font.glyphSpacingX = 1;
   m_Font.glyphSpacingY = 1;

   m_FontCaption.quadWidth = 1;
   m_FontCaption.quadHeight = 1;
   m_FontCaption.quadSpacingX = 0;
   m_FontCaption.quadSpacingY = 0;
   m_FontCaption.glyphSpacingX = 1;
   m_FontCaption.glyphSpacingY = 1;

   m_FontNoData.quadWidth = 6;
   m_FontNoData.quadHeight = 6;
   m_FontNoData.quadSpacingX = 1;
   m_FontNoData.quadSpacingY = 1;
   m_FontNoData.glyphSpacingX = 5;
   m_FontNoData.glyphSpacingY = 5;

   m_LineNumColWidth = computeLineNumColWidth();
   m_HeaderHeight = std::max( 18, m_FontCaption.getTextSize("W_").y + 6 );
   m_LineHeight = m_Font.getTextSize("W_").y + 6;

   //setBackgroundRole( QPalette::NoRole );
   setFocusPolicy( Qt::StrongFocus );
   setMouseTracking( true );
   //setMinimumSize( 160, 100 );
   setColumnCount( 9 );
   setColumnHeader(0,tr("X"),40,1);
   setColumnHeader(1,tr("Y"),40,1);
   setColumnHeader(2,tr("Z"),40,1);
   setColumnHeader(3,tr("NX"),30,1);
   setColumnHeader(4,tr("NY"),30,1);
   setColumnHeader(5,tr("NZ"),30,1);
   setColumnHeader(6,tr("RGBA"),52,0);
   setColumnHeader(7,tr("U"),30,1);
   setColumnHeader(8,tr("V"),30,1);
}

S3DVertexTable::~S3DVertexTable()
{

}

void
S3DVertexTable::paintEvent( QPaintEvent* event )
{
   QRect r = rect();
   int w = r.width();
   int h = r.height();
   if ( w < 2 || h < 2 ) return;

   QPainter dc( this );
   dc.setRenderHint( QPainter::NonCosmeticDefaultPen );

   auto drawCaptionRect = [&] ( QRect pos )
   {
      int x = pos.x();
      int y = pos.y();
      int w = pos.width();
      int h = pos.height();
      drawRect( dc, pos, 0xFFC8D0D4 );
      drawLine( dc, x,     y,     x+w-2, y,     0xFFFFFFFF );
      drawLine( dc, x,     y,     x,     y+h-2, 0xFFFFFFFF );
      drawLine( dc, x+w-1, y,     x+w-1, y+h-1, 0xFF000000 );
      drawLine( dc, x,     y+h-1, x+w-2, y+h-1, 0xFF000000 );
      drawLine( dc, x+w-2, y+1,   x+w-2, y+h-2, 0xFF64686A );
      drawLine( dc, x+1,   y+h-2, x+w-2, y+h-2, 0xFF64686A );
   };

   auto drawCellRect = [&] ( QRect pos )
   {
      int x = pos.x();
      int y = pos.y();
      int w = pos.width();
      int h = pos.height();
      drawRect( dc, pos, 0xFFFFFFFF );
      drawLine( dc, x+w-1, y,     x+w-1, y+h-1, 0xFFA4A0A0 );
      drawLine( dc, x,     y+h-1, x+w-2, y+h-1, 0xFFA4A0A0 );
   };

   // TitleRect
   QRect m_TitleRect( 2, 2, m_LineNumColWidth, m_HeaderHeight );
   drawRect( dc, m_TitleRect, 0xFFC8D0D4, 0x00000000); // panel grey
//   m_FontCaption.drawText( dc,
//         m_TitleRect.center().x()+2,
//         m_TitleRect.center().y()+2, "S3DVtx", 0xFF000000, de::Align::Centered );

   int x = 2, y = 2;

   if ( m_Data )
   {
      x += m_LineNumColWidth;
   }

   for ( int i = 0; i < m_Cols.size(); ++i )
   {
      auto & col = m_Cols[ i ];
      auto width = col.width;
      //auto align = col.align;

      dc.setBrush( QBrush( toQColor(0xFFC8D0D4) ) ); // panel grey
      dc.setPen( QPen( toQColor(0xFFFFFFFF), 1 ) ); // white border
      QRect r_caption( x, y, width, m_HeaderHeight );
      drawCaptionRect( r_caption );

      m_FontCaption.drawText( dc,
            r_caption.center().x(),
            r_caption.center().y()+1, col.name, 0xFF000000, de::Align::Centered );
      x += width;
   }
   y += m_HeaderHeight;

   if ( m_Data )
   {
      for ( int j = 0; j < m_RowCount; ++j )
      {
         int lineNum = m_RowStart + j;
         if ( lineNum >= m_Data->size() ) continue;

         x = 2;
         QRect r_linenum( x,y, m_LineNumColWidth, m_LineHeight );
         drawCaptionRect( r_linenum );

         m_Font.drawText( dc,
            r_linenum.x()+5,
            r_linenum.center().y()+1, QString::number(lineNum), 0xFF000000, de::Align::LeftMiddle );

         x += m_LineNumColWidth;

         for ( int i = 0; i < m_Cols.size(); ++i )
         {
            auto & col = m_Cols[ i ];
            auto width = col.width;
            //auto align = col.align;
            QRect r_cell( x, y, width, m_LineHeight );
            drawCellRect( r_cell );
            int tx = x+3;
            int ty = y+3;

   //         m_Font.drawText( dc, tx, ty, QString::number( 0.0 ), 0xFF000000 );

            void* valuePtr = getData( i, j );
            if ( valuePtr )
            {
               if ( i != 6 )
               {
                  float value = *reinterpret_cast< float* >( valuePtr );
                  m_Font.drawText( dc, tx, ty, QString::number( value ), 0xFF000000 );
               }
               else
               {
                  uint32_t value = *reinterpret_cast< uint32_t* >( valuePtr );
                  m_Font.drawText( dc, tx, ty, QString::number( value,16 ).toUpper(), 0xFF000000 );
               }
            }

            x += width;
         }
         y += m_LineHeight;
      }

   }
   else
   {
      x = 2;
      QRect r_white( x, y, r.width()-4, r.height()-2-y );
      drawRect( dc, r_white, 0xFFFFFFFF );

      //auto rc = r_white.center();
      //m_FontNoData.drawText( dc, rc.x(), rc.y(), "No Data", 0xFF0000FF, de::Align::Centered );
   }

   drawRect( dc, QRect( 0, 0, w-1, h-1 ), 0x00000000, 0xFF64686A); // grey border
   drawRect( dc, QRect( 1, 1, w-3, h-3 ), 0x00000000, 0xFF000000); // black border

   if ( m_IsFocused )
   {
      drawRect( dc, QRect(r.x(),r.y(),r.width()-1,r.height()-1), 0x00000000, 0xFF0060CF );
   }
   if ( m_IsHovered )
   {
      drawRect( dc, QRect(r.x()+1,r.y()+1,r.width()-3,r.height()-3), 0x00000000, 0xFF00A0EF );
   }
}

void
S3DVertexTable::setData( de::gpu::SMeshBuffer* ptr )
{
   if ( !ptr ) { DE_ERROR("No mesh buffer") return; }
   auto dataPtr = &ptr->Vertices;
   m_Data = dataPtr;
   m_RowStart = 0;
   m_RowCount = std::min( computeVisibleRowCount(), int( dataPtr ? dataPtr->size() : 0) );
   m_LineNumColWidth = computeLineNumColWidth();
   update();
}

int
S3DVertexTable::getRowStart() const
{
   return m_RowStart;
}

void
S3DVertexTable::setRowStart( int start )
{
   if ( m_Data )
   {
      m_RowStart = start;

      if ( m_RowStart < 0 )
      {
         m_RowStart = 0;
      }

      if ( m_Data->size() > 0 && m_RowStart >= m_Data->size() )
      {
         m_RowStart = int(m_Data->size())-1;
      }

      if ( m_RowStart < 0 )
      {
         m_RowStart = 0;
      }

      m_LineNumColWidth = computeLineNumColWidth();

      update();
   }
}

void*
S3DVertexTable::getData( int xcol, int yrow )
{
   if ( !m_Data ) return nullptr;
   int i = yrow + m_RowStart;
   if ( i < 0 || i >= m_Data->size() ) return nullptr;
   de::gpu::S3DVertex & vtx = (*m_Data)[ i ];
        if ( xcol == 0 ) { return reinterpret_cast< void* >( &vtx.pos.x ); }
        if ( xcol == 1 ) { return &vtx.pos.y; }
   else if ( xcol == 2 ) { return &vtx.pos.z; }
   else if ( xcol == 3 ) { return &vtx.normal.x; }
   else if ( xcol == 4 ) { return &vtx.normal.y; }
   else if ( xcol == 5 ) { return &vtx.normal.z; }
   else if ( xcol == 6 ) { return &vtx.color; }
   else if ( xcol == 7 ) { return &vtx.tex.x; }
   else if ( xcol == 8 ) { return &vtx.tex.y; }
   else                  { return &vtx.pos.x; }
}

void
S3DVertexTable::startUpdateTimer( int ms )
{
   if ( ms < 1 ) { ms = 1; }
   if ( m_TimerId ) { killTimer( m_TimerId ); }
   m_TimerId = startTimer( ms, Qt::PreciseTimer );
}

void
S3DVertexTable::stopUpdateTimer()
{
   if ( m_TimerId )
   {
      killTimer( m_TimerId );
      m_TimerId = 0;
   }
}

void
S3DVertexTable::resizeEvent( QResizeEvent* event )
{
   int w = event->size().width();
   int h = event->size().height();
//   if ( m_ViewImg.width() < w || m_ViewImg.height() < h )
//   {
//      m_ViewImg = QImage( w, h );
//      m_ViewImg.fill( 0x00000000 );
//   }

   int minW = computeTableMinWidth();
   int extraW = int( float(w - minW) / float(m_Cols.size()) );

   for ( int i = 0; i < m_Cols.size(); ++i )
   {
      if ( extraW > 0 )
      {
         m_Cols[ i ].width = m_Cols[ i ].min_width + extraW;
      }
   }

   m_RowCount = std::min( computeVisibleRowCount(), int( m_Data ? m_Data->size() : 0) );
   m_LineNumColWidth = computeLineNumColWidth();
   update();
}

void
S3DVertexTable::timerEvent( QTimerEvent* event )
{
   if ( event->timerId() == m_TimerId )
   {
      update();
   }
}



void
S3DVertexTable::mouseMoveEvent( QMouseEvent* event )
{
   int mx = event->x();
   int my = event->y();
   DE_DEBUG("mx = ",mx,", my = ",my)
}

void
S3DVertexTable::mousePressEvent( QMouseEvent* event )
{
   //m_MousePos = glm::ivec2( event->x(), event->y() ); // current mouse pos
}

void
S3DVertexTable::mouseReleaseEvent( QMouseEvent* event )
{
   //m_MousePos = glm::ivec2( event->x(), event->y() ); // current mouse pos
   //m_MouseMove = { 0,0 };
}

void
S3DVertexTable::wheelEvent( QWheelEvent* event )
{
   // if ( m_Driver )
   // {
      // de::SEvent post;
      // post.type = de::EET_MOUSE_EVENT;
      // post.mouseEvent.m_Flags = de::SMouseEvent::Wheel;
      // post.mouseEvent.m_X = event->x();
      // post.mouseEvent.m_Y = event->y();
      // post.mouseEvent.m_Wheel = event->angleDelta().y(); //    //QPoint delta = event->pixelDelta();
      // // DE_DEBUG("post.mouseEvent.m_Wheel = ",post.mouseEvent.m_Wheel)
      // m_Driver->postEvent( post );
   // }

   if ( event->angleDelta().y() > 0)
   {
      int rowStart = getRowStart();
      rowStart -= (2*m_RowCount) / 3;
      setRowStart( rowStart );
      update();
   }
   else if ( event->angleDelta().y() < 0)
   {
      int rowStart = getRowStart();
      rowStart += (2*m_RowCount) / 3;
      setRowStart( rowStart );
      update();
   }

   event->accept();
}

void
S3DVertexTable::focusInEvent( QFocusEvent* event)
{
   m_IsFocused = true;
   update();
}

void
S3DVertexTable::focusOutEvent( QFocusEvent* event)
{
   m_IsFocused = false;
   update();
}

void
S3DVertexTable::enterEvent( QEvent* event)
{
   m_IsHovered = true;
   update();
}

void
S3DVertexTable::leaveEvent( QEvent* event)
{
   m_IsHovered = false;
   update();
}

void
S3DVertexTable::keyPressEvent( QKeyEvent* event )
{
   if ( event->key() == Qt::Key_End )
   {
      DE_DEBUG("Key_End")
      if ( m_Data )
      {
         setRowStart( int(m_Data->size()) - m_RowCount );
      }
      event->accept();
      return;
   }
   if ( event->key() == Qt::Key_PageUp )
   {
      int rowStart = getRowStart();
      rowStart -= (8*m_RowCount) / 9;
      setRowStart( rowStart );
   }
   if ( event->key() == Qt::Key_PageDown )
   {
      int rowStart = getRowStart();
      rowStart += (8*m_RowCount) / 9;
      setRowStart( rowStart );
   }

   QWidget::keyPressEvent( event );

}

void
S3DVertexTable::keyReleaseEvent( QKeyEvent* event )
{
//   if ( event->key() == Qt::Key_End )
//   {
//      DE_DEBUG("Key_End Release")
//      if ( m_Data )
//      {
//         setRowStart( int(m_Data->size()) - m_RowCount );
//      }
//      event->accept();
//      return;
//   }
//   if ( event->key() == Qt::Key_A )
//   {
//      DE_DEBUG("Key_A Release")
//      if ( m_Data )
//      {
//         setRowStart( int(m_Data->size()) - m_RowCount );
//      }
//      event->accept();
//      return;
//   }


   QWidget::keyPressEvent( event );
}
