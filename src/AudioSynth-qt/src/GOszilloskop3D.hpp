#ifndef DE_GPU_OSZILLOSKOP3D_QWIDGET_HPP
#define DE_GPU_OSZILLOSKOP3D_QWIDGET_HPP

#include <QWidget>
#include <QImage>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QKeyEvent>

#include "ShiftMatrix.hpp"
#include <de/audio/dsp/IDspChainElement.hpp>
#include <functional>

//#ifndef SHOW_3D_LINES
//#define SHOW_3D_LINES
//#endif

#ifndef SHOW_3D_TRIANGLES
#define SHOW_3D_TRIANGLES
#endif

#include <de_kissfft.hpp>

// =======================================================================
struct GOszilloskop3D : public QWidget, public de::audio::IDspChainElement
// =======================================================================
{
   Q_OBJECT
public:
   enum eRenderFlag
   {
      RenderNone = 0,
      RenderLines = 0x01,
      RenderTriangles = 0x02,
      RenderWaveform = 0x04,
      RenderSpektrum = 0x08,
      RenderXLog10 = 0x10,
      RenderFast = 0x20,
      eRenderFlagCount = 5
   };

   GOszilloskop3D( QWidget* parent );
   ~GOszilloskop3D() override;

   uint64_t
   readSamples( double pts, float* dst, uint32_t dstFrames,
               uint32_t dstChannels, uint32_t dstRate ) override;

signals:
public slots:
   void setupDspElement( uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate );
   void clearInputSignals();
   void setInputSignal( int i, de::audio::IDspChainElement* input );
private slots:
   void startFpsTimer();
   void stopFpsTimer();
protected:
   void render();
   void timerEvent( QTimerEvent* event) override;
   void paintEvent( QPaintEvent* event ) override;
   void resizeEvent( QResizeEvent* event ) override;
   void hideEvent( QHideEvent* event ) override;
   void showEvent( QShowEvent* event ) override;
   void keyPressEvent( QKeyEvent* event ) override;
   void keyReleaseEvent( QKeyEvent* event ) override;
   void mousePressEvent( QMouseEvent* event ) override;
   void mouseReleaseEvent( QMouseEvent* event ) override;
   void mouseMoveEvent( QMouseEvent* event ) override;
   void wheelEvent( QWheelEvent* event ) override;
protected:
   DE_CREATE_LOGGER("GOszilloskop3D")
   de::gpu::IVideoDriver* m_driver;
   de::gpu::ICamera m_camera;
   int m_fpsTimerId;
   uint32_t m_renderFlags;
   de::audio::IDspChainElement* m_inputSignal;
   uint32_t m_channelCount;
   uint32_t m_sampleRate;  // for fft
   uint32_t m_frameIndex;
   float L_min;
   float L_max;
   float R_min;
   float R_max;

   //std::vector< float > m_inputBuffer;
   //std::vector< float > m_channelBuffer;

   std::vector< float > m_inputBuffer;
   std::vector< float > m_channelBuffer;

#ifdef SHOW_3D_LINES
   ShiftMesh_LineStrips m_wavlinesL;
   ShiftMesh_LineStrips m_wavlinesR;
#endif

   float m_sizeX;
   float m_sizeY;
   float m_sizeZ;

#ifdef SHOW_3D_TRIANGLES
   ShiftMatrix< float > m_shiftMatrixL;
   ShiftMatrix< float > m_shiftMatrixR;

   de::gpu::SMeshBuffer m_trianglesL;
   de::gpu::SMeshBuffer m_trianglesR;

//   std::vector< float > m_inputBuffer;
//   std::vector< float > m_outputBuffer; // in deciBel dB
   de::audio::ShiftBuffer m_fftL;   // input collector
   de::audio::ShiftBuffer m_fftR;   // output collector
   std::vector< float > m_fftOutput;         // intermediary output, is fed to shiftMatrix.
   de::KissFFT m_fft;

//   de::gpu::SMeshBuffer m_wavtrisFrontL;
//   de::gpu::SMeshBuffer m_wavtrisFrontR;

#endif

   de::LinearColorGradient m_cgWaveform;
   de::LinearColorGradient m_cgSpektrum;

};

#endif

