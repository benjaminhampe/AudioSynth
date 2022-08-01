#include "GDelay.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>

using namespace de::audio;

GDelay::GDelay( QWidget* parent )
    : QWidget(parent)
{
   setObjectName( "GDelay" );
   m_menu = new GGroupV("DELAY", this);
   m_delaySlider = new GSliderV( "TimeMs", 60,0,1000,"",1, this );
   m_volumeSlider = new GSliderV( "Gain", 85,0,200,"",1, this );

   QHBoxLayout* h = new QHBoxLayout();
   h->setContentsMargins(0,0,0,0);
   h->setSpacing( 3 );
   h->addWidget( m_menu );
   h->addWidget( m_delaySlider );
   h->addWidget( m_volumeSlider );
   setLayout( h );

   //setFrequency( float( m_freq->value() ) );
//   setFrequency( 60.0f );
//   setQ( 0.01f * float( m_qpeak->value() ) );
//   setVolume( m_volume->value() ); // Boost volume, can be over 100%, ca. 300%.
//   setInputVolume( 100 ); // Boost means additional to original signal

   connect( m_menu, SIGNAL(toggledBypass(bool)), this, SLOT(on_menuButton_bypass(bool)) );
   connect( m_menu, SIGNAL(toggledMore(bool)), this, SLOT(on_menuButton_more(bool)) );
   connect( m_menu, SIGNAL(toggledHideAll(bool)), this, SLOT(on_menuButton_hideAll(bool)) );
   connect( m_delaySlider, SIGNAL(valueChanged(int)), this, SLOT(on_delayChanged(int)), Qt::QueuedConnection );
   connect( m_volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(on_volumeChanged(int)), Qt::QueuedConnection );

   m_menu->setMoreButtonVisible( false );
}

GDelay::~GDelay()
{
}


void
GDelay::on_menuButton_bypass( bool checked )
{
   m_delay.setBypassed( checked );
}

void
GDelay::on_menuButton_more( bool checked )
{
   //m_delaySlider->setVisible( checked );
}

void
GDelay::on_menuButton_hideAll( bool checked )
{
   m_delaySlider->setVisible( !checked );
   m_volumeSlider->setVisible( !checked );
}

void
GDelay::on_delayChanged( int v )
{
   m_delay.setDelayInMs( v );
}

void
GDelay::on_volumeChanged( int v )
{
   m_delay.setVolume( v );
}

uint64_t
GDelay::readSamples( double pts,
                     float* dst,
                     uint32_t dstFrames,
                     uint32_t dstChannels,
                     uint32_t dstRate )
{
   return m_delay.readSamples( pts, dst, dstFrames, dstChannels, dstRate );
}
