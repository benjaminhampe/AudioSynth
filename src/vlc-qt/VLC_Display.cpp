#include <VLC_Display.hpp>
#include <QPainter>
#include <QSurfaceFormat>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLExtraFunctions>
#include <QDebug>

VLC_Display::VLC_Display( QWidget* parent )
   : QWidget( parent, Qt::NoDropShadowWindowHint )
   , m_Driver( nullptr )
   , m_TimerId( 0 )
   , m_IsMouseMoving( false )
   , m_IsLeftPressed( false )
   , m_IsRightPressed( false )
   , m_IsMiddlePressed( false )
//   , m_MoveSpeed( 1.1f )
//   , m_StrafeSpeed( 2.1f )
//   , m_TurnSpeed( 1.1f )
//   , m_MouseTurnSpeedX( 0.01f )
//   , m_MouseTurnSpeedY( 0.01f )
   , m_Player( nullptr )
{
   // setBackgroundRole( QPalette::NoRole );
   setMinimumSize( 320, 240 );
   setFocusPolicy( Qt::StrongFocus );
   setMouseTracking( true );

   for ( auto & keyState : m_KeyStates )
   {
      keyState = false;
   }

   auto winW = width();
   auto winH = height();
   auto winHandle = uint64_t( winId() );
   m_Driver = de::gpu::createVideoDriver( winW, winH, winHandle );
   if ( m_Driver )
   {
      setFpsTimer( 30 );
      m_Driver->setClearColor( glm::vec4( 0.1f, 0.1f, 0.1f, 1.f ) ); // dark grey
   }

   std::string fontDir = "../../media/fonts/";
   de::addFontFamily( fontDir + "arial.ttf", de::Font("arial", 32 ) );
   de::addFontFamily( fontDir + "fontawesome463.ttf", de::Font("fa", 32 ) );
   de::addFontFamily( fontDir + "la-brands-400.ttf", de::Font("fa-brands", 64 ) );
   de::addFontFamily( fontDir + "la-regular-400.ttf", de::Font("fa-regular", 16 ) );
   de::addFontFamily( fontDir + "la-brands-400.ttf", de::Font("fa-brands", 64 ) );

}

VLC_Display::~VLC_Display()
{
   if ( m_TimerId )
   {
      killTimer( m_TimerId );
      m_TimerId = 0;
   }

   if ( m_Driver )
   {
      m_Driver->close();
      delete m_Driver;
      m_Driver = nullptr;
   }
}

de::gpu::IVideoDriver*
VLC_Display::getVideoDriver() const
{
   return m_Driver;
}

void
VLC_Display::setPlayer( de::vlc::Player* player )
{
   m_Player = player;
}

void
VLC_Display::render()
{
   if ( !m_Driver ) { return; } // No driver.
   int w = m_Driver->getScreenWidth();
   int h = m_Driver->getScreenHeight();
   if ( w < 1 || h < 1 ) { return; } // No screen.

   m_Driver->beginRender();
   emit frameStarted();

   if ( m_Player )
   {
      m_Player->render( m_Driver );
   }

   //m_Driver->getSceneManager()->render();    // draw 3d scene
   //m_Driver->getGUIEnvironment()->render();  // draw gui on top

   m_Driver->endRender();
   emit frameFinished();
}


void
VLC_Display::setFpsTimer( int ms )
{
   if ( ms < 1 )
   {
      ms = 1;
   }

   if ( m_TimerId )
   {
      killTimer( m_TimerId );
   }

   m_TimerId = startTimer( ms, Qt::PreciseTimer );
}

void
VLC_Display::resizeEvent( QResizeEvent* event )
{
   if ( !m_Driver ) { return; } // No driver.
   int w = event->size().width();
   int h = event->size().height();
   m_Driver->resize( w, h );
   emit_canvasResized();
   // update(); // Seems to create a cycle ( infinite loop )
}

void
VLC_Display::timerEvent( QTimerEvent* event )
{
   if ( event->timerId() == m_TimerId )
   {
      render();
   }
}

void
VLC_Display::paintEvent( QPaintEvent* event )
{
   render();
}

void
VLC_Display::emit_canvasResized()
{
   // qDebug() << "Emit CanvasResized";
   emit canvasResized();
}

void
VLC_Display::mouseMoveEvent( QMouseEvent* event )
{
   m_MousePos = glm::ivec2( event->x(), event->y() ); // current mouse pos

   if ( m_MousePos != m_MousePosLast ) // Some button is clicked...
   {
      m_MouseMove = m_MousePos - m_MousePosLast; // current mouse pos

      m_MousePosLast = m_MousePos;
   }
   else
   {
      m_MouseMove = { 0,0 };
   }

//   int mx = m_MouseMove.x;
//   int my = m_MouseMove.y;
}

void
VLC_Display::mousePressEvent( QMouseEvent* event )
{
   m_MousePos = glm::ivec2( event->x(), event->y() ); // current mouse pos
   m_MouseMove = { 0,0 };
   m_IsLeftPressed = event->button() == Qt::LeftButton;
   m_IsRightPressed = event->button() == Qt::RightButton;
   m_IsMiddlePressed = event->button() == Qt::MiddleButton;
}

