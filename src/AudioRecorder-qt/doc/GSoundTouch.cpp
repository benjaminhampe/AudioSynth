#include "GSoundTouch.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
using namespace de::audio;

//uint32_t
//receiveSamples( soundtouch::SoundTouch & m_soundTouch, float* dst, float dstFrames )
//{
//      // Now the input file is processed, yet 'flush' few last samples that are
//   // hiding in the SoundTouch's internal processing pipeline.
//   m_SoundTouch.flush();
//   do
//   {
//      recvFrames = m_SoundTouch.receiveSamples( reinterpret_cast< float* >( chunk.data() ), chunk.getFrameCount() );
//      m_FrameIndex += recvFrames;
//      if (recvFrames < in.getFrameCount() )
//      {
//         DE_DEBUG("After Flush: recvFrames(",recvFrames,") < in.getFrameCount(",chunk.getFrameCount(),")")
//         chunk.m_FrameCount = recvFrames;
//      }
//      liste.append( chunk );
//   }
//   while (recvFrames != 0);
//}

GSoundTouch::GSoundTouch( QWidget* parent )
   : QWidget(parent)
   , m_isBypassed( false )
   , m_inputSignal( nullptr )
   , m_channelCount( 2 )
   , m_srcRate( 44100 )
   , m_dstRate( 48000 )
   , m_srcFrames( 1024 )
   , m_dstFrames( 1024 )
   , m_callCounter( 0 )
   , m_putFrames( 0 )
   , m_popFrames( 0 )
{
   setObjectName( "GSoundTouch" );

   bool debug = false;
   if ( debug )
   {
   DE_DEBUG("DEFAULT_AA_FILTER_LENGTH = ", m_soundTouch.getSetting(SETTING_AA_FILTER_LENGTH ) )
   DE_DEBUG("DEFAULT_SEQUENCE_MS = ", m_soundTouch.getSetting(SETTING_SEQUENCE_MS ) )
   DE_DEBUG("DEFAULT_SEEKWINDOW_MS = ", m_soundTouch.getSetting(SETTING_SEEKWINDOW_MS ) )
   DE_DEBUG("DEFAULT_OVERLAP_MS = ", m_soundTouch.getSetting(SETTING_OVERLAP_MS ) )
   }

   m_soundTouch.setSampleRate( 44100 );
   m_soundTouch.setChannels( 2 );
   m_soundTouch.setRate( 1.0f );
   m_soundTouch.setSetting(SETTING_USE_QUICKSEEK, 0 );
   m_soundTouch.setSetting(SETTING_USE_AA_FILTER, 1 );
   m_soundTouch.setSetting(SETTING_AA_FILTER_LENGTH,128 );
   m_soundTouch.setSetting(SETTING_SEQUENCE_MS,28 );
   m_soundTouch.setSetting(SETTING_SEEKWINDOW_MS, 28);
   m_soundTouch.setSetting(SETTING_OVERLAP_MS,80 );

   bool speechConfig = false;
   if (speechConfig)
   {
      // use settings for speech processing
      m_soundTouch.setSetting(SETTING_SEQUENCE_MS, 40);
      m_soundTouch.setSetting(SETTING_SEEKWINDOW_MS, 15);
      m_soundTouch.setSetting(SETTING_OVERLAP_MS, 8);
      if ( debug )
      {
      DE_DEBUG("Tune processing parameters for speech processing.")
      }
   }

   if ( debug )
   {
   DE_DEBUG("Final Setting:" )
   DE_DEBUG("SETTING_AA_FILTER_LENGTH = ", m_soundTouch.getSetting(SETTING_AA_FILTER_LENGTH ) )
   DE_DEBUG("SETTING_SEQUENCE_MS = ", m_soundTouch.getSetting(SETTING_SEQUENCE_MS ) )
   DE_DEBUG("SETTING_SEEKWINDOW_MS = ", m_soundTouch.getSetting(SETTING_SEEKWINDOW_MS ) )
   DE_DEBUG("SETTING_OVERLAP_MS = ", m_soundTouch.getSetting(SETTING_OVERLAP_MS ) )
   }

   // Read in Params first time.
   m_realTimeParams.quickSeek = (m_soundTouch.getSetting(SETTING_USE_QUICKSEEK ) != 0);
   m_realTimeParams.sequenceMs = m_soundTouch.getSetting(SETTING_SEQUENCE_MS);
   m_realTimeParams.seekWinMs = m_soundTouch.getSetting(SETTING_SEEKWINDOW_MS);
   m_realTimeParams.overlapMs = m_soundTouch.getSetting(SETTING_OVERLAP_MS);
   m_realTimeParams.aaFilterLen = m_soundTouch.getSetting(SETTING_AA_FILTER_LENGTH);
   if ( m_soundTouch.getSetting(SETTING_USE_AA_FILTER) == 0 )
   {
      m_realTimeParams.aaFilterLen = 0;
   }

   m_menu = new GGroupV("SoundTouch", this);
   m_tempo = new GSliderV( "Tempo", 0,-100, 100,"%",1, this );
   m_pitch = new GSliderV("Pitch", 0,-100, 100,"%",1, this );
   m_aafilter = new GSliderV("AA Filter",m_realTimeParams.aaFilterLen,0,128,"taps",8, this );
   m_sequenceMs = new GSliderV("SeqWin",m_realTimeParams.sequenceMs,0,100,"ms",1, this );
   m_seekWinMs = new GSliderV("SeekWin",m_realTimeParams.seekWinMs,0,100,"ms",1, this );
   m_overlapMs = new GSliderV("Overlap",m_realTimeParams.overlapMs,0,200,"ms",1, this );
   m_volume = new GSliderV("Gain",m_realTimeParams.volume,0,200,"%",1, this );

   auto h = new QHBoxLayout();
   h->setContentsMargins(0,0,0,0);
   h->setSpacing( 1 );
   h->addWidget( m_menu );
   h->addWidget( m_tempo );
   h->addWidget( m_pitch );
   h->addWidget( m_aafilter );
   h->addWidget( m_sequenceMs );
   h->addWidget( m_seekWinMs );
   h->addWidget( m_overlapMs );
   h->addWidget( m_volume );
   setLayout( h );

   connect( m_menu, SIGNAL(toggledBypass(bool)), this, SLOT(on_menuButton_bypass(bool)) );
   connect( m_menu, SIGNAL(toggledMore(bool)), this, SLOT(on_menuButton_more(bool)) );
   connect( m_menu, SIGNAL(toggledHideAll(bool)), this, SLOT(on_menuButton_hideAll(bool)) );
   //connect( m_rate, SIGNAL(valueChanged(int)), this, SLOT(on_rateSlider(int)) );
   connect( m_tempo, SIGNAL(valueChanged(int)), this, SLOT(on_tempoSlider(int)) );
   connect( m_pitch, SIGNAL(valueChanged(int)), this, SLOT(on_pitchSlider(int)) );
   connect( m_aafilter, SIGNAL(valueChanged(int)), this, SLOT(on_aalenSlider(int)) );
   connect( m_sequenceMs, SIGNAL(valueChanged(int)), this, SLOT(on_sequenceSlider(int)) );
   connect( m_seekWinMs, SIGNAL(valueChanged(int)), this, SLOT(on_seekwinSlider(int)) );
   connect( m_overlapMs, SIGNAL(valueChanged(int)), this, SLOT(on_overlapSlider(int)) );
   connect( m_volume, SIGNAL(valueChanged(int)), this, SLOT(on_volumeSlider(int)) );

   m_inputBuffer.resize( 8192, 0.0f );
   m_outputBuffer.resize( 8192, 0.0f );
}

