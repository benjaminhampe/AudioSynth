/// ==========================================================================
/// @file GLES.hpp
/// @author 2018 Benjamin Hampe <benjaminhampe@gmx.de>
/// @copyright Free open source software
/// ==========================================================================

#ifndef DE_GPU_TEX_TEXMANAGER_HPP
#define DE_GPU_TEX_TEXMANAGER_HPP

#include <de/gpu/Tex.hpp>

namespace de {
namespace gpu {

// ===========================================================================
struct TexManager
// ===========================================================================
{
   DE_CREATE_LOGGER("de.gpu.TexManager")
   TexManager();
   ~TexManager();

// ###########################
// ###   TexUnitManager    ###
// ###########################
   uint32_t getUnitCount() const;
   // Stupid raw power
   bool bindTexture2D( int stage, uint32_t texId );
   bool unbindTexture2D( uint32_t texId );
   // AutoSelect a free tex unit and return its index for use in setSamplerUniform().
   int bindTexture2D( uint32_t texId );
protected:
   int32_t findUnit( uint32_t texId ) const;
public:
   // High level calls for Tex class, not the raw tex index.
   bool bindTexture( int stage, Tex* tex );
   bool unbindTexture( Tex* tex );
   // High level call with more intelligence.
   int bindTexture( Tex* tex );

// ######################
// ###   TexManager   ###
// ######################
   void init();
   void dump();
   int32_t findTex( Tex* tex ) const;
   int32_t findTex( std::string const & name ) const;
   bool hasTex( std::string const & name ) const;
   // Get texture by name ( fast lut map ), does not load, only a lookup for efficiency at runtime.
   TexRef getTex( std::string const & name ) const;
   // Load texture from image file ( HDD )
   TexRef load2D( std::string const & name, std::string uri, bool keepImage = true, SO so = SO() );
   // Add texture from memory image ( RAM )
   TexRef add2D( std::string const & name, Image const & img, bool keepImage = true, SO so = SO() );
   // Full upload
   bool upload( Tex* tex, Image const & src );
   // [Slim] LoadImageFromFile ( HDD )
   // Advantage: easy
   inline TexRef load2D( std::string const & uri, bool keepImage = true, SO so = SO() )
   { return load2D( uri, uri, keepImage, so ); }
   inline TexRef add2D( Image const & img, bool keepImage = true, SO so = SO() )
   { return add2D( img.getUri(), img, keepImage, so ); }
   void clearTextures();
   void updateTextures();
   void removeTexture( Tex* tex );
   void removeTexture( std::string const & key );

// ###########################
// ###   TexUnitManager    ###
// ###########################
   std::vector< uint32_t > m_TexUnits; // Resized to max avail texture Units
// ######################
// ###   TexManager   ###
// ######################
   std::vector< Tex* > m_Textures; // All big memory pages ( atlas textures )
   std::unordered_map< std::string, TexRef > m_TexLUT; // has indices into m_Textures.
   std::vector< Tex* > m_TexturesToRemove;   // All textures that exist
// ######################
// ###   RefManager   ###
// ######################
   //std::vector< TexRef > m_Refs;    // All small textures we can use
   //std::unordered_map< std::string, int > m_RefLUT; // has indices into m_TexRefs
};

} // end namespace gpu.
} // end namespace de.

#endif

