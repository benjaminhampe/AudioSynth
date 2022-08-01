#include "GFilePlayer.hpp"

#include <de/audio/GetCoverArt.hpp>
#include <de/audio/buffer/BufferIO.hpp>
#include <QString>


GFilePlayer::GFilePlayer( bool bRight, QWidget* parent )
   : QWidget(parent)
   , m_state( Unloaded )
   , m_frameIndex( 0 )
   , m_isLooping( false )
   , m_loopIndex( 0 )
   , m_loopCount( 0 )
   , m_sampleBuffer( de::audio::ST_F32I, 2, 44100 )
{
   setObjectName( "GFilePlayer" );

   m_edtUri = new QLineEdit( "Unloaded.nothing", this );
   m_btnLoad = new QPushButton( "...", this );
   m_btnPlay = new QPushButton( "Play", this );
   m_btnStop = new QPushButton( "Stop", this );

   m_lblPosition = new QLabel( "-:-", this );
   m_sldPosition = new QSlider( Qt::Horizontal, this );
   m_sldPosition->setMinimum( 0 );
   m_sldPosition->setMaximum( 1255 );
   m_sldPosition->setValue( 0 );
   m_lblDuration = new QLabel( "-:-", this );

   m_timeSeek = new GTimeSeek( this );
   m_timeSeek->setBuffer( &m_sampleBuffer );

   m_coverArt = new QImageWidget( this );
   m_coverArt->setMinimumSize(200,200);
   m_coverArt->setMaximumSize(200,200);
   m_coverArt->setVisible( false );

   QHBoxLayout* h1 = new QHBoxLayout();
   h1->setContentsMargins(0,0,0,0);
   h1->setSpacing( 5 );
   h1->addWidget( m_edtUri );
   h1->addWidget( m_btnLoad );

   QHBoxLayout* h2 = new QHBoxLayout();
   h2->setContentsMargins(0,0,0,0);
   h2->setSpacing( 5 );
   h2->addWidget( m_btnPlay );
   h2->addWidget( m_btnStop );

   QHBoxLayout* h3 = new QHBoxLayout();
   h3->setContentsMargins(0,0,0,0);
   h3->setSpacing( 5 );
   h3->addWidget( m_lblPosition );
   h3->addWidget( m_sldPosition );
   h3->addWidget( m_lblDuration );

   QVBoxLayout* v = new QVBoxLayout();
   v->setContentsMargins(0,0,0,0);
   v->setSpacing( 5 );
   v->addLayout( h1 );
   v->addLayout( h2 );
   v->addLayout( h3 );
   v->addWidget( m_timeSeek );

   QHBoxLayout* h4 = new QHBoxLayout();
   h4->setContentsMargins(0,0,0,0);
   h4->setSpacing( 5 );

   if ( bRight )
   {
      h4->addLayout( v );
      h4->addWidget( m_coverArt );
   }
   else
   {
      h4->addWidget( m_coverArt );
      h4->addLayout( v );
   }
   setLayout( h4 );

   connect( m_btnLoad, SIGNAL(clicked(bool)), this, SLOT(on_clicked_load(bool)), Qt::QueuedConnection );
   connect( m_btnPlay, SIGNAL(clicked(bool)), this, SLOT(on_clicked_play(bool)), Qt::QueuedConnection );
   connect( m_btnStop, SIGNAL(clicked(bool)), this, SLOT(on_clicked_stop(bool)), Qt::QueuedConnection );
   connect( m_sldPosition, SIGNAL(valueChanged(int)), this, SLOT(on_slider_pos(int)), Qt::QueuedConnection );

}

GFilePlayer::~GFilePlayer()
{

}

uint64_t
GFilePlayer::readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
{
// using namespace de::audio;

   size_t dstSamples = size_t( dstFrames ) * dstChannels;
   de::audio::DSP_FILLZERO( dst, dstSamples );

   if ( m_state != Playing )
   {
      //DE_ERROR("Not playing")
      return dstSamples;
   }

   if ( m_sampleBuffer.getSampleType() != de::audio::ST_F32I )
   {
      DE_ERROR("No ST_F32I")
      return dstSamples;
   }

//   size_t k = m_sampleBuffer.read(
//                  m_frameIndex, dstFrames, dstChannels, dst, ST_F32I );

   size_t k = m_sampleBuffer.readF32I( dst, dstFrames, dstChannels, m_frameIndex );
   //DE_DEBUG( "k = ",k,", dst.getChannelCount() = ",dst.getChannelCount() )
   m_frameIndex += k;

   if ( m_frameIndex >= m_sampleBuffer.getFrameCount() )
   {
      //DE_DEBUG( "m_FrameIndex(",m_FrameIndex,") + n(",n,") >= m(",m,")" )
      m_state = Stopped;
      m_frameIndex = 0; // rewind dirty hack
   }

   return dstSamples;
}

