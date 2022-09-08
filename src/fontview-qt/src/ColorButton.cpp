#include "ColorButton.hpp"
#include <QImage>
#include <QColor>
#include <QColorDialog>
#include <QDebug>
#include <sstream>

namespace /* anonymous */ {

inline QColor
getPixel( QImage const & img, int x, int y, QColor const & colorKey = QColor( 0,0,0,0 ) )
{
   if ( x < 0 || y < 0 || x >= img.width() || y >= img.height() ) return colorKey;
   return img.pixelColor( x, y );
}

inline void
setPixel( QImage & img, int x, int y, QColor const & color, bool blend = false, QColor const & colorKey = QColor( 0,0,0,0 ) )
{
   if ( x < 0 || y < 0 || x >= img.width() || y >= img.height() ) return;
   if ( !blend )
   {
      img.setPixelColor( x, y, color );
   }
   else
   {
      if ( color.alpha() >= 255 ) return;
      QColor oldColor = getPixel( img, x, y, colorKey );
      auto r1 = float( oldColor.redF() );
      auto g1 = float( oldColor.greenF() );
      auto b1 = float( oldColor.blueF() );
      //auto a1 = float( oldColor.alphaF() );
      auto dr = float( color.redF() ) - r1;
      auto dg = float( color.greenF() ) - g1;
      auto db = float( color.blueF() )- b1;
      //auto da = float( color.alphaF() ) - a1;

      auto a = float( color.alphaF() );
      float r = r1 + dr * a;
      float g = g1 + dg * a;
      float b = b1 + db * a;
      QColor finalColor = QColor::fromRgbF( r, g, b, 1.0 );
      setPixel( img, x, y, finalColor );
   }
}

inline void
drawHLine( QImage & img, int x1, int x2, int y, QColor lineColor, bool blend = false )
{
   if ( img.isNull() || img.width() < 1 || img.height() < 1 ) return;
   //if ( x1 > x2 ) { std::swap( x1, x2 ); }
   for ( int x = std::min( x1, x2 ); x <= std::max( x1, x2 ); ++x )
   {
      setPixel( img, x, y, lineColor, blend );
   }
}

inline void
drawVLine( QImage & img, int y1, int y2, int x, QColor lineColor, bool blend = false )
{
   if ( img.isNull() || img.width() < 1 || img.height() < 1 ) return;
   //if ( y1 > y2 ) { std::swap( y1, y2 ); }
   for ( int y = std::min( y1, y2 ); y <= std::max( y1, y2 ); ++y )
   {
      setPixel( img, x, y, lineColor, blend );
   }
}

inline void
drawLineRect( QImage & img,
              int x, int y, int w, int h,
              QColor lineColor, int lineWidth = 1, bool blend = false )
{
   if ( img.isNull() || img.width() < 1 || img.height() < 1 ) return;
   for ( int i = 0; i < lineWidth; ++i )
   {
      drawHLine( img, x+i, x+w-1-i, y+i, lineColor, blend );
      drawVLine( img, y+i, y+h-1-i, x+i, lineColor, blend );
      drawVLine( img, y+i, y+h-1-i, x+w-1-i, lineColor, blend );
      drawHLine( img, x+i, x+w-1-i, y+h-1-i, lineColor, blend );
   }
}

inline void
drawFillRect( QImage & img,
              int x, int y, int w, int h,
              QColor fillColor, bool blend = false )
{
   if ( img.isNull() || img.width() < 1 || img.height() < 1 ) return;
   for ( int j = 0; j < h; ++j )
   {
      for ( int i = 0; i < w; ++i )
      {
         setPixel( img, x + i, y + j, fillColor, blend );
      }
   }
}

inline void
drawRect( QImage & img,
               int x, int y, int w, int h,
               QColor fillColor,
               QColor lineColor, int lineWidth = 1, bool blend = false )
{
   if ( fillColor.alpha() > 0 )
   {
      drawFillRect( img, x, y, w, h, fillColor, blend );
   }
   if ( lineColor.alpha() > 0 )
   {
      drawLineRect( img, x, y, w, h, lineColor, lineWidth, blend );
   }
}

}// end namespace anonymous

ColorButton::ColorButton( QString name, uint32_t colorRGBA, QWidget * parent )
   : QPushButton( "", parent )
   , m_Color( 0,0,0,0 )
   , m_Index( 0 )
{
   setColorRGBA( colorRGBA );
}

