#include "GLevelMeter.hpp"

using namespace de::audio;

GLevelMeter::GLevelMeter( QWidget* parent )
    : QWidget(parent)
    , m_updateTimerId( 0 )
    , m_inputSignal( nullptr )
{
   setObjectName( "GLevelMeter" );
   setMinimumSize( 8, 16 );
   setMaximumWidth( 16 );
   //qDebug() << "DSP thread id = " << QThread::currentThreadId();

   //m_inputBuffer.resize( 1024, 0.0f );

   m_ColorGradient.addStop( 0.0f, 0xFFFFFFFF );
   m_ColorGradient.addStop( 0.1f, 0xFF000000 );
   m_ColorGradient.addStop( 0.5f, 0xFF00FF00 );
   m_ColorGradient.addStop( 0.6f, 0xFF002000 );
   m_ColorGradient.addStop( 0.8f, 0xFF00FFFF );
   m_ColorGradient.addStop( 1.0f, 0xFF0000FF );
   m_ColorGradient.addStop( 1.1f, 0xFFFF00FF );

   // Feed LevelMeter
   m_Lmin = m_Lmax = m_Rmin = m_Rmax = 0.0f;

//       connect( this, SIGNAL(newSamples(float*,uint32_t,uint32_t)),
//                this, SLOT(pushSamples(float*,uint32_t,uint32_t)), Qt::QueuedConnection );
   startUpdateTimer();
}

GLevelMeter::~GLevelMeter()
{
   stopUpdateTimer();
}

void
GLevelMeter::stopUpdateTimer()
{
   if ( m_updateTimerId )
   {
      killTimer( m_updateTimerId );
      m_updateTimerId = 0;
   }
}

void
GLevelMeter::startUpdateTimer( int ms )
{
   stopUpdateTimer();
   if ( ms < 1 ) { ms = 1; }
   m_updateTimerId = startTimer( ms, Qt::CoarseTimer );
}

void
GLevelMeter::clearInputSignals()
{
   m_inputSignal = nullptr;
}

void
GLevelMeter::setInputSignal( de::audio::IDspChainElement* input )
{
   m_inputSignal = input;
}

uint64_t
GLevelMeter::readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
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
      }
      // Slower path O(N) = one loop for each channel of N channels.
      else
      {
         float const* pSrc = dst;
         for ( uint64_t i = 0; i < dstFrames; ++i )
         {
            float sample = *pSrc;
            m_Lmin = std::min( m_Lmin, sample );
            m_Lmax = std::max( m_Lmax, sample );
            pSrc += dstChannels;
         }

         if ( dstChannels > 1 )
         {
            pSrc = dst + 1;
            for ( uint64_t i = 0; i < dstFrames; ++i )
            {
               float sample = *pSrc;
               m_Rmin = std::min( m_Rmin, sample );
               m_Rmax = std::max( m_Rmax, sample );
               pSrc += dstChannels;
            }
         }
      }
   }

   // Main audio bypass ( oszilloskop shall not change data )
   //de::audio::DSP_COPY( m_inputBuffer.data(), dst, dstSamples );
   return dstSamples;
}

void
GLevelMeter::timerEvent( QTimerEvent* event )
{
   if ( event->timerId() == m_updateTimerId )
   {
      update();
   }
}

namespace {

QColor toQColor( uint32_t color )
{
   int r = de::RGBA_R( color );
   int g = de::RGBA_G( color );
   int b = de::RGBA_B( color );
   int a = de::RGBA_A( color );
   return QColor( r,g,b,a );
}

} // end namespace

// ============================================================================
void GLevelMeter::paintEvent( QPaintEvent* event )
// ============================================================================
{
   QPainter dc(this);
   dc.setRenderHint( QPainter::NonCosmeticDefaultPen );
   //dc.setBrush( Qt::NoBrush );
   dc.fillRect( rect(), QColor( 10,10,10 ) );

   int w = width();
   int h = height();

   //dc.setCompositionMode( QPainter::CompositionMode_SourceOver );

   //DE_DEBUG("m_Lmax = ", m_Lmax )
   int yL = int( (1.0f - m_Lmax) * h );
   int yR = int( (1.0f - m_Rmax) * h );
   QColor colorL = toQColor( m_ColorGradient.getColor32( m_Lmax ) );
   QColor colorR = toQColor( m_ColorGradient.getColor32( m_Rmax ) );
   QColor colorBg( 25,25,25 );
   //QColor color = toQColor( 0xFF10BF20 );

   dc.setPen( Qt::NoPen );
   dc.setBrush( QBrush( colorBg ) );
   dc.drawRect( QRect(0,0,w/2-1,yL) );

   dc.setBrush( QBrush( colorL ) );
   dc.drawRect( QRect(0,yL,w/2-1,h-1-yL) );


   dc.setPen( Qt::NoPen );
   dc.setBrush( QBrush( colorBg ) );
   dc.drawRect( QRect(w/2,0,w/2-1,yR) );

   dc.setBrush( QBrush( colorR ) );
   dc.drawRect( QRect(w/2,yR,w/2-1,h-1-yR) );

//   m_font5x8.drawText( dc, w/2, 1, QString("m_Lmin(%1), m_Lmax(%2), m_Rmin(%3), m_Rmax(%4)")
//      .arg( m_Lmin).arg( m_Lmax ).arg( m_Rmin).arg( m_Rmax ), 0xFFFFFFFF, de::Align::CenterTop );

}
