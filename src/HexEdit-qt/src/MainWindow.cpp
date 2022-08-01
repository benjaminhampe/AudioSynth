#include "MainWindow.hpp"


MainWindow::MainWindow()
   : QMainWindow( nullptr )
{
   createMenuBar();
   setContentsMargins( 5,5,5,5 );
   m_hexViewer = new QHexViewer( this );
   setCentralWidget( m_hexViewer );
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
   QMenu* helpMenu = menuBar->addMenu( "&Help" );
   QAction* act_help_about = helpMenu->addAction( "Show &About" );

   this->setMenuBar( menuBar );

   connect( act_open, &QAction::triggered, this, &MainWindow::onMenuFileOpen );
   connect( act_exit, &QAction::triggered, this, &MainWindow::onMenuFileExit );
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
   m_hexViewer->openFile();

}