//ColorButton::~ColorButton()
//{

//}


void
ColorButton::updateButtonImage()
{
   QImage img( 16, 16, QImage::Format_ARGB32 );
   if ( m_Color.alpha() < 255 )
   {
      img.fill( Qt::white );
      int dx = 8;
      int dy = 8;
      QColor evenLineColor = QColor( 150,150,150,255 );
      QColor evenFillColor = QColor( 150,150,150,255 );
      QColor oddLineColor = QColor( 100,100,100,255 );
      QColor oddFillColor = QColor( 100,100,100,255 );
      // drawCheckered( img, 0, 0, img.width() - 1, img.height() - 1, dx, dy );
      drawRect( img, 0, 0, 8, 8, evenFillColor, evenLineColor );
      drawRect( img, 8, 0, 8, 8, oddFillColor, oddLineColor );
      drawRect( img, 0, 8, 8, 8, oddFillColor, oddLineColor );
      drawRect( img, 8, 8, 8, 8, evenFillColor, evenLineColor );

      for ( int j = 0; j < img.height(); ++j )
      {
         for ( int i = 0; i < img.width(); ++i )
         {
            setPixel( img, i, j, m_Color, true );
         }
      }
   }
   else
   {
      img.fill( m_Color ); // Fill solid, no transparency with check pattern necessary.
   }

//   if ( m_IsSelected )
//   {
//      QColor borderColor = Qt::black;
//      if ( averageRGB( m_Color ) < int( 127 ) )
//      {
//         borderColor = Qt::white;
//      }

//      drawLineRect( img, 0, 0, img.width(), img.height(), borderColor, 2, false );
//   }

   QPixmap pix = QPixmap::fromImage( img );
   setIcon( QIcon( pix ) );
   update();
}

void
ColorButton::emit_fgcolor()
{
   emit fgcolorChanged( m_Color );
   qDebug() << __FUNCTION__ << "(" << m_Color.red() << "," << m_Color.green() << "," << m_Color.blue() << "," << m_Color.alpha() << ")";
}
void
ColorButton::emit_bgcolor()
{
   emit bgcolorChanged( m_Color );
   qDebug() << __FUNCTION__ << "(" << m_Color.red() << "," << m_Color.green() << "," << m_Color.blue() << "," << m_Color.alpha() << ")";
}

void
ColorButton::setColorRGBA( uint32_t colorRGBA )
{
   QColor color( colorRGBA & 0xFF,
              ( colorRGBA >> 8 ) & 0xFF,
              ( colorRGBA >> 16 ) & 0xFF,
              ( colorRGBA >> 24 ) & 0xFF );
   setColor( color );
}

void
ColorButton::setColor( QColor color )
{
   if ( m_Color == color ) return;
   m_Color = color;
   updateButtonImage();
   std::stringstream s;
   s << "rgba(" << m_Color.red() << "," << m_Color.green() << "," << m_Color.blue() << "," << m_Color.alpha() << ")";
   s << std::hex
     << "\n#" << uint8_t( m_Color.red() )
              << uint8_t( m_Color.green() )
              << uint8_t( m_Color.blue() )
              << uint8_t( m_Color.alpha() );
   setToolTip( QString::fromStdString( s.str() ) );
}

void
ColorButton::mousePressEvent( QMouseEvent * me )
{
   if ( me->buttons() & Qt::LeftButton )
   {
      emit_fgcolor();
   }
   else if ( me->buttons() & Qt::RightButton )
   {
      emit_bgcolor();
   }
   QPushButton::mousePressEvent( me );
}

void
ColorButton::mouseReleaseEvent( QMouseEvent * me )
{
   QPushButton::mouseReleaseEvent( me );
}

void
ColorButton::mouseDoubleClickEvent( QMouseEvent * me )
{
   if ( me->buttons() & Qt::LeftButton )
   {
      QColorDialog dlg( m_Color, this );
      dlg.exec();
      setColor( dlg.currentColor() );
      emit_fgcolor();
   }
   else if ( me->buttons() & Qt::RightButton )
   {
      QColorDialog dlg( m_Color, this );
      dlg.exec();
      setColor( dlg.currentColor() );
      emit_bgcolor();
   }
}
