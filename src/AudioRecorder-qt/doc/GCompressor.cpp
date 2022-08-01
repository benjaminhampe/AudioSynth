#include "GCompressor.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
using namespace de::audio;

GCompressor::GCompressor( QWidget* parent )
   : QWidget(parent)
{
   setObjectName( "GCompressor" );
   setContentsMargins(0,0,0,0);

   m_menu = new GGroupV("Compressor", this);
   m_threshKnob = new GSliderV( "Threshold", 0,0,100,"*",1 , this );
   m_ratioKnob = new GSliderV( "Ratio",20,11,160,"2:1",1, this );
   m_attackKnob = new GSliderV( "Attack", 5,0,200,"ms",1, this );
   m_releaseKnob = new GSliderV( "Release", 5,0,200,"ms",1, this );
   m_gainKnob = new GSliderV( "Gain", 100,0,200,"%",1, this );

   QHBoxLayout* h = new QHBoxLayout();
   h->setContentsMargins(0,0,0,0);
   h->setSpacing( 0 );
   h->addWidget( m_menu );
   h->addWidget( m_threshKnob );
   h->addWidget( m_ratioKnob );
   h->addWidget( m_attackKnob );
   h->addWidget( m_releaseKnob );
   h->addWidget( m_gainKnob );
   setLayout( h );

   connect( m_menu, SIGNAL(toggledBypass(bool)), this, SLOT(on_menuButton_bypass(bool)) );
   connect( m_menu, SIGNAL(toggledMore(bool)), this, SLOT(on_menuButton_more(bool)) );
   connect( m_menu, SIGNAL(toggledHideAll(bool)), this, SLOT(on_menuButton_hideAll(bool)) );
   connect( m_threshKnob, SIGNAL(valueChanged(int)), this, SLOT(on_thresholdKnob(int)) );
   connect( m_ratioKnob, SIGNAL(valueChanged(int)), this, SLOT(on_ratioKnob(int)) );
   connect( m_attackKnob, SIGNAL(valueChanged(int)), this, SLOT(on_attackKnob(int)) );
   connect( m_releaseKnob, SIGNAL(valueChanged(int)), this, SLOT(on_releaseKnob(int)) );
   connect( m_gainKnob, SIGNAL(valueChanged(int)), this, SLOT(on_gainKnob(int)) );

}

GCompressor::~GCompressor()
{
}


void GCompressor::on_menuButton_bypass(bool checked)
{
   m_compressor.setBypassed( checked );
}

void GCompressor::on_menuButton_more(bool checked)
{
}

void GCompressor::on_menuButton_hideAll(bool checked)
{
   m_threshKnob->setVisible( false );
   m_ratioKnob->setVisible( false );
   m_attackKnob->setVisible( false );
   m_releaseKnob->setVisible( false );
   m_gainKnob->setVisible( false );
}

void GCompressor::on_thresholdKnob( int v )
{
   DE_DEBUG("value = ",v)
   m_compressor.setThreshold( 0.01f * v );
}

void GCompressor::on_ratioKnob( int v )
{
   DE_DEBUG("value = ",v)
   m_compressor.setRatio( 0.1f * v );
}

void GCompressor::on_attackKnob( int v )
{
   DE_DEBUG("value = ",v)
   m_compressor.setAttackTime( v );
}

void GCompressor::on_releaseKnob( int v )
{
   DE_DEBUG("value = ",v)
   m_compressor.setReleaseTime( v );
}
void GCompressor::on_gainKnob( int v )
{
   DE_DEBUG("value = ",v)
   m_compressor.setOutputGain( 0.01f * v );
}


void
GCompressor::clearInputSignals()
{
   m_compressor.clearInputSignals();
}

void
GCompressor::setInputSignal( int i, de::audio::IDspChainElement* input )
{
   m_compressor.setInputSignal( i, input );
}

void
GCompressor::setBypassed( bool bypassed )
{
   m_menu->setBypassButton( bypassed );
}

uint64_t
GCompressor::readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
{
   return m_compressor.readSamples( pts, dst, dstFrames, dstChannels, dstRate );
}

/*

void
GCompressor::setBypassed( bool enabled )
{
   m_isBypassed = enabled;
}

void
GCompressor::setChannel( int channel )
{
   m_channelIndex = channel;
}

void
GCompressor::setVolume( int volume_in_pc )
{
   m_volume = std::clamp( volume_in_pc, 0, 100 );
   m_slider->setValue( m_volume );
   m_labelB->setText( QString::number( m_volume ) + " %" );
}

void
GCompressor::timerEvent( QTimerEvent* event )
{
   if ( event->timerId() == m_updateTimerId )
   {
      update();
   }
}

// ============================================================================
void GCompressor::paintEvent( QPaintEvent* event )
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
