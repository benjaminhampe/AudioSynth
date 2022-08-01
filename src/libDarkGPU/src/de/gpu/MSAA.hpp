/// ==========================================================================
/// @file MSAA.hpp
/// @author 2018 Benjamin Hampe <benjaminhampe@gmx.de>
/// @copyright Free open source software
/// ==========================================================================

#ifndef DE_GPU_MSAA_HPP
#define DE_GPU_MSAA_HPP

#include <de/gpu/IVideoDriver.hpp>
#include <de/gpu/GLESv3.hpp>

namespace de {
namespace gpu {

// ===========================================================================
struct MSAA_Combined_DepthStencil_Target_Info
// ===========================================================================
{
   DE_CREATE_LOGGER("de.gpu.MSAA_Combined_DepthStencil_Target")
   int width;        // screen width
   int height;       // screen height
   int msaa;        // msaa ( factor to screen sizes )
   GLuint fbo;          // GL id
   Tex* texR8G8B8A8;  // GL texture id ( RGB color with A transparency, TODO: some HDR and 10bit )
   Tex* texD24S8;     // GL texture id ( not a renderbuffer, its a real tex, ergo downloadable/saveable )
   int getWidth() const { return width * msaa; }
   int getHeight() const { return height * msaa; }

   MSAA_Combined_DepthStencil_Target_Info()
      : width(0)
      , height(0)
      , msaa(0)
      , fbo(0)
      , texR8G8B8A8(nullptr)
      , texD24S8(nullptr)
   {}
};

inline Tex*
createTex( int w, int h, IVideoDriver* driver, TexFormat::EFormat eType )
{
   GLuint texId = 0;
   ::glGenTextures( 1, &texId ); GL_VALIDATE;
   if ( !texId )
   {
      DEM_ERROR("Cant create TexR8G8B8A8")
      return nullptr;
   }

   int stage = driver->bindTexture2D( texId );
   if ( stage < 0 )
   {
      DEM_ERROR("Cant bind TexR8G8B8A8")
      return nullptr;
   }

   TexInternalFormat tif;

   if ( eType == TexFormat::R8G8B8A8 )
   {
      tif.internal = GL_RGBA;
      tif.format = GL_RGBA;
      tif.type = GL_UNSIGNED_BYTE;
   }
   else if ( eType == TexFormat::R8G8B8 )
   {
      tif.internal = GL_RGB;
      tif.format = GL_RGB;
      tif.type = GL_UNSIGNED_BYTE;
   }
   else if ( eType == TexFormat::D24S8 )
   {
      tif.internal = GL_DEPTH24_STENCIL8;
      tif.format = GL_DEPTH_STENCIL;
      tif.type = GL_UNSIGNED_INT_24_8;
   }

   ::glTexImage2D( GL_TEXTURE_2D,   // GLenum target
                  0,                // GLint level
                  GLenum( tif.internal ), // GLenum internalFormat
                  GLsizei( w ),  // GLsizei width
                  GLsizei( h ), // GLsizei height
                  0,                // GLint border
                  GLenum( tif.format ), // GLenum format
                  GLenum( tif.type ),   // GLenum type
                  nullptr ); GL_VALIDATE;

   SO so;
   so.min = SO::MinLinear;
   so.mag = SO::MagLinear;
   so.wrapS = SO::ClampToEdge;
   so.wrapT = SO::ClampToEdge;
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ); GL_VALIDATE
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ); GL_VALIDATE
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); GL_VALIDATE
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); GL_VALIDATE
   //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, af ); GL_VALIDATE

   Tex* tex = new Tex( driver->getTexManager() );
   tex->m_Width = w;
   tex->m_Height = h;
   tex->m_SamplerOptions = so;
   tex->m_Format = eType;
   tex->m_TexUnit = stage;
   tex->m_TextureId = texId;
   tex->m_InternalFormat = tif;
   return tex;
}

inline Tex*
createTexR8G8B8A8( int w, int h, IVideoDriver* driver )
{
   return createTex( w, h, driver, TexFormat::R8G8B8A8 );
}

inline Tex*
createTexR8G8B8( int w, int h, IVideoDriver* driver )
{
   return createTex( w, h, driver, TexFormat::R8G8B8 );
}

inline Tex*
createTexD24S8( int w, int h, IVideoDriver* driver )
{
   return createTex( w, h, driver, TexFormat::D24S8 );
}

