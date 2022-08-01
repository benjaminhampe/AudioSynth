#include "GMixerItem.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
using namespace de::audio;

GMixerItem::GMixerItem( QString title, QWidget* parent )
   : QWidget(parent)
   , m_updateTimerId( 0 )
   , m_volume( 100 )
   , m_inputSignal( nullptr )
{
   setObjectName( "GMixerItem" );
   setContentsMargins(0,0,0,0);

   m_title = new QLabel( title, this );
   m_slider = new QSlider( this );
   //m_slider->setMinimumHeight( 64 );
   m_slider->setMinimum( 0 );
   m_slider->setMaximum( 100 );
   m_slider->setValue( m_volume );
   m_amount = new QLabel( QString::number( m_volume ) + " %", this );

   QVBoxLayout* v = new QVBoxLayout();
   v->setContentsMargins(0,0,0,0);
   v->setSpacing( 3 );
   v->addWidget( m_title );
   v->addWidget( m_slider,1 );
   v->addWidget( m_amount );
   v->setAlignment( m_title, Qt::AlignHCenter );
   v->setAlignment( m_slider, Qt::AlignHCenter );
   v->setAlignment( m_amount, Qt::AlignHCenter );

   QHBoxLayout* h = new QHBoxLayout();
   h->setContentsMargins(0,0,0,0);
   h->setSpacing( 0 );
   h->addLayout( v );
   setLayout( h );

   m_inputBuffer.resize( 1024, 0.0f );

   connect( m_slider, SIGNAL(valueChanged(int)), this, SLOT(on_sliderChanged(int)), Qt::QueuedConnection );
}

GMixerItem::~GMixerItem()
{
   // stopUpdateTimer();
}

void
GMixerItem::setupDspElement( uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate ) 
{
   uint64_t dstSamples = dstFrames * dstChannels;
   DSP_RESIZE( m_inputBuffer, dstSamples );
}

uint64_t
GMixerItem::readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
{
   uint64_t dstSamples = dstFrames * dstChannels;
   if ( !m_inputSignal )
   {
      DSP_FILLZERO( dst, dstSamples );
      return dstSamples;
   }

   DSP_RESIZE( m_inputBuffer, dstSamples );
   DSP_FILLZERO( m_inputBuffer.data(), dstSamples );

   uint64_t retSamples = m_inputSignal->readSamples( pts, m_inputBuffer.data(), dstFrames, dstChannels, dstRate );
   if ( retSamples != dstSamples )
   {
      DE_WARN("retSamples != dstSamples")
   }

   float vol = 0.0001f * (m_volume * m_volume);
   DSP_COPY_SCALED( m_inputBuffer.data(), dst, dstSamples, vol );

/*
   L_min = std::numeric_limits< float >::max();
   L_max = std::numeric_limits< float >::lowest();
   R_min = std::numeric_limits< float >::max();
   R_max = std::numeric_limits< float >::lowest();

   if ( dstChannels > 1 )
   {
      for ( uint64_t i = 0; i < dstFrames; ++i )
      {
         float sample = dst[ i * dstChannels ];
         L_min = std::min( L_min, sample );
         L_max = std::max( L_max, sample );
      }
   }

   if ( dstChannels > 1 )
   {
      for ( uint64_t i = 0; i < dstFrames; ++i )
      {
         float sample = dst[ i * dstChannels + 1 ];
         R_min = std::min( R_min, sample );
         R_max = std::max( R_max, sample );
      }
   }
*/

   //DSP_COPY_SCALED( m_inputBuffer.data(), dst, dstSamples, vol );
   return dstSamples;
}

void
GMixerItem::on_volumeChanged( int v )
{
   m_volume = std::clamp( m_slider->value(), 0, 100 );
   m_amount->setText( QString::number( m_volume ) + " %" );
}

// void
// GMixerItem::setVolume( int volume_in_pc )
// {
   // m_volume = std::clamp( volume_in_pc, 0, 100 );
   // m_slider->setValue( m_volume );
   // m_amount->setText( QString::number( m_volume ) + " %" );
// }

void
GMixerItem::clearInputSignals()
{
   m_inputSignal = nullptr;
}

void
GMixerItem::setInputSignal( IDspChainElement* input )
{
   m_inputSignal = input;
}