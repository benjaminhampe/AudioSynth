#ifndef G_DOUBLE_PLAYER_DECK_HPP
#define G_DOUBLE_PLAYER_DECK_HPP

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
#include <GCrossFader.hpp>
#include <GOszilloskop.hpp>
#include <GSpektrometer.hpp>

#include <GDeck.hpp>
#include "GFilePlayer.hpp"
#include <de/audio/dsp/IDspChainElement.hpp>

// Placed in the DSP chain.
// Controls volume of one incoming (stereo) audio signal only.
// ============================================================================
class GFilePlayerDeck : public QImageWidget, public de::audio::IDspChainElement
// ============================================================================
{
   Q_OBJECT
public:
   GFilePlayerDeck( QWidget* parent = 0 );
   ~GFilePlayerDeck();

   uint64_t readSamples( double pts, float* dst, uint32_t dstFrames,
               uint32_t dstChannels, uint32_t dstRate ) override;

signals:
//   void newSamples( float* samples, uint32_t frameCount, uint32_t channels );

public slots:
   void setVisibleButton(bool visible) { m_menu->setVisibleButton( visible ); }
   void setBypassButton(bool bypassed) { m_menu->setBypassButton( bypassed ); }
private slots:
   void on_menuButton_bypass(bool checked);
   void on_menuButton_more(bool checked);
   void on_menuButton_hideAll(bool checked);
protected:
private:
   DE_CREATE_LOGGER("GFilePlayerDeck")

   GDeck* m_menu;
   QImageWidget* m_body;

   GFilePlayer* m_leftPlayer;
   GLowPass3* m_leftFilter;
   GSoundTouch* m_leftTouch;
   GLevelMeter* m_leftLevel;

   //GOszilloskop* m_leftWaveform;
   //GSpektrometer* m_leftSpektrum;

   GFilePlayer* m_rightPlayer;
   GLowPass3* m_rightFilter;
   GSoundTouch* m_rightTouch;
   GLevelMeter* m_rightLevel;

   //GOszilloskop* m_rightWaveform;
   //GSpektrometer* m_rightSpektrum;
   GCrossFader* m_crossFader;

};

#endif // G_OSZILLOSKOP_H
