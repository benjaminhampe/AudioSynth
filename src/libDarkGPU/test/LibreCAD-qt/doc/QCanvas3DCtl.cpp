#include <QCanvas3DCtl.hpp>
#include <QGridLayout>
#include <QDebug>

QCanvas3DCtl::QCanvas3DCtl( QWidget* parent )
   : QWidget( parent )
   , m_Canvas( nullptr )
   , m_FpsDisplayUpdateTimerId( 0 )
   , m_FrameTimeEdit( nullptr )
   , m_ScreenShotButton( nullptr )
{
   m_FpsDisplay = new QLabel("0", this);
   m_FrameTimeEdit = new QSpinBox( this );
   m_FrameTimeEdit->setMinimum( 1 );
   m_FrameTimeEdit->setMaximum( 20000 );
   m_FrameTimeEdit->setValue( 30 );
   m_ScreenShotButton = new QPushButton( "Take Screenshot", this );

   int row = 0;
   QGridLayout* layout = new QGridLayout( this );
   layout->addWidget( new QLabel( "FPS:", this ), row, 0, 1, 1 );
   layout->addWidget( m_FpsDisplay, row, 1, 1, 1 ); row++;
   layout->addWidget( new QLabel( "FPS Update Timer in [ms]:", this ), row, 0, 1, 1 );
   layout->addWidget( m_FrameTimeEdit, row, 1, 1, 1 ); row++;
   layout->addWidget( new QLabel( "OpenGLES ScreenShot:", this ), row, 0, 1, 1 );
   layout->addWidget( m_ScreenShotButton, row, 1, 1, 1 ); row++;
   setLayout( layout );

   connect( m_FrameTimeEdit, SIGNAL(valueChanged(int)), this, SLOT(onSetFpsTimer(int)) );
   m_FpsDisplayUpdateTimerId = startTimer( 200 );
}

QCanvas3DCtl::~QCanvas3DCtl()
{
   if ( m_FpsDisplayUpdateTimerId != 0 )
   {
      killTimer( m_FpsDisplayUpdateTimerId );
      m_FpsDisplayUpdateTimerId = 0;
   }
}

void
QCanvas3DCtl::timerEvent( QTimerEvent* e )
{
   if ( e->timerId() == m_FpsDisplayUpdateTimerId )
   {
      updateFpsLabel();
   }
}

void
QCanvas3DCtl::setCanvas3D( QCanvas3D* canvas )
{
   m_Canvas = canvas;
   if ( !m_Canvas ) return;
   connect( m_Canvas, SIGNAL(frameFinished()), this, SLOT(updateFpsLabel()) );

   updateFpsLabel();
}

void
QCanvas3DCtl::onSetFpsTimer( int time_per_frame_in_ms )
{
   if ( !m_Canvas ) return;
   m_Canvas->setFpsTimer( time_per_frame_in_ms );
}

void
QCanvas3DCtl::updateFpsLabel()
{
   if ( !m_Canvas ) return;
   de::gpu::IVideoDriver* driver = m_Canvas->getVideoDriver();
   if ( !driver ) return;
   double fps = driver->getFPS();
   m_FpsDisplay->setText( QString::number( fps ) );
}

void
QCanvas3DCtl::updateCanvasInfoLabel()
{
   if ( !m_Canvas ) return;
   de::gpu::IVideoDriver* driver = m_Canvas->getVideoDriver();
   if ( !driver ) return;

/*
   int w = driver->getScreenWidth();
   int h = driver->getScreenHeight();

   std::stringstream s; s <<
   "Screen(" << w << "," << h << "), "
   "Time(" << driver->getTimer() << "), "
   "FPS(" << driver->getFPS() << "), "
   "FrameCount(" << driver->getFrameCount() << "), "
   "CamPos(" << glm::ivec3( driver->getCameraPos() ) << "), "
   "X(" << driver->getCameraVec1() << "), "
   "Y(" << driver->getCameraVec2() << "), "
   "Z(" << driver->getCameraVec3() << ")"
   //"CamDir(" << cam.getDir() << ")";
   // "CamAng(" << glm::ivec3(cam.getRotationDEG()) << "), ";
   ;
   m_FpsDisplay->setText( QString::fromStdString(s.str()) );
*/
   m_FpsDisplay->setText( QString::number( driver->getFPS() ) );
}

void
QCanvas3DCtl::saveScreenShot()
{
   if ( !m_Canvas ) return;
   de::gpu::IVideoDriver* driver = m_Canvas->getVideoDriver();
   if ( !driver ) return;

   de::Image screenShot;
   if ( driver->createScreenShot( screenShot ) )
   {
      bool ok = dbSaveImage( screenShot, "QCanvas3DCtl.png" );
      if ( !ok )
      {
         DE_MAIN_ERROR("Screenshot not saved ")
      }
   }
}
