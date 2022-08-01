#include "HelpViewer.hpp"

HelpViewer::HelpViewer( App & app, QWidget* parent )
   : QWidget( parent )
   , m_app( app )
{
   setObjectName( "HelpViewer" );
   setContentsMargins(0,0,0,0);
   setMinimumWidth( 100 );
}

HelpViewer::~HelpViewer()
{
}


//void
//HelpViewer::resizeEvent( QResizeEvent* event )
//{
//   update();
//   QWidget::resizeEvent( event );
//}

void
HelpViewer::paintEvent( QPaintEvent* event )
{
   int w = width();
   int h = height();

   if ( w > 1 && h > 1 )
   {
      LiveSkin const & skin = m_app.m_skin;

      QPainter dc( this );
      dc.setRenderHint( QPainter::NonCosmeticDefaultPen );

      // Draw background
      dc.fillRect( rect(), skin.contentColor );

      // Draw title rect
      dc.fillRect( QRect(0, 0, w-1, skin.titleH ), skin.titleColor );

      // Draw table header
      dc.fillRect( QRect(0, skin.titleH, w-1, skin.tdH), skin.tdColor );

   }

   QWidget::paintEvent( event );
}
