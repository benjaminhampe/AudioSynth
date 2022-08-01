#ifndef G_MIXER_ITEM_HPP
#define G_MIXER_ITEM_HPP

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
class GMixerItem : public QWidget, public de::audio::IDspChainElement
// ============================================================================
{
   Q_OBJECT
public:
   GMixerItem( QString label, QWidget* parent = 0 );
   ~GMixerItem();

   uint64_t 
   readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate ) override;

   void
   setupDspElement( uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate );

signals:
//   void newSamples( float* samples, uint32_t frameCount, uint32_t channels );

public slots:
   void clearInputSignals();
   void setInputSignal( de::audio::IDspChainElement* input );

private slots:
   void on_volumeChanged( int v );
//   void stopUpdateTimer()
//   {
//      if ( m_updateTimerId )
//      {
//         killTimer( m_updateTimerId );
//         m_updateTimerId = 0;
//      }
//   }
//   void startUpdateTimer( int ms = 67 )
//   {
//      stopUpdateTimer();
//      if ( ms < 1 ) { ms = 1; }
//      m_updateTimerId = startTimer( ms, Qt::PreciseTimer );
//   }
protected:
   //void timerEvent( QTimerEvent* event) override;
   //void paintEvent( QPaintEvent* event ) override;
private:
   DE_CREATE_LOGGER("GMixerItem")
   QLabel* m_title;   
   QSlider* m_slider;
   QLabel* m_amount;
   int m_updateTimerId;
   int m_volume;
   de::audio::IDspChainElement* m_inputSignal;
   float L_min;
   float L_max;
   float R_min;
   float R_max;

   std::vector< float > m_inputBuffer;
   QFont5x8 m_font5x8;
};

#endif // G_OSZILLOSKOP_H
