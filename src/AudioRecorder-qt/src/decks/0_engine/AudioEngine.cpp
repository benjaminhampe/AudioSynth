#include "GDetuneSynthDeck.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <de/Color.hpp>
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

}
// ===================================================================

GDetuneSynthDeck::GDetuneSynthDeck( QWidget* parent )
   : QImageWidget( parent )
{
   setObjectName( "GDetuneSynthDeck" );
   setContentsMargins(0,0,0,0);

   m_menu = new GDeck("GDetuneSynthDeck", this);
   m_body = new QImageWidget( this );
   //m_scrollArea = new QScrollArea( this ); //
   //m_scrollArea->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
   m_mixer.setInputSignalCount( 8 );

   auto h = new QHBoxLayout();
   h->setContentsMargins(0,0,0,0);
   h->setSpacing( 0 );

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
         auto synth = new GDetuneSynth( this );
         synth->setBackgroundColor( bgColor );
         v->addWidget( synth );

         m_synths[ linearIndex ] = synth;
         m_mixer.setInputSignal( linearIndex, synth );
      }

      v->addStretch(1);
      h->addLayout( v );
   }

   m_body->setLayout( h );

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

   connect( m_menu, SIGNAL(toggledBypass(bool)), this, SLOT(on_menuButton_bypass(bool)) );
   connect( m_menu, SIGNAL(toggledMore(bool)), this, SLOT(on_menuButton_more(bool)) );
   connect( m_menu, SIGNAL(toggledHideAll(bool)), this, SLOT(on_menuButton_hideAll(bool)) );

}

GDetuneSynthDeck::~GDetuneSynthDeck()
{
   // stopUpdateTimer();
}

void
GDetuneSynthDeck::sendNote( de::audio::Note const & note )
{
   for ( auto & synth : m_synths )
   {
      if ( synth && !synth->isBypassed() ) synth->sendNote( note );
   }
}

void
GDetuneSynthDeck::on_menuButton_bypass(bool checked)
{
   //m_isBypassed = checked;
}

void
GDetuneSynthDeck::on_menuButton_more(bool checked)
{
   //m_qpeak->setVisible( checked );
   //m_volume->setVisible( checked );
}

void
GDetuneSynthDeck::on_menuButton_hideAll(bool checked)
{
   if ( checked )
   {
      m_body->setVisible( false );
      //m_volume->setVisible( false );
   }
   else
   {
      m_body->setVisible( true );
      if ( m_menu->isMore() )
      {
         //m_volume->setVisible( true );
      }
   }
}

uint64_t
GDetuneSynthDeck::readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
{
   return m_mixer.readSamples( pts, dst, dstFrames, dstChannels, dstRate );

//   uint64_t dstSamples = dstFrames * dstChannels;

//   DSP_RESIZE( m_inputBuffer, dstSamples );

//   if ( m_menu->isBypassed() )
//   {
//      DSP_FILLZERO( dst, dstSamples );

//   }
//   else
//   {
//      // DSP_RESIZE( m_mixerBuffer, dstSamples );
//      // DSP_FILLZERO( m_mixerBuffer );

//      for ( auto & r : m_synths )
//      {
//         if ( !r ) continue;
//         DSP_FILLZERO( m_inputBuffer );
//         r->readSamples( pts, m_inputBuffer.data(), dstFrames, dstChannels, dstRate );
//         DSP_ADD( m_inputBuffer.data(), dst, dstSamples );
//      }

//      //float vol = 1.0f; // * m_volume;
//      //de::audio::DSP_COPY( m_mixerBuffer.data(), dst, dstSamples );


//   }

//   return dstSamples;
}
