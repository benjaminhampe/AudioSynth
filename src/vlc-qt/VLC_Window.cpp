/// @author Benjamin Hampe <benjamin.hampe@gmx.de>

#include "VLC_Window.hpp"
#include <QWidget>
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QDebug>
#include <QFileIconProvider>

QPushButton* makeButton( wchar_t wc, QWidget* parent )
{
   QPushButton* btn = new QPushButton( QString( wc ), parent );
   btn->setMinimumSize( 64,64 );
   return btn;
}

VLC_Window::VLC_Window()
   : QMainWindow( nullptr )
   , m_Display( nullptr )
{
   setFont( QFont("fontawesome", 12 ) );
   createMenuBar();
   QFileIconProvider provider;

   m_BtnPlay = makeButton( fa::play, this );
   m_BtnPause = makeButton( fa::pause, this );
   m_BtnStop = makeButton( fa::stop, this );
   m_BtnPrev = makeButton( fa::backward, this );
   m_BtnNext = makeButton( fa::forward, this );
   m_BtnFullscreen = makeButton( fa::adjust, this );
   m_BtnPlaylist = makeButton( fa::list, this );

   m_LblPos = new QLabel( this );
   m_LblPos->setText("--:--");

   m_LblDur = new QLabel( this );
   m_LblDur->setText("--:--");

   m_SldPos = new QSlider( Qt::Horizontal, this );
   m_SldPos->setMinimum( 0 );
   m_SldPos->setMaximum( 65535 );
   m_SldPos->setValue( 0 );

   m_Display = new VLC_Display( this );
   m_Display->setPlayer( &m_Player );

   QHBoxLayout* h1 = new QHBoxLayout;
   h1->setContentsMargins(0,0,0,0);
   h1->addWidget( m_LblPos );
   h1->addWidget( m_SldPos,1 );
   h1->addWidget( m_LblDur );

   QHBoxLayout* h2 = new QHBoxLayout;
   h2->setContentsMargins(0,0,0,0);
   h2->addWidget( m_BtnPlay );
   h2->addWidget( m_BtnPause );
   h2->addSpacing( 10 );
   h2->addWidget( m_BtnPrev );
   h2->addWidget( m_BtnStop );
   h2->addWidget( m_BtnNext );
   h2->addSpacing( 10 );
   h2->addWidget( m_BtnFullscreen );
   h2->addWidget( m_BtnPlaylist );

   QVBoxLayout* v12 = new QVBoxLayout;
   v12->setContentsMargins(0,0,0,0);
   v12->addLayout( h1 );
   v12->addLayout( h2 );

   QWidget* playerControls = new QWidget( this );
   playerControls->setLayout( v12 );

   QVBoxLayout* mainLayout = new QVBoxLayout;
   mainLayout->setContentsMargins(0,0,0,0);
   mainLayout->addWidget( m_Display,1 );
   mainLayout->addWidget( playerControls );

   QWidget* centralWidget = new QWidget( this );
   centralWidget->setLayout( mainLayout );

   setCentralWidget( centralWidget );

   connect( m_BtnPlay, SIGNAL(clicked()), this, SLOT(onPlay()) );
   connect( m_BtnPause, SIGNAL(clicked()), this, SLOT(onPause()) );
   connect( m_BtnStop, SIGNAL(clicked()), this, SLOT(onStop()) );
   connect( m_BtnPrev, SIGNAL(clicked()), this, SLOT(onPrev()) );
   connect( m_BtnNext, SIGNAL(clicked()), this, SLOT(onNext()) );
   connect( m_BtnFullscreen, SIGNAL(clicked()), this, SLOT(onShowFullscreen()) );
   connect( m_BtnPlaylist, SIGNAL(clicked()), this, SLOT(onShowPlaylist()) );
   //connect( m_Nemo, SIGNAL(selectedUri(QString)), this, SLOT(onNemo_selectedUri(QString)) );
   //connect( m_Nemo, SIGNAL(selectedUri(QString)), this, SLOT(onNemo_selectedUri(QString)) );
}

VLC_Window::~VLC_Window()
{}

void
VLC_Window::startUpdateTimer()
{
   // stop
   m_LblPosUpdateTimerId = startTimer( 20, Qt::PreciseTimer );
}

void
VLC_Window::stopUpdateTimer()
{
   if ( m_LblPosUpdateTimerId )
   {
      killTimer( m_LblPosUpdateTimerId );
      m_LblPosUpdateTimerId = 0;
   }
}



void
VLC_Window::timerEvent( QTimerEvent* event )
{
   if ( event->timerId() == m_LblPosUpdateTimerId )
   {
      onUpdateLblPos();
   }
}