GSoundTouch::~GSoundTouch()
{

}

void GSoundTouch::on_menuButton_bypass(bool checked)
{
   m_isBypassed = checked;
   //DE_DEBUG("m_isBypassed = ",m_isBypassed,", ",objectName().toStdString())
}

void GSoundTouch::on_menuButton_more(bool checked)
{
   m_aafilter->setVisible( checked );
   m_sequenceMs->setVisible( checked );
   m_seekWinMs->setVisible( checked );
   m_overlapMs->setVisible( checked );
   m_volume->setVisible( checked );
}

void GSoundTouch::on_menuButton_hideAll(bool checked)
{
   if ( checked )
   {
      m_tempo->setVisible( false );
      m_pitch->setVisible( false );
      m_aafilter->setVisible( false );
      m_sequenceMs->setVisible( false );
      m_seekWinMs->setVisible( false );
      m_overlapMs->setVisible( false );
      m_volume->setVisible( false );
   }
   else
   {
      m_tempo->setVisible( true );
      m_pitch->setVisible( true );
      if ( m_menu->isMore() )
      {
         m_aafilter->setVisible( true );
         m_sequenceMs->setVisible( true );
         m_seekWinMs->setVisible( true );
         m_overlapMs->setVisible( true );
         m_volume->setVisible( true );
      }
   }
}