uint32_t
GFilePlayer::getState() const { return m_state; }

double
GFilePlayer::getDuration() const { return m_sampleBuffer.getDuration(); }

double
GFilePlayer::getPosition() const
{
   double a = m_frameIndex;
   double b = m_sampleBuffer.getFrameCount();
   double c = m_sampleBuffer.getDuration();
   return (a / b) * c;
}

void
GFilePlayer::on_slider_pos( int value )
{
   typedef double T;
   int v = value; // m_sldPosition->value();
   int m = m_sldPosition->minimum();
   int n = m_sldPosition->maximum();
   T t = T( v - m ) / T( n - m );

   m_frameIndex = t * double(m_sampleBuffer.getFrameCount());

   T dur = t * m_sampleBuffer.getDuration();
   T pos = t * dur;

   m_lblPosition->setText( QString::fromStdString( dbStrSeconds( pos ) ) );
   m_lblDuration->setText( QString::fromStdString( dbStrSeconds( dur ) ) );
}

void
GFilePlayer::on_clicked_load(bool checked)
{
   load();
}

void
GFilePlayer::on_clicked_play(bool checked)
{
   play();
   m_edtUri->setText( QString::fromStdString( m_sampleBuffer.getUri() ) );
}

void
GFilePlayer::on_clicked_stop(bool checked)
{
   stop();
}

QImage
toQImage( de::Image const & img )
{
   uint32_t w = img.getWidth();
   uint32_t h = img.getHeight();

   if ( w < 1 || h < 1 )
   {
      return QImage();
   }

   QImage dst( w,h, QImage::Format_ARGB32 );

   for ( size_t j = 0; j < h; ++j )
   {
      for ( size_t i = 0; i < w; ++i )
      {
         uint32_t color = img.getPixel( i,j );
         int r = de::RGBA_R( color );
         int g = de::RGBA_G( color );
         int b = de::RGBA_B( color );
         int a = de::RGBA_A( color );
         dst.setPixelColor( i,j, QColor( r,g,b,255 ) );
      }
   }

   return dst;
}

void
GFilePlayer::load()
{
   // QApplication::applicationDirPath()
   auto files = QFileDialog::getOpenFileNames( this, "Open audio file", "../../" );
   if ( files.count() < 1 )
   {
      qDebug() << "No files selected";
      return;
   }

   if ( m_state > Stopped )
   {
      stop();
   }

   QString uri = files.at( 0 );
   //auto wri = uri.toStdWString();

   stop();

   //m_coverArt->setImage( QImage(), true );

//   DE_DEBUG("Load CoverArt ", uri )
//   DE_FLUSH

   de::audio::CoverArt coverArt;
   de::audio::CoverArt::getCoverArt( coverArt, uri.toStdWString() );
   m_coverArt->setImage( toQImage( coverArt.img ) );

//   DE_DEBUG("Load SampleBuffer ", uri )
//   DE_FLUSH

#if 1
   m_sampleBuffer.clear();
   m_sampleBuffer.shrink_to_fit();
   if ( de::audio::BufferIO::load( m_sampleBuffer, uri.toStdString(), -1, de::audio::ST_F32I ) )
   {
      m_state = Stopped;
      m_edtUri->setText( uri );
      m_lblDuration->setText( QString::fromStdString( dbStrSeconds( m_sampleBuffer.getDuration() ) ) );
   }
   else
   {
      m_state = Unloaded;
      m_edtUri->setText( "LoadError" );
      m_lblDuration->setText( QString::fromStdString( dbStrSeconds( m_sampleBuffer.getDuration() ) ) );
   }

//   DE_DEBUG("Loaded SampleBuffer ", uri )
//   DE_FLUSH

   m_timeSeek->setBuffer( &m_sampleBuffer );
   //m_sampleBuffer.setUri( uri );
#endif
}

void
GFilePlayer::play()
{
   if ( m_state == Unloaded )
   {
      load();
   }

   if ( m_state == Stopped )
   {
      m_state = Playing;
   }
   else if ( m_state == Playing )
   {
      m_state = Paused;
   }
   else if ( m_state == Paused )
   {
      m_state = Playing;
   }
}

void
GFilePlayer::stop()
{
   if ( m_state > Stopped )
   {
      m_state = Stopped;
      m_frameIndex = 0;
      m_lblPosition->setText( "00:00" );
   }
}

void
GFilePlayer::pause()
{
   if ( m_state == Playing )
   {
      m_state = Paused;
   }
}
