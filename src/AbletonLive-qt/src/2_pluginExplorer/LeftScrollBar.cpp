#include "LeftScrollBar.hpp"

LeftScrollBar::LeftScrollBar( App & app, QWidget* parent ) 
   : QWidget( parent )
   , m_app( app )
   , m_isHovered( false )
{
   setObjectName( "LeftScrollBar" );
   setContentsMargins(0,0,0,0);
   setMinimumWidth( 33 );
   setMaximumWidth( 33 );
   update();
}

LeftScrollBar::~LeftScrollBar()
{}

void
LeftScrollBar::enterEvent( QEvent* event )
{ 
   m_isHovered = true; 
   QWidget::enterEvent( event ); 
}

void
LeftScrollBar::leaveEvent( QEvent* event )
{ 
   m_isHovered = false; 
   QWidget::leaveEvent( event ); 
}

void
LeftScrollBar::paintEvent( QPaintEvent* event )
{
   int w = width();
   int h = height();

   if ( w > 1 && h > 1 )
   {
      QPainter dc( this );
      dc.setRenderHint( QPainter::NonCosmeticDefaultPen );

      dc.end();
   }
   // QWidget::paintEvent( event );
}
