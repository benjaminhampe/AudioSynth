#ifndef G_DETUNE_VOICE_SYNTH_HPP
#define G_DETUNE_VOICE_SYNTH_HPP

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
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QScrollArea>
#include <QFont5x8.hpp>

#include "GDeck.hpp"
#include "GADSR.hpp"
#include "GLFO.hpp"
#include "GVolume.hpp"
#include "GLevelMeter.hpp"
#include "GOszilloskop.hpp"
#include "GSpektrometer.hpp"
//#include "GFilter.hpp"
#include "GLowPass3.hpp"
//#include "GLoPass.hpp"
//#include "GDelay.hpp"
//#include "GSoundTouch.hpp"
//#include "GLoudnessLFO.hpp"
#include "GGroupV.hpp"

#include "DetuneSynth.hpp"

// Placed in the DSP chain.
// Controls volume of one incoming (stereo) audio signal only.
// ============================================================================
class GDetuneSynth : public QWidget, public de::audio::IDspChainElement
// ============================================================================
{
   Q_OBJECT
public:
   GDetuneSynth( QWidget* parent = 0 );
   ~GDetuneSynth();

   //de::audio::ADSR const* getADSR() const { return &m_adsr; }
   //de::audio::ADSR* getADSR() { return &m_adsr; }
   //de::audio::LFOf const* getLFO() const { return &m_lfo; }
   //de::audio::LFOf* getLFO() { return &m_lfo; }
   bool
   isBypassed() const { return m_menu->isBypassed(); }
   //void
   //setBypassed( bool bypass );

   uint64_t
   readSamples(
      double pts,
      float* dst,
      uint32_t dstFrames,
      uint32_t dstChannels,
      uint32_t dstRate ) override;

   bool
   isSynth() const override { return true; }

signals:
public slots:
   void sendNote( de::audio::Note const & note ) override;
   void setTitleImageText( QString title ) { m_menu->setTitleImageText( title ); }
   void setBackgroundColor( QColor bgColor ) { m_menu->setBackgroundColor( bgColor ); }
private slots:
   void on_menuButton_bypass(bool);
   void on_menuButton_more(bool);
   void on_menuButton_hideAll(bool);
   void on_oscTypeChanged( int v );
//   void on_lfoPhaseTypeChanged( int v );
//   void on_lfoPulseWidthTypeChanged( int v );
//   void on_lfoDetuneTypeChanged( int v );

protected:
   //void timerEvent( QTimerEvent* event) override;
   //void paintEvent( QPaintEvent* event ) override;

private:
   DE_CREATE_LOGGER("GDetuneSynth")
   de::audio::DetuneSynth m_synth;

   std::vector< float > m_inputBuffer;

   GDeck* m_menu;
   QImageWidget* m_body;

   QLabel* m_oscLabel;
   QComboBox* m_oscEdit;
   QLabel* m_noteLabel;
   QSpinBox* m_noteEdit;
   QLabel* m_noteDetuneLabel;
   QSpinBox* m_noteDetuneEdit;
   GLFO* m_lfoPhase;
   GLFO* m_lfoPulseWidth;
   GLFO* m_lfoDetune;
   GADSR* m_adsr;
   GLowPass3* m_lowPass3;
   GLevelMeter* m_levelMeter;
   GVolume* m_volumeSlider;
   GOszilloskop* m_waveform;
   //GLoudnessLFO* m_pianoLFO;
   //GDelay* m_pianoDelay;

//   GADSR* m_voiceADSR;
//   GLFO* m_voiceLFO;
//   GDelay* m_voiceDelay;
//   GOszilloskop* m_voiceOszilloskop;
//   GVolume* m_voiceVolume;
//   GLevelMeter* m_voiceLevelMeter;

};

#endif // G_OSZILLOSKOP_H
