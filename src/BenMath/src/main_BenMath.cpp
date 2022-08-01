#include <DarkImage.hpp>
#include <functional>

namespace de {

struct BenMath
{
   DE_CREATE_LOGGER("BenMath")

   typedef float S;
   typedef float T;

   static T
   testFunc( std::function< S(T) > const & func, T min, T max, uint64_t n )
   {
      // PerformanceTimer perf;
      // perf.start();

      T t = min;
      T dt = (max - min) / T(n);

      // std::ostringstream o;
      // o << "n = " << n << "\n";
      // o << "min = " << min << "\n";
      // o << "max = " << max << "\n";
      // o << "dt = " << dt << "\n";

      T accum = T(0);

      for ( uint64_t i = 0; i < n; i++ )
      {
         accum += func( t );
         //o << "i = " << i << ", t = " << t << ", s = " << s << ", dt = " << dt << "\n";
         t += dt;
      }

      // perf.stop();
      // DE_DEBUG("=========================================================")
      // DE_DEBUG("TEST:\n", o.str() )
      // DE_FLUSH
      return accum;
   }

   // phase is in range 0 -> (2^32)-1 and maps to 0 -> ~2PI
//   static float
//   fastsin( float t, float freq, float sampleRate )
//   {
//      uint32_t phase = mod( t, float(0), float(2.0 * M_PI));
//      uint32_t dt = freq * float(uint64_t(1)<<32) / sampleRate; // powf(2.0f, 32.0f)
//      // Then it's just:
//      out = fastsin(phase);
//      phase += dt;
//   }

   // phase is in range 0 -> (2^32)-1 and maps to 0 -> ~2PI
   static float
   fastabs( float v )
   {
      uint32_t u = *reinterpret_cast< uint32_t* >( &v ) & 0x7fffffff;
      return *reinterpret_cast< float* >( &u );
   }

   // phase is in range 0 -> (2^32)-1 and maps to 0 -> ~2PI
   static float
   fastsin( uint32_t phase )
   {
      const float frf3 = -1.0f / 6.0f;
      const float frf5 = 1.0f / 120.0f;
      const float frf7 = -1.0f / 5040.0f;
      const float frf9 = 1.0f / 362880.0f;
      const float f0pi5 = 1.570796327f;
      float x, x2, asin;
      uint32_t tmp = 0x3f800000 | (phase >> 7);
      if (phase & 0x40000000)
      tmp ^= 0x007fffff;
      x = ( * ((float * )&tmp) - 1.0f) * f0pi5;
      x2 = x * x;
      asin = ((((frf9 * x2 + frf7) * x2 + frf5) * x2 + frf3) * x2 + 1.0f) * x;
      return (phase & 0x80000000) ? -asin : asin;
   }

   static float
   fastsin2( uint32_t phase )
   {
      const float frf3 = -1.0f / 6.0f;
      const float frf5 = 1.0f / 120.0f;
      const float frf7 = -1.0f / 5040.0f;
      const float f0pi5 = 1.570796327f;
      float x, x2, asin;
      uint32_t tmp = 0x3f800000 | (phase >> 7);
      if (phase & 0x40000000)
      tmp ^= 0x007fffff;
      x = ( * ((float * )&tmp) - 1.0f) * f0pi5;
      x2 = x * x;
      asin = (((frf7 * x2 + frf5) * x2 + frf3) * x2 + 1.0f) * x; // faster here
      return (phase & 0x80000000) ? -asin : asin;
   }

