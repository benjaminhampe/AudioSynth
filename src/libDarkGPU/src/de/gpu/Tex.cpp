#include <de/gpu/Tex.hpp>
#include <de/gpu/IVideoDriver.hpp>

namespace de {
namespace gpu {

// ===========================================================================
SO::SO()
// ===========================================================================
   : mag( MagLinear )
   , min( MinMipmapLinearLinear )
   , wrapS( RepeatMirrored ), wrapT( RepeatMirrored )
   , anisotropicFilter( 4 )
{}

SO::SO( EMagFilter u_mag, EMinFilter u_min, EWrap s_wrap, EWrap t_wrap )
   : mag( u_mag )
   , min( u_min )
   , wrapS( s_wrap )
   , wrapT( t_wrap )
   , anisotropicFilter( 4 )
{}

SO::SO( SO const & o )
   : mag( o.mag )
   , min( o.min )
   , wrapS( o.wrapS )
   , wrapT( o.wrapT )
   , anisotropicFilter( o.anisotropicFilter )
{}

//   SO &
//   operator=( SO const & o )

bool
SO::hasMipmaps() const
{
   if ( min == MinNearest ) return false;
   if ( min == MinLinear ) return false;
   return true;
}

std::string
SO::toString() const
{
   std::stringstream s;
   s << int(min) << "," << int(mag) << "," << int(wrapS) << "," << int(wrapT);
   return s.str();
}

// ===========================================================================
Tex::Tex( TexManager* driver )
// ===========================================================================
   : m_Driver( driver )
   , m_Type( TexType::Tex2D )
   , m_Format( TexFormat::R8G8B8A8 )
   , m_Width( 0 )
   , m_Height( 0 )
   , m_Depth( 0 )
   , m_MaxMipLevel( 4 )
   , m_SamplerOptions()
   , m_TexUnit( -1 )
   , m_TextureId( 0 )
   , m_IsRenderBuffer( false )
   , m_KeepImage( true )
   , m_Name()
   , m_Uri()
   , m_Image()
{
}

Tex::~Tex()
{
   if ( m_Driver )
   {
      m_Driver->removeTexture( getName() );
   }
}

std::string
Tex::toString() const
{
   std::ostringstream s;
   s  << "w:" << getWidth() << ", "
      << "h:" << getHeight()   << ", "
      << "id:" << m_TextureId << ", "
      << "u:" << m_TexUnit << ", "
      << m_Name;
   return s.str();
}


// ===========================================================================
TexRef::TexRef()
   : texWidth(0)
   , texHeight(0)
   , tex(nullptr)
   , rect( 0,0,0,0 )
   , coords( 0,0,1,1 )
   , repeat( 1,1 )
{}

TexRef::TexRef( Tex* ptr )
   : texWidth(0)
   , texHeight(0)
   , tex( ptr )
   , rect( 0,0,0,0 )
   , coords( 0,0,1,1 )
   , repeat( 1,1 )
{
   if ( tex )
   {
      texWidth = tex->getWidth();
      texHeight = tex->getHeight();
      rect = Recti( 0, 0, texWidth, texHeight );
      coords = Rectf::fromRecti( rect, texWidth, texHeight );
   }
}

TexRef::TexRef( Tex* ptr, Recti const & pos )
   : texWidth(0)
   , texHeight(0)
   , tex( ptr )
   , rect( pos )
   , coords( 0,0,1,1 )
   , repeat( 1,1 )
{
   if ( tex )
   {
      texWidth = tex->getWidth();
      texHeight = tex->getHeight();
      coords = Rectf::fromRecti( rect, texWidth, texHeight );
   }
}

std::string
TexRef::toString() const
{
   std::string msg = "0";
   if (tex) msg = tex->toString();
   std::ostringstream s;
   s << "rect(" << getRect() << "), tex(" << msg << ")";
   return s.str();
}

bool
TexRef::empty() const
{
   if ( !tex ) return true;
   if ( getWidth() < 1 ) return true;
   if ( getHeight() < 1 ) return true;
   return false;
}

/*
Image
TexRef::copyImage() const
{
   uint32_t w = ref.rect.getWidth();
   uint32_t h = ref.rect.getHeight();
   uint32_t x = ref.rect.getX();
   uint32_t y = ref.rect.getY();
   DE_DEBUG("id:",id, ", w:",w, ", h:",h, ", x:",x, ", y:",y, ", img:", ref.img->toString())

   Image img( w,h );
   for ( uint32_t j = 0; j < h; ++j )
   {
   for ( uint32_t i = 0; i < w; ++i )
   {
      img.setPixel( i,j, ref.img->getPixel(x+i,y+j) );
   }
   }

   return img;
}
*/

void
TexRef::setMatrix( float tx, float ty, float sx, float sy )
{
   coords = Rectf( tx,ty,sx,sy );
   rect = coords.toRecti( texWidth, texHeight );
}

// =======================================================================
/*
Tex2D::Tex2D( IVideoDriver* driver, int w, int h, SurfaceFormat const & fmt, SamplerOptions const & so )
   : ITexture()
   , m_Driver( driver )
{
   GLuint texId;

   if ( fmt.isRenderBuffer() )
   {
      ::glGenTextures( 1, &texId ); GL_VALIDATE;
   }
   else
   {
      ::glGenRenderbuffers( 1, &texId ); GL_VALIDATE;
   }


   m_TextureId = texId;
   m_Type = ITexture::Tex2D;
   m_Width = 0;
   m_Height = 0;

   if ( fmt.r > 0 || fmt.g > 0 || fmt.b > 0 )
   {
      if ( fmt.a > 0 )
      {
         m_InternalFormat = GL_RGBA;
         m_InternalType = GL_UNSIGNED_BYTE;
      }
      else
      {
         m_InternalFormat.internal = GL_RGB;
         m_InternalFormat.format = GL_RGB;
         m_InternalFormat.type = GL_UNSIGNED_BYTE;
      }
   }
   else if ( fmt.d == 24 && fmt.s == 8 )
   {
      m_InternalFormat.internal = GL_DEPTH24_STENCIL8;
      m_InternalFormat.format = GL_DEPTH_STENCIL;
      m_InternalFormat.type = GL_UNSIGNED_INT_24_8;
   }
   else
   {
      throw std::runtime_error( dbStrJoin("Unsupported tex format ", fmt.toString()) );
   }

   m_Format = fmt;
   m_SamplerOptions = so;
   resize( w, h );
}

Tex2D::~Tex2D()
{
   destroy();
}

void
Tex2D::destroy()
{
   if ( !m_TextureId )
   {
      return;
   }

   if ( isRenderBuffer() )
   {
      DE_DEBUG("Delete rbo ", m_TextureId)
      ::glDeleteRenderbuffers( 1, &m_TextureId ); GL_VALIDATE
   }
   else
   {
      DE_DEBUG("Delete tex2D ", m_TextureId)
      m_Driver->deactivateTexture( this );
      ::glDeleteTextures( 1, &m_TextureId ); GL_VALIDATE
   }

   m_TextureId = 0;
}

void
Tex2D::resize( int w, int h )
{
   if ( m_Driver->activateTexture( this ) )
   {
      DE_ERROR("Cant bind texture ", toString() )
      return;
   }

   GLES::setSamplerOptions2D( m_SamplerOptions );

   m_Width = w;
   m_Height = h;

   if ( isRenderBuffer() && m_Format.d > 0 )
   {
      ::glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h ); GL_VALIDATE
   }
   else
   {
      if ( getType() == ITexture::Tex2D )
      {
         ::glTexImage2D( GL_TEXTURE_2D,   // GLenum target
                        0,                // GLint level
                        GLenum( m_InternalFormat ), // GLenum internalFormat
                        GLsizei( m_Width ),  // GLsizei width
                        GLsizei( m_Height ), // GLsizei height
                        0,                // GLint border
                        GLenum( m_InternalFormat ), // GLenum format
                        GLenum( m_InternalType ),   // GLenum type
                        nullptr );

         bool ok = GL_VALIDATE;
         if ( !ok )
         {
            DE_ERROR("Cant upload" )
            return;
         }

         // requires mipmaps. Generate them automatically.
         if ( getSamplerOptions().hasMipmaps() )
         {
            ::glGenerateMipmap( GL_TEXTURE_2D ); GL_VALIDATE
         }

         ::glBindTexture( GL_TEXTURE_2D, 0 ); GL_VALIDATE
      }
      else
      {
         throw std::runtime_error("Invalid texture type");
      }

   }

}


bool
Tex2D::upload( Image const & img )
{
   if ( m_TextureId == 0 )
   {
      DE_ERROR("No GPU handle" )
      return false;
   }

   auto const & fmt = img.getFormat();
   if ( fmt != ColorFormat::RGBA8888 &&
        fmt != ColorFormat::RGB888 )
   {
      DE_ERROR("Unsupported image format ", img.toString() )
   }

   //::glActiveTexture( GL_TEXTURE0 ); GL_VALIDATE
   ::glBindTexture( GL_TEXTURE_2D, m_TextureId );
   bool ok = GL_VALIDATE;
   if ( !ok )
   {
      DE_ERROR("Cant bind m_TextureId(", m_TextureId, ")" )
      return m_TextureId;
   }

   if ( fmt == ColorFormat::RGB888 )
   {
      m_InternalFormat.internal = GL_RGBA;
      m_InternalFormat.format = GL_RGBA;
      m_InternalFormat.type = GL_UNSIGNED_BYTE;
   }
   else
   {
      m_InternalFormat.internal = GL_RGB;
      m_InternalFormat.format = GL_RGB;
      m_InternalFormat.type = GL_UNSIGNED_BYTE;
   }

   // Poor filtering, or ...
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); GL_VALIDATE
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); GL_VALIDATE

   GLES::setSamplerOptions2D( getSamplerOptions() );

   void const* p_data = nullptr;

   if ( !img.empty() )
   {
      p_data = img.readPtr< void >();
      m_Width = img.getWidth();
      m_Height = img.getHeight();
   }

   // Give the image to OpenGL
   ::glTexImage2D( GL_TEXTURE_2D,   // GLenum target
                  0,                // GLint level
                  GLenum( m_InternalFormat.internal ), // GLenum internalFormat
                  GLsizei( m_Width ),  // GLsizei width
                  GLsizei( m_Height ), // GLsizei height
                  0,                // GLint border
                  GLenum( m_InternalFormat.format ), // GLenum format
                  GLenum( m_InternalFormat.type ),   // GLenum type
                  p_data ); // void const * pixels

   ok = GL_VALIDATE;
   if ( !ok )
   {
      DE_ERROR("Cant upload" )
      return false;
   }

   // requires mipmaps. Generate them automatically.
   if ( getSamplerOptions().hasMipmaps() )
   {
      ::glGenerateMipmap( GL_TEXTURE_2D ); GL_VALIDATE
   }

   // m_Manager.setActive( 0, nullptr );
   ::glBindTexture( GL_TEXTURE_2D, 0 ); GL_VALIDATE
   return true;
}

bool
Tex2D::download( Image & img )
{
   if ( m_TextureId == 0 )
   {
      throw std::runtime_error("No textureId generated to download from.");
      return false;
   }

   return true;
}
*/

} // end namespace gpu.
} // end namespace de.

