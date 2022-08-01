#include <de/audio/SampleBuffer.hpp>
/*
#include <de/audio/Wav.hpp>
*/

namespace de {
namespace audio {

// ===========================================================================
SampleBuffer::SampleBuffer()
// ===========================================================================
   : m_SampleType( ST_Unknown )
   , m_ChannelCount( 0 )
   , m_BytesPerSample( 0 )
   , m_SampleRate( 0 )
   , m_FrameCount(0)
   , m_PTS(0.0)
   , m_Uri()
{
   //DE_DEBUG(toString())
}

SampleBuffer::SampleBuffer( ESampleType sampleType, int channels, float sampleRate, uint64_t frames )
   : SampleBuffer()
{
   setFormat( sampleType, channels, sampleRate );
   if ( frames > 0 ) resize( frames );
   //DE_DEBUG(toString())
}

SampleBuffer::~SampleBuffer()
{
   //DE_DEBUG(toString())
   clear();
}

/*
bool
SampleBuffer::saveWAV_noperf( std::string uri, ESampleType dstType ) const
{
   //int audioBitRate = 192000;
   //ESampleType srcType = dst.getSampleType();
   SampleBuffer dst;
   if ( !this->convert( dst, ST_S16I ) )
   {
      DE_ERROR("Cant convert to S16interleaved")
      return false;
   }
   wav::WavOutFile file( uri.c_str(), dst.getSampleRate(), dst.getBitsPerSample(), dst.getChannelCount() );
   file.write_S16( reinterpret_cast< int16_t const* >( dst.data() ), dst.getSampleCount() );
   return true;
}

bool
SampleBuffer::saveWAV( std::string uri, ESampleType dstType ) const
{
   de::PerformanceTimer perf;
   perf.start();
   DE_DEBUG("[Save] uri = ", toString() )
   bool ok = saveWAV_noperf( uri, dstType );
   perf.stop();
   DE_DEBUG("[Save] needed ", perf.toSeconds(), " seconds.")
   return ok;
}

//   bool
//   loadWAV_noperf( std::string uri, ESampleType dstType = ST_S16I ) const
//   {
//      //int audioBitRate = 192000;
//      //ESampleType srcType = dst.getSampleType();
//      wav::WavInFile file( uri.c_str() );

//      SampleBuffer dst;
//      dst.setSampleRate(
//      file. dst.getSampleRate(), dst.getBitsPerSample(), dst.getChannelCount()
//      file.write_S16( reinterpret_cast< int16_t const* >( dst.data() ), dst.getSampleCount() );

//      if ( !this->convert( dst, dstType ) )
//      {
//         DE_ERROR("Cant convert to S16interleaved")
//         return false;
//      }
//      return true;
//   }

//   bool
//   loadWAV( std::string uri, ESampleType dstType = ST_S16I ) const
//   {
//      de::PerformanceTimer perf;
//      perf.start();
//      DE_DEBUG("[Save] uri = ", toString() )
//      bool ok = save_noperf( uri, dstType );
//      perf.stop();
//      DE_DEBUG("[Save] needed ", perf.toSeconds(), " seconds.")
//      return ok;
//   }
*/


void
SampleBuffer::dump()
{
   std::ostringstream s;
   for ( size_t ch = 0; ch < getChannelCount(); ++ch )
   {
      if ( getChannelCount() > 1 && ch > 0 )
      {
         s << " | ";
      }
      s << "Ch" << ch;
   }
   DE_DEBUG(" ",s.str(), ", channelCount = ", getChannelCount(), ", frameCount = ", getFrameCount() )
   DE_DEBUG("------------------------------" )
   for ( size_t i = 0; i < getFrameCount(); ++i )
   {
      std::ostringstream s;
      //s << "\t";
      for ( size_t ch = 0; ch < getChannelCount(); ++ch )
      {
         if ( getChannelCount() > 1 && ch > 0 )
         {
            s << " | ";
         }
         int16_t sample = getSampleS16( i, ch );
         std::string t = std::to_string( sample );
         s << t;
      }
      //s << "\n";

      //DE_DEBUG("Frame[",i,"]: " )
      DE_DEBUG( s.str() )
   }

   // s << ", dur:" << dbSecondsToString( totalDuration() );
   // s << ", bytes:" << dbStrByteCount( totalBytes() );
}

/*
SampleBuffer&
SampleBuffer::operator= ( SampleBuffer const & other )
{
   if ( getSampleType() == ST_Unknown )
   {
      setSampleType( other.getSampleType() );
   }

   setUri( other.getUri() );
   setChannelCount( other.getChannelCount() );
   setSampleRate( other.getSampleRate() );

   if ( getSampleType() == other.getSampleType() )
   {
      if ( getFrameCount() < other.getFrameCount() )
      {
         resize( other.getFrameCount() );
      }
   }
   else
   {
      DE_DEBUG("Must reset format and resize")
      resize( other.getFrameCount() );
   }

   ::memcpy( this->data(), other.data(), size() );

   return *this;
}
*/

//static
SampleBuffer::Ptr
SampleBuffer::createPtr( ESampleType sampleType, int channels, float sampleRate, uint64_t frames )
{
   Ptr p( new SampleBuffer( sampleType, channels, sampleRate, frames ) );
   return p;
}

// static
SampleBuffer::SharedPtr
SampleBuffer::createShared( ESampleType sampleType, int channels, float sampleRate, uint64_t frames )
{
   SharedPtr p = std::make_shared< SampleBuffer >( sampleType, channels, sampleRate, frames );
   return p;
}

// static
SampleBuffer
SampleBuffer::create( ESampleType sampleType, int channels, float sampleRate, uint64_t frames )
{
   return SampleBuffer( sampleType, channels, sampleRate, frames );
}

//static
SampleBuffer*
SampleBuffer::createNew( ESampleType sampleType, int channels, float sampleRate, uint64_t frames )
{
   return new SampleBuffer( sampleType, channels, sampleRate, frames );
}

std::string
SampleBuffer::getSampleTypeStr() const { return ST_toString( getSampleType() ); }

std::string
SampleBuffer::toString( bool printEnergy ) const
{
   std::stringstream s;
   s << "st:" << getSampleTypeStr() << "; "
     << "ch:" << getChannelCount() << "; "
     << "sr:" << getSampleRate() << "; "
     << "d:" << dbStrSeconds( getDuration() ) << "; "
     << "fc:" << getFrameCount() << ";";

   if ( printEnergy )
   {
      for ( size_t c = 0; c < getChannelCount(); ++c )
      {
         double energy = getChannelAverageEnergyPerSample( c );
         s << ", EnergyCh" << c << " = " << energy;
      }
   }

   return s.str();
}

double
SampleBuffer::getPTS() const { return m_PTS; }
void
SampleBuffer::setPTS( double pts ) { m_PTS = pts; }

ESampleType
SampleBuffer::getSampleType() const { return m_SampleType; }
uint32_t
SampleBuffer::getBytesPerSample() const { return m_BytesPerSample; }
uint32_t
SampleBuffer::getBitsPerSample() const { return m_BytesPerSample << 3; }
uint32_t
SampleBuffer::getChannelCount() const { return m_ChannelCount; }
uint32_t
SampleBuffer::getFrameSize() const { return getBytesPerSample() * getChannelCount(); }
bool
SampleBuffer::isInterleaved() const { return ST_isInterleaved( m_SampleType ); }
float
SampleBuffer::getSampleRate() const { return m_SampleRate; }
uint64_t
SampleBuffer::getFrameCount() const { return m_FrameCount; }
uint64_t
SampleBuffer::getSampleCount() const { return m_FrameCount * getChannelCount(); }

//uint64_t
//SampleBuffer::capacity() const { return m_FrameCount; }
uint64_t
SampleBuffer::getByteCount() const { return m_Samples.size(); }

uint64_t
SampleBuffer::size() const { return m_Samples.size(); }
uint8_t*
SampleBuffer::data() { return m_Samples.data(); }
uint8_t const*
SampleBuffer::data() const { return m_Samples.data(); }

double
SampleBuffer::getDuration() const
{
   if ( getSampleRate() < 0.0001f ) { return 0.0; }
   else { return double( getFrameCount() ) / double( getSampleRate() ); }
}

std::string const &
SampleBuffer::getUri() const { return m_Uri; }
void
SampleBuffer::setUri( std::string const & uri ) { m_Uri = uri; }
void
SampleBuffer::setChannelCount( int32_t channels )
{
   m_ChannelCount = uint16_t( channels );
   if ( m_ChannelCount > 16 )
   {
      DE_ERROR("Got m_ChannelCount(",m_ChannelCount,") > 16")
   }
}
void
SampleBuffer::setSampleRate( float rateInHz ) { m_SampleRate = rateInHz; }
void
SampleBuffer::setSampleType( ESampleType sampleType )
{
   m_SampleType = sampleType;
   m_BytesPerSample = ST_getBytesPerSample( sampleType );
}
void
SampleBuffer::setFormat( ESampleType sampleType, int channels, float sampleRate )
{
   setSampleType( sampleType );
   setChannelCount( channels );
   setSampleRate( sampleRate );
}

bool
SampleBuffer::isValid() const
{
   if ( m_BytesPerSample < 1 ) return false;
   if ( m_ChannelCount < 1 ) return false;
   if ( m_SampleRate < 0.0001f ) return false;
   return true;
}

// WaveFormat64 const & getFormat() const { return m_Header; }

void
SampleBuffer::shrink_to_fit()
{
   m_Samples.shrink_to_fit();
}

void
SampleBuffer::clear()
{
   m_Samples.clear();
   m_FrameCount = 0;
   // m_SampleType = ST_Unknown;
   // m_ChannelCount = m_BytesPerSample = 0;
   // m_SampleRate = 0.f;
}

void
SampleBuffer::resize( uint64_t frameCount )
{
   m_FrameCount = frameCount;
   uint64_t byteCount = frameCount * m_BytesPerSample * m_ChannelCount;
   if ( m_Samples.size() < byteCount )
   {
      //DE_DEBUG("resize(",m_Samples.size()," -> ",byteCount,") :: ", toString() )
      m_Samples.resize( byteCount, 0x00 );
   }
}

uint64_t
SampleBuffer::getStride() const
{
   uint32_t bps = getBytesPerSample();
   if ( bps < 1 ) { return 0; }
   return( isInterleaved() ? bps * getChannelCount() : getFrameCount() * bps );
}

// but ffmpeg want an lumpy signed int 32bit,
// so i dont know how future prove this can be, i guess never.
int32_t
SampleBuffer::getStridei() const
{
   uint64_t stride = getStride();
   if ( stride > uint64_t( INT32_MAX ) )
   {
      //DE_ERROR("Too many frames for ffmpeg!?")
      stride = uint64_t( INT32_MAX );
   }
   return stride;
}

uint64_t
SampleBuffer::computeByteIndex( uint64_t frame, uint32_t channel ) const
{
   if ( getChannelCount() == 1 ) channel = 0;
   if ( isInterleaved() )
   {
      return (frame * getChannelCount() + channel) * getBytesPerSample();
   }
   else
   {
      return (getFrameCount() * channel + frame) * getBytesPerSample();
   }
}


// if planar (ch0,ch0,ch1,ch1): get full frameCount times bps, which is ( in worst case ) already 64bit
// if interleaved (ch0,ch1,ch0,ch1) : get number of channels times bps.

// Frames are x-axis, channels are on y-axis, buffer is like an image.
uint8_t*
SampleBuffer::getSamples( uint64_t frame, uint32_t channel )
{
   uint64_t b = computeByteIndex( frame, channel );
   if ( b >= m_Samples.size() )
   {
      // DE_ERROR("byteIndex(",byteIndex,") >= m_Samples.size(",m_Samples.size(),")")
      return nullptr;
   }
   return &m_Samples[ b ];
}

uint8_t const*
SampleBuffer::getSamples( uint64_t frame, uint32_t channel ) const
{
   uint64_t b = computeByteIndex( frame, channel );
   if ( b >= m_Samples.size() )
   {
      //DE_ERROR("byteIndex(",byteIndex,") >= m_Samples.size(",m_Samples.size(),")")
      return nullptr;
   }
   return &m_Samples[ b ];
}

// Most important data getter, can access any sample like a pixel,
// no matter if you loop over frames or channels first, no-matter the buffer
// is interleaved or planar. Just access the sample you want.
bool
SampleBuffer::getSample( uint64_t frame, uint32_t channel, void * dst ) const
{
   if ( !dst ) { DE_ERROR("No dst") return false; }

   if ( getChannelCount() == 1 ) channel = 0;

   uint8_t const* src = getSamples( frame, channel );
   if ( !src ) { DE_ERROR("No src") return false; }
   uint32_t bps = getBytesPerSample();
   if ( bps < 1 ) { DE_ERROR("No bps") return false; }
   ::memcpy( dst, src, bps );
   return true;
}

bool
SampleBuffer::getSample( uint64_t frame, uint32_t channel, void * dst, SampleTypeConverter::Converter_t const & converter ) const
{
   if ( !dst ) { DE_ERROR("No dst") return false; }
   if ( getChannelCount() == 1 ) channel = 0;
   uint8_t const* src = getSamples( frame, channel );
   if ( !src ) { DE_ERROR("No src") return false; }
   converter( src, dst, 1 );
   return true;
}

bool
SampleBuffer::getSample( uint64_t frame, uint32_t channel, void * dst, ESampleType dstType ) const
{
   if ( !dst ) { DE_ERROR("No dst") return false; }
   if ( getChannelCount() == 1 ) channel = 0;

   uint8_t const* src = getSamples( frame, channel );
   if ( !src ) { DE_ERROR("No src") return false; }
   SampleTypeConverter::Converter_t converter =
      SampleTypeConverter::getConverter( getSampleType(), dstType );
   if ( !converter ) { DE_ERROR("No converter") return false; }
   converter( src, dst, 1 );
   return true;
}

// Most important data setter, can set any single sample like a pixel in an image,
// no matter if you loop over frames or channels first, no-matter the buffer
// is interleaved or planar. Just set the sample you want like setPixel( img,x,y,color ).
bool
SampleBuffer::setSample( uint64_t frame, uint32_t channel, void const * src )
{
   if ( !src ) { DE_ERROR("No src") return false; }
   if ( getChannelCount() == 1 ) channel = 0;
   uint8_t* dst = getSamples( frame, channel );
   if ( !dst ) { DE_ERROR("No dst") return false; }
   uint32_t bps = getBytesPerSample();
   if ( bps < 1 ) { DE_ERROR("No bps") return false; }
   ::memcpy( dst, src, bps );
   return true;
}

bool
SampleBuffer::setSample( uint64_t frame, uint32_t channel, void const * src, ESampleType srcType )
{
   if ( !src ) { DE_ERROR("No src") return false; }

   if ( getChannelCount() == 1 ) channel = 0;

   uint8_t* dst = getSamples( frame, channel );
   if ( !dst ) { DE_ERROR("No dst") return false; }
   SampleTypeConverter::Converter_t converter =
      SampleTypeConverter::getConverter( srcType, getSampleType() );
   if ( !converter ) { DE_ERROR("No converter") return false; }
   converter( src, dst, 1 );
   return true;
}

bool
SampleBuffer::setSampleS16( uint64_t frame, uint32_t channel, int16_t value )
{
   int16_t* dst = reinterpret_cast< int16_t* >( getSamples( frame, channel ) );
   if ( !dst ) { DE_ERROR("No dst") return false; }
   *dst = value;
   return true;
}

bool
SampleBuffer::setSampleF32( uint64_t frame, uint32_t channel, float value )
{
   auto dst = reinterpret_cast< float* >( getSamples( frame, channel ) );
   if (!dst) { DE_ERROR("No dst") return false; }
   *dst = value;
   return true;
}

int16_t
SampleBuffer::getSampleS16( uint64_t frame, uint32_t channel ) const
{
   int16_t const* dst = reinterpret_cast< int16_t const* >( getSamples( frame, channel ) );
   if ( !dst )
   {
      DE_ERROR("OutOfBounds "
               "frame(",frame,"), "
               "channel(",channel,"), "
               "frameCount(",getFrameCount(),"), "
               "channelCount(",getChannelCount(),")")
      return 0;
   }
   return *dst;
}

float
SampleBuffer::getSampleF32( uint64_t frame, uint32_t channel ) const
{
   auto dst = reinterpret_cast< float const* >( getSamples( frame, channel ) );
   if (!dst)
   {
      DE_ERROR("OutOfBounds "
               "frame(",frame,"), "
               "channel(",channel,"), "
               "frameCount(",getFrameCount(),"), "
               "channelCount(",getChannelCount(),")")
      return 0;
   }

   return *dst;
}


// Interleaved <-> Deinterleaved ( its the same operation )
void
SampleBuffer::interleave()
{
   //DE_DEBUG( "interleave")

   std::vector< uint8_t > tmp( m_Samples.size() );

   size_t w = getFrameCount();
   size_t h = getChannelCount();
   size_t bps = getBytesPerSample();

   for ( size_t y = 0; y < h; ++y )
   {
      for ( size_t x = 0; x < w; ++x )
      {
         size_t i = (w * y + x) * bps; // original
         size_t j = (h * x + y) * bps; // rotated 90° ccw ( math positive )
         uint8_t const* src = reinterpret_cast< uint8_t const* >( m_Samples.data() ) + i;
         uint8_t* dst = reinterpret_cast< uint8_t* >( tmp.data() ) + j;
         ::memcpy( dst, src, bps );
      }
   }

   m_Samples = std::move( tmp );
}

// Convert
bool
SampleBuffer::convert( SampleBuffer & dst, ESampleType dstType ) const
{
   ESampleType const srcType = getSampleType();
   if ( srcType == ST_Unknown )
   {
      DE_ERROR("Cant convert empty buffer")
      return false;
   }

   if ( dstType == ST_Unknown )
   {
      dstType = dst.getSampleType();
   }

   if ( srcType == dstType )
   {
      dst = *this;
      DE_WARN("Copies ",dst.getSampleCount()," samples "
            "from srcType(",ST_toString(srcType),") to dstType(",ST_toString(dstType),")")
      return true;
   }

   SampleTypeConverter::Converter_t func = SampleTypeConverter::getConverter( srcType, dstType );
   if ( !func )
   {
      DE_ERROR("No Converter from srcType(",ST_toString(srcType),") "
               "to dstType(",ST_toString(dstType),") with ",dst.getSampleCount()," samples")
      return false;
   }

   dst.clear();
   dst.setFormat( dstType, getChannelCount(), getSampleRate() );
   dst.resize( getFrameCount() );

   func( this->data(), dst.data(), dst.getSampleCount() );

   if ( ST_isInterleaved( srcType ) != ST_isInterleaved( dstType ) )
   {
      dst.interleave();

      // DE_DEBUG("Converted + Interleaved ",dst.getSampleCount()," samples "
      //         "from srcType(",ST_toString(srcType),") "
      //         "to dstType(",ST_toString(dstType),")")
   }
   else {
      //DE_DEBUG("Converted ",dst.getSampleCount()," samples "
      //   "from srcType(",ST_toString(srcType),") "
      //   "to dstType(",ST_toString(dstType),")")
   }

   return true;
}
//uint64_t push_back_samples( void const* src, uint64_t sampleCount, ESampleType srcType );

bool
SampleBuffer::sub( SampleBuffer & dst,
             uint64_t frameStart,
             uint64_t frameCount,
             ESampleType dstType ) const
{
#if 0
   DE_DEBUG("SUB() :: dst(", dst.toString(), "), "
            "frameStart(", frameStart, "), "
            "frameCount(", frameCount, "), "
            "dstType(", ST_toString(dstType), ")" )
#endif
   if ( frameCount < 1 )
   {
      DE_WARN("frameCount < 1")
      return false;
   }

   if ( frameStart > getFrameCount() )
   {
      DE_WARN("frameStart > getFrameCount()")
      return false;
   }

   if ( frameStart + frameCount > getFrameCount() )
   {
      DE_WARN("frameStart + frameCount > getFrameCount()")
      frameCount = getFrameCount() - frameStart;
   }

   ESampleType srcType = getSampleType();
   if ( dstType == ST_Unknown )
   {
      dstType = getSampleType();
   }

   SampleTypeConverter::Converter_t converter =
      SampleTypeConverter::getConverter( srcType, dstType );
   if ( !converter )
   {
      DE_WARN("No converter srcType(",ST_toString(srcType),") to "
                           "dstType(",ST_toString(dstType),")")
      return false;
   }

   dst.setFormat( dstType, getChannelCount(), getSampleRate() );
   dst.resize( frameCount );

   for ( size_t i = 0; i < frameCount; ++i )
   {
      for ( size_t ch = 0; ch < getChannelCount(); ++ch )
      {
         uint8_t const * p_src = getSamples( i + frameStart, ch );
         if ( !p_src )
         {
            DE_ERROR("no src pointer, frame(",i,"), channel(",ch,")")
            continue;
         }

         uint8_t* p_dst = dst.getSamples( i, ch );
         if ( !p_dst )
         {
            DE_ERROR("no dst pointer, frame(",frameStart + i,"), channel(",ch,")")
            continue;
         }
         converter( p_src, p_dst, 1 );
      }
   }

   return true;
}

/*
SampleBuffer
SampleBuffer::sub( uint64_t frameStart, uint64_t frameCount, ESampleType dstType ) const
{
   if ( frameCount < 1 )
   {
      DE_WARN("frameCount < 1")
      return {};
   }

   if ( frameStart > getFrameCount() )
   {
      DE_WARN("frameStart > getFrameCount()")
      return {};
   }

   if ( frameStart + frameCount > getFrameCount() )
   {
      DE_WARN("frameStart + frameCount > getFrameCount()")
      frameCount = getFrameCount() - frameStart;
   }

   ESampleType srcType = getSampleType();
   if ( dstType == ST_Unknown )
   {
      dstType = getSampleType();
   }

   SampleTypeConverter::Converter_t converter =
      SampleTypeConverter::getConverter( srcType, dstType );
   if ( !converter )
   {
      DE_WARN("No converter srcType(",ST_toString(srcType),") to "
                           "dstType(",ST_toString(dstType),")")
      return {};
   }

   SampleBuffer dst;
   dst.setFormat( dstType, getChannelCount(), getSampleRate() );
   dst.resize( frameCount );

   for ( size_t i = 0; i < frameCount; ++i )
   {
      for ( size_t ch = 0; ch < getChannelCount(); ++ch )
      {
         uint8_t const * p_src = getSamples( i + frameStart, ch );
         if ( !p_src )
         {
            DE_ERROR("no src pointer, frame(",i,"), channel(",ch,")")
            continue;
         }

         uint8_t* p_dst = dst.getSamples( i, ch );
         if ( !p_dst )
         {
            DE_ERROR("no dst pointer, frame(",frameStart + i,"), channel(",ch,")")
            continue;
         }
         converter( p_src, p_dst, 1 );
      }
   }

   return dst;
}
*/

uint64_t
SampleBuffer::write( uint64_t dstFrameStart,
      SampleBuffer src, uint64_t srcFrameStart, uint64_t srcFrameCount )
{
   // Check V:
   if ( src.getFrameCount() < 1 )
   {
      //DE_WARN("Nothing to write")
      return 0;
   }

   // Check V:
   if ( srcFrameCount < 1 )
   {
      //DE_WARN("Nothing to write")
      return 0;
   }

// Check I:
   uint32_t srcChannels = src.getChannelCount();
   uint32_t dstChannels = this->getChannelCount();
   if ( srcChannels != dstChannels )
   {
      //DE_WARN("srcChannels(",srcChannels,") != dstChannels(",dstChannels,"), need limiter.")
      //continue;
   }

// Check II:
   ESampleType srcType = src.getSampleType();
   ESampleType dstType = this->getSampleType();
   bool needConverter = false;
   if ( srcType != dstType )
   {
      needConverter = true;
      //DE_WARN("srcType(",ST_toString(srcType),") != dstType(",ST_toString(dstType),"), need converter.")
      //continue;
   }

// Check III:
   float srcRate = src.getSampleRate();
   float dstRate = this->getSampleRate();
   bool needResampler = false;
   if ( srcRate != dstRate )
   {
      needResampler = true;
      //DE_WARN("srcRate(",srcRate,") != dstRate(",dstRate,"), need resampler!")
   }

// Check IV:
   bool srcInterleaved = ST_isInterleaved( srcType );
   bool dstInterleaved = ST_isInterleaved( dstType );
   bool needInterleave = false;
   if ( srcInterleaved != dstInterleaved )
   {
      needInterleave = true;
      //DE_WARN("srcInterleaved(",srcInterleaved,") != dstInterleaved(",dstInterleaved,"), need interleave!")
      src.interleave();
   }

// Check V: Ok
   SampleTypeConverter::Converter_t converter = SampleTypeConverter::getConverter( srcType, dstType );
   if ( !converter )
   {
      //DE_WARN("No converter srcType(",ST_toString(srcType),") to dstType(",ST_toString(dstType),")")
      // continue;
      return 0;
   }

//   uint32_t src_bps = src.getBytesPerSample();
//   uint32_t dst_bps = this->getBytesPerSample();

//   uint8_t buf[256];
//   memset( buf, 0, 256 );

   for ( size_t i = 0; i < srcFrameCount; ++i )
   {
      for ( size_t ch = 0; ch < std::min( srcChannels, dstChannels ); ++ch )
      {
         size_t srcFrameIndex = i + srcFrameStart;
         uint8_t const * p_src = src.getSamples( srcFrameIndex, ch );
         if ( !p_src )
         {
            DE_ERROR("no src pointer, frame(",srcFrameIndex,"), channel(",ch,")")
            continue;
         }

         size_t dstFrameIndex = i + dstFrameStart;
         uint8_t* p_dst = this->getSamples( dstFrameIndex, ch );
         if ( !p_dst )
         {
            DE_ERROR("no dst pointer, frame(",dstFrameIndex,"), channel(",ch,")")
            continue;
         }
         converter( p_src, p_dst, 1 );
      }
   }

   return srcFrameCount;
}


uint64_t
SampleBuffer::write( uint64_t frameStart, SampleBuffer src )
{
#if 0
   DE_DEBUG("Write src(",src.toString(),") to dst(",toString(),") at frameStart(",frameStart,")")
#endif
   // Check V:
   uint64_t srcFrames = src.getFrameCount();
   if ( srcFrames < 1 )
   {
      //DE_WARN("Nothing to write")
      return 0;
   }

// Check I:
   uint32_t srcChannels = src.getChannelCount();
   uint32_t dstChannels = this->getChannelCount();
   if ( srcChannels != dstChannels )
   {
      //DE_WARN("srcChannels(",srcChannels,") != dstChannels(",dstChannels,"), need limiter.")
      //continue;
   }

// Check II:
   ESampleType srcType = src.getSampleType();
   ESampleType dstType = this->getSampleType();
   bool needConverter = false;
   if ( srcType != dstType )
   {
      needConverter = true;
      //DE_WARN("srcType(",ST_toString(srcType),") != dstType(",ST_toString(dstType),"), need converter.")
      //continue;
   }

// Check III:
   float srcRate = src.getSampleRate();
   float dstRate = this->getSampleRate();
   bool needResampler = false;
   if ( srcRate != dstRate )
   {
      needResampler = true;
      //DE_WARN("srcRate(",srcRate,") != dstRate(",dstRate,"), need resampler!")
   }

// Check IV:
   bool srcInterleaved = ST_isInterleaved( srcType );
   bool dstInterleaved = ST_isInterleaved( dstType );
   bool needInterleave = false;
   if ( srcInterleaved != dstInterleaved )
   {
      needInterleave = true;
      //DE_WARN("srcInterleaved(",srcInterleaved,") != dstInterleaved(",dstInterleaved,"), need interleave!")
      src.interleave();
   }

// Check V: Ok
   SampleTypeConverter::Converter_t converter = SampleTypeConverter::getConverter( srcType, dstType );
   if ( !converter )
   {
      //DE_WARN("No converter srcType(",ST_toString(srcType),") to dstType(",ST_toString(dstType),")")
      // continue;
      return 0;
   }

//   uint32_t src_bps = src.getBytesPerSample();
//   uint32_t dst_bps = this->getBytesPerSample();

//   uint8_t buf[256];
//   memset( buf, 0, 256 );

   for ( size_t frame = 0; frame < srcFrames; ++frame )
   {
      for ( size_t ch = 0; ch < std::min( srcChannels, dstChannels ); ++ch )
      {
         uint8_t const * p_src = src.getSamples( frame, ch );
         if ( !p_src )
         {
            DE_ERROR("no src pointer, frame(",frame,"), channel(",ch,")")
            continue;
         }

         uint8_t* p_dst = this->getSamples( frameStart + frame, ch );
         if ( !p_dst )
         {
            //DE_ERROR("no dst pointer, frame(",frameStart + frame,"), channel(",ch,")")
            continue;
         }
         converter( p_src, p_dst, 1 );
      }
   }

   return srcFrames;
}

// interleaved or planar. Just set the sample you want like setPixel( img,x,y,color ).
uint64_t
SampleBuffer::read( uint64_t frameStart, uint64_t frameCount, uint32_t dst_channels, void* dst, ESampleType dstType ) const
{
   ST_Validate( getSampleType() );

   uint32_t src_channels = this->getChannelCount();
   ESampleType srcType = this->getSampleType();
   if ( srcType == ST_Unknown )
   {
      DE_ERROR("Cant convert empty buffer")
      return 0;
   }

   if ( dstType == ST_Unknown ) { dstType = srcType; }

   SampleTypeConverter::Converter_t converter =
      SampleTypeConverter::getConverter( srcType, dstType );
   if ( !converter )
   {
      DE_ERROR("[Read] No ST_Converter from "
               "srcType(",ST_toString(srcType),") to "
               "dstType(",ST_toString(dstType),")")
      return 0;
   }
   else
   {
//      DE_DEBUG("ST_Converter from "
//               "srcType(",ST_toString(srcType),") to "
//               "dstType(",ST_toString(dstType),")")
   }
   if ( dst_channels != src_channels )
   {
      //DE_WARN("dst_channels(",dst_channels,") != src_channels(",src_channels,")")
   }
   if ( ST_isInterleaved( srcType ) != ST_isInterleaved( dstType ) )
   {
      DE_WARN("ST_isInterleaved(",ST_toString(srcType),") != ST_isInterleaved(",ST_toString(dstType),")")

   }

   if ( !ST_isInterleaved( dstType ) )
   {
      DE_WARN("!ST_isInterleaved( dstType )")
   }

   uint32_t dst_bps = ST_getBytesPerSample( dstType );
   uint8_t* pDst = reinterpret_cast< uint8_t* >( dst );
   for ( uint64_t i = 0; i < frameCount; ++i )
   {
      for ( uint32_t c = 0; c < dst_channels; ++c )
      {
         auto pSrc = getSamples( frameStart + i, c );
         if ( pSrc )
         {
            converter( pSrc, pDst, 1 );
         }
         else
         {
            DE_ERROR("No pSrc ch(",c,"), frame(",i,")")
         }
         pDst += dst_bps;

      }
   }

   return frameCount;
}

uint64_t
SampleBuffer::zeroMemory( uint64_t frameStart )
{
   if ( frameStart >= getFrameCount() )
   {
      return 0;
   }

   uint64_t byteCount = (getFrameCount() - frameStart)*getBytesPerSample();
   if ( byteCount < 1 )
   {
      return 0;
   }

   if ( ST_isInterleaved( getSampleType() ) )
   {
      byteCount *= getChannelCount();
      auto p = getSamples( frameStart );
      if ( p && byteCount )
      {
         memset( p, 0, byteCount );
      }
   }
   else
   {
      for ( uint32_t c = 0; c < getChannelCount(); ++c )
      {
         auto p = getSamples( frameStart, c );
         if ( p )
         {
            memset( p, 0, byteCount );
         }
      }
   }
}

// interleaved or planar. Just set the sample you want like setPixel( img,x,y,color ).
uint64_t
SampleBuffer::read( uint64_t srcFrameStart, SampleBuffer & dst, uint64_t dstFrameStart ) const
{
   if ( dstFrameStart >= dst.getFrameCount() )
   {
      DE_ERROR("dstFrameStart >= dst.getFrameCount()")
      return 0;
   }

   SampleBuffer const & src = *this;
   uint64_t srcFrames = src.getFrameCount();
   if ( srcFrames < 1 )
   {
      DE_ERROR("No srcFrames")
      return 0;
   }

   uint64_t dstFrames = dst.getFrameCount() - dstFrameStart;
   if ( dstFrames < 1 )
   {
      DE_ERROR("No dstFrames")
      return 0;
   }

   ESampleType srcType = src.getSampleType();
   ESampleType dstType = dst.getSampleType();
   if ( srcType == ST_Unknown )
   {
      DE_ERROR("No srcType")
      return 0;
   }
   if ( dstType == ST_Unknown )
   {
      DE_ERROR("No dstType")
      return 0;
   }

   uint32_t srcChannels = src.getChannelCount();
   uint32_t dstChannels = dst.getChannelCount();
   if ( srcChannels != dstChannels )
   {
      DE_ERROR("srcChannels(",srcChannels,") != dstChannels(",dstChannels,")")
   }

   SampleTypeConverter::Converter_t converter =
      SampleTypeConverter::getConverter( srcType, dstType );
   if ( !converter )
   {
      DE_ERROR("[Read] No ST_Converter from "
               "srcType(",ST_toString(srcType),") to "
               "dstType(",ST_toString(dstType),")")
      return 0;
   }

//      DE_DEBUG("ST_Converter from "
//               "srcType(",ST_toString(srcType),") to "
//               "dstType(",ST_toString(dstType),")")
   if ( !ST_isInterleaved( dstType ) )
   {
      DE_ERROR("!isInterleaved( dstType )")
   }


   uint64_t avaFrames = 0;

   if ( srcFrameStart + dstFrames >= srcFrames )
   {
      DE_WARN("READ BUFFER END")
      avaFrames = srcFrames - srcFrameStart;
      if ( avaFrames >= dstFrames ) { DE_ERROR("ERRROOOOR") return 0; }
   }
   else
   {
      avaFrames = dstFrames;
   }

   if ( ST_isInterleaved( srcType ) != ST_isInterleaved( dstType ) )
   {
      DE_WARN("isInterleaved(",ST_toString(srcType),") != isInterleaved(",ST_toString(dstType),")")

   }
//   else
//   {
//      uint32_t dst_bps = ST_getBytesPerSample( dstType );
//      auto pDst = reinterpret_cast< uint8_t* >( dst.data() );
//      auto pSrc = src.getSamples( srcFrameStart, c );

//      for ( size_t i = 0; i < dst.getSampleCount(); ++i )
//      {

//            converter( pSrc, pDst, 1 );
//            pDst += dst_bps;
//         }
//      }

//   }

   // Slow generic very flexible version
   std::array< uint8_t, 8 > sample{ 0 };

   for ( uint64_t i = 0; i < avaFrames; ++i )
   {
      for ( uint32_t c = 0; c < dstChannels; ++c )
      {
         auto pSrc = src.getSamples( srcFrameStart + i, c );
         if (!pSrc)
         {
            continue;
         }

         converter( pSrc, sample.data(), 1 );
         dst.setSample( dstFrameStart + i, c, sample.data() );
      }
   }

   return avaFrames;
}

double
SampleBuffer::getChannelAverageEnergyPerSample( int channel ) const
{
   double energy = 0.0;

   ESampleType srcType = getSampleType();
   ESampleType dstType = ST_F32I;
   SampleTypeConverter::Converter_t converter = SampleTypeConverter::getConverter( srcType, dstType );
   if ( !converter )
   {
      //DE_WARN("No converter srcType(",ST_toString(srcType),") to dstType(",ST_toString(dstType),")")
      return energy;
   }

   for ( size_t i = 0; i < getFrameCount(); ++i )
   {
      float sample;
      converter( getSamples( i, channel ), &sample, 1 );
      energy += double( sample ) * double( sample );
   }

   if ( getFrameCount() > 0 )
   {
      // average energy per sample,
      // gives more info about bugs than just the sum over
      // any (wrong) number of samples/frames, where the sum would stay const.
      energy /= double( getFrameCount() );
   }
   return energy;
}








SampleBufferQueue::SampleBufferQueue()
   : m_FrameCount( 0 )
   , m_FrameIndex( 0 )
   , m_ChunkIndex( 0 )
   , m_FrameOffset( 0 )
{
   DE_DEBUG("Constructor")
}

SampleBufferQueue::~SampleBufferQueue()
{
   DE_DEBUG("Destructor")
   clear();
}

std::string
SampleBufferQueue::toString() const
{
   std::stringstream s;
   s << ", chunkCount = " << getChunkCount();
   s << ", chunkIndex = " << getChunkIndex();
   s << ", chunkOffset = " << getChunkOffset();
   s << ", frameIndex = " << getFrameCount();
   s << ", frameCount = " << getFrameIndex();
   // s << ", dur:" << dbSecondsToString( totalDuration() );
   // s << ", bytes:" << dbStrByteCount( totalBytes() );
   return s.str();
}

void
SampleBufferQueue::dump()
{
   for ( size_t i = 0; i < getChunkCount(); ++i )
   {
      if ( !m_Queue[ i ] )
      {
         DE_ERROR("Got nullptr Queue[",i,"]:" )
      }
      else {
         m_Queue[ i ]->dump();
      }

   }
   DE_DEBUG("chunkCount = ", getChunkCount() )
   DE_DEBUG("chunkIndex = ", getChunkIndex() )
   DE_DEBUG("chunkOffset = ", getChunkOffset() )
   DE_DEBUG("frameIndex = ", getFrameCount() )
   DE_DEBUG("frameCount = ", getFrameIndex() )

   // s << ", dur:" << dbSecondsToString( totalDuration() );
   // s << ", bytes:" << dbStrByteCount( totalBytes() );
}

// uint64_t SampleBufferQueue::getChunkIndex() const { return m_ChunkIndex; }
// uint64_t SampleBufferQueue::getChunkOffset() const { return m_FrameOffset; }
// uint64_t SampleBufferQueue::getChunkCount() const { return m_Queue.size(); }

// uint64_t SampleBufferQueue::getFrameIndex() const { return m_FrameIndex; }
// uint64_t SampleBufferQueue::getFrameCount() const { return m_FrameCount; }
// uint64_t SampleBufferQueue::getAvailCount() const { return m_FrameCount - (m_FrameIndex + m_FrameOffset ); }

void
SampleBufferQueue::clear()
{
   std::lock_guard< std::mutex > guard( m_Mutex );
   m_Queue.clear();
   m_FrameIndex = 0;
   m_FrameCount = 0;
   m_ChunkIndex = 0;
   m_FrameOffset = 0;
}

// [1.]
// ---------------------------------------------------------
//    m_FrameCount = 0, m_FrameIndex = 0;
// push_back( AAAAAABBBBBB )
//    [AAAAAABBBBBB], m_FrameCount = 12, m_FrameIndex = 0;
// pop_front( KKKKKKKK )
//    [BBBB], m_FrameCount = 12, m_FrameIndex = 8;
// push_back( CCCCCC )
//    [BBBBCCCCCC], m_FrameCount = 18, m_FrameIndex = 8;
// pop_front( KKKKKKKK )
//    [CC], m_FrameCount = 18, m_FrameIndex = 16;
// push_back( DDDDDD )
//    [CCDDDDDD], m_FrameCount = 24, m_FrameIndex = 16;
// pop_front( KKKKKKKK )
//    [], m_FrameCount = 24, m_FrameIndex = 24;

//#ifndef DE_DEBUG_BUFFER_QUEUE
//#define DE_DEBUG_BUFFER_QUEUE
//#endif

//#ifndef DE_TRACE_BUFFER_QUEUE
//#define DE_TRACE_BUFFER_QUEUE
//#endif
void
SampleBufferQueue::push_back( SampleBuffer::Ptr && src, double pts )
{
   if ( !src )
   {
      DE_ERROR("Got nullptr")
      return;
   }

   if ( src->getFrameCount() < 1 )
   {
      DE_ERROR("Got empty buffer")
      return;
   }

   std::lock_guard< std::mutex > guard( m_Mutex );

   m_FrameCount += src->getFrameCount();

#ifdef DE_DEBUG_BUFFER_QUEUE
   DE_DEBUG("push_back(",src->getFrameCount(),"), "
            "fc(",m_FrameCount,"), fi(",m_FrameIndex,"), pts(",pts,")" )
#endif

   m_Queue.emplace_back( std::move( src ) );
}

uint64_t
SampleBufferQueue::pop_front( SampleBuffer & dst, double pts )
{
   uint64_t dstFrames = dst.getFrameCount();
   if ( dstFrames < 1 )
   {
      DE_ERROR("Got empty dst buffer")
      return 0;
   }

   std::lock_guard< std::mutex > guard( m_Mutex );

   if ( m_FrameIndex + dstFrames > m_FrameCount )
   {
      DE_ERROR("Not enough data dstFrames(",dstFrames,"), "
                                 "fc(",m_FrameCount,"), "
                                 "fi(",m_FrameIndex,")")
      return 0;
   }

// 1st :: WRITE CHUNKS UNTIL queue is empty OR dstFrames == readFrames
   //ESampleType dstType = dst.getSampleType(); // Not needed, write() already uses dst.getSampleType().
   uint64_t desiredFrames = dst.getFrameCount(); // used as count-down, makes life easier.
   uint64_t totalFrames = 0;
   //size_t finishedFrames = 0; // count-up does not really help.
   //size_t readChunks = 0;
   //size_t chunkIndex = 0;
   uint64_t i = 0;
#ifdef DE_DEBUG_BUFFER_QUEUE
   DE_DEBUG("pop_front(",dst.getFrameCount(),"), "
            "off(",m_FrameOffset,"), "
            "c(",m_FrameCount,"), "
            "i(",m_FrameIndex,"), "
            "pts(",pts,")" )
#endif

   while ( desiredFrames > 0 )
   {
      if ( m_Queue.empty() )  // moved check into while body for debug message
      {
         DE_ERROR("Queue is empty")
         break;
      }

      SampleBuffer::Ptr & chunk = m_Queue.front();
      if ( !chunk )
      {
         DE_ERROR("Got nullptr, unexpected")
         m_Queue.pop_front();
         i++;
         continue;
      }

      uint64_t availFrames = chunk->getFrameCount() - m_FrameOffset;
#ifdef DE_DEBUG_BUFFER_QUEUE
      DE_DEBUG("it[", i, "] availFrames = ", availFrames )
//         DE_DEBUG("it[", i, "] chunk->getFrameCount() = ", chunk->getFrameCount() )
//         DE_DEBUG("it[", i, "] m_FrameOffset = ", m_FrameOffset )
//         DE_DEBUG("it[", i, "] m_FrameIndex = ", m_FrameIndex )
//         DE_DEBUG("it[", i, "] m_FrameCount = ", m_FrameCount )
#endif

      if ( availFrames < 1 )
      {
         DE_ERROR("No frames avail in this chunk")
         m_FrameIndex += chunk->getFrameCount();
         m_FrameOffset = 0;
         m_ChunkIndex++;
         m_Queue.pop_front();
         //m_FrameIndex = 0;
         //m_FrameIndex = 0;
         i++;
         continue;
      }

      // [1-A] Need to eat chunk entirely.
      if ( desiredFrames >= availFrames )
      {
         // Read rest of chunk
         if ( m_FrameOffset > 0 )
         {
#ifdef DE_DEBUG_BUFFER_QUEUE
            DE_DEBUG("REST OF CHUNK" )
            DE_DEBUG("it[", i, "] m_FrameOffset = ", m_FrameOffset )
#endif
            chunk->sub( m_WorkBuffer, m_FrameOffset, availFrames ); // chunkFrames is already adapted ( see -= ).
            uint64_t writtenFrames = dst.write( totalFrames, m_WorkBuffer );
#ifdef DE_DEBUG_BUFFER_QUEUE
            DE_DEBUG("Wrote REST ", writtenFrames )
            if ( writtenFrames != m_WorkBuffer.getFrameCount() )
            {
               DE_ERROR("Wrote ",writtenFrames," frames of ", m_WorkBuffer.getFrameCount())
            }
#endif
            desiredFrames -= writtenFrames;
            totalFrames += writtenFrames;
#ifdef DE_DEBUG_BUFFER_QUEUE
            DE_DEBUG("it[", i, "] m_WorkBuffer = ", m_WorkBuffer.toString() )
            DE_DEBUG("it[", i, "] writtenFrames = ", writtenFrames )
            DE_DEBUG("it[", i, "] desiredFrames = ", desiredFrames )
            DE_DEBUG("it[", i, "] totalFrames = ", totalFrames )
#endif
         }
         // Read total chunk
         else
         {
            uint64_t writtenFrames = dst.write( totalFrames, *chunk.get() );
#ifdef DE_DEBUG_BUFFER_QUEUE
            DE_DEBUG("Wrote FULL ", writtenFrames )
            if ( writtenFrames != chunk->getFrameCount() )
            {
               DE_ERROR("Wrote ",writtenFrames," frames of ", chunk->getFrameCount())
            }
#endif
            desiredFrames -= writtenFrames;
            totalFrames += writtenFrames;
#ifdef DE_DEBUG_BUFFER_QUEUE
            DE_DEBUG("FULL CHUNK:" )
            DE_DEBUG("it[", i, "] writtenFrames = ", writtenFrames )
            DE_DEBUG("it[", i, "] desiredFrames = ", desiredFrames )
            DE_DEBUG("it[", i, "] totalFrames = ", totalFrames )
#endif
         }
         m_FrameIndex += chunk->getFrameCount();
         m_FrameOffset = 0;
         m_ChunkIndex++;
         m_Queue.pop_front();
      }
      // [1-B] Need to process only a part of this chunk -> we reached the last chunk to process.
      else // if ( desiredFrames < availFrames )
      {
#ifdef DE_DEBUG_BUFFER_QUEUE
         DE_DEBUG( "desiredFrames(", desiredFrames,") < availFrames(",availFrames,")" )
         // Read rest of chunk
         if ( m_FrameOffset > 0 )
         {
            DE_ERROR( "Unexpected m_FrameOffset(",m_FrameOffset,") > 0 for last chunk" )
            //break;
         }
#endif
         // Read last needed chunk ( can be in between bigger queue chunk )
         if ( desiredFrames > 0 )
         {
#ifdef DE_DEBUG_BUFFER_QUEUE
            DE_DEBUG("END CHUNK desiredFrames(",desiredFrames,")" )
#endif
            chunk->sub( m_WorkBuffer, 0, desiredFrames );
            if ( m_WorkBuffer.getFrameCount() != desiredFrames )
            {
               DE_ERROR("Differ desiredFrames(",desiredFrames,") != ", m_WorkBuffer.getFrameCount())
            }
            uint64_t writtenFrames = dst.write( totalFrames, m_WorkBuffer );
#ifdef DE_DEBUG_BUFFER_QUEUE
            DE_DEBUG("END CHUNK writtenFrames(", writtenFrames, "), desiredFrames(",desiredFrames,")" )
            if ( writtenFrames != m_WorkBuffer.getFrameCount() )
            {
               DE_ERROR("Wrote ",writtenFrames," frames of ", m_WorkBuffer.getFrameCount())
            }
#endif
            desiredFrames -= writtenFrames;
            totalFrames += writtenFrames;
            m_FrameOffset += writtenFrames;
#ifdef DE_DEBUG_BUFFER_QUEUE
            DE_DEBUG("it[", i, "] m_WorkBuffer = ", m_WorkBuffer.toString() )
            DE_DEBUG("it[", i, "] writtenFrames = ", writtenFrames )
            DE_DEBUG("it[", i, "] desiredFrames = ", desiredFrames )
            DE_DEBUG("it[", i, "] totalFrames = ", totalFrames )
#endif
            i++;
         }

      }
      i++;
   }

#ifdef DE_DEBUG_BUFFER_QUEUE
   DE_DEBUG("totalFrames = ", totalFrames )
#endif
   return totalFrames;
}



// ---------------------------------------------------------
//    m_FrameCount = 0, m_FrameIndex = 0;
// push_back( AAAAAABBBBBB )
//    [AAAAAABBBBBB], m_FrameCount = 12, m_FrameIndex = 0;
// pop_front( KKKKKKKK )
//    [BBBB], m_FrameCount = 12, m_FrameIndex = 8;
// push_back( CCCCCC )
//    [BBBBCCCCCC], m_FrameCount = 18, m_FrameIndex = 8;
// pop_front( KKKKKKKK )
//    [CC], m_FrameCount = 18, m_FrameIndex = 16;
// push_back( DDDDDD )
//    [CCDDDDDD], m_FrameCount = 24, m_FrameIndex = 16;
// pop_front( KKKKKKKK )
//    [], m_FrameCount = 24, m_FrameIndex = 24;
// ---------------------------------------------------------

#ifndef CPP_EQUAL
#define CPP_EQUAL(x,expected) \
if ((x)!=(expected)) { \
   std::ostringstream s; \
   s << "Failed expression: " #x " == " #expected "\n"; \
   s << "Failed line:" << __LINE__ << "\n"; \
   s << "Expected:" << #expected << "\n"; \
   s << "Got:" << (x) << ""; \
   throw std::runtime_error( s.str() ); \
}
#endif

void
SampleBufferQueueTest::test()
{
   DE_DEBUG("TEST BEGIN:" )

   SampleBufferQueue Q; // testObj

   DE_WARN("=======================================")
   DE_WARN("[pushA] testObj" )
   DE_WARN("=======================================")
   SampleBuffer::Ptr A = SampleBuffer::createPtr( ST_S16I, 2, 44100, 3 );
   A->setSampleS16( 0,0, 1 );
   A->setSampleS16( 0,1, 2 );
   A->setSampleS16( 1,0, 3 );
   A->setSampleS16( 1,1, 4 );
   A->setSampleS16( 2,0, 5 );
   A->setSampleS16( 2,1, 6 );
   Q.push_back( std::move( A ), 0.0 );
   Q.dump();

   CPP_EQUAL( Q.getChunkIndex(), 0 )
   CPP_EQUAL( Q.getChunkOffset(), 0 )
   CPP_EQUAL( Q.getFrameCount(), 3 )
   CPP_EQUAL( Q.getFrameIndex(), 0 )

   DE_WARN("=======================================")
   DE_WARN("[pushB] testObj" )
   DE_WARN("=======================================")
   SampleBuffer::Ptr B = SampleBuffer::createPtr( ST_S16I, 2, 44100, 3 );
   B->setSampleS16( 0,0, 7 );
   B->setSampleS16( 0,1, 8 );
   B->setSampleS16( 1,0, 9 );
   B->setSampleS16( 1,1, 10 );
   B->setSampleS16( 2,0, 11 );
   B->setSampleS16( 2,1, 12 );
   Q.push_back( std::move( B ), 0.3 );
   Q.dump();

   CPP_EQUAL( Q.getChunkIndex(), 0 )
   CPP_EQUAL( Q.getChunkOffset(), 0 )
   CPP_EQUAL( Q.getFrameCount(), 6 )
   CPP_EQUAL( Q.getFrameIndex(), 0 )

   DE_WARN("=======================================")
   DE_WARN("[popC] testObj" )
   DE_WARN("=======================================")
   SampleBuffer::Ptr C = SampleBuffer::createPtr( ST_S16I, 2, 44100, 4 );
   Q.pop_front( *C, 0.0 );
   Q.dump();

   CPP_EQUAL( Q.getChunkIndex(), 1 )
   CPP_EQUAL( Q.getChunkOffset(), 1 )
   CPP_EQUAL( Q.getFrameCount(), 6 )
   CPP_EQUAL( Q.getFrameIndex(), 3 )

   DE_WARN("=======================================")
   DE_WARN("[pushD] testObj" )
   DE_WARN("=======================================")
   SampleBuffer::Ptr D = SampleBuffer::createPtr( ST_S16I, 2, 44100, 1 );
   D->setSampleS16( 0,0, 13 );
   D->setSampleS16( 0,1, 14 );
   Q.push_back( std::move( D ), 0.4 );
   Q.dump();

   CPP_EQUAL( Q.getChunkIndex(), 1 )
   CPP_EQUAL( Q.getChunkOffset(), 1 )
   CPP_EQUAL( Q.getFrameCount(), 7 )
   CPP_EQUAL( Q.getFrameIndex(), 3 )

   DE_WARN("=======================================")
   DE_WARN("[popE] testObj" )
   DE_WARN("=======================================")
   SampleBuffer::Ptr E = SampleBuffer::createPtr( ST_S16I, 2, 44100, 3 );
   Q.pop_front( *E, 0.0 );
   Q.dump();

   CPP_EQUAL( Q.getChunkIndex(), 3 )
   CPP_EQUAL( Q.getChunkOffset(), 0 )
   CPP_EQUAL( Q.getFrameCount(), 7 )
   CPP_EQUAL( Q.getFrameIndex(), 7 )

   CPP_EQUAL( Q.getChunkIndex(), 3 )
   CPP_EQUAL( Q.getChunkOffset(), 0 )
   CPP_EQUAL( Q.getFrameCount(), 7 )
   CPP_EQUAL( Q.getFrameIndex(), 7 )

   DE_DEBUG("TEST END:" )
}


} // end namespace audio
} // end namespace de
