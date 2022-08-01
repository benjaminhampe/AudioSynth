#include <de/audio/dsp/MultiOSC.hpp>

namespace de {
namespace audio {

std::string
MultiOSCf::getFunctionString( int typ )
{
   switch ( typ )
   {
      case Sine: return "Sine";
      case Triangle: return "Triangle";
      case Rect: return "Rect";
      case Ramp: return "Ramp";
      case Saw: return "Saw";
      case Bell: return "Bell";
      case Noise: return "Noise";
      case Organ: return "Organ";
      case Piano: return "Piano";
      case AnalogRect: return "AnalogRect";
      case AnalogSaw: return "AnalogSaw";
      default: return "Unknown";
   }
}

uint32_t
MultiOSCf::getTypeCount() const { return eTypeCount; }

std::string
MultiOSCf::toString() const
{
   std::stringstream s;
   s << "m_isBypassed(" << m_isBypassed << "), ";
   s << "m_type(" << getFunctionString() << "), ";
   s << "m_volume(" << m_volume << "), ";
   s << "m_amp(" << m_amp << "), ";
   s << "m_freq(" << m_freq << "), ";
   s << "m_period(" << m_period << "), ";
   s << "m_phase(" << m_phase << "), ";
   s << "m_pulseWidth(" << m_pulseWidth << "), ";
   s << "m_center(" << m_center << "), ";
   s << "m_range(" << m_range << "), ";
   s << "m_bypassValue(" << m_bypassValue << ")";
   return s.str();
}

MultiOSCf::MultiOSCf()
   : m_isBypassed( false )
   , m_type( Sine )
   , m_volume( 100 )
   , m_freq( T(440) )
   , m_period( T(1000) / m_freq )
   , m_amp( T(1) )
   , m_phase( T(0) )
   , m_pulseWidth( T(0.5) )
   , m_bypassValue( 0.0f )
   , m_center( 0.0f )
   , m_range( 1.0f )
   , m_func( fnSine )
{
   ::srand( uint32_t( time( nullptr ) ) ); // for noise
}

bool MultiOSCf::isBypassed() const { return m_isBypassed; }
void MultiOSCf::setBypassed( bool bypassed ) { m_isBypassed = bypassed; }
void MultiOSCf::setBypassValue( T defaultValue ) { m_bypassValue = defaultValue; }
void MultiOSCf::toggleBypassed() { m_isBypassed = !m_isBypassed; }

int MultiOSCf::getFunction() const { return m_type; }
std::string MultiOSCf::getFunctionString() const { return getFunctionString( m_type ); }

void
MultiOSCf::setFunction( int typ )
{
   if ( typ < 0 || typ >= MultiOSCf::eTypeCount ) return;
   m_type = eType( typ );
   switch ( m_type )
   {
      case Sine: m_func = fnSine; break;
      case Triangle: m_func = fnTriangle; break;
      case Rect: m_func = fnRect; break;
      case Ramp: m_func = fnRamp; break;
      case Saw: m_func = fnSaw; break;
      case Bell: m_func = fnBell; break;
      case Noise: m_func = fnNoise; break;
      case Organ: m_func = fnOrgan; break;
      case Piano: m_func = fnPiano; break;
      case AnalogRect: m_func = fnAnalogRect; break;
      case AnalogSaw: m_func = fnAnalogSaw; break;
      default: m_func = fnSine; break;
   }
}

// Gain/Volume
int MultiOSCf::getVolume() const { return m_volume; }
void MultiOSCf::setVolume( int volume )
{
   m_volume = std::clamp( volume, 0, 200 );
   m_amp = T(0.0001) * (m_volume*m_volume);
}

// Total frequency = getFreqFromMidiNote( midiNote + detune_in_cents/100 ) + lfo + etc...
MultiOSCf::T
MultiOSCf::getFrequency() const { return m_freq; }
void
MultiOSCf::setFrequency( T freq )
{
   m_freq = freq;
   m_period = T(1000) / m_freq;
}

// Total phase in range [0,1]
MultiOSCf::T
MultiOSCf::getPhase() const { return m_phase; }
void
MultiOSCf::setPhase( T phase ) { m_phase = phase; }

// Total pulseWidth in range [0,1], default: 0.5, near 0 or 1 its really impulse or noise like
MultiOSCf::T
MultiOSCf::getPulseWidth() const { return m_pulseWidth; }
void
MultiOSCf::setPulseWidth( T pulseWidth ) { m_pulseWidth = pulseWidth; }

// Center of oscillator, default: 0.0 = neutral addition
MultiOSCf::T
MultiOSCf::getCenter() const { return m_center; }
void
MultiOSCf::setCenter( float center ) { m_center = center; }

// Range of oscillator, default: 1.0 = neutral multiplication
MultiOSCf::T
MultiOSCf::getRange() const { return m_range; }
void
MultiOSCf::MultiOSCf::setRange( float range ) { m_range = range; }

//   |  /|   | ma = m_amp / (pulseWidth * m_period)
//   | / |   | mb = m_amp / ((1.0 - pulseWidth) * m_period)
//  _|/  |   |
//   | a | b/| pulseWidth = a / (a + b), T = 1 / f
//   |   | / |
//   |   |/  |   Like two mirrored ADSR without release phase,

float
MultiOSCf::computeSample( T t ) const
{
   if ( m_isBypassed || m_volume < 1 )
      return m_bypassValue;

   // float limited = std::clamp( fnGenerator( t ), T(-1), T(1) );
   //float unlimited = fnGenerator( t );
   float unlimited = m_func( t, m_freq, m_period, m_phase, m_pulseWidth );
   float sample = (m_range * unlimited) + m_center; // fma( m_range, unlimited, m_center );
   return m_amp * sample;
}

float
MultiOSCf::fnNoise( T /*t*/, T /*freq*/, T /*period*/, T /*phase*/, T /*pulseWidth*/ )
{
   return (2.0 * (double( rand() % (1<<30) ) / double( (1<<30) ))) - 1.0;
}

float
MultiOSCf::fnSine( T t, T freq, T period, T phase, T /*pulseWidth*/ )
{
   t = fmod( T(1000)*t + phase * period, period );     // [s] to [ms]!
   float arg = fmod( (T(M_PI*0.002) * freq) * t, T(M_PI*2.0) );     // [s] to [ms]!
   return sinf( arg ); // [ms] to [s]!
}

// Remember period is 1/freq and removes the division per sample.
float
MultiOSCf::fnRect( T t, T /*freq*/, T period, T phase, T pulseWidth )
{
   t = fmod( T(1000)*t + phase * period, period ); // seconds to milliseconds
   if ( t <= pulseWidth * period )
   {
      return T(1);
   }
   else
   {
      return -T(1);
   }
}

float
MultiOSCf::fnTriangle( T t, T /*freq*/, T period, T phase, T pulseWidth )
{
   t = fmod( T(1000)*t + (phase * period), period ); //  + m_phase * m_period seconds to milliseconds
   T ta = std::clamp( pulseWidth * period, T(0.0001), period );
   if ( t < ta )
   {
      T ma = T(2) / ta;
      return T(1) - t * ma;
   }

   t -= ta;
   T tb = std::clamp( (T(1) - pulseWidth) * period, T(0.0001), period );
   T mb = T(2) / tb;
   return T(-1) + mb * t ;
}

float
MultiOSCf::fnRamp( T t, T /*freq*/, T period, T phase, T /*pulseWidth*/ )
{
   t = fmod( T(1000)*t + (phase * period), period ); // seconds to milliseconds
   T m = T(2) / period;
   return T(-1) + m * t;
}

float
MultiOSCf::fnSaw( T t, T /*freq*/, T period, T phase, T /*pulseWidth*/ )
{
   t = fmod( T(1000)*t + (phase * period), period ); // seconds to milliseconds
   T m = T(2) / period;
   return T(1) - m * t;
}

float
MultiOSCf::fnOrgan( T t, T freq, T period, T phase, T pulseWidth )
{
   t = fmod( T(1000)*t + (phase * period), period ); // seconds to milliseconds
   auto t2 = fmod( T(1000)*t + ((phase + pulseWidth) * period), period ); // seconds to milliseconds
//      auto t3 = fmod( T(1000)*t + ((phase - pulseWidth) * period), period ); // seconds to milliseconds
   T OMEGA = T(M_PI*2.0*0.001) * freq; // time in [ms] not [s]!
   float s1 = 0.25f * sinf( 1.f * OMEGA * t2 );
   float s2 = 0.5f * sinf( 2.f * OMEGA * t );
   float s3 = 1.0f * sinf( OMEGA * t );
   float s4 = 0.5f * sinf( 0.5f * OMEGA * t );
   float s5 = 0.25f * sinf( 0.25f * OMEGA * t );

//      float s6 = 0.25f * sinf( 2.001f * OMEGA * t2 );
//      float s7 = 0.2f * sinf( 0.5001f * OMEGA * t2 );
//      float s8 = 0.15f * sinf( 0.501f * OMEGA * t3 );
//      float s9 = 0.25f * sinf( 0.25001f * OMEGA * t3 );
   return s1 + s2 + s3 + s4 + s5; // + s6 + s7 + s8 + s9;
}

float
MultiOSCf::fnPiano( T t, T freq, T period, T phase, T pulseWidth )
{
   float OMEGA = T(M_PI*2.0) * freq;
   float s1 = 0.5f * sinf( OMEGA * (t + phase) );
   float s2 = sinf( OMEGA * t );
   float s3 = 0.5f * sinf( OMEGA * (t + pulseWidth) );
   return s1 + s2 + s3;
}

float
MultiOSCf::fnAnalogRect( T t, T freq, T period, T phase, T pulseWidth )
{
   return fnOrgan( t, freq, period, phase );

//   t = fmod( T(1000)*t + (phase * period), period ); // seconds to milliseconds
//   float sampleRate = 48000;
//   uint64_t phase = 0; // uint64_t( t * float(uint64_t(1)<<32) ) % (uint64_t(1)<<32);
//   uint32_t dt = freq * float(uint64_t(1)<<32) / m_sampleRate; // powf(2.0f, 32.0f)
//   float s = 0.0f;
//   fastsin2( phase );

//      //o << "i = " << i << ", t = " << t << ", s = " << s << ", dt = " << dt << "\n";
//      phase += dt;
//      if ( phase > 0xffffffff )
//      {
//         phase -= 0xffffffff;
//      }
//   }
}

float
MultiOSCf::fnAnalogSaw( T t, T freq, T period, T phase, T pulseWidth )
{
   return fnOrgan( t, freq, period, phase );
}

//   inline float
//   fnGenerator( T t ) const
//   {
//      if ( m_type == Noise )
//      {
//         return (2.0 * (double( rand() % (1<<30) ) / double( (1<<30) ))) - 1.0;
//      }
//      else if ( m_type == Sine )
//      {
//         t = fmod( T(1000)*t + m_phase * m_period, m_period );     // [s] to [ms]!
//         return sinf( T(M_PI*2.0 * 0.001) * m_freq * t ); // [ms] to [s]!
//      }
//      else if ( m_type == Rect )
//      {
//         t = fmod( T(1000)*t + m_phase * m_period, m_period ); // seconds to milliseconds
//         if ( t < m_pulseWidth * m_period )
//         {
//            return T(1);
//         }
//         else
//         {
//            return -T(1);
//         }
//      }
//      else if ( m_type == Triangle )
//      {
//         t = fmod( T(1000)*t + (m_phase * m_period), m_period ); //  + m_phase * m_period seconds to milliseconds
//         T ta = std::clamp( m_pulseWidth * m_period, T(0.0001), m_period );
//         if ( t < ta )
//         {
//            T ma = T(2) / ta;
//            return T(1) - t * ma;
//         }

//         t -= ta;
//         T tb = std::clamp( (T(1) - m_pulseWidth) * m_period, T(0.0001), m_period );
//         T mb = T(2) / tb;
//         return T(-1) + mb * t ;
//      }
//      else if ( m_type == Ramp )
//      {
//         t = fmod( T(1000)*t + (m_phase * m_period), m_period ); // seconds to milliseconds
//         T m = T(2) / m_period;
//         return T(-1) + m * t;
//      }
//      else if ( m_type == Saw )
//      {
//         t = fmod( T(1000)*t + (m_phase * m_period), m_period ); // seconds to milliseconds
//         T m = T(2) / m_period;
//         return T(1) - m * t;
//      }
//      else
//      {
//         return m_bypassValue;
//      }
//   }




} // end namespace audio.
} // end namespace de.


