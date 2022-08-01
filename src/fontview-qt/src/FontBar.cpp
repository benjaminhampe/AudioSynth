#include "FontBar.hpp"
#include <DarkImage.hpp>
namespace {

QToolButton*
createToolButton( QString const & buttonText, QWidget* parent )
{
   QToolButton* b = nullptr;
   b = new QToolButton( parent );
   //b->setMinimumSize( 32, 32 );
   b->setText( buttonText );
   b->setFont( QFont( "fontawesome", 16 ) );
   return b;
}

}

FontBar::FontBar( QWidget * parent )
   : QToolBar( parent )
   , m_CurrFamily( "fontawesome" )
   , m_CurrLineColor( 0,0,0,255 )
   , m_CurrFillColor( 50,50,50,255 )
   , m_CurrPointSize( 12 )
   , m_CurrWeight( 400 )
   , m_IsBold( false )
   , m_IsItalic( false )
   , m_IsStriked( false )
   , m_IsUnderlined( false )
   , m_CurrTextAlign( 0 )
{
   setContentsMargins( 0,0,0,0 );

   m_glyphIndex = new QComboBox( this );

   QStringList items;
   for ( uint32_t i = 0; i < 128*128*128; i += 128 )
   {
      items << QString::fromStdString( dbStrHex( i ) );
   }
   m_glyphIndex->addItems( items );

   m_FontInfo = new QPushButton( "FontInfo: ", this );

   m_FontFamily = new QComboBox( this );

   QStringList symbolFams = QFontDatabase().families( QFontDatabase::Symbol );
   for ( int i = 0; i < symbolFams.count(); ++i )
   {
      m_FontFamily->addItem( symbolFams[ i ], symbolFams[ i ] );
   }

   QStringList fams = QFontDatabase().families( QFontDatabase::Latin );
   for ( int i = 0; i < fams.count(); ++i )
   {
      m_FontFamily->addItem( fams[ i ], fams[ i ] );
   }

   m_FontSize = new QComboBox( this );
   for ( int i = 1; i < 25; ++i )
   {
      m_FontSize->addItem( QString::number( i ), QVariant( i ) );
   }

   m_FontSize->addItem( QString::number( 28 ), QVariant( 28 ) );
   m_FontSize->addItem( QString::number( 30 ), QVariant( 30 ) );
   m_FontSize->addItem( QString::number( 32 ), QVariant( 32 ) );
   m_FontSize->addItem( QString::number( 36 ), QVariant( 36 ) );
   m_FontSize->addItem( QString::number( 42 ), QVariant( 42 ) );
   m_FontSize->addItem( QString::number( 48 ), QVariant( 48 ) );
   m_FontSize->addItem( QString::number( 64 ), QVariant( 64 ) );
   m_FontSize->addItem( QString::number( 72 ), QVariant( 72 ) );
   m_FontSize->addItem( QString::number( 96 ), QVariant( 96 ) );
   m_FontSize->addItem( QString::number( 100 ), QVariant( 100 ) );
   m_FontSize->addItem( QString::number( 128 ), QVariant( 128 ) );

   m_LineColor = new ColorButton( "Line", 0xFFFFFFFF, this );
   m_FillColor = new ColorButton( "Fill", 0xFF000000, this );

   m_Bold = createToolButton( QChar( fa::bold ), this );
   m_Italic = createToolButton( QChar( fa::italic ), this );
   m_Underline = createToolButton( QChar( fa::underline ), this );
   m_StrikeThrough = createToolButton( QChar( fa::strikethrough ), this );

   m_Bold->setCheckable( true );
   m_Italic->setCheckable( true );
   m_Underline->setCheckable( true );
   m_StrikeThrough->setCheckable( true );

   addWidget( m_glyphIndex );
   addWidget( m_FontInfo );
   addWidget( m_FontFamily );
   addWidget( m_FontSize );
   addWidget( m_LineColor );
   addWidget( m_FillColor );
   addWidget( m_Bold );
   addWidget( m_Italic );
   addWidget( m_Underline );
   addWidget( m_StrikeThrough );

   connect( m_glyphIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(on_glyphIndexChanged(int)) );

   void (QComboBox:: *indexChangedSignal)( int ) = &QComboBox::currentIndexChanged;
   connect( m_FontFamily, indexChangedSignal, this, &FontBar::onComboIndex_Family );
   connect( m_FontSize, indexChangedSignal, this, &FontBar::onComboIndex_Size );

   connect( m_FontInfo, &QPushButton::clicked, this, &FontBar::onButtonClick_Font );
   connect( m_Bold, &QPushButton::clicked, this, &FontBar::onButtonClick_Bold );
   connect( m_Italic, &QPushButton::clicked, this, &FontBar::onButtonClick_Italic );
   connect( m_Underline, &QPushButton::clicked, this, &FontBar::onButtonClick_Underline );
   connect( m_StrikeThrough, &QPushButton::clicked, this, &FontBar::onButtonClick_StrikeThrough );

   m_CurrFont = font();
   m_CurrFamily = m_CurrFont.family();
   m_CurrWeight = m_CurrFont.weight();
   m_CurrPointSize = m_CurrFont.pointSize();
   m_IsItalic = m_CurrFont.italic();
   updateFont();
}

