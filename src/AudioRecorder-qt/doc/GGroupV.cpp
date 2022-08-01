#include "GGroupV.hpp"
#include <QVBoxLayout>

namespace {
QImage
rotateLeft( QImage const & img )
{
   int w = img.width();
   int h = img.height();
   QImage dst( h, w, img.format() );

   for ( int32_t y = 0; y < h; ++y )
   {
      for ( int32_t x = 0; x < w; ++x )
      {
         uint32_t color = img.pixel( x,y );
         dst.setPixel( y, w - 1 - x, color);
      }
   }

   return dst;
}
}

GGroupV::GGroupV( QString name, QWidget* parent )
   : QWidget(parent)
{
   setObjectName( "GGroupV" );
   setContentsMargins( 0,0,0,0 );

   setMinimumWidth(24);
   setMaximumWidth(24);
   //setMinimumHeight(64);

   // Create Image
   auto ts = m_font.getTextSize( name );
   int w = ts.width()+2;
   int h = ts.height()+2;
   QImage img( w, h, QImage::Format_ARGB32 );
   img.fill( QColor(255,255,255,255) );

//   int tw = ts.width();
//   int th = ts.height();
//   int x = 1;
//   int y = h / 2;
   m_font.drawText( img, 1,1, name, QColor(0,0,0) );

   m_title = new QImageWidget( this );
   m_title->setImage( rotateLeft( img ), true );
   m_title->setImageScaling( false );

   m_btnEnabled = new QPushButton( "E", this );
   m_btnEnabled->setToolTip("This DSP element is now (e)nabled = not bypassed");
   m_btnEnabled->setCheckable( true );
   m_btnEnabled->setChecked( true );
   m_btnEnabled->setMinimumSize( 20,20 );
   m_btnEnabled->setMaximumSize( 20,20 );

   m_btnMore = new QPushButton( "-", this );
   m_btnMore->setToolTip("All DSP options are visible now");
   m_btnMore->setCheckable( true );
   m_btnMore->setChecked( true );
   m_btnMore->setMinimumSize( 20,20 );
   m_btnMore->setMaximumSize( 20,20 );

   m_btnVisible = new QPushButton( "S", this );
   m_btnVisible->setToolTip( "This DSP element is fully visible now and (s)hown." );
   m_btnVisible->setCheckable( true );
   m_btnVisible->setChecked( true );
   m_btnVisible->setMinimumSize( 20,20 );
   m_btnVisible->setMaximumSize( 20,20 );


   QVBoxLayout* v = new QVBoxLayout();
   v->setContentsMargins( 1,1,1,1 );
   v->setSpacing( 1 );
   v->addWidget( m_btnEnabled );
   v->addStretch( 1 );
   v->addWidget( m_title );
   v->addStretch( 1 );
   v->addWidget( m_btnMore );
   v->addWidget( m_btnVisible );

   v->setAlignment( m_btnEnabled, Qt::AlignHCenter );
   v->setAlignment( m_title, Qt::AlignHCenter );
   v->setAlignment( m_btnMore, Qt::AlignHCenter );
   v->setAlignment( m_btnVisible, Qt::AlignHCenter );
   setLayout( v );

   connect( m_btnEnabled, SIGNAL(toggled(bool)), this, SLOT(on_buttonToggled_enabled(bool)) );
   connect( m_btnMore, SIGNAL(toggled(bool)), this, SLOT(on_buttonToggled_more(bool)) );
   connect( m_btnVisible, SIGNAL(toggled(bool)), this, SLOT(on_buttonToggled_visible(bool)) );

}

GGroupV::~GGroupV()
{
}

void GGroupV::on_buttonToggled_enabled(bool checked)
{
   if ( checked )
   {
      m_btnEnabled->setText("E");
      m_btnEnabled->setToolTip("This DSP element is now enabled = not bypassed");
   }
   else
   {
      m_btnEnabled->setText("B");
      m_btnEnabled->setToolTip("This DSP element is now disabled = bypassed");
   }
   emit toggledBypass( !checked );
}

void GGroupV::on_buttonToggled_more(bool checked)
{
   if ( checked )
   {
      m_btnMore->setText("-");
      m_btnMore->setToolTip("Press to show less");
   }
   else
   {
      m_btnMore->setText("+");
      m_btnMore->setToolTip("Press to show more");
   }
   emit toggledMore( checked );
}

void GGroupV::on_buttonToggled_visible(bool checked)
{
   if ( m_btnVisible->isChecked() )
   {
      m_btnVisible->setText("S");
      m_btnVisible->setToolTip("This DSP element is fully visible now");
      m_btnMore->setVisible( true );
   }
   else
   {
      m_btnVisible->setText("H");
      m_btnVisible->setToolTip("This DSP element is fully hidden now");
      m_btnMore->setVisible( false );
   }
   emit toggledHideAll( !checked );
}
