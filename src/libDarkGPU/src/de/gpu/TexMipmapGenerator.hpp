/// ==========================================================================
/// @file GLES.hpp
/// @author 2018 Benjamin Hampe <benjaminhampe@gmx.de>
/// @copyright Free open source software
/// ==========================================================================

#ifndef DE_GPU_TEX_MIPMAPMAKER_HPP
#define DE_GPU_TEX_MIPMAPMAKER_HPP

#include <de/gpu/GLESv3.hpp>

namespace de {
namespace gpu {

struct TexMipMapper
{
   DE_CREATE_LOGGER("de.gpu.TexMipMapper")

   GLuint m_ReadFBO;
   GLuint m_DrawFBO;

   TexMipMapper()
      : m_ReadFBO( 0 )
      , m_DrawFBO( 0 )
   {}

   ~TexMipMapper()
   {
      close();
   }

   inline void init()
   {
      if ( !m_ReadFBO )
      {
         glGenFramebuffers( 1, &m_ReadFBO ); GL_VALIDATE
      }
      if ( !m_DrawFBO )
      {
         glGenFramebuffers( 1, &m_DrawFBO ); GL_VALIDATE
      }
   }

   void close()
   {
      if ( m_ReadFBO )
      {
         glDeleteFramebuffers( 1, &m_ReadFBO ); GL_VALIDATE
         m_ReadFBO = 0;
      }
      if ( m_DrawFBO )
      {
         glDeleteFramebuffers( 1, &m_DrawFBO ); GL_VALIDATE
         m_DrawFBO = 0;
      }
   }

   /// @brief Creates all mipmaps ( resized texture ) for all levels determined by ITexture->getMaxMipmapLevel() if they are not up-to-date (already generated and the data was not changed)
   ///        and the texture is bound to a texture unit.
   /// @param[in] textureBase The texture, for which the mipmaps should be generated.
   /// @param[in] singleLayer Layer for which mipmaps need to be generated, or -1 to generate for all array texture layers

   bool
   createMipmaps( Tex* tex, int specificDepthLayer = -1 )
   {
      if ( !tex ) return true; // Nothing was done very successfully.

      int texId = tex->getTextureId();
      if ( !texId )
      {
         DE_ERROR("No texId ", tex->toString() )
         return false;
      }

      init();
      GLint oldReadFBO = 0;
      GLint oldDrawFBO = 0;
      glGetIntegerv( GL_READ_FRAMEBUFFER_BINDING, &oldReadFBO );
      glGetIntegerv( GL_DRAW_FRAMEBUFFER_BINDING, &oldDrawFBO );
      glBindFramebuffer( GL_READ_FRAMEBUFFER, m_ReadFBO ); GL_VALIDATE
      glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_DrawFBO ); GL_VALIDATE

      int w = tex->getWidth();
      int h = tex->getHeight();
      int w2 = w >> 1; // Maybe your mipmaps do look wrong because your tex is not a multiple of 2.
      int h2 = h >> 1; // Maybe your mipmaps do look wrong because your tex is not a multiple of 2.

      // For Tex3D, Tex2DArray, TexCube?
      if ( tex->getDepth() > 0 )
      {
         int depthS = 0;
         int depthE = tex->getDepth();
         if ( specificDepthLayer > -1 )
         {
            depthS = depthE = specificDepthLayer;
         }

         for ( int i = 1; i <= tex->getMaxMipLevel(); ++i )
         {
            for ( int d = depthS; d <= depthE; ++d )
            {
               glFramebufferTextureLayer( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texId, i - 1, d );
               glFramebufferTextureLayer( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texId, i, d );
               glBlitFramebuffer( 0, 0, w, h, 0, 0, w2, h2, GL_COLOR_BUFFER_BIT, GL_LINEAR );
            }
            w = w2;
            h = h2;
            w2 >>= 1;
            h2 >>= 1;
         }
      }
      // For Tex2D
      else
      {
         if ( tex->getType() != TexType::Tex2D )
         {
            for ( int i = 1; i <= tex->getMaxMipLevel(); ++i )
            {
               glFramebufferTexture2D( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, i - 1 );
               glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, i );
               glBlitFramebuffer( 0, 0, w, h, 0, 0, w2, h2, GL_COLOR_BUFFER_BIT, GL_LINEAR );
               w = w2;
               h = h2;
               w2 >>= 1;
               h2 >>= 1;
            }

            return true;
         }
         else
         {
            DE_ERROR("Unsupported texType ", tex->toString() )
            return false;
         }
      }
   }
};

} // end namespace gpu.
} // end namespace de.

#endif
