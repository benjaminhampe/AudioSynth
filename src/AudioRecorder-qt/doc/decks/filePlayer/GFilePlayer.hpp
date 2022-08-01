#ifndef G_FILE_TO_RAM_PLAYER_HPP
#define G_FILE_TO_RAM_PLAYER_HPP

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
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QFileDialog>

#include <de/audio/dsp/IDspChainElement.hpp>
#include <de/audio/buffer/BufferIO.hpp>
#include <QImageWidget.hpp>
#include "GTimeSeek.hpp"

// ============================================================================
class GFilePlayer : public QWidget, public de::audio::IDspChainElement
// ============================================================================
{
   Q_OBJECT
   DE_CREATE_LOGGER("GFilePlayer")
public:
   GFilePlayer( bool bRight, QWidget* parent = 0 );
   ~GFilePlayer();

   uint64_t
   readSamples(
      double pts,
      float* dst,
      uint32_t dstFrames,
      uint32_t dstChannels,
      uint32_t dstRate ) override;

   virtual uint32_t
   getInputSampleRate() const { return m_sampleBuffer.getSampleRate(); }

   enum eState
   {
      Unloaded = 0,
      Loading,
      Stopped,
      Playing,
      Paused,
      Looping,
      eStateCount
   };

   uint32_t getState() const;

   double getDuration() const;

   double getPosition() const;
signals:
public slots:
   void load();
   //void unload();
   void play();
   void stop();
   void pause();
   //void setPos();


private slots:
   void on_slider_pos( int );
   void on_clicked_load(bool checked);
   void on_clicked_play(bool checked);
   void on_clicked_stop(bool checked);

   // void stopUpdateTimer();
   // void startUpdateTimer( int ms = 67 );
   // void clearInputSignals();
   // void setInputSignal( de::audio::IDspChainElement* input );

protected:
   // void timerEvent( QTimerEvent* event) override;
   // void paintEvent( QPaintEvent* event ) override;

private:
   QLineEdit* m_edtUri;

   QImageWidget* m_coverArt;

   QPushButton* m_btnLoad;
   QPushButton* m_btnPlay;
   QPushButton* m_btnStop;

   QLabel* m_lblPosition;
   QSlider* m_sldPosition;
   QLabel* m_lblDuration;

   GTimeSeek* m_timeSeek;

   uint32_t m_state;
   uint64_t m_frameIndex;

   bool m_isLooping;
   int m_loopIndex;
   int m_loopCount;

   de::audio::Buffer m_sampleBuffer;
   std::string m_uri;
};

#endif // G_OSZILLOSKOP_H
