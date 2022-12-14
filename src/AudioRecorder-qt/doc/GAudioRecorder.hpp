#ifndef G_AUDIO_RECORDER_HPP
#define G_AUDIO_RECORDER_HPP

#include <QWidget>
#include <QImage>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QThread>
#include <QPushButton>
#include <QImageWidget.hpp>
#include <QFont5x8.hpp>
#include <de/ColorGradient.hpp>
#include <de/audio/dsp/IDspChainElement.hpp>

#include <de/audio/buffer/BufferList.hpp>

#include <vector>

// ============================================================================
class GAudioRecorder : public QWidget, public de::audio::IDspChainElement
// ============================================================================
{
   Q_OBJECT
   DE_CREATE_LOGGER("GAudioRecorder")
public:
   GAudioRecorder( QWidget* parent = 0 );
   ~GAudioRecorder();

   uint64_t readSamples( double pts, float* dst, uint32_t dstFrames,
               uint32_t dstChannels, uint32_t dstRate ) override;
signals:
public slots:
   void clearInputSignals();
   void setInputSignal( int i, de::audio::IDspChainElement* input );

   void on_btnRecClear(bool);
   void on_btnRecStart(bool);
   void on_btnRecPause(bool);
   void on_btnRecStop(bool);
   void on_btnSaveRaw(bool);
   void on_btnSaveCompressed(bool);
protected:
   void stopUpdateTimer();
   void startUpdateTimer();
   void timerEvent( QTimerEvent* event) override;
//   void paintEvent( QPaintEvent* event ) override;

private:
   int m_updateTimerId;
   de::audio::IDspChainElement* m_inputSignal;
   std::vector< float > m_inputBuffer;
   QPushButton* m_btnRecClear;
   QPushButton* m_btnRecStart;
   QPushButton* m_btnRecPause;
   QPushButton* m_btnRecStop;
   QPushButton* m_btnSaveRaw;
   QPushButton* m_btnSaveCompressed;
   QImageWidget* m_imgWaveform;

   float m_Lmin;
   float m_Lmax;
   float m_Rmin;
   float m_Rmax;

//   std::vector< float > m_accumBuffer;
   QFont5x8 m_font5x8;
   de::LinearColorGradient m_ColorGradient;


   de::audio::BufferList m_bufferList;
};

#endif // G_OSZILLOSKOP_H
