#include "GBetterSynthDeck.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
using namespace de::audio;

namespace {

QColor toQColor( uint32_t color )
{
   int32_t r = de::RGBA_R(color);
   int32_t g = de::RGBA_G(color);
   int32_t b = de::RGBA_B(color);
   int32_t a = de::RGBA_A(color);
   return QColor( b,g,r,a );
}

} // end namespace

// ===================================================================

GBetterSynthDeck::GBetterSynthDeck( QWidget* parent )
   : QImageWidget( parent )
{
   setObjectName( "GBetterSynthDeck" );
   setContentsMargins(0,0,0,0);

   m_menu = new GDeck("GBetterSynthDeck", this);
   m_body = new QImageWidget( this );

   m_lowPass3 = new GLowPass3( this );
   m_levelMeter = new GLevelMeter( this );
   m_volumeSlider = new GVolume( "P1-Vol.", this );
   m_waveform = new GOszilloskop( this );

   auto h1 = new QHBoxLayout();
   h1->setContentsMargins(0,0,0,0);
   h1->setSpacing( 1 );
   h1->addWidget( m_lowPass3 );
   h1->addWidget( m_waveform,1 );
   h1->addWidget( m_volumeSlider );
   h1->addWidget( m_levelMeter );

   auto h2 = new QHBoxLayout();
   h2->setContentsMargins(0,0,0,0);
   h2->setSpacing( 0 );

   for ( size_t col = 0; col < 2; ++col )
   {
      auto v = new QVBoxLayout();
      v->setContentsMargins(0,0,0,0);
      v->setSpacing( 0 );

      for ( size_t i = 0; i < 4; ++i )
      {
         auto linearIndex = 4*col + i;
         float t = float( linearIndex ) / 7.0f;
         QColor bgColor = toQColor( de::RainbowColor::computeColor32( t ) );
         GBetterVoice* voice = new GBetterVoice( this );
         voice->setBackgroundColor( bgColor );
         voice->setVoice( &m_synth.m_voices[ linearIndex ] );
         v->addWidget( voice );
      }

      v->addStretch(1);
      h2->addLayout( v,1 );
   }

   h2->addLayout( h1,1 );

//   auto v1 = new QVBoxLayout();
//   v1->setContentsMargins(0,0,0,0);
//   v1->setSpacing( 0 );
//   v1->addLayout( h1 );
//   v1->addLayout( h2,1 );
   m_body->setLayout( h2 );

//   auto bodyLayout = new QVBoxLayout();
//   bodyLayout->setContentsMargins(0,0,0,0);
//   bodyLayout->setSpacing( 0 );
//   bodyLayout->addWidget( m_body,1 );
//   m_scrollArea->setWidget( m_body );

   QVBoxLayout* mainLayout = new QVBoxLayout();
   mainLayout->setContentsMargins(0,0,0,0);
   mainLayout->setSpacing( 0 );
   mainLayout->addWidget( m_menu );
   mainLayout->addWidget( m_body,1 );
   setLayout( mainLayout );

   //DSP_RESIZE( m_inputBuffer, 2*1024 );
   //DSP_RESIZE( m_mixerBuffer, 1024 );


   m_waveform->setShiftBufferSize( 1024 );

   m_lowPass3->setInputSignal( 0, &m_synth );
   m_volumeSlider->setInputSignal( 0, m_lowPass3 );
   m_levelMeter->setInputSignal( m_volumeSlider );
   m_waveform->setInputSignal( 0, m_levelMeter );
   m_menu->setInputSignal( 0, m_waveform );


   m_lowPass3->setVisibleButton( false );
   m_lowPass3->setBypassed( true );

   connect( m_menu, SIGNAL(toggledBypass(bool)), this, SLOT(on_menuButton_bypass(bool)) );
   connect( m_menu, SIGNAL(toggledMore(bool)), this, SLOT(on_menuButton_more(bool)) );
   connect( m_menu, SIGNAL(toggledHideAll(bool)), this, SLOT(on_menuButton_hideAll(bool)) );

}

GBetterSynthDeck::~GBetterSynthDeck()
{
   // stopUpdateTimer();
}

void
GBetterSynthDeck::sendNote( de::audio::Note const & note )
{
   m_synth.sendNote( note );
}

void
GBetterSynthDeck::on_menuButton_bypass(bool checked)
{
   m_synth.setBypassed( checked );
}

void
GBetterSynthDeck::on_menuButton_more(bool checked)
{
   //m_qpeak->setVisible( checked );
   //m_volume->setVisible( checked );
}

void
GBetterSynthDeck::on_menuButton_hideAll(bool checked)
{
   m_body->setVisible( !checked );
}

uint64_t
GBetterSynthDeck::readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
{
   return m_menu->readSamples( pts, dst, dstFrames, dstChannels, dstRate );
}
