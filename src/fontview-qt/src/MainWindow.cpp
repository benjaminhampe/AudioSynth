#include "MainWindow.hpp"

MainWindow::MainWindow( QWidget* parent )
   : QMainWindow( parent )
{
   setAcceptDrops( true );
   setContentsMargins( 5, 5, 5, 5 );

   m_FontToolBar = new FontBar( nullptr );
   addToolBar( m_FontToolBar );

   m_glyphView = new GlyphDetailView( this );
   m_glyphView->setViewFont( getFontAwesome( 32 ) );

   auto h = new QHBoxLayout;
   h->setContentsMargins( 0,0,0,0 );
   h->addWidget( m_glyphView );

   auto w = new QWidget( this );
   w->setLayout( h );
   setCentralWidget( w );

   connect( m_FontToolBar, SIGNAL(fontChanged(QFont)), m_glyphView, SLOT(setViewFont(QFont)) );
   connect( m_FontToolBar, SIGNAL(glyphIndexChanged(int)), m_glyphView, SLOT(setViewIndex(int)) );

}

MainWindow::~MainWindow()
{

}
