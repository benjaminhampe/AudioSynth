#include "Live0_Body.hpp"

Splitter::Splitter( App & app, Qt::Orientation orientation, QWidget* parent )
   : QSplitter( orientation, parent )
   , m_app( app )
{
   setObjectName( "Splitter" );
   setContentsMargins(0,0,0,0);
}

void
Splitter::paintEvent( QPaintEvent* event )
{
   //QSplitter::paintEvent( event );
//   int w = width();
//   int h = height();

//   if ( w > 1 && h > 1 )
//   {
//      LiveSkin const & skin = m_app.m_skin;
//      QPainter dc( this );
//      dc.setRenderHint( QPainter::NonCosmeticDefaultPen );
//      dc.fillRect( rect(), skin.windowColor );
//      dc.end();
//   }
}


Live0_Body::Live0_Body( QWidget* parent )
   : QWidget( parent )
{
   setContentsMargins( 6,8,6,8 );
   m_enginePanel = new Live1_Panel( m_app, this );
   m_pluginPanel = new Live2_Panel( m_app, this );
   m_arrangePanel = new Live3_Panel( m_app, this );
   m_trackPanel = new Live5_Panel( m_app, this );
   m_footerPanel = new Live7_Panel( m_app, this );

   // TopBody = SplitH( pluginDb, arrangement )
   m_hSplit = new Splitter( m_app, Qt::Horizontal, this );
   //m_hSplit->setSizePolicy( QSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored ) );
   //sh->setOpaqueResize( false );
   //sh->setFrameShape( QFrame::NoFrame );
   //m_pluginPanel->hide();
   m_hSplit->addWidget( m_pluginPanel );
   m_hSplit->addWidget( m_arrangePanel );
   //m_hSplit->setCollapsible(0,1);
   //m_hSplit->setCollapsible(1,1);
   //m_hSplit->setStretchFactor(0,0);
   //m_hSplit->setStretchFactor(1,0);
   QList<int> shs; shs << 0 << 0;
   m_hSplit->setSizes( shs );
   m_hSplit->setHandleWidth( 1 );

   auto vTop = createVBox();
   vTop->addWidget( m_hSplit );
   vTop->addSpacing( 5 );
   auto wTop = createWidget( this );
   wTop->setLayout( vTop );

   auto vTrack = createVBox();
   vTrack->addSpacing( 5 );
   vTrack->addWidget( m_trackPanel,1 );

   auto wTrack = createWidget( this );
   wTrack->setLayout( vTrack );

   m_vSplit = new Splitter( m_app, Qt::Vertical, this );
   //m_vSplit->setSizePolicy( QSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored ) );
   m_vSplit->setHandleWidth( 1 );
   m_vSplit->addWidget( wTop );

   //wTrack->hide();
   m_vSplit->addWidget( wTrack );
   //m_vSplit->setCollapsible(0,1);
   //m_vSplit->setCollapsible(1,1);
   //m_vSplit->setStretchFactor(0,0);
   //m_vSplit->setStretchFactor(1,0);
   QList<int> l; l << 0 << 0;
   m_vSplit->setSizes( l );

   auto v = createVBox();
   v->addWidget( m_enginePanel );
   v->addWidget( m_vSplit );
   //v->addSpacing( 10 );
   v->addWidget( m_footerPanel );
   setLayout( v );

//   auto v = createVBox();
//   v->addWidget( m_enginePanel );
//   v->addWidget( m_pluginPanel );
//   setLayout( v );

   // Show/Hide PluginExplorer
   connect( m_pluginPanel->m_deviceBar->m_btnShowExplorer, SIGNAL(clicked(bool)), this, SLOT(on_showExplorer(bool)) );
   // Show/Hide PluginExplorer
   connect( m_footerPanel->m_showHelpButton, SIGNAL(clicked(bool)), this, SLOT(on_showHelpPanel(bool)) );
   connect( m_footerPanel->m_showPanelButton, SIGNAL(clicked(bool)), this, SLOT(on_showClipAndFxPanel(bool)) );
}

Live0_Body::~Live0_Body()
{}

void
Live0_Body::on_showExplorer( bool checked )
{
   m_pluginPanel->m_pluginPanel->setVisible( checked );
   m_hSplit->updateGeometry();
}

void
Live0_Body::on_showHelpPanel( bool checked )
{
   m_trackPanel->m_helpWidget->setVisible( checked );
   m_vSplit->updateGeometry();
}

void
Live0_Body::on_showClipAndFxPanel( bool checked )
{
   m_trackPanel->setVisible( checked );
   m_vSplit->updateGeometry();
}

//void
//Live0_Body::resizeEvent( QResizeEvent* event )
//{
//   update();
//   QWidget::resizeEvent( event );
//}

void
Live0_Body::paintEvent( QPaintEvent* event )
{
   int w = width();
   int h = height();

   if ( w > 1 && h > 1 )
   {
      LiveSkin const & skin = m_app.m_skin;

      QPainter dc( this );
      dc.setRenderHint( QPainter::NonCosmeticDefaultPen );

      dc.fillRect( rect(), skin.windowColor );
   }

   QWidget::paintEvent( event );
}
