#include <de/gpu/TexManager.hpp>
#include <de/gpu/TexMipmapGenerator.hpp>

namespace de {
namespace gpu {

TexManager::TexManager()
{
   //init();
}

TexManager::~TexManager()
{
   //unbindAll();
}

uint32_t
TexManager::getUnitCount() const
{
   return m_TexUnits.size();
}

// ###########################
// ###                     ###
// ###   TextureManager    ###
// ###                     ###
// ###########################

void
TexManager::init()
{
   //::glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ); GL_VALIDATE   // Affects glTexImage2D()
   //::glPixelStorei( GL_PACK_ALIGNMENT, 1 ); GL_VALIDATE     // Affects glReadPixels()

   int nTexUnits = glGetIntegerDE( GL_MAX_TEXTURE_IMAGE_UNITS );
   m_TexUnits.clear();
   m_TexUnits.resize( nTexUnits, 0 );
}

void
TexManager::dump()
{
   DE_DEBUG( "TexUnit.Count = ", m_TexUnits.size() )
   for ( size_t i = 0; i < m_TexUnits.size(); ++i )
   {
      DE_DEBUG( "TexUnit[",i,"] = ", m_TexUnits[ i ] )
   }

   DE_DEBUG( "TexCache.Count = ", m_Textures.size() )
   for ( size_t i = 0; i < m_Textures.size(); ++i )
   {
      Tex* tex = m_Textures[ i ];
      if ( tex )
      {
         DE_DEBUG( "TexCache[",i,"] = ", tex->toString() )
      }
      else
      {
         DE_ERROR( "TexCache[",i,"] = nullptr" )
      }
   }
}


// From image file
TexRef
TexManager::load2D( std::string const & name, std::string uri, bool keepImage, SO so )
{
#if 1
   if ( name.empty() ) { DE_ERROR("No name") return nullptr; }
   if ( uri.empty() ) { DE_ERROR("No uri") return nullptr; }

   Image img;
   if ( !dbLoadImage( img, uri ) )
   {
      DE_ERROR("Cant load image ", uri)
      return nullptr;
   }

   return add2D( name, img, keepImage, so );
#else
   DE_ERROR("Deactivated USE_LIB_DARKIMAGE")
   return TexRef();
#endif
}

// From image in RAM
TexRef
TexManager::add2D( std::string const & name, Image const & img, bool keepImage, SO so )
{
   if ( name.empty() ) { DE_ERROR("No name") return TexRef(); }
   if ( img.empty() ) { DE_ERROR("No image") return TexRef(); }
   if ( img.getFormat() != ColorFormat::RGBA8888 &&
        img.getFormat() != ColorFormat::RGB888 )
   {
      DE_ERROR("Unsupported image format ", img.toString() )
      return TexRef();
   }

   if ( hasTex( name ) )
   {
      DE_ERROR("Texture name already in use, ", name )
      return getTex( name );
   }

   GLuint texId = 0;
   glGenTextures( 1, &texId ); GL_VALIDATE
   bool ok = GL_VALIDATE;
   if ( !ok || texId < 1 )
   {
      DE_ERROR("Cant gen texture ", texId )
      return TexRef();
   }

   Tex* tex = new Tex();
   tex->m_Name = name;
   tex->m_Type = TexType::Tex2D;
   tex->m_SamplerOptions = so;
   tex->m_TextureId = texId;
   tex->m_KeepImage = keepImage;
   if ( keepImage )
   {
      tex->m_Image = img;
   }

   ok = upload( tex, img );
   if ( !ok )
   {
      DE_ERROR("Cant upload image ", tex->toString() )
      removeTexture( tex );
      return TexRef();
   }

   m_Textures.emplace_back( tex );
   m_TexLUT[ name ] = TexRef(tex);

   return TexRef(tex);
}



// Recti const & pos, SO const & so
bool
TexManager::upload( Tex* tex, Image const & img )
{
   if ( !tex ) { DE_ERROR("No tex") return false; }
   auto texId = tex->m_TextureId;
   if ( !texId ) { DE_ERROR("No texture id") return false; }

   if ( img.empty() ) { DE_ERROR("No image") return false; }

//   if ( m_KeepImages )
//   {
//      m_Images.emplace_back( img );
//   }

   if ( img.getFormat() != ColorFormat::RGBA8888 &&
        img.getFormat() != ColorFormat::RGB888 )
   {
      DE_ERROR("Unsupported image format ", img.toString() )
      return false;
   }

//   auto found = findTexture( key );
//   if ( found > -1 )
//   {
//      DE_DEBUG("Texture already exist(", key, ")")
//      return nullptr;
//   }

   int stage = bindTexture( tex );
   if (stage < 0)
   {
      DE_ERROR("No free stage, tex ", tex->toString() )
      return false;
   }

   //DE_DEBUG("Activated tex unit [",stage,"] for ", tex->toString() )

   //glActiveTexture( GL_TEXTURE0 + tex->getTexUnit() ); GL_VALIDATE
   //glBindTexture( GL_TEXTURE_2D, texId ); GL_VALIDATE
   int w = img.getWidth();
   int h = img.getHeight();
   GLenum internal = GL_RGBA;
   GLenum format = GL_RGBA;
   GLenum type = GL_UNSIGNED_BYTE;
   //SurfaceFormat benniFormat(8,8,8,8,0,0);
   TexFormat::EFormat texFormat = TexFormat::R8G8B8A8;
   if ( img.getFormat() == ColorFormat::RGB888 )
   {
      internal = GL_RGB;
      format = GL_RGB;
      type = GL_UNSIGNED_BYTE;
      texFormat = TexFormat::R8G8B8;
   }

   GLES::setSamplerOptions2D( tex->m_SamplerOptions );

   ::glTexImage2D( GL_TEXTURE_2D,   // GLenum target
                  0,                // GLint level
                  GLenum( internal ), // GLenum internalFormat
                  GLsizei( w ),  // GLsizei width
                  GLsizei( h ), // GLsizei height
                  0,                // GLint border
                  GLenum( format ), // GLenum format
                  GLenum( type ),   // GLenum type
                  img.data() ); // void const * pixels

   bool ok = GL_VALIDATE;
   if ( !ok ) { DE_ERROR("Cant upload" ) return false; }

   if ( tex->m_SamplerOptions.hasMipmaps() )
   {
      ::glGenerateMipmap( GL_TEXTURE_2D ); GL_VALIDATE
   }

   // ::glBindTexture( GL_TEXTURE_2D, 0 ); GL_VALIDATE

   tex->m_Width = w;
   tex->m_Height = h;
   //tex->m_SamplerOptions = so;
   tex->m_Format = texFormat;
   tex->m_TexUnit = -1;
   tex->m_TextureId = texId;
   tex->m_InternalFormat.internal = internal;
   tex->m_InternalFormat.format = format;
   tex->m_InternalFormat.type = type;
   return true;
}


// ###########################
// ###                     ###
// ###   TexUnitManager    ###
// ###                     ###
// ###########################

// Native GL
bool
TexManager::bindTexture2D( int stage, uint32_t texId )
{
   if ( texId < 1 ) { DE_WARN("More an unbind with texId = ", texId ) }
   if ( stage < 0 || stage >= int(m_TexUnits.size()) )
   {
      DE_ERROR("No valid stage ",stage,"/", int(m_TexUnits.size()) )
      return false;
   }
   if ( m_TexUnits[ stage ] == texId )
   {
      return true; // Cache hit, nothing todo.
   }

   m_TexUnits[ stage ] = texId; // Update unit
   ::glActiveTexture( GL_TEXTURE0 + stage ); GL_VALIDATE
   ::glBindTexture( GL_TEXTURE_2D, texId );
   bool ok = GL_VALIDATE;
   if ( !ok )
   {
      throw std::runtime_error( dbStrJoin( "No stage(",stage,") and texId(",texId,")" ) );
   }

   // GLES::setSamplerOptions2D( tex->getSamplerOptions() );
   return true;
}

int32_t
TexManager::findUnit( uint32_t texId ) const
{
   if ( texId < 1 ) return -1; // Not an id.
   for ( size_t i = 0; i < m_TexUnits.size(); ++i )
   {
      if ( m_TexUnits[ i ] == texId )
      {
         return int( i );
      }
   }
   return -1;
}

bool
TexManager::unbindTexture2D( uint32_t texId )
{
   if ( texId < 1 ) return true;  // Nothing to unbind.

   // Search unit with this tex...
   int found = findUnit( texId );
   if ( found < 0 )
   {
      return true; // Nothing to unbind.
   }

   m_TexUnits[ found ] = 0;
   ::glActiveTexture( GL_TEXTURE0 + found ); GL_VALIDATE
   ::glBindTexture( GL_TEXTURE_2D, 0 ); GL_VALIDATE

   // DE_DEBUG( "Unbound texture ", texId, " from stage ", found, " and target=", target )

   return true;
}

// More intelligent GL: stage/unit is auto select.
int
TexManager::bindTexture2D( uint32_t texId )
{
   if ( texId < 1 ) { return -1; }

   int unit = findUnit( texId );
   if ( unit > -1 )
   {
      return unit; // Cache hit, activated and bound.
   }

   // Find free unit, if any, and activate it
   for ( size_t i = 0; i < m_TexUnits.size(); ++i )
   {
      if ( !m_TexUnits[ i ] )
      {
         if ( bindTexture2D( i, texId ) )
         {
            //DE_DEBUG("Bound tex ", texId, " at unit ", i)
            return int( i );
         }
         else
         {
            DE_ERROR("Not bound unit(", i, ") with texId(",texId,")")
         }
      }
   }

   static int unitCounter = 0;
   unitCounter = (unitCounter + 1) % int(m_TexUnits.size());
   unit = unitCounter;
   if ( !bindTexture2D( unit, texId ) )
   {
      DE_ERROR("No force bound unit(", unit, ") with texId(",texId,")")
      return -1;
   }

   return unit;
}



// Benni's HighLevelCalls (class Tex) to native bindTexture2D

bool
TexManager::bindTexture( int stage, Tex* tex )
{
   if ( !tex ) { DE_ERROR("No tex") return false; }

   if ( tex->getType() == TexType::Tex2D )
   {
      if ( bindTexture2D( stage, tex->getTextureId() ) )
      {
         tex->setTexUnit( stage );
         return true;
      }
   }

//   else if ( tex->isRenderBuffer() )
//   {
//      ::glBindRenderbuffer( GL_RENDERBUFFER, tex->getTextureId() ); GL_VALIDATE
////      ::glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h ); GL_VALIDATE
////      ::glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
////            GL_RENDERBUFFER, m_depthTex ); GL_VALIDATE
//   }

   throw std::runtime_error("Cant bind invalid tex type");
}


bool
TexManager::unbindTexture( Tex* tex )
{
   if ( !tex ) { return true; }
   if ( tex->getType() == TexType::Tex2D )
   {
      if ( !unbindTexture2D( tex->getTextureId() ) )
      {
         DE_ERROR("Cant unbind tex ", tex->toString())
         return false;
      }
      else
      {
         return true;
      }
   }
   else
   {
      DE_ERROR("Unsupported tex type ", tex->toString())
      return false;
   }
}

int
TexManager::bindTexture( Tex* tex )
{
   if ( !tex ) { DE_ERROR("No tex") return -1; }
   if ( tex->getType() == TexType::Tex2D )
   {
      return bindTexture2D( tex->getTextureId() );
   }
   else
   {
      // throw std::runtime_error("Invalid texture type");
   }
   return -1;
}

// bool
// TexManager::activateTexture( int stage, std::string const & key )
// {
   // return activateTexture( stage, getTexture( key ) );
// }


bool
TexManager::hasTex( std::string const & name ) const
{
   auto it = m_TexLUT.find( name );
   if (it == m_TexLUT.end())
   {
      return false;
   }
   else
   {
      return true;
   }
}

TexRef
TexManager::getTex( std::string const & name ) const
{
   if ( name.empty() )
   {
      DE_DEBUG("Got empty name")
      return TexRef();
   }

   auto it = m_TexLUT.find( name );
   if (it == m_TexLUT.end())
   {
      DE_ERROR("No tex named ",name)
      return TexRef();
   }

   return it->second; // Cache Hit
}


void
TexManager::removeTexture( Tex* tex )
{
   if ( !tex ) { DE_DEBUG("Got nullptr") return; }

   // Delete cache entry
   auto cacheIt = std::find_if( m_Textures.begin(), m_Textures.end(),
         [&] ( Tex const * const p ) { return p == tex; } );
   if ( cacheIt != m_Textures.end() )
   {
      Tex* cached = *cacheIt;
      if ( cached )
      {
         DE_DEBUG("[TexCache] DeleteEntry ", cached->toString() )
      }
      m_Textures.erase( cacheIt );
   }

   unbindTexture( tex );
   m_TexturesToRemove.emplace_back( tex );
}

void
TexManager::removeTexture( std::string const & name )
{
   auto ref = getTex( name );
   removeTexture( ref.tex );
}

void
TexManager::clearTextures()
{
   for ( size_t i = 0; i < m_Textures.size(); ++i )
   {
      Tex* cached = m_Textures[ i ];
      if ( cached )
      {
         DE_DEBUG( "MarkAsObsolete Tex[",i,"] ", cached->toString() )
         m_TexturesToRemove.emplace_back( cached );
      }
   }
   m_Textures.clear();
}

void
TexManager::updateTextures()
{
   for ( size_t i = 0; i < m_TexturesToRemove.size(); ++i )
   {
      Tex* markedAsTrash = m_TexturesToRemove[ i ];
      if ( markedAsTrash )
      {
         DE_DEBUG( "Delete Tex[", i, "] ", markedAsTrash->toString() )
         if ( markedAsTrash->m_TextureId > 0 )
         {
            ::glDeleteTextures( 1, &markedAsTrash->m_TextureId );
            GL_VALIDATE
         }

         delete markedAsTrash;
      }
   }
   m_TexturesToRemove.clear();
}




} // end namespace gpu.
} // end namespace de.

