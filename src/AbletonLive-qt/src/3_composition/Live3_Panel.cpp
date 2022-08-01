#include "Live3_Panel.hpp"

Live3_Panel::Live3_Panel( App & app, QWidget* parent )
   : QWidget( parent )
   , m_app( app )
{
   setContentsMargins( 0,0,0,0 );

   m_overview = new TimeLineOverview( m_app, this );
   m_arrangement = new Arrangement( m_app, this );
   m_arrangePanel = new Live0_Panel( m_app, this );
   m_composeBar = new ComposeBar( m_app, this );

   auto v = createVBox();
   v->addWidget( m_overview );
   v->addWidget( m_arrangement,1 );
   m_arrangePanel->setLayout( v );

   auto h = createHBox();
   h->addSpacing( 5 );
   h->addWidget( m_arrangePanel,1 );
   h->addWidget( m_composeBar );
   setLayout( h );
}

Live3_Panel::~Live3_Panel()
{}
