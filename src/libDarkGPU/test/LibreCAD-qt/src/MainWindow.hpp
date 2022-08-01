/// (c) 2017 - 20180 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef DE_QGPUFPSCUBE_MAINWINDOW_HPP
#define DE_QGPUFPSCUBE_MAINWINDOW_HPP

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
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QDial>

#include <Canvas3D.hpp>
#include <MeshTool.hpp>

#include <SceneEditor.hpp>

//#include <SMeshTree.hpp>
//#include <MeshPool.hpp>

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
   Canvas3D* m_Canvas;
   MeshTool* m_MeshTool;
   SceneEditor* m_SceneEditor;


};

#endif
