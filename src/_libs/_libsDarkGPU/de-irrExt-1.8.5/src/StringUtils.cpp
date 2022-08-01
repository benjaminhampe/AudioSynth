#include <irrExt/StringUtils.hpp>

#include <time.h>

std::string
printDate()
{
   tm* t = localtime( nullptr );
   std::stringstream s;
   s   << t->tm_year
   << "-" << t->tm_mon
   << "-" << t->tm_mday
   << " " << t->tm_hour
   << ":" << t->tm_min
   << ":" << t->tm_sec;
   return s.str();
}

// PerfTimer :

#if defined(_WIN32) || defined(WIN32)
   #ifndef WIN32_LEAN_AND_MEAN
   #define WIN32_LEAN_AND_MEAN
   #endif
   #include <windows.h>

   int64_t
   getPerfTimer()
   {
      LARGE_INTEGER li;
      QueryPerformanceCounter( &li );
      return static_cast< int64_t >( li.QuadPart );
   }

   int64_t
   getPerfTimerFrequency()
   {
      LARGE_INTEGER li;
      QueryPerformanceFrequency( &li );
      return static_cast< int64_t >( li.QuadPart );
   }


   std::string
   printSystemTime()
   {
      SYSTEMTIME t;
      GetSystemTime( &t );
      std::stringstream s;
      s   << t.wYear
      << "-" << t.wMonth
      << "-" << t.wDay
      << " " << t.wHour
      << ":" << t.wMinute
      << ":" << t.wSecond
      << "." << t.wMilliseconds;
      return s.str();
   }

   std::string
   printLocalTime()
   {
      SYSTEMTIME t; GetLocalTime( &t );
      std::stringstream s;
      s   << t.wYear
      << "-" << t.wMonth
      << "-" << t.wDay
      << " " << t.wHour
      << ":" << t.wMinute
      << ":" << t.wSecond
      << "." << t.wMilliseconds;
      return s.str();
   }

#else // Linux

   int64_t
   getPerfTimer()
   {
      return 0;
   }

   int64_t
   getPerfTimerFrequency()
   {
      return 0;
   }

   std::string
   printSystemTime()
   {
      return "";
   }

   std::string
   printLocalTime()
   {
      return "";
   }
#endif


std::string printTimer()
{
    std::stringstream s;
    s   << getPerfTimer() << ", f=" << getPerfTimerFrequency() << " Hz\n";
    return s.str();
}
