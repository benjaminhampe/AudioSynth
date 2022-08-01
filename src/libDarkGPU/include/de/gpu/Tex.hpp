/// ==========================================================================
/// @file State.hpp
/// @brief DarkEngine header offering OpenGL ES 3.x Benni extensions
/// @author 2018 Benjamin Hampe <benjaminhampe@gmx.de>
/// @copyright Free open source software
/// to encapsulate much overheader.
/// ==========================================================================

#ifndef DE_GPU_TEX_TEX_HPP
#define DE_GPU_TEX_TEX_HPP

#include <de/gpu/State.hpp>

namespace de {
namespace gpu {

// ===========================================================================
struct SurfaceFormat
// ===========================================================================
{
   SurfaceFormat()
      : r( 8 ), g( 8 ), b( 8 ), a( 8 ), d( 24 ), s( 8 ), offscreen( false ), dummy( false )
   {}
   SurfaceFormat( int r, int g, int b, int a = 0, int d = 0, int s = 0, bool isRendBuf = false, bool offscreen = false )
      : r( r ), g( g ), b( b ), a( a ), d( d ), s( s ), offscreen( offscreen ), dummy( isRendBuf )
   {}

   bool
   isRenderBuffer() const { return dummy; }

   int
   getColorBits() const { return r + g + b + a; }

   bool
   hasTransparency() const { return a > 0; }

   bool
   hasColor() const { return (r > 0 || g > 0 || g > 0); }

   bool
   hasDepth() const { return d > 0; }

   bool
   hasStencil() const { return s > 0; }

   bool
   isOnlyColor() const { return (hasColor() || hasTransparency()) && d == 0 && s == 0; }

   std::string
   toString() const {
      std::stringstream txt;
      if ( a > 0 ) { txt << "A"; }
      if ( r > 0 ) { txt << "R"; }
      if ( g > 0 ) { txt << "G"; }
      if ( b > 0 ) { txt << "B"; }
      if ( a > 0 ) { txt << a; }
      if ( r > 0 ) { txt << r; }
      if ( g > 0 ) { txt << g; }
      if ( b > 0 ) { txt << b; }
      if ( d > 0 ) { txt << "_D" << d; }
      if ( s > 0 ) { txt << "_S" << s; }
      if ( offscreen ) { txt << "_Offscreen"; }
      return txt.str();
   }

public:
   int r, g, b, a, d, s;
   bool offscreen;
   bool dummy; // IsRenderBuffer
};

// ===========================================================================
struct TexType
// ===========================================================================
{
   enum EType
   {
      Tex2D = 0,
      Tex3D,
      Tex2DArray,
      TexCube,
      ETypeCount
   };
};

// ===========================================================================
struct TexFormat
// ===========================================================================
{
   enum EFormat
   {
      Auto = 0,
      R8G8B8A8,
      R8G8B8,
      R8,
      D24S8,
      D32F,
      D32FS8,
      EFormatCount
   };
};

// ===========================================================================
struct TexSemantic
// ===========================================================================
{
   enum ESemantic
   {
      Diffuse = 0,
      Bump, // = Elevation,
      //Normal2D,
      //Normal3D,
      //Reflection,
      //Refraction,
      ESemanticCount
   };
};

// ===========================================================================
struct SO // SamplerOptions
// ===========================================================================
{
   enum EMagFilter { MagNearest = 0, MagLinear };
   enum EMinFilter { MinNearest = 0, MinLinear,
      MinMipmapNearestNearest, MinMipmapNearestLinear,
      MinMipmapLinearNearest, MinMipmapLinearLinear
   };
   enum EWrap { ClampToEdge = 0, ClampToBorder, Repeat, RepeatMirrored };

   SO();
   SO( EMagFilter u_mag, EMinFilter u_min, EWrap s_wrap, EWrap t_wrap );
   SO( SO const & o );
   bool hasMipmaps() const;
   std::string toString() const;

public:
   EMagFilter mag;
   EMinFilter min;
   uint8_t wrapS;
   uint8_t wrapT;
   uint8_t wrapR; // 3D
   int anisotropicFilter;
};

struct TexManager;

// ===========================================================================
struct TexInternalFormat
// ===========================================================================
{
   uint32_t internal = 0;  // GL_RGB;
   uint32_t format = 0;    // GL_RGB;
   uint32_t type = 0;      // GL_UNSIGNED_BYTE;
};

// TODO Add shared_ptr
// ===========================================================================
struct Tex
// ===========================================================================
{
   DE_CREATE_LOGGER("de.gpu.Tex")

   Tex( TexManager* driver = nullptr );
   ~Tex();

   std::string toString() const;
   void setTexManager( TexManager* driver ) { m_Driver = driver; }
   void setTexUnit( int32_t unit ) { m_TexUnit = unit; }
   void setName( std::string const & name ) { m_Name = name; }
   void setSamplerOptions( SO const & so ) { m_SamplerOptions = so; }

   TexFormat::EFormat const & getFormat() const { return m_Format; }
   TexType::EType getType() const { return m_Type; }
   std::string const & getName() const { return m_Name; }
   int32_t getWidth() const { return m_Width; }
   int32_t getHeight() const { return m_Height; }
   int32_t getDepth() const { return m_Depth; }
   uint32_t getTextureId() const { return m_TextureId; }
   int32_t getTexUnit() const { return m_TexUnit; }
   uint32_t getMaxMipLevel() const { return m_MaxMipLevel; }
   SO const & getSamplerOptions() const { return m_SamplerOptions; }
   bool hasTransparency() const { return m_Format == TexFormat::R8G8B8A8; }
   bool isRenderBuffer() const { return m_IsRenderBuffer; }

   void clearCPUSide() { m_Image.clear( true ); }
   Image const & getImage() const { return m_Image; }
   Image & getImage() { return m_Image; }


   TexManager* m_Driver;
   TexType::EType m_Type;        // e.g. Tex2D
   TexFormat::EFormat m_Format;  // e.g. R8G8B8A8
   TexInternalFormat m_InternalFormat; // Filled by GL
   int32_t m_Width;
   int32_t m_Height;
   int32_t m_Depth;        // for 3d textures
   int32_t m_MaxMipLevel;  // for Mipmapping
   SO m_SamplerOptions;
   int32_t m_TexUnit;
   uint32_t m_TextureId;
   bool m_IsRenderBuffer;
   bool m_KeepImage;
   std::string m_Name;  // Key the engine/modeler uses to find this tex by name.
   std::string m_Uri;   // The original image uri, if any.
   // CPU-side,
   // has the original uri string to export the texture elsewhere, single image or image atlas page
   Image m_Image; // CPU-side


};


// ===========================================================================
struct TexRef
// ===========================================================================
{
   int id;
   std::string name;
   std::string uri;
   Image* img;       // CPU-Side, the original image and uri, not a combined atlas page
   uint32_t texWidth;
   uint32_t texHeight;
   Tex* tex;    // GPU-Side
   Recti rect;
   Rectf coords;
   glm::vec2 repeat;
   // int refCount;

   TexRef();
   TexRef( Tex* ptr );
   TexRef( Tex* ptr, Recti const & pos );
   int getTexWidth() const { return texWidth; }
   int getTexHeight() const { return texHeight; }
   glm::ivec2 getTexSize() const { return glm::ivec2( getTexWidth(), getTexHeight() ); }
   glm::vec2 getTexSizef() const { return glm::vec2( getTexWidth(), getTexHeight() ); }
   int getX() const { return rect.getX(); }
   int getY() const { return rect.getY(); }
   int getWidth() const { return rect.getWidth(); }
   int getHeight() const { return rect.getHeight(); }
   Recti const & getRect() const { return rect; }
   Rectf const & getTexCoords() const { return coords; }
   float getU1() const { return coords.getU1(); }
   float getV1() const { return coords.getV1(); }
   float getU2() const { return coords.getU2(); }
   float getV2() const { return coords.getV2(); }
   float getDU() const { return coords.getDU(); }
   float getDV() const { return coords.getDV(); }

   std::string
   toString() const;

//   Image const &
//   getImage() const { return img; }

   void
   setMatrix( float tx, float ty, float sx, float sy );

   bool
   empty() const;

   // vec4.<x,y> = translation x,y ( atlas tex chunk pos )
   // vec4.<z,w> = scale x,y ( atlas tex chunk size )
   glm::vec4
   getTexTransform() const
   {
      return glm::vec4( coords.x, coords.y, coords.w, coords.h );
   }
};

} // end namespace gpu.
} // end namespace de.

inline std::ostream &
operator<< ( std::ostream & o, de::gpu::SO const & so )
{
   o << so.toString();
   return o;
}

#endif

