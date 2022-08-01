#include <de/GL_Validate.hpp>
#include <iostream>

// ===========================================================================
// GL_Validate
// ===========================================================================

std::string
glGetErrorString( GLenum errorCode )
{
   switch ( errorCode )
   {
   case GL_NO_ERROR:       return "GL_NO_ERROR";
   case GL_OUT_OF_MEMORY:  return "GL_OUT_OF_MEMORY";
   case GL_INVALID_ENUM:   return "GL_INVALID_ENUM";
   case GL_INVALID_VALUE:  return "GL_INVALID_VALUE";
   case GL_INVALID_OPERATION:  return "GL_INVALID_OPERATION";
   case GL_INVALID_FRAMEBUFFER_OPERATION:  return "GL_INVALID_FRAMEBUFFER_OPERATION";
   //case GL_INVALID_FRAMEBUFFER_OPERATION_OES:  return "GL_INVALID_FRAMEBUFFER_OPERATION_OES";
   default:                return "GL_UNKNOWN_ERROR";
   }
}

bool
glValidate( char const * file, int line, char const * func )
{
   bool ok = true;
#if 1
   GLenum err = ::glGetError();
   while( err != 0 )
   {
      ok = false;
      std::string msg = glGetErrorString( err );
      std::stringstream s;
      s << "[Error] " << file << ":" << line << " " << func << " :: [GL] "
        "Got errorCode(" << err << "), message(" << msg << ")\n";
      std::cout << s.str();
      err = ::glGetError();
   }
#endif
   return ok;
}

// ===========================================================================
// GL_Getter
// ===========================================================================

bool
glIsEnabledDE( GLenum value )
{
   bool hasMe = (GL_FALSE != ::glIsEnabled( value ));
   GL_VALIDATE
   return hasMe;
}

int32_t
glGetIntegerDE( GLenum value )
{
   GLint i = 0;
   ::glGetIntegerv( value, &i );
   bool ok = GL_VALIDATE
   if (!ok)
   {
      std::cout << "Invalid GLenum value(" << i << ")" << std::endl;
   }
   return i;
}

bool
glGetBoolDE( uint32_t query )
{
   GLboolean value = 0;
   ::glGetBooleanv( query, &value );
   GL_VALIDATE
   return ( GL_TRUE == value );
}

/// ==========================================================================
/// ===  glHasExtension()
/// ==========================================================================

bool
glHasExtension( std::string const & searchToken )
{
   std::string const s { (char const *)::glGetString( GL_EXTENSIONS ) };

   if ( s.find( searchToken ) != std::string::npos )
   {
      return true;
   }
   else
   {
      return false;
   }
}

/// ==========================================================================
/// ===  glPrintVendor()
/// ==========================================================================

std::string
glGetVendorString()
{
   std::stringstream s; s << glGetString( GL_VENDOR ); return s.str();
}

std::string
glGetRendererString()
{
   std::stringstream s; s << glGetString( GL_RENDERER ); return s.str();
}

std::string
glGetVersionString()
{
   std::stringstream s; s << glGetString( GL_VERSION ); return s.str();
}

std::string
glGetExtensionString()
{
   std::stringstream s; s << glGetString( GL_EXTENSIONS ); return s.str();
}

std::string
glGetShaderLangVersionString()
{
   std::stringstream s; s << glGetString( GL_SHADING_LANGUAGE_VERSION ); return s.str();
}

void
glPrintVendor()
{
   std::cout << "GL_VENDOR = " << glGetString( GL_VENDOR ) << "\n";
   std::cout << "GL_VERSION = " << glGetString( GL_VERSION ) << "\n";
   std::cout << "GL_SHADING_LANGUAGE_VERSION = " << glGetString( GL_SHADING_LANGUAGE_VERSION ) << "\n";
   std::cout << "GL_EXTENSIONS = " << glGetString( GL_EXTENSIONS ) << "\n";
   std::cout << "glHasExtension( GL_EXT_gpu_shader4 ) = " << glHasExtension( "GL_EXT_gpu_shader4" ) << "\n";
   std::cout << "glHasExtension( GL_EXT_gpu_shader5 ) = " << glHasExtension( "GL_EXT_gpu_shader5" ) << "\n";
}
