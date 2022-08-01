#ifndef DE_AUDIO_MEMORY_SHIFT_BUFFER_HPP
#define DE_AUDIO_MEMORY_SHIFT_BUFFER_HPP

#include <de/Logger.hpp>
#include <de/Math.hpp>
#include <de/audio/SampleType.hpp>
#include <functional>

namespace de {
namespace audio {

template < typename T >
inline void
DSP_RESIZE( std::vector< T > & dst, uint64_t dstSamples )
{
   if ( dstSamples != dst.size() ) dst.resize( dstSamples );
}

template < typename T >
inline void
DSP_FILLZERO( std::vector< T > & dst )
{
   auto p = dst.data();
   for ( size_t i = 0; i < dst.size(); ++i ) { *p = T(0); p++; }
}

inline void
DSP_FILLZERO( std::vector< float > & dst, uint64_t sampleCount )
{
   auto p = dst.data();
   sampleCount = std::min( sampleCount, dst.size() );
   for ( size_t i = 0; i < sampleCount; ++i ) { *p = 0.0f; p++; }
}

inline void
DSP_FILLZERO( float* dst, uint64_t sampleCount )
{
   if ( !dst ) return;
   if ( sampleCount < 1 ) return;
   for ( size_t i = 0; i < sampleCount; ++i ) { *dst = 0.0f; dst++; }
}

inline void
DSP_FILLZERO( double* dst, uint64_t sampleCount )
{
   if ( !dst ) return;
   if ( sampleCount < 1 ) return;
   for ( size_t i = 0; i < sampleCount; ++i ) { *dst = 0.0; dst++; }
}

inline void
DSP_GET_CHANNEL(
   float* dst, uint64_t dstFrames,
   float const* src, uint64_t srcFrames, uint32_t srcChannel, uint32_t srcChannels )
{
   float* pDst = dst;
   float const* pSrc = src + srcChannel;
   auto n = std::min( srcFrames, dstFrames );
   for ( size_t i = 0; i < n; i++ )
   {
      *pDst = (*pSrc);
      pDst++;
      pSrc += srcChannels; // jump to next sample of srcChannel.
   }
}

inline void
DSP_COPY( float const* src, float* dst, uint64_t sampleCount )
{
   for ( size_t i = 0; i < sampleCount; ++i )
   {
      *dst = (*src);
      dst++;
      src++;
   }
}

inline void
DSP_ADD( float const* src, float* dst, uint64_t sampleCount )
{
   for ( size_t i = 0; i < sampleCount; ++i )
   {
      *dst += (*src);
      dst++;
      src++;
   }
}

inline void
DSP_MUL( float* dst, uint64_t sampleCount, float scalar )
{
   for ( size_t i = 0; i < sampleCount; ++i )
   {
      *dst *= scalar;
      dst++;
   }
}

inline void
DSP_COPY_SCALED( float const* src, float* dst, uint64_t sampleCount, float scale )
{
   for ( size_t i = 0; i < sampleCount; ++i )
   {
      *dst = (*src) * scale;
      dst++;
      src++;
   }
}

inline void
DSP_COPY_SCALED_LIMITED( float const* src, float* dst, uint64_t sampleCount, float scale )
{
   for ( size_t i = 0; i < sampleCount; ++i )
   {
      *dst = std::clamp( (*src) * scale, -1.0f, 1.0f ); // Limiter.
      dst++;
      src++;
   }
}

//inline void
//DSP_PROC( float const* src, float* dst, uint32_t sampleCount, std::function<float(float,float)> const & manipSample )
//{
//   for ( size_t i = 0; i < sampleCount; ++i )
//   {
//      *dst = manipSample(*src,*dst);
//      dst++;
//      src++;
//   }
//}

// RMS - root means square:
inline float
DSP_RMSf( uint32_t channel, float const* src, uint64_t srcFrames, uint32_t srcChannels )
{
   float rms = 0.0f;

   src += channel; // Advance to first sample of 'channel'

   for ( size_t i = 0; i < srcFrames; ++i )
   {
      float sample = *src;
      src += srcChannels;
      rms += sample * sample;
   }

   rms /= float( srcFrames );
   return sqrtf( rms );
}

// RMS - root means square:
inline double
DSP_RMSd( uint32_t channel, float const* src, uint64_t srcFrames, uint32_t srcChannels )
{
   double rms = 0.0;

   src += channel; // Advance to first sample of 'channel'

   for ( size_t i = 0; i < srcFrames; ++i )
   {
      double sample = *src;
      src += srcChannels;
      rms += sample * sample;
   }

   rms /= double( srcFrames );
   return sqrt( rms );
}


// ----------------------------------------------------
//
// ----------------------------------------------------
template < typename T >
struct ShiftBuffer
{
   DE_CREATE_LOGGER("de.audio.ShiftBuffer")
   uint64_t m_fillCounter;
   std::vector< T > m_data;
   //std::vector< T > m_copy;

