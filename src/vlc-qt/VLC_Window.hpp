/// (c) 2017 - 20180 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef DE_VLC_WINDOW_HPP
#define DE_VLC_WINDOW_HPP

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QLineEdit>

#include "VLC_Display.hpp"
#include <DarkVLC.hpp>

class VLC_Window : public QMainWindow
{
   Q_OBJECT
   DE_CREATE_LOGGER("CNCMS.Window")
public:
   VLC_Window();
   ~VLC_Window() override;

public slots:
   void onExitApplication();
   void onShowAbout();

   void onLoadFile();
   void onPlay();
   void onPause();
   void onStop();
   void onPrev();
   void onNext();
   void onShowFullscreen();
   void onShowPlaylist();

   void startUpdateTimer();
   void stopUpdateTimer();

   void onUpdateLblPos();
   //void onUpdateSldPos();

protected:
   void createMenuBar();
   void timerEvent( QTimerEvent* event );

private:
   QPushButton* m_BtnPlay;
   QPushButton* m_BtnPause;
   QPushButton* m_BtnStop;
   QPushButton* m_BtnPrev;
   QPushButton* m_BtnNext;

   QPushButton* m_BtnFullscreen;
   QPushButton* m_BtnPlaylist;

   QLabel* m_LblPos;
   QLabel* m_LblDur;
   QSlider* m_SldPos;

   VLC_Display* m_Display;

   de::vlc::Player m_Player;

   int m_LblPosUpdateTimerId;
   //de::audio::AudioPlayer m_AudioPlayer;
   //de::video::VideoPlayer m_VideoPlayer;
};

#endif
