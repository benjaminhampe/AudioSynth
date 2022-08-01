#include "Arrangement.hpp"

// ============================================================================
Arrangement::Arrangement( App & app, QWidget* parent )
   : QWidget( parent )
   , m_app( app )
{
   setObjectName( "Arrangement" );
   setContentsMargins(0,0,0,0);
   //setMinimumWidth( 250 );

   reset();
}

Arrangement::~Arrangement()
{
}

void
Arrangement::reset()
{
   m_focusedTrack = -1;
   m_headHeight = 20;
   m_setHeight = 24;
   m_pinHeight = 8;
   m_trackHeight = 17;
   m_trackSpace = 2;
   m_footHeight = m_headHeight;
}

void
Arrangement::paintEvent( QPaintEvent* event )
{
   int w = width();
   int h = height();

   if ( w > 1 && h > 1 )
   {
      LiveSkin const & skin = m_app.m_skin;
      BeatGrid const & grid = m_app.m_beatGrid;
      TrackList const & trackList = m_app.m_trackList;

      QPainter dc( this );
      dc.setRenderHint( QPainter::NonCosmeticDefaultPen );

      // Draw background
      int y = 0;
      int pxPerBeat = grid.time2pixel( 1 );
      int k = float( w ) / float( pxPerBeat );
      // Draw beat indices
      dc.fillRect( QRect(0,0,w-1,m_headHeight), skin.panelColor );
      dc.setBrush( Qt::NoBrush );
      dc.setPen( QPen( skin.windowColor ) );
      dc.setFont( QFont("Arial Black", 8) );
      for ( int i = 0; i < k; ++i )
      {
         int x = grid.time2pixel( i );
         int y1 = y + m_headHeight - 1 - 8;
         int y2 = y + m_headHeight - 2;
         drawLine( dc, x, y1, x, y2, skin.windowColor );
         drawLine( dc, x, y1, x+2, y1, skin.windowColor );

         dc.drawText( x + 4, y2-2, QString::number( i ) );
      }
      y += m_headHeight;

      // Draw ContentRect
      QRect r_content(0,m_headHeight,w,h-m_headHeight-m_footHeight);
      dc.fillRect( r_content, skin.contentColor );

      // Draw BeatGrid
      for ( int i = 0; i < k; ++i )
      {
         int x = grid.time2pixel( i );
         int y1 = m_headHeight;
         int y2 = h - 1 - m_headHeight - m_footHeight;
         drawLine( dc, x, y1, x, y2, skin.panelColor );

         x = grid.time2pixel( double(i)+0.5 );
         drawLine( dc, x, y1, x, y2, skin.semiBeatColor );

         dc.drawText( x + 7, y2-2, QString::number( i ) );
      }

      // Draw TimePin
      y = m_headHeight + m_setHeight;
      dc.fillRect( QRect(0,y,w-1,m_pinHeight), skin.panelColor );
      y += m_pinHeight;

      // Draw tracks
      for ( int i = 0; i < trackList.trackCount(); ++i )
      {
         // highlight #3
         if ( i == m_focusedTrack )
         {
            dc.fillRect( QRect(0,y,w-1,m_trackHeight), skin.editBoxColor );
         }

         Track const & track = trackList.track( i );
         for ( int c = 0; c < track.clipCount(); ++c )
         {
            Clip const & clip = track.clip( c );
            int x1 = grid.time2pixel( clip.m_timeBeg );
            int x2 = grid.time2pixel( clip.m_timeEnd )-1;
            dc.setBrush( QBrush( toQColor( clip.m_color ) ) );
            dc.setPen( QPen( QColor(0,0,0) ) );
            dc.drawRect( QRect( x1, y+1, x2-x1, m_trackHeight-3 ) );
         }

         y += m_trackHeight;

         dc.fillRect( QRect(0,y,w-1,m_trackSpace), skin.panelColor );
         y += m_trackSpace;
      }
   }

   QWidget::paintEvent( event );
}
