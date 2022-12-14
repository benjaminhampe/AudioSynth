#include "ImageButton.hpp"

void ImageButton::enterEvent( QEvent* event )
{
   m_isHovered = true;
   QPushButton::enterEvent( event );
}
void
ImageButton::leaveEvent( QEvent* event )
{
   m_isHovered = false;
   QPushButton::leaveEvent( event );
}

void ImageButton::resizeEvent( QResizeEvent* event )
{
   QPushButton::resizeEvent( event );
   update();
}

void ImageButton::paintEvent( QPaintEvent* event )
{
   if ( !isVisible() ) return;

   int w = width();
   int h = height();

   if ( w < 1 || h < 1 ) return;

   QPainter dc( this );
   dc.setRenderHint( QPainter::NonCosmeticDefaultPen );

   int x = 0;
   int y = 0;
   if ( isDown() || (isCheckable() && isChecked()) )
   {
      dc.drawImage( x, y, m_isHovered ? m_imgActiveHover : m_imgActive );
   }
   else
   {
      dc.drawImage( x, y, m_isHovered ? m_imgIdleHover : m_imgIdle );
   }

   // QPushButton::paintEvent( event );
}
