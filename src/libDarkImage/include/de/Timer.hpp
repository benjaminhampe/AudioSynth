#ifndef DARKTIMER_HPP
#define DARKTIMER_HPP

#include <cstdint>
#include <sstream>

// ===========================================================================
// DarkRandom
// ===========================================================================
void     dbRandomize();
double   dbRndRadians();
double   dbRndDegrees();

// ===========================================================================
// DarkTimer
// ===========================================================================
int64_t  dbNanoseconds();
int64_t  dbMicroseconds();
int32_t  dbMilliseconds();
double   dbSeconds();


// ===========================================================================
// PerformanceTimer
// ===========================================================================

namespace de {

struct PerformanceTimer
{
   PerformanceTimer();
   int64_t ns() const;  // Nanoseconds
   int64_t us() const;  // Microseconds
   int32_t ms() const;  // Milliseconds
   double toSeconds() const;  // Seconds

   void start();
   void stop();
   void stopAndLog( std::string message );

   int64_t m_CurrTime; // In Nanoseconds
   int64_t m_LastTime; // In Nanoseconds
   bool m_IsStarted = false;
};

} // end namespace de

#endif // DARKTIMER_HPP
