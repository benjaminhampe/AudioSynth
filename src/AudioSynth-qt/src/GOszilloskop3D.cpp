#include "GOszilloskop3D.hpp"
#include "parseQtKey.hpp"
#include <QPainter>
#include <QSurfaceFormat>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLExtraFunctions>
#include <QDebug>

using namespace de::audio;

GOszilloskop3D::GOszilloskop3D( QWidget* parent )
   : QWidget( parent )
   , m_driver( nullptr )
   , m_fpsTimerId( 0 )
   , m_renderFlags( RenderTriangles | RenderSpektrum | RenderXLog10 ) //
   , m_inputSignal( nullptr )
   , m_channelCount( 2 )
   , m_sampleRate( 48000 )
   , m_sizeX( 2000.0f)
   , m_sizeY( 500.0f)
   , m_sizeZ( 20000.0f)
   , m_fft( 8*1024, false )
{
   setAttribute( Qt::WA_OpaquePaintEvent );
   setContentsMargins( 0,0,0,0 );
   setMinimumSize( 64, 64 );
   setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
   setFocusPolicy( Qt::StrongFocus );
   setMouseTracking( true );
   m_driver = de::gpu::createVideoDriver( width(), height(), uint64_t( winId() ) );
   m_driver->setActiveCamera( &m_camera );
   m_camera.setFOV( 90.0f );
   m_camera.lookAt( glm::vec3(25,514,-218), glm::vec3(25,507.5,-210) );
   m_cgWaveform.addStop( 0, de::RGBA(0,0,0) );
   m_cgWaveform.addStop( 0.1f, de::RGBA(255,255,255) );
   m_cgWaveform.addStop( 0.2f, de::RGBA(0,0,255) );
   m_cgWaveform.addStop( 0.3f, de::RGBA(0,20,0) );
   m_cgWaveform.addStop( 0.6f, de::RGBA(0,200,0) );
   m_cgWaveform.addStop( 0.8f, de::RGBA(255,255,0) );
   m_cgWaveform.addStop( 1.0f, de::RGBA(255,0,0) );
//   m_cgWaveform.addStop( 1.1f, de::RGBA(255,0,255) );
//   m_cgWaveform.addStop( 1.3f, de::RGBA(0,0,255) );
//   m_cgWaveform.addStop( 1.5f, de::RGBA(0,0,155) );
   m_cgWaveform.addStop( 2.5f, de::RGBA(205,105,5) );

   m_cgSpektrum.addStop( 0, de::RGBA(0,0,0) );
   m_cgSpektrum.addStop( 0.45, de::RGBA(125,125,125) );
   m_cgSpektrum.addStop( 0.5, de::RGBA(0,0,255) );
   m_cgSpektrum.addStop( 0.6, de::RGBA(0,200,0) );
   m_cgSpektrum.addStop( 0.8, de::RGBA(255,255,0) );
   m_cgSpektrum.addStop( 1.0, de::RGBA(255,0,0) );
   setupDspElement( 1024, 2, 48000 );
   startFpsTimer();
}

GOszilloskop3D::~GOszilloskop3D()
{
   stopFpsTimer();

   if ( m_driver )
   {
      m_driver->close();
      delete m_driver;
      m_driver = nullptr;
   }
}

void GOszilloskop3D::startFpsTimer()
{
   stopFpsTimer();
   m_fpsTimerId = startTimer( 32, Qt::CoarseTimer );
}

void GOszilloskop3D::stopFpsTimer()
{
   if ( m_fpsTimerId )
   {
      killTimer( m_fpsTimerId );
      m_fpsTimerId = 0;
   }
}

void GOszilloskop3D::clearInputSignals()
{
   m_inputSignal = nullptr;
}

void GOszilloskop3D::setInputSignal( int i, IDspChainElement* input )
{
   m_inputSignal = input;
}

