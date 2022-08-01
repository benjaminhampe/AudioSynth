#ifndef BENNI_GPU_COMPILECONFIG_HPP
#define BENNI_GPU_COMPILECONFIG_HPP

#include <DarkImage.hpp>
#include <de_glm.hpp>
#include <tinyxml2/tinyxml2.h>
#include <sstream>
#include <vector>

// #ifndef USE_LIB_DARKFONT
// #define USE_LIB_DARKFONT
// #endif

// #ifndef USE_LIB_DARKIMAGE
// #define USE_LIB_DARKIMAGE
// #endif

// #ifndef USE_LIB_ASSIMP
// #define USE_LIB_ASSIMP
// #endif

#ifndef r_cast
#define r_cast reinterpret_cast
#endif

#ifndef c_cast
#define c_cast const_cast
#endif

#ifndef s_cast
#define s_cast static_cast
#endif

#endif

