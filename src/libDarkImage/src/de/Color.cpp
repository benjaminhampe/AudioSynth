#include <de/Color.hpp>

namespace de {

// static
uint32_t
ColorFormat::getChannelCount( EColorFormat colorFormat )
{
   switch( colorFormat )
   {
      case RGBA8888: return 4;
      case RGB888: return 3;
      case ARGB1555: return 4;
      case RGB565: return 3;
      case R32F: return 1;
      case R8: return 1;
      case R16: return 1;
      case R24: return 1;
      case R32: return 1;
      default: return 0;
   }
}

// static
uint32_t
ColorFormat::getBitsPerChannel( int channel, EColorFormat colorFormat )
{
   switch( colorFormat )
   {
      case RGBA8888: return 8;
      case RGB888:   return 8;
      case ARGB1555: return (channel == 0) ? 1 : 5;
      case RGB565:   return (channel == 1) ? 6 : 5;
      case R32F:     return 32;
      case R8:       return 8;
      case R16:      return 16;
      case R24:      return 24;
      case R32:      return 32;
      default:       return 0;
   }
}

// static
uint32_t
ColorFormat::getMaxBitsPerChannel( EColorFormat colorFormat )
{
   switch( colorFormat )
   {
      case RGBA8888: return 8;
      case RGB888:   return 8;
      case ARGB1555: return 5;
      case RGB565:   return 6;
      case R32F:     return 32;
      case R8:       return 8;
      case R16:      return 16;
      case R24:      return 24;
      case R32:      return 32;
      default:       return 0;
   }
}

// static
uint32_t
ColorFormat::getBitsPerPixel( EColorFormat colorFormat )
{
   switch( colorFormat )
   {
      case RGBA8888: return 32;
      case RGB888:   return 24;
      case ARGB1555: return 16;
      case RGB565:   return 16;
      case R32F:     return 32;
      case R8:       return 8;
      case R16:      return 16;
      case R24:      return 24;
      case R32:      return 32;
      default:       return 0;
   }
}

// static
uint32_t
ColorFormat::getBytesPerPixel( EColorFormat colorFormat )
{
   switch( colorFormat )
   {
      case RGBA8888: return 4;
      case RGB888:   return 3;
      case ARGB1555: return 2;
      case RGB565:   return 2;
      case R32F:     return 4;
      case R8:       return 1;
      case R16:      return 2;
      case R24:      return 3;
      case R32:      return 4;
      default:       return 0;
   }
}

// static
std::string
ColorFormat::getString( EColorFormat colorFormat )
{
   switch( colorFormat )
   {
      case RGBA8888: return "RGBA8888";
      case RGB888: return "RGB888";
      case ARGB1555: return "ARGB1555";
      case RGB565: return "RGB565";
      case R32F: return "R32F";
      case R8: return "R8";
      case R16: return "R16";
      case R24: return "R24";
      case R32: return "R32";
      case BGRA8888: return "BGRA8888";
      case BGR888: return "BGR888";
      default: return "Unknown";
   }
}

// static
std::string
RGBA_toHexString( uint32_t color )
{
   return dbStrHex( color );
}
// ===================================================================
std::string
RGBA_toString( uint32_t color )
{
   std::stringstream s;
   s << int( RGBA_R( color ) ) << ","
     << int( RGBA_G( color ) ) << ","
     << int( RGBA_B( color ) ) << ","
     << int( RGBA_A( color ) );
   return s.str();
}
// ===================================================================
std::string
RGBA_toCSS( uint32_t color )
{
   std::stringstream s;
   s << "rgba("
     << int( RGBA_R( color ) ) << ","
     << int( RGBA_G( color ) ) << ","
     << int( RGBA_B( color ) ) << ","
     << int( RGBA_A( color ) ) << ")";
   return s.str();
}

// ===================================================================
uint8_t
clampByte( int v )
{
   return std::max( 0, std::min( v, 255 ) );
}

// ===================================================================
uint32_t
RGBA( uint8_t r, uint8_t g, uint8_t b, uint8_t a )
{
   return ( uint32_t( a ) << 24 )
        | ( uint32_t( b ) << 16 )
        | ( uint32_t( g ) << 8 )
        | uint32_t( r );
}
// ===================================================================
uint8_t
RGBA_R( uint32_t color ) { return color & 0x000000ff; }
uint8_t
RGBA_G( uint32_t color ) { return ( color >> 8 ) & 0x000000ff; }
uint8_t
RGBA_B( uint32_t color ) { return ( color >> 16 ) & 0x000000ff; }
uint8_t
RGBA_A( uint32_t color ) { return ( color >> 24 ) & 0x000000ff; }
// ===================================================================
int
RGBA_Ri( uint32_t color ) { return color & 0x000000ff; }
int
RGBA_Gi( uint32_t color ) { return ( color >> 8 ) & 0x000000ff; }
int
RGBA_Bi( uint32_t color ) { return ( color >> 16 ) & 0x000000ff; }
int
RGBA_Ai( uint32_t color ) { return ( color >> 24 ) & 0x000000ff; }
// ===================================================================
uint32_t RGBA_A( uint32_t color, uint8_t alpha ) { return RGBA( RGBA_R( color ), RGBA_G( color ), RGBA_B( color ), alpha ); }
uint32_t RGBA_B( uint32_t color, uint8_t blue ) { return RGBA( RGBA_R( color ), RGBA_G( color ), blue, RGBA_A( color ) ); }
uint32_t RGBA_G( uint32_t color, uint8_t green ) { return RGBA( RGBA_R( color ), green, RGBA_B( color ), RGBA_A( color ) ); }
uint32_t RGBA_R( uint32_t color, uint8_t red ) { return RGBA( red, RGBA_G( color ), RGBA_B( color ), RGBA_A( color ) ); }
// ===================================================================
uint32_t RGBA( glm::vec4 const & color128 )
{
   return RGBA( Math::float2byte( color128.r ),
                Math::float2byte( color128.g ),
                Math::float2byte( color128.b ),
                Math::float2byte( color128.a ) );
}
// ===================================================================
void RGBA_setA( uint32_t & color, int alpha )
{
   // new = RGBA with deleted alpha  +  new alpha
   color = ( color & 0x00FFFFFF ) | (( uint32_t( alpha ) & 0xFF ) << 24);
}
// ===================================================================
float RGBA_Rf( uint32_t color ) { return Math::byte2float( RGBA_R( color ) ); }
float RGBA_Gf( uint32_t color ) { return Math::byte2float( RGBA_G( color ) ); }
float RGBA_Bf( uint32_t color ) { return Math::byte2float( RGBA_B( color ) ); }
float RGBA_Af( uint32_t color ) { return Math::byte2float( RGBA_A( color ) ); }
// ===================================================================
glm::vec4 RGBAf( uint32_t color32 )
{
   return glm::vec4( Math::byte2float( RGBA_R( color32 ) ),
                     Math::byte2float( RGBA_G( color32 ) ),
                     Math::byte2float( RGBA_B( color32 ) ),
                     Math::byte2float( RGBA_A( color32 ) ) );
}
// ===================================================================
uint32_t lerpColor( uint32_t bg, uint32_t fg, float t )
{
//   float const s = RGBA_Af( bg );
//   float const d = RGBA_Af( fg );
//   int32_t r = Math::round32( 255.0f * ( RGBA_Rf( bg ) + d * RGBA_Rf( fg ) ) );
//   int32_t g = Math::round32( 255.0f * ( RGBA_Gf( bg ) + d * RGBA_Gf( fg ) ) );
//   int32_t b = Math::round32( 255.0f * ( RGBA_Bf( bg ) + d * RGBA_Bf( fg ) ) );
//   int32_t a = Math::round32( 255.0f * ( RGBA_Af( bg ) + d * RGBA_Af( fg ) ) );
//   r = Math::clampi( r, 0, 255 );
//   g = Math::clampi( g, 0, 255 );
//   b = Math::clampi( b, 0, 255 );
//   a = Math::clampi( a, 0, 255 );
   int32_t r = RGBA_R( bg );
   int32_t g = RGBA_G( bg );
   int32_t b = RGBA_B( bg );
   int32_t a = RGBA_A( bg );
   int32_t dr = int( RGBA_R( fg ) ) - r;
   int32_t dg = int( RGBA_G( fg ) ) - g;
   int32_t db = int( RGBA_B( fg ) ) - b;
   int32_t da = int( RGBA_A( fg ) ) - a;

   r = Math::clampi( Math::round32( t * dr + float( r ) ), 0, 255 );
   g = Math::clampi( Math::round32( t * dg + float( g ) ), 0, 255 );
   b = Math::clampi( Math::round32( t * db + float( b ) ), 0, 255 );
   a = Math::clampi( Math::round32( t * da + float( a ) ), 0, 255 );
   return RGBA( r, g, b, a );
}

// ===================================================================
uint32_t
blendColor( uint32_t bg, uint32_t fg )
{
   // blend(0x10000000,0x10FFFFFF) = 0x207F7F7F
   int32_t r1 = RGBA_R( bg );
   int32_t g1 = RGBA_G( bg );
   int32_t b1 = RGBA_B( bg );
   int32_t a1 = RGBA_A( bg );
   int32_t r2 = RGBA_R( fg );
   int32_t g2 = RGBA_G( fg );
   int32_t b2 = RGBA_B( fg );
   int32_t a2 = RGBA_A( fg );
   float v = RGBA_Af( fg );
   float u = 1.0f - v; //RGBA_Af( fg );
//   int32_t a_sum = a1 + a2;
//   float u = float( a1 ) / float( a_sum );
//   float v = float( a2 ) / float( a_sum );

//   int32_t r = Math::clampi( Math::round32( v * float(r1) + u * (r2-r1 ) ), 0, 255 );
//   int32_t g = Math::clampi( Math::round32( v * float(g1) + u * (g2-g1 ) ), 0, 255 );
//   int32_t b = Math::clampi( Math::round32( v * float(b1) + u * (b2-b1 ) ), 0, 255 );
//   int32_t a = Math::clampi( Math::round32( v * float(a1) + u * (a2-a1 ) ), 0, 255 );

   int32_t r = Math::clampi( Math::round32( r1 + v * (r2-r1) ), 0, 255 );
   int32_t g = Math::clampi( Math::round32( g1 + v * (g2-g1) ), 0, 255 );
   int32_t b = Math::clampi( Math::round32( b1 + v * (b2-b1) ), 0, 255 );
   int32_t a = Math::clampi( a1 + a2, 0, 255 );
   return RGBA( r, g, b, a );

   // Final.rgb = fg.rgb * ( fg.a ) + ( 1-f.a) * bg.rgb
//   float const u = RGBA_Af( fg );
//   float const v = 1.0f - u;
//   float rr = u * RGBA_Rf( fg ) + v * RGBA_Rf( bg );
//   float gg = u * RGBA_Gf( fg ) + v * RGBA_Gf( bg );
//   float bb = u * RGBA_Bf( fg ) + v * RGBA_Bf( bg );
//   float aa = u * RGBA_Af( fg ) + v * RGBA_Af( bg );
//   int32_t r = Math::clampi( Math::round32( 255.0f * rr ), 0, 255 );
//   int32_t g = Math::clampi( Math::round32( 255.0f * gg ), 0, 255 );
//   int32_t b = Math::clampi( Math::round32( 255.0f * bb ), 0, 255 );
//   //int32_t a = Math::clampi( Math::round32( 255.0f * aa ), 0, 255 );
//   return RGBA( r, g, b, 255 );
//   float const t = RGBA_Af( fg );
//   return lerpColor( bg, fg, t );
}
// ===================================================================
uint32_t
parseColor( std::string const & line )
{
   if ( line.size() < 6 )
   {
      return 0; // none = transparent
   }

   if ( line[0] == '#' )
   {
      int r = 0, g = 0, b = 0;
      sscanf( line.c_str(), "#%02x%02x%02x)", &r, &g, &b );
      return RGBA( r & 0xFF, g & 0xFF, b & 0xFF );
   }
   else if ( line[0] == 'r' &&
             line[1] == 'g' &&
             line[2] == 'b' )
   {
      if ( line[3] == 'a' )
      {
         int r = 0, g = 0, b = 0, a = 0;
         sscanf( line.c_str(), "rgba(%i,%i,%i,%i)", &r, &g, &b, &a );
         return RGBA( r & 0xFF, g & 0xFF, b & 0xFF, a & 0xFF );
      }
      else
      {
         int r = 0, g = 0, b = 0;
         sscanf( line.c_str(), "rgb(%i,%i,%i)", &r, &g, &b );
         return RGBA( r & 0xFF, g & 0xFF, b & 0xFF );
      }
   }
   else
   {
      return 0;
   }
}

uint8_t
randomByte()
{
   return uint8_t( rand() % 255 );
}

// ===================================================================
uint32_t randomRGBA( uint8_t alpha )
{
   return RGBA( randomByte(), randomByte(), randomByte(), alpha );
}

// ===================================================================
uint32_t randomRGB( int minVal )
{
   if ( minVal > -1 )
   {
      minVal = std::clamp( minVal, 0, 128 );
      std::clamp( minVal, 0, 128 );
      return RGBA(
            std::clamp( minVal + Math::randomByte(), 0, 255 ),
            std::clamp( minVal + Math::randomByte(), 0, 255 ),
            std::clamp( minVal + Math::randomByte(), 0, 255 ),
            255 );
   }
   else
   {
      return RGBA(
               Math::randomByte(),
               Math::randomByte(),
               Math::randomByte(),
               255 );
   }
}
glm::vec4 randomRGBf()
{
   return glm::vec4(
               Math::randomFloat(),
               Math::randomFloat(),
               Math::randomFloat(), 1.0f );
}
// ===================================================================
uint32_t randomColor( int minVal )
{
   return randomRGB( minVal );
}
uint32_t randomColorInRange( uint8_t range_min, uint8_t range_max, uint8_t a )
{
   return RGBA( Math::randomByteInRange( range_min, range_max ),
                Math::randomByteInRange( range_min, range_max ),
                Math::randomByteInRange( range_min, range_max ),
                a );
}
// ===================================================================
uint32_t lightenColor( uint32_t color, int offset )
{
   int32_t r = RGBA_R( color );
   int32_t g = RGBA_G( color );
   int32_t b = RGBA_B( color );
   int32_t const a = RGBA_A( color );
   r = Math::clampT< int >( r + offset, 0, 255 );
   g = Math::clampT< int >( g + offset, 0, 255 );
   b = Math::clampT< int >( b + offset, 0, 255 );
   return RGBA( r, g, b, a );
}
// ===================================================================
uint32_t
darkenColor( uint32_t color, int offset )
{
   int32_t r = RGBA_R( color );
   int32_t g = RGBA_G( color );
   int32_t b = RGBA_B( color );
   int32_t const a = RGBA_A( color );
   r = Math::clampT< int >( r - offset, 0, 255 );
   g = Math::clampT< int >( g - offset, 0, 255 );
   b = Math::clampT< int >( b - offset, 0, 255 );
   return RGBA( r, g, b, a );
}
// ===================================================================
uint32_t
varyColor( uint32_t color, int variance )
{
   int32_t r = RGBA_R( color );
   int32_t g = RGBA_G( color );
   int32_t b = RGBA_B( color );
   int32_t const a = RGBA_A( color );
   r = Math::clampT< int >( r - variance / 2 + ( rand() % variance ), 0, 255 );
   g = Math::clampT< int >( g - variance / 2 + ( rand() % variance ), 0, 255 );
   b = Math::clampT< int >( b - variance / 2 + ( rand() % variance ), 0, 255 );
   return RGBA( r, g, b, a );
}

#if 0

uint32_t
toOpenGLColor( uint32_t color ) { return RGBAtoABGR( color ); }

// ============================================================================
static QColor blendColors( const QColor& src, const QColor& dst )
// ============================================================================
{
   const float s = (float)src.alpha() / 255.0f;
   const float d = (float)dst.alpha() / 255.0f;

   const int r = math::min32( 255, math::round32( s * src.red() + d * dst.red() ) );
   const int g = math::min32( 255, math::round32( s * src.green() + d * dst.green() ) );
   const int b = math::min32( 255, math::round32( s * src.blue() + d * dst.blue() ) );
   const int a = math::min32( 255, math::round32( s * src.alpha() + d * dst.alpha() ) );

   return QColor( r,g,b,a );
}

#endif


// static
glm::vec4
RainbowColor::computeFromWaveLength( float64_t lambda, float64_t gamma, float alpha )
{
   glm::dvec3 c;

   // setze limits ? ? [380, 780]
   // if (lambda < 380.0) lambda = 380.0;
   // if (lambda > 780.0) lambda = 780.0;
   lambda = glm::clamp( lambda, 380.0, 780.0 );

   // Jeder Farbkanal hat seine eigene dem (sichtbaren) Sonnenspektrum angelehnt interpolierte Kurve:
   if ((lambda >= 380.0) && (lambda < 440.0))
   {
      c.r = (440.0-lambda)/(440.0-380.0);
      c.g = 0.0;
      c.b = 1.0;
   }
   else if ((lambda >= 440.0) && (lambda < 490.0))
   { c.r = 0.0; c.g = (lambda-440.0)/(490.0-440.0); c.b = 1.0; }
   else if ((lambda >= 490.0) && (lambda < 510.0))
   { c.r = 0.0; c.g = 1.0; c.b = (510.0-lambda)/(510.0-490.0); }
   else if ((lambda >= 510.0) && (lambda < 580.0))
   { c.r = (lambda-510.0)/(580.0-510.0); c.g = 1.0; c.b = 0.0; }
   else if ((lambda >= 580.0) && (lambda < 645.0))
   { c.r = 1.0; c.g = (645.0-lambda)/(645.0-580.0); c.b = 0.0; }
   else /* if ((lambda >= 645.0) && (lambda <= 780.0)) */
   { c.r = 1.0; c.g = 0.0; c.b = 0.0; }

   // reduce intesity at the borders
   float64_t f;
   if ((lambda >= 380.0) && (lambda < 420.0))
   { f = 0.3 + 0.7*(lambda-380.0)/(420.0-380.0); }
   else if ((lambda >= 420.0) && (lambda <= 700.0))
   { f = 1.0; }
   else /* if ((lambda > 700.0) && (lambda <= 780.0)) */
   { f = 0.3 + 0.7*(780.0-lambda)/(780.0-700.0); }

   // eigentliche Korrektur
   if ( f != 1.0 ) { c *= f; }

   // Gamma Korrektur
   if (gamma!=1.0)
   {
      c.r = ::pow( c.r, gamma );
      c.g = ::pow( c.g, gamma );
      c.b = ::pow( c.b, gamma );
   }

   return glm::clamp(glm::vec4( glm::vec3( c ), alpha ),
                     glm::vec4( 0.0f, 0.0f, 0.0f, 0.0f ),
                     glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );
}

// static
glm::vec4
RainbowColor::computeColor128( float t, float gamma, float alpha )
{
   return computeFromWaveLength( 360.0f + 400.0f * glm::clamp( t, 0.0f, 1.0f ), gamma, alpha );
}


// static
uint32_t
RainbowColor::computeColor32( float t, float gamma, float alpha )
{
   glm::vec4 const c = computeColor128( t, gamma, alpha );
   int32_t r = glm::clamp( int32_t( c.r * 255.0f ), 0, 255 );
   int32_t g = glm::clamp( int32_t( c.g * 255.0f ), 0, 255 );
   int32_t b = glm::clamp( int32_t( c.b * 255.0f ), 0, 255 );
   //int32_t a = glm::clamp( int32_t( c.a * 255.0f ), 0, 255 );
   return RGBA( r, g, b, 255 );
}


// static
void
ColorConverter::convert_RGB888toRGB888( void const * src, void* dst, size_t n )
{
   ::memcpy( dst, src, n * 3 );
}

// static
void
ColorConverter::convert_ARGB8888toRGB888( void const * src, void* dst, size_t n )
{
   uint8_t* s = (uint8_t*)src;
   uint8_t* d = (uint8_t*)dst;
   for ( size_t i = 0; i < n; ++i )
   {
      d[0] = s[0];
      d[1] = s[1];
      d[2] = s[2];
      s += 4; // source[3] is alpha, jumps over A.
      d += 3;
   }
}

// static
void
ColorConverter::convert_RGB565toRGB888( void const * src, void* dst, size_t n )
{
   uint16_t* s = (uint16_t*)src;
   uint8_t * d = (uint8_t *)dst;

   for ( size_t i = 0; i < n; ++i )
   {
      d[2] = (*s & 0xf800) >> 8;
      d[1] = (*s & 0x07e0) >> 3;
      d[0] = (*s & 0x001f) << 3;
      s += 1; // += 2 Byte
      d += 3; // += 3 Byte
   }
}

// static
void
ColorConverter::convert_ARGB1555toRGB888( void const * src, void* dst, size_t n )
{
   uint16_t* s = (uint16_t*)src;
   uint8_t*  d = (uint8_t*)dst;

   for ( size_t i = 0; i < n; ++i )
   {
      d[2] = (*s & 0x7c00) >> 7;
      d[1] = (*s & 0x03e0) >> 2;
      d[0] = (*s & 0x1f) << 3;
      s += 1;
      d += 3;
   }
}

// static
void
ColorConverter::convert_ARGB8888toARGB8888( void const * src, void* dst, size_t n )
{
   ::memcpy( dst, src, n * 4 );
}

// static
uint32_t
ColorConverter::ARGB1555toARGB8888( uint16_t color )
{
   return (( -( (int32_t) color & 0x00008000 ) >> (int32_t) 31 ) & 0xFF000000 )
        | (( color & 0x00007C00 ) << 9) | (( color & 0x00007000 ) << 4)
        | (( color & 0x000003E0 ) << 6) | (( color & 0x00000380 ) << 1)
        | (( color & 0x0000001F ) << 3) | (( color & 0x0000001C ) >> 2);
}

// static
void
ColorConverter::convert_ARGB1555toARGB8888( void const * src, void* dst, size_t n )
{
   uint16_t* s = (uint16_t*)src;
   uint32_t* d = (uint32_t*)dst;

   for ( size_t i = 0; i < n; ++i )
   {
      *d++ = ARGB1555toARGB8888(*s++);
   }
}

// static
void
ColorConverter::convert_ARGB1555toARGB1555( void const * src, void* dst, size_t n )
{
   ::memcpy( dst, src, n * 2 );
}

// static
void
ColorConverter::convert_RGB888toARGB8888( void const * src, void* dst, size_t n )
{
   uint8_t* s = (uint8_t*)src;
   uint8_t* d = (uint8_t*)dst;

   for ( size_t i = 0; i < n; ++i )
   {
      d[0] = s[0];
      d[1] = s[1];
      d[2] = s[2];
      d[3] = 255;
      s += 3;
      d += 4;
   }
}

// static
uint16_t
ColorConverter::RGB565toARGB1555( uint16_t color ) // Returns A1R5G5B5 Color from R5G6B5 color
{
   return 0x8000 | ( ( ( color & 0xFFC0 ) >> 1 ) | ( color & 0x1F ) );
}

// Used in: ImageWriterTGA

// static
void
ColorConverter::convert_RGB565toARGB1555( void const * src, void* dst, size_t n )
{
   uint16_t* s = (uint16_t*)src;
   uint16_t* d = (uint16_t*)dst;

   for ( size_t i = 0; i < n; ++i )
   {
      *d++ = RGB565toARGB1555( *s++ );
   }
}

// Used in: ImageWriterBMP

// static
void
ColorConverter::convert24BitTo24Bit( uint8_t const * in, uint8_t* out, int32_t width, int32_t height, int32_t linepad, bool flip, bool bgr )
{
   if ( !in || !out ) return;
   int32_t const lineWidth = 3 * width;
   if ( flip )
   {
      out += lineWidth * height;
   }

   for ( int32_t y = 0; y < height; ++y )
   {
      if ( flip )
      {
         out -= lineWidth;
      }
      if ( bgr )
      {
         for ( int32_t x = 0; x < lineWidth; x += 3 )
         {
            out[ x+0 ] = in[ x+2 ];
            out[ x+1 ] = in[ x+1 ];
            out[ x+2 ] = in[ x+0 ];
         }
      }
      else
      {
         ::memcpy( out, in, lineWidth );
      }
      if ( !flip )
      {
         out += lineWidth;
      }
      in += lineWidth;
      in += linepad;
   }
}

// Used in: ImageWriterBMP

// static
void
ColorConverter::convert_ARGB8888toBGR888( void const * src, void* dst, size_t n )
{
   uint8_t const* s = (uint8_t const*)src;
   uint8_t* d = (uint8_t*)dst;

   for ( size_t i = 0; i < n; ++i )
   {
      d[0] = s[2];
      d[1] = s[1];
      d[2] = s[0];
      s += 4; // s[3] is alpha and skipped, jumped over.
      d += 3;
   }
}

// static
void
ColorConverter::convert_RGB888toBGR888( void const * src, void* dst, size_t n )
{
   uint8_t const* s = (uint8_t const*)src;
   uint8_t* d = (uint8_t*)dst;

   for ( size_t i = 0; i < n; ++i )
   {
      d[0] = s[2];
      d[1] = s[1];
      d[2] = s[0];
      s += 3;
      d += 3;
   }
}


// static
void
ColorConverter::convert_BGR888toRGB888( void const * src, void* dst, size_t n )
{
   convert_RGB888toBGR888( src, dst, n );
}

// static
void
ColorConverter::convert_ARGB1555toBGR888( void const * src, void* dst, size_t n )
{
   uint16_t const* s = (uint16_t const*)src;
   uint8_t* d = (uint8_t*)dst;

   for ( size_t i = 0; i < n; ++i )
   {
      d[0] = (*s & 0x7c00) >> 7;
      d[1] = (*s & 0x03e0) >> 2;
      d[2] = (*s & 0x1f) << 3;
      s += 1;
      d += 3;
   }
}

// static
void
ColorConverter::convert_RGB565toBGR888( void const * src, void* dst, size_t n )
{
   uint16_t const* s = (uint16_t const*)src;
   uint8_t* d = (uint8_t*)dst;
   for ( size_t i = 0; i < n; ++i )
   {
      d[0] = (*s & 0xf800) >> 8;
      d[1] = (*s & 0x07e0) >> 3;
      d[2] = (*s & 0x001f) << 3;
      s += 1;
      d += 3;
   }
}

// static
void
ColorConverter::convert_ARGB8888toABGR8888( void const * src, void* dst, size_t n )
{
   uint8_t const* s = (uint8_t const*)src;
   uint8_t* d = (uint8_t*)dst;
   for ( size_t i = 0; i < n; ++i )
   {
      d[0] = s[2];
      d[1] = s[1];
      d[2] = s[0];
      d[3] = s[3];
      s += 4;
      d += 4;
   }
}

// static
void
ColorConverter::convert_RGB888toABGR8888( void const * src, void* dst, size_t n )
{
   uint8_t const* s = (uint8_t const*)src;
   uint8_t* d = (uint8_t*)dst;
   for ( size_t i = 0; i < n; ++i )
   {
      d[0] = s[2];
      d[1] = s[1];
      d[2] = s[0];
      d[3] = 255;
      s += 3;
      d += 4;
   }
}


// used in bmp format
void
ColorConverter::convert_BGR888toARGB8888( void const * src, void* dst, size_t n )
{
   uint8_t const* s = (uint8_t const*)src;
   uint8_t* d = (uint8_t*)dst;
   for ( size_t i = 0; i < n; ++i )
   {
      d[0] = s[2];   // B <= B
      d[1] = s[1];   // G <= G
      d[2] = s[0];   // R <= R
      d[3] = 255;    // A = 255
      s += 3;
      d += 4;
   }
}

// static
ColorConverter::Converter_t
ColorConverter::getConverter( ColorFormat::EColorFormat const & src, ColorFormat::EColorFormat const & dst )
{
   if ( src == ColorFormat::RGBA8888 )
   {
      if ( dst == ColorFormat::RGBA8888 )
      {
         return convert_ARGB8888toARGB8888;
      }
      else if ( dst == ColorFormat::RGB888 )
      {
         return convert_ARGB8888toRGB888;
      }
      else if ( dst == ColorFormat::ARGB1555 )
      {
         throw std::runtime_error( "Not implemented convert_ARGB8888toARGB1555" );
         //return convert_ARGB8888toARGB1555;
         return nullptr;

      }
      else if ( dst == ColorFormat::RGB565 )
      {
         throw std::runtime_error( "Not implemented convert_ARGB8888toRGB565" );
         //return convert_ARGB8888toRGB565;
         return nullptr;
      }
      else if ( dst == ColorFormat::BGR888 )
      {
         return convert_ARGB8888toBGR888;
      }
   }
   else if ( src == ColorFormat::RGB888 )
   {
      if ( dst == ColorFormat::RGBA8888 )
      {
         return convert_RGB888toARGB8888;
      }
      else if ( dst == ColorFormat::RGB888 )
      {
         return convert_RGB888toRGB888;
      }
      else if ( dst == ColorFormat::ARGB1555 )
      {
         throw std::runtime_error( "Not implemented convert_RGB888toARGB1555" );
         //return convert_RGB888toARGB1555;
         return nullptr;
      }
      else if ( dst == ColorFormat::RGB565 )
      {
         throw std::runtime_error( "Not implemented convert_RGB888toRGB565" );
         //return convert_RGB888toRGB565;
         return nullptr;
      }
      else if ( dst == ColorFormat::BGR888 )
      {
         return convert_RGB888toBGR888;
      }
   }
   else if ( src == ColorFormat::ARGB1555 )
   {
      if ( dst == ColorFormat::RGBA8888 )
      {
         return convert_ARGB1555toARGB8888;
      }
      else if ( dst == ColorFormat::RGB888 )
      {
         return convert_ARGB1555toRGB888;
      }
      else if ( dst == ColorFormat::ARGB1555 )
      {
         return convert_ARGB1555toARGB1555;
      }
      else if ( dst == ColorFormat::RGB565 )
      {
         throw std::runtime_error( "Not implemented convert_ARGB1555toRGB565" );
         //return convert_ARGB1555toRGB565;
         return nullptr;
      }
      else if ( dst == ColorFormat::BGR888 )
      {
         return convert_ARGB1555toBGR888;
      }
   }
   else if ( src == ColorFormat::RGB565 )
   {
      if ( dst == ColorFormat::RGBA8888 )
      {
         throw std::runtime_error( "Not implemented convert_RGB565toARGB8888" );
         // return convert_RGB565toARGB8888;
         return nullptr;
      }
      else if ( dst == ColorFormat::RGB888 )
      {
         return convert_RGB565toRGB888;
      }
      else if ( dst == ColorFormat::ARGB1555 )
      {
         return convert_RGB565toARGB1555;
      }
      else if ( dst == ColorFormat::RGB565 )
      {
         throw std::runtime_error( "Not implemented convert_RGB565toRGB565" );
         //return convert_RGB565toRGB565;
         return nullptr;
      }
      else if ( dst == ColorFormat::BGR888 )
      {
         return convert_RGB565toBGR888;
      }
   }
   else if ( src == ColorFormat::BGR888 )
   {
      if ( dst == ColorFormat::RGBA8888 )
      {
         return convert_BGR888toARGB8888;
      }
      else if ( dst == ColorFormat::RGB888 )
      {
         return convert_BGR888toRGB888;
      }
      else
      {
         throw std::runtime_error( "Not implemented convert_BGR888toFancy" );
      }
   }

   {
      std::stringstream msg; msg << "Unknown color converter "
         "from (" << ColorFormat::getString( src ) << ") "
         "to (" << ColorFormat::getString( dst ) << ")";
      throw std::runtime_error( msg.str() );
   }
   return nullptr;
}

} // end namespace de.