// ===========================================================================
inline bool
createMSAA( MSAA_Combined_DepthStencil_Target_Info & dat,
            int w, int h, int msaa, IVideoDriver* driver )
{
   if ( !driver ) {
      DEM_ERROR("No driver") return false;
   }
   if ( msaa < 1 ) {
      return true;
   }

   dat.msaa = msaa;
   dat.width = w;
   dat.height = h;

   if ( !dat.texR8G8B8A8 ) {
      dat.texR8G8B8A8 = createTexR8G8B8A8( w,h, driver );
   }
   if ( !dat.texD24S8 ) {
      dat.texD24S8 = createTexD24S8( w,h, driver );
   }
   if ( !dat.texR8G8B8A8 ) {
      DEM_ERROR("No texR8G8B8A8") return false;
   }
   if ( !dat.texD24S8 ) {
      DEM_ERROR("No texD24S8") return false;
   }

   if ( !dat.fbo ) {
      GLint fbo = 0;
      ::glGetIntegerv( GL_FRAMEBUFFER_BINDING, &fbo ); GL_VALIDATE;
      ::glGenFramebuffers( 1, &dat.fbo ); GL_VALIDATE
      ::glBindFramebuffer( GL_FRAMEBUFFER, dat.fbo ); GL_VALIDATE
      ::glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dat.texR8G8B8A8->getTextureId(), 0 ); GL_VALIDATE
      ::glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, dat.texD24S8->getTextureId(), 0 ); GL_VALIDATE
      DEM_DEBUG("Create MSAA w:",dat.getWidth(),", h:",dat.getHeight())
      GLenum status = ::glCheckFramebufferStatus( GL_FRAMEBUFFER ); // GL_READ_F, GL_DRAW_F
      bool ok = (status == GL_FRAMEBUFFER_COMPLETE);
      if (!ok)
      {
         std::string msg = "UNKNOWN_ERROR";
         switch ( status )
         {
            case GL_FRAMEBUFFER_UNDEFINED: msg = "GL_FRAMEBUFFER_UNDEFINED"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: msg = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: msg = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"; break;
            case GL_FRAMEBUFFER_UNSUPPORTED: msg = "GL_FRAMEBUFFER_UNSUPPORTED The combination of internal formats"
                  "used by attachments in the framebuffer results in a nonrenderable target."; break;
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: msg = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"; break;
            default: break;
         }
         DEM_ERROR("Incomplete MSAA ", msg )
      }
      ::glBindFramebuffer( GL_FRAMEBUFFER, fbo ); GL_VALIDATE
      if ( !driver->unbindTexture( dat.texR8G8B8A8 ) ) {
         DEM_ERROR("Cant unbind texA8R8G8B8")
      }
      if ( !driver->unbindTexture( dat.texD24S8 ) ) {
         DEM_ERROR("Cant unbind texD24S8")
      }
   }

   //::glBindFramebuffer( GL_FRAMEBUFFER, 0 ); GL_VALIDATE

   return true;
}

inline void
destroyMSAA( MSAA_Combined_DepthStencil_Target_Info & dat, IVideoDriver* driver )
{
   GLint fbo = 0;
   ::glGetIntegerv( GL_FRAMEBUFFER_BINDING, &fbo ); GL_VALIDATE;

   if ( fbo != 0 && fbo == dat.fbo ) {
      ::glBindFramebuffer( GL_FRAMEBUFFER, 0 ); GL_VALIDATE
   }

   // Unbind textures.
   if ( driver ) {
      driver->unbindTexture( dat.texR8G8B8A8 );
      driver->unbindTexture( dat.texD24S8 );
   }

   // Destroy fbo.
   if ( dat.fbo ) {
      ::glDeleteFramebuffers( 1, &dat.fbo ); GL_VALIDATE
      dat.fbo = 0;
   }

   // Destroy textures.
   if ( dat.texR8G8B8A8 ) {
      GLuint tex = dat.texR8G8B8A8->getTextureId();
      if ( tex ) {
         ::glDeleteTextures( 1, &tex );  GL_VALIDATE
      }
      delete dat.texR8G8B8A8;
      dat.texR8G8B8A8 = nullptr;
   }

   if ( dat.texD24S8 ) {
      GLuint tex = dat.texD24S8->getTextureId();
      if ( tex ) {
         ::glDeleteTextures( 1, &tex );  GL_VALIDATE
      }
      delete dat.texD24S8;
      dat.texD24S8 = nullptr;
   }
}


