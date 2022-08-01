#include <de/EGL_Validate.hpp>
#include <iostream>

// ===========================================================================
// EGL_Validate
// ===========================================================================

std::string
eglGetErrorString( EGLint errorCode )
{
   switch( errorCode )
   {
      case EGL_NOT_INITIALIZED: return "EGL_NOT_INITIALIZED"; // 0x3001
      case EGL_BAD_ACCESS: return "EGL_BAD_ACCESS"; // 0x3002
      case EGL_BAD_ALLOC: return "EGL_BAD_ALLOC";// 0x3003
      case EGL_BAD_ATTRIBUTE: return "EGL_BAD_ATTRIBUTE";// 0x3004
      case EGL_BAD_CONFIG: return "EGL_BAD_CONFIG";// 0x3005
      case EGL_BAD_CONTEXT: return "EGL_BAD_CONTEXT";// 0x3006
      case EGL_BAD_CURRENT_SURFACE: return "EGL_BAD_CURRENT_SURFACE";// 0x3007
      case EGL_BAD_DISPLAY: return "EGL_BAD_DISPLAY"; // 0x3008
      case EGL_BAD_MATCH: return "EGL_BAD_MATCH";     // 0x3009
      case EGL_BAD_NATIVE_PIXMAP: return "EGL_BAD_NATIVE_PIXMAP";// 0x300A
      case EGL_BAD_NATIVE_WINDOW: return "EGL_BAD_NATIVE_WINDOW";// 0x300B
      case EGL_BAD_PARAMETER: return "EGL_BAD_PARAMETER";// 0x300C
      case EGL_BAD_SURFACE: return "EGL_BAD_SURFACE";// 0x300D
      default: return "Unknown EGL Error, if any";
   }
}

std::string
eglGetErrorString()
{
   EGLint errorCode = eglGetError();
   return ::eglGetErrorString( errorCode );
}

void
eglValidate( EGLBoolean status, char const * callerName )
{
   if ( status == EGL_FALSE )
   {
      EGLint err = ::eglGetError();
      std::string msg = ::eglGetErrorString( err );
      std::stringstream s;
      s << "[Error] " << callerName << " :: [EGL] Got errorCode(" << std::hex << err << "), message(" << msg << ")";
      std::cout << s.str() << std::endl;
   }
}

void
eglValidate( char const * file, int line, char const * func )
{
   EGLint err = ::eglGetError();
   if ( err != EGL_SUCCESS )
   {
      std::string msg = ::eglGetErrorString( err );
      std::stringstream s;
      s << "[Error] " << file << ":" << line << " " << func << " :: [EGL] "
        "Got errorCode(" << std::hex << err << "), message(" << msg << ")\n";
      std::cout << s.str();
   }
}
