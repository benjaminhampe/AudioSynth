#ifndef G_LEVELMETER_HPP
#define G_LEVELMETER_HPP

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
#include <de/ColorGradient.hpp>
#include <de/audio/dsp/IDspChainElement.hpp>

// ============================================================================
class GLevelMeter : public QWidget, public de::audio::IDspChainElement
// ============================================================================
{
   Q_OBJECT
   DE_CREATE_LOGGER("GLevelMeter")
public:
   GLevelMeter( QWidget* parent = 0 );
   ~GLevelMeter();

   uint64_t readSamples( double pts, float* dst, uint32_t dstFrames,
               uint32_t dstChannels, uint32_t dstRate ) override;
signals:
public slots:
   void stopUpdateTimer();
   void startUpdateTimer( int ms = 37 );
   void clearInputSignals();
   void setInputSignal( de::audio::IDspChainElement* input );

protected:
   void timerEvent( QTimerEvent* event) override;
   void paintEvent( QPaintEvent* event ) override;

private:
   int m_updateTimerId;

   de::audio::IDspChainElement* m_inputSignal;
   //std::vector< float > m_inputBuffer;
   float m_Lmin;
   float m_Lmax;
   float m_Rmin;
   float m_Rmax;

//   std::vector< float > m_accumBuffer;
   QFont5x8 m_font5x8;
   de::LinearColorGradient m_ColorGradient;
};

#endif // G_OSZILLOSKOP_H
