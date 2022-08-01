#ifndef DARKSTRING_HPP
#define DARKSTRING_HPP

// Standalone header
#include <cstdint>
#include <cwchar>
#include <string>
#include <sstream>
#include <vector>
#include <initializer_list>

std::string
dbSystem( std::string const & commandLine );

std::string
dbsprintf( char const * formatString, ... );
std::wstring
dbwprintf( const wchar_t* formatString, ... );

template< typename ... T >
std::string
dbStrJoin( T const & ... t )
{
   std::ostringstream s;
   (void)std::initializer_list<int>{ (s<<t, 0)... };
   return s.str();
}

//std::wstring
//dbStrJoinW( wchar_t const * formatString, ... );

template< typename ... T >
std::wstring
dbStrJoinW( T const & ... t )
{
   std::wostringstream s;
   (void)std::initializer_list<int>{ (s<<t, 0)... };
   return s.str();
}

//#ifndef _
//#define _(x) dbStrJoin( (x) )
//#endif

//#ifndef _w
//#define _w(x) dbStrJoinW( (x) )
//#endif

std::string
dbStrJoinV( std::vector< std::string > const & v, std::string const & prefix );

std::string
dbStr( std::wstring const & txt );
std::string
dbStr( wchar_t const wc );
std::wstring
dbStrW( std::string const & txt );

std::vector< std::string >
dbStrSplit( std::string const & txt, char searchChar );
std::vector< std::wstring >
dbStrSplit( std::wstring const & txt, wchar_t searchChar );

uint32_t
dbStrReplace( std::string & s, std::string const & from, std::string const & to );
uint32_t
dbStrReplace( std::wstring & s, std::wstring const & from, std::wstring const & to );

char
dbStrLowNibble( uint8_t byte );
char
dbStrHighNibble( uint8_t byte );
std::string
dbStrByte( uint8_t byte );
std::string
dbStrHex( uint32_t color );

void dbStrLowerCase( std::string & out, std::locale const & loc = std::locale() );
void dbStrLowerCase( std::wstring & out, std::locale const & loc = std::locale() );
void dbStrUpperCase( std::string & out, std::locale const & loc = std::locale() );
void dbStrUpperCase( std::wstring & out, std::locale const & loc = std::locale() );

bool dbStrStartsWith( std::string const & str, char c );
bool dbStrStartsWith( std::string const & str, std::string const & query, bool ignoreCase = true );
bool dbStrStartsWith( std::wstring const & str, std::wstring const & query, bool ignoreCase = true );

bool dbStrEndsWith( std::string const & str, char c );
bool dbStrEndsWith( std::string const & str, char const * c );
bool dbStrEndsWith( std::string const & str, std::string const & query, bool ignoreCase = true );
bool dbStrEndsWith( std::wstring const & str, std::wstring const & query, bool ignoreCase = true );

void dbStrTrim( std::string & str );
std::string dbStrTrimLeft( std::string txt );
std::string dbStrTrimRight( std::string txt );

void STRING_LEFT( std::string const & original, std::string const & searchToken, std::string & left );
void STRING_RIGHT( std::string const & original, std::string const & searchToken, std::string & right );
void STRING_RLEFT( std::string const & original, std::string const & searchToken, std::string & left );
void STRING_RRIGHT( std::string const & original, std::string const & searchToken, std::string & right );

inline void
dbMakePosix( std::string & uri )
{
   dbStrReplace( uri, "\\", "/" ); // CR, LF
}

std::string
dbStrByteCount( uint64_t bytes );

std::string
dbStrSeconds( double seconds );

inline std::string
dbSecondsToString( double seconds ) { return dbStrSeconds( seconds ); }

inline std::string
dbFileSizeToString( uint64_t bytes ) { return dbStrByteCount( bytes ); }

void
dbDate( int32_t* year = nullptr, int32_t* month = nullptr, int32_t* dayOfMonth = nullptr,
        int32_t* hour = nullptr, int32_t* minute = nullptr, int32_t* second = nullptr );

std::string
dbStrDate( int32_t year, int32_t month, int32_t dayOfMonth, int32_t hour, int32_t minute, int32_t second );

std::string
dbStrDate();

#endif // DARKSTRING_HPP
