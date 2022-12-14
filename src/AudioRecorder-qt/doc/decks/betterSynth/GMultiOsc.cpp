#include "GMultiOsc.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

using namespace de::audio;

GMultiOsc::GMultiOsc( QWidget* parent )
   : QWidget( parent )
   //, m_osc( nullptr )
{
   setObjectName( "GMultiOsc" );
   setContentsMargins(0,0,0,0);
   //setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

//   QStringList typeList;
//   //osc_types <<"Sine" << "Triangle" << "Rect" << "Ramp" << "Saw" << "Noise";

//   for ( int i = 0; i < int(de::audio::MultiOSCf::eTypeCount); ++i )
//   {
//      typeList << QString::fromStdString( de::audio::MultiOSCf::getFunctionString( i ) );
//   }

   m_menu = new GGroupV( "OSC", this );
   m_body = new QImageWidget( this );

   m_oscTypeLabel = new QLabel( "OSC", this );
   m_oscType = new QComboBox( this );

   for ( int i = 0; i < int(de::audio::MultiOSCf::eTypeCount); ++i )
   {
      auto s = QString::fromStdString( de::audio::MultiOSCf::getFunctionString( i ) );
      m_oscType->addItem( s, QVariant(i) );
   }

   //m_oscType->addItems( typeList );
   m_oscType->setCurrentIndex( 0 );

   m_coarseTuneLabel = new QLabel( "Note", this );
   m_coarseTune = new QSpinBox( this );
   m_coarseTune->setMinimum( -100 );
   m_coarseTune->setMaximum( 100 );
   m_coarseTune->setValue( 0 );
   m_coarseTune->setSingleStep( 1 );

   m_fineTuneLabel = new QLabel( "Detune", this );
   m_fineTune = new QSpinBox( this );
   m_fineTune->setMinimum( -100 );
   m_fineTune->setMaximum( 100 );
   m_fineTune->setValue( 0 );
   m_fineTune->setSingleStep( 1 );

   m_phaseLabel = new QLabel( "Phase", this );
   m_phase = new QSpinBox( this );
   m_phase->setMinimum( 0 );
   m_phase->setMaximum( 100 );
   m_phase->setValue( 0 );
   m_phase->setSingleStep( 1 );

   m_volume = new GSliderV("Vol.",85,0,100,"%",1,this);

   auto f = new QFormLayout();
   f->setContentsMargins( 0,0,0,0 );
   f->addRow( m_oscTypeLabel, m_oscType );
   f->addRow( m_coarseTuneLabel, m_coarseTune );
   f->addRow( m_fineTuneLabel, m_fineTune );
   f->addRow( m_phaseLabel, m_phase );

   auto h = new QHBoxLayout();
   h->setContentsMargins( 0,0,0,0 );
   h->addLayout( f );
   h->addWidget( m_volume );

   m_body->setLayout( h );

   auto h2 = new QHBoxLayout();
   h2->setContentsMargins( 0,0,0,0 );
   h2->addWidget( m_menu );
   h2->addWidget( m_body );
   setLayout( h2 );

   connect( m_menu, SIGNAL(toggledBypass(bool)), this, SLOT(on_menuButton_bypass(bool)) );
   connect( m_menu, SIGNAL(toggledMore(bool)), this, SLOT(on_menuButton_more(bool)) );
   connect( m_menu, SIGNAL(toggledHideAll(bool)), this, SLOT(on_menuButton_hideAll(bool)) );

   connect( m_oscType, SIGNAL(currentIndexChanged(int)), this, SLOT(on_oscTypeChanged(int)) );
   connect( m_coarseTune, SIGNAL(valueChanged(int)), this, SLOT(on_coarseTuneChanged(int)) );
   connect( m_fineTune, SIGNAL(valueChanged(int)), this, SLOT(on_fineTuneChanged(int)) );
   connect( m_phase, SIGNAL(valueChanged(int)), this, SLOT(on_phaseChanged(int)) );
   connect( m_volume, SIGNAL(valueChanged(int)), this, SLOT(on_volumeChanged(int)) );

   m_menu->setVisibleButton( false );
   m_menu->setBypassButton( true );
}

GMultiOsc::~GMultiOsc()
{
   // stopUpdateTimer();
}

//void
//GMultiOsc::setOSC( de::audio::MultiOSCf* osc )
//{
//   m_osc = osc;
//   if ( m_osc )
//   {
//      m_oscType->setCurrentIndex( m_osc->getFunction() );
//      m_phase->setValue( 100.0f * m_osc->getPhase() );
//      m_coarseTune->
//      //m_osc->setFrequency( getFrequencyFromMidiNote( m_coarseTune->value() + 0.01f * m_fineTune->value() ) );
//      //m_osc->setCenter( m_centerEdit->value() );
//      //m_osc->setRange( m_rangeEdit->value() );
//   }
//   else {
//      DE_ERROR("No osc")
//   }
//}
//   void setVolume( int volume_in_pc );
//void setChannel( int channel = -1 );

//void
//GMultiOsc::setValues( int type, double rate, double center, double range )
//{
//   m_typeEdit->setCurrentIndex( type );
//   m_rateEdit->setValue( rate );
//   m_centerEdit->setValue( center );
//   m_rangeEdit->setValue( range );
//}

void
GMultiOsc::on_menuButton_bypass(bool checked)
{
   //if ( m_osc ) m_osc->setBypassed( checked );
}

void
GMultiOsc::on_menuButton_more(bool checked)
{
}

void
GMultiOsc::on_menuButton_hideAll(bool checked)
{
   m_body->setVisible( !checked );
}

void
GMultiOsc::on_oscTypeChanged( int v )
{
//   if ( !m_osc ) return;
//   m_osc->setFunction( v );
   emit oscTypeChanged( m_oscType->itemData(v,Qt::UserRole).toInt() );
}

void
GMultiOsc::on_coarseTuneChanged( int v )
{
//   if ( !m_osc ) return;
//   float note = m_coarseTune->value();
//   float detune = 0.01f * m_fineTune->value();
//   m_osc->setFrequency( getFrequencyFromMidiNote( note + detune ) );

   emit coarseTuneChanged(v);
}

void
GMultiOsc::on_fineTuneChanged( int v )
{
//   if ( !m_osc ) return;
//   float note = m_coarseTune->value();
//   float detune = 0.01f * m_fineTune->value();
//   m_osc->setFrequency( getFrequencyFromMidiNote( note + detune ) );
   emit fineTuneChanged(v);
}

void
GMultiOsc::on_phaseChanged( int v )
{
//   if ( !m_osc ) return;
//   float phase = 0.01f * m_phase->value();
//   m_osc->setPhase( phase );
   emit phaseChanged(v);
}

void
GMultiOsc::on_volumeChanged( int v )
{
//   if ( !m_osc ) return;
//   m_osc->setVolume( v );
   emit volumeChanged(v);
}
