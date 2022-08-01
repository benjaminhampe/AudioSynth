/// (c) 2017 - 20180 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef DE_SYNTH_MAINWINDOW_HPP
#define DE_SYNTH_MAINWINDOW_HPP

#include <sstream>
#include <chrono>
#include <cmath>

#include <QMainWindow>
#include <QScreen>
#include <QWidget>
#include <QDesktopWidget>

#include <QWidget>
#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QDial>
#include <QDebug>
#include <QThread>

#include <QWidget>
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QFileDialog>

#include <QScrollArea>

#include <de/audio/engine/MixerStream.hpp>
//#include "Piano3D.hpp"
#include "GVolume.hpp"
#include "GLevelMeter.hpp"
#include "GLFO.hpp"
#include "GADSR.hpp"
#include "GDelay.hpp"
#include "GMixer.hpp"
//#include "GFilter.hpp"
//#include "GLowPass.hpp"
#include "GLowPass3.hpp"
#include "GBassBoost.hpp"
#include "GOverdrive.hpp"
//#include "GLoPass.hpp"
#include "GLoudnessLFO.hpp"
#include "GSoundTouch.hpp"
#include "GPianoKeyboard.hpp"
#include "GOszilloskop.hpp"
#include "GSpektrometer.hpp"
#include "GOszilloskop3D.hpp"
//#include "GSpektrometer3D.hpp"

//#include "decksseq16/Rack_Sequencer16.hpp"
//#include "GSynth.hpp"

#include "GFilePlayerDeck.hpp"
#include "GMasterDeck.hpp"

#include "GAudioRecorder.hpp"
#include "GMidiSequencer.hpp"

#include "GDetuneSynthDeck.hpp"
#include "GBetterSynthDeck.hpp"

#include "GVstDeck.hpp"
#include "GSequencerCtl.hpp"
class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   MainWindow();
   ~MainWindow() override;

   void createMenuBar();

public slots:
   void onShowAboutDialog();
   void onMenuFileExit();
   void onMenuFileOpen();

protected:
   DE_CREATE_LOGGER("MainWindow")

   //GOszilloskop3D* m_Oszi3L; EGL_BAD_SURFACE for more than one 3d widget
   //GOszilloskop3D* m_Oszi3R; EGL_BAD_SURFACE for more than one 3d widget

//   GSynth* m_synth;
//   GADSR* m_voiceADSR;
//   GLFO* m_voiceLFO;
   GAudioRecorder* m_audioRecorder;
   //GMidiSequencer* m_midiSequencer;
   GOszilloskop3D* m_waveform3D;
   GMasterDeck* m_master;
   GMixer* m_mixer;
   GFilePlayerDeck* m_deckAB;
   GFilePlayerDeck* m_deckCD;
   GFilePlayerDeck* m_deckEF;
   GVstDeck* m_vstDeck;
   GDetuneSynthDeck* m_detuneSynth;
   GBetterSynthDeck* m_betterSynth;

   //Rack_Sequencer16* m_seq16;


//   //GFilter* m_masterFilter;
//   GOverdrive* m_masterOverdrive;
//   //GLowPass* m_masterLopass1;
//   //GLowPass* m_masterLopass2;
//   GLowPass3* m_masterLopass3;
//   //GLoPass* m_masterLopass4;
//   GBassBoost* m_masterBassBoost;

//   GVolume* m_masterVolume;
//   GLevelMeter* m_masterLevelMeter;
//   GOszilloskop* m_masterWaveform;
//   GSpektrometer* m_masterSpektrum;

   //Piano3D* m_Piano3D;
   GPianoKeyboard* m_Piano2D;
   GSequencerCtl* m_sequencer;
   //GAudioMatrix3D m_FreqMatrix3D;

//   de::audio::LoudnessLFOf m_lfoPiano;
//   de::audio::LoudnessLFOf m_lfoSynth;
   de::audio::MixerStream m_endPoint;
};

#endif
