#ifndef DE_GPU_S3DVERTEX_QCUSTOMTABLE_HPP
#define DE_GPU_S3DVERTEX_QCUSTOMTABLE_HPP

#include <QWidget>
#include <QImage>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QKeyEvent>

#include <DarkGPU.hpp>
#include <QFont5x8.hpp>

// =======================================================================
inline bool
isMouseOver( int mx, int my, QRect const & pos )
{
   int x1 = pos.x();
   int y1 = pos.y();
   int x2 = pos.x() + pos.width() - 1;
   int y2 = pos.y() + pos.height() - 1;
   if ( x1 > x2 ) std::swap( x1, x2 );
   if ( y1 > y2 ) std::swap( y1, y2 );
   if ( mx < x1 ) return false;
   if ( my < y1 ) return false;
   if ( mx > x2 ) return false;
   if ( my > y2 ) return false;
   return true;
}

// =======================================================================
// Qt is more BGRA than me RGBA.
inline QColor
toQColor( uint32_t color )
{
   return QColor( de::RGBA( de::RGBA_B(color),
                            de::RGBA_G(color),
                            de::RGBA_R(color),
                            de::RGBA_A(color) ));
}

inline void
drawLine( QPainter & dc, int x1, int y1, int x2, int y2, uint32_t color )
{
   dc.setBrush( Qt::NoBrush );
   dc.setPen( QPen( toQColor(color), 1 ) ); // grey border
   dc.drawLine( x1, y1, x2, y2 );
}

inline void
drawRect( QPainter & dc, QRect pos, uint32_t fillColor, uint32_t borderColor = 0x00000000 )
{
   if ( de::RGBA_A( fillColor ) > 0 )
   {
      dc.setBrush( QBrush( toQColor(fillColor) ) );
      dc.setPen( Qt::NoPen );
      dc.drawRect( pos );
   }

   if ( de::RGBA_A( borderColor ) > 0 )
   {
      dc.setBrush( Qt::NoBrush );
      dc.setPen( QPen( toQColor(borderColor), 1 ) );
      dc.drawRect( pos );
   }
}

struct S3DVertexTable : public QWidget
{
   Q_OBJECT
public:
   struct Column
   {
      QString name;
      int width = 0;
      de::Align::EAlign align;
      int proportion = 0;
      int min_width = 0;
   };

   S3DVertexTable( QWidget* parent );
   ~S3DVertexTable() override;

   int computeVisibleRowCount() const
   {
      if ( m_LineHeight < 2 ) return 0;
      return 1 + ((rect().height() - 1 - m_HeaderHeight) / m_LineHeight);
   }

   int computeLineNumColWidth() const
   {
      return std::max( 33, 10 + m_Font.getTextSize( QString::number( m_RowStart + m_RowCount - 1 ) ).x ); // # chars in str(val)
   }

   int computeTableWidth() const
   {
      int w = 0;
      if ( m_Data )
      {
         w = computeLineNumColWidth();
      }

      for ( int i = 0; i < m_Cols.size(); ++i )
      {
         w += m_Cols[ i ].width;
      }
      return w;
   }

   int computeTableMinWidth() const
   {
      int w = 0;
      if ( m_Data )
      {
         w = computeLineNumColWidth();
      }

      for ( int i = 0; i < m_Cols.size(); ++i )
      {
         w += m_Cols[ i ].min_width;
      }
      return w;
   }

   void* getData( int xcol, int yrow );
   void setRowStart( int start );
   int getRowStart() const;

   bool computeHoveredCell( QRect & hoverRect, int & col, int & row );

signals:
   void canvasResized();

public slots:
   void setData( de::gpu::SMeshBuffer* ptr );

   void setColumnCount( int cols )
   {
      m_Cols.resize( cols );
   }

   void setColumnHeader( int i, QString text, int width, int proportion = 0 )
   {
      if ( i + 1 > m_Cols.size() )
      {
         m_Cols.resize( i + 1 );
      }
      m_Cols[ i ].width = width;
      m_Cols[ i ].min_width = width;
      m_Cols[ i ].proportion = proportion;
      m_Cols[ i ].name = text;
   }

   void setRowCount( int rows )
   {
      m_RowCount = rows;
   }

   void startUpdateTimer( int ms = 650 );
   void stopUpdateTimer();

protected:
   void focusInEvent( QFocusEvent* event) override;
   void focusOutEvent( QFocusEvent* event) override;
   void enterEvent( QEvent* event) override;
   void leaveEvent( QEvent* event) override;

   void timerEvent( QTimerEvent* event) override;
   void paintEvent( QPaintEvent* event ) override;
   void resizeEvent( QResizeEvent* event ) override;
   void keyPressEvent( QKeyEvent* event ) override;
   void keyReleaseEvent( QKeyEvent* event ) override;
   void mousePressEvent( QMouseEvent* event ) override;
   void mouseReleaseEvent( QMouseEvent* event ) override;
   void mouseMoveEvent( QMouseEvent* event ) override;
   void wheelEvent( QWheelEvent* event ) override;

protected:
   DE_CREATE_LOGGER("S3DVertexTable")

   std::vector< de::gpu::S3DVertex >* m_Data;

   QFont5x8 m_Font;
   QFont5x8 m_FontCaption;
   QFont5x8 m_FontNoData;

   bool m_IsFocused;
   bool m_IsHovered;

   int m_LineNumColWidth;
   int m_HeaderHeight;
   int m_LineHeight;
   //QRect m_TitleRect;
   //QRect m_NumRect;
   //QRect m_HeaderRect;
   //QRect m_BodyRect;
   //QRect m_DataRect;
   int m_HoverCol;
   int m_HoverRow;
   QRect m_HoverRect;

   QImage m_ViewImg;

   int m_TimerId;

   int m_RowStart;
   int m_RowCount;
   int m_SelectStart;
   int m_SelectCount;

   std::vector< Column > m_Cols;
};

#endif
