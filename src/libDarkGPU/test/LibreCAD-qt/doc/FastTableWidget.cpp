#include "FastTableWidget.hpp"

/*

#include <QPainter>
#include <QSurfaceFormat>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLExtraFunctions>
//#include <QPropertyAnimation>
//#include <QPauseAnimation>
//#include <QSequentialAnimationGroup>
#include <QDebug>

void parseQtKey( QKeyEvent* event, de::SEvent & post );

void
FastTableWidget::setHorizontalHeaderItemText( int col, QString text )
{

}

void
FastTableWidget::setColumnCount( int cols )
{

}

void
FastTableWidget::setRowCount( int rows )
{

}

void
FastTableWidget::resize( int cols, int rows )
{

}


FastTableWidget::FastTableWidget( QWidget* parent )
   : QWidget( parent, Qt::NoDropShadowWindowHint )
   , m_Driver( nullptr )
   , m_TimerId( 0 )
   , m_ShowPerfOverlay( false )
   , m_ShowVisualDebug( false )
   , m_IsDragging( false )
   , m_IsMouseLeft( false )
   , m_IsMouseRight( false )
   , m_IsMouseMiddle( false )
{
   // setBackgroundRole( QPalette::NoRole );
   setMinimumSize( 320, 240 );
   setFocusPolicy( Qt::StrongFocus );
   setMouseTracking( true );
   startDriver();
   for ( auto & keyState : m_KeyStates ) { keyState = false; }
}

FastTableWidget::~FastTableWidget()
{
   closeDriver();
}

void
FastTableWidget::flipMeshX()
{
   de::gpu::SMeshTool::flipMeshX( m_Mesh );
   m_Mesh.recalculateNormals();
   m_Mesh.upload();
}
void
FastTableWidget::flipMeshY()
{
   de::gpu::SMeshTool::flipMeshY( m_Mesh );
   m_Mesh.recalculateNormals();
   m_Mesh.upload();
}
void
FastTableWidget::flipMeshZ()
{
   de::gpu::SMeshTool::flipMeshZ( m_Mesh );
   m_Mesh.recalculateNormals();
   m_Mesh.upload();
}
void
FastTableWidget::flipMeshNormals()
{
   de::gpu::SMeshTool::flipNormals( m_Mesh );
   m_Mesh.upload();
}
void
FastTableWidget::flipMeshXY()
{
   de::gpu::SMeshTool::flipMeshXY( m_Mesh );
   m_Mesh.recalculateNormals();
   m_Mesh.upload();
}
void
FastTableWidget::flipMeshYZ()
{
   de::gpu::SMeshTool::flipMeshYZ( m_Mesh );
   m_Mesh.recalculateNormals();
   m_Mesh.upload();
}
void
FastTableWidget::flipMeshXZ()
{
   de::gpu::SMeshTool::flipMeshXZ( m_Mesh );
   m_Mesh.recalculateNormals();
   m_Mesh.upload();
}


void
FastTableWidget::initScene()
{

}

void
FastTableWidget::clearScene()
{

}

void
FastTableWidget::startDriver()
{
   if ( m_Driver ) return; // Already open;
   int w = width();
   int h = height();
   uint64_t windowHandle = uint64_t( winId() );
   m_Driver = de::gpu::createVideoDriver( w,h,windowHandle );
   if ( !m_Driver ) return; // No driver
   // int vsync = 1; // Limit to ScreenRefreshRate
   setFpsTimer( 30 );
   m_Driver->setActiveCamera( &m_Camera );
   m_Driver->setClearColor( 0.1f, 0.1f, 0.1f, 1.f ); // dark grey
   initScene();
}

void
FastTableWidget::closeDriver()
{
   if ( m_TimerId )
   {
      killTimer( m_TimerId );
      m_TimerId = 0;
   }

   clearScene();

   if ( m_Driver )
   {
      m_Driver->close();
      delete m_Driver;
      m_Driver = nullptr;
   }
}

void
FastTableWidget::setFpsTimer( int ms )
{
   if ( ms < 1 ) { ms = 1; }
   if ( m_TimerId ) { killTimer( m_TimerId ); }
   m_TimerId = startTimer( ms, Qt::PreciseTimer );
}

void
FastTableWidget::render()
{
   if ( !m_Driver ) { return; } // No driver.
   int w = m_Driver->getScreenWidth();
   int h = m_Driver->getScreenHeight();
   if ( w < 1 || h < 1 ) { return; } // No screen.
   //emit frameStarted();
   m_Driver->beginRender();
   m_Driver->setModelMatrix( m_MeshTRS.getModelMatrix() );
   m_Driver->draw3D( m_Mesh );
   if ( m_ShowVisualDebug )
   {
      m_Driver->draw3D( m_MeshDbg.debugMesh );
   }
   if ( m_ShowPerfOverlay )
   {
      m_Driver->draw2DPerfOverlay5x8();
   }
   //m_Driver->getSceneManager()->render();    // draw 3d scene
   //m_Driver->getUI()->render();  // draw gui on top
   m_Driver->endRender();
   //emit frameFinished();
}

void
FastTableWidget::resizeEvent( QResizeEvent* event )
{
   if ( !m_Driver ) { return; } // No driver.
   int w = event->size().width();
   int h = event->size().height();
   m_Driver->resize( w, h );
   emit_canvasResized();
   // update(); // Seems to create a cycle ( infinite loop )
}

void
FastTableWidget::timerEvent( QTimerEvent* event )
{
   if ( event->timerId() == m_TimerId )
   {
      render();
   }
}

void
FastTableWidget::paintEvent( QPaintEvent* event )
{
   render();
}

void
FastTableWidget::emit_canvasResized()
{
   // qDebug() << "Emit CanvasResized";
   //emit canvasResized();
}

void
FastTableWidget::mouseMoveEvent( QMouseEvent* event )
{
   if ( m_Driver )
   {
      de::SEvent post;
      post.type = de::EET_MOUSE_EVENT;
      post.mouseEvent.m_Flags = de::SMouseEvent::Moved;
      post.mouseEvent.m_X = event->x();
      post.mouseEvent.m_Y = event->y();
      post.mouseEvent.m_Wheel = 0.0f;
      m_Driver->postEvent( post );
   }

   m_MousePos = glm::ivec2( event->x(), event->y() ); // current mouse pos
   if ( m_MousePos != m_LastMousePos ) // Some button is clicked...
   {
      m_MouseMove = m_MousePos - m_LastMousePos; // current mouse pos
      m_LastMousePos = m_MousePos;
   }
   else
   {
      m_MouseMove = { 0,0 };
   }
}

void
FastTableWidget::mousePressEvent( QMouseEvent* event )
{
   if ( m_Driver )
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
      m_Driver->postEvent( post );
   }
   //m_MousePos = glm::ivec2( event->x(), event->y() ); // current mouse pos
}

void
FastTableWidget::mouseReleaseEvent( QMouseEvent* event )
{
   if ( m_Driver )
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
      m_Driver->postEvent( post );
   }

   //m_MousePos = glm::ivec2( event->x(), event->y() ); // current mouse pos
   //m_MouseMove = { 0,0 };
}


void
FastTableWidget::wheelEvent( QWheelEvent* event )
{
   if ( m_Driver )
   {
      de::SEvent post;
      post.type = de::EET_MOUSE_EVENT;
      post.mouseEvent.m_Flags = de::SMouseEvent::Wheel;
      post.mouseEvent.m_X = event->x();
      post.mouseEvent.m_Y = event->y();
      post.mouseEvent.m_Wheel = event->angleDelta().y(); //    //QPoint delta = event->pixelDelta();
      // DE_DEBUG("post.mouseEvent.m_Wheel = ",post.mouseEvent.m_Wheel)
      m_Driver->postEvent( post );
   }

   event->accept();
}

void
FastTableWidget::keyPressEvent( QKeyEvent* event )
{
   if ( m_Driver )
   {
      de::SEvent post;
      post.type = de::EET_KEY_EVENT;
      post.keyEvent.Key = de::KEY_UNKNOWN;
      post.keyEvent.Flags = de::SKeyEvent::Pressed;
      if ( event->modifiers() & Qt::ShiftModifier ) { post.keyEvent.Flags |= de::SKeyEvent::Shift; }
      if ( event->modifiers() & Qt::ControlModifier ) { post.keyEvent.Flags |= de::SKeyEvent::Ctrl; }
      if ( event->modifiers() & Qt::AltModifier ) { post.keyEvent.Flags |= de::SKeyEvent::Alt; }
      parseQtKey( event, post );
      m_Driver->postEvent( post );
   }
   event->accept();
}

void
FastTableWidget::keyReleaseEvent( QKeyEvent* event )
{
   if ( event->key() == Qt::Key_F2 )
   {
      m_ShowPerfOverlay != m_ShowPerfOverlay;
   }
   if ( event->key() == Qt::Key_F3 )
   {
      m_Driver->setMSAA( m_Driver->getMSAA() - 1 );
   }
   if ( event->key() == Qt::Key_F4 )
   {
      m_Driver->setMSAA( m_Driver->getMSAA() + 1 );
   }
   if ( event->key() == Qt::Key_F5 )
   {
      // F5 - Load Object3D
      std::string uri = dbOpenFileDlg( "Open model file");
      if ( !uri.empty() )
      {
         m_Mesh.clear();
         m_MeshDbg.clear();

         bool ok = de::gpu::SMeshIO::load( m_Mesh, uri, m_Driver );
         if ( ok )
         {
            glm::dvec3 bb = m_Mesh.getBoundingBox().getSize();
            glm::dvec3 scale = glm::dvec3( 100,100,100 ) / bb;
            m_MeshTRS.setS( scale.x, scale.y, scale.z );

            m_MeshDbg.add( m_Mesh, de::gpu::VisualDebugData::ALL );
            DE_DEBUG("F5 - ",uri," has 3D Size(",bb,")")
         }

         //de::gpu::SMeshTool::fitVertices( m_Mesh );

//         if ( m_Mesh.getMeshBufferCount() > 0 )
//         {
//            if ( !m_Mesh.getMeshBuffer( 0 ).Material.hasDiffuseMap() )
//            {
//               m_Mesh.getMeshBuffer( 0 ).setTexture( 0, m_Driver->getTexture( "irr" ) );
//            }
//         }
      }
   }
   if ( event->key() == Qt::Key_F6 )
   {
      m_ShowVisualDebug = !m_ShowVisualDebug;
   }
   if ( event->key() == Qt::Key_F7 )
   {
      int illum = std::clamp( m_Mesh.getLighting() - 1, 0, 4 );
      m_Mesh.setLighting( illum );
   }
   if ( event->key() == Qt::Key_F8 )
   {
      int illum = std::clamp( m_Mesh.getLighting() + 1, 0, 4 );
      m_Mesh.setLighting( illum );
   }
   if ( event->key() == Qt::Key_F9 )
   {
      de::gpu::SMeshTool::flipNormals( m_Mesh );
      m_MeshDbg.clear();
      m_MeshDbg.add( m_Mesh );
   }

   if ( m_Driver )
   {
      de::SEvent post;
      post.type = de::EET_KEY_EVENT;
      post.keyEvent.Key = de::KEY_UNKNOWN;
      post.keyEvent.Flags = de::SKeyEvent::None;
      if ( event->modifiers() & Qt::ShiftModifier ) { post.keyEvent.Flags |= de::SKeyEvent::Shift; }
      if ( event->modifiers() & Qt::ControlModifier ) { post.keyEvent.Flags |= de::SKeyEvent::Ctrl; }
      if ( event->modifiers() & Qt::AltModifier ) { post.keyEvent.Flags |= de::SKeyEvent::Alt; }
      parseQtKey( event, post );
      m_Driver->postEvent( post );
   }


   event->accept();
}



void parseQtKey( QKeyEvent* event, de::SEvent & post )
{
   switch ( event->key() )
   {
      case Qt::Key_0: post.keyEvent.setKey( de::KEY_0 ); break;
      case Qt::Key_1: post.keyEvent.setKey( de::KEY_1 ); break;
      case Qt::Key_2: post.keyEvent.setKey( de::KEY_2 ); break;
      case Qt::Key_3: post.keyEvent.setKey( de::KEY_3 ); break;
      case Qt::Key_4: post.keyEvent.setKey( de::KEY_4 ); break;
      case Qt::Key_5: post.keyEvent.setKey( de::KEY_5 ); break;
      case Qt::Key_6: post.keyEvent.setKey( de::KEY_6 ); break;
      case Qt::Key_7: post.keyEvent.setKey( de::KEY_7 ); break;
      case Qt::Key_8: post.keyEvent.setKey( de::KEY_8 ); break;
      case Qt::Key_9: post.keyEvent.setKey( de::KEY_9 ); break;

      case Qt::Key_A: post.keyEvent.setKey( de::KEY_A ); break;
      case Qt::Key_B: post.keyEvent.setKey( de::KEY_B ); break;
      case Qt::Key_C: post.keyEvent.setKey( de::KEY_C ); break;
      case Qt::Key_D: post.keyEvent.setKey( de::KEY_D ); break;
      case Qt::Key_E: post.keyEvent.setKey( de::KEY_E ); break;
      case Qt::Key_F: post.keyEvent.setKey( de::KEY_F ); break;
      case Qt::Key_G: post.keyEvent.setKey( de::KEY_G ); break;
      case Qt::Key_H: post.keyEvent.setKey( de::KEY_H ); break;
      case Qt::Key_I: post.keyEvent.setKey( de::KEY_I ); break;
      case Qt::Key_J: post.keyEvent.setKey( de::KEY_J ); break;
      case Qt::Key_K: post.keyEvent.setKey( de::KEY_K ); break;
      case Qt::Key_L: post.keyEvent.setKey( de::KEY_L ); break;
      case Qt::Key_M: post.keyEvent.setKey( de::KEY_M ); break;
      case Qt::Key_N: post.keyEvent.setKey( de::KEY_N ); break;
      case Qt::Key_O: post.keyEvent.setKey( de::KEY_O ); break;
      case Qt::Key_P: post.keyEvent.setKey( de::KEY_P ); break;
      case Qt::Key_Q: post.keyEvent.setKey( de::KEY_Q ); break;
      case Qt::Key_R: post.keyEvent.setKey( de::KEY_R ); break;
      case Qt::Key_S: post.keyEvent.setKey( de::KEY_S ); break;
      case Qt::Key_T: post.keyEvent.setKey( de::KEY_T ); break;
      case Qt::Key_U: post.keyEvent.setKey( de::KEY_U ); break;
      case Qt::Key_V: post.keyEvent.setKey( de::KEY_V ); break;
      case Qt::Key_W: post.keyEvent.setKey( de::KEY_W ); break;
      case Qt::Key_X: post.keyEvent.setKey( de::KEY_X ); break;
      case Qt::Key_Y: post.keyEvent.setKey( de::KEY_Y ); break;
      case Qt::Key_Z: post.keyEvent.setKey( de::KEY_Z ); break;

      case Qt::Key_Escape: post.keyEvent.setKey( de::KEY_ESCAPE ); break;
      case Qt::Key_F1: post.keyEvent.setKey( de::KEY_F1 ); break;
      case Qt::Key_F2: post.keyEvent.setKey( de::KEY_F2 ); break;
      case Qt::Key_F3: post.keyEvent.setKey( de::KEY_F3 ); break;
      case Qt::Key_F4: post.keyEvent.setKey( de::KEY_F4 ); break;
      case Qt::Key_F5: post.keyEvent.setKey( de::KEY_F5 ); break;
      case Qt::Key_F6: post.keyEvent.setKey( de::KEY_F6 ); break;
      case Qt::Key_F7: post.keyEvent.setKey( de::KEY_F7 ); break;
      case Qt::Key_F8: post.keyEvent.setKey( de::KEY_F8 ); break;
      case Qt::Key_F9: post.keyEvent.setKey( de::KEY_F9 ); break;
      case Qt::Key_F10: post.keyEvent.setKey( de::KEY_F10 ); break;
      case Qt::Key_F11: post.keyEvent.setKey( de::KEY_F11 ); break;
      case Qt::Key_F12: post.keyEvent.setKey( de::KEY_F12 ); break;

      case Qt::Key_Enter: post.keyEvent.setKey( de::KEY_ENTER ); break;
      case Qt::Key_Backspace: post.keyEvent.setKey( de::KEY_BACKSPACE ); break;
      case Qt::Key_Backslash: post.keyEvent.setKey( de::KEY_BACKSLASH ); break;
      case Qt::Key_Shift: post.keyEvent.setKey( de::KEY_SHIFT ); break;
      case Qt::Key_Control: post.keyEvent.setKey( de::KEY_CTRL ); break;

      case Qt::Key_Up: post.keyEvent.setKey( de::KEY_UP ); break;
      case Qt::Key_Down: post.keyEvent.setKey( de::KEY_DOWN ); break;
      case Qt::Key_Left: post.keyEvent.setKey( de::KEY_LEFT ); break;
      case Qt::Key_Right: post.keyEvent.setKey( de::KEY_RIGHT ); break;
      default: break;
   }
}

*/
