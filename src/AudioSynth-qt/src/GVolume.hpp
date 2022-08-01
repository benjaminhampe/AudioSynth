#ifndef G_VOLUME_HPP
#define G_VOLUME_HPP

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
// Placed in the DSP chain.
// Controls volume of one incoming (stereo) audio signal only.
// ============================================================================
class GVolume : public QWidget, public de::audio::IDspChainElement
// ============================================================================
{
   Q_OBJECT
public:
   GVolume( QString label, QWidget* parent = 0 );
   ~GVolume();

   uint64_t readSamples( double pts, float* dst, uint32_t dstFrames,
               uint32_t dstChannels, uint32_t dstRate ) override;
signals:
//   void newSamples( float* samples, uint32_t frameCount, uint32_t channels );

public slots:
   void clearInputSignals();
   void setInputSignal( int i, de::audio::IDspChainElement* input );
   void setVolume( int volume_in_pc );
   //void setBypassed( bool enabled );
   //void setChannel( int channel = -1 );

private slots:
   void on_sliderChanged( int v );
//   void on_sliderPressed();
//   void on_sliderReleased();
protected:
   //void timerEvent( QTimerEvent* event) override;
   //void paintEvent( QPaintEvent* event ) override;

private:
   DE_CREATE_LOGGER("GVolume")
   de::audio::IDspChainElement* m_inputSignal;
   //bool m_isBypassed;
   int m_volume;
//   uint32_t m_channelCount;
//   int32_t m_channelIndex;
   int m_updateTimerId;
//   float L_min;
//   float L_max;
//   float R_min;
//   float R_max;

   QSlider* m_slider;
   QLabel* m_labelT;
   QLabel* m_labelB;
   QString m_origText;

   //std::vector< float > m_inputBuffer;
   //de::audio::ShiftBuffer< float > m_shiftBuffer;
//   std::vector< float > m_accumBuffer;
   QFont5x8 m_font5x8;
};

#endif // G_OSZILLOSKOP_H