   ShiftBuffer() : m_fillCounter( 0 ) {}
   uint64_t size() const { return m_data.size(); }
   T const * data() const { return m_data.data(); }
   T * data() { return m_data.data(); }

   T const & operator[] ( size_t i ) const { return m_data[ i ]; }
   T & operator[] ( size_t i ) { return m_data[ i ]; }

   void
   resize( size_t elemCount )
   {
      if ( elemCount != size() )
      {
         m_data.resize( elemCount );
         //m_copy.resize( elemCount );
         m_fillCounter = 0;
         //zeroMemory();
      }
   }

   // push samples at back of vector, destroy n older elements by shifting
   void
   push( std::string caller, float const* src, size_t srcCount, bool debug = false )
   {
      auto n = m_data.size();
      if ( n < srcCount )
      {
         DE_ERROR("caller(",caller,") -> ShiftBuffer to small n(",n,"), srcCount(",srcCount,"), fillCounter(",m_fillCounter,")")
         //resize( srcCount );
         return;
      }

      if ( m_fillCounter < n ) // we can still push_back()/emplace_back()
      {
         auto freeSlots = std::min( n - m_fillCounter, srcCount );
         if ( freeSlots > 0 )
         {
            if ( debug )
            {
               DE_DEBUG("FreeSlots(",freeSlots,"), n(",n,"), srcCount(",srcCount,"), fillCounter(",m_fillCounter,")")
            }
            auto pDst = m_data.data() + m_fillCounter;
            for ( size_t i = 0; i < freeSlots; ++i )
            {
               *pDst = *src;
               pDst++;
               src++;
            }
            srcCount -= freeSlots;        // Update srcCount for check later
            m_fillCounter += freeSlots;
         }
      }

      if ( srcCount > 0 )
      {
         if ( debug )
         {
            DE_DEBUG("ShiftLeft(",srcCount,"), n(",n,"), fillCounter(",m_fillCounter,")")
         }
         shiftLeft( srcCount, debug );
         DSP_COPY( src, m_data.data() + n - srcCount, srcCount );
      }
   }

   void zeroMemory()
   {
      //for ( auto & f : m_copy ) f = 0.0f;
      for ( auto & f : m_data ) f = 0.0f;
   }

   void shiftLeft( uint64_t shifts, bool debug = false )
   {
      //DE_ERROR("Shift(",shifts,"), m_fillCounter(",m_fillCounter,")")
      if ( shifts >= m_data.size() )
      {
         if ( debug )
         {
            DE_ERROR("Nothing to shift(",shifts,"), m_data.size(",m_data.size(),")")
         }
         return; // Nothing todo, we can only overwrite entirely
      }

      if ( debug )
      {
         DE_DEBUG("Shift(",shifts,"), m_data.size(",m_data.size(),")")
      }

      for ( uint64_t i = shifts; i < m_data.size(); ++i )
      {
         m_data[ i-shifts ] = m_data[ i ];
      }
   }


   void pushZero( size_t srcCount )
   {
      size_t n = m_data.size();
      if ( n < srcCount )
      {
         n = srcCount;
         m_data.resize( n );
         m_fillCounter = 0;
      }

      int64_t shifts = int64_t(m_fillCounter + srcCount) - int64_t(n);
      if ( shifts > 0 )
      {
         shiftLeft( shifts );
      }

      for ( uint64_t i = 0; i < srcCount; ++i )
      {
         m_data[ i ] = 0.0f;
      }

      m_fillCounter += srcCount;
   }

   std::string
   toString() const
   {
      size_t n = m_data.size();
      std::stringstream s;
      s << "n = " << n << "\n";
      for ( uint64_t i = 0; i < n; ++i )
      {
         s << "[" << i << "] " << m_data[ i ] << "\n";
      }
      return s.str();
   }

   static void test()
   {
      ShiftBuffer testObj;
      testObj.resize( 20 );

      std::vector< T > a{ 0,1,2,3,4,5,6,7,8,9};
      std::vector< T > b{ 10,11,12,13,14,15,16,17,18,19};
      std::vector< T > c{ 20,21,22,23,24,25,26,27,28,29};
      DE_DEBUG( "Test{0}.Resize(20): ", testObj.toString() )
      testObj.push( a.data(), 10, true );
      DE_DEBUG( "Test{1}.Push(a): ", testObj.toString() )
      testObj.push( b.data(), 10, true );
      DE_DEBUG( "Test{2}.Push(b): ", testObj.toString() )
      testObj.push( c.data(), 10, true );
      DE_DEBUG( "Test{3}.Push(c): ", testObj.toString() )
   }
};

} // end namespace audio
} // end namespace de

#endif