void
VLC_Window::onUpdateLblPos()
{
   if ( m_Player.is_open() )
   {
      std::string lbl_pos = dbStrSeconds( m_Player.tell() );
      bool has_dur = m_Player.duration() > 0.0;
      std::string lbl_dur = "N/A";
      if ( has_dur )
      {
         lbl_dur = dbStrSeconds( m_Player.duration() );
      }
      m_LblPos->setText( QString::fromStdString( lbl_pos ) );
      m_LblDur->setText( QString::fromStdString( lbl_dur ) );
   }
   else // if ( m_Player.is_open() )
   {
      m_LblPos->setText( "--:--" );
      m_LblDur->setText( "--:--" );
   }
}

void
VLC_Window::onExitApplication()
{
   close();
}


void
VLC_Window::onShowAbout()
{
   //AboutDialog dlg( this );
   //dlg.exec();
}


void
VLC_Window::onPlay()
{
   if ( !m_Player.is_open() )
   {
      onLoadFile();
      m_Player.play();
   }
   else
   {
      if ( m_Player.is_playing() )
      {
         m_Player.pause();
         m_BtnPlay->setText( QString( wchar_t( fa::play ) ) );
         stopUpdateTimer();
      }
      else
      {
         m_Player.play();
         m_BtnPlay->setText( QString( wchar_t( fa::pause ) ) );
         startUpdateTimer();
      }
   }

}

void
VLC_Window::onStop()
{
   m_Player.stop();
}

void
VLC_Window::onPause()
{
   m_Player.pause();
}

void
VLC_Window::onPrev()
{
}

void
VLC_Window::onNext()
{
}

void
VLC_Window::onShowFullscreen()
{

}

void
VLC_Window::onShowPlaylist()
{

}

void
VLC_Window::onLoadFile()
{
//   if ( !m_Driver )
//   {
//      qDebug() << "No video driver";
//      return;
//   }

   QStringList files = QFileDialog::getOpenFileNames( this, "Load CNC file", QApplication::applicationDirPath() );
   if ( files.count() < 1 )
   {
      qDebug() << "No files selected";
      return;
   }

//   de::gpu::ITexture* tex_coverArt = nullptr;
//   de::Image img_coverArt;
//   if ( dbGetCoverArt( img_coverArt, uri ) )
//   {
//      DEM_DEBUG("Got cover art " )
//      tex_coverArt = m_Driver->addTexture2D("coverart", img_coverArt );
//   }

   std::string uri = files.at( 0 ).toStdString();
   m_Player.open( uri );
   m_SldPos->setValue( 0 );
   onUpdateLblPos();



   setWindowTitle( files.at( 0 ) );

   //m_Player.play();
}


void
VLC_Window::createMenuBar()
{
   QMenuBar* menuBar = new QMenuBar( nullptr );
   QMenu* fileMenu = menuBar->addMenu( "Files" );
   QAction* act_fileLoad = fileMenu->addAction( "Load file" );
   QAction* act_fileExit = fileMenu->addAction( "Exit Program" );
   connect( act_fileLoad, &QAction::triggered, this, &VLC_Window::onLoadFile );
   //connect( act_save, &QAction::triggered, this, &VLC_Window::onSaveMachineCode );
   connect( act_fileExit, &QAction::triggered, this, &VLC_Window::onExitApplication );

   QMenu* audioMenu = menuBar->addMenu( "Audio" );
   QAction* act_audio1 = audioMenu->addAction( "A" );
   QAction* act_audio2 = audioMenu->addAction( "B" );
   //connect( act_audio1, &QAction::triggered, this, &VLC_Window::onChooseAudio );

   QMenu* videoMenu = menuBar->addMenu( "Video" );
   QAction* act_video1 = videoMenu->addAction( "Stream A" );
   QAction* act_video2 = videoMenu->addAction( "Stream B" );
   //connect( act_video1, &QAction::triggered, this, &VLC_Window::onChooseStream );
   QAction* act_video3 = videoMenu->addAction( "DVD Chapter Menu" );
   QAction* act_video4 = videoMenu->addAction( "Chapter A" );
   QAction* act_video5 = videoMenu->addAction( "Chapter B" );
   //connect( act_video1, &QAction::triggered, this, &VLC_Window::onChooseDVDChapter );

   QMenu* helpMenu = menuBar->addMenu( "Help" );
   QAction* act_helpAbout = helpMenu->addAction( "Show About" );
   connect( act_helpAbout, &QAction::triggered, this, &VLC_Window::onShowAbout );
   this->setMenuBar( menuBar );
}
