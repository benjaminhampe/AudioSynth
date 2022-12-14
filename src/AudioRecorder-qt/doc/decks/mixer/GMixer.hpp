#ifndef G_MIXER_HPP
#define G_MIXER_HPP

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
#include <QSlider>
#include <QDial>
#include <QLabel>

#include <QFont5x8.hpp>
#include <de/audio/dsp/IDspChainElement.hpp>


// Placed in the DSP chain.
// Controls volume of one incoming (stereo) audio signal only.
// ============================================================================
class GMixer : public QWidget, public de::audio::IDspChainElement
// ============================================================================
{
   Q_OBJECT
public:
   GMixer( QWidget* parent = 0 );
   ~GMixer();

   uint64_t
   readSamples( double pts, float* dst, uint32_t dstFrames,
                uint32_t dstChannels, uint32_t dstRate ) override;
signals:
//   void newSamples( float* samples, uint32_t frameCount, uint32_t channels );

public slots:
   void clearInputSignals();
   void setInputSignal( int i, de::audio::IDspChainElement* input );

private slots:
   void on_volumeSlider( int v );

protected:
   //void timerEvent( QTimerEvent* event) override;
   //void paintEvent( QPaintEvent* event ) override;

private:
   DE_CREATE_LOGGER("GMixer")
   bool m_isBypassed;
   int m_updateTimerId;
   float L_min;
   float L_max;
   float R_min;
   float R_max;
   std::vector< float > m_inputBuffer;
   std::vector< float > m_mixerBuffer;
   QFont5x8 m_font5x8;

   struct MixerItem
   {
      de::audio::IDspChainElement* inputSignal;
      QLabel* label;
      QSlider* volume;
      QLabel* value;
      MixerItem()
         : inputSignal( nullptr )
         , label( nullptr )
         , volume( nullptr )
         , value( nullptr )
         {}
   };

   std::array< MixerItem, 8 > m_inputs;

};

#endif // G_OSZILLOSKOP_H
