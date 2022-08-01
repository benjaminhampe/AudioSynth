/// ==========================================================================
/// @file DarkEngineTypes.hpp
/// @brief Base header used by most other DarkEngine headers
/// @author 2018 Benjamin Hampe <benjaminhampe@gmx.de>
/// @copyright Free open source software
/// ==========================================================================

#ifndef DE_EGL_VALIDATE_HPP
#define DE_EGL_VALIDATE_HPP

#include <de/opengles3.hpp>
#include <sstream>

// ===========================================================================
// EGL_Validate
// ===========================================================================
std::string
eglGetErrorString( EGLint errorCode );

std::string
eglGetErrorString();

void
eglValidate( EGLBoolean status, char const * callerName );

void
eglValidate( char const * file, int line, char const * func );

#ifndef EGL_VALIDATE
#define EGL_VALIDATE ::eglValidate( __FILE__, __LINE__, __FUNCTION__ );
#endif

#endif

