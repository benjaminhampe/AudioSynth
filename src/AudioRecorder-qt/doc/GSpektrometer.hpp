#ifndef G_SPEKTRUM_HPP
#define G_SPEKTRUM_HPP

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

#ifdef _IRR_LINUX_PLATFORM_
#include <fftw3.h>
#else
#include <fftw3/fftw3.h>
#endif

#include <de_kissfft.hpp>

// ============================================================================
class GSpektrometer : public QWidget, public de::audio::IDspChainElement
// ============================================================================
{
   Q_OBJECT
   DE_CREATE_LOGGER("GSpektrometer")
public:
   GSpektrometer( QWidget* parent = 0 );
   ~GSpektrometer();

   void setFFTSize( uint32_t bufferSize );

   uint64_t readSamples( double pts, float* dst, uint32_t dstFrames,
               uint32_t dstChannels, uint32_t dstRate ) override;
signals:
//   void newSamples( float* samples, uint32_t frameCount, uint32_t channels );

public slots:

   void stopUpdateTimer()
   {
      if ( m_updateTimerId )
      {
         killTimer( m_updateTimerId );
         m_updateTimerId = 0;
      }
   }

   void startUpdateTimer()
   {
      stopUpdateTimer();
      m_updateTimerId = startTimer( 67, Qt::CoarseTimer );
   }

   void clearInputSignals()
   {
      m_inputSignal = nullptr;
   }

   void setInputSignal( de::audio::IDspChainElement* input )
   {
      m_inputSignal = input;
   }

protected:
   void updateImage( int w, int h );
   void hideEvent( QHideEvent* event ) override;
   void showEvent( QShowEvent* event ) override;
   void timerEvent( QTimerEvent* event) override;
   void paintEvent( QPaintEvent* event ) override;

private:
   de::audio::IDspChainElement* m_inputSignal;
   uint32_t m_channelCount;
   uint32_t m_sampleRate;

   bool m_isPlaying;
   int m_updateTimerId;
   float L_min;
   float L_max;
   float R_min;
   float R_max;

   QImage m_img;
   de::audio::ShiftBuffer m_shiftBuffer;
   std::vector< float > m_inputBuffer;
   std::vector< float > m_outputBuffer; // in deciBel dB

#if 1
   de::KissFFT m_fft;
#else
   uint32_t m_fftSize;
   std::string m_fftWisdomFile;
   double* m_fftInput;
   //std::vector< double > m_fftOutput; // complex output-vector
   fftw_complex* m_fftOutput; // complex output ( needs malloc() )
   fftw_plan m_fftPlan;
#endif

   QFont5x8 m_font5x8;
};

#endif // G_OSZILLOSKOP_H