// ===========================================================================
inline bool
resizeMSAA( MSAA_Combined_DepthStencil_Target_Info & dat, int w, int h, int msaa, IVideoDriver* driver )
{
/*
   if ( msaa == dat.msaa && w == dat.width && h == dat.height )
   {
      return true; // nothing todo
   }

   dat.msaa = msaa;
   dat.width = w;
   dat.height = h;

   GLint fbo = 0;
   ::glGetIntegerv( GL_FRAMEBUFFER_BINDING, &fbo ); GL_VALIDATE;

   if ( fbo != 0 && fbo == dat.fbo )
   {
      ::glBindFramebuffer( GL_FRAMEBUFFER, 0 ); GL_VALIDATE
   }

   if ( !driver || !driver->bindTexture2D( dat.texR8G8B8A8 ) )
   {
      DEM_ERROR("Cant bind texA8R8G8B8")
   }

   ::glTexImage2D( GL_TEXTURE_2D,   // GLenum target
                  0,                // GLint level
                  GLenum( dat.texColorFormat.internal ), // GLenum internalFormat
                  GLsizei( dat.getWidth() ),  // GLsizei width
                  GLsizei( dat.getHeight() ), // GLsizei height
                  0,                // GLint border
                  GLenum( dat.texColorFormat.format ), // GLenum format
                  GLenum( dat.texColorFormat.type ),   // GLenum type
                  nullptr ); GL_VALIDATE;

   SamplerOptions so;
   GLES::setSamplerOptions2D( so );

   if ( !driver || !driver->bindTexture2D( dat.texD24S8 ) )
   {
      DEM_ERROR("Cant bind texD24S8")
   }

   ::glTexImage2D( GL_TEXTURE_2D,   // GLenum target
                  0,                // GLint level
                  GLenum( dat.texDSFormat.internal ), // GLenum internalFormat
                  GLsizei( dat.getWidth() ),  // GLsizei width
                  GLsizei( dat.getHeight() ), // GLsizei height
                  0,                // GLint border
                  GLenum( dat.texDSFormat.format ), // GLenum format
                  GLenum( dat.texDSFormat.type ),   // GLenum type
                  nullptr ); GL_VALIDATE;

   //GLES::setSamplerOptions2D( so );

   ::glBindFramebuffer( GL_FRAMEBUFFER, fbo ); GL_VALIDATE

   DEM_DEBUG("Resize MSAA w:",dat.getHeight(),", h:",dat.getHeight())
*/
   return true;
}

