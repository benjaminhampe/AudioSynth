#ifndef G_VST_DECK_HPP
#define G_VST_DECK_HPP

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

#include <de/audio/dsp/Mixer.hpp>
#include "GVstPlugin.hpp"

// Placed in the DSP chain.
// Controls volume of one incoming (stereo) audio signal only.
// ============================================================================
class GVstDeck : public QImageWidget, public de::audio::IDspChainElement
// ============================================================================
{
   Q_OBJECT
public:
   GVstDeck( QWidget* parent = 0 );
   ~GVstDeck();

   uint64_t readSamples( double pts, float* dst, uint32_t dstFrames,
               uint32_t dstChannels, uint32_t dstRate ) override;

signals:
public slots:
   void sendNote( de::audio::Note const & note ) override;
   void setVisibleButton(bool visible) { m_menu->setVisibleButton( visible ); }
   void setBypassButton(bool bypassed) { m_menu->setBypassButton( bypassed ); }
private slots:
   void on_menuButton_bypass(bool checked);
   void on_menuButton_more(bool checked);
   void on_menuButton_hideAll(bool checked);
//   void on_addedSynth( de::audio::IDspChainElement* );
//   void on_removedSynth( de::audio::IDspChainElement* );
protected:
private:
   DE_CREATE_LOGGER("GVstDeck")

   GDeck* m_menu;
   //QScrollArea* m_scrollArea;
   QImageWidget* m_body;

   GVstPlugin* m_vst1;
   GVstPlugin* m_vst2;
   GVstPlugin* m_vst3;
   GVstPlugin* m_vst4;
   GVstPlugin* m_vst5;

   std::vector< de::audio::IDspChainElement* > m_synths;
   de::audio::Mixer m_mixer;

};

#endif // G_OSZILLOSKOP_H
