#include "MainWindow.hpp"

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

MainWindow::MainWindow()
   : QMainWindow( nullptr )
   , m_Canvas( nullptr )
{
   createMenuBar();

   m_Canvas = new Canvas3D( this );
   m_MeshTool = new MeshTool( this );
   m_MeshTool->setCanvas3D( m_Canvas );
   m_SceneEditor = new SceneEditor( this );
   m_SceneEditor->setSceneManager( m_Canvas->getVideoDriver()->getSceneManager() );

   QVBoxLayout* leftLayout = new QVBoxLayout( this );
   leftLayout->setContentsMargins( 0,0,0,0 );
   leftLayout->addWidget( m_MeshTool, 0 );
   //leftLayout->addWidget( m_tvMesh, 0 );

   QWidget* leftWidget = new QWidget( this );
   leftWidget->setContentsMargins( 0,0,0,0 );
   leftWidget->setLayout( leftLayout );

   QSplitter* splitLeft = new QSplitter( Qt::Horizontal );
   splitLeft->addWidget( leftWidget );
   splitLeft->addWidget( m_Canvas );
   splitLeft->addWidget( m_SceneEditor );
   setCentralWidget( splitLeft );

   connect( m_MeshTool, &MeshTool::meshCreated,
            m_SceneEditor, &SceneEditor::addMesh );

   connect( m_SceneEditor, &SceneEditor::activatedNode,
            m_MeshTool, &MeshTool::setSceneNode );

}

MainWindow::~MainWindow()
{}

void
MainWindow::createMenuBar()
{
   QMenuBar* menuBar = new QMenuBar( nullptr );
   QMenu* menu = menuBar->addMenu( "Files" );
   QAction* act_open = menu->addAction( "Open file" );
   QAction* act_exit = menu->addAction( "Exit file" );
   connect( act_open, &QAction::triggered, this, &MainWindow::onMenuFileOpen );
   connect( act_exit, &QAction::triggered, this, &MainWindow::onMenuFileExit );
   QMenu* menuHelp = menuBar->addMenu( "Help" );
   QAction* act_help_about = menuHelp->addAction( "Show About" );
   connect( act_help_about, &QAction::triggered, this, &MainWindow::onShowAboutDialog );
   this->setMenuBar( menuBar );
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

   de::gpu::IVideoDriver * m_Driver = m_Canvas->getVideoDriver();
   if ( !m_Driver )
   {
      qDebug() << "No video driver";
      return;
   }

   QString uri = files.at( 0 );

   auto sri = uri.toStdString();
   auto smgr = m_Driver->getSceneManager();
   auto mesh = smgr->getMesh( sri );
   if ( mesh )
   {
      m_SceneEditor->addMesh( mesh );
      DE_DEBUG("+ Loaded 3D file ",sri)
   }
   else
   {
      DE_ERROR("--- Cant Load 3D file ",sri)
   }
}

