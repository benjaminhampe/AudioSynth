#include "GBetterVoice.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QDebug>

using namespace de::audio; // DSP_RESIZE, DSP_FILLZERO, DSP_COPY

GBetterVoice::GBetterVoice( QWidget* parent )
    : QWidget(parent)
    , m_voice( nullptr )
{
   setObjectName( "GBetterVoice" );
   setContentsMargins(0,0,0,0);
   //setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

   m_menu = new GDeck( "Voice 01",this );
   m_body = new QImageWidget( this );
   m_osc = new GMultiOsc( this );
   m_lfoPhase = new GLFO( "Phase", this );
   m_lfoPulseWidth = new GLFO( "PW", this );
   m_lfoDetune = new GLFO( "Detune", this );
   m_adsr = new GADSR( this );

   auto h = new QHBoxLayout();
   h->setContentsMargins(0,0,0,0);
   h->setSpacing( 1 );
   h->addWidget( m_osc );
   h->addWidget( m_adsr );
   h->addWidget( m_lfoPhase );
   h->addWidget( m_lfoPulseWidth );
   h->addWidget( m_lfoDetune );
   h->addStretch( 1 );
   m_body->setLayout( h );

   auto mainLayout = new QVBoxLayout();
   mainLayout->setContentsMargins(0,0,0,0);
   mainLayout->setSpacing( 0 );
   mainLayout->addWidget( m_menu );
   mainLayout->addWidget( m_body,1 );
   setLayout( mainLayout );

   m_lfoPhase->setValues( 0, 1, 0, 0 );
   m_lfoPulseWidth->setValues( 0, 1, 0.5, 0 );
   m_lfoDetune->setValues( 0, 1, 0, 0 );

   connect( m_menu, SIGNAL(toggledBypass(bool)), this, SLOT(on_menuButton_bypass(bool)) );
   connect( m_menu, SIGNAL(toggledMore(bool)), this, SLOT(on_menuButton_more(bool)) );
   connect( m_menu, SIGNAL(toggledHideAll(bool)), this, SLOT(on_menuButton_hideAll(bool)) );
   connect( m_osc, SIGNAL(oscTypeChanged(int)), this, SLOT(on_oscTypeChanged(int)) );
   connect( m_osc, SIGNAL(coarseTuneChanged(int)), this, SLOT(on_coarseTuneChanged(int)) );
   connect( m_osc, SIGNAL(fineTuneChanged(int)), this, SLOT(on_fineTuneChanged(int)) );
   connect( m_osc, SIGNAL(phaseChanged(int)), this, SLOT(on_phaseChanged(int)) );
   connect( m_osc, SIGNAL(volumeChanged(int)), this, SLOT(on_volumeChanged(int)) );

   m_menu->setVisibleButton( false );
   m_menu->setBypassButton( true );

   m_osc->setVisibleButton( false );
   m_osc->setBypassButton( true );

   m_lfoPhase->setVisibleButton( false );
   m_lfoPhase->setBypassButton( true );

   m_lfoPulseWidth->setVisibleButton( false );
   m_lfoPulseWidth->setBypassButton( true );

   m_lfoDetune->setVisibleButton( false );
   m_lfoDetune->setBypassButton( true );

   m_adsr->setVisibleButton( false );
   m_adsr->setBypassButton( false );

   //m_menu->setInputSignal( 0, m_waveform );
}

GBetterVoice::~GBetterVoice()
{
   // stopUpdateTimer();
}

de::audio::BetterVoice*
GBetterVoice::getVoice() const { return m_voice; }

void
GBetterVoice::setVoice( de::audio::BetterVoice* voice )
{
   m_voice = voice;
   if ( voice )
   {
      //m_osc->setOSC( &voice->m_osc );
      m_lfoPhase->setLFO( &voice->m_lfoPhase );
      m_lfoPulseWidth->setLFO( &voice->m_lfoPulseWidth );
      m_lfoDetune->setLFO( &voice->m_lfoDetune );
      m_adsr->setADSR( &voice->m_adsr );
   }
   else
   {
      //m_osc->setOSC( nullptr );
      m_lfoPhase->setLFO( nullptr );
      m_lfoPulseWidth->setLFO( nullptr );
      m_lfoDetune->setLFO( nullptr );
      m_adsr->setADSR( nullptr );
   }
}

void GBetterVoice::on_menuButton_bypass(bool checked)
{
   if ( m_voice ) m_voice->setBypassed( checked );
}

void GBetterVoice::on_menuButton_more(bool checked)
{
   //m_waveform->setVisible( checked );
}

void GBetterVoice::on_menuButton_hideAll(bool checked)
{
   m_body->setVisible( !checked );
}

void GBetterVoice::on_oscTypeChanged( int v )
{
   if ( !m_voice ) { DE_ERROR("No voice") return; }
   m_voice->setOscType( v );
}
void GBetterVoice::on_coarseTuneChanged( int v )
{
   if ( !m_voice ) { DE_ERROR("No voice") return; }
   m_voice->setDetune( v );
}
void GBetterVoice::on_fineTuneChanged( int v )
{
   if ( !m_voice ) { DE_ERROR("No voice") return; }
   m_voice->setFineDetune( v );
}
void GBetterVoice::on_phaseChanged( int v )
{
   if ( !m_voice ) { DE_ERROR("No voice") return; }
   m_voice->setPhaseLfoType( v );
}
void GBetterVoice::on_volumeChanged( int v )
{
   if ( !m_voice ) { DE_ERROR("No voice") return; }
   m_voice->setVolume( v );
}
