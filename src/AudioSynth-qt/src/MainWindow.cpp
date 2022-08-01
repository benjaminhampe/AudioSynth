#include "MainWindow.hpp"

MainWindow::MainWindow()
   : QMainWindow( nullptr )
{
   createMenuBar();
   setContentsMargins( 5,5,5,5 );

   //QImage bg;
   //bg.load( "../../media/rack_ui/rack_bg.png" );
   //m_scrollArea = new QScrollArea( this );
   m_audioRecorder = new GAudioRecorder( this );
   //m_midiSequencer = new GMidiSequencer( this );

   m_waveform3D = new GOszilloskop3D( this );
   m_master = new GMasterDeck( this );
   m_master->setBypassButton( false );
   m_master->setVisibleButton( false );

   m_mixer = new GMixer( this );

   m_vstDeck = new GVstDeck( this );
   m_vstDeck->setBypassButton( false );
   m_vstDeck->setVisibleButton( true );

   m_deckAB = new GFilePlayerDeck( this );
   m_deckAB->setBypassButton( false );
   m_deckAB->setVisibleButton( true );

   m_deckCD = new GFilePlayerDeck( this );
   m_deckCD->setBypassButton( false );
   m_deckCD->setVisibleButton( false );

   m_deckEF = new GFilePlayerDeck( this );
   m_deckEF->setBypassButton( false );
   m_deckEF->setVisibleButton( false );

   m_detuneSynth = new GDetuneSynthDeck( this );
   m_detuneSynth->setBypassButton( false );
   m_detuneSynth->setVisibleButton( false );

   m_betterSynth = new GBetterSynthDeck( this );
   m_betterSynth->setBypassButton( false );
   m_betterSynth->setVisibleButton( false );

   //m_Piano3D = new Piano3D( this );
   m_Piano2D = new GPianoKeyboard( this );
   m_sequencer = new GSequencerCtl( this );
   //m_deckAB->setBackgroundImage( bg );

//   QVBoxLayout* seqLayout = new QVBoxLayout();
//   seqLayout->setContentsMargins( 0,0,0,0 );
//   seqLayout->setSpacing( 5 );
//   seqLayout->addLayout( h3,1 );
//   seqLayout->addWidget( m_seq );
//   QWidget* seqWidget = new QWidget( this );
//   seqWidget->setContentsMargins( 0,0,0,0 );
//   seqWidget->setLayout( seqLayout );

//   auto topLayout = new QVBoxLayout();
//   topLayout->setContentsMargins( 0,0,0,0 );
//   topLayout->setSpacing( 0 );
//   topLayout->addWidget( m_waveform3D,1 );
//   topLayout->addWidget( m_Piano2D );
//   auto topWidget = new QImageWidget( this );
//   topWidget->setLayout( topLayout );

   auto rackLayout = new QVBoxLayout();
   rackLayout->setContentsMargins( 0,0,0,0 );
   rackLayout->setSpacing( 0 );
   rackLayout->addWidget( m_audioRecorder );
   rackLayout->addWidget( m_master );
   rackLayout->addWidget( m_deckAB );
   rackLayout->addWidget( m_deckCD );
   rackLayout->addWidget( m_deckEF );
   rackLayout->addWidget( m_vstDeck );
   rackLayout->addWidget( m_detuneSynth );
   rackLayout->addWidget( m_betterSynth );
   //rackLayout->addStretch( 1 );
   rackLayout->addWidget( m_sequencer,1 );
   rackLayout->addWidget( m_Piano2D );

   auto rackWidget = new QImageWidget( this );
   rackWidget->setLayout( rackLayout );

   auto splitV = new QSplitter( Qt::Vertical, this );
   splitV->addWidget( m_waveform3D );
   splitV->addWidget( rackWidget );
   splitV->setChildrenCollapsible( true );
//   splitV->setStretchFactor( 0, 1 );
//   splitV->setStretchFactor( 1, 0 );

   auto centralLayout = new QVBoxLayout();
   centralLayout->setContentsMargins( 0,0,0,0 );
   centralLayout->setSpacing( 0 );
   centralLayout->addWidget( splitV );
   //centralLayout->addStretch( splitV );
//   auto centralLayout = new QVBoxLayout();
//   centralLayout->setContentsMargins( 0,0,0,0 );
//   centralLayout->setSpacing( 0 );
//   centralLayout->addWidget( m_waveform3D );
//   centralLayout->addWidget( m_master );
//   centralLayout->addWidget( m_deckAB );
//   centralLayout->addWidget( m_deckCD );
//   centralLayout->addWidget( m_deckEF );
//   centralLayout->addWidget( m_detuneSynth );
//   centralLayout->addWidget( m_Piano2D,1 );




   //QImage img;
   //img.load( "../../media/background/wave_3d.jpg");
   //img.load( "../../media/background/desert.jpg");
   auto centralWidget = new QImageWidget( this );
   //centralWidget->setImage( img );
   //centralWidget->setImagePreserveAspectWoH( true );
   centralWidget->setLayout( centralLayout );

   setCentralWidget( centralWidget );



//   QWidget* centralWidget = new QWidget( this );
//   centralWidget->setContentsMargins( 0,0,0,0 );
//   centralWidget->setLayout( v );
//   setCentralWidget( centralWidget );
   //m_soundtouch->setVisible( false );

   //m_synth2Waveform->setupDspElement( 512,2,48000 );
   //m_masterWaveform->setupDspElement( 2*1024,2,48000 );

   //m_synth2ADSR->setADSR( m_synth2->getADSR() );
   //m_synth2LFO->setLFO( m_synth2->getLFO() );

   //m_synth2Delay->setInputSignal( m_synth2 );
   //m_synth2Delay->setBypassed( true );
//   m_synth2LFO->setInputSignal( m_synth2Delay );
//   m_synth2LFO->setBypassed( true );
   //m_synth2Waveform->setInputSignal( m_synth2Delay );
   //m_synth2Volume->setInputSignal( m_synth2Waveform );
   //m_synth2LevelMeter->setInputSignal( m_synth2Volume );

   m_mixer->setInputSignal( 0, m_detuneSynth );
   m_mixer->setInputSignal( 1, m_deckAB );
   m_mixer->setInputSignal( 2, m_deckCD );
   m_mixer->setInputSignal( 3, m_deckEF );
   m_mixer->setInputSignal( 4, m_betterSynth );
   m_mixer->setInputSignal( 5, m_vstDeck );

   m_master->setInputSignal( 0, m_mixer );

   //m_master->m_timeStretch->setInputSignal( m_mixer );
   m_master->m_delay->setInputSignal( 0, m_mixer );
   m_master->m_pan->setInputSignal( 0, m_master->m_delay );
   m_master->m_lowPass3->setInputSignal( 0, m_master->m_pan );
   m_master->m_overdrive->setInputSignal( m_master->m_lowPass3 );
   m_master->m_lowPass4->setInputSignal( 0, m_master->m_overdrive );
   m_master->m_bassBoost->setInputSignal( m_master->m_lowPass4 );
   m_master->m_compressor->setInputSignal( 0, m_master->m_bassBoost );
   m_master->m_limiter->setInputSignal( 0, m_master->m_compressor );
   m_master->m_waveform->setInputSignal( 0, m_master->m_limiter );
   m_waveform3D->setInputSignal( 0, m_master->m_waveform );
   m_master->m_spektrum->setInputSignal( m_waveform3D );
   m_master->m_volume->setInputSignal( 0, m_master->m_spektrum );
   m_master->m_levelMeter->setInputSignal( m_master->m_volume );
   m_audioRecorder->setInputSignal( 0, m_master->m_levelMeter );
   m_master->m_menu->setInputSignal( 0, m_audioRecorder );
   //m_waveform->setInputSignal( 0, m_inputSignal );


   m_endPoint.setInputSignal( 0, m_master );
   m_endPoint.play();

   m_Piano2D->setSynth( 0, m_detuneSynth );
   m_Piano2D->setSynth( 1, m_betterSynth );
   m_Piano2D->setSynth( 2, m_vstDeck );

   //m_midiFilePlayer->setSynth( 0, m_detuneSynth );
   //m_midiSequencer->setSynth( 0, m_betterSynth );
   //m_sequencer->setSynth( 0, m_betterSynth );

   //m_Piano3D->setSynth( 0, m_synth );
   //m_Piano3D->setSynth( 1, m_synth2 );
   //m_Piano3D->setFps( 20 );

}

