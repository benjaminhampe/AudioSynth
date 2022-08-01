#ifndef G_BETTER_VOICE_HPP
#define G_BETTER_VOICE_HPP

#include <QWidget>

//#include <QImage>
//#include <QTimer>
//#include <QPainter>
//#include <QPaintEvent>
//#include <QResizeEvent>
//#include <QMouseEvent>
//#include <QKeyEvent>
//#include <QThread>
//#include <QSlider>
//#include <QDial>
#include <QLabel>
#include <QComboBox>
//#include <QSpinBox>
#include <QDoubleSpinBox>

//#include <QScrollArea>

#include <QFont5x8.hpp>
#include "GDeck.hpp"
#include "GGroupV.hpp"
#include "GMultiOsc.hpp"
#include "GLFO.hpp"
#include "GADSR.hpp"
#include "GVolume.hpp"
#include "GLevelMeter.hpp"
//#include "GOszilloskop.hpp"
//#include "GSpektrometer.hpp"
//#include "GFilter.hpp"
//#include "GLowPass3.hpp"
//#include "GLoPass.hpp"
//#include "GDelay.hpp"
//#include "GSoundTouch.hpp"
//#include "GLoudnessLFO.hpp"

#include "BetterSynth.hpp"

// Placed in the DSP chain.
// Controls volume of one incoming (stereo) audio signal only.
// ============================================================================
class GBetterVoice : public QWidget
// ============================================================================
{
   Q_OBJECT
public:
   GBetterVoice( QWidget* parent = 0 );
   ~GBetterVoice();

   de::audio::BetterVoice*
   getVoice() const;

   void
   setVoice( de::audio::BetterVoice* voice );

signals:
public slots:
   void setTitleImageText( QString title ) { m_menu->setTitleImageText( title ); }
   void setBackgroundColor( QColor bgColor ) { m_menu->setBackgroundColor( bgColor ); }

private slots:
   void on_menuButton_bypass(bool);
   void on_menuButton_more(bool);
   void on_menuButton_hideAll(bool);
   void on_oscTypeChanged( int v );
   void on_coarseTuneChanged( int v );
   void on_fineTuneChanged( int v );
   void on_phaseChanged( int v );
   void on_volumeChanged( int v );

private:
   DE_CREATE_LOGGER("GBetterVoice")
   de::audio::BetterVoice* m_voice;

   GDeck* m_menu;
   QImageWidget* m_body;

   GMultiOsc* m_osc;
   GLFO* m_lfoPhase;
   GLFO* m_lfoPulseWidth;
   GLFO* m_lfoDetune;
   GADSR* m_adsr;

   //GLoudnessLFO* m_pianoLFO;
   //GDelay* m_delay;

//   GADSR* m_voiceADSR;
//   GLFO* m_voiceLFO;
//   GDelay* m_voiceDelay;
//   GOszilloskop* m_voiceOszilloskop;
//   GVolume* m_voiceVolume;
//   GLevelMeter* m_voiceLevelMeter;

};

#endif // G_OSZILLOSKOP_H
