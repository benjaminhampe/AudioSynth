#include "Button.hpp"

Button::Button( QWidget* parent ) : QPushButton( parent )
{
   setObjectName( "Button" );
   setContentsMargins(0,0,0,0);
   m_isHovered = false;
   update();
}

Button::~Button()
{}

void
Button::setImage( int i, QImage const & img )
{
   switch( i )
   {
      case 0: m_imgIdle = img; break;
      case 1: m_imgIdleHover = img; break;
      case 2: m_imgActive = img; break;
      case 3: m_imgActiveHover = img; break;
      default: break;
   }
   setMinimumSize( img.size() );
   setMaximumSize( img.size() );
}

void
Button::paintEvent( QPaintEvent* event )
{
   int w = width();
   int h = height();

   if ( w > 1 && h > 1 )
   {
      QPainter dc( this );
      dc.setRenderHint( QPainter::NonCosmeticDefaultPen );

      bool m_isActive = isChecked() || isDown();
      if ( m_isActive )
      {
         if ( m_isHovered )
         {
            dc.drawImage( 0,0,m_imgActiveHover );
         }
         else
         {
            dc.drawImage( 0,0,m_imgActive );
         }
      }
      else // if ( !m_isActive )
      {
         if ( m_isHovered )
         {
            dc.drawImage( 0,0,m_imgIdleHover );
         }
         else
         {
            dc.drawImage( 0,0,m_imgIdle );
         }
      }
      dc.end();
   }

   //m_font5x8.drawText( img, 42,2, "Hypersonic", LiveSkin::getQColor( LiveSkin::TitleText ) );

   // QWidget::paintEvent( event );
}