MainWindow::~MainWindow()
{}

void
MainWindow::createMenuBar()
{
   QMenuBar* menuBar = new QMenuBar( nullptr );

   QMenu* fileMenu = menuBar->addMenu( "&Files" );
   QAction* act_open = fileMenu->addAction( "Open file" );
   QAction* act_exit = fileMenu->addAction( "Exit file" );

   QMenu* viewMenu = menuBar->addMenu( "&View" );
   QAction* act_show_oszi_3d = viewMenu->addAction( "Show 3d Oszilloskop - F3" );
   QAction* act_show_master = viewMenu->addAction( "Show Master" );
   QAction* act_show_mixer = viewMenu->addAction( "Show Mixer - F2" );
   QAction* act_show_playerAB = viewMenu->addAction( "Show Player A+B" );
   QAction* act_show_playerCD = viewMenu->addAction( "Show Player C+D" );
   QAction* act_show_playerEF = viewMenu->addAction( "Show Player E+F" );
   QAction* act_show_seq16 = viewMenu->addAction( "Show Sequencer16 - F4" );
   QAction* act_show_synth1 = viewMenu->addAction( "Show DetuneSynth 1 - F5" );

   QMenu* helpMenu = menuBar->addMenu( "&Help" );
   QAction* act_help_about = helpMenu->addAction( "Show &About" );

   this->setMenuBar( menuBar );

   connect( act_open, &QAction::triggered, this, &MainWindow::onMenuFileOpen );
   connect( act_exit, &QAction::triggered, this, &MainWindow::onMenuFileExit );

   connect( act_show_oszi_3d, &QAction::triggered, this, [&] () {
         m_waveform3D->setVisible( !m_waveform3D->isVisible() );
      });
   connect( act_show_master, &QAction::triggered, this, [&] () {
         m_master->setVisible( !m_master->isVisible() );
      });
   connect( act_show_mixer, &QAction::triggered, this, [&] () {
         m_mixer->setVisible( !m_mixer->isVisible() );
      });
   connect( act_show_playerAB, &QAction::triggered, this, [&] () {
         m_deckAB->setVisible( !m_deckAB->isVisible() );
      });
   connect( act_show_playerCD, &QAction::triggered, this, [&] () {
         m_deckCD->setVisible( !m_deckCD->isVisible() );
      });
   connect( act_show_playerEF, &QAction::triggered, this, [&] () {
         m_deckEF->setVisible( !m_deckEF->isVisible() );
      });
//   connect( act_show_seq16, &QAction::triggered, this, [&] () {
//         m_seq16->setVisible( !m_seq16->isVisible() );
//      });
   connect( act_show_synth1, &QAction::triggered, this, [&] () {
         m_detuneSynth->setVisible( !m_detuneSynth->isVisible() );
      });

   connect( act_help_about, &QAction::triggered, this, &MainWindow::onShowAboutDialog );

}

