#include <de/audio/SpektrumFFTW3.hpp>

#ifdef _IRR_LINUX_PLATFORM_
#include <fftw3.h>
#else
#include <fftw3/fftw3.h>
#endif

namespace de {
namespace audio {

// FFT real to complex
struct FourierTransformRtoC
{
   DE_CREATE_LOGGER("de.audio.FourierTransformRtoC")
private:
   uint32_t m_fftSize;
   double* m_InBuf;	// real input-vector
   fftw_complex* m_OutBuf; // complex output-vector
   fftw_plan m_fftPlan;
   std::string m_fftWisdomFile;

public:
   FourierTransformRtoC( uint32_t m_fftSize = -1, std::string wisdomUri = "FFT_RtoC.wis" )
      : m_fftSize(0)
      , m_InBuf(0)
      , m_OutBuf(0)
      , m_fftWisdomFile( wisdomUri )
   {
      // Check for wisdom
      if( fftw_import_wisdom_from_filename( m_fftWisdomFile.c_str() ) == 0 )
      {
         printf("wisdom not loaded.\n");
      }
      else
      {
         printf("wisdom loaded.\n");
      }

      if (m_fftSize < 0)
         m_fftSize = 1024;	// default
      else
         m_fftSize = (uint32_t)m_fftSize;

      m_InBuf = new double[m_fftSize];
      if (!m_InBuf)
      {
         printf("Could not create FFT_InputVectorArray.\n");
      }
      else
      {
         for (uint32_t i=0; i<m_fftSize; i++)
         {
            m_InBuf[i] = 0.0;
            //m_OutBuf[i].re = 0.0;
            //m_OutBuf[i].im = 0.0;
         }
      }

      m_OutBuf = (fftw_complex*) fftw_malloc( m_fftSize*sizeof(fftw_complex) );
      if (!m_OutBuf)
      {
         printf("Could not create FFT_OutputVectorArray.\n");
      }
      else
      {
   //				for (uint32_t i=0; i<m_fftSize; i++)
   //				{
   //					m_InBuf[i] = 0.0;
   //					//m_OutBuf[i].re = 0.0;
   //					//m_OutBuf[i].im = 0.0;
   //				}
      }

      m_fftPlan = fftw_plan_dft_r2c_1d( m_fftSize, m_InBuf, m_OutBuf, FFTW_MEASURE );
   }


   ~FourierTransformRtoC()
   {
      fftw_export_wisdom_to_filename( m_fftWisdomFile.c_str() ); // Store Wisdom
      fftw_destroy_plan(m_fftPlan);
      if (m_InBuf) { delete [] m_InBuf; m_InBuf = 0; }
      if (m_OutBuf) { fftw_free(m_OutBuf); m_OutBuf = 0; }
      fftw_cleanup();
      //m_fftSize = 0;
   }

   uint32_t
   getFFTSize() const { return m_fftSize; }

   void
   fft() { fftw_execute( m_fftPlan ); }

   std::string
   getPlanInfo() const
   {
      double fft_cost = fftw_cost( m_fftPlan );
      double plan_add, plan_mul, plan_fma;
      fftw_flops( m_fftPlan, &plan_add, &plan_mul, &plan_fma );

      std::ostringstream s; s << "fftPlan{ n:" << m_fftSize << "; "
         "cost:" << fft_cost << "; "
         "add:" << plan_add << "; "
         "mul:" << plan_mul << "; "
         "fma:" << plan_fma << "; }";
      return s.str();
   }

   template < typename T >
   void setInputData( std::vector< T > & _in )
   {
      size_t i_max = std::min( _in.size(), size_t(m_fftSize) );
      for ( size_t i = 0; i < i_max; i++ )
      {
         m_InBuf[ i ] = (double)_in[ i ];
      }

      if ( i_max < m_fftSize )
      {
         for (uint32_t i=i_max; i<m_fftSize; i++)
         {
            m_InBuf[i] = 0.0;
         }
      }
   }

   template < typename T >
   glm::tvec2< T >
   computeMinMax( std::vector< T > const & src )
   {
      glm::tvec2< T > result(
         std::numeric_limits< T >::max(),       // will contain min, starts with max.
         std::numeric_limits< T >::lowest() );  // will contain max, starts with min.

      for ( size_t i = 0; i < src.size(); i++ )
      {
         T value = src[ i ];
         result.x = std::min( result.x, value );
         result.y = std::max( result.y, value );
      }
      return result;
   }

   template < typename T >
   void
   getScaledPowerSpectrum( std::vector< T > & _out, double dB_min = 0.0, double dB_max = 150.0 )
   {
      size_t const i_max = std::min( _out.size(), size_t(m_fftSize) );
      _out.clear();
      _out.reserve( i_max );
      double const dB_range_inv = 1.0 / ( dB_max - dB_min );
      for ( size_t i = 0; i < i_max; i++ )
      {
         fftw_complex const & z = m_OutBuf[ i ];	// read only access to complex number
         double z_abs = sqrt( z[0]*z[0] + z[1]*z[1] );	// magnitude ( abs ) of complex number / Betrag der komplexen Zahl z
         z_abs = 1.0f*log10( z_abs ) ;				// convert to decibels / Umwandlung in Dezibels
         z_abs -= dB_min;							// value is in range [ 0, dB_range ] now
         if ( z_abs > dB_max )						// clip values to maximum, if greater
              z_abs = dB_max;
         z_abs *= dB_range_inv;						// value is in range [ 0, 1 ] now, normalized
         _out.push_back( T( z_abs ) );				// insert at the end of array, no allocation involved
      }
   }

   template < typename T >
   void
   getPowerSpectrum( std::vector< T > & _out )
   {
      size_t const i_max = std::min( _out.size(), size_t(m_fftSize) );
      _out.clear();
      _out.reserve( i_max );
      for ( size_t i = 0; i < i_max; i++ )
      {
         fftw_complex const & z = m_OutBuf[ i ];		// Lesezugriff auf komplexe Zahl
         double z_abs = sqrt( z[0]*z[0] + z[1]*z[1] );	// Betrag der komplexen Zahl z
         _out.push_back( T( z_abs ) );
      }

      glm::tvec2< T > z_minmax(
         std::numeric_limits< T >::max(),       // will contain min, starts with max.
         std::numeric_limits< T >::lowest() );  // will contain max, starts with min.

      const double z_range_inv = 100.0 / ( double(z_minmax.y) - double(z_minmax.x) );

      for ( size_t i = 0; i < i_max; i++ )
      {
         T value = T( z_range_inv*( double( _out[i] ) - double( z_minmax.x ) ) );
         _out[i] = value;
      }
   }

   template < typename T >
   void
   getPowerSpectrumAsDecibels( std::vector< T > & _out )
   {
      size_t const i_max = std::min( _out.size(), size_t(m_fftSize) );
      _out.clear();
      _out.reserve( i_max );
      for ( size_t i = 0; i < i_max; i++ )
      {
         fftw_complex const & z = m_OutBuf[ i ];	// Lesezugriff auf komplexe Zahl
         double z_abs = sqrt( z[0]*z[0] + z[1]*z[1] );	// Betrag der komplexen Zahl z
         z_abs = 20.0f*log10( z_abs );				// Umwandlung in Dezibels
         _out.push_back( T( z_abs ) );
      }
   }

};


// ===========================================================================
bool
SpektrumTools::draw( SampleBuffer const & src, int channel, Image & dst, Recti pos )
// ===========================================================================
{
   int w = pos.getWidth();
   int h = pos.getHeight();


}


} // end namespace audio
} // end namespace de

