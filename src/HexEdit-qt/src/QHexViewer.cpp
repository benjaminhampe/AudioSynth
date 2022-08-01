#include "QHexViewer.hpp"


QHexViewer::QHexViewer( QWidget* parent )
   : QWidget( parent )
   , m_lineStart( 0 )
   , m_lineCount( 0 )
{
   setContentsMargins( 0,0,0,0 );
   setMinimumSize( 2*128, 2*128 );
}

QHexViewer::~QHexViewer()
{}

bool
QHexViewer::openFile()
{
   // QApplication::applicationDirPath()
   QStringList files = QFileDialog::getOpenFileNames( this, "Open binary file", "../.." );
   if ( files.count() < 1 )
   {
      qDebug() << "No files selected";
      return false;
   }

   if ( files.count() < 1 )
   {
      qDebug() << "No File selected";
      return false;
   }

   return openFile( files.at( 0 ) );
}

bool
QHexViewer::openFile( QString uri )
{
   m_uri = uri;

   if ( !QFileInfo( m_uri ).exists() )
   {
      qDebug() << "Uri does not exist " << m_uri;

      return false;
   }
   if ( !QFileInfo( m_uri ).isFile() )
   {
      qDebug() << "Uri is not a file " << m_uri;
      return false;
   }

   qDebug() << "Open uri " << m_uri;

   m_bytes.clear();
   if ( !loadBinary( m_uri.toStdString(), m_bytes ) )
   {
      qDebug() << "Cant load binary file " << m_uri;
      return false;
   }

   m_lineStart = 0;
   m_lineCount = m_bytes.size() / 16; // 16 bytes per line

   qDebug() << "Loaded binary file" << m_uri;
   qDebug() << "Binary file has" << m_bytes.size() << "bytes";
   qDebug() << "Binary file has" << m_lineCount << "lines";

   update();
   return true;
}

void QHexViewer::closeFile()
{
   m_lineStart = 0;
   m_lineCount = 0;
}

void QHexViewer::resizeEvent( QResizeEvent* event )
{
   update();
}

void QHexViewer::paintEvent( QPaintEvent* event )
{
   int w = width();
   int h = height();

   if ( m_lineCount < 1 )
   {
      QPainter dc( this );
      dc.setRenderHint( QPainter::NonCosmeticDefaultPen );
      dc.fillRect( rect(), Qt::white );
      return;
   }

   if ( w != m_img.width() || h != m_img.height() )
   {
      m_img = QImage( w,h, QImage::Format_ARGB32 );
   }

   m_img.fill(  Qt::white );

   auto ts = m_font.getTextSize("W_");
   int ln = ts.height();
   int lineCount = h / ln;

//      if ( m_lineStart + lineCount > m_lineCount )
//      {
//         m_lineStart = std::max( m_lineCount - lineCount, 0 );
//         lineCount = m_lineCount - m_lineStart;
//      }

   qDebug() << "Draw " << lineCount << "lines";


   for ( int y = 0; y < lineCount; ++y )
   {
      size_t i = size_t( y + m_lineStart ) * size_t( 16 );

      if ( i + 16 < m_lineCount )
      {
         std::stringstream s;
         s << dbStrHex( uint32_t(i) ) << "|";
         s << dbStrByte( m_bytes[i] )   << " ";
         s << dbStrByte( m_bytes[i+1] ) << " ";
         s << dbStrByte( m_bytes[i+2] ) << " ";
         s << dbStrByte( m_bytes[i+3] ) << "|";
         s << dbStrByte( m_bytes[i+4] ) << " ";
         s << dbStrByte( m_bytes[i+5] ) << " ";
         s << dbStrByte( m_bytes[i+6] ) << " ";
         s << dbStrByte( m_bytes[i+7] ) << "|";
         s << dbStrByte( m_bytes[i+8] ) << " ";
         s << dbStrByte( m_bytes[i+9] ) << " ";
         s << dbStrByte( m_bytes[i+10] ) << " ";
         s << dbStrByte( m_bytes[i+11] ) << "|";
         s << dbStrByte( m_bytes[i+12] ) << " ";
         s << dbStrByte( m_bytes[i+13] ) << " ";
         s << dbStrByte( m_bytes[i+14] ) << " ";
         s << dbStrByte( m_bytes[i+15] ) << "| ";

         s << char( m_bytes[i] ) << char( m_bytes[i+1] );
         s << char( m_bytes[i+2] ) << char( m_bytes[i+3] );
         s << char( m_bytes[i+4] ) << char( m_bytes[i+5] );
         s << char( m_bytes[i+6] ) << char( m_bytes[i+7] );
         s << char( m_bytes[i+8] ) << char( m_bytes[i+9] );
         s << char( m_bytes[i+10] ) << char( m_bytes[i+11] );
         s << char( m_bytes[i+12] ) << char( m_bytes[i+13] );
         s << char( m_bytes[i+14] ) << char( m_bytes[i+15] );

         auto msg = QString::fromStdString( s.str() );
         m_font.drawText( m_img, 0,y * ln, msg, 0xFF000000 );
      }
   }

   QPainter dc( this );
   dc.setRenderHint( QPainter::NonCosmeticDefaultPen );
   dc.drawImage( 0,0,m_img );

}

