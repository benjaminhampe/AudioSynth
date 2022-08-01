#include "GVstDeck.hpp"
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
GVstDeck::GVstDeck( QWidget* parent )
// ===================================================================
   : QImageWidget( parent )
{
   setObjectName( "GVstDeck" );
   setContentsMargins(0,0,0,0);

   m_menu = new GDeck("GVstDeck", this);
   m_body = new QImageWidget( this );

   m_vst1 = new GVstPlugin( this );
   m_vst2 = new GVstPlugin( this );
   m_vst3 = new GVstPlugin( this );
   m_vst4 = new GVstPlugin( this );
   m_vst5 = new GVstPlugin( this );

   //m_scrollArea = new QScrollArea( this ); //
   //m_scrollArea->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

   auto h = new QHBoxLayout();
   h->setContentsMargins(0,0,0,0);
   h->setSpacing( 0 );
   h->addWidget( m_vst1 );
   h->addWidget( m_vst2 );
   h->addWidget( m_vst3 );
   h->addWidget( m_vst4 );
   h->addWidget( m_vst5 );
   m_body->setLayout( h );

   QVBoxLayout* mainLayout = new QVBoxLayout();
   mainLayout->setContentsMargins(0,0,0,0);
   mainLayout->setSpacing( 0 );
   mainLayout->addWidget( m_menu );
   mainLayout->addWidget( m_body,1 );
   setLayout( mainLayout );

   connect( m_menu, SIGNAL(toggledBypass(bool)), this, SLOT(on_menuButton_bypass(bool)) );
   connect( m_menu, SIGNAL(toggledMore(bool)), this, SLOT(on_menuButton_more(bool)) );
   connect( m_menu, SIGNAL(toggledHideAll(bool)), this, SLOT(on_menuButton_hideAll(bool)) );

//   connect( m_vst1, SIGNAL(addedSynth(de::audio::synth::ISynth*)),
//               this, SLOT(on_addedSynth(de::audio::synth::ISynth*)) );
//   connect( m_vst1, SIGNAL(removedSynth(de::audio::synth::ISynth*)),
//               this, SLOT(on_removedSynth(de::audio::synth::ISynth*)) );

   m_mixer.setInputSignal( 0, m_vst1 );
   m_mixer.setInputSignal( 1, m_vst2 );

   m_synths.emplace_back( m_vst1 );
   m_synths.emplace_back( m_vst2 );
   m_synths.emplace_back( m_vst3 );
   m_synths.emplace_back( m_vst4 );
   m_synths.emplace_back( m_vst5 );

}

GVstDeck::~GVstDeck()
{
   // stopUpdateTimer();
}

void
GVstDeck::sendNote( de::audio::Note const & note )
{
   for ( auto & synth : m_synths )
   {
      //if ( synth )
      synth->sendNote( note );
      //if ( synth && !synth->isBypassed() ) synth->sendNote( note );
   }
}

uint64_t
GVstDeck::readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
{
   return m_mixer.readSamples( pts, dst, dstFrames, dstChannels, dstRate );
}

void
GVstDeck::on_menuButton_bypass(bool checked)
{
   //m_isBypassed = checked;
}

void
GVstDeck::on_menuButton_more(bool checked)
{
   //m_qpeak->setVisible( checked );
   //m_volume->setVisible( checked );
}

void
GVstDeck::on_menuButton_hideAll(bool checked)
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
/*
void
GVstDeck::on_addedSynth( de::audio::IDspChainElement* synth )
{
   auto it = std::find_if( m_synths.begin(), m_synths.end(),
               [&] ( de::audio::IDspChainElement* cached ) { return cached == synth; } );
   if ( it == m_synths.end() )
   {
      m_synths.emplace_back( synth );
      DE_DEBUG("Added synth ", (void*)synth, ", n = ", m_synths.size() )
   }
}

void
GVstDeck::on_removedSynth( de::audio::IDspChainElement* synth )
{
   auto it = std::find_if( m_synths.begin(), m_synths.end(),
               [&] ( de::audio::IDspChainElement* cached ) { return cached == synth; } );
   if ( it != m_synths.end() )
   {
      m_synths.erase( it );
      DE_DEBUG("Removed synth ", (void*)synth, ", n = ", m_synths.size() )
   }
}
*/

