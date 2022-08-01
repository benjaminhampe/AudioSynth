#include "DeviceBar.hpp"

inline Button*
createShowExplorerButton( LiveSkin const & skin, QWidget* parent )
{
   int bw = skin.circleButtonSize;
   int bh = skin.circleButtonSize;
   Button* btn = new Button( parent );
   btn->setCheckable( true );
   btn->setChecked( false );

   auto bgColor = skin.windowColor;
   auto fgColor = skin.focusColor; // or panelColor

   //QFont font = getFontAwesome( 14 );
   std::string
   msg =
         "#########\n"
         "#########\n"
         " #######\n"
         " #######\n"
         "  #####\n"
         "  #####\n"
         "   ###\n"
         "   ###\n"
         "    #\n"
         ;
   // [idle]
   QImage ico = createAsciiArt( bgColor, fgColor, msg );
   QImage img = createCircleImage( bw,bh, bgColor, fgColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]

   msg = "##\n"
         "####\n"
         "######\n"
         "########\n"
         "#########\n"
         "########\n"
         "######\n"
         "####\n"
         "##\n";

   ico = createAsciiArt( skin.activeColor, fgColor, msg );
   img = createCircleImage( bw,bh, bgColor, fgColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

DeviceBar::DeviceBar( App & app, QWidget* parent )
   : QWidget( parent )
   , m_app( app )
{
   setContentsMargins( 0,0,0,0 );
   setMinimumWidth(28);
   setMaximumWidth(28);

   LiveSkin const & skin = m_app.m_skin;
   m_btnShowExplorer = createShowExplorerButton( skin, this );
   m_btnInternDevice = createShowExplorerButton( skin, this );
   m_btnExternDevice = createShowExplorerButton( skin, this );
   m_btnExplorer1 = createShowExplorerButton( skin, this );
   m_btnExplorer2 = createShowExplorerButton( skin, this );
   m_btnExplorer3 = createShowExplorerButton( skin, this );
   m_leftScrollBar = new LeftScrollBar( m_app, this );
   m_btnQuickLibrary = createShowExplorerButton( skin, this );

   auto v = createVBox( 5 );
   v->addWidget( m_btnShowExplorer );
   v->addWidget( m_btnInternDevice );
   v->addWidget( m_btnExternDevice );
   v->addWidget( m_btnExplorer1 );
   v->addWidget( m_btnExplorer2 );
   v->addWidget( m_btnExplorer3 );
   v->addStretch( 1 );
   v->addWidget( m_leftScrollBar );
   v->addStretch( 1 );
   v->addWidget( m_btnQuickLibrary );
   setLayout( v );

   auto h = createHBox( 0 );
   h->addLayout( v );
   h->addStretch( 1 );
   setLayout( h );
}

DeviceBar::~DeviceBar()
{}

//void
//DeviceBar::resizeEvent( QResizeEvent* event )
//{
//   update();
//   QWidget::resizeEvent( event );
//}

void
DeviceBar::paintEvent( QPaintEvent* event )
{
   QWidget::paintEvent( event );
}
