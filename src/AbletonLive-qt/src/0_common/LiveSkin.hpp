/// (c) 2017 - 20180 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef DE_LIVE_SKIN_HPP
#define DE_LIVE_SKIN_HPP

#include <sstream>
#include <chrono>
#include <cmath>

#include <QMainWindow>
#include <QScreen>
#include <QWidget>
#include <QDesktopWidget>

#include <QWidget>
#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QDial>
#include <QDebug>
#include <QThread>

#include <QWidget>
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QFileDialog>

#include <QScrollArea>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QPushButton>

#include <QFontDatabase>
#include <QColor>


#include <DarkImage.hpp>      // DarkImage

#include <de_fontawesome.hpp> // DarkImage

#include <QFont5x8.hpp>       // Benni extra
#include <QImageWidget.hpp>   // Benni extra
#include <de/Color.hpp>       // DarkImage

inline QWidget*
createWidget( QWidget* parent, int cm1 = 0, int cm2 = 0, int cm3 = 0, int cm4 = 0 )
{
   auto w = new QWidget( parent );
   w->setContentsMargins( cm1,cm2,cm3,cm4 );
   return w;
}

inline QHBoxLayout*
createHBox( int spacing = 0, int cm1 = 0, int cm2 = 0, int cm3 = 0, int cm4 = 0 )
{
   auto h = new QHBoxLayout();
   h->setContentsMargins( cm1,cm2,cm3,cm4 );
   h->setSpacing( spacing );
   return h;
}

inline QVBoxLayout*
createVBox( int spacing = 0, int cm1 = 0, int cm2 = 0, int cm3 = 0, int cm4 = 0 )
{
   auto v = new QVBoxLayout();
   v->setContentsMargins( cm1,cm2,cm3,cm4 );
   v->setSpacing( spacing );
   return v;
}

inline QFont getFontAwesome( int size = 10, bool bold = false )
{
   QFont font("FontAwesome", size, bold ? QFont::Bold : QFont::Normal, false );
   font.setHintingPreference( QFont::PreferFullHinting );
   font.setKerning( true );
   font.setStyleStrategy( QFont::PreferAntialias );
   return font;
}

inline void addFontAwesome463()
{
   int fontAwesome = QFontDatabase::addApplicationFont( "fontawesome463.ttf" );
   //int fontAwesome = QFontDatabase::addApplicationFont( "la-regular-400.ttf" );
   auto names = QFontDatabase::applicationFontFamilies( fontAwesome );
   for ( int i = 0; i < names.size(); ++i )
   {
      std::wcout << "Font[" << i << "] = " << names.at( i ).toStdWString() << std::endl;
   }
}

/*
int
dbAddFontFamily( QString const & fontFamily )
{
   QFileInfo fileInfo( fontFamily );
   if ( !fileInfo.exists() )
   {
      qDebug() << "[Error] " << __FUNCTION__ << "(" << fontFamily << ") File does not exist";
      return -1;
   }

   if ( !fileInfo.isFile() )
   {
      qDebug() << "[Error] " << __FUNCTION__ << "(" << fontFamily << ") Is not a file";
      return -1;
   }

   int fontId = QFontDatabase::addApplicationFont( fontFamily );
   if ( fontId >= 0 )
   {
      QStringList fonts = QFontDatabase::applicationFontFamilies( fontId );

      qDebug() << __FUNCTION__ << "(" << fontFamily << ") -> fontId(" << fontId << "), familyCount(" << fonts.size() << ")";
      for ( int i = 0; i < fonts.size(); ++i )
      {
         qDebug() << "Font[" << i << "] " << fonts[ i ];
      }
   }
   else {
      qDebug() << __FUNCTION__ << "(" << fontFamily << ") -> Cannot load.";
   }
   return fontId;
}
*/

inline void setFontAwesome( QWidget* widget, int size = 10 )
{
   if ( !widget ) return;
   widget->setFont( getFontAwesome( size ) );
}

inline void setFontAwesome( QPainter & dc, int size = 10 )
{
   dc.setFont( getFontAwesome( size ) );
}

inline void
drawLine( QPainter & dc, int x1, int y1, int x2, int y2, QColor color )
{
   //dc.setBrush( Qt::NoBrush );
   dc.setPen( QPen( color ) );
   dc.drawLine( x1, y1, x2, y2 );
}

