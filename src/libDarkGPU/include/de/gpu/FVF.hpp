/// ==========================================================================
/// @file State.hpp
/// @brief DarkEngine header offering OpenGL ES 3.x Benni extensions
/// @author 2018 Benjamin Hampe <benjaminhampe@gmx.de>
/// @copyright Free open source software
/// to encapsulate much overheader.
/// ==========================================================================

#ifndef DE_GPU_FVF_HPP
#define DE_GPU_FVF_HPP

#include <de/gpu/Math3D.hpp>
#include <functional>

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) if ( (x) ) { delete (x); (x) = nullptr; }
#endif

namespace de {
namespace gpu {

// ===========================================================================
struct AnimTimer
// ===========================================================================
{
   double start = 0.0;
   double last = 0.0;
   double curr = 0.0;
   bool running = true;
   bool paused = false;
};

// ===========================================================================
struct PrimitiveType // FaceType
// ===========================================================================
{
   enum EType
   {
      Points = 0,    // 1 GL_POINTS
      Lines,         // 2 GL_LINES
      LineStrip,     // 2 GL_LINE_STRIP
      LineLoop,      // 2 GL_LINE_LOOP
      Triangles,     // 3 GL_TRIANGLES
      TriangleStrip, // 3 GL_TRIANGLE_STRIP
      TriangleFan,   // 3 GL_TRIANGLE_FAN
      Quads,         // 4 GL_QUADS -> not really impl on GPU but used for saving meshes.
      //Polygon,       // 5-N GL_POLYGON = VERTEX_OUTLINE_LIST, FILLED lINE_lOOP
      Max
   };

   static std::string
   getString( EType const primitiveType )
   {
      switch ( primitiveType )
      {
         case Lines: return "Lines";
         case LineStrip: return "LineStrip";
         case LineLoop: return "LineLoop";
         case Triangles: return "Triangles";
         case TriangleStrip: return "TriangleStrip";
         case TriangleFan: return "TriangleFan";
         case Quads: return "Quads";
         //case Polygon: return "Polygon";
         default: return "Points";
      }
   }

   // indexCount should be statement: getIndexCount() == 0 ? getVertexCount() : getIndexCount();
   static uint32_t
   getPrimitiveCount( EType const primitiveType, uint32_t const indexCount )
   {
      switch( primitiveType )
      {
         case Points: return indexCount;            // #num points
         case Lines: return indexCount / 2;         // #num Lines
         case LineLoop: return indexCount - 1;
         case Triangles: return indexCount / 3;     // #num triangles in Triangles
         case TriangleStrip: return indexCount - 2; // #num triangles in Strip
         case TriangleFan: return indexCount - 2;   // #num triangles in Fan
         case Quads: return indexCount / 4;         // #num quads in Quads
         //case Polygon: return indexCount; // entire MeshBuffer is one face only
         default: return 0;
      }
   }

   //   static uint32_t
   //   getEdgeCount( EType const primitiveType, uint32_t const indexCount )
   //   {
   //      switch( primitiveType )
   //      {
   //         case Points: return 0;
   //         case Lines: return indexCount / 2;
   //         case LineLoop: return indexCount - 1;
   //         case Triangles: return indexCount;
   //         case TriangleStrip: return indexCount;
   //         case TriangleFan: return indexCount;
   //         //case Polygon: return indexCount;
   //         //case Quads: return indexCount;
   //         default: return 0;
   //      }
   //   }

   // PrimitiveType() : m_Type( Points ) {}
   // PrimitiveType( EType primitiveType ) : m_Type( primitiveType ) {}
   // operator EType() { return m_Type; }
   // EType getValue() const { return m_Type; }
   // void setValue( EType type ) { m_Type = type; }
   // EType m_Type; ///< The data member.
};

// ===========================================================================
struct IndexType
// ===========================================================================
{
   enum EType { U8 = 0, U16, U32, ETypeMax };

   static std::string getString( EType const indexType )
   {
      switch ( indexType )
      {
         case U8: return "U8";
         case U16: return "U16";
         case U32: return "U32";
         default: return "Unknown";
      }
   }

   template < typename Unsigned >
   static EType getIndexType()
   {
           if ( sizeof( Unsigned ) == 1 ) return U8;
      else if ( sizeof( Unsigned ) == 2 ) return U16;
      else if ( sizeof( Unsigned ) == 4 ) return U32;
      else return ETypeMax;
   }

   static uint32_t
   getIndexSize( EType typ )
   {
           if ( typ == U8 ) return 1;
      else if ( typ == U16 ) return 2;
      else if ( typ == U32 ) return 4;
      else return 0;
   }
};

// ===========================================================================
struct VertexAttribType
// ===========================================================================
{
   enum EType { F32 = 0, U8, U16, U32, S8, S16, S32, ETypeMax };

   static std::string
   toString( EType const type )
   {
      switch( type )
      {
         case F32: return "F32";
         case U8: return "U8";
         case U16: return "U16";
         case U32: return "U32";
         case S8: return "S8";
         case S16: return "S16";
         case S32: return "S32";
         default: return "Unknown";
      }
   }

   static size_t
   getByteSize( EType const type )
   {
      switch( type )
      {
         case F32: return 4;
         case U8: return 1;   // mostly 8 bit unsigned color channel, 3 rgb or 4 rgba.
         case U16: return 2;  // mostly an index or unsigned height 0..64km.
         case U32: return 4;  // mostly entirety of an index or RGB(A) color.
         case S8: return 1;   // mostly 8 bit fast repeating line stipple or low prec result
         case S16: return 2;  // half precision float 2.14 or heightmap -32km + 32km, min eps step 10cm.
         case S32: return 4;  // mostly anything that can be a math result as int32_t. Math likes sings, like a 2d raster image. We can start at -100000 and anything outside the window is simply not displayed.
         default: return 0;   // dummy? or error? im not sure, thatswhy dummy. No assertions.
      }
   }
};

// ===========================================================================
struct VertexAttribSemantic
// ===========================================================================
{
   enum EType
   {
      Pos = 0, Normal, Tangent, Bitangent, Color, Distance, Index,
      Tex, Tex1, Tex2, Tex3,
      Tex3D, TexCube, Tex2DArray, Max
   };

   static std::string
   toString( EType const semantic )
   {
      switch( semantic )
      {
         case Pos: return "a_pos";
         case Normal: return "a_normal";
         case Tangent: return "a_tangent";
         case Bitangent: return "a_bitangent";
         case Color: return "a_color";
         case Distance: return "a_dist"; // a_distFromStart
         case Index: return "a_index";
         case Tex: return "a_tex";
         case Tex1: return "a_tex1";
         case Tex2: return "a_tex2";
         case Tex3: return "a_tex3";
         case Tex3D: return "a_tex3D";
         //case Tex3D_1: return "a_tex3D_1";
         case TexCube: return "a_texCube";
         case Tex2DArray: return "a_tex2DArray";
         default: return "unknown";
      }
   }
};


// ===========================================================================
struct VertexAttrib
// ===========================================================================
{
   VertexAttribType::EType m_Type;
   uint8_t m_Count;
   uint16_t m_ByteSize;
   bool m_Normalize;
   bool m_Enabled;
   VertexAttribSemantic::EType m_Semantic;

   // No default ctr is meaningful, fvf need lots of detailed data descript.
   VertexAttrib() = delete;

   // Normal ctr
   VertexAttrib( VertexAttribType::EType type,
                 uint8_t count,
                 VertexAttribSemantic::EType semantic,
                 bool normalize = false )
      : m_Type( type )
      , m_Count( count )
      , m_ByteSize( uint16_t( VertexAttribType::getByteSize( type ) * count ) )
      , m_Normalize( normalize )
      , m_Enabled( true )
      , m_Semantic( semantic )
   {}

   // Special ctr
   // Can deactivate specific attributes for shaders that dont use all attributes
   // of a common used vertex-type like S3DVertex -> can reuse vertex type in
   // different shaders like Color3DRenderer that dont uses normals and uv-coords.
   VertexAttrib( bool enabled,
                 VertexAttribType::EType type,
                 uint8_t count,
                 VertexAttribSemantic::EType semantic,
                 bool normalize = false )
      : m_Type( type )
      , m_Count( count )
      , m_ByteSize( uint16_t( VertexAttribType::getByteSize( type ) * count ) )
      , m_Normalize( normalize )
      , m_Enabled( enabled )
      , m_Semantic( semantic )
   {}

   std::string
   toString() const
   {
      std::stringstream s;
      if ( !m_Enabled )
      {
         s << "DISABLED: ";
      }
      s << VertexAttribSemantic::toString( m_Semantic ) << ", "
        << int32_t( m_Count ) << "x "
        << VertexAttribType::toString( m_Type ) << ", "
        << "size:" << int32_t( m_ByteSize );
      if ( m_Normalize )
      {
         s << ", normalized";
      }
      return s.str();
   }
};

// ===========================================================================
// | FVF | FlexibleVertexFormat - A container of Vertex attributes
// ===========================================================================
struct FVF
{
   FVF()
      : m_Stride( 0 ), m_Data()
   {}
   FVF( uint32_t strideBytes, std::vector< VertexAttrib > && data )
      : m_Stride( strideBytes )
      , m_Data( std::move( data ) )
   {}

   std::string
   toString() const
   {
      std::ostringstream s;
      s << "stride:" << m_Stride << ",attribs:" << m_Data.size() << "\n";
      size_t offset = 0;
      for ( size_t i = 0 ; i < m_Data.size(); ++i )
      {
         s << "[" << i << "] o:" << offset << ", " << m_Data[ i ].toString() << "\n";
         offset += m_Data[ i ].m_ByteSize;
      }
      return s.str();
   }

   bool
   isValid() const {  if ( m_Stride < 1 ) { return false; } return true; }

   size_t
   getByteSize() const { return m_Stride; }

   size_t
   getAttribCount() const { return m_Data.size(); }

   VertexAttrib const &
   getAttrib( size_t i ) const { return m_Data[ i ]; }

   VertexAttrib &
   getAttrib( size_t i ) { return m_Data[ i ]; }

public:
   uint32_t m_Stride;
   std::vector< VertexAttrib > m_Data;
};


} // end namespace gpu.
} // end namespace de.

#endif

