#include "GLimiter.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
using namespace de::audio;

GLimiter::GLimiter( QWidget* parent )
    : QWidget(parent)
{
   setObjectName( "GLimiter" );
   setContentsMargins(0,0,0,0);

   m_menu = new GGroupV("Limiter", this);
   m_attackTime = new GSliderV( "Vol.",10,1,800,"ms",1, this );
   m_releaseTime = new GSliderV( "Clean", 500,1,2000,"ms",1, this );
   m_gain = new GSliderV( "Gain", 100,0,200,"%",1, this );

   m_attackTime->setValue( int(m_limiter.m_attackInMs) );
   m_releaseTime->setValue( int(m_limiter.m_releaseInMs) );

   QHBoxLayout* h = new QHBoxLayout();
   h->setContentsMargins(0,0,0,0);
   h->setSpacing( 0 );
   h->addWidget( m_menu );
   h->addWidget( m_attackTime );
   h->addWidget( m_releaseTime );
   h->addWidget( m_gain );
   //h->addWidget( m_blendKnob );
   setLayout( h );

   connect( m_menu, SIGNAL(toggledBypass(bool)), this, SLOT(on_menuButton_bypass(bool)) );
   connect( m_menu, SIGNAL(toggledMore(bool)), this, SLOT(on_menuButton_more(bool)) );
   connect( m_menu, SIGNAL(toggledHideAll(bool)), this, SLOT(on_menuButton_hideAll(bool)) );

   connect( m_attackTime, SIGNAL(valueChanged(int)), this, SLOT(on_attackTime(int)), Qt::QueuedConnection );
   connect( m_releaseTime, SIGNAL(valueChanged(int)), this, SLOT(on_releaseTime(int)), Qt::QueuedConnection );
   connect( m_gain, SIGNAL(valueChanged(int)), this, SLOT(on_gain(int)), Qt::QueuedConnection );

}

GLimiter::~GLimiter()
{}

void GLimiter::on_menuButton_bypass(bool checked)
{
   m_limiter.setBypassed( checked );
}

void GLimiter::on_menuButton_more(bool checked)
{
   m_gain->setVisible( checked );
}

void GLimiter::on_menuButton_hideAll(bool checked)
{
   if ( checked )
   {
      m_attackTime->setVisible( false );
      m_releaseTime->setVisible( false );
      m_gain->setVisible( false );

   }
   else
   {
      m_attackTime->setVisible( true );
      m_releaseTime->setVisible( true );
      if ( m_menu->isMore() )
      {
         m_gain->setVisible( true );
      }
   }
}

void
GLimiter::clearInputSignals()
{
   m_limiter.clearInputSignals();
}

void
GLimiter::setInputSignal( int i, IDspChainElement* input )
{
   m_limiter.setInputSignal( i, input );
}

void
GLimiter::setBypassed( bool bypassed )
{
   m_menu->setBypassButton( bypassed );
}

uint64_t
GLimiter::readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
{
   return m_limiter.readSamples( pts, dst, dstFrames, dstChannels, dstRate );
   // auto gainVolume = 0.0001f * float( m_volumeKnob->value() * m_volumeKnob->value() );
   // return dstSamples;
}

void
GLimiter::on_attackTime( int v )
{
   DE_DEBUG("value = ",v)
   m_limiter.setAttackTime( v );
}

void
GLimiter::on_releaseTime( int v )
{
   DE_DEBUG("value = ",v)
   m_limiter.setReleaseTime( v );
}

void
GLimiter::on_gain( int v )
{
   DE_DEBUG("value = ",v)
   //m_limiter.setGain( v );
}
/*
void
GLimiter::timerEvent( QTimerEvent* event )
{
   if ( event->timerId() == m_updateTimerId )
   {
      update();
   }
}

// ============================================================================
void GLimiter::paintEvent( QPaintEvent* event )
// ============================================================================
{
   QPainter dc(this);
   dc.fillRect( rect(), QColor( 25,25,25 ) );

   int w = width();
   int h = height();
   int h2 = height() / 2;

   uint64_t srcSamples = m_shiftBuffer.size();
   uint64_t srcFrames = srcSamples / m_channelCount;
   uint64_t srcChannels = m_channelCount;
   if (srcFrames < 2) { return; }

   float const dx = float(w) / float(srcFrames);
   float const dy = float(h) * 0.5f;

   dc.setBrush( Qt::NoBrush );
   dc.setCompositionMode( QPainter::CompositionMode_SourceOver );

   for ( uint64_t c = 0; c < srcChannels; ++c )
   {
      if ( c == 0 )
         dc.setPen( QPen( QColor(255,55,55,63) ) );
      else if ( c == 1 )
         dc.setPen( QPen( QColor(255,225,55,63) ) );
      else if ( c == 2 )
         dc.setPen( QPen( QColor(55,200,55,63) ) );
      else
         dc.setPen( QPen( QColor(255,255,255,63) ) );

      for ( uint64_t i = 1; i < srcFrames; ++i )
      {
         const float sample1 = m_shiftBuffer[ srcChannels*(i-1) + c ];
         const float sample2 = m_shiftBuffer[ srcChannels*(i) + c ];
         const int x1 = dx * (i-1);
         const int x2 = dx * (i);
         const int y1 = dy - dy * sample1;
         const int y2 = dy - dy * sample2;
         dc.drawLine( x1,y1,x2,y2 );
      }
   }

   m_font5x8.drawText( dc, w/2, 1, QString("L_min(%1), L_max(%2), R_min(%3), R_max(%4)")
      .arg( L_min).arg( L_max ).arg( R_min).arg( R_max ), 0xFFFFFFFF, de::Align::CenterTop );
}
*/