void
VLC_Display::mouseReleaseEvent( QMouseEvent* event )
{
   m_MousePos = glm::ivec2( event->x(), event->y() ); // current mouse pos
   m_MouseMove = { 0,0 };
   if ( event->button() == Qt::LeftButton )
   {
      m_IsLeftPressed = false;
   }
   if ( event->button() == Qt::RightButton )
   {
      m_IsRightPressed = false;
   }
   if ( event->button() == Qt::MiddleButton )
   {
      m_IsMiddlePressed = false;
   }
}


void
VLC_Display::wheelEvent( QWheelEvent* event )
{
   // QWidget::wheelEvent( event );

   //QPoint delta = event->pixelDelta();
   //float wheel = dbSqrt( delta.x()*delta.x() + delta.y()*delta.y() );

   //QPoint delta = event->angleDelta() / 8;
   //float wheel = delta.y();

//   QPoint numDegrees = event->angleDelta() / 8;
//   if ( !numPixels.isNull() )
//   {
//      scrollWithPixels(numPixels);
//   }
//   else if ( !numDegrees.isNull() )
//   {
//      QPoint numSteps = numDegrees / 15;
//      scrollWithDegrees(numSteps);
//   }

//   event->accept();
//   event->delta()

}

void
VLC_Display::keyPressEvent( QKeyEvent* event )
{
/*
   auto key = event->key();
   if ( key == Qt::Key_Up )   { m_KeyStates[ de::KeyCode::KEY_UP ] = true; }
   if ( key == Qt::Key_Down ) { m_KeyStates[ de::KeyCode::KEY_DOWN ] = true; }
   if ( key == Qt::Key_Left ) { m_KeyStates[ de::KeyCode::KEY_LEFT ] = true; }
   if ( key == Qt::Key_Right ){ m_KeyStates[ de::KeyCode::KEY_RIGHT ] = true; }
   if ( key == Qt::Key_W ) { m_KeyStates[ de::KeyCode::KEY_W ] = true; }
   if ( key == Qt::Key_A ) { m_KeyStates[ de::KeyCode::KEY_A ] = true; }
   if ( key == Qt::Key_S ) { m_KeyStates[ de::KeyCode::KEY_S ] = true; }
   if ( key == Qt::Key_D ) { m_KeyStates[ de::KeyCode::KEY_D ] = true; }
   if ( key == Qt::Key_Q ) { m_KeyStates[ de::KeyCode::KEY_Q ] = true; }
   if ( key == Qt::Key_E ) { m_KeyStates[ de::KeyCode::KEY_E ] = true; }
   if ( key == Qt::Key_C ) { m_KeyStates[ de::KeyCode::KEY_C ] = true; }
   if ( key == Qt::Key_R ) { m_KeyStates[ de::KeyCode::KEY_R ] = true; }
   if ( key == Qt::Key_Shift ) { m_KeyStates[ de::KeyCode::KEY_SHIFT ] = true; }
   if ( key == Qt::Key_Control ) { m_KeyStates[ de::KeyCode::KEY_CONTROL ] = true; }
*/
}

void
VLC_Display::keyReleaseEvent( QKeyEvent* event )
{
//   if ( m_Node )
//   {
//      if ( event->key() == Qt::Key_F1 )
//      {
//         auto dd = m_Node->getDebugData();
//         dd.flags = de::gpu::VisualDebugData::NONE;
//         m_Node->setDebugData( dd );
//      }
//      // Enable all
//      else if ( event->key() == Qt::Key_F2 )
//      {
//         auto dd = m_Node->getDebugData();
//         dd.toggle( de::gpu::VisualDebugData::ALL );
//         m_Node->setDebugData( dd );
//      }
//      // Toggle main mesh bbox
//      else if ( event->key() == Qt::Key_F3 )
//      {
//         auto dd = m_Node->getDebugData();
//         dd.toggle( de::gpu::VisualDebugData::BBOX );
//         m_Node->setDebugData( dd );
//      }
//      // Toggle sub meshbuffer bboxes
//      else if ( event->key() == Qt::Key_F4 )
//      {
//         auto dd = m_Node->getDebugData();
//         dd.toggle( de::gpu::VisualDebugData::BBOXES );
//         m_Node->setDebugData( dd );
//      }
//      // Toggle normals
//      else if ( event->key() == Qt::Key_F5 )
//      {
//         auto dd = m_Node->getDebugData();
//         dd.toggle( de::gpu::VisualDebugData::NORMALS );
//         m_Node->setDebugData( dd );
//      }
//      // Toggle wireframe overlay
//      else if ( event->key() == Qt::Key_F6 )
//      {
//         auto dd = m_Node->getDebugData();
//         dd.toggle( de::gpu::VisualDebugData::WIREFRAME );
//         m_Node->setDebugData( dd );
//      }
//      // Toggle tangents and bitangents
//      else if ( event->key() == Qt::Key_F7 )
//      {
//         auto dd = m_Node->getDebugData();
//         dd.toggle( de::gpu::VisualDebugData::TANGENTS );
//         m_Node->setDebugData( dd );
//      }
//   }
}
