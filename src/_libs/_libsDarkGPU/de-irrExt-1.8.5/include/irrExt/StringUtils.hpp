#ifndef DE_ALPHASONIC_COMMON_STRING_UTILS_HPP
#define DE_ALPHASONIC_COMMON_STRING_UTILS_HPP

#include <irrExt/_common.hpp>
#include <iostream>

// Move to system
int64_t getPerfTimer();
int64_t getPerfTimerFrequency();
std::string printTimer();
std::string printDate();
std::string printSystemTime();
std::string printLocalTime();

//struct GregorianDate
//{
//    int32_t year;
//    int32_t month;
//    int32_t day;
//};

//struct GregorianTime
//{
//    int32_t hour;
//    int32_t minute;
//    int32_t second;
//    int32_t microsecond;
//};

//struct JulianDays
//{
//    float64_t jd;
//};


inline void
XML_writeHeader( std::stringstream & out )
{
    out << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
}

inline std::string
XML_ATTRIBUTE( std::string key, std::string value )
{
    std::stringstream s;
    s << ' ' << key << '=' << '"' << value << '"';
    return s.str();
}

inline int
XML_findAttribute( irr::io::IXMLReaderUTF8 * xml, const char* attribName )
{
    int const count = xml->getAttributeCount();

    for ( int i = 0; i < count; ++i )
    {
        std::string const tmp = xml->getAttributeName( i );

        if ( tmp == attribName )
        {
            std::cout << __FUNCTION__ << "(" << attribName << ") :: Found attribute[" << i << "]\n";
            return i;
        }
    }

    std::cout << __FUNCTION__ << "(" << attribName << ") :: Cannot find attribute.\n";

    return -1;
}


inline bool
XML_hasAttribute( irr::io::IXMLReaderUTF8 * xml, const char* attribName )
{
    if ( XML_findAttribute( xml, attribName ) != -1 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

#endif // DE_ALPHASONIC_COMMON_STRING_UTILS_HPP
