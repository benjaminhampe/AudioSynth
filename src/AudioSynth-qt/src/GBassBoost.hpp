#ifndef G_BASS_BOOST_HPP
#define G_BASS_BOOST_HPP

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
#include <QFont5x8.hpp>
#include "GSliderV.hpp"
#include "GGroupV.hpp"
#include <de/audio/dsp/LowPass_IIR_Biquad.hpp>

// ============================================================================
class GBassBoost : public QWidget, public de::audio::LowPass_IIR_Biquad
// ============================================================================
{
   Q_OBJECT
public:
   GBassBoost( QWidget* parent = 0 );
   ~GBassBoost();
signals:
public slots:
   void setBypassed( bool bypassed );
private slots:
   void on_menuButton_bypass(bool checked);
   void on_menuButton_more(bool checked);
   void on_menuButton_hideAll(bool checked);
   void on_freqChanged( int v );
   void on_qChanged( int v );
   void on_volumeChanged( int v );
protected:
   DE_CREATE_LOGGER("GBassBoost")
   GGroupV* m_menu;
   GSliderV* m_freq;
   GSliderV* m_qpeak;
   GSliderV* m_volume;

};

#endif // G_LOPASS1_HPP
