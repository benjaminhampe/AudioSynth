// console_rgb.hpp
// Copyright (C) 2002-2012 Benjamin Hampe
// allowed for free use

#ifndef DE_CONSOLE_RGB_HPP
#define DE_CONSOLE_RGB_HPP

#include <cstdint>
#include <sstream>
#include <fstream>
#include <DarkImage.hpp>

namespace de {

   struct console
   {
      static void
      rgb( int r, int g, int b );
   };

} // end namespace de.

#endif