   static void
   testAll( int argc, char** argv )
   {
      uint64_t n = 100 * 1000 * 1000;
      float fMin = std::numeric_limits< float >::lowest();
      float fMax = std::numeric_limits< float >::max();

      DE_DEBUG("[BenMath] n = ", n )
      DE_FLUSH

      PerformanceTimer perf;
   {
      perf.start();
      volatile int32_t s = 0;
      for ( uint64_t i = 0; i < n; i++ )
      {
         s += int32_t( i );
      }

      perf.stop();
      double ms = 1e-6 * perf.ns();
      double opms = 1e-3 * double( n )/ms;
      DE_DEBUG("[addi32] needed = ", ms, " ms. Operations per Microsecond = ",opms, ", s= ",s )
      DE_FLUSH
   }
   {
      perf.start();
      volatile int64_t s = 0;
      for ( uint64_t i = 0; i < n; i++ )
      {
         s += int64_t( i );
      }

      perf.stop();
      double ms = 1e-6 * perf.ns();
      double opms = 1e-3 * double( n )/ms;
      DE_DEBUG("[addi64] needed = ", ms, " ms. Operations per Microsecond = ",opms, ", s= ",s )
      DE_FLUSH
   }
   {
      perf.start();
      volatile uint32_t s = 0;
      for ( uint64_t i = 0; i < n; i++ )
      {
         s += uint32_t( i );
      }

      perf.stop();
      double ms = 1e-6 * perf.ns();
      double opms = 1e-3 * double( n )/ms;
      DE_DEBUG("[addu32] needed = ", ms, " ms. Operations per Microsecond = ",opms, ", s= ",s )
      DE_FLUSH
   }
   {
      perf.start();
      volatile uint64_t s = 0;
      for ( uint64_t i = 0; i < n; i++ )
      {
         s += uint64_t( i );
      }

      perf.stop();
      double ms = 1e-6 * perf.ns();
      double opms = 1e-3 * double( n )/ms;
      DE_DEBUG("[addu64] needed = ", ms, " ms. Operations per Microsecond = ",opms, ", s= ",s )
      DE_FLUSH
   }
   {
      //testFunc( fmodf, fMin, fMax, n );
      perf.start();
      T t = fMin;
      T dt = (fMax - fMin) / T(n);

      float s = 0.0f;
      for ( uint64_t i = 0; i < n; i++ )
      {
         s += fmodf( t, float(2.0 * M_PI) );
         t += dt;
      }

      perf.stop();
      double ms = 1e-6 * perf.ns();
      double opms = 1e-3 * double( n )/ms;
      DE_DEBUG("[fmodf] needed = ", ms, " ms. Operations per Microsecond = ",opms, ", s= ",s )
      DE_FLUSH
   }
   {
      //testFunc( fmodf, fMin, fMax, n );
      perf.start();
      uint64_t s = 0;
      for ( uint64_t i = 0; i < n; i++ )
      {
         s += uint32_t( i ) % uint32_t( 65 );
      }

      perf.stop();
      double ms = 1e-6 * perf.ns();
      double opms = 1e-3 * double( n )/ms;
      DE_DEBUG("[modi32] needed = ", ms, " ms. Operations per Microsecond = ",opms, ", s= ",s )
      DE_FLUSH
   }
   {
      //testFunc( fmodf, fMin, fMax, n );
      perf.start();
      uint64_t s = 0;
      for ( uint64_t i = 0; i < n; i++ )
      {
         s += uint64_t( i ) % uint64_t( 65 );
      }

      perf.stop();
      double ms = 1e-6 * perf.ns();
      double opms = 1e-3 * double( n )/ms;
      DE_DEBUG("[modi64] needed = ", ms, " ms. Operations per Microsecond = ",opms, ", s= ",s )
      DE_FLUSH
   }

   {
      perf.start();
      float s = testFunc( sinf, fMin, fMax, n );
      perf.stop();
      double ms = 1e-6 * perf.ns();
      double opms = 1e-3 * double( n )/ms;
      DE_DEBUG("[sinf] needed = ", ms, " ms. Operations per Microsecond = ",opms, ", s= ",s )
      DE_FLUSH
   }
   {
      perf.start();
      float freq = 440.0f;
      float sampleRate = 48000;
      uint64_t phase = 0; // uint64_t( t * float(uint64_t(1)<<32) ) % (uint64_t(1)<<32);
      uint32_t dt = freq * float(uint64_t(1)<<32) / sampleRate; // powf(2.0f, 32.0f)
      float s = 0.0f;
      for ( uint64_t i = 0; i < n; i++ )
      {
         s += fastsin( phase );

         //o << "i = " << i << ", t = " << t << ", s = " << s << ", dt = " << dt << "\n";
         phase += dt;
         if ( phase > 0xffffffff )
         {
            phase -= 0xffffffff;
         }
      }
      perf.stop();
      double ms = 1e-6 * perf.ns();
      double opms = 1e-3 * double( n )/ms;
      DE_DEBUG("[fastsin] needed = ", ms, " ms. Operations per Microsecond = ",opms, ", s= ",s )
      DE_FLUSH
}
{
      perf.start();
      float freq = 440.0f;
      float sampleRate = 48000;
      uint64_t phase = 0; // uint64_t( t * float(uint64_t(1)<<32) ) % (uint64_t(1)<<32);
      uint32_t dt = freq * float(uint64_t(1)<<32) / sampleRate; // powf(2.0f, 32.0f)
      float s = 0.0f;
      for ( uint64_t i = 0; i < n; i++ )
      {
         s += fastsin2( phase );

         //o << "i = " << i << ", t = " << t << ", s = " << s << ", dt = " << dt << "\n";
         phase += dt;
         if ( phase > 0xffffffff )
         {
            phase -= 0xffffffff;
         }
      }
      perf.stop();
      double ms = 1e-6 * perf.ns();
      double opms = 1e-3 * double( n )/ms;
      DE_DEBUG("[fastsin2] needed = ", ms, " ms. Operations per Microsecond = ",opms, ", s= ",s )
      DE_FLUSH
}
{
      perf.start();
      float s = testFunc( tanf, fMin, fMax, n );
      perf.stop();
      double ms = 1e-6 * perf.ns();
      double opms = 1e-3 * double( n )/ms;
      DE_DEBUG("[tanf] needed = ", ms, " ms. Operations per Microsecond = ",opms, ", s= ",s )
      DE_FLUSH
}
{
      perf.start();
      float s = testFunc( fabs, fMin, fMax, n );
      perf.stop();
      double ms = 1e-6 * perf.ns();
      double opms = 1e-3 * double( n )/ms;
      DE_DEBUG("[fabs] needed = ", ms, " ms. Operations per Microsecond = ",opms, ", s= ",s )
      DE_FLUSH
}
{
      perf.start();
      float s = testFunc( fastabs, fMin, fMax, n );
      perf.stop();
      double ms = 1e-6 * perf.ns();
      double opms = 1e-3 * double( n )/ms;
      DE_DEBUG("[fastabs] needed = ", ms, " ms. Operations per Microsecond = ",opms, ", s= ",s )
      DE_FLUSH
}
{
      perf.start();
      float s = testFunc( sqrt, fMin, fMax, n );
      perf.stop();
      double ms = 1e-6 * perf.ns();
      double opms = 1e-3 * double( n )/ms;
      DE_DEBUG("[sqrtf] needed = ", ms, " ms. Operations per Microsecond = ",opms, ", s= ",s )
      DE_FLUSH
}

   }

};

} // end namespace de


//========================================================================
int main(int argc, char** argv)
//========================================================================
{
   de::BenMath::testAll( argc, argv );
   return 0;
}

