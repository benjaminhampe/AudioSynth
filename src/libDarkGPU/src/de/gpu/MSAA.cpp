#include <de/gpu/MSAA.hpp>

namespace de {
namespace gpu {

/*
MSAA::MSAA( IVideoDriver* driver )
   : m_Driver( driver )
   , m_fbo( 0 )
   , m_colorTex( 0 )
   , m_depthTex( 0 )
   , m_stencilTex( 0 )
   , m_Width( 0 )
   , m_Height( 0 )
   , m_ColorTex( nullptr )
   , m_DepthTex( nullptr )
{

}

MSAA::~MSAA()
{
   destroy();
}

void
MSAA::create( int w, int h )
{
   DE_DEBUG("name:",m_Name,", w:",w,", h:",h)

   m_ColorTex = new Tex( m_Driver, w, h, SurfaceFormat(8,8,8,8), SO() );
   m_DepthTex = new Tex( m_Driver, w, h, SurfaceFormat(0,0,0,0,24,8,true), SO() );

   m_fbo = 0;
   ::glGenFramebuffers( 1, &m_fbo ); GL_VALIDATE
   ::glBindFramebuffer( GL_FRAMEBUFFER, m_fbo ); GL_VALIDATE

   //m_Driver->activateTexture( m_ColorTex );
   ::glBindTexture( GL_TEXTURE_2D, m_ColorTex->getTextureId() ); GL_VALIDATE
   ::glFramebufferTexture2D(
         GL_FRAMEBUFFER,
         GL_COLOR_ATTACHMENT0,
         GL_TEXTURE_2D,
         m_ColorTex->getTextureId(), 0 ); GL_VALIDATE

   if ( m_DepthTex->isRenderBuffer() )
   {
      ::glFramebufferRenderbuffer(
         GL_FRAMEBUFFER,
         GL_DEPTH_ATTACHMENT,
         GL_RENDERBUFFER,
         m_DepthTex->getTextureId() ); GL_VALIDATE
   }
   else
   {
      //m_Driver->activateTexture( m_DepthTex );
      ::glBindTexture( GL_TEXTURE_2D, m_DepthTex->getTextureId() ); GL_VALIDATE
      ::glFramebufferTexture2D(
         GL_FRAMEBUFFER,
         GL_DEPTH_ATTACHMENT,
         GL_TEXTURE_2D,
         m_DepthTex->getTextureId(),
         0 ); GL_VALIDATE
   }

   GLenum status = ::glCheckFramebufferStatus( GL_FRAMEBUFFER ); // GL_READ_F, GL_DRAW_F
   if ( status != GL_FRAMEBUFFER_COMPLETE )
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
      DE_ERROR("Incomplete MSAA ", msg )
   }

   ::glBindFramebuffer( GL_FRAMEBUFFER, 0 ); GL_VALIDATE
   ::glBindTexture( GL_TEXTURE_2D, 0 ); GL_VALIDATE
}

void
MSAA::destroy()
{
   if ( !m_fbo )
   {
      return;
   }

   ::glBindFramebuffer( GL_FRAMEBUFFER, 0 );  GL_VALIDATE
   ::glDeleteFramebuffers( 1, &m_fbo ); GL_VALIDATE
   m_fbo = 0;

   SAFE_DELETE(m_ColorTex);
   SAFE_DELETE(m_DepthTex);
}

void
MSAA::resize( int w, int h )
{
   DE_DEBUG("w:",w,", h:",h)

   m_fbo = 0;
   if ( m_ColorTex )
   {
      m_ColorTex->resize( w, h );
   }

   if ( m_DepthTex )
   {
      m_DepthTex->resize( w, h );
   }
}
*/

} // end namespace gpu.
} // end namespace de.
