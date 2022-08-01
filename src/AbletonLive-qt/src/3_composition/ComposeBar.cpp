#include "ComposeBar.hpp"


inline Button*
createArrangementButton( LiveSkin const & skin, QWidget* parent )
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
   msg = "###########\n"
         "###########\n"
         "###########\n"
         " \n"
         " \n"
         "###########\n"
         "###########\n"
         "###########\n"
         " \n"
         " \n"
         "###########\n"
         "###########\n"
         "###########\n"
         ;

   // [idle]
   QImage ico = createAsciiArt( bgColor, fgColor, msg );
   QImage img = createCircleImage( bw,bh, bgColor, fgColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( skin.activeColor, fgColor, msg );
   img = createCircleImage( bw,bh, bgColor, fgColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

inline Button*
createSessionButton( LiveSkin const & skin, QWidget* parent )
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
   msg = "###  ###  ###\n"
         "###  ###  ###\n"
         "###  ###  ###\n"
         "###  ###  ###\n"
         "###  ###  ###\n"
         "###  ###  ###\n"
         "###  ###  ###\n"
         "###  ###  ###\n"
         "###  ###  ###\n"
         ;

   // [idle]
   QImage ico = createAsciiArt( bgColor, fgColor, msg );
   QImage img = createCircleImage( bw,bh, bgColor, fgColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( skin.activeColor, fgColor, msg );
   img = createCircleImage( bw,bh, bgColor, fgColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

inline Button*
createQuickLibraryButton( LiveSkin const & skin, QWidget* parent )
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
   msg = "###  ###  ###\n"
         "###  ###  ###\n"
         "###  ###  ###\n"
         "###  ###  ###\n"
         "###  ###  ###\n"
         "###  ###  ###\n"
         "###  ###  ###\n"
         "###  ###  ###\n"
         "###  ###  ###\n"
         ;

   // [idle]
   QImage ico = createAsciiArt( bgColor, fgColor, msg );
   QImage img = createCircleImage( bw,bh, bgColor, fgColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( skin.activeColor, fgColor, msg );
   img = createCircleImage( bw,bh, bgColor, fgColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

ComposeBar::ComposeBar( App & app, QWidget* parent )
   : QWidget( parent )
   , m_app( app )
{
   setMinimumWidth(33);
   setMaximumWidth(33);

   LiveSkin const & skin = m_app.m_skin;
   BeatGrid const & grid = m_app.m_beatGrid;

   m_btnShowArrangement = createArrangementButton( skin, this );
   m_btnShowSession = createSessionButton( skin, this );
   m_btnQuickLibrary = createQuickLibraryButton( skin, this );

   auto v = new QVBoxLayout();
   v->setContentsMargins(5,10,5,10);
   v->setSpacing( 5 );
   v->addWidget( m_btnShowArrangement );
   v->addWidget( m_btnShowSession );
   v->addStretch( 1 );
   v->addWidget( m_btnQuickLibrary );
   setLayout( v );


   connect( m_btnShowArrangement, SIGNAL(clicked(bool)), this, SLOT(on_btnShowArrangement(bool)) );
   connect( m_btnShowSession, SIGNAL(clicked(bool)), this, SLOT(on_btnShowSession(bool)) );
   //connect( m_stopButton, SIGNAL(clicked(bool)), this, SLOT(on_stopButton(bool)) );

}

ComposeBar::~ComposeBar()
{

}

void
ComposeBar::on_btnShowArrangement( bool checked )
{
   emit showArrangement( checked );
}

void
ComposeBar::on_btnShowSession( bool checked )
{
   emit showSession( checked );
}

//void
//ComposeBar::resizeEvent( QResizeEvent* event )
//{
//   update();
//   QWidget::resizeEvent( event );
//}

void
ComposeBar::paintEvent( QPaintEvent* event )
{
   QWidget::paintEvent( event );
}
