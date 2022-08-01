#ifndef DE_VLC_DISPLAY_QWIDGET_HPP
#define DE_VLC_DISPLAY_QWIDGET_HPP

#include <QWidget>
#include <QImage>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QKeyEvent>

#include <DarkVLC.hpp>

class VLC_Display : public QWidget
{
   Q_OBJECT
public:
   VLC_Display( QWidget* parent );
   ~VLC_Display() override;
   void render();
   void setPlayer( de::vlc::Player* player );
   de::gpu::IVideoDriver* getVideoDriver() const;
signals:
   void canvasResized();
   void frameStarted();
   void frameFinished();

public slots:
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
   de::gpu::IVideoDriver* m_Driver;
   int m_TimerId;

   // State
   std::array< bool, 256 > m_KeyStates;
   glm::ivec2 m_MousePos;
   glm::ivec2 m_MousePosLast;
   glm::ivec2 m_MouseMove;
   // bool m_IsDragging( false );
   bool m_IsMouseMoving;
   bool m_IsLeftPressed;
   bool m_IsRightPressed;
   bool m_IsMiddlePressed;

   de::vlc::Player* m_Player;
};

#endif
