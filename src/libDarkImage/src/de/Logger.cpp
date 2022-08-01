#include <de/Logger.hpp>

#ifdef USE_DE_LOGGING

#include <de/Timer.hpp>

namespace de {

// ===========================================================================
Logger::Logger( std::string name )
// ===========================================================================
   : m_Name( std::move( name ) )
{
   m_TimeStart = dbSeconds();
   m_Time = 0.0;
   m_LastTime = 0.0;
}

std::string const &
Logger::getName() const { return m_Name; }

void
Logger::setName( std::string const & name ) { m_Name = name; }

Logger&
Logger::begin( int line, LogLevel const & logLevel, std::string const & file, std::string const & func, std::thread::id threadId )
{
   m_Time = dbSeconds() - m_TimeStart;
   // flush();
   //std::ostringstream s;
   size_t const BUFFER_SIZE = 32;
   char txt_time[BUFFER_SIZE];
   snprintf(txt_time, BUFFER_SIZE, "%.8lf", m_Time );

   m_Stream.str("");
   m_Stream << "[" << txt_time << "] "
            << "[" << threadId << "] "
            << "[" << logLevel.toString() << "] "
            << m_Name << "." << func << "():" << line
            //<< " " << file << ":" << line
            << " :: "
            ;

   return *this;
}

Logger &
Logger::trace( int line, std::string const & file, std::string const & func, std::thread::id threadId )
{
   return begin( line, LogLevel::Trace, file, func, threadId );
}

Logger &
Logger::debug( int line, std::string const & file, std::string const & func, std::thread::id threadId )
{
   return begin( line, LogLevel::Debug, file, func, threadId );
}

Logger &
Logger::info( int line, std::string const & file, std::string const & func, std::thread::id threadId )
{
   return begin( line, LogLevel::Info, file, func, threadId );
}

Logger &
Logger::warn( int line, std::string const & file, std::string const & func, std::thread::id threadId )
{
   return begin( line, LogLevel::Warn, file, func, threadId );
}

Logger &
Logger::error( int line, std::string const & file, std::string const & func, std::thread::id threadId )
{
   return begin( line, LogLevel::Error, file, func, threadId );
}

Logger &
Logger::fatal( int line, std::string const & file, std::string const & func, std::thread::id threadId )
{
   return begin( line, LogLevel::Fatal, file, func, threadId );
}

void
Logger::endl( bool flush )
{
   std::cout << m_Stream.str();

   if ( m_Time > m_LastTime + 0.133 )
   {
      std::cout << std::endl; // Flush once a second to console.
      m_LastTime = m_Time;
   }
   else
   {
      if ( flush )
      {
         std::cout << std::endl;
      }
      else
      {
         std::cout << '\n';   // Dont flush else
      }
   }
}

/*
void
Logger::flush()
{
   //std::cout << dbStr( m_Stream.str() ) << std::endl;
   std::cout.flush();
   std::cout << m_Stream.str() << std::endl;
   //std::cout.flush();
}
*/

} // end namespace de.

#endif
