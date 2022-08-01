#ifndef DB_GET_COVERART_HPP
#define DB_GET_COVERART_HPP

#include <sstream>
#include <cstdint>
#include <DarkImage.hpp>

bool 
dbGetCoverArt( de::Image & img, std::string uri, int videoStreamIndex = -1 );


#endif

