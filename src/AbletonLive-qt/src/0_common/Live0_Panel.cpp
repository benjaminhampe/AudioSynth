#include "Live0_Panel.hpp"

Live0_Panel::Live0_Panel( App & app, QWidget* parent )
   : QWidget( parent )
   , m_app( app )
   , m_hasFocus( false )
{
   setContentsMargins( 6,6,6,6 );
   setFocusPolicy( Qt::ClickFocus );
}

Live0_Panel::~Live0_Panel()
{

}

void
Live0_Panel::focusInEvent( QFocusEvent* event )
{
   std::cout << "Live0_Panel::focusInEvent" << std::endl;
   m_hasFocus = true;
   emit focusChanged( m_hasFocus );
   update();
}

void
Live0_Panel::focusOutEvent( QFocusEvent* event )
{
   std::cout << "Live0_Panel::focusOutEvent" << std::endl;
   m_hasFocus = false;
   emit focusChanged( m_hasFocus );
   update();
}

void
Live0_Panel::paintEvent( QPaintEvent* event )
{
   int w = width();
   int h = height();

   if ( w > 1 && h > 1 )
   {
      LiveSkin const & skin = m_app.m_skin;

      QPainter dc( this );
      dc.setRenderHint( QPainter::NonCosmeticDefaultPen );

      // Draw background
      dc.fillRect( rect(), skin.windowColor );

      int px = 0;
      int py = 0;

      dc.setPen( Qt::NoPen );

      if ( m_hasFocus )
      {
         std::cout << "Live0_Panel has focus" << std::endl;
      }

      // Draw panel background
      QRect rc( px, py, w-2*px, h-2*py);
      dc.setBrush( QBrush( m_hasFocus ? skin.focusColor : skin.panelColor ) );
      dc.drawRoundedRect( rc, skin.radius, skin.radius );

      //drawRectBorder( dc, rect(), QColor(255,100,0) );

      dc.end();
   }

   //m_font5x8.drawText( img, 42,2, "Hypersonic", LiveSkin::getQColor( LiveSkin::TitleText ) );

   QWidget::paintEvent( event );
}
