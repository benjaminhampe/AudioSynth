#include "GDeck.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace {

QColor toQColor( uint32_t color )
{
   int32_t r = de::RGBA_R(color);
   int32_t g = de::RGBA_G(color);
   int32_t b = de::RGBA_B(color);
   int32_t a = de::RGBA_A(color);
   return QColor( b,g,r,a );
}

}

GDeck::GDeck( QString title, QWidget* parent )
   : QWidget(parent)
   , m_deckColor( 1,1,1,128 )
   , m_inputSignal( nullptr )
   , m_updateTimerId( 0 )
{
   setObjectName( "GDeck" );
   setContentsMargins( 1,1,1,1 );
   setMinimumHeight(24);
   setMaximumHeight(24);

   m_title = new QImageWidget( this );
   m_title->setImageScaling( false );
   setTitleImageText( title );

   m_btnEnabled = new QPushButton( "E", this );
   m_btnEnabled->setToolTip("This DSP element is now (e)nabled = not bypassed");
   m_btnEnabled->setCheckable( true );
   m_btnEnabled->setChecked( true );
   m_btnEnabled->setMinimumSize( 20,20 );
   m_btnEnabled->setMaximumSize( 20,20 );

   m_more = new QPushButton( "-", this );
   m_more->setToolTip("All DSP options are visible now");
   m_more->setCheckable( true );
   m_more->setChecked( true );
   m_more->setMinimumSize( 20,20 );
   m_more->setMaximumSize( 20,20 );

   m_btnVisible = new QPushButton( "S", this );
   m_btnVisible->setToolTip( "This DSP element is fully visible now and (s)hown." );
   m_btnVisible->setCheckable( true );
   m_btnVisible->setChecked( true );
   m_btnVisible->setMinimumSize( 20,20 );
   m_btnVisible->setMaximumSize( 20,20 );

   m_levelL = new GLevelMeterH( this );
   m_levelR = new GLevelMeterH( this );

   auto v = new QVBoxLayout();
   v->setContentsMargins( 0,0,0,0 );
   v->setSpacing( 1 );
   v->addWidget( m_levelL );
   v->addWidget( m_levelR );
   setLayout( v );

   auto w = new QWidget( this );
   w->setContentsMargins( 0,0,0,0 );
   w->setLayout( v );

   auto h = new QHBoxLayout();
   h->setContentsMargins( 1,1,1,1 );
   h->setSpacing( 1 );
   h->addWidget( m_btnVisible );
   h->addWidget( m_btnEnabled );
   h->addWidget( m_more );
   h->addSpacing( 20 );
   h->addWidget( w );
   h->addSpacing( 20 );
   h->addWidget( m_title );
   h->addStretch( 1 );
   //h->addStretch( 1 );
   h->setAlignment( w, Qt::AlignVCenter );
   h->setAlignment( m_title, Qt::AlignVCenter );
   setLayout( h );

   m_ColorGradient.addStop( 0.0f, 0xFF000000 );
   m_ColorGradient.addStop( 0.1f, 0xFFFFFFFF );
   m_ColorGradient.addStop( 0.4f, 0xFF00FF00 );
   m_ColorGradient.addStop( 0.6f, 0xFF00FFFF );
   m_ColorGradient.addStop( 1.0f, 0xFF0000FF );
   m_ColorGradient.addStop( 1.1f, 0xFFFF00FF );

   // Feed LevelMeter
   m_Lmin = m_Lmax = m_Rmin = m_Rmax = 0.0f;

   // horizontal gradient filling
   QImage img( 128,5, QImage::Format_ARGB32 );
   float const fx = 1.2f / float( img.width() );
   for ( int32_t x = 0; x < img.width(); ++x )
   {
      uint32_t c = m_ColorGradient.getColor32( fx*x );
      int32_t r = de::RGBA_R( c );
      int32_t g = de::RGBA_G( c );
      int32_t b = de::RGBA_B( c );
      int32_t a = de::RGBA_A( c );
      QColor color( r,g,b,a );
      for ( int32_t y = 0; y < img.height(); ++y )
      {
         img.setPixelColor( x, y, color );
      }
   }

   m_levelL->setImage( img );
   m_levelR->setImage( img );

   connect( m_btnEnabled, SIGNAL(toggled(bool)), this, SLOT(on_buttonToggled_enabled(bool)) );
   connect( m_more, SIGNAL(toggled(bool)), this, SLOT(on_buttonToggled_more(bool)) );
   connect( m_btnVisible, SIGNAL(toggled(bool)), this, SLOT(on_buttonToggled_visible(bool)) );

   startUpdateTimer();
}

GDeck::~GDeck()
{
   stopUpdateTimer();
}

void
GDeck::stopUpdateTimer()
{
   if ( m_updateTimerId )
   {
      killTimer( m_updateTimerId );
      m_updateTimerId = 0;
   }
}

void
GDeck::startUpdateTimer()
{
   stopUpdateTimer();
   m_updateTimerId = startTimer( 67, Qt::CoarseTimer );
}

void
GDeck::clearInputSignals()
{
   m_inputSignal = nullptr;
}

void
GDeck::setInputSignal( int i, de::audio::IDspChainElement* input )
{
   m_inputSignal = input;
}

