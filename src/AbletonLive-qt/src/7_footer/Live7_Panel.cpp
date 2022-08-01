#include "Live7_Panel.hpp"

LongTextDisplay::LongTextDisplay( App & app, QWidget* parent )
   : QWidget( parent )
   , m_app( app )
   , m_longText( "Empty" )
{
   setContentsMargins(0,0,0,0);
   setMinimumHeight( 16 );
   setMaximumHeight( 16 );
   //setMinimumWidth(30);
}

void
LongTextDisplay::paintEvent( QPaintEvent* event )
{
   int w = width();
   int h = height();
   if ( w > 1 && h > 1 )
   {
      LiveSkin const & skin = m_app.m_skin;

      QPainter dc( this );
      dc.setRenderHint( QPainter::NonCosmeticDefaultPen );
      dc.fillRect( rect(), skin.contentColor );
      dc.setPen( QPen( skin.windowColor ) );
      dc.setFont( QFont("Courier New", 14 ) );
      dc.drawText( 10, 12, m_longText );
      dc.end();
   }

   QWidget::paintEvent( event );
}

MidiDisplay::MidiDisplay( App & app, QWidget* parent )
   : QWidget( parent )
   , m_app( app )
{
   setContentsMargins(0,0,0,0);
   setMinimumHeight( 16 );
   setMaximumHeight( 16 );
   setMinimumWidth(200);
}

void
MidiDisplay::paintEvent( QPaintEvent* event )
{
   int w = width();
   int h = height();
   if ( w > 1 && h > 1 )
   {
      LiveSkin const & skin = m_app.m_skin;

      QPainter dc( this );
      dc.setRenderHint( QPainter::NonCosmeticDefaultPen );
      dc.fillRect( rect(), skin.contentColor );
      dc.end();
   }

   QWidget::paintEvent( event );
}

AudioDisplay::AudioDisplay( App & app, QWidget* parent )
   : QWidget( parent )
   , m_app( app )
{
   setContentsMargins(0,0,0,0);
   setMinimumHeight( 16 );
   setMaximumHeight( 16 );
   //setMinimumWidth(30);
   setMinimumWidth(64);
}

void
AudioDisplay::paintEvent( QPaintEvent* event )
{
   int w = width();
   int h = height();
   if ( w > 1 && h > 1 )
   {
      LiveSkin const & skin = m_app.m_skin;

      QPainter dc( this );
      dc.setRenderHint( QPainter::NonCosmeticDefaultPen );
      dc.fillRect( rect(), skin.focusColor );
      dc.end();
   }

   QWidget::paintEvent( event );
}

inline Button*
createShowHelpButton( LiveSkin const & skin, QWidget* parent )
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
   msg = "##\n"
         "####\n"
         "######\n"
         "########\n"
         "#########\n"
         "########\n"
         "######\n"
         "####\n"
         "##\n";

   // [idle]
   QImage ico = createAsciiArt( bgColor, fgColor, msg );
   QImage img = createCircleImage( bw,bh, bgColor, fgColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   msg = "    #\n"
         "   ###\n"
         "   ###\n"
         "  #####\n"
         "  #####\n"
         " #######\n"
         " #######\n"
         "#########\n"
         "#########\n";

   ico = createAsciiArt( skin.activeColor, fgColor, msg );
   img = createCircleImage( bw,bh, bgColor, fgColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

inline Button*
createShowPanelButton( LiveSkin const & skin, QWidget* parent )
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
   msg = "       ##\n"
         "     ####\n"
         "   ######\n"
         " ########\n"
         "#########\n"
         " ########\n"
         "   ######\n"
         "     ####\n"
         "       ##\n";

   // [idle]
   QImage ico = createAsciiArt( bgColor, fgColor, msg );
   QImage img = createCircleImage( bw,bh, bgColor, fgColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   msg = "    #\n"
         "   ###\n"
         "   ###\n"
         "  #####\n"
         "  #####\n"
         " #######\n"
         " #######\n"
         "#########\n"
         "#########\n";

   ico = createAsciiArt( skin.activeColor, fgColor, msg );
   img = createCircleImage( bw,bh, bgColor, fgColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

Live7_Panel::Live7_Panel( App & app, QWidget* parent )
   : QWidget( parent )
   , m_app( app )
{
   setContentsMargins(0,10,0,0);
   LiveSkin const & skin = m_app.m_skin;
   m_showHelpButton = createShowHelpButton( skin, this );
   m_showPanelButton = createShowPanelButton( skin, this );
   m_longText = new LongTextDisplay( m_app, this );
   m_midi = new MidiDisplay( m_app, this );
   m_audio = new AudioDisplay( m_app, this );
   m_longTextPanel = new Live0_Panel( m_app, this );
   m_midiPanel = new Live0_Panel( m_app, this );
   m_audioPanel = new Live0_Panel( m_app, this );

   auto p1 = createHBox();
   p1->addWidget( m_longText );
   m_longTextPanel->setLayout( p1 );

   auto p2 = createHBox();
   p2->addWidget( m_midi );
   m_midiPanel->setLayout( p2 );

   auto p3 = createHBox();
   p3->addWidget( m_audio );
   m_audioPanel->setLayout( p3 );

   auto h = createHBox( 10 );
   h->addWidget( m_showHelpButton );
   h->addWidget( m_longTextPanel,1 );
   h->addWidget( m_midiPanel );
   h->addWidget( m_audioPanel );
   h->addWidget( m_showPanelButton );
   setLayout( h );

   // Show/Hide PluginExplorer
//   connect( m_showHelpButton, SIGNAL(clicked(bool)), this, SLOT(on_showExplorer(bool)) );

   // Show/Hide PluginExplorer
//   connect( m_footerPanel->m_showHelpButton, SIGNAL(showExplorer(bool)), this, SLOT(on_showExplorer(bool)) );
}

//void
//Live7_Panel::on_showExplorer( bool checked )
//{
//   //m_splitPlugCompo->refresh();
//   if ( !checked )
//   {

//   }

////   else
////   {
////      m_pluginListPanel->hide();
////   }
//   update();
//}


void
Live7_Panel::paintEvent( QPaintEvent* event )
{
   int w = width();
   int h = height();

   if ( w > 1 && h > 1 )
   {
      LiveSkin const & skin = m_app.m_skin;

      QPainter dc( this );
      dc.setRenderHint( QPainter::NonCosmeticDefaultPen );
      dc.fillRect( rect(), skin.windowColor );

//      int px = 0;
//      int py = 0;
//      int btnW = 20;
//      int btnD = 16;
//      int scrollbarH = h - 8 * ( btnW ) - 2 * py;
//      dc.setPen( Qt::NoPen );

//      // Draw scrollbar
//      int x = 5;
//      int y = 5;

//      dc.setBrush( QBrush( skin.panelColor ) );

//      dc.drawEllipse( x, y, btnW, btnW );
//      y += btnW + py;
//      dc.drawEllipse( x, y, btnW, btnW );
//      y += btnW + py;
//      dc.drawEllipse( x, y, btnW, btnW );
//      y += btnW + py;
//      dc.drawEllipse( x, y, btnW, btnW );
//      y += btnW + py;
//      dc.drawEllipse( x, y, btnW, btnW );
//      y += btnW + py;
//      dc.drawEllipse( x, y, btnW, btnW );
//      y += btnW + py;

      dc.end();
   }

   QWidget::paintEvent( event );
}
