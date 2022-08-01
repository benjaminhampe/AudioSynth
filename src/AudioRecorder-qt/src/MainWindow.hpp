/// (c) 2017 - 20180 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef DE_SYNTH_MAINWINDOW_HPP
#define DE_SYNTH_MAINWINDOW_HPP

#include "Common.hpp"

class MainWindow : public QMainWindow
{
   Q_OBJECT
   // GAudioRecorder* m_audioRecorder;
   // //GMidiSequencer* m_midiSequencer;
   // GOszilloskop3D* m_waveform3D;
   // GMasterDeck* m_master;
   // GMixer* m_mixer;
   // GFilePlayerDeck* m_deckAB;
   // GFilePlayerDeck* m_deckCD;
   // GFilePlayerDeck* m_deckEF;
   // GVstDeck* m_vstDeck;
   // GDetuneSynthDeck* m_detuneSynth;
   // GBetterSynthDeck* m_betterSynth;
//   GOverdrive* m_masterOverdrive;
//   GLowPass3* m_masterLopass3;
//   GBassBoost* m_masterBassBoost;
//   GVolume* m_masterVolume;
//   GLevelMeter* m_masterLevelMeter;
//   GOszilloskop* m_masterWaveform;
//   GSpektrometer* m_masterSpektrum;

   de::audio::MixerStream m_endPoint;
public:
   MainWindow() : QMainWindow( nullptr )
   {
      createMenuBar();
      setContentsMargins( 5,5,5,5 );

      m_audioRecorder = new GAudioRecorder( this );
      m_master = new GMasterDeck( this );
      m_master->setBypassButton( false );
      m_master->setVisibleButton( false );
      m_mixer = new GMixer( this );

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

      auto centralWidget = new QImageWidget( this );

      centralWidget->setLayout( centralLayout );

      setCentralWidget( centralWidget );

      m_mixer->setInputSignal( 0, m_detuneSynth );
      m_master->setInputSignal( 0, m_mixer );

      // //m_master->m_timeStretch->setInputSignal( m_mixer );
      // m_master->m_delay->setInputSignal( 0, m_mixer );
      // m_master->m_pan->setInputSignal( 0, m_master->m_delay );
      // m_master->m_lowPass3->setInputSignal( 0, m_master->m_pan );
      // m_master->m_overdrive->setInputSignal( m_master->m_lowPass3 );
      // m_master->m_lowPass4->setInputSignal( 0, m_master->m_overdrive );
      // m_master->m_bassBoost->setInputSignal( m_master->m_lowPass4 );
      // m_master->m_compressor->setInputSignal( 0, m_master->m_bassBoost );
      // m_master->m_limiter->setInputSignal( 0, m_master->m_compressor );
      // m_master->m_waveform->setInputSignal( 0, m_master->m_limiter );
      // m_waveform3D->setInputSignal( 0, m_master->m_waveform );
      // m_master->m_spektrum->setInputSignal( m_waveform3D );
      // m_master->m_volume->setInputSignal( 0, m_master->m_spektrum );
      // m_master->m_levelMeter->setInputSignal( m_master->m_volume );
      // m_audioRecorder->setInputSignal( 0, m_master->m_levelMeter );
      // m_master->m_menu->setInputSignal( 0, m_audioRecorder );
      // //m_waveform->setInputSignal( 0, m_inputSignal );


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

   ~MainWindow() override {}
   void createMenuBar();
public slots:
   void onShowAboutDialog();
   void onMenuFileExit();
   void onMenuFileOpen();


};

#endif
