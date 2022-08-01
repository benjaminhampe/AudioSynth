#ifndef G_MASTER_DECK_HPP
#define G_MASTER_DECK_HPP

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

#include <QImageWidget.hpp>
#include <QFont5x8.hpp>

#include <GVolume.hpp>
#include <GLevelMeter.hpp>
#include <GLowPass3.hpp>
#include <GSoundTouch.hpp>
#include <GOverdrive.hpp>
#include <GBassBoost.hpp>

#include <GOszilloskop.hpp>
#include <GSpektrometer.hpp>
#include <GDeck.hpp>

#include <GCompressor.hpp>
#include <GLimiter.hpp>
#include <GDelay.hpp>
#include <GPan.hpp>
#include <de/audio/dsp/IDspChainElement.hpp>

// Placed in the DSP chain.
// Controls volume of one incoming (stereo) audio signal only.
// ============================================================================
class GMasterDeck : public QImageWidget, public de::audio::IDspChainElement
// ============================================================================
{
   Q_OBJECT
public:
   GMasterDeck( QWidget* parent = 0 );
   ~GMasterDeck();

   uint64_t readSamples( double pts, float* dst, uint32_t dstFrames,
               uint32_t dstChannels, uint32_t dstRate ) override;

signals:
//   void newSamples( float* samples, uint32_t frameCount, uint32_t channels );

public slots:
   void clearInputSignals();
   void setInputSignal( int i, IDspChainElement* input );
   void setVisibleButton(bool visible) { m_menu->setVisibleButton( visible ); }
   void setBypassButton(bool bypassed) { m_menu->setBypassButton( bypassed ); }

private slots:
   void on_menuButton_bypass(bool checked);
   void on_menuButton_more(bool checked);
   void on_menuButton_hideAll(bool checked);
protected:
public:
   DE_CREATE_LOGGER("GMasterDeck")
   IDspChainElement* m_inputSignal;
   GDeck* m_menu;
   QImageWidget* m_body;

   //GSoundTouch* m_timeStretch;
   //GFilter* m_masterFilter;
   GDelay* m_delay;
   GPan* m_pan;
   GLowPass3* m_lowPass3;
   GOverdrive* m_overdrive;
   //GLowPass* m_masterLopass1;
   //GLowPass* m_masterLopass2;
   GLowPass3* m_lowPass4;
   //GLoPass* m_masterLopass4;
   GBassBoost* m_bassBoost;
   GVolume* m_volume;
   GLevelMeter* m_levelMeter;
   GOszilloskop* m_waveform;
   GSpektrometer* m_spektrum;

   GCompressor* m_compressor;
   GLimiter* m_limiter;
};

#endif // G_OSZILLOSKOP_H
