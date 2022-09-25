// HtmlWriter.hpp
// Copyright (C) 2002-2012 Benjamin Hampe
// allowed for free use

#ifndef DE_CAPTURE_SCREEN_WIN32API_HPP
#define DE_CAPTURE_SCREEN_WIN32API_HPP

#include <cstdint>
#include <sstream>
#include <fstream>

#include <DarkImage.hpp>

namespace de {

   struct CaptureScreen
   {
      static de::Image
      createImageFromDesktop();

      static void
      copyBitmapToClipboard();
   };

} // end namespace de.

#endif
