#include "Live1_Panel.hpp"

inline Button*
createTapButton( LiveSkin const & skin, QWidget* parent )
{
   int bw = 32;
   int bh = 17;
   Button* btn = new Button( parent );

   QFont font = getFontAwesome( 14 );
   QString msg = "TAP";
   // [idle]
   QImage ico = createImageFromText( 0,0, msg, font, skin.eTextColor, skin.eFillColor );
   QImage img = createRectImage( bw,bh, skin.ePenColor, skin.eFillColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createImageFromText( 1,1, msg, font, skin.eTextColor, skin.activeColor );
   img = createRectImage( bw,bh, skin.ePenColor, skin.activeColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

inline Button*
createBpmEdit( LiveSkin const & skin, QWidget* parent )
{
   int bw = 48;
   int bh = 17;
   Button* btn = new Button( parent );

   QFont font = getFontAwesome( 14 );
   QString msg = "120.00";
   // [idle]
   QImage ico = createImageFromText( 1,1, msg, font, skin.eTextColor, skin.eFillColor );
   QImage img = createRectImage( bw,bh, skin.ePenColor, skin.eFillColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createImageFromText( 1,1, msg, font, skin.eTextColor, skin.activeColor );
   img = createRectImage( bw,bh, skin.ePenColor, skin.activeColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}


inline Button*
createNudgeSlowButton( LiveSkin const & skin, QWidget* parent )
{
   int bw = 16;
   int bh = 17;
   Button* btn = new Button( parent );
   btn->setCheckable( true );
   btn->setChecked( false );

   std::string
   msg = "## #  #   #\n"
         "## #  #   #\n"
         "## #  #   #\n"
         "## #  #   #\n"
         "## #  #   #\n"
         "## #  #   #\n"
         "## #  #   #\n";

   // [idle]
   QImage ico = createAsciiArt( skin.eTextColor, skin.eFillColor, msg );
   QImage img = createRectImage( bw,bh, skin.ePenColor, skin.eFillColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( skin.eTextColor, skin.activeColor, msg );
   img = createRectImage( bw,bh, skin.ePenColor, skin.activeColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

inline Button*
createNudgeFastButton( LiveSkin const & skin, QWidget* parent )
{
   int bw = 16;
   int bh = 17;
   Button* btn = new Button( parent );
   btn->setCheckable( true );
   btn->setChecked( false );

   std::string
   msg = "#   #  # ##\n"
         "#   #  # ##\n"
         "#   #  # ##\n"
         "#   #  # ##\n"
         "#   #  # ##\n"
         "#   #  # ##\n"
         "#   #  # ##\n";

   // [idle]
   QImage ico = createAsciiArt( skin.eTextColor, skin.eFillColor, msg );
   QImage img = createRectImage( bw,bh, skin.ePenColor, skin.eFillColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( skin.eTextColor, skin.activeColor, msg );
   img = createRectImage( bw,bh, skin.ePenColor, skin.activeColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

inline Button*
createBeatSignatureEdit( LiveSkin const & skin, QWidget* parent )
{
   int bw = 50;
   int bh = 17;
   Button* btn = new Button( parent );

   QFont font = getFontAwesome( 14 );
   QString msg = "  4  /  4  ";
   // [idle]
   QImage ico = createImageFromText( 1,1, msg, font, skin.eTextColor, skin.eFillColor );
   QImage img = createRectImage( bw,bh, skin.ePenColor, skin.eFillColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createImageFromText( 1,1, msg, font, skin.eTextColor, skin.activeColor );
   img = createRectImage( bw,bh, skin.ePenColor, skin.activeColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

inline Button*
createMetronomToggle( LiveSkin const & skin, QWidget* parent )
{
   int bw = 24;
   int bh = 17;
   Button* btn = new Button( parent );
   btn->setCheckable( true );
   btn->setChecked( false );

   //QFont font = getFontAwesome( 14 );
   std::string msg = "  ###      ###\n"
                  " #   #    #####\n"
                  "#     #  #######\n"
                  "#     #  #######\n"
                  "#     #  #######\n"
                  " #   #    #####\n"
                  "  ###      ###\n";
   // [idle]
   QImage ico = createAsciiArt( skin.eTextColor, skin.eFillColor, msg );
   QImage img = createRectImage( bw,bh, skin.ePenColor, skin.eFillColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( skin.eTextColor, skin.activeColor, msg );
   img = createRectImage( bw,bh, skin.ePenColor, skin.activeColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

inline Button*
createDisplayFollowToggle( LiveSkin const & skin, QWidget* parent )
{
   int bw = 24;
   int bh = 17;
   Button* btn = new Button( parent );
   btn->setCheckable( true );
   btn->setChecked( false );

   //QFont font = getFontAwesome( 14 );
   std::string msg = "        #\n"
                  "     #  ##\n"
                  " #  ### ###\n"
                  "     #  ##\n"
                  "        #\n";
   // [idle]
   QImage ico = createAsciiArt( skin.eTextColor, skin.eFillColor, msg );
   QImage img = createRectImage( bw,bh, skin.ePenColor, skin.eFillColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( skin.eTextColor, skin.activeColor, msg );
   img = createRectImage( bw,bh, skin.ePenColor, skin.activeColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

inline Button*
createArrangmentPosEdit( LiveSkin const & skin, QWidget* parent )
{
   int bw = 86;
   int bh = 17;
   Button* btn = new Button( parent );

   QFont font = getFontAwesome( 14 );
   QString msg = "    1 .   1 .   1";
   // [idle]
   QImage ico = createImageFromText( 1,1, msg, font, skin.eTextColor, skin.eFillColor );
   QImage img = createRectImage( bw,bh, skin.ePenColor, skin.eFillColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createImageFromText( 1,1, msg, font, skin.eTextColor, skin.activeColor );
   img = createRectImage( bw,bh, skin.ePenColor, skin.activeColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

inline Button*
createPlayButton( LiveSkin const & skin, QWidget* parent )
{
   int bw = 17;
   int bh = 17;
   Button* btn = new Button( parent );
   btn->setCheckable( true );
   btn->setChecked( false );

   QFont font = getFontAwesome( 14 );
   QString msg = QChar(fa::play);

   // [idle]
   QImage ico = createImageFromText( 1,1, msg, font, skin.eTextColor, skin.eFillColor );
   QImage img = createRectImage( bw,bh, skin.ePenColor, skin.eFillColor, ico, 0, 1 );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createImageFromText( 1,1, msg, font, skin.playColor, skin.eFillColor );
   img = createRectImage( bw,bh, skin.ePenColor, skin.eFillColor, ico, 0, 1 );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

inline Button*
createStopButton( LiveSkin const & skin, QWidget* parent )
{
   int bw = 17;
   int bh = 17;
   Button* btn = new Button( parent );
   QFont font = getFontAwesome( 14 );
   QString msg = QChar(fa::stop);

   // [idle]
   QImage ico = createImageFromText( 1,1, msg, font, skin.eTextColor, skin.eFillColor );
   QImage img = createRectImage( bw,bh, skin.ePenColor, skin.eFillColor, ico, 0, 1 );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createImageFromText( 1,1, msg, font, skin.eTextColor, skin.eFillColor );
   img = createRectImage( bw,bh, skin.ePenColor, skin.eFillColor, ico, 0, 1 );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

inline Button*
createRecordButton( LiveSkin const & skin, QWidget* parent )
{
   int bw = 16;
   int bh = 17;
   Button* btn = new Button( parent );
   btn->setCheckable( true );
   btn->setChecked( false );

   QFont font = getFontAwesome( 14 );
   QString msg = QChar( 0xf111 );

   // [idle]
   QImage ico = createImageFromText( 1,1, msg, font, skin.eTextColor, skin.eFillColor );
   QImage img = createRectImage( bw,bh, skin.ePenColor, skin.eFillColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createImageFromText( 1,1, msg, font, skin.recColor, skin.eFillColor );
   img = createRectImage( bw,bh, skin.ePenColor, skin.eFillColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

inline Button*
createOverdubButton( LiveSkin const & skin, QWidget* parent )
{
   int bw = 32;
   int bh = 17;
   Button* btn = new Button( parent );
   btn->setCheckable( true );
   btn->setChecked( false );

   QFont font = getFontAwesome( 14 );
   QString msg = "OVR";

   // [idle]
   QImage ico = createImageFromText( 1,1, msg, font, skin.eTextColor, skin.eFillColor );
   QImage img = createRectImage( bw,bh, skin.ePenColor, skin.eFillColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createImageFromText( 1,1, msg, font, skin.eTextColor, skin.activeColor );
   img = createRectImage( bw,bh, skin.ePenColor, skin.activeColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

inline Button*
createBackToArrangementButton( LiveSkin const & skin, QWidget* parent )
{
   int bw = 24;
   int bh = 17;
   Button* btn = new Button( parent );

   std::string
   msg = "#########\n"
         "             #\n"
         "            ##\n"
         "#########  #######\n"
         "            ##\n"
         "             #\n"
         "#########\n";
   // [idle]
   QImage ico = createAsciiArt( skin.eTextColor, skin.eFillColor, msg );
   QImage img = createRectImage( bw,bh, skin.ePenColor, skin.eFillColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( skin.eTextColor, skin.activeColor, msg );
   img = createRectImage( bw,bh, skin.ePenColor, skin.activeColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

inline Button*
createBarComboBox( LiveSkin const & skin, QWidget* parent )
{
   int bw = 48;
   int bh = 17;
   Button* btn = new Button( parent );
   btn->setCheckable( true );
   btn->setChecked( false );

   QFont font = getFontAwesome( 14 );
   QString msg = "1 Bar v";

   // [idle]
   QImage ico = createImageFromText( 1,1, msg, font, skin.eTextColor, skin.eFillColor );
   QImage img = createRectImage( bw,bh, skin.ePenColor, skin.eFillColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createImageFromText( 1,1, msg, font, skin.eTextColor, skin.activeColor );
   img = createRectImage( bw,bh, skin.ePenColor, skin.activeColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

inline Button*
createPenButton( LiveSkin const & skin, QWidget* parent )
{
   int bw = 24;
   int bh = 17;
   Button* btn = new Button( parent );
   btn->setCheckable( true );
   btn->setChecked( false );

   //QFont font = getFontAwesome( 14 );
   std::string msg = "      #\n"
                  "     ###\n"
                  "    ## ##\n"
                  "   ## ####\n"
                  "  ## ####\n"
                  " ## ####\n"
                  "## ####\n"
                  "# ####\n"
                  "#  ##\n"
                  "####\n";
   // [idle]
   QImage ico = createAsciiArt( skin.eTextColor, skin.eFillColor, msg );
   QImage img = createRectImage( bw,bh, skin.ePenColor, skin.eFillColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( skin.eTextColor, skin.activeColor, msg );
   img = createRectImage( bw,bh, skin.ePenColor, skin.activeColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

inline Button*
createPunchInButton( LiveSkin const & skin, QWidget* parent )
{
   int bw = 24;
   int bh = 17;
   Button* btn = new Button( parent );
   //QFont font = getFontAwesome( 14 );
   std::string msg = "######\n"
                     "      #\n"
                     "       #\n"
                     "        #\n"
                     "         ######\n";
   // [idle]
   QImage ico = createAsciiArt( skin.eTextColor, skin.eFillColor, msg );
   QImage img = createRectImage( bw,bh, skin.ePenColor, skin.eFillColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( skin.eTextColor, skin.activeColor, msg );
   img = createRectImage( bw,bh, skin.ePenColor, skin.activeColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}


inline Button*
createLoopButton( LiveSkin const & skin, QWidget* parent )
{
   int bw = 24;
   int bh = 17;
   Button* btn = new Button( parent );
   std::string msg = " ###########\n"
                     "#           #\n"
                     "#           #\n"
                     "#           #\n"
                     "#           #\n"
                     "#       #   #\n"
                     "#      ##   #\n"
                     " ###  ######\n"
                     "       ##\n"
                     "        #\n";
   // [idle]
   QImage ico = createAsciiArt( skin.eTextColor, skin.eFillColor, msg );
   QImage img = createRectImage( bw,bh, skin.ePenColor, skin.eFillColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( skin.eTextColor, skin.activeColor, msg );
   img = createRectImage( bw,bh, skin.ePenColor, skin.activeColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

inline Button*
createPunchOutButton( LiveSkin const & skin, QWidget* parent )
{
   int bw = 24;
   int bh = 17;
   Button* btn = new Button( parent );
   std::string msg = "         ######\n"
                     "        #\n"
                     "       #\n"
                     "      #\n"
                     "######\n";
   // [idle]
   QImage ico = createAsciiArt( skin.eTextColor, skin.eFillColor, msg );
   QImage img = createRectImage( bw,bh, skin.ePenColor, skin.eFillColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( skin.eTextColor, skin.activeColor, msg );
   img = createRectImage( bw,bh, skin.ePenColor, skin.activeColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

inline Button*
createKeyMidiButton( LiveSkin const & skin, QWidget* parent )
{
   int bw = 16;
   int bh = 17;
   Button* btn = new Button( parent );
   std::string msg = "ooooooooooo\n"
                     "o o#o o#o o\n"
                     "o o#o o#o o\n"
                     "o o#o o#o o\n"
                     "o o#o o#o o\n"
                     "o o#o o#o o\n"
                     //"o o#o o#o o\n"
                     "o  o   o  o\n"
                     "o  o   o  o\n"
                     "o  o   o  o\n"
                     "o  o   o  o\n"
                     "ooooooooooo\n";
   // [idle]
   QImage ico = createAsciiArt( skin.eTextColor, skin.eFillColor, msg );
   QImage img = createRectImage( bw,bh, skin.ePenColor, skin.eFillColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( skin.eTextColor, skin.activeColor, msg );
   img = createRectImage( bw,bh, skin.ePenColor, skin.activeColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

inline Button*
createKeyMidiIn( LiveSkin const & skin, QWidget* parent )
{
   int bw = 8;
   int bh = 8;
   Button* btn = new Button( parent );

   // [idle]
   QImage img = createRectImage( bw,bh, skin.ePenColor, skin.eFillColor );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   img = createRectImage( bw,bh, skin.ePenColor, skin.activeColor );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

inline Button*
createKeyMidiOut( LiveSkin const & skin, QWidget* parent )
{
   int bw = 8;
   int bh = 8;
   Button* btn = new Button( parent );

   // [idle]
   QImage img = createRectImage( bw,bh, skin.ePenColor, skin.eFillColor );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   img = createRectImage( bw,bh, skin.ePenColor, skin.activeColor );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

Live1_Panel::Live1_Panel( App & app, QWidget* parent )
   : QWidget( parent )
   , m_app( app )
{
   setObjectName( "Live1_Panel" );
   setContentsMargins(0,0,0,0);
   setMinimumHeight( 26 );
   setMaximumHeight( 26 );

   LiveSkin const & skin = m_app.m_skin;

   m_tapButton = createTapButton( skin, this );
   m_bpmEdit = createBpmEdit( skin, this );
   m_nudgeSlow = createNudgeSlowButton( skin, this );
   m_nudgeFast = createNudgeFastButton( skin, this );
   m_beatSig = createBeatSignatureEdit( skin, this );
   m_metronom = createMetronomToggle( skin, this );
   m_displayFollow = createDisplayFollowToggle( skin, this );
   m_edtArrangementPos = createArrangmentPosEdit( skin, this );
   m_playButton = createPlayButton( skin, this );
   m_stopButton = createStopButton( skin, this );
   m_recButton = createRecordButton( skin, this );
   m_overdubButton = createOverdubButton( skin, this );
   m_backToArrangement = createBackToArrangementButton( skin, this );
   m_barCombo = createBarComboBox( skin, this );
   m_penButton = createPenButton( skin, this );
   //m_beat2 = createButton( "  1.  1.  1", this );

   m_edtLoopStart = createArrangmentPosEdit( skin, this );
   m_togPunchIn = createPunchInButton( skin, this );
   m_togLooping = createLoopButton( skin, this );
   m_togPunchOut = createPunchOutButton( skin, this );
   m_edtLoopLength = createArrangmentPosEdit( skin, this );

   m_togKeyMidi = createKeyMidiButton( skin, this );
   m_ledKeyMidiIn = createKeyMidiIn( skin, this );
   m_ledKeyMidiOut = createKeyMidiOut( skin, this );
   m_togKeyMapping = createPenButton( skin, this );
   m_togMidiMapping = createPenButton( skin, this );
   m_barCpuUsage = createPenButton( skin, this );
   m_ledHddOverload = createPenButton( skin, this );
   m_ledLiveMidiIn = createPenButton( skin, this );
   m_ledLiveMidiOut = createPenButton( skin, this );


   auto h = new QHBoxLayout();
   h->setContentsMargins(0,0,0,0);
   h->setSpacing( 2 );
   h->addWidget( m_tapButton );
   h->addWidget( m_bpmEdit );
   h->addWidget( m_nudgeSlow );
   h->addWidget( m_nudgeFast );
   h->addWidget( m_beatSig );
   h->addWidget( m_metronom );

   h->addStretch( 1 );
   h->addWidget( m_displayFollow );
   h->addWidget( m_edtArrangementPos );
   h->addWidget( m_playButton );
   h->addWidget( m_stopButton );
   h->addWidget( m_recButton );
   h->addWidget( m_overdubButton );
   h->addWidget( m_backToArrangement );
   h->addWidget( m_barCombo );
   h->addWidget( m_penButton );
   h->addStretch( 1 );

   h->addWidget( m_edtLoopStart );
   h->addWidget( m_togPunchIn );
   h->addWidget( m_togLooping );
   h->addWidget( m_togPunchOut );
   h->addWidget( m_edtLoopLength );

   h->addStretch( 1 );

   h->addWidget( m_togKeyMidi );
   h->addWidget( m_ledKeyMidiIn );
   h->addWidget( m_ledKeyMidiOut );
   h->addWidget( m_togKeyMapping );
   h->addWidget( m_togMidiMapping );
   h->addWidget( m_barCpuUsage );
   h->addWidget( m_ledHddOverload );
   h->addWidget( m_ledLiveMidiIn );
   h->addWidget( m_ledLiveMidiOut );

   auto v = new QVBoxLayout();
   v->setContentsMargins(0,0,0,0);
   v->setSpacing( 0 );
   //v->addStretch( 1 );
   v->addLayout( h );
   v->addStretch( 1 );

   setLayout( v );

   // m_mixer->setInputSignal( 0, m_detuneSynth );
   // m_mixer->setInputSignal( 1, m_deckAB );
   // m_mixer->setInputSignal( 2, m_deckCD );
   // m_mixer->setInputSignal( 3, m_deckEF );
   // m_mixer->setInputSignal( 4, m_betterSynth );
   // m_mixer->setInputSignal( 5, m_vstDeck );

   // m_master->setInputSignal( 0, m_mixer );

   // //m_master->m_timeStretch->setInputSignal( m_mixer );
   // m_master->m_delay->setInputSignal( 0, m_mixer );
   // m_master->m_pan->setInputSignal( 0, m_master->m_delay );
   // m_master->m_lowPass3->setInputSignal( 0, m_master->m_pan );
   // m_master->m_overdrive->setInputSignal( m_master->m_lowPass3 );
   // m_master->m_lowPass4->setInputSignal( 0, m_master->m_overdrive );
   // m_master->m_bassBoost->setInputSignal( m_master->m_lowPass4 );
   // m_master->m_compressor->setInputSignal( 0, m_master->m_bassBoost );
   // m_master->m_limiter->setInputSignal( 0, m_master->m_compressor );
   // m_master->m_waveform->setInputSignal( 0, m_master->m_limiter );
   // m_waveform3D->setInputSignal( 0, m_master->m_waveform );
   // m_master->m_spektrum->setInputSignal( m_waveform3D );
   // m_master->m_volume->setInputSignal( 0, m_master->m_spektrum );
   // m_master->m_levelMeter->setInputSignal( m_master->m_volume );
   // m_audioRecorder->setInputSignal( 0, m_master->m_levelMeter );
   // m_master->m_menu->setInputSignal( 0, m_audioRecorder );
   // //m_waveform->setInputSignal( 0, m_inputSignal );


   // m_endPoint.setInputSignal( 0, m_master );
   // m_endPoint.play();

   // m_Piano2D->setSynth( 0, m_detuneSynth );
   // m_Piano2D->setSynth( 1, m_betterSynth );
   // m_Piano2D->setSynth( 2, m_vstDeck );


   connect( m_playButton, SIGNAL(clicked(bool)), this, SLOT(on_playButton(bool)) );
   connect( m_stopButton, SIGNAL(clicked(bool)), this, SLOT(on_stopButton(bool)) );
}

Live1_Panel::~Live1_Panel()
{

}

void
Live1_Panel::on_playButton( bool checked )
{
   play();
}

void
Live1_Panel::on_stopButton( bool checked )
{
   stop();
}

void
Live1_Panel::play()
{
   m_app.m_audioEndPoint.play();
}

void
Live1_Panel::stop()
{
   m_app.m_audioEndPoint.stop();
}
