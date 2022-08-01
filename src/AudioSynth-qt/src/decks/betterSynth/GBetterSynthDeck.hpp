#ifndef G_BETTER_SYNTH_DECK_HPP
#define G_BETTER_SYNTH_DECK_HPP

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
#include <QImageWidget.hpp>
#include <GVolume.hpp>
#include <GLevelMeter.hpp>
#include <GLowPass3.hpp>
#include <GSoundTouch.hpp>
#include <GOszilloskop.hpp>
#include <GSpektrometer.hpp>

#include <GDeck.hpp>
#include "GBetterVoice.hpp"

// Placed in the DSP chain.
// Controls volume of one incoming (stereo) audio signal only.
// ============================================================================
class GBetterSynthDeck : public QImageWidget, public de::audio::IDspChainElement
// ============================================================================
{
   Q_OBJECT
public:
   GBetterSynthDeck( QWidget* parent = 0 );
   ~GBetterSynthDeck();

   uint64_t readSamples( double pts, float* dst, uint32_t dstFrames,
               uint32_t dstChannels, uint32_t dstRate ) override;
   bool
   isSynth() const override { return true; }

signals:
//   void newSamples( float* samples, uint32_t frameCount, uint32_t channels );
public slots:
   void sendNote( de::audio::Note const & note ) override;
   void setVisibleButton(bool visible) { m_menu->setVisibleButton( visible ); }
   void setBypassButton(bool bypassed) { m_menu->setBypassButton( bypassed ); }
private slots:
   void on_menuButton_bypass(bool checked);
   void on_menuButton_more(bool checked);
   void on_menuButton_hideAll(bool checked);
protected:
private:
   DE_CREATE_LOGGER("GBetterSynthDeck")
   de::audio::BetterSynth m_synth;
   GDeck* m_menu;
   QImageWidget* m_body;

   GLowPass3* m_lowPass3;
   GLevelMeter* m_levelMeter;
   GVolume* m_volumeSlider;
   GOszilloskop* m_waveform;
};

#endif // G_OSZILLOSKOP_H
