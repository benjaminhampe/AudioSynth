#ifndef DE_GPU_CANVAS3D_CONTROL_QWIDGET_HPP
#define DE_GPU_CANVAS3D_CONTROL_QWIDGET_HPP

#include <QCanvas3D.hpp>
#include <QWidget>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>

struct QCanvas3DCtl : public QWidget
{
   Q_OBJECT
public:
   QCanvas3DCtl( QWidget* parent );
   ~QCanvas3DCtl();

public slots:
   void setCanvas3D( QCanvas3D* canvas );
   void saveScreenShot();

   void onSetFpsTimer( int time_per_frame_in_ms );

   void updateFpsLabel();
   void updateCanvasInfoLabel();

protected:
   void timerEvent( QTimerEvent* e ) override;
public:
   QCanvas3D* m_Canvas;

   int m_FpsDisplayUpdateTimerId;
   QLabel*   m_FpsDisplay;
   QSpinBox* m_FrameTimeEdit;
   QPushButton* m_ScreenShotButton;
};

#endif
