#include "GMasterDeck.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
using namespace de::audio;

GMasterDeck::GMasterDeck( QWidget* parent )
   : QImageWidget( parent )
   , m_inputSignal( nullptr )
{
   setObjectName( "GMasterDeck" );
   setContentsMargins(0,0,0,0);

   m_menu = new GDeck( "Master Effect Deck", this );
   m_body = new QImageWidget( this );

//   m_timeStretch = new GSoundTouch( this );
//   m_timeStretch->setObjectName( "MasterTimeStretch" );
   m_delay = new GDelay( this );
   m_pan = new GPan( this );
   m_lowPass3 = new GLowPass3( this );
   m_overdrive = new GOverdrive( this );
   //m_masterLopass1 = new GLowPass( this );
   //m_masterLopass2 = new GLowPass( this );
   m_lowPass4 = new GLowPass3( this );
   //m_masterLopass4 = new GLoPass( this );
   m_bassBoost = new GBassBoost( this );
   m_waveform = new GOszilloskop( this );
   //m_waveform3D = new GOszilloskop3D( this );
   m_spektrum = new GSpektrometer( this );
   m_volume = new GVolume( "Master Vol.", this );
   m_levelMeter = new GLevelMeter( this );

   m_compressor = new GCompressor( this );
   m_limiter = new GLimiter( this );

   auto h = new QHBoxLayout();
   h->setContentsMargins( 0,0,0,0 );
   h->setSpacing( 1 );
   h->addWidget( m_delay );
   h->addWidget( m_pan );
   h->addWidget( m_lowPass3 );
   h->addWidget( m_overdrive );
   h->addWidget( m_lowPass4 );
   h->addWidget( m_waveform,1 );
   h->addWidget( m_spektrum,1 );
   h->addWidget( m_bassBoost );
   h->addWidget( m_compressor );
   h->addWidget( m_limiter );
   h->addWidget( m_volume );
   h->addWidget( m_levelMeter );
   m_body->setLayout( h );

   auto mainLayout = new QVBoxLayout();
   mainLayout->setContentsMargins(0,0,0,0);
   mainLayout->setSpacing( 0 );
   mainLayout->addWidget( m_menu );
   mainLayout->addWidget( m_body,1 );
   setLayout( mainLayout );

   //m_synth2Waveform->setupDspElement( 512,2,48000 );
//   m_masterFilter->setInputSignal( m_mixer );
//   m_masterLopass1->setInputSignal( m_masterFilter );
//   m_masterLopass2->setInputSignal( m_masterLopass1 );
//   m_masterLopass3->setInputSignal( 0, m_masterLopass2 );
//   m_masterLopass4->setInputSignal( m_masterLopass3 );
//   m_masterOverdrive->setInputSignal( m_masterLopass4 );
   //m_timeStretch->setInputSignal( m_inputSignal );
   m_delay->setInputSignal( 0, m_inputSignal );
   m_pan->setInputSignal( 0, m_delay );
   m_lowPass3->setInputSignal( 0, m_pan );
   m_overdrive->setInputSignal( m_lowPass3 );
   m_lowPass4->setInputSignal( 0, m_overdrive );
   m_bassBoost->setInputSignal( m_lowPass4 );
   m_compressor->setInputSignal( 0, m_bassBoost );
   m_limiter->setInputSignal( 0, m_compressor );
   m_waveform->setInputSignal( 0, m_limiter );
   //m_waveform3D->setInputSignal( m_waveform );
   m_spektrum->setInputSignal( m_waveform );
   m_volume->setInputSignal( 0, m_spektrum );
   m_levelMeter->setInputSignal( m_volume );


   m_menu->setInputSignal( 0, m_levelMeter );
   //m_waveform->setInputSignal( 0, m_inputSignal );

   m_waveform->setupDspElement( 2*1024,2,48000 );

   m_delay->setBypassed( true );
   m_pan->setBypassed( true );
   m_lowPass3->setBypassed( true );
   m_overdrive->setBypassed( true );
   m_lowPass4->setBypassed( true );
   m_bassBoost->setBypassed( true );
   m_compressor->setBypassed( true );
   m_limiter->setBypassed( true );

   connect( m_menu, SIGNAL(toggledBypass(bool)), this, SLOT(on_menuButton_bypass(bool)) );
   connect( m_menu, SIGNAL(toggledMore(bool)), this, SLOT(on_menuButton_more(bool)) );
   connect( m_menu, SIGNAL(toggledHideAll(bool)), this, SLOT(on_menuButton_hideAll(bool)) );

}

GMasterDeck::~GMasterDeck()
{

}

void
GMasterDeck::clearInputSignals()
{
   m_inputSignal = nullptr;
}

void
GMasterDeck::setInputSignal( int i, IDspChainElement* input )
{
   m_lowPass3->setInputSignal( i, input ); // Lowpass is start of DSP chain
}

uint64_t
GMasterDeck::readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
{
   // Menu is end of DSP chain ( has a level meter )
   return m_menu->readSamples( pts, dst, dstFrames, dstChannels, dstRate );
}

void
GMasterDeck::on_menuButton_bypass(bool checked)
{
   //m_isBypassed = checked;
}

void
GMasterDeck::on_menuButton_more(bool checked)
{
   //m_qpeak->setVisible( checked );
   //m_volume->setVisible( checked );
}

void
GMasterDeck::on_menuButton_hideAll(bool checked)
{
   m_body->setVisible( !checked );
}
