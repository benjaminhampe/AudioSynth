/// ==========================================================================
/// @file DarkEngineTypes.hpp
/// @brief Base header used by most other DarkEngine headers
/// @author 2018 Benjamin Hampe <benjaminhampe@gmx.de>
/// @copyright Free open source software
/// ==========================================================================

#ifndef DE_OPENGL_VALIDATE_HPP
#define DE_OPENGL_VALIDATE_HPP

#include <de/opengles3.hpp>
#include <sstream>
// ===========================================================================
// GL_Validate
// ===========================================================================

std::string
glGetErrorString( GLenum errorCode );

bool
glValidate( char const * file, int line, char const * func );

#ifndef GL_VALIDATE
#define GL_VALIDATE ::glValidate( __FILE__, __LINE__, __FUNCTION__ );
#endif


#ifndef GL_VALIDATE_RETURN
#define GL_VALIDATE_RETURN(retVal) { \
   bool ok = ::glValidate( __FILE__, __LINE__, __FUNCTION__ ); \
   if ( !ok ) { return (retVal); } }
#endif

#ifndef GL_VALIDATE_RETURN_VOID
#define GL_VALIDATE_RETURN_VOID { \
   bool ok = ::glValidate( __FILE__, __LINE__, __FUNCTION__ ); \
   if ( !ok ) { return; } }
#endif

// ===========================================================================
// GL_Getter
// ===========================================================================

bool
glIsEnabledDE( GLenum value );

int32_t
glGetIntegerDE( GLenum value );

bool
glGetBoolDE( uint32_t query );

// ==========================================================================
// glHasExtension()
// ==========================================================================

bool
glHasExtension( std::string const & searchToken );

// ==========================================================================
// glGetVendor|Version|Renderer|ExtensionString()
// ==========================================================================

std::string
glGetVendorString();

std::string
glGetRendererString();

std::string
glGetVersionString();

std::string
glGetExtensionString();

std::string
glGetShaderLangVersionString();

// ==========================================================================
// print to std::cout
// ==========================================================================

void
glPrintVendor();

#endif

