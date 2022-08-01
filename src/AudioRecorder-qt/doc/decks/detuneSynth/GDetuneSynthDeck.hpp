#ifndef G_DETUNE_SYNTH_DECK_HPP
#define G_DETUNE_SYNTH_DECK_HPP

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
#include "GDetuneSynth.hpp"

#include <de/audio/dsp/Mixer.hpp>

// Placed in the DSP chain.
// Controls volume of one incoming (stereo) audio signal only.
// ============================================================================
class GDetuneSynthDeck : public QImageWidget, public de::audio::IDspChainElement
// ============================================================================
{
   Q_OBJECT
public:
   GDetuneSynthDeck( QWidget* parent = 0 );
   ~GDetuneSynthDeck();

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
   DE_CREATE_LOGGER("GDetuneSynthDeck")

   GDeck* m_menu;
   //QScrollArea* m_scrollArea;
   QImageWidget* m_body;
   std::array< GDetuneSynth*, 8 > m_synths;
   std::vector< float > m_inputBuffer;
   //std::vector< float > m_mixerBuffer;

   de::audio::Mixer m_mixer;

};

#endif // G_OSZILLOSKOP_H