void
GOszilloskop3D::setupDspElement( uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
{
   m_channelCount = dstChannels;
   m_sampleRate = dstRate;

   // Holds all interleaved input,
   // is then cut channelwise into m_channelBuffer
//   if ( m_inputBuffer.size() != dstFrames * dstChannels )
//   {
//      m_inputBuffer.resize( dstFrames * dstChannels );
//   }

   // Holds all frames of one single channel to be processed.
   // Is feed this mono way to shiftmatrices L and R.
   DSP_RESIZE( m_channelBuffer, dstFrames );

#ifdef SHOW_3D_TRIANGLES
   // New fft mode:
   if ( m_renderFlags & RenderSpektrum ) // Only in FFT mode
   {
      uint32_t n = 1024;   // The x size ( colums ) of shift matrices L+R

      if ( n != m_fft.size() ) // we need the shiftMatrix to have colums
      {
         n = m_fft.size(); // exactly of count fftSize for 2^x performance reasons.
      }

      // Input shift buffer in full fft size
      DSP_RESIZE( m_fftL, n );
      DSP_RESIZE( m_fftR, n );

      // temporary buffer for the fft output
      DSP_RESIZE( m_fftOutput, n );

      // Keeps only half of spektrum, half of fftSize
      m_shiftMatrixL.resize( n/8, 64 );
      m_shiftMatrixR.resize( n/8, 128 );
   }
   else
   {
      // Default waveform:
      auto n = std::max( dstFrames, uint32_t(256) );
      m_shiftMatrixL.resize( n, n );
      m_shiftMatrixR.resize( n, n );
   }

#endif
}


uint64_t
GOszilloskop3D::readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
{
   uint64_t dstSamples = dstFrames * dstChannels;
   if ( !m_inputSignal )
   {
      DSP_FILLZERO( dst, dstSamples );
      return dstSamples;
   }

   // Can be called once at engine start
   // Does only something if need to.
   setupDspElement( dstFrames, dstChannels, dstRate );

   // We have all buffers prepared lets read the input...
   m_inputSignal->readSamples( pts, dst, dstFrames, dstChannels, dstRate );
//   uint64_t retSamples = m_inputSignal->readSamples( pts,
//                         m_inputBuffer.data(), dstFrames, dstChannels, dstRate );
//   if ( retSamples != dstSamples )
//   {
//      DE_WARN("retSamples != dstSamples")
//   }

//   DSP_FILLZERO( m_inputBuffer );
//   DSP_FILLZERO( m_channelBuffer );

   if ( m_renderFlags & RenderSpektrum )
   {
      DSP_FUSE_STEREO_TO_MONO( m_channelBuffer.data(), dst, dstFrames, dstChannels );

      auto onFullR = [&] ()
      {
         // The shiftBuffer has collected enough samples for a new matrix row.
         // Matrix should have half fftSize in x direction == m_colCount
         // Reads and overwrites the front row in shift matrix only.
         // Bad: means we do more ffts than we would doing when rendering
         m_fft.setInput( m_fftR.data(), m_fft.size() );
         m_fft.fft();
         m_fft.getOutputInDecibel( m_fftOutput.data(), m_fft.size() );
         m_shiftMatrixR.push( m_fftOutput.data(), m_fft.size() / 8 );
      };
      m_fftR.push( "m_fftR", m_channelBuffer.data(), dstFrames, false, onFullR );
   }

#if 0
   // Feed L waveform
   if ( dstChannels > 0 )
   {
      DSP_GET_CHANNEL( m_channelBuffer.data(), dstFrames, dst, dstFrames, 0, dstChannels );
#ifdef SHOW_3D_LINES
      m_wavlinesL.push( m_channelBuffer.data(), dstFrames );
#endif
#ifdef SHOW_3D_TRIANGLES
      if ( m_renderFlags & RenderSpektrum )
      {
         auto onFullL = [&] ()
         {
            // The shiftBuffer has collected enough samples for a new matrix row.
            // Matrix should have half fftSize in x direction == m_colCount
            // Reads and overwrites the front row in shift matrix only.
            // Bad: means we do more ffts than we would doing when rendering
            m_fft.setInput( m_fftL.data(), m_fft.size() );
            m_fft.fft();
            m_fft.getOutputInDecibel( m_fftOutput.data(), m_fft.size() );
            m_shiftMatrixL.push( m_fftOutput.data(), m_fft.size() / 2 );
         };
         m_fftL.push( "m_fftL", m_channelBuffer.data(), dstFrames, false, onFullL );
      }
      else
      {
         m_shiftMatrixL.push( m_channelBuffer.data(), dstFrames );
      }
#endif
   }
   // Feed R waveform
   if ( dstChannels > 1 )
   {
      DSP_GET_CHANNEL( m_channelBuffer.data(), dstFrames, dst, dstFrames, 1, dstChannels );
#ifdef SHOW_3D_LINES
      m_wavlinesR.push( m_channelBuffer.data(), dstFrames );
#endif
#ifdef SHOW_3D_TRIANGLES
      if ( m_renderFlags & RenderSpektrum )
      {
         auto onFullR = [&] ()
         {
            // The shiftBuffer has collected enough samples for a new matrix row.
            // Matrix should have half fftSize in x direction == m_colCount
            // Reads and overwrites the front row in shift matrix only.
            // Bad: means we do more ffts than we would doing when rendering
            m_fft.setInput( m_fftR.data(), m_fft.size() );
            m_fft.fft();
            m_fft.getOutputInDecibel( m_fftOutput.data(), m_fft.size() );
            m_shiftMatrixR.push( m_fftOutput.data(), m_fft.size() / 2 );
         };
         m_fftR.push( "m_fftR", m_channelBuffer.data(), dstFrames, false, onFullR );
      }
      else
      {
         m_shiftMatrixR.push( m_channelBuffer.data(), dstFrames );
      }
#endif
   }
#endif

   //DSP_COPY( m_inputBuffer.data(), dst, dstSamples );
   return dstSamples;
}

void GOszilloskop3D::render()
{
   if ( !m_driver ) { return; } // No driver.
   int w = m_driver->getScreenWidth();
   int h = m_driver->getScreenHeight();
   if ( w < 1 || h < 1 ) { return; } // No screen.

   // int dBscale = 1;

#ifdef SHOW_3D_TRIANGLES
   if ( m_renderFlags & RenderSpektrum )
   {
      float sampleRate_over_fftSize = float( m_sampleRate / m_fft.size() );
      uint32_t xAxisScaleLog10 = (m_renderFlags & RenderXLog10) != 0;

//      createMatrixTriangleMesh( 0, m_trianglesL,
//         m_shiftMatrixL.m_data,
//         2*m_sizeX, m_sizeY, m_sizeZ, m_cgSpektrum, 1, sampleRate_over_fftSize, xAxisScaleLog10 );

      createMatrixTriangleMesh( 1, m_trianglesR,
         m_shiftMatrixR.m_data,
         2.0f*m_sizeX, m_sizeY, m_sizeZ, m_cgSpektrum, 1, sampleRate_over_fftSize, xAxisScaleLog10 );
   }
   else
   {
      float sampleRate_over_fftSize = 0.0f;

      createMatrixTriangleMesh( 0, m_trianglesL,
         m_shiftMatrixL.m_data,
         m_sizeX, m_sizeY, m_sizeZ, m_cgWaveform, 0, sampleRate_over_fftSize, 0 );

      createMatrixTriangleMesh( 1, m_trianglesR,
         m_shiftMatrixR.m_data,
         m_sizeX, m_sizeY, m_sizeZ, m_cgWaveform, 0, sampleRate_over_fftSize, 0 );
   }

//   createMatrixFrontTriangleMesh( 0,
//      m_wavtrisFrontL,
//      m_shiftMatrixL.m_data[0]->data(),
//      m_shiftMatrixL.m_data[0]->size(),
//      m_sizeX, m_sizeY, m_sizeZ, m_cgWaveform, expMode );

//   createMatrixFrontTriangleMesh( 1,
//      m_wavtrisFrontR,
//      m_shiftMatrixR.m_data[0]->data(),
//      m_shiftMatrixR.m_data[0]->size(),
//      m_sizeX, m_sizeY, m_sizeZ, m_cgWaveform, expMode );
#endif

   m_driver->setClearColor( 0.1f, 0.1f, 0.1f, 1.f ); // dark grey
   m_driver->beginRender();

   de::gpu::TRSd trs;

#ifdef SHOW_3D_TRIANGLES

//// Draw 'L' TriangleMatrix below
//   trs = de::gpu::TRSd();
//   trs.setTX( -(45.0f + m_sizeX) );
//   trs.setTZ( 0.0f );
//   trs.setRZ( -90.0f );
//   trs.setRX( 35.0f );
//   //trs.setSX( -1.0f );
//   m_driver->setModelMatrix( trs.getModelMatrix() );
//   m_driver->draw3D( m_trianglesL );

//// Draw 'R' TriangleMatrix below
//   trs = de::gpu::TRSd();
//   trs.setTX( 45.0f + m_sizeX );
//   trs.setTZ(  0.0f );
//   trs.setRZ( 90.0f );
//   trs.setRX( 35.0f );
//   m_driver->setModelMatrix( trs.getModelMatrix() );
//   m_driver->draw3D( m_trianglesR );

//// Draw 'L' TriangleFront
//   trs = de::gpu::TRSd();
//   trs.setTX( -15.0f );
//   trs.setTZ( -200.0f );
//   //trs.setSX( -1.0f );
//   m_driver->setModelMatrix( trs.getModelMatrix() );
//   m_driver->draw3D( m_wavtrisFrontL );

//// Draw 'R' TriangleFront
//   trs = de::gpu::TRSd();
//   trs.setTX( 15.0f );
//   trs.setTZ( -200.0f );
//   m_driver->setModelMatrix( trs.getModelMatrix() );
//   m_driver->draw3D( m_wavtrisFrontR );

   if ( m_renderFlags & RenderSpektrum )
   {
   // Draw 'R' TriangleMatrix below
      trs = de::gpu::TRSd();
      trs.setTX( -m_sizeX );
      trs.setTZ(  0.0f );
      m_driver->setModelMatrix( trs.getModelMatrix() );
      m_driver->draw3D( m_trianglesR );
   }
   else
   {
   // Draw 'L' TriangleMatrix below
      trs = de::gpu::TRSd();
      trs.setTX( -5.0f );
      trs.setTZ( 0.0f );
      m_driver->setModelMatrix( trs.getModelMatrix() );
      m_driver->draw3D( m_trianglesL );

   // Draw 'R' TriangleMatrix below
      trs = de::gpu::TRSd();
      trs.setTX( 5.0f );
      trs.setTZ(  0.0f );
      m_driver->setModelMatrix( trs.getModelMatrix() );
      m_driver->draw3D( m_trianglesR );

   }

#endif

#ifdef SHOW_3D_LINES

// Draw Lines
   int n = m_wavlinesL.m_data.size();
   float fx = 4.0f / float( n );
   float dx = m_wavlinesL.m_sizeX;
   float dy = m_wavlinesL.m_sizeY;
   float dz = m_wavlinesL.m_sizeZ / float( n );
   float tz = 0.0f;
   float sx = 5.0f;
   for ( int i = n-1; i >= 0; i-- )
   {
      de::gpu::TRSd trs;
      tz = (dz * i);
      trs.setTX( -10.0f );
      trs.setTZ( tz );
      trs.setSX( -sx );
      sx -= fx;
      //sz += 0.075f;
      m_driver->setModelMatrix( trs.getModelMatrix() );
      m_driver->draw3D( *m_wavlinesL.m_data[ i ] );
   }

   tz = 0.0f;
   sx = 5.0f;
   for ( int i = n-1; i >= 0; i-- )
   {
      de::gpu::TRSd trs;
      tz = (dz * i);
      trs.setTX( 10.0f );
      trs.setTZ( tz );
      trs.setSX( sx );
      sx -= fx;
      //sz += 0.075f;
      m_driver->setModelMatrix( trs.getModelMatrix() );
      m_driver->draw3D( *m_wavlinesR.m_data[ i ] );
   }

#endif

   //m_driver->getSceneManager()->render();

//   m_driver->resetModelMatrix();
//   m_driver->draw2DPerfOverlay5x8();
   m_driver->endRender();
}

void
GOszilloskop3D::hideEvent( QHideEvent* event )
{
   stopFpsTimer();
   //DE_DEBUG("")
   event->accept();
}

void
GOszilloskop3D::showEvent( QShowEvent* event )
{
   //DE_DEBUG("")
   startFpsTimer();
   event->accept();
}

void
GOszilloskop3D::timerEvent( QTimerEvent* event )
{
   if ( event->timerId() == m_fpsTimerId )
   {
      render();
   }
}

void
GOszilloskop3D::paintEvent( QPaintEvent* event )
{
   render();
}

void GOszilloskop3D::resizeEvent( QResizeEvent* event )
{
   if ( !m_driver ) { return; } // No driver.
   int w = event->size().width();
   int h = event->size().height();
   m_driver->resize( w, h );
   //emit_canvasResized();
   // update(); // Seems to create a cycle ( infinite loop )
}


void
GOszilloskop3D::mouseMoveEvent( QMouseEvent* event )
{
   if ( m_driver )
   {
      de::SEvent post;
      post.type = de::EET_MOUSE_EVENT;
      post.mouseEvent.m_Flags = de::SMouseEvent::Moved;
      post.mouseEvent.m_X = event->x();
      post.mouseEvent.m_Y = event->y();
      post.mouseEvent.m_Wheel = 0.0f;
      m_driver->postEvent( post );
   }
}

void
GOszilloskop3D::mousePressEvent( QMouseEvent* event )
{
   if ( m_driver )
   {
      de::SEvent post;
      post.type = de::EET_MOUSE_EVENT;
      post.mouseEvent.m_Flags = de::SMouseEvent::Pressed;
      post.mouseEvent.m_X = event->x();
      post.mouseEvent.m_Y = event->y();
      post.mouseEvent.m_Wheel = 0.0f;

      if ( event->button() == Qt::LeftButton )
      {
         post.mouseEvent.m_Flags |= de::SMouseEvent::BtnLeft;
      }
      else if ( event->button() == Qt::RightButton )
      {
         post.mouseEvent.m_Flags |= de::SMouseEvent::BtnRight;
      }
      else if ( event->button() == Qt::MiddleButton )
      {
         post.mouseEvent.m_Flags |= de::SMouseEvent::BtnMiddle;
      }
      m_driver->postEvent( post );

      // auto camera = m_driver->getActiveCamera();
      // if ( camera )
      // {
         // de::gpu::Ray3d ray = camera->computeRay( event->x(), event->y() );
         // auto a = ray.getPos() + ray.getDir() * 10.0;
         // auto b = ray.getPos() + ray.getDir() * 1000.0;
         // m_LineRenderer.add3DLine( a,b, de::randomColor() );
      // }

   }

   //m_MousePos = glm::ivec2( event->x(), event->y() ); // current mouse pos
}

void
GOszilloskop3D::mouseReleaseEvent( QMouseEvent* event )
{
   if ( m_driver )
   {
      de::SEvent post;
      post.type = de::EET_MOUSE_EVENT;
      post.mouseEvent.m_Flags = de::SMouseEvent::Released;
      post.mouseEvent.m_X = event->x();
      post.mouseEvent.m_Y = event->y();
      post.mouseEvent.m_Wheel = 0.0f;
      if ( event->button() == Qt::LeftButton )
      {
         post.mouseEvent.m_Flags |= de::SMouseEvent::BtnLeft;
      }
      else if ( event->button() == Qt::RightButton )
      {
         post.mouseEvent.m_Flags |= de::SMouseEvent::BtnRight;
      }
      else if ( event->button() == Qt::MiddleButton )
      {
         post.mouseEvent.m_Flags |= de::SMouseEvent::BtnMiddle;
      }
      m_driver->postEvent( post );
   }

   //m_MousePos = glm::ivec2( event->x(), event->y() ); // current mouse pos
   //m_MouseMove = { 0,0 };
}


void
GOszilloskop3D::wheelEvent( QWheelEvent* event )
{
   if ( m_driver )
   {
      de::SEvent post;
      post.type = de::EET_MOUSE_EVENT;
      post.mouseEvent.m_Flags = de::SMouseEvent::Wheel;
      post.mouseEvent.m_X = event->x();
      post.mouseEvent.m_Y = event->y();
      post.mouseEvent.m_Wheel = event->angleDelta().y(); //    //QPoint delta = event->pixelDelta();
      // DE_DEBUG("post.mouseEvent.m_Wheel = ",post.mouseEvent.m_Wheel)
      m_driver->postEvent( post );
   }

   //event->accept();
}

void
GOszilloskop3D::keyPressEvent( QKeyEvent* event )
{
   //DE_DEBUG("KeyPress(",event->key(),")")

   if ( m_driver )
   {
      de::SEvent post;
      post.type = de::EET_KEY_EVENT;
      post.keyEvent.Key = de::KEY_UNKNOWN;
      post.keyEvent.Flags = de::SKeyEvent::Pressed;
      if ( event->modifiers() & Qt::ShiftModifier ) { post.keyEvent.Flags |= de::SKeyEvent::Shift; }
      if ( event->modifiers() & Qt::ControlModifier ) { post.keyEvent.Flags |= de::SKeyEvent::Ctrl; }
      if ( event->modifiers() & Qt::AltModifier ) { post.keyEvent.Flags |= de::SKeyEvent::Alt; }
      parseQtKey( event, post );
      m_driver->postEvent( post );
   }

   event->accept();
}

void
GOszilloskop3D::keyReleaseEvent( QKeyEvent* event )
{
   //DE_DEBUG("KeyRelease(",event->key(),")")

   if ( m_driver )
   {
      de::SEvent post;
      post.type = de::EET_KEY_EVENT;
      post.keyEvent.Key = de::KEY_UNKNOWN;
      post.keyEvent.Flags = de::SKeyEvent::None;
      if ( event->modifiers() & Qt::ShiftModifier ) { post.keyEvent.Flags |= de::SKeyEvent::Shift; }
      if ( event->modifiers() & Qt::ControlModifier ) { post.keyEvent.Flags |= de::SKeyEvent::Ctrl; }
      if ( event->modifiers() & Qt::AltModifier ) { post.keyEvent.Flags |= de::SKeyEvent::Alt; }
      parseQtKey( event, post );
      m_driver->postEvent( post );
   }

   event->accept();
}


