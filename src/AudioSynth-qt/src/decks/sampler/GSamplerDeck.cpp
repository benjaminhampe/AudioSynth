#include "GSamplerDeck.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
using namespace de::audio;

GSamplerDeck::GSamplerDeck( QWidget* parent )
   : QWidget( parent )
{
   setObjectName( "GSamplerDeck" );
   setContentsMargins(0,0,0,0);

   m_menu = new GDeck("StereoPlayerDeck A+B with Filter and TimeStretch", this);
   m_body = new QWidget( this);

   QVBoxLayout* vm = new QVBoxLayout();
   vm->setContentsMargins(0,0,0,0);
   vm->setSpacing( 0 );
   vm->addWidget( m_menu );
   vm->addWidget( m_body,1 );
   setLayout( vm );

   // m_leftPlayer = new GFilePlayer( false, this );
   // m_leftFilter = new GLowPass3( this );
   // m_leftTouch = new GSoundTouch( this );
   // m_leftLevel = new GLevelMeter( this );

   // //GOszilloskop* m_leftWaveform;
   // //GSpektrometer* m_leftSpektrum;

   // m_rightPlayer = new GFilePlayer( true, this );
   // m_rightFilter = new GLowPass3( this );
   // m_rightTouch = new GSoundTouch( this );
   // m_rightLevel = new GLevelMeter( this );

   // //GOszilloskop* m_rightWaveform;
   // //GSpektrometer* m_rightSpektrum;
   // m_crossFader = new GCrossFader( this );

   // QHBoxLayout* h1 = new QHBoxLayout();
   // h1->setContentsMargins(0,0,0,0);
   // h1->setSpacing( 0 );
   // h1->addWidget( m_leftPlayer );
   // h1->addWidget( m_leftFilter );
   // h1->addWidget( m_leftTouch );
   // h1->addWidget( m_leftLevel );

   // QHBoxLayout* h2 = new QHBoxLayout();
   // h2->setContentsMargins(0,0,0,0);
   // h2->setSpacing( 0 );
   // h2->addWidget( m_rightLevel );
   // h2->addWidget( m_rightTouch );
   // h2->addWidget( m_rightFilter );
   // h2->addWidget( m_rightPlayer );

   // QHBoxLayout* v = new QHBoxLayout();
   // v->setContentsMargins(0,0,0,0);
   // v->setSpacing( 3 );
   // v->addLayout( h1,1 );
   // v->addWidget( m_crossFader );
   // v->addLayout( h2,1 );
   // m_body->setLayout( v );

   // QVBoxLayout* mainLayout = new QVBoxLayout();
   // mainLayout->setContentsMargins(0,0,0,0);
   // mainLayout->setSpacing( 0 );
   // mainLayout->addWidget( m_menu );
   // mainLayout->addWidget( m_body,1 );
   // setLayout( mainLayout );

   // // m_soundtouch->setInputSignal( m_mixer );
   // m_leftFilter->setInputSignal( 0, m_leftPlayer );
   // m_leftLevel->setInputSignal( m_leftFilter );
   // m_leftTouch->setInputSignal( m_leftLevel );

   // m_rightFilter->setInputSignal( 0, m_rightPlayer );
   // m_rightLevel->setInputSignal( m_rightFilter );
   // m_rightTouch->setInputSignal( m_rightLevel );

   // m_crossFader->setInputSignal( 0, m_leftTouch );
   // m_crossFader->setInputSignal( 1, m_rightTouch );

   // m_leftFilter->setBypassButton( true );
   // m_leftFilter->setVisibleButton( false );

   // m_leftTouch->setBypassButton( true );
   // m_leftTouch->setVisibleButton( false );

   // m_rightFilter->setBypassButton( true );
   // m_rightFilter->setVisibleButton( false );

   // m_rightTouch->setBypassButton( true );
   // m_rightTouch->setVisibleButton( false );

   // m_menu->setInputSignal( 0, m_crossFader );

   connect( m_menu, SIGNAL(toggledBypass(bool)), this, SLOT(on_menuButton_bypass(bool)) );
   connect( m_menu, SIGNAL(toggledMore(bool)), this, SLOT(on_menuButton_more(bool)) );
   connect( m_menu, SIGNAL(toggledHideAll(bool)), this, SLOT(on_menuButton_hideAll(bool)) );

}

GSamplerDeck::~GSamplerDeck()
{
   // stopUpdateTimer();
}


void
GSamplerDeck::on_menuButton_bypass(bool checked)
{
   //m_isBypassed = checked;
}

void
GSamplerDeck::on_menuButton_more(bool checked)
{
   //m_qpeak->setVisible( checked );
   //m_volume->setVisible( checked );
}

void
GSamplerDeck::on_menuButton_hideAll(bool checked)
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
GSamplerDeck::readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
{
   return m_menu->readSamples( pts, dst, dstFrames, dstChannels, dstRate );
}