FontBar::~FontBar()
{

}

void
FontBar::on_glyphIndexChanged( int value )
{
   emit glyphIndexChanged( 128 * value );
}


void
FontBar::updateFont()
{
   emit fontChanged( m_CurrFont );

   std::stringstream s;
   s << m_CurrFamily.toStdString();
   if ( m_IsBold ) s << " Bold";
   if ( m_IsItalic ) s << " Italic";
   s << ", " << m_CurrPointSize << " pt";
   m_FontInfo->setText( QString::fromStdString( s.str() ) );

   QFont displayFont = m_CurrFont;
   displayFont.setPointSize( 10 );
   m_FontInfo->setFont( displayFont );
   qDebug() << "Changed Font: " << s.str().c_str() << ".";
}

void
FontBar::onButtonClick_Font( bool checked )
{
   QFontDialog dlg;
   dlg.setCurrentFont( m_CurrFont );
   if ( dlg.exec() != QFontDialog::Accepted )
   {
      return;
   }

   m_CurrFont = dlg.currentFont();
   m_CurrFamily = m_CurrFont.family();
   m_CurrWeight = m_CurrFont.weight();
   m_CurrPointSize = m_CurrFont.pointSize();
   m_IsItalic = m_CurrFont.italic();

   int found = -1;
   for ( int i = 0; i < m_FontFamily->count(); ++i )
   {
      if ( m_FontFamily->itemData( i ).toString() == m_CurrFamily )
      {
         found = i;
         break;
      }
   }

   if ( found > -1 )
   {
      m_FontFamily->setCurrentIndex( found );
   }

   found = -1;
   for ( int i = 0; i < m_FontSize->count(); ++i )
   {
      if ( m_FontSize->itemData( i ).toInt() == m_CurrPointSize )
      {
         found = i;
         break;
      }
   }

   if ( found > -1 )
   {
      m_FontSize->setCurrentIndex( found );
   }
   else
   {
      m_FontSize->addItem( QString::number( m_CurrPointSize ), m_CurrPointSize );
      m_FontSize->setCurrentIndex( m_FontSize->count() - 1 );
   }

   if ( m_Italic )
   {
      m_Italic->setChecked( m_IsItalic );
   }

   if ( m_Bold )
   {
      m_Bold->setChecked( ( m_CurrWeight >= 75 ) );
   }

   updateFont();
}

void
FontBar::onComboIndex_Family( int )
{
   int currentIndex = m_FontFamily->currentIndex();
   if ( currentIndex < 0 )
   {
      m_CurrFamily = "fontawesome";
   }
   else
   {
      m_CurrFamily = m_FontFamily->itemData( currentIndex ).toString();
   }
   m_CurrFont = QFont( m_CurrFamily, m_CurrPointSize, m_CurrWeight, m_IsItalic );
   updateFont();
}

void
FontBar::onComboIndex_Size( int )
{
   int currentIndex = m_FontSize->currentIndex();
   if ( currentIndex < 0 )
   {
      m_CurrPointSize = 10;
   }
   else
   {
      m_CurrPointSize = m_FontSize->itemData( currentIndex ).toInt();
   }
   m_CurrFont = QFont( m_CurrFamily, m_CurrPointSize, m_CurrWeight, m_IsItalic );
   updateFont();
}

void
FontBar::onButtonClick_Bold( bool checked )
{
   qDebug() << __FUNCTION__;

   if ( checked )
   {
      m_CurrWeight = 75;
   }
   else
   {
      m_CurrWeight = 50;
   }

   m_IsBold = checked;
   m_CurrFont = QFont( m_CurrFamily, m_CurrPointSize, m_CurrWeight, m_IsItalic );
   updateFont();
}

void
FontBar::onButtonClick_Italic( bool checked )
{
   qDebug() << __FUNCTION__;

   m_IsItalic = checked;
   m_CurrFont = QFont( m_CurrFamily, m_CurrPointSize, m_CurrWeight, m_IsItalic );
   updateFont();
}

void
FontBar::onButtonClick_Underline( bool )
{
   qDebug() << __FUNCTION__;
}

void
FontBar::onButtonClick_StrikeThrough( bool )
{
   qDebug() << __FUNCTION__;
}
