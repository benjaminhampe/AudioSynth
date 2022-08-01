#ifndef G_OSC_MULTI_HPP
#define G_OSC_MULTI_HPP

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
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>

#include <de/audio/dsp/MultiOSC.hpp> // LFO

#include "GGroupV.hpp"
#include "GSliderV.hpp"

// Placed in the DSP chain.
// Controls volume of one incoming (stereo) audio signal only.
// ============================================================================
class GMultiOsc : public QWidget
// ============================================================================
{
   Q_OBJECT
public:
   GMultiOsc( QWidget* parent = 0 );
   ~GMultiOsc();

signals:
   //void newSamples( float* samples, uint32_t frameCount, uint32_t channels );
   void oscTypeChanged(int);
   void coarseTuneChanged(int);
   void fineTuneChanged(int);
   void phaseChanged(int);
   void volumeChanged(int);
public slots:
   //void clearInputSignals();
   //void setInputSignal( de::audio::IDspChainElement* input );
   //void setBypassed( bool enabled );
   //void setVolume( int volume_in_pc );
   //void setChannel( int channel = -1 );
   //void setOSC( de::audio::MultiOSCf* osc );
   //void setValues( int type, double rate, double center, double range = 0.0 );
   void setVisibleButton(bool visible) { m_menu->setVisibleButton( visible ); }
   void setBypassButton(bool bypassed) { m_menu->setBypassButton( bypassed ); }
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
   DE_CREATE_LOGGER("GMultiOsc")
public:
   //de::audio::MultiOSCf* m_osc;
   GGroupV* m_menu;
   QImageWidget* m_body;

   QLabel* m_oscTypeLabel;
   QLabel* m_coarseTuneLabel;
   QLabel* m_fineTuneLabel;
   QLabel* m_phaseLabel;

   QComboBox* m_oscType;
   QSpinBox* m_coarseTune;
   QSpinBox* m_fineTune;
   QSpinBox* m_phase;
   GSliderV* m_volume;
};

#endif // G_OSZILLOSKOP_H
