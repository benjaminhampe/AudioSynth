/// ==========================================================================
/// @file DarkEngineTypes.hpp
/// @brief Base header used by most other DarkEngine headers
/// @author 2018 Benjamin Hampe <benjaminhampe@gmx.de>
/// @copyright Free open source software
/// ==========================================================================

#ifndef DE_OPENGLES_3X_COMPILECONFIG_HPP
#define DE_OPENGLES_3X_COMPILECONFIG_HPP

#include <cstdint>
#include <sstream>
#include <vector>

// 0 = Desktop OpenGL, 1 = embedded GLES3x

// #ifndef USE_OPENGLES_AND_EGL
// #define USE_OPENGLES_AND_EGL 0
// #endif

#ifdef USE_OPENGLES_AND_EGL

   #ifndef GL_EXT_PROTOTYPES
   #define GL_EXT_PROTOTYPES 1
   #endif

   // KHR - Khronos Platform Types ( for desktop and embedded systems )
   #include <KHR/khrplatform.h>
   // EGL ( for desktop and embedded systems )
   #include <EGL/egl.h>
   #include <EGL/eglext.h>
   #include <EGL/eglplatform.h>
   // OpenGL ES 1.x ( for desktop and embedded systems )
   //#include <GLES/egl.h>
   //#include <GLES/gl.h>
   //#include <GLES/glext.h>
   //#include <GLES/glplatform.h>
   // OpenGL ES 2.x ( for desktop and embedded systems )
   //#include <GLES2/gl2.h>
   //#include <GLES2/gl2ext.h>
   //#include <GLES2/gl2platform.h>
   // OpenGL ES 3.x ( for desktop and embedded systems )
   //#include <GLES3/gl3.h>
   //#include <GLES3/gl3platform.h>
   #include <GLES3/gl31.h>
   //#include <GLES3/gl32.h>
#else
   #ifndef GL_GLEXT_PROTOTYPES
   #define GL_GLEXT_PROTOTYPES 1
   #endif

   #include <GL/gl.h>
   #include <GL/glu.h>
   #include <GL/glext.h>
//   #include <GL/glcorearb.h>
   //#include <GL/glplatform.h>
#endif

#ifndef GL_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#endif

#ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#endif

#endif