inline void
drawRectBorder( QPainter & dc, int x, int y, int w, int h, QColor color )
{
   dc.setPen( QPen( color ) );
   dc.setBrush( Qt::NoBrush );
   dc.drawRect( QRect(x,y,w-1,h-1) );
}

inline void
drawRectBorder( QPainter & dc, QRect pos, QColor color )
{
   int x = pos.x();
   int y = pos.y();
   int w = pos.width();
   int h = pos.height();
   drawRectBorder( dc, x,y,w,h, color );
}


inline void
fillRect( QPainter & dc, int x, int y, int w, int h, QColor color )
{
   dc.setPen( Qt::NoPen );
   dc.setBrush( QBrush( color ) );
   dc.drawRect( QRect(x,y,w-1,h-1) );
}

//inline void
//fillRect( QPainter & dc, QRect pos, QColor color )
//{
//   fillRect( dc, pos.x(), pos.y(), pos.width(), pos.height(), color );
//}

inline void
drawRect( QPainter & dc, int x, int y, int w, int h, QColor fillColor, QColor penColor )
{
   dc.setPen( QPen( penColor ) );
   dc.setBrush( QBrush( fillColor ) );
   dc.drawRect( QRect(x,y,w-1,h-1) );
}

inline void
drawText( QPainter & dc, int x, int y, QString msg,
          QColor textColor = QColor(255,255,255) )
{
   dc.setPen( QPen( textColor ) );
   dc.setBrush( QBrush( textColor ) );
   dc.drawText( x, y, msg );
}

inline void
drawText5x8( QPainter & dc, int x, int y, QString msg,
             QColor textColor = QColor(255,255,255),
             de::Align::EAlign textAlign = de::Align::Default )
{
   QFont5x8().drawText( dc, x, y, msg, textColor, textAlign );
}

inline QImage
createImageFromText( int px, int py, QString msg, QFont const & font, QColor fg = QColor(255,255,255), QColor bg = QColor(0,0,0,0) )
{
   QPainter dc;
   dc.setFont( font );
   auto ts = dc.fontMetrics().boundingRect( msg ).size();

   int w = ts.width() + 2*px;
   int h = ts.height() + 2*py;
   QImage img( w, h, QImage::Format_ARGB32 );
   img.fill( bg );
   if ( dc.begin( &img ) )
   {
      dc.setPen( QPen( fg ) );
      dc.setBrush( QBrush( fg ) );
      dc.drawText( px, h-3 - py, msg );
      dc.end();
   }

   return img;
}


inline QImage
loadImage( QString uri )
{
   QImage img;
   img.load( uri );
   return img;
}

/*
inline QImage
createTransparencyArt(
      QColor fg = QColor(255,255,255),
      QColor bg = QColor(0,0,0,0),
      std::vector< std::vector< uint8_t > > const & pixmap )
{
   int w = 0;
   int h = 0;
   int m = pixmap.size();
   for ( int j = 0; j < m; ++j )
   {
      auto const & line = pixmap[ j ];
      int n = line.size();
      w = std::max( w, n );
      if ( n > 0 )
      {
         h = j + 1; // this line is new end of image.
      }
   }

   std::cout << "Create ascii image(" << w << "," << h << ") from " << m << " lines." << std::endl;

   QImage img( w, h, QImage::Format_ARGB32 );
   img.fill( bg );

   for ( int j = 0; j < m; ++j )
   {
      auto const & line = lines[ j ];
      int n = line.size();
      for ( int i = 0; i < n; ++i )
      {
         if ( line[ i ] != ' ' )
         {
            img.setPixelColor( i,j, fg );
         }
      }
   }

   return img;
}
*/

inline QImage
createAsciiArt(
      std::vector< std::string > const & lines,
      QColor fg = QColor(255,255,255),
      QColor bg = QColor(0,0,0,0) )
{
   int w = 0;
   int h = 0;
   int m = lines.size();
   for ( int j = 0; j < m; ++j )
   {
      auto const & line = lines[ j ];
      int n = line.size();
      w = std::max( w, n );
      if ( n > 0 )
      {
         h = j + 1; // this line is new end of image.
      }
   }

   std::cout << "Create ascii image(" << w << "," << h << ") from " << m << " lines." << std::endl;

   QImage img( w, h, QImage::Format_ARGB32 );
   img.fill( bg );

   for ( int j = 0; j < m; ++j )
   {
      auto const & line = lines[ j ];
      int n = line.size();
      for ( int i = 0; i < n; ++i )
      {
         if ( line[ i ] != ' ' )
         {
            img.setPixelColor( i,j, fg );
         }
      }
   }

   return img;
}

