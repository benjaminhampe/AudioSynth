/// ==========================================================================
/// @file EGL.hpp
/// @author 2018 Benjamin Hampe <benjaminhampe@gmx.de>
/// @copyright Free open source software
/// ==========================================================================

#ifndef DE_GPU_EGL_HPP
#define DE_GPU_EGL_HPP

#include <de/gpu/Tex.hpp>
#include <de_opengl.hpp>

#ifdef USE_OPENGLES_AND_EGL

namespace de {
namespace gpu {

// ===========================================================================
// EGL
// ===========================================================================
struct EGL
{
   DE_CREATE_LOGGER("de.gpu.EGL")
// ===========================================================================
// Helper
// ===========================================================================
   static std::vector< EGLint >
   toSurfaceFormatAttribVector( SurfaceFormat const & fmt );
   static std::vector< EGLint >
   toWindowAttribVector( bool offscreen );
   static std::vector< EGLint >
   toContextAttribVector();
//   static EGLint
//   getConfigId( EGLDisplay display, EGLConfig config );
//   static std::string
//   getConfigString( EGLDisplay display, EGLConfig config );
// ===========================================================================
// Displays
// ===========================================================================
   static bool
   makeCurrent( EGLDisplay display, EGLSurface surface, EGLContext context );
   static EGLDisplay
   createDisplay( bool debug = false );
   static bool
   destroyDisplay( EGLDisplay & display, bool debug = false );
// ===========================================================================
// DisplayConfigs with SurfaceFormat
// ===========================================================================
   static SurfaceFormat
   getSurfaceFormat( EGLDisplay display, EGLConfig config );
   static std::vector< EGLConfig >
   setSurfaceFormat( EGLDisplay display, SurfaceFormat fmt );
   static std::vector< EGLConfig >
   getConfigVector( EGLDisplay display, SurfaceFormat const & fmt );
   static EGLint
   getConfigCount( EGLDisplay display ); // xxx
   static std::vector< EGLConfig >
   getConfigVector( EGLDisplay display );
   static EGLint
   getConfigValue( EGLDisplay display, EGLConfig config, EGLint attr );
   static EGLint
   getConfigId( EGLDisplay display, EGLConfig config );
   static std::string
   getConfigString( EGLDisplay display, EGLConfig config );
// ===========================================================================
// Contexts
// ===========================================================================
   static EGLContext
   createContext( EGLDisplay display, EGLSurface surface, EGLConfig selected );
   static bool
   destroyContext( EGLDisplay display, EGLContext & context );
// ===========================================================================
// Surface
// ===========================================================================
   static bool
   destroySurface( EGLDisplay display, EGLSurface & surface );

   static EGLSurface
   createWindowSurface( EGLDisplay & display,
                        int32_t w,
                        int32_t h,
                        uint64_t nativeHandle,
                        SurfaceFormat fmt,
                        EGLConfig * outSelectedConfig = nullptr, bool debug = false );

   static EGLSurface
   createPBufferSurface( EGLDisplay & display,
                         int32_t w,
                         int32_t h,
                         SurfaceFormat fmt,
                         EGLConfig * outSelectedConfig = nullptr, bool debug = false );
}; // end class Utils.

} // end namespace gpu.
} // end namespace de.

#endif // USE_OPENGLES_AND_EGL

#endif

