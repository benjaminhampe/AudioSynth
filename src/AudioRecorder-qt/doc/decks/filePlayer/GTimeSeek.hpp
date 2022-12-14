#ifndef G_TIMESEEK_HPP
#define G_TIMESEEK_HPP

#include <QWidget>
#include <QTimerEvent>
#include <QMouseEvent>
#include <de/audio/buffer/Buffer.hpp>

// ============================================================================
class GTimeSeek : public QWidget
// ============================================================================
{
   Q_OBJECT
   DE_CREATE_LOGGER("GTimeSeek")
public:
   GTimeSeek( QWidget* parent = 0 );
   ~GTimeSeek() override;
signals:
   //void newSamples( float* samples, int channels, unsigned int frameCount );
public slots:
   void setBuffer( de::audio::Buffer* sampleBuffer );
   //void setSamples( float const* src, uint64_t frameCount, uint32_t channels, uint32_t sampleRate );
   void setPosition( float percent );
   void startUpdateTimer();
   void stopUpdateTimer();
   void updateImage();
protected:
   void resizeEvent( QResizeEvent* event ) override;
   void timerEvent( QTimerEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;
   void mousePressEvent( QMouseEvent* event ) override;
   void mouseReleaseEvent( QMouseEvent* event ) override;

private:
   QImage m_img;
   QString  m_uri;
   de::audio::Buffer* m_sampleBuffer;
   uint64_t m_position;
   float    m_percent;
//   int      _counter = 0;
//   int      _maxCount = 1024;
//   float    _timeStep = 0.0f;
//   bool     _aboutToStop = false;
   int      m_updateTimerID;
//   bool     _isDragging = false;
};

#endif // DSP_TIMESEEK_H
