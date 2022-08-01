/// (c) 2017 - 20180 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef DE_HEXEDIT_MAINWINDOW_HPP
#define DE_HEXEDIT_MAINWINDOW_HPP


#include <sstream>
#include <fstream>
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
#include <QFileInfo>
#include <QScrollArea>

#include <DarkImage.hpp>
#include <QImageWidget.hpp>
#include <QFont5x8.hpp>

#include <QHexViewer.hpp>

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
   QHexViewer* m_hexViewer;
};

#endif
