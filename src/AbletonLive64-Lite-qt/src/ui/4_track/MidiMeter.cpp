#include "MidiMeter.hpp"
#include <QResizeEvent>

#include "App.hpp"

// ============================================================================
MidiMeter::MidiMeter( App & app, QWidget* parent )
// ============================================================================
   : QWidget(parent)
   , m_app( app )
   , m_hasFocus( false )
{
   setObjectName( "MidiMeter" );
   setContentsMargins( 0,0,0,0 );
}

void
MidiMeter::resizeEvent( QResizeEvent* event )
{
   QWidget::resizeEvent( event );

   int w = event->size().width();
   int h = event->size().height();
   //std::cout << "w = " << w << ", h = " << h << std::endl;

   //if ( w < 4 ) return;
   //if ( h < 4 ) return;

   //update();
}

void MidiMeter::paintEvent( QPaintEvent* event )
{
   int w = width();
   int h = height();

   auto const & skin = m_app.m_skin;

   QPainter dc( this );
   dc.setRenderHint( QPainter::NonCosmeticDefaultPen );

   dc.setPen( Qt::NoPen );
   dc.setBrush( QBrush( skin.symbolColor ) );

   int x = w/2-2;
   int y = (h - 40)/2;
   if ( x < 0 ) x = 0;
   if ( y < 0 ) y = 0;
   for ( int j = 0; j < 10; ++j )
   {
      dc.drawRect( QRect(x,y,3,3) ); y += 4;
   }
}

void
MidiMeter::focusInEvent( QFocusEvent* event )
{
   m_hasFocus = true;
   update();
   QWidget::focusInEvent( event );
}

void
MidiMeter::focusOutEvent( QFocusEvent* event )
{
   m_hasFocus = true;
   update();
   QWidget::focusOutEvent( event );
}

void
MidiMeter::enterEvent( QEvent* event )
{
   QWidget::enterEvent( event );
}

void
MidiMeter::leaveEvent( QEvent* event )
{
   QWidget::leaveEvent( event );
}