uint64_t
GDeck::readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
{
   uint64_t dstSamples = dstFrames * dstChannels;

   m_Lmin = m_Lmax = m_Rmin = m_Rmax = 0.0f;

   if ( m_inputSignal )
   {
      auto ret = m_inputSignal->readSamples( pts, dst, dstFrames, dstChannels, dstRate );
      if ( ret != dstSamples )
      {
         DE_ERROR("retSamples != dstSamples, need zero filling")
      }

      // Feed LevelMeter
      m_Lmin = std::numeric_limits< float >::max();
      m_Lmax = std::numeric_limits< float >::lowest();
      m_Rmin = std::numeric_limits< float >::max();
      m_Rmax = std::numeric_limits< float >::lowest();

      // Fast interleaved stereo path O(1) = one loop over samples
      if ( dstChannels == 2 )
      {
         float const* pSrc = dst;
         for ( uint64_t i = 0; i < dstFrames; ++i )
         {
            float L = *pSrc++;
            float R = *pSrc++;
            m_Lmin = std::min( m_Lmin, L );
            m_Lmax = std::max( m_Lmax, R );
            m_Rmin = std::min( m_Rmin, L );
            m_Rmax = std::max( m_Rmax, R );
         }

         m_levelL->setValue( m_Lmax );
         m_levelR->setValue( m_Rmax );

      }
      // Slower path O(N) = one loop for each channel of N channels.
      else
      {
//         float const* pSrc = dst;
//         for ( uint64_t i = 0; i < dstFrames; ++i )
//         {
//            float sample = *pSrc;
//            m_Lmin = std::min( m_Lmin, sample );
//            m_Lmax = std::max( m_Lmax, sample );
//            pSrc += dstChannels;
//         }

//         if ( dstChannels > 1 )
//         {
//            pSrc = dst + 1;
//            for ( uint64_t i = 0; i < dstFrames; ++i )
//            {
//               float sample = *pSrc;
//               m_Rmin = std::min( m_Rmin, sample );
//               m_Rmax = std::max( m_Rmax, sample );
//               pSrc += dstChannels;
//            }
//         }
      }
   }

   // Main audio bypass ( oszilloskop shall not change data )
   //de::audio::DSP_COPY( m_inputBuffer.data(), dst, dstSamples );
   return dstSamples;
}

void
GDeck::setTitleImageText( QString title )
{
   auto ts = m_font5x8.getTextSize( title );
   QImage img( ts.width()+8, ts.height()+4, QImage::Format_ARGB32 );
   img.fill( QColor(255,255,255,255) );
   m_font5x8.drawText( img, 4,2, title, QColor(0,0,0) );
   m_title->setImage( img, true );
}


void
GDeck::timerEvent( QTimerEvent* event )
{
   if ( event->timerId() == m_updateTimerId )
   {
      update();
   }
}


void
GDeck::paintEvent( QPaintEvent* event )
{
   int w = width();
   int h = height();

   QPainter dc(this);
   dc.setRenderHint( QPainter::NonCosmeticDefaultPen );

   // Draw deck background rectangle
   dc.setPen( QPen( QColor( 255,255,255,255 ) ) );
   dc.setBrush( QBrush( m_deckColor ) );
   dc.drawRect( QRect(0,0,w-1,h-1) );

//   // Draw Color Gradient
//   int lvx = w/2;
//   int lvx = w/2;

//   dc.drawImage( w/,0, m_imgColorGradient );

//   dc.drawImage( 0,0, m_imgColorGradient );

//   //
//   int xL = int( m_Lmax * w );
//   int xR = int( m_Rmax * h );
//   int dL = w - 1 - xL;
//   int dR = w - 1 - xR;

//   QColor colorL = toQColor( m_ColorGradient.getColor32( m_Lmax ) );
//   QColor colorR = toQColor( m_ColorGradient.getColor32( m_Rmax ) );
//   QColor colorBg( 25,25,25 );
//   //QColor color = toQColor( 0xFF10BF20 );

//   dc.setPen( Qt::NoPen );
//   dc.setBrush( QBrush( colorBg ) );
//   dc.drawRect( QRect(0,0,w/2-1,yL) );

//   dc.setBrush( QBrush( colorL ) );
//   dc.drawRect( QRect(0,yL,w/2-1,h-1-yL) );


//   dc.setPen( Qt::NoPen );
//   dc.setBrush( QBrush( colorBg ) );
//   dc.drawRect( QRect(w/2,0,w/2-1,yR) );

//   dc.setBrush( QBrush( colorR ) );
//   dc.drawRect( QRect(w/2,yR,w/2-1,h-1-yR) );


   QWidget::paintEvent( event );
}

void
GDeck::on_buttonToggled_enabled(bool checked)
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

void
GDeck::on_buttonToggled_more(bool checked)
{
   if ( checked )
   {
      m_more->setText("-");
      m_more->setToolTip("Press to show less");
   }
   else
   {
      m_more->setText("+");
      m_more->setToolTip("Press to show more");
   }
   emit toggledMore( checked );
}

void
GDeck::on_buttonToggled_visible(bool checked)
{
   if ( m_btnVisible->isChecked() )
   {
      m_btnVisible->setText("S");
      m_btnVisible->setToolTip("This DSP element is fully visible now");
      m_more->setVisible( true );
   }
   else
   {
      m_btnVisible->setText("H");
      m_btnVisible->setToolTip("This DSP element is fully hidden now");
      m_more->setVisible( false );
   }
   emit toggledHideAll( !checked );
}
