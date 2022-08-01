#ifndef DE_GPU_QWIDGET_FAST_TABLE_HPP
#define DE_GPU_QWIDGET_FAST_TABLE_HPP

#include <QWidget>
#include <QImage>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QKeyEvent>

#include <DarkGPU.hpp>

/*
struct FastTableWidget : public QWidget
{
   Q_OBJECT

public:
   FastTableWidget( QWidget* parent );
   ~FastTableWidget() override;
   virtual void render();
   virtual void setClearColor( float r, float g, float b, float a = 1.0f )
   {
      if ( m_Driver ) m_Driver->setClearColor( r,g,b,a );
   }

   virtual de::gpu::ICamera* getActiveCamera() { return m_Driver->getActiveCamera(); }
   virtual de::gpu::IVideoDriver* getVideoDriver() { return m_Driver; }

signals:
   void canvasResized();
   void frameStarted();
   void frameFinished();

public slots:
   void setHorizontalHeaderItemText( int col, QString text );
   void setColumnCount( int cols );
   void setRowCount( int rows );
   void resize( int cols, int rows );

   //void setData( void * ptr, size_t elementSizeInBytes, size_t elementCount = 1 );



   /// @brief OpenGL draws and EGL swaps to QtWidget, works fine.
   /// This function is called 60 times per sec using a timer.
   /// We dont call Qt's paintEvent function directly.
   void emit_canvasResized();

   /// @brief Renders 'n' times per sec using the FPS timer.
   /// e.g. setFpsTimer( 16 ) = 60 fps
   /// e.g. setFpsTimer( 1 ) = 1000 fps
   /// e.g. setFpsTimer( 100 ) = 10 fps
   void setFpsTimer( int ms );
   void setFps( float fps )
   {
      if ( fps < 0.01f ) fps = 0.01f;
      if ( fps > 720 ) fps = 720.0f;
      int dt = std::ceil( 1000.0f / fps );
      if ( dt < 1 ) dt = 1; // timer min interval is 1 ms.
      this->setFpsTimer( dt );
   }
   void setFps10() { setFps( 10.0f ); }
   void setFps20() { setFps( 20.0f ); }
   void setFps30() { setFps( 30.0f ); }
   void setFps60() { setFps( 60.0f ); }
   void setFps85() { setFps( 85.0f ); }
   void sleep() { setFps( 0.1f ); }
   void wakeup() { setFps( 20.0f ); }

protected:
   void timerEvent( QTimerEvent* event) override;
   void paintEvent( QPaintEvent* event ) override;
   void resizeEvent( QResizeEvent* event ) override;
   void keyPressEvent( QKeyEvent* event ) override;
   void keyReleaseEvent( QKeyEvent* event ) override;
   void mousePressEvent( QMouseEvent* event ) override;
   void mouseReleaseEvent( QMouseEvent* event ) override;
   void mouseMoveEvent( QMouseEvent* event ) override;
   void wheelEvent( QWheelEvent* event ) override;

protected:
   virtual void initScene();
   virtual void clearScene();
   virtual void startDriver();
   virtual void closeDriver();
protected:
   DE_CREATE_LOGGER("FastTableWidget")

   de::gpu::IVideoDriver* m_Driver;
   de::gpu::ICamera m_Camera;
   int m_TimerId;
   bool m_ShowPerfOverlay;
   bool m_ShowVisualDebug;
   std::array< bool, 256 > m_KeyStates;
   glm::ivec2 m_MousePos;
   glm::ivec2 m_LastMousePos;
   glm::ivec2 m_MouseMove;
   bool m_IsDragging;
   bool m_IsMouseLeft;
   bool m_IsMouseRight;
   bool m_IsMouseMiddle;
public:
   de::gpu::SMesh m_Mesh;
   de::gpu::TRSd m_MeshTRS;
   de::gpu::SMeshDebug m_MeshDbg;



};

*/

#endif
