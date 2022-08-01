#ifndef G_DELAY_HPP
#define G_DELAY_HPP


#include <QWidget>
#include <QImage>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QThread>

#include <QFont5x8.hpp>
#include <de/audio/dsp/IDspChainElement.hpp>
#include "GSliderV.hpp"
#include "GGroupV.hpp"

namespace de {
namespace audio {

// ============================================================================
struct Delay : public IDspChainElement
// ============================================================================
{
   DE_CREATE_LOGGER("de.audio.Delay")
public:
   IDspChainElement* m_inputSignal;
   bool m_isBypassed;
   int m_volume; // Volume in [pc]
   uint32_t m_sampleRate; // in [Hz] = [1/sec]
   float m_delay; // Delay in [sec]
   //std::vector< float > m_inputBuffer;
   ShiftBuffer m_delayBuffer;

   Delay()
      : m_inputSignal( nullptr )
      , m_isBypassed( false )
      , m_volume( 85 )
      , m_sampleRate( 48000 )
      , m_delay( 0.05f ) // 50ms
   {
      m_delayBuffer.resize( 48000 * 2 );
      uint32_t delayFrames = m_delay * m_sampleRate;
      m_delayBuffer.pushZero( delayFrames * 2 );
      //setVolume( m_volume->value() ); // Boost volume, can be over 100%, ca. 300%.
   }

   //~Delay() {}

   void
   clearInputSignals() { m_inputSignal = nullptr; }
   void
   setInputSignal( int i, IDspChainElement* input ) { m_inputSignal = input; }

   bool
   isBypassed() const { return m_isBypassed; }
   void
   setBypassed( bool enabled ) { m_isBypassed = enabled; }

   float
   getDelayInMs() const { return 1000.f * m_delay; }
   void
   setDelayInMs( float ms )
   {
      m_delay = 0.001f * ms;
      uint32_t delayFrames = m_delay * m_sampleRate;
      m_delayBuffer.clear();
      m_delayBuffer.pushZero( delayFrames * 2 );
      DE_DEBUG("DelayFrames = ", delayFrames, ", delay = ", m_delay)
   }

   int
   getVolume() const { return m_volume; }
   void
   setVolume( int vol ) { m_volume = std::clamp( vol, 0, 100 ); }

   uint64_t
   readSamples( double pts,
                float* dst,
                uint32_t dstFrames,
                uint32_t dstChannels,
                uint32_t dstRate ) override
   {
      m_sampleRate = dstRate;
      uint64_t dstSamples = dstFrames * dstChannels;

      //DSP_RESIZE( m_inputBuffer, dstSamples );

      if ( m_inputSignal )
      {
         m_inputSignal->readSamples( pts, dst, dstFrames, dstChannels, dstRate );
      }
      else
      {
         DSP_FILLZERO( dst, dstSamples );
      }

      if ( m_isBypassed )
      {
         return dstSamples;
      }

      m_delayBuffer.push( "GDelay", dst, dstSamples );

      float vol = 0.0001f * (m_volume * m_volume);

      auto delayData = m_delayBuffer.data();
      for ( size_t i = 0; i < dstFrames; i++ )
      {
         for ( size_t c = 0; c < dstChannels; c++ )
         {
            float delay = *delayData++;
            float sample = *dst;
            *dst = (vol * delay ) + sample; // fma
            dst++;
         }
      }

      return dstSamples;
   }

};

} // end namespace audio
} // end namespace de

// ============================================================================
class GDelay : public QWidget, public de::audio::IDspChainElement
// ============================================================================
{
   Q_OBJECT
   DE_CREATE_LOGGER("GDelay")
public:
   GDelay( QWidget* parent = 0 );
   ~GDelay();

   uint64_t
   readSamples( double pts,
                float* dst,
                uint32_t dstFrames,
                uint32_t dstChannels,
                uint32_t dstRate ) override;
signals:
public slots:
   void clearInputSignals() { m_delay.clearInputSignals(); }
   void setInputSignal( int i, de::audio::IDspChainElement* input ) { m_delay.setInputSignal( i, input ); }
   bool isBypassed() const { return m_delay.isBypassed(); }
   void setBypassed( bool enabled ) { m_menu->setBypassButton( enabled ); }

protected slots:
   void on_menuButton_bypass( bool checked );
   void on_menuButton_more( bool checked );
   void on_menuButton_hideAll( bool checked );
   void on_delayChanged( int v );
   void on_volumeChanged( int v );

protected:
//   void timerEvent( QTimerEvent* event) override;
//   void paintEvent( QPaintEvent* event ) override;

private:
   de::audio::Delay m_delay;

   GGroupV* m_menu;
   QWidget* m_body;
   GSliderV* m_delaySlider;
   GSliderV* m_volumeSlider;
   QFont5x8 m_font5x8;
};

#endif // G_OSZILLOSKOP_H