/*
// ===========================================================================
struct MSAA
// ===========================================================================
{
   DE_CREATE_LOGGER("de.gpu.MSAA")

   MSAA( IVideoDriver* driver );
   ~MSAA();
   void create( int w, int h );
   void destroy();
   void resize( int w, int h );

   IVideoDriver* m_Driver;
   GLuint m_fbo;
   GLuint m_colorTex;
   GLuint m_depthTex;
   GLuint m_stencilTex;
   int m_Width;
   int m_Height;
   SurfaceFormat m_Format;

   Tex* m_ColorTex;
   Tex* m_DepthTex;

   std::string m_Name;


   Tex* getColorAttachment( int id = 0 ) { return m_ColorTex; }
   Tex* getDepthAttachment( int id = 0 ) { return m_DepthTex; }
};


// ===========================================================================
inline bool
createMSAA( MSAA_Combined_DepthStencil_Target_Info & dat,
            int w, int h, int msaa, IVideoDriver* driver )
{
   if ( !driver )
   {
      DEM_ERROR("No driver") return false;
   }

   if ( msaa < 1 )
   {
      return true;
   }

   dat.msaa = msaa;
   dat.width = w;
   dat.height = h;
   dat.fbo = 0;
   dat.texR8G8B8A8 = createTexR8G8B8A8( w,h, driver );
   dat.texD24S8 = createTexD24S8( w,h, driver );

   GLint fbo = 0;
   ::glGetIntegerv( GL_FRAMEBUFFER_BINDING, &fbo ); GL_VALIDATE;

   float af = 4.0f; // Anisotropic filtering

   if ( !dat.texR8G8B8A8 )
   {

   }
   ::glGenTextures( 1, &dat.texR8G8B8A8 ); GL_VALIDATE;

   if ( !driver || !driver->bindTexture2D( dat.texR8G8B8A8 ) )
   {
      DEM_ERROR("Cant bind texR8G8B8A8")
   }

   //dat.texColorFormat.internal = GL_RGB;
   //dat.texColorFormat.format = GL_RGB;
   //dat.texColorFormat.type = GL_UNSIGNED_BYTE;

   ::glTexImage2D( GL_TEXTURE_2D,   // GLenum target
                  0,                // GLint level
                  GLenum( GL_RGBA ), // GLenum internalFormat
                  GLsizei( dat.getWidth() ),  // GLsizei width
                  GLsizei( dat.getHeight() ), // GLsizei height
                  0,                // GLint border
                  GLenum( GL_RGBA ), // GLenum format
                  GLenum( GL_UNSIGNED_BYTE ),   // GLenum type
                  nullptr ); GL_VALIDATE;

   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ); GL_VALIDATE
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ); GL_VALIDATE
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); GL_VALIDATE
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); GL_VALIDATE
   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, af ); GL_VALIDATE

   // Combined DepthStencil Texture
   ::glGenTextures( 1, &dat.texD24S8 ); GL_VALIDATE;

   if ( !driver || !driver->bindTexture2D( dat.texD24S8 ) )
   {
      DEM_ERROR("Cant bind texA8R8G8B8")
   }

   ::glTexImage2D( GL_TEXTURE_2D,   // GLenum target
                  0,                // GLint level
                  GLenum( GL_DEPTH24_STENCIL8 ), // GLenum internalFormat
                  GLsizei( dat.getWidth() ),  // GLsizei width
                  GLsizei( dat.getHeight() ), // GLsizei height
                  0,                // GLint border
                  GLenum( GL_DEPTH_STENCIL ), // GLenum format
                  GLenum( GL_UNSIGNED_INT_24_8 ),   // GLenum type
                  nullptr ); GL_VALIDATE;

   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ); GL_VALIDATE
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ); GL_VALIDATE
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); GL_VALIDATE
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); GL_VALIDATE
   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, af ); GL_VALIDATE

   // FBO:
   ::glGenFramebuffers( 1, &dat.fbo ); GL_VALIDATE
   ::glBindFramebuffer( GL_FRAMEBUFFER, dat.fbo ); GL_VALIDATE

   ::glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
         GL_TEXTURE_2D, dat.texR8G8B8A8, 0 ); GL_VALIDATE

   ::glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
         GL_TEXTURE_2D, dat.texD24S8, 0 ); GL_VALIDATE


   DEM_DEBUG("Create MSAA w:",dat.getHeight(),", h:",dat.getHeight())


   GLenum status = ::glCheckFramebufferStatus( GL_FRAMEBUFFER ); // GL_READ_F, GL_DRAW_F
   bool ok = (status == GL_FRAMEBUFFER_COMPLETE);
   if (!ok)
   {
      std::string msg = "UNKNOWN_ERROR";
      switch ( status )
      {
         case GL_FRAMEBUFFER_UNDEFINED: msg = "GL_FRAMEBUFFER_UNDEFINED"; break;
         case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: msg = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"; break;
         case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: msg = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"; break;
         case GL_FRAMEBUFFER_UNSUPPORTED: msg = "GL_FRAMEBUFFER_UNSUPPORTED The combination of internal formats"
               "used by attachments in the framebuffer results in a nonrenderable target."; break;
         case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: msg = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"; break;
         default: break;
      }
      DEM_ERROR("Incomplete MSAA ", msg )
   }

   ::glBindFramebuffer( GL_FRAMEBUFFER, fbo ); GL_VALIDATE

   if ( !driver || !driver->unbindTexture2D( dat.texR8G8B8A8 ) )
   {
      DEM_ERROR("Cant unbind texA8R8G8B8")
   }
   if ( !driver || !driver->unbindTexture2D( dat.texD24S8 ) )
   {
      DEM_ERROR("Cant unbind texD24S8")
   }

   //::glBindFramebuffer( GL_FRAMEBUFFER, 0 ); GL_VALIDATE

   return true;
}
*/

} // end namespace gpu.
} // end namespace de.

#endif

