#ifndef G_LOWPASS3_HPP
#define G_LOWPASS3_HPP

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

#include <QSlider>
#include <QDial>
#include <QLabel>
#include <QCheckBox>

#include "GSliderV.hpp"
#include "GGroupV.hpp"

// 2nd order biquad IIR lowpass ( IIR = uses output, which makes the response infinite )

// ============================================================================
class GLowPass3 : public QWidget, public de::audio::IDspChainElement
// ============================================================================
{
   Q_OBJECT
public:
   GLowPass3( QWidget* parent = 0 );
   ~GLowPass3();

   void
   computeCoefficients( uint32_t sampleRate, float freq, float q = 0.707071 );

   uint64_t
   readSamples( double pts,
                float* dst,
                uint32_t dstFrames,
                uint32_t dstChannels,
                uint32_t dstRate ) override;
signals:
public slots:
   void clearInputSignals();
   void setInputSignal( int i, de::audio::IDspChainElement* input );
   void setBypassed( bool bypassed );
   void setVisibleButton(bool visible) { m_menu->setVisibleButton( visible ); }
   //void setBypassButton(bool bypassed) {  }
private slots:
   void on_menuButton_bypass(bool checked);
   void on_menuButton_more(bool checked);
   void on_menuButton_hideAll(bool checked);
   void on_freqChanged( int v );
   void on_qpeakChanged( int v );
protected:
   DE_CREATE_LOGGER("GLowPass3")
   bool m_isBypassed;
   GGroupV* m_menu;
   GSliderV* m_freq;
   GSliderV* m_qpeak;
   GSliderV* m_volume;
   de::audio::IDspChainElement* m_inputSignal;
   std::vector< float > m_inputBuffer;

   // Coefficients and z^-1 delays for input and output ( IIR )
   // y[n] = b0*x[n] + b1*x[n-1] + b2*x[n-2] - a1*y[n-1] - a2*y[n-2]
   float m_b0;
   float m_b1;
   float m_b2;
   float m_a1;
   float m_a2;
   float m_x1L; // z^-1 delay register for input signal L
   float m_x2L; // z^-1 delay register for input signal L
   float m_y1L; // z^-1 delay register for output signal L
   float m_y2L; // z^-1 delay register for output signal L

   float m_x1R; // z^-1 delay register for input signal R
   float m_x2R; // z^-1 delay register for input signal R
   float m_y1R; // z^-1 delay register for output signal R
   float m_y2R; // z^-1 delay register for output signal R

};

#endif // G_LOPASS1_HPP
