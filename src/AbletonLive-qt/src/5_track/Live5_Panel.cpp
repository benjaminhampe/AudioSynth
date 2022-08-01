#include "Live5_Panel.hpp"

Live5_Panel::Live5_Panel( App & app, QWidget* parent )
   : QWidget( parent )
   , m_app( app )
{
   setContentsMargins( 0,0,0,0 );
   setMinimumHeight( 100 );
   m_helpViewer = new HelpViewer( m_app, this );
   auto hPanel = createHBox();
   hPanel->addWidget( m_helpViewer );
   hPanel->addSpacing( 10 );
   m_helpPanel = new Live0_Panel( m_app, this );
   m_helpPanel->setLayout( hPanel );

   auto hHelp = createHBox();
   hHelp->addWidget( m_helpPanel );
   hHelp->addSpacing( 10 );
   m_helpWidget = createWidget( this );
   m_helpWidget->setLayout( hHelp );

   m_clipEditor = new ClipEditor( m_app, this );
   auto hEdit = createHBox();
   hEdit->addWidget( m_clipEditor );
   m_clipPanel = new Live0_Panel( m_app, this );
   m_clipPanel->setLayout( hEdit );

   m_fxPanel = new FxPanel( m_app, this );
   auto h = createHBox();
   h->addWidget( m_helpWidget );
   h->addWidget( m_clipPanel,1 );
   h->addSpacing( 10 );
   h->addWidget( m_fxPanel,1 );
   setLayout( h );
}

Live5_Panel::~Live5_Panel()
{}