void
MainWindow::onShowAboutDialog()
{
   //AboutDialog dlg( this );
   //dlg.exec();
}

void
MainWindow::onMenuFileExit()
{
   close();
}

void
MainWindow::onMenuFileOpen()
{
   // QApplication::applicationDirPath()
   QStringList files = QFileDialog::getOpenFileNames( this, "Open 3d model file", "G:/media/models" );
   if ( files.count() < 1 )
   {
      qDebug() << "No files selected";
      return;
   }

   if ( files.count() < 1 )
   {
      qDebug() << "No File selected";
      return;
   }

   qDebug() << "Selected File[0] " << files.at( 0 );

//   de::gpu::IVideoDriver * m_Driver = m_Canvas->getVideoDriver();
//   if ( !m_Driver )
//   {
//      qDebug() << "No video driver";
//      return;
//   }

//   QString uri = files.at( 0 );

//   auto sri = uri.toStdString();
//   auto mesh = new de::gpu::SMesh( sri );
//   bool ok = de::gpu::SMeshIO::load( *mesh, sri, m_Driver );
//   if ( ok )
//   {
//      //glm::dvec3 bb = m_Canvas->m_Mesh.getBoundingBox().getSize();
//      //glm::dvec3 scale = glm::dvec3( 100,100,100 ) / bb;
//      //m_Canvas->m_MeshTRS.setS( scale.x, scale.y, scale.z );
//      DE_DEBUG("+ Loaded 3D file ",sri)
//   }
//   else
//   {
//      DE_ERROR("--- Cant Load 3D file ",sri)
//   }

//   m_SceneEditor->addMesh( mesh );
}


