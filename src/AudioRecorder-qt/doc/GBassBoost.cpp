#include "GBassBoost.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>

GBassBoost::GBassBoost( QWidget* parent )
   : QWidget(parent)
{
   setObjectName( "GBassBoost" );
   setContentsMargins(0,0,0,0);

   m_menu = new GGroupV("BASS BOOST", this);
   //m_freq = new GSliderV( "LowPass3", 60,0,10000,"Hz",1, this );
   m_qpeak = new GSliderV( "QPeak", 200,0,1000,"",1, this );
   m_volume = new GSliderV( "Vol.", 100,0,200,"%",1, this );

   //setFrequency( float( m_freq->value() ) );
   setFrequency( 60.0f );
   setQ( 0.01f * float( m_qpeak->value() ) );
   setVolume( m_volume->value() ); // Boost volume, can be over 100%, ca. 300%.
   setInputVolume( 100 ); // Boost means additional to original signal

   QHBoxLayout* h = new QHBoxLayout();
   h->setContentsMargins(0,0,0,0);
   h->setSpacing( 3 );
   h->addWidget( m_menu );
   //h->addWidget( m_freq );
   h->addWidget( m_qpeak );
   h->addWidget( m_volume );
   setLayout( h );

   connect( m_menu, SIGNAL(toggledBypass(bool)), this, SLOT(on_menuButton_bypass(bool)) );
   connect( m_menu, SIGNAL(toggledMore(bool)), this, SLOT(on_menuButton_more(bool)) );
   connect( m_menu, SIGNAL(toggledHideAll(bool)), this, SLOT(on_menuButton_hideAll(bool)) );

   //connect( m_freq, SIGNAL(valueChanged(int)), this, SLOT(on_freqChanged(int)), Qt::QueuedConnection );
   connect( m_qpeak, SIGNAL(valueChanged(int)), this, SLOT(on_qChanged(int)), Qt::QueuedConnection );
   connect( m_volume, SIGNAL(valueChanged(int)), this, SLOT(on_volumeChanged(int)), Qt::QueuedConnection );
}

GBassBoost::~GBassBoost()
{}

void
GBassBoost::setBypassed( bool bypassed )
{
   m_menu->setBypassButton( bypassed );
}

void
GBassBoost::on_menuButton_bypass(bool checked)
{
   m_isBypassed = checked;
}

void
GBassBoost::on_menuButton_more(bool checked)
{
   //m_qpeak->setVisible( checked );
   m_volume->setVisible( checked );
}

void
GBassBoost::on_menuButton_hideAll(bool checked)
{
   if ( checked )
   {
      m_qpeak->setVisible( false );
      m_volume->setVisible( false );
   }
   else
   {
      m_qpeak->setVisible( true );
      if ( m_menu->isMore() )
      {
         m_volume->setVisible( true );
      }
   }
}

void
GBassBoost::on_freqChanged( int v )
{
   //setFrequency( float( v ) );
}

void
GBassBoost::on_qChanged( int v )
{
   setQ( 0.01f * v );
}

void
GBassBoost::on_volumeChanged( int v )
{
   setVolume( v ); // Boost volume, can be over 100%, ca. 300%.
}