void GSoundTouch::on_tempoSlider( int value )
{
   m_realTimeParams.virtualTempo = std::clamp( 1.0f + 0.01f * float(value ), 0.02f, 2.0f );
   DE_DEBUG("value = ",value,", virtualTempo = ",m_realTimeParams.virtualTempo)
}
void GSoundTouch::on_pitchSlider( int value )
{
   m_realTimeParams.virtualPitch = std::clamp( 1.0f + 0.01f * float(value ), 0.02f, 2.0f );
   DE_DEBUG("value = ",value,", virtualPitch = ",m_realTimeParams.virtualPitch)
}

void GSoundTouch::on_aalenSlider( int value )
{
   m_realTimeParams.aaFilterLen = 8 * (std::clamp( value, 0, 128 ) / 8);
   DE_DEBUG("value = ",value,", aaFilterLen = ",m_realTimeParams.aaFilterLen)
}
void GSoundTouch::on_sequenceSlider( int value )
{
   m_realTimeParams.sequenceMs = std::clamp( value, 0, 200 );
   DE_DEBUG("value = ",value,", sequenceMs = ",m_realTimeParams.sequenceMs)
}
void GSoundTouch::on_seekwinSlider( int value )
{
   m_realTimeParams.seekWinMs = std::clamp( value, 0, 200 );
   DE_DEBUG("value = ",value,", seekWinMs = ",m_realTimeParams.seekWinMs)
}
void GSoundTouch::on_overlapSlider( int value )
{
   m_realTimeParams.overlapMs = std::clamp( value, 0, 200 );
   DE_DEBUG("value = ",value,", overlapMs = ",m_realTimeParams.overlapMs)
}
void GSoundTouch::on_volumeSlider( int value )
{
   m_realTimeParams.volume = std::clamp( value, 0, 200 );
   DE_DEBUG("value = ",value,", volume = ",m_realTimeParams.volume)
}

namespace {

int powi( int base, int exponent )
{
   int k = base;
   for ( int i = 1; i < exponent; ++i )
   {
      k *= base;
   }
   return k;
}

} // end namespace

