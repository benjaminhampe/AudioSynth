#include "GOverdrive.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
using namespace de::audio;

GOverdrive::GOverdrive( QWidget* parent )
    : QWidget(parent)
    , m_inputSignal( nullptr )
    , m_isBypassed( false )
{
   setObjectName( "GOverdrive" );
   setContentsMargins(0,0,0,0);

   m_menu = new GGroupV("OverDrive", this);
   m_gainKnob = new GSliderV( "Gain", 0,0,1000,"*",1 , this );
   m_volumeKnob = new GSliderV( "Vol.",0,0,200,"%",1, this );
   m_cleanKnob = new GSliderV( "Clean", 100,0,200,"%",1, this );
   //m_blendKnob = new GSliderV( "Blend",50,0,100,"%",1, this );

   QHBoxLayout* h = new QHBoxLayout();
   h->setContentsMargins(0,0,0,0);
   h->setSpacing( 0 );
   h->addWidget( m_menu );
   h->addWidget( m_gainKnob );
   h->addWidget( m_volumeKnob );
   h->addWidget( m_cleanKnob );
   //h->addWidget( m_blendKnob );
   setLayout( h );

   DSP_RESIZE( m_inputBuffer, 2*1024 );

   connect( m_menu, SIGNAL(toggledBypass(bool)), this, SLOT(on_menuButton_bypass(bool)) );
   connect( m_menu, SIGNAL(toggledMore(bool)), this, SLOT(on_menuButton_more(bool)) );
   connect( m_menu, SIGNAL(toggledHideAll(bool)), this, SLOT(on_menuButton_hideAll(bool)) );
}

GOverdrive::~GOverdrive()
{
}


void GOverdrive::on_menuButton_bypass(bool checked)
{
   m_isBypassed = checked;
}

void GOverdrive::on_menuButton_more(bool checked)
{
   //m_gainKnob->setVisible( checked );
   //m_volumeKnob->setVisible( checked );
   m_cleanKnob->setVisible( checked );
}

void GOverdrive::on_menuButton_hideAll(bool checked)
{
   if ( checked )
   {
      m_gainKnob->setVisible( false );
      m_volumeKnob->setVisible( false );
      m_cleanKnob->setVisible( false );
   }
   else
   {
      m_gainKnob->setVisible( true );
      m_volumeKnob->setVisible( true );
      if ( m_menu->isMore() )
      {
         m_cleanKnob->setVisible( true );
      }
   }
}

void
GOverdrive::clearInputSignals()
{
   m_inputSignal = nullptr;
}

void
GOverdrive::setInputSignal( de::audio::IDspChainElement* input )
{
   m_inputSignal = input;
}

void
GOverdrive::setBypassed( bool bypassed )
{
   m_menu->setBypassButton( bypassed ); // calls then on_menu_bypass
}

uint64_t
GOverdrive::readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
{
   uint64_t dstSamples = dstFrames * dstChannels;
   if ( !m_inputSignal )
   {
      DE_WARN("No input")
      DSP_FILLZERO( dst, dstSamples );
      return dstSamples;
   }

   if ( m_isBypassed )
   {
      m_inputSignal->readSamples( pts, dst, dstFrames, dstChannels, dstRate );
      return dstSamples;
   }

   DSP_RESIZE( m_inputBuffer, dstSamples );
   DSP_FILLZERO( m_inputBuffer );
   m_inputSignal->readSamples( pts, m_inputBuffer.data(), dstFrames, dstChannels, dstRate );

   // One knob controls clean signal
   // We allow clean signal to be enhanced by factor 3x so in range [0,300%] or more later
   auto origVolume = 0.0001f * float( m_cleanKnob->value() * m_cleanKnob->value() );

   // Two knobs controls drive signal
   // We allow drive signal to be enhanced by factor 3000x or so
   auto gainFactor = float( m_gainKnob->value() );
   auto gainVolume = 0.0001f * float( m_volumeKnob->value() * m_volumeKnob->value() );

   auto const TWO_OVER_PI = float( 2.0 / M_PI );
   //auto const ONE_OVER_BLEND = 1.0f / (blend < 0.0001f ? 0.0001f : blend);

   auto pSrc = m_inputBuffer.data();
   auto pDst = dst;

   for ( uint64_t i = 0; i < dstFrames; ++i )
   {
      for ( uint32_t c = 0; c < dstChannels; ++c )
      {
         float origSig = *pSrc;
         float gainSig = TWO_OVER_PI * atanf( origSig * gainFactor );  // atan makes it very smooth
         *pDst = gainSig * gainVolume + origSig * origVolume;     // average and apply final volume control
         pSrc++;  // advance to next sample
         pDst++;  // in and out buffer must atleast be in same order ( both interleaved or both planar with same channel count )
      }
   }

   return dstSamples;
}


/*

void
GOverdrive::setBypassed( bool enabled )
{
   m_isBypassed = enabled;
}

void
GOverdrive::setChannel( int channel )
{
   m_channelIndex = channel;
}

void
GOverdrive::setVolume( int volume_in_pc )
{
   m_volume = std::clamp( volume_in_pc, 0, 100 );
   m_slider->setValue( m_volume );
   m_labelB->setText( QString::number( m_volume ) + " %" );
}

void
GOverdrive::timerEvent( QTimerEvent* event )
{
   if ( event->timerId() == m_updateTimerId )
   {
      update();
   }
}

// ============================================================================
void GOverdrive::paintEvent( QPaintEvent* event )
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
