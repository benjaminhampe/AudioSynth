#ifndef DE_LIVE1_ENGINE_BUTTON_HPP
#define DE_LIVE1_ENGINE_BUTTON_HPP

#include <Live0_Panel.hpp>


inline QImage
createRectImage( int w, int h, QColor fg, QColor bg,
             QImage const & symbol = QImage(), int symX = 0, int symY = 0 )
{
   QImage img( w, h, QImage::Format_ARGB32 );
   img.fill( fg );

   QPainter dc;
   if ( dc.begin( &img ) )
   {
      dc.setRenderHint( QPainter::NonCosmeticDefaultPen );
      dc.fillRect( QRect(1,1,w-2,h-2), bg );

      if ( !symbol.isNull() )
      {
         int x = symX + ( w - symbol.width() ) / 2;
         int y = symY + ( h - symbol.height() ) / 2;
         dc.drawImage( x, y, symbol );
      }

      dc.end();
   }

   return img;
}


inline QImage
createCircleImage( int w, int h, QColor groundColor, QColor shapeColor,
             QImage const & icon = QImage(), int icoX = 0, int icoY = 0 )
{
   QImage img( w, h, QImage::Format_ARGB32 );
   //img.fill( groundColor );
   img.fill( QColor(255,140,30) );

   QPainter dc;
   if ( dc.begin( &img ) )
   {
      dc.setRenderHint( QPainter::Antialiasing ); // NonCosmeticDefaultPen
      dc.setPen( Qt::NoPen );
      dc.setBrush( QBrush( shapeColor ) );
      dc.drawEllipse( QRect(0,0,w,h) );

      if ( !icon.isNull() )
      {
         int x = icoX + ( w - icon.width() ) / 2;
         int y = icoY + ( h - icon.height() ) / 2;
         dc.drawImage( x, y, icon );
      }

      dc.end();
   }

   return img;
}

// ============================================================================
class Button : public QPushButton
// ============================================================================
{
   Q_OBJECT
   bool m_isHovered;
   QImage m_imgIdle;
   QImage m_imgIdleHover;
   QImage m_imgActive;
   QImage m_imgActiveHover;

   //QImage m_imgSymbol;
   //int m_imgOffsetX;
   //int m_imgOffsetY;
public:
   //Button( QString msg, QWidget* parent = 0 );
   //Button( QImage sym, QWidget* parent = 0 );
   Button( QWidget* parent = 0 );
   ~Button();

signals:
public slots:
   //void setImageOffset( int x, int y ) { m_imgOffsetX = x; m_imgOffsetY = y; }
   void setImage( int i, QImage const & img );

protected:
   //void updateImage();
   //void resizeEvent( QResizeEvent* event ) override;
   void enterEvent( QEvent* event ) override { m_isHovered = true; QPushButton::enterEvent( event ); }
   void leaveEvent( QEvent* event ) override { m_isHovered = false; QPushButton::leaveEvent( event ); }
   void paintEvent( QPaintEvent* event ) override;

protected:
};

#endif // G_LOPASS1_HPP