uint64_t
GSoundTouch::readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
{
   uint64_t dstSamples = dstFrames * dstChannels;

//   if ( objectName() == "MasterTimeStretch" )
//   {
//      DE_WARN("Hello MasterTimeStretch1")
//   }

   if ( !m_inputSignal )
   {
      DE_WARN("No input ",objectName().toStdString())
      DSP_FILLZERO( dst, dstSamples );
      return dstSamples;
   }

   if ( m_menu->isBypassed() )
   {
      //DE_WARN("bypassed ",objectName().toStdString())
      //m_soundTouch.flush();
      m_soundTouch.clear();
      m_callCounter = 0;
      m_putFrames = 0;
      m_popFrames = 0;
      m_inputSignal->readSamples( pts, dst, dstFrames, dstChannels, dstRate );
      return dstSamples;
   }

//   if ( objectName() == "MasterTimeStretch" )
//   {
//      DE_WARN("Hello MasterTimeStretch2")
//   }

   m_channelCount = dstChannels;
   m_dstRate = dstRate;

//=====================
// Update SoundTouch
//=====================
   m_soundTouch.setRate( m_realTimeParams.virtualRate );
   m_soundTouch.setTempo( m_realTimeParams.virtualTempo );
   m_soundTouch.setPitch( m_realTimeParams.virtualPitch );

   if ( m_realTimeParams.aaFilterLen > 0 )
   {
      auto enabled = (m_soundTouch.getSetting( SETTING_USE_AA_FILTER ) != 0);
      if ( !enabled )
      {
         m_soundTouch.setSetting( SETTING_USE_AA_FILTER, 1 );
      }

      auto old = m_soundTouch.getSetting( SETTING_AA_FILTER_LENGTH );
      if ( old != m_realTimeParams.aaFilterLen )
      {
         m_soundTouch.setSetting( SETTING_AA_FILTER_LENGTH, m_realTimeParams.aaFilterLen );
      }
   }
   else
   {
      auto enabled = (m_soundTouch.getSetting( SETTING_USE_AA_FILTER ) != 0);
      if ( enabled )
      {
         m_soundTouch.setSetting( SETTING_USE_AA_FILTER, 0 );
      }
   }

   auto sequenceMs = m_soundTouch.getSetting( SETTING_SEQUENCE_MS );
   if ( sequenceMs != m_realTimeParams.sequenceMs )
   {
      m_soundTouch.setSetting( SETTING_SEQUENCE_MS, m_realTimeParams.sequenceMs );
   }

   auto seekWinMs = m_soundTouch.getSetting( SETTING_SEEKWINDOW_MS );
   if ( seekWinMs != m_realTimeParams.seekWinMs )
   {
      m_soundTouch.setSetting( SETTING_SEEKWINDOW_MS, m_realTimeParams.seekWinMs );
   }

   auto overlapMs = m_soundTouch.getSetting( SETTING_OVERLAP_MS );
   if ( overlapMs != m_realTimeParams.overlapMs )
   {
      m_soundTouch.setSetting( SETTING_OVERLAP_MS, m_realTimeParams.overlapMs );
   }

//=====================
// Prepare buffers:
//=====================
   uint64_t srcFrames = 64;
   DSP_RESIZE( m_inputBuffer, srcFrames * dstChannels );
   DSP_RESIZE( m_outputBuffer, dstSamples );
   DSP_FILLZERO( m_outputBuffer );

//=====================
// Request 'dstFrames' frames from soundtouch to satisfy caller.
// feed more samples to soundtouch when we dont receive anything/enough, until done.
//=====================

   uint64_t loopCounter = 0;
   uint64_t dstCounter = dstFrames;

   auto pDst = dst;

   while ( dstCounter > 0 )
   {
      // [Receive]
      uint64_t popFrames = m_soundTouch.receiveSamples( m_outputBuffer.data(), dstCounter );
      m_popFrames += popFrames;

      // [Received something]
      if ( popFrames > 0 )
      {
         // [Received too many, information loss since we dont store it]
         if ( popFrames > dstCounter )
         {
             DE_WARN("popFrames(",popFrames,") > dstCounter(",dstCounter,")")
//            DSP_COPY( m_outputBuffer.data(), pDst, dstCounter * dstChannels );
//            pDst += dstCounter * dstChannels;
         }

         // [Write as much as we can]
         uint64_t popSamples = std::min( popFrames, uint64_t(dstFrames) ) * dstChannels;
         DSP_COPY( m_outputBuffer.data(), pDst, popSamples );
         pDst += popSamples;
      }
      // [Received not enough, ergo we need to feed the dragon]
      else if ( popFrames < dstCounter )
      {
         // Read input, dont care if too few input samples, we cleared the buffer.
         DSP_FILLZERO( m_inputBuffer );
         // Read input...
         uint64_t gotSamples = m_inputSignal->readSamples( pts, m_inputBuffer.data(), srcFrames, dstChannels, m_srcRate );
         uint64_t gotFrames = gotSamples / dstChannels;
         if ( gotFrames != srcFrames )
         {
            DE_WARN("gotFrames(",gotFrames,") != srcFrames(",srcFrames,")")
         }

         m_soundTouch.putSamples( m_inputBuffer.data(), gotFrames );
         m_putFrames += gotFrames;
      }

      dstCounter -= popFrames;

#if 0
      DE_WARN("call(",m_callCounter,"), "
               "loop(",loopCounter,"), "
               "m_putFrames(",m_putFrames,"), "
               "m_popFrames(",m_popFrames,"), "
               "popFrames(",popFrames,"), "
               "dstCounter(",dstCounter,")")
#endif
      loopCounter++;
   }

   m_callCounter++;
   //DE_DEBUG("pts(",pts,"), channels(",m_channelCount,"), loopCounter(",loopCounter,")")
//   DE_DEBUG("dstRate(",m_dstRate," Hz), dstFrames(",dstFrames,")")
//   DE_DEBUG("srcRate(",m_srcRate," Hz), srcFrames(",1024,")")
//   DE_DEBUG("m_virtualRate", m_virtualRate)
//   DE_DEBUG("m_rateChange",m_rateChange)
//   DE_DEBUG("rateChange",rateChange)

   float volume = 0.0001f * float( m_realTimeParams.volume * m_realTimeParams.volume );
   DSP_MUL( dst, dstSamples, volume );

   return dstSamples;
}