inline QImage
createAsciiArt15(
      QColor fg = QColor(255,255,255),
      QColor bg = QColor(0,0,0,0),
      std::string const & s0 = "",
      std::string const & s1 = "",
      std::string const & s2 = "",
      std::string const & s3 = "",
      std::string const & s4 = "",
      std::string const & s5 = "",
      std::string const & s6 = "",
      std::string const & s7 = "",
      std::string const & s8 = "",
      std::string const & s9 = "",
      std::string const & s10 = "",
      std::string const & s11 = "",
      std::string const & s12 = "",
      std::string const & s13 = "",
      std::string const & s14 = "" )
{
   return createAsciiArt( std::vector< std::string >{ s0, s1, s2, s3, s4,
                                                      s5, s6, s7, s8, s9,
                                                      s10, s11, s12, s13, s14 }, fg, bg );
}

inline QImage
createAsciiArt( QColor fg, QColor bg, std::string const & multiLineText )
{
   return createAsciiArt( dbStrSplit( multiLineText, '\n' ), fg, bg );
}


// namespace de {

inline QColor
toQColor( uint32_t color )
{
   int32_t r = de::RGBA_R(color);
   int32_t g = de::RGBA_G(color);
   int32_t b = de::RGBA_B(color);
   int32_t a = de::RGBA_A(color);
   return QColor( r,g,b,a );
}

inline void
drawKey( QPainter & dc, QRect pos, QColor brushColor, QColor penColor )
{
   int x = pos.x();
   int y = pos.y();
   int w = pos.width()-1;
   int h = pos.height()-1;

   if ( h < 6 )
   {
      dc.setPen( Qt::NoPen );
      dc.setBrush( QBrush( brushColor ) );
      dc.drawRect( pos );
   }
   else
   {
      dc.setPen( Qt::NoPen );
      dc.setBrush( QBrush( brushColor ) );
      dc.drawRect( QRect(x+1,y+1,w-2,h-2) );

      dc.setPen( QPen( penColor ) );
      dc.drawLine( x+1,y, x+w-2, y );
      dc.drawLine( x+1,y+h-1, x+w-2, y+h-1 );
      dc.drawLine( x,y+1, x, y+h-2 );
      dc.drawLine( x+w-1,y+1, x+w-1, y+h-2 );
   }
}

struct LiveSkin
{
   int margin = 4;
   int padd = 4;
   int radius = 8;
   int titleH = 12;
   int searchBarH = 20;
   int tdH = 12;

   int circleButtonSize = 23; //19;

   QColor windowColor = QColor(24,30,35);  // dark blue
   QColor panelColor = QColor(103,116,140);  // semi dark blue
   QColor focusColor = QColor(127,137,147); // darker gray
   QColor activeColor = QColor( 238, 246, 13 ); // yellow highlight

   QColor titleColor = QColor(217,221,226);   // light white blue
   QColor titleTextColor = windowColor;  // same as Window
   QColor contentColor = QColor(170,178,183); // greyish
   QColor editBoxColor = QColor(232,232,232);  // almost white
   QColor tdColor = QColor(140,153,175);  // table header

   QColor blackEditColor = QColor(217,221,226);
   QColor whiteEditColor = QColor(196,200,204);

   QColor ePenColor = QColor( 166, 181, 186 ); // Engine Button
   QColor eFillColor = QColor( 146, 157, 168 );
   QColor semiBeatColor = QColor(146,156,167);  // light grey between main beat grid
   QColor eTextColor = QColor( 29, 31, 33 );

   QColor playColor = QColor(79,254,29);
   QColor recColor = QColor(254,49,34);

//   enum EColor
//   {
//      Window = 0,
//      BluePanel,
//      GrayPanel,
//      Title,
//      TitleText,
//      Content,
//      EditBox,
//      EColorCount
//   };

//   static QColor
//   getQColor( EColor colorId )
//   {
//      //return toQColor( LiveSkin::getColor( colorId ) );
//      switch ( colorId )
//      {
//         case Window: return
//         case BluePanel: return
//         case GrayPanel: return
//         case Title: return
//         case TitleText: return QColor(24,30,35); // same as Window
//         case Content: return
//         case EditBox: return ;
//         default: return QColor(24,30,35); // debug violett
//      }
//   }
};

// } end namespace de

#endif
