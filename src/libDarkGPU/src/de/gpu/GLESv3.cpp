#include <de/gpu/GLESv3.hpp>

namespace de {
namespace gpu {

Uniform::EType
GLES::toEUniformType( GLenum const uniformType )
{
   switch ( uniformType )
   {
      case GL_FLOAT:       return Uniform::FLOAT;
      case GL_FLOAT_VEC2:  return Uniform::VEC2;
      case GL_FLOAT_VEC3:  return Uniform::VEC3;
      case GL_FLOAT_VEC4:  return Uniform::VEC4;

      case GL_INT:       return Uniform::INT;
      case GL_INT_VEC2:  return Uniform::IVEC2;
      case GL_INT_VEC3:  return Uniform::IVEC3;
      case GL_INT_VEC4:  return Uniform::IVEC4;

      case GL_UNSIGNED_INT:       return Uniform::UINT;
      case GL_UNSIGNED_INT_VEC2:  return Uniform::UVEC2;
      case GL_UNSIGNED_INT_VEC3:  return Uniform::UVEC3;
      case GL_UNSIGNED_INT_VEC4:  return Uniform::UVEC4;

      case GL_BOOL:       return Uniform::BVEC1;
      case GL_BOOL_VEC2:  return Uniform::BVEC2;
      case GL_BOOL_VEC3:  return Uniform::BVEC3;
      case GL_BOOL_VEC4:  return Uniform::BVEC4;

      case GL_FLOAT_MAT2:  return Uniform::MAT2;
      case GL_FLOAT_MAT3:  return Uniform::MAT3;
      case GL_FLOAT_MAT4:  return Uniform::MAT4;

      case GL_SAMPLER_2D:  return Uniform::SAMPLER_2D;
      case GL_SAMPLER_3D:  return Uniform::SAMPLER_3D;
      case GL_SAMPLER_CUBE:  return Uniform::SAMPLER_CUBE;
      case GL_SAMPLER_2D_ARRAY:  return Uniform::SAMPLER_2D_ARRAY;

      case GL_SAMPLER_2D_SHADOW:  return Uniform::SAMPLER_2D_SHADOW;
      case GL_SAMPLER_CUBE_SHADOW:  return Uniform::SAMPLER_CUBE;
      case GL_SAMPLER_2D_ARRAY_SHADOW:  return Uniform::SAMPLER_2D_ARRAY_SHADOW;

      case GL_INT_SAMPLER_2D:  return Uniform::ISAMPLER_2D;
      case GL_INT_SAMPLER_3D:  return Uniform::ISAMPLER_3D;
      case GL_INT_SAMPLER_CUBE:  return Uniform::ISAMPLER_CUBE;
      case GL_INT_SAMPLER_2D_ARRAY:  return Uniform::ISAMPLER_2D_ARRAY;

      case GL_UNSIGNED_INT_SAMPLER_2D:  return Uniform::USAMPLER_2D;
      case GL_UNSIGNED_INT_SAMPLER_3D:  return Uniform::USAMPLER_3D;
      case GL_UNSIGNED_INT_SAMPLER_CUBE:  return Uniform::USAMPLER_CUBE;
      case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:  return Uniform::USAMPLER_2D_ARRAY;

      default: DE_ERROR("Unknown") return Uniform::ETypeCount;
   }
}

GLenum
GLES::fromEUniformType( Uniform::EType const uniformType )
{
   switch ( uniformType )
   {
      case Uniform::FLOAT: return GL_FLOAT;
      case Uniform::VEC2:  return GL_FLOAT_VEC2;
      case Uniform::VEC3:  return GL_FLOAT_VEC3;
      case Uniform::VEC4:  return GL_FLOAT_VEC4;

      case Uniform::INT:   return GL_INT;
      case Uniform::IVEC2: return GL_INT_VEC2;
      case Uniform::IVEC3: return GL_INT_VEC3;
      case Uniform::IVEC4: return GL_INT_VEC4;

      case Uniform::UINT:  return GL_UNSIGNED_INT;
      case Uniform::UVEC2: return GL_UNSIGNED_INT_VEC2;
      case Uniform::UVEC3: return GL_UNSIGNED_INT_VEC3;
      case Uniform::UVEC4: return GL_UNSIGNED_INT_VEC4;

      case Uniform::BVEC1: return GL_BOOL;
      case Uniform::BVEC2: return GL_BOOL_VEC2;
      case Uniform::BVEC3: return GL_BOOL_VEC3;
      case Uniform::BVEC4: return GL_BOOL_VEC4;

      case Uniform::MAT2:  return GL_FLOAT_MAT2;
      case Uniform::MAT3:  return GL_FLOAT_MAT3;
      case Uniform::MAT4:  return GL_FLOAT_MAT4;

      case Uniform::SAMPLER_2D:  return GL_SAMPLER_2D;
      case Uniform::SAMPLER_3D:  return GL_SAMPLER_3D;
      case Uniform::SAMPLER_CUBE:  return GL_SAMPLER_CUBE;
      case Uniform::SAMPLER_2D_ARRAY:  return GL_SAMPLER_2D_ARRAY;

      case Uniform::SAMPLER_2D_SHADOW:  return GL_SAMPLER_2D_SHADOW;
      case Uniform::SAMPLER_CUBE_SHADOW:  return GL_SAMPLER_CUBE_SHADOW;
      case Uniform::SAMPLER_2D_ARRAY_SHADOW:  return GL_SAMPLER_2D_ARRAY_SHADOW;

      case Uniform::ISAMPLER_2D:  return GL_INT_SAMPLER_2D;
      case Uniform::ISAMPLER_3D:  return GL_INT_SAMPLER_3D;
      case Uniform::ISAMPLER_CUBE:  return GL_INT_SAMPLER_CUBE;
      case Uniform::ISAMPLER_2D_ARRAY: return GL_INT_SAMPLER_2D_ARRAY;

      case Uniform::USAMPLER_2D: return GL_UNSIGNED_INT_SAMPLER_2D;
      case Uniform::USAMPLER_3D:  return GL_UNSIGNED_INT_SAMPLER_3D;
      case Uniform::USAMPLER_CUBE:  return GL_UNSIGNED_INT_SAMPLER_CUBE;
      case Uniform::USAMPLER_2D_ARRAY:  return GL_UNSIGNED_INT_SAMPLER_2D_ARRAY;

      default: DE_ERROR("Unknown") return GL_INT;
   }
}

// ===========================================================================
// FVFUtils
// ===========================================================================
#ifdef USE_SMESH_GEOMETRY

bool
GLES::drawPrimitiveList( std::string const & caller, IMeshBuffer const & buf )
{
   return GLES::drawPrimitiveList( __func__, buf.getPrimitiveType(),
               buf.getVertices(), buf.getVertexCount(), buf.getFVF(),
               buf.getIndices(), buf.getIndexCount(), buf.getIndexType() );
}

#endif

bool
GLES::drawPrimitiveList(
   std::string const & caller,
   PrimitiveType::EType primType,
   void const* vertices, uint32_t vertexCount, FVF const & fvf,
   void const* indices, uint32_t indexCount, IndexType::EType indexType, bool debug )
{
   if ( !vertices ) { return false; } // DE_DEBUG( "No vertices. caller(",caller,")" )
   if ( vertexCount < 1 ) { return false; } // DE_DEBUG( "Empty vertices. caller(",caller,")" )
   if ( indexCount > 0 && !indices ) { DE_ERROR( "Invalid indices. caller(",caller,")" ) return false; }

   if ( debug )
   {
      uint32_t primCount = PrimitiveType::getPrimitiveCount( primType, indexCount > 0 ? indexCount : vertexCount );
      uint64_t byteCount = uint64_t(fvf.getByteSize()) * vertexCount +
                           uint64_t(IndexType::getIndexSize( indexType )) * indexCount;
      DE_DEBUG( caller," with ", primCount,
         " ", PrimitiveType::getString( primType ),", "
         "v:",vertexCount,", ",
         "i:",indexCount,"|",IndexType::getString( indexType ), ", "
         "fvf:", fvf.getByteSize(), ", "
         "siz:", byteCount
      )
   }

   bool ok = GLES::enableAttributesGL( fvf, (uint8_t const* )vertices );
   if ( !ok ) { DE_DEBUG( "No enabled attributes, caller(",caller,")" ) return false; }
   if ( indexCount < 1 )
   {
      //DE_DEBUG("VERTEX-PRIM(", PrimitiveType::getString( primType), "), v(", vertexCount,"), i(", indexCount, "), i-type(", IndexType::getString( indexType ), ")" )
      ::glDrawArrays( GLenum( GLES::fromPrimitiveType( primType ) ),
                      0, GLsizei( vertexCount ) ); GL_VALIDATE
   }
   else
   {
      // DE_DEBUG("INDEX-PRIM(", PrimitiveType::getString( primType), "), v(", vertexCount,"), i(", indexCount, "), i-type(", IndexType::getString( indexType ), ")" )
      ::glDrawElements( GLenum( GLES::fromPrimitiveType( primType ) ),
                        GLsizei( indexCount ),
                        GLenum( GLES::fromIndexType( indexType ) ),
                        indices ); GL_VALIDATE
   }

   GLES::disableAttributesGL( fvf );
   return true;
}

// New:

// moved here from FVF, because FVF is API only now.
bool
GLES::enableAttributesGL( FVF const & fvf, uint8_t const * vertices )
{
   if ( !vertices ) { DE_WARN("No vertices") return false; }

   GLuint k = 0;

   for ( size_t i = 0 ; i < fvf.m_Data.size(); ++i )
   {
      VertexAttrib const & attr = fvf.m_Data[ i ];
      if ( !attr.m_Enabled ) continue;

      //DE_DEBUG("glEnableVertexAttribArray(",k,")")

      GLint const attrCount = GLint( attr.m_Count );
      GLenum const attrType = GLES::fromVertexAttribType( attr.m_Type );
      GLsizei const attrStride = GLsizei( fvf.m_Stride );
      GLboolean const shouldNormalize = attr.m_Normalize ? GL_TRUE : GL_FALSE;

      ::glEnableVertexAttribArray( k ); GL_VALIDATE

      ::glVertexAttribPointer( k, attrCount, attrType,
                             shouldNormalize, attrStride, vertices ); GL_VALIDATE

      // DE_DEBUG("glEnableVertexAttribArray(",k,")")
      // DE_DEBUG("glVertexAttribPointer(", k,",", va_count,","
      //       , uint32_t(va_type),",", uint32_t(va_normalize),","
      //       , uint32_t(va_stride),",", (void*)vertices )

      vertices += attr.m_ByteSize; // Advance ptr to next item (attribute).
      k++;
   }

   return true;
}

// New:

// moved here from FVF, because FVF is API only now.
bool
GLES::disableAttributesGL( FVF const & fvf )
{
   GLuint k = 0;

   for ( size_t i = 0 ; i < fvf.m_Data.size(); ++i )
   {
      VertexAttrib const & attr = fvf.m_Data[ i ];
      if ( !attr.m_Enabled ) continue;

      ::glDisableVertexAttribArray( GLuint( i ) ); GL_VALIDATE

      //DE_DEBUG("glDisableVertexAttribArray(",k,")")
      k++;
   }
   return true;
}

//   void
//   drawGL( uint8_t const * vertices )
//   {
//      // DE_DEBUG("")
//      enableAttributesGL( vertices );
//      ::glDrawArrays( getPrimitiveType(), 0, getVertexCount() );
//      disableAttributesGL();
//   }


uint32_t
GLES::fromPrimitiveType( PrimitiveType::EType const primitiveType )
{
   switch ( primitiveType )
   {
      case PrimitiveType::Points: return GL_POINTS;
      case PrimitiveType::Lines: return GL_LINES;
      case PrimitiveType::LineStrip: return GL_LINE_STRIP;
      case PrimitiveType::LineLoop: return GL_LINE_LOOP;
      case PrimitiveType::Triangles: return GL_TRIANGLES;
      case PrimitiveType::TriangleStrip: return GL_TRIANGLE_STRIP;
      case PrimitiveType::TriangleFan: return GL_TRIANGLE_FAN;
      default: return GL_POINTS;
   }
}

PrimitiveType::EType
GLES::toPrimitiveType( uint32_t const primitiveType )
{
   switch ( primitiveType )
   {
      case GL_POINTS: return PrimitiveType::Points;
      case GL_LINES: return PrimitiveType::Lines;
      case GL_LINE_STRIP: return PrimitiveType::LineStrip;
      case GL_LINE_LOOP: return PrimitiveType::LineLoop;
      case GL_TRIANGLES: return PrimitiveType::Triangles;
      case GL_TRIANGLE_STRIP: return PrimitiveType::TriangleStrip;
      case GL_TRIANGLE_FAN: return PrimitiveType::TriangleFan;
      default: return PrimitiveType::Points;
   }
}

uint32_t
GLES::fromIndexType( IndexType::EType const indexType )
{
   switch ( indexType )
   {
      case IndexType::U8: return GL_UNSIGNED_BYTE;
      case IndexType::U16: return GL_UNSIGNED_SHORT;
      case IndexType::U32: return GL_UNSIGNED_INT;
      default: assert( false ); return 0;
   }
}

// I. converter from OpenGL backend
VertexAttribType::EType
GLES::toVertexAttribType( uint32_t type )
{
   switch( type )
   {
      case GL_FLOAT: return VertexAttribType::F32;
      case GL_BYTE: return VertexAttribType::S8;
      case GL_SHORT: return VertexAttribType::S16;
      case GL_INT: return VertexAttribType::S32;
      case GL_UNSIGNED_BYTE: return VertexAttribType::U8;
      case GL_UNSIGNED_SHORT: return VertexAttribType::U16;
      case GL_UNSIGNED_INT: return VertexAttribType::U32;
      default: { assert( false );
         return VertexAttribType::ETypeMax;
      }
   }
}

// I. converter to OpenGL backend
uint32_t
GLES::fromVertexAttribType( VertexAttribType::EType type )
{
   switch( type )
   {
      case VertexAttribType::F32: return GL_FLOAT;
      case VertexAttribType::S8: return GL_BYTE;
      case VertexAttribType::S16: return GL_SHORT;
      case VertexAttribType::S32: return GL_INT;
      case VertexAttribType::U8: return GL_UNSIGNED_BYTE;
      case VertexAttribType::U16: return GL_UNSIGNED_SHORT;
      case VertexAttribType::U32: return GL_UNSIGNED_INT;
      default: { assert( false );
         return 0;
      }
   }
}

// =======================================================================
// ShaderUtils
// =======================================================================

// static
uint32_t
GLES::fromShaderType( ShaderType::EType const shaderType )
{
//   #ifndef GL_GEOMETRY_SHADER
//   #define GL_GEOMETRY_SHADER 0x8DD9
//   #endif

   switch ( shaderType )
   {
      case ShaderType::vs: return GL_VERTEX_SHADER;
      case ShaderType::fs: return GL_FRAGMENT_SHADER;
      case ShaderType::gs: return 0x8DD9; // GL_GEOMETRY_SHADER;
      default: assert( false ); return 0;
   }
}

//static
uint32_t
GLES::compileShader( ShaderType::EType shaderType, std::string const & source, bool debug )
{
   //debug = true;
   if ( debug )
   {
      DE_DEBUG("Compile (", ShaderType::getString( shaderType ), ") shader with len(", source.size(), ")" )
      DE_DEBUG("Debug-Text:\n", GLES::printShaderWithLineNumbers( source ) )
   }

   uint32_t shader = ::glCreateShader( GLES::fromShaderType( shaderType ) ); GL_VALIDATE
   if ( shader == 0 )
   {
      DE_ERROR("Cant create shader." )
      return 0;
   }

   char const * const pcszText = source.c_str();
   ::glShaderSource( shader, 1, &pcszText, nullptr ); GL_VALIDATE
   ::glCompileShader( shader ); GL_VALIDATE
   GLint ok;
   ::glGetShaderiv( shader, GL_COMPILE_STATUS, &ok ); GL_VALIDATE
   if ( ok == GL_FALSE )
   {
      DE_ERROR("Cant compile (", ShaderType::getString( shaderType ), ") shader "
               "program(", GLES::getShaderInfoString( shader ), " )" )
      ::glDeleteShader( shader ); GL_VALIDATE
      DE_ERROR("Error-Shader-Text:\n", GLES::printShaderWithLineNumbers( source ) )
      return 0;
   }

   return shader;
}

// static
uint32_t
GLES::linkProgram( std::string const & name, std::string const & VS, std::string const & FS, bool debug )
{
   uint32_t vs = compileShader( ShaderType::vs, VS.c_str(), debug );
   if ( !vs ) { DE_ERROR( "Cannot create vs shader ",name ) return 0; }
   uint32_t fs = compileShader( ShaderType::fs, FS.c_str(), debug );
   if ( !fs ) { DE_ERROR( "Cannot create fs shader ",name ) return 0; }
   uint32_t po = ::glCreateProgram(); GL_VALIDATE
   if ( !po ) { DE_ERROR( "Cannot create shader program ",name ) return 0; }
   ::glAttachShader( po, vs ); GL_VALIDATE
   ::glAttachShader( po, fs ); GL_VALIDATE
   ::glLinkProgram( po ); GL_VALIDATE
   GLint ok;
   ::glGetProgramiv( po, GL_LINK_STATUS, &ok ); GL_VALIDATE
   if ( ok == GL_FALSE )
   {
      DE_ERROR( "Program[",name,"].LinkStatus(",po,") ", getProgramInfoString(po) )
      ::glDeleteProgram ( po );
      return 0;
   }
   return po;
}

// static
void
GLES::deleteProgram( uint32_t & program )
{
   if ( !program ) { return; }
   ::glDeleteProgram( program );
   program = 0;
}

//   GL_ACTIVE_ATTRIBUTES
//   GL_ACTIVE_ATTRIBUTE_MAX_LENGTH
//   GL_ACTIVE_UNIFORM_BLOCK
//   GL_ACTIVE_UNIFORM_BLOCK_MAX_LENGTH
//   GL_ACTIVE_UNIFORMS
//   GL_ACTIVE_UNIFORM_MAX_LENGTH
//   GL_ATTACHED_SHADERS
//   GL_DELETE_STATUS
//   GL_INFO_LOG_LENGTH
//   GL_LINK_STATUS
//   GL_PROGRAM_BINARY_RETRIEVABLE_HINT
//   GL_TRANSFORM_FEEDBACK_BUFFER_MODE
//   GL_TRANSFORM_FEEDBACK_VARYINGS
//   GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH
//   GL_VALIDATE_STATUS

// static
std::string
GLES::getShaderInfoString( uint32_t shader )
{
   GLint n; ::glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &n );
   GL_VALIDATE
   if ( n < 1 ) { return ""; }
   std::vector< char > txt;
   txt.reserve( n );
   ::glGetShaderInfoLog( shader, txt.capacity(), NULL, txt.data() );
   GL_VALIDATE
   return txt.data();
}

//static
std::string
GLES::getProgramInfoString( uint32_t program )
{
   GLint n; ::glGetProgramiv( program, GL_INFO_LOG_LENGTH, &n );
   GL_VALIDATE
   if ( n < 1 ) { return ""; }
   std::vector< char > txt;
   txt.reserve( n );
   ::glGetProgramInfoLog( program, txt.capacity(), nullptr, txt.data() );
   GL_VALIDATE
   return txt.data();
}

// static
std::string
GLES::printShaderWithLineNumbers( std::string const & src )
{
   std::stringstream s;
   std::vector< std::string > lines = dbStrSplit( src, '\n' );
   for ( size_t i = 0; i < lines.size(); ++i )
   {
      if ( i < 100 ) s << "_";
      if ( i < 10 ) s << "_";
      s << i << " " << lines[ i ] << '\n';
   }
   return s.str();
}

//static
GLint
GLES::getProgramValue( uint32_t program, uint32_t query )
{
   GLint value;
   ::glGetProgramiv( program, query, &value );
   return value;
}

//static
bool
GLES::setUniformValue( uint32_t programId, std::string const & u_name, int u_value )
{
//   if ( !programId )
//   {
//      DE_ERROR( "Invalid programId ",programId,"." )
//      return false;
//   }

   uint32_t loc = glGetUniformLocation( programId, u_name.c_str() );
   GL_VALIDATE
   if ( loc < 0 )
   {
      DE_ERROR( "Invalid uniform ",u_name,"." )
      return false;
   }

   ::glUniform1i( loc, u_value );  // u_sampler, state
   GL_VALIDATE
   return true;
}

//static
bool
GLES::setUniformValue( uint32_t programId, std::string const & u_name, uint32_t u_colorRGBA )
{
//   if ( !programId )
//   {
//      DE_ERROR( "Invalid programId ",programId,"." )
//      return false;
//   }

   uint32_t loc = glGetUniformLocation( programId, u_name.c_str() );
   GL_VALIDATE
   if ( loc < 0 )
   {
      DE_ERROR( "Invalid uniform ",u_name,"." )
      return false;
   }

   ::glUniform1uiv( loc, 1, &u_colorRGBA );  // u_color, colorRGBA
   GL_VALIDATE
   return true;
}

//static
bool
GLES::setUniformValue( uint32_t programId, std::string const & u_name, glm::vec3 const & u_value )
{
//   if ( !programId )
//   {
//      DE_ERROR( "Invalid programId ",programId,"." )
//      return false;
//   }

   uint32_t loc = glGetUniformLocation( programId, u_name.c_str() );
   GL_VALIDATE
   if ( loc < 0 )
   {
      DE_ERROR( "Invalid uniform ",u_name,"." )
      return false;
   }

   glUniform3fv( loc, 1, glm::value_ptr( u_value ) );
   GL_VALIDATE
   return true;
}

//static
bool
GLES::setUniformValue( uint32_t programId, std::string const & u_name, glm::mat4 const & u_matrix )
{
//   if ( !programId )
//   {
//      DE_ERROR( "Invalid programId ",programId,"." )
//      return false;
//   }

   uint32_t loc = glGetUniformLocation( programId, u_name.c_str() );
   GL_VALIDATE
   if ( loc < 0 )
   {
      DE_ERROR( "Invalid uniform ",u_name,"." )
      return false;
   }

   ::glUniformMatrix4fv( loc, 1, GL_FALSE, glm::value_ptr( u_matrix ) );
   GL_VALIDATE
   return true;
}
// GLint GLES::glGetProgramInfoValue_GL_DELETE_STATUS( uint32_t program ) { return getProgramValue( program, GL_DELETE_STATUS ); }
// GLint glGetProgramInfoValue_GL_INFO_LOG_LENGTH( uint32_t program ) { return getProgramValue( program, GL_INFO_LOG_LENGTH ); }
// GLint glGetProgramInfoValue_GL_LINK_STATUS( uint32_t program ) { return getProgramValue( program, GL_LINK_STATUS ); }
// GLint glGetProgramInfoValue_GL_PROGRAM_BINARY_RETRIEVABLE_HINT( uint32_t program ) { return getProgramValue( program, GL_LINK_STATUS ); }
// GLint glGetProgramInfoValue_GL_TRANSFORM_FEEDBACK_BUFFER_MODE( uint32_t program ) { return getProgramValue( program, GL_TRANSFORM_FEEDBACK_BUFFER_MODE ); }
// GLint glGetProgramInfoValue_GL_TRANSFORM_FEEDBACK_VARYINGS( uint32_t program ) { return getProgramValue( program, GL_TRANSFORM_FEEDBACK_VARYINGS ); }
// GLint glGetProgramInfoValue_GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH( uint32_t program ) { return getProgramValue( program, GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH ); }
// GLint glGetProgramInfoValue_GL_VALIDATE_STATUS( uint32_t program ) { return getProgramValue( program, GL_VALIDATE_STATUS ); }
// GLint glGetProgramInfoValue_GL_ACTIVE_ATTRIBUTES( uint32_t program ) { return getProgramValue( program, GL_ACTIVE_ATTRIBUTES ); }
// GLint glGetProgramInfoValue_GL_ACTIVE_ATTRIBUTE_MAX_LENGTH( uint32_t program ) { return getProgramValue( program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH ); }
// GLint glGetProgramInfoValue_GL_ACTIVE_UNIFORMS( uint32_t program ) { return getProgramValue( program, GL_ACTIVE_UNIFORMS ); }
// GLint glGetProgramInfoValue_GL_ACTIVE_UNIFORM_MAX_LENGTH( uint32_t program ) { return getProgramValue( program, GL_ACTIVE_UNIFORM_MAX_LENGTH ); }
// GLint glGetProgramInfoValue_GL_ATTACHED_SHADERS( uint32_t program ) { return getProgramValue( program, GL_ATTACHED_SHADERS ); }
// GLint glGetProgramInfoValue_GL_ACTIVE_UNIFORM_BLOCK( uint32_t program ) { return getProgramValue( program, GL_ACTIVE_UNIFORM_BLOCK ); }
// GLint glGetProgramInfoValue_GL_ACTIVE_UNIFORM_BLOCK_MAX_LENGTH( uint32_t program ) { return getProgramValue( program, GL_ACTIVE_UNIFORM_BLOCK_MAX_LENGTH ); }

// void glGetActiveAttrib( uint32_t program, uint32_t index, GLsizei bufsize, GLsizei *length, GLenum *type, GLint *size, GLchar *name);
//   program - name of a program object that was successfully linked previously.
//   index - specifies the vertex attribute to query and will be a value between 0 and GL_ACTIVE_ATTRIBUTES – 1. The value of GL_ACTIVE_ATTRIBUTES is determined with glGetProgramiv .
//   bufsize - specifies the maximum number of characters that may be written into name , including the null terminator.
//   length - returns the number of characters written into name, excluding the null terminator, if length is not NULL .
//   type - returns the type of the attribute. Valid values are GL_FLOAT, GL_FLOAT_VEC2, GL_FLOAT_VEC3, GL_FLOAT_VEC4, GL_FLOAT_MAT2, GL_FLOAT_MAT3, GL_FLOAT_MAT4, GL_FLOAT_MAT2x3, GL_FLOAT_MAT2x4, GL_FLOAT_MAT3x2, GL_FLOAT_MAT3x4, GL_FLOAT_MAT4x2, GL_FLOAT_MAT_4x3, GL_INT, GL_INT_VEC2, GL_INT_VEC3, GL_INT_VEC4, GL_UNSIGNED_INT, GL_UNSIGNED_INT_VEC2, GL_UNSIGNED_INT_VEC3, GL_UNSIGNED_INT_VEC4
//   size - returns the size of the attribute. This is specified in units of the type returned by type . If the variable is not an array, size will always be 1. If the variable is an array, then size returns the size of the array.
//   name - name of the attribute variable as declared in the vertex shader.

// static
std::string
GLES::getActiveAttribString( uint32_t program )
{
   std::stringstream s;
   GLint attribCount = getProgramValue( program, GL_ACTIVE_ATTRIBUTES );
   //GLint maxAttribCount = getProgramValue( program, GL_MAX_ATTRIBUTES );
//   GLint maxUniformLen;
//   GLint maxUniforms;
   s << "[GL] Attributes.Count = " << attribCount << "\n";
//   s << "[GL] Uniforms.MaxLen = " << maxUniformLen << "\n";
//   std::vector< char > uniformName;
//   uniformName.reserve( maxUniformLen );
//   for ( GLint i = 0; i < maxUniforms; ++i )
//   {
//      GLint uniformSize;
//      GLenum uniformType;
//      ::glGetActiveUniform( program, i, maxUniformLen, NULL, &uniformSize, &uniformType, uniformName.data() );
//      GLint location = ::glGetUniformLocation ( program, uniformName.data() );

//      std::string uniformTypeStr;
//      switch ( uniformType )
//      {
//         case GL_FLOAT:       uniformTypeStr = "float"; break;
//         case GL_FLOAT_VEC2:  uniformTypeStr = "vec2"; break;
//         case GL_FLOAT_VEC3:  uniformTypeStr = "vec3"; break;
//         case GL_FLOAT_VEC4:  uniformTypeStr = "vec4"; break;
//         case GL_INT:         uniformTypeStr = "int"; break;
//         default: break;
//      }
//      s << "[GL] Uniforms[" << i << "] loc:" << location << ", name:" << uniformName.data() << ", type:" << uniformType << ":" << uniformTypeStr << ", size:" << uniformSize << "\n";
//   }
   return s.str();
}

// static
GLint
GLES::getUniformCount( uint32_t program )
{
   if (!program) {
      return 0;
   }
   GLint uniformCount = 0;
   ::glGetProgramiv( program, GL_ACTIVE_UNIFORMS, &uniformCount );
   return uniformCount;
}

// static
GLint
GLES::getUniformLenMax( uint32_t program )
{
   if (!program) {
      return 0;
   }
   GLint value = 0;
   ::glGetProgramiv( program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &value );
   return value;
}

// static
std::string
GLES::getUniformString( uint32_t program )
{
   std::stringstream s;
   s << "uniformCount(" << getUniformCount( program ) << ", "
         "lenMax(" << getUniformLenMax( program );

   /*
   std::vector< char > uniformName;
   uniformName.reserve( maxUniformLen );

   for ( GLint i = 0; i < maxUniforms; ++i )
   {
      GLint uniformSize;
      GLenum uniformType;
      ::glGetActiveUniform( program, i, maxUniformLen, NULL, &uniformSize, &uniformType, uniformName.data() );
      GLint location = ::glGetUniformLocation( program, uniformName.data() );

      std::string uniformTypeStr;
      switch ( uniformType )
      {
         case GL_FLOAT:       uniformTypeStr = "float"; break;
         case GL_FLOAT_VEC2:  uniformTypeStr = "vec2"; break;
         case GL_FLOAT_VEC3:  uniformTypeStr = "vec3"; break;
         case GL_FLOAT_VEC4:  uniformTypeStr = "vec4"; break;
         case GL_INT:         uniformTypeStr = "int"; break;
         default: break;
      }
      s << "[GL] Uniforms[" << i << "] loc:" << location << ", name:" << uniformName.data() << ", type:" << uniformType << ":" << uniformTypeStr << ", size:" << uniformSize << "\n";
   }
   */
   return s.str();
}

// ===========================================================================
// GL_EXT_texture_filter_anisotropic
// ===========================================================================
void
GLES::setAF( float af )
{
//   glTexParameterf( , GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f);
//   GLfloat maxAF = 0.0f;
//   glGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAF );
}

// GL_EXT_texture_filter_anisotropic
float
GLES::getAF()
{
   GLfloat maxAF = 0.0f;
   glGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAF );
   GL_VALIDATE
   return maxAF;
}

// ===========================================================================
// StateManager
// ===========================================================================

PolygonOffset
GLES::queryPolygonOffset()
{
   PolygonOffset polyfill;
   polyfill.enabled = ( ::glIsEnabled( GL_POLYGON_OFFSET_FILL ) != GL_FALSE );
   GL_VALIDATE

   GLfloat offsetFactor = 0.0f;
   ::glGetFloatv( GL_POLYGON_OFFSET_FACTOR, &offsetFactor );
   GL_VALIDATE
   polyfill.offsetFactor = offsetFactor;

   GLfloat offsetUnits = 0.0f;
   ::glGetFloatv( GL_POLYGON_OFFSET_UNITS, &offsetUnits );
   GL_VALIDATE
   polyfill.offsetUnits = offsetUnits;

   return polyfill;
}

PolygonOffset
GLES::applyPolygonOffset( PolygonOffset const & alt, PolygonOffset const & neu )
{
   if ( neu.enabled != alt.enabled )
   {
      if ( neu.enabled )
      {
         ::glEnable( GL_POLYGON_OFFSET_FILL ); GL_VALIDATE
         ::glPolygonOffset( neu.offsetFactor, neu.offsetUnits ); GL_VALIDATE
         DE_DEBUG("PolygonOffsetFill ON")
      }
      else
      {
         ::glDisable( GL_POLYGON_OFFSET_FILL ); GL_VALIDATE
         DE_DEBUG("PolygonOffsetFill OFF")
      }
   }

   return neu;
}

// ===========================================================================
// StateManager
// ===========================================================================

uint32_t
GLES::toClearMask( Clear const & clear )
{
   uint32_t mask = 0;
   if ( clear.hasColorBit() ) mask |= GL_COLOR_BUFFER_BIT;
   if ( clear.hasDepthBit() ) mask |= GL_DEPTH_BUFFER_BIT;
   if ( clear.hasStencilBit() ) mask |= GL_STENCIL_BUFFER_BIT;
   return mask;
}

Clear
GLES::queryClear()
{
   glm::vec4 color;
   ::glGetFloatv( GL_COLOR_CLEAR_VALUE, glm::value_ptr( color ) ); GL_VALIDATE
   GLfloat depth = 1.0f;
   ::glGetFloatv( GL_DEPTH_CLEAR_VALUE, &depth ); GL_VALIDATE
   GLint stencil = 0;
   ::glGetIntegerv( GL_STENCIL_CLEAR_VALUE, &stencil ); GL_VALIDATE
   Clear clear;
   clear.color = color;
   clear.depth = depth;
   clear.stencil = uint8_t( stencil );
   return clear;
}

/*
void
GLES::clearImpl( Clear const & state )
Clear
GLES::applyClear( Clear const & alt, Clear const & neu )
{
   // Different clear color
   if ( neu.color != alt.color )
   {
      //DE_DEBUG("Apply new clear color")
      ::glClearColor( neu.color.r, neu.color.g, neu.color.b, neu.color.a );
      GL_VALIDATE
   }
   // Different clear depth value
   if ( neu.depth != alt.depth )
   {
      //DE_DEBUG("Apply new clear depth")
      ::glClearDepthf( neu.depth );
      GL_VALIDATE
   }
   // Different clear stencil value
   if ( neu.stencil != alt.stencil )
   {
      //DE_DEBUG("Apply new clear stencil")
      ::glClearStencil( neu.stencil );
      GL_VALIDATE
   }
   // Actually do clear if mask differs from 0.
//   if ( neu.mask > 0 )
//   {
      //DE_DEBUG("Apply clear(", neu.toString(), ")")
      ::glClear( GLES::toClearMask( neu ) );
      GL_VALIDATE
//   }
   return neu;
}
*/

Clear
GLES::applyClear( Clear const & alt, Clear const & neu )
{
   // Different clear color
   if ( neu.color != alt.color )
   {
      //DE_DEBUG("Apply new clear color")
      ::glClearColor( neu.color.r, neu.color.g, neu.color.b, neu.color.a );
      GL_VALIDATE
   }
   // Different clear depth value
   if ( neu.depth != alt.depth )
   {
      //DE_DEBUG("Apply new clear depth")
      ::glClearDepthf( neu.depth );
      GL_VALIDATE
   }
   // Different clear stencil value
   if ( neu.stencil != alt.stencil )
   {
      //DE_DEBUG("Apply new clear stencil")
      ::glClearStencil( neu.stencil );
      GL_VALIDATE
   }
   // Actually do clear if mask differs from 0.
//   if ( neu.mask > 0 )
//   {
      //DE_DEBUG("Apply clear(", neu.toString(), ")")
//      ::glClear( GLES::toClearMask( neu ) );
//      GL_VALIDATE
//   }
   return neu;
}


Viewport
GLES::queryViewport()
{
   GLint vp[ 4 ];
   ::glGetIntegerv( GL_VIEWPORT, vp );
   GL_VALIDATE

   Viewport viewport;
   viewport.x = vp[ 0 ];
   viewport.y = vp[ 1 ];
   viewport.w = vp[ 2 ];
   viewport.h = vp[ 3 ];
   return viewport;
}

Viewport
GLES::applyViewport( Viewport const & alt, Viewport const & neu )
{
   if ( alt != neu )
   {
      ::glViewport( neu.x, neu.y, neu.w, neu.h );
      GL_VALIDATE
      //std::cout << "Changed Viewport from(" << this->toString() << " ) to( " << other.toString() << " )\n";
   }
   return neu;
}

DepthRange
GLES::queryDepthRange()
{
   GLfloat depthRange[ 2 ];
   ::glGetFloatv( GL_DEPTH_RANGE, depthRange ); GL_VALIDATE
   return DepthRange( depthRange[ 0 ], depthRange[ 1 ] );
}

DepthRange
GLES::applyDepthRange( DepthRange const & alt, DepthRange const & neu )
{
   if ( alt != neu )
   {
      ::glDepthRangef( neu.n, neu.f );
      GL_VALIDATE
      //std::cout << "Changed Viewport from(" << this->toString() << " ) to( " << other.toString() << " )\n";
   }
   return neu;
}

Scissor
GLES::queryScissor()
{
   Scissor scissor;
   scissor.enabled = ( ::glIsEnabled( GL_SCISSOR_TEST ) != GL_FALSE );
   GL_VALIDATE
   return scissor;
}

Scissor
GLES::applyScissor( Scissor const & alt, Scissor const & neu )
{
   if ( neu == alt ) { return neu; }
   if ( neu.enabled != alt.enabled )
   {
      ::glScissor( neu.x, neu.y, neu.w, neu.h );
      GL_VALIDATE

      ::glEnable( GL_SCISSOR_TEST );
      GL_VALIDATE
   }
   else
   {
      ::glDisable( GL_SCISSOR_TEST );
      GL_VALIDATE
   }

   return neu;
}

Culling
GLES::queryCulling()
{
   Culling culling;
   bool enabled = glIsEnabledDE( GL_CULL_FACE );
   culling.setEnabled( enabled );
   int cullMode = glGetIntegerDE( GL_CULL_FACE_MODE ); // GL_BACK|GL_FRONT|GL_FRONT_AND_BACK
   switch ( cullMode )
   {
      case GL_BACK: culling.back(); break;
      case GL_FRONT: culling.front(); break;
      case GL_FRONT_AND_BACK: culling.frontAndBack(); break;
      default: culling.back(); DE_ERROR("Unknown Cull mode, maybe not init") break;
   }

   bool isCW = ( GL_CW == glGetIntegerDE( GL_FRONT_FACE ) ); // GL_CCW|GL_CW
   if ( isCW )
      culling.cw(); // GL_CCW|GL_CW
   else
      culling.ccw(); // GL_CCW|GL_CW

   return culling;
}

Culling
GLES::applyCulling( Culling const & alt, Culling const & neu )
{
   if ( neu == alt )
   {
      return neu;
   }

   if ( neu.isEnabled() )
   {
      ::glEnable( GL_CULL_FACE );  GL_VALIDATE

      // [mode] GL_BACK, GL_FRONT
      GLenum mode = GL_BACK;
      if ( neu.isFrontAndBack() ) { mode = GL_FRONT_AND_BACK; }
      else if ( neu.isFront() ) { mode = GL_FRONT; }
      ::glCullFace( mode ); GL_VALIDATE

      // [windingOrder] CW,CCW
      ::glFrontFace( neu.isCCW() ? GL_CCW : GL_CW ); GL_VALIDATE

      //DE_DEBUG("Culling ON ", neu.toString())
   }
   else
   {
      ::glDisable( GL_CULL_FACE ); GL_VALIDATE
      //DE_DEBUG("Culling OFF")
   }

   return neu;
}


Depth::EFunc
GLES::toDepthFunction( GLint value )
{
   switch ( value )
   {
      case GL_LESS: return Depth::Less;
      case GL_LEQUAL: return Depth::LessEqual;
      case GL_GREATER: return Depth::Greater;
      case GL_GEQUAL: return Depth::GreaterEqual;
      case GL_EQUAL: return Depth::Equal;
      case GL_NOTEQUAL: return Depth::NotEqual;
      case GL_ALWAYS: return Depth::AlwaysPass;
      case GL_NEVER: return Depth::Never;
      default: return Depth::LessEqual;
   }
}

uint32_t
GLES::fromDepthFunction( Depth::EFunc value  )
{
   switch ( value )
   {
      case Depth::Less: return GL_LESS;
      case Depth::LessEqual: return GL_LEQUAL;
      case Depth::Greater: return GL_GREATER;
      case Depth::GreaterEqual: return GL_GEQUAL;
      case Depth::Equal: return GL_EQUAL;
      case Depth::NotEqual: return GL_NOTEQUAL;
      case Depth::AlwaysPass: return GL_ALWAYS;
      case Depth::Never: return GL_NEVER;
      default: return GL_LEQUAL;
   }
}


Depth
GLES::queryDepth()
{
   bool enabled = glIsEnabledDE( GL_DEPTH_TEST );
   bool zwrite = glGetBoolDE( GL_DEPTH_WRITEMASK );
   Depth::EFunc depthFunc = GLES::toDepthFunction( glGetIntegerDE( GL_DEPTH_FUNC ) );
   return Depth( enabled, zwrite, depthFunc );
}

Depth
GLES::applyDepth( Depth const & alt, Depth const & neu )
{
   if ( neu == alt ) { return neu; }

//   if ( neu == alt )
//   {
//      DE_DEBUG("Nothing todo neu(",neu.toString(),")")
//      return neu; // Nothing todo
//   }

   if ( neu.isEnabled() )
   {
      ::glEnable( GL_DEPTH_TEST );
      GL_VALIDATE

      ::glDepthFunc( GLES::fromDepthFunction( neu.getFunc() ) );
      GL_VALIDATE

      ::glDepthMask( neu.isZWriteEnabled() ? GL_TRUE : GL_FALSE );
      GL_VALIDATE

      //DE_DEBUG("DepthTest ON")
   }
   else
   {
      ::glDisable( GL_DEPTH_TEST );
      GL_VALIDATE
      //DE_DEBUG("DepthTest OFF")
   }

   return neu;
}

Stencil
GLES::queryStencil()
{
   Stencil stencil;
   stencil.enabled = glIsEnabledDE( GL_STENCIL_TEST );
   return stencil;
}

Stencil
GLES::applyStencil( Stencil const & alt, Stencil const & neu )
{
   if ( neu == alt )
   {
      return neu; // No difference, nothing todo.
   }

   if ( neu.enabled )
   {
      ::glDisable( GL_STENCIL_TEST ); GL_VALIDATE
      //::glEnable( GL_STENCIL_TEST ); GL_VALIDATE
      //::glStencilFunc( to ); GL_VALIDATE
      //::glStencilMask(); GL_VALIDATE
      //::glStencilOp(); GL_VALIDATE
      //DE_DEBUG( "Stencil ON ", neu.toString() )
   }
   else
   {
      ::glDisable( GL_STENCIL_TEST ); GL_VALIDATE
      //DE_DEBUG( "Stencil OFF" )
   }

//      std::cout << "Changed Stencil from(" << this->toString() << " ) to( " << neu.toString() << " )\n";
//      m_StencilFail = neu.m_StencilFail;
//      m_StencilKeep = neu.m_StencilKeep;
   return neu;
}

Blend::EEquation
GLES::toBlendEquation( GLint value )
{
   switch ( value )
   {
      case GL_FUNC_ADD: return Blend::Add;
      case GL_FUNC_SUBTRACT: return Blend::Sub;
      case GL_FUNC_REVERSE_SUBTRACT: return Blend::ReverseSub;
      case GL_MIN: return Blend::Min;
      case GL_MAX: return Blend::Max;
      default: return Blend::Add;
   }
}

uint32_t
GLES::fromBlendEquation( Blend::EEquation value  )
{
   switch ( value )
   {
      case Blend::Add: return GL_FUNC_ADD;
      case Blend::Sub: return GL_FUNC_SUBTRACT;
      case Blend::ReverseSub: return GL_FUNC_REVERSE_SUBTRACT;
      case Blend::Min: return GL_MIN;
      case Blend::Max: return GL_MAX;
      default: return GL_FUNC_ADD;
   }
}

Blend::EFunction
GLES::toBlendFunction( GLint value )
{
   switch ( value )
   {
      case GL_SRC_ALPHA: return Blend::SrcAlpha;
      case GL_ONE_MINUS_SRC_ALPHA: return Blend::OneMinusSrcAlpha;

      case GL_ZERO: return Blend::Zero;
      case GL_ONE: return Blend::One;

      case GL_DST_ALPHA: return Blend::DstAlpha;
      case GL_ONE_MINUS_DST_ALPHA: return Blend::OneMinusDstAlpha;
      case GL_SRC_ALPHA_SATURATE: return Blend::SrcAlphaSaturate;
      case GL_CONSTANT_ALPHA: return Blend::ConstantAlpha;

      case GL_SRC_COLOR: return Blend::SrcColor;
      case GL_ONE_MINUS_SRC_COLOR: return Blend::OneMinusSrcColor;
      case GL_DST_COLOR: return Blend::DstColor;
      case GL_ONE_MINUS_DST_COLOR: return Blend::OneMinusDstColor;

      case GL_CONSTANT_COLOR: return Blend::ConstantColor;
      case GL_ONE_MINUS_CONSTANT_COLOR: return Blend::OneMinusConstantColor;

      case GL_BLEND_COLOR: return Blend::BlendColor;
//         case GL_SRC1_ALPHA_EXT: return Blend::Src1Alpha;
//         case GL_ONE_MINUS_SRC1_ALPHA_EXT: return Blend::OneMinusSrc1Alpha;
//         case GL_SRC1_COLOR_EXT: return Blend::Src1Color;
//         case GL_ONE_MINUS_SRC1_COLOR_EXT: return Blend::OneMinusSrc1Color;
      default: return Blend::Zero;
   }
}

uint32_t
GLES::fromBlendFunction( Blend::EFunction value )
{
   switch ( value )
   {
      case Blend::SrcAlpha: return GL_SRC_ALPHA;
      case Blend::OneMinusSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA;

      case Blend::Zero: return GL_ZERO;
      case Blend::One: return GL_ONE;

      case Blend::DstAlpha: return GL_DST_ALPHA;
      case Blend::OneMinusDstAlpha: return GL_ONE_MINUS_DST_ALPHA;
      case Blend::SrcAlphaSaturate: return GL_SRC_ALPHA_SATURATE;
      case Blend::ConstantAlpha: return GL_CONSTANT_ALPHA;

      case Blend::SrcColor: return GL_SRC_COLOR;
      case Blend::OneMinusSrcColor: return GL_ONE_MINUS_SRC_COLOR;
      case Blend::DstColor: return GL_DST_COLOR;
      case Blend::OneMinusDstColor: return GL_ONE_MINUS_DST_COLOR;

      case Blend::ConstantColor: return GL_CONSTANT_COLOR;
      case Blend::OneMinusConstantColor: return GL_ONE_MINUS_CONSTANT_COLOR;

      case Blend::BlendColor: return GL_BLEND_COLOR;
//         case Blend::Src1Alpha: return GL_SRC1_ALPHA_EXT;
//         case Blend::OneMinusSrc1Alpha: return GL_ONE_MINUS_SRC1_ALPHA_EXT;
//         case Blend::Src1Color: return GL_SRC1_COLOR_EXT;
//         case Blend::OneMinusSrc1Color: return GL_ONE_MINUS_SRC1_COLOR_EXT;
      default: return GL_ZERO;
   }
}

Blend
GLES::queryBlend()
{
   Blend blend;
   // Blend enabled
   blend.enabled = glIsEnabledDE( GL_BLEND );
   // Blend equation
   blend.equation = GLES::toBlendEquation( glGetIntegerDE( GL_BLEND_EQUATION ) );
   // Blend source RGB factor
   blend.src_rgb = GLES::toBlendFunction( glGetIntegerDE( GL_BLEND_SRC_RGB ) );
   // Blend destination RGB factor
   blend.dst_rgb = GLES::toBlendFunction( glGetIntegerDE( GL_BLEND_DST_RGB ) );
   // Blend source Alpha factor
   blend.src_a = GLES::toBlendFunction( glGetIntegerDE( GL_BLEND_SRC_ALPHA ) );
   // Blend destination Alpha factor
   blend.dst_a = GLES::toBlendFunction( glGetIntegerDE( GL_BLEND_DST_ALPHA ) );
   return blend;
}

Blend
GLES::applyBlend( Blend const & alt, Blend const & neu )
{
   if ( neu == alt )
   {
      return neu; // Nothing todo
   }

//   if ( neu.enabled != alt.enabled )
//   {
   if ( neu.enabled )
   {
      ::glEnable( GL_BLEND ); GL_VALIDATE

      ::glBlendEquation(
            GLES::fromBlendEquation( neu.equation ) ); GL_VALIDATE
      ::glBlendFunc(
            GLES::fromBlendFunction( neu.src_a ),
            GLES::fromBlendFunction( neu.dst_a ) ); GL_VALIDATE
//         ::glBlendEquationSeparate(
//               GLES::fromBlendEquation( neu.equation ),
//               GLES::fromBlendEquation( neu.equation ) ); GL_VALIDATE
//         ::glBlendFuncSeparate(
//               GLES::fromBlendFunction( neu.src_rgb ),
//               GLES::fromBlendFunction( neu.dst_rgb ),
//               GLES::fromBlendFunction( neu.src_a ),
//               GLES::fromBlendFunction( neu.dst_a ) ); GL_VALIDATE
      //DE_DEBUG("Blending ON")
   }
   else
   {
      ::glDisable( GL_BLEND ); GL_VALIDATE
      //DE_DEBUG("Blending OFF")
   }

   return neu;
}

RasterizerDiscard
GLES::queryRasterizerDiscard()
{
   RasterizerDiscard rdiscard;
   rdiscard.enabled = ( ::glIsEnabled( GL_RASTERIZER_DISCARD ) != GL_FALSE );
   GL_VALIDATE
   return rdiscard;
}

RasterizerDiscard
GLES::applyRasterizerDiscard( RasterizerDiscard const & alt,
                              RasterizerDiscard const & neu )
{
   if ( neu.enabled != alt.enabled )
   {
      if ( neu.enabled )
      {
         ::glEnable( GL_RASTERIZER_DISCARD ); GL_VALIDATE
         DE_DEBUG("RasterizerDiscard ON")
      }
      else
      {
         ::glDisable( GL_RASTERIZER_DISCARD ); GL_VALIDATE
         DE_DEBUG("RasterizerDiscard OFF")
      }
   }
   return neu;
}

LineWidth
GLES::queryLineWidth()
{
   LineWidth state;

   GLfloat line_width;
   ::glGetFloatv( GL_LINE_WIDTH, &line_width ); GL_VALIDATE
   state.m_Now = line_width;

   GLfloat line_range[2];
   ::glGetFloatv( GL_ALIASED_LINE_WIDTH_RANGE, line_range ); GL_VALIDATE
   state.m_Min = line_range[ 0 ];
   state.m_Max = line_range[ 1 ];

   return state;
}

LineWidth
GLES::applyLineWidth( LineWidth const & alt, LineWidth const & neu )
{
   if ( std::abs( neu.m_Now - alt.m_Now ) > std::numeric_limits< float32_t >::epsilon() )
   {
      //DE_DEBUG( "glLineWidth(", neu.m_Now, ")" )
      ::glLineWidth( neu.m_Now );
      bool ok = GL_VALIDATE;
      if ( !ok )
      {
         DE_ERROR( "glLineWidth(", neu.m_Now, ") invalid value" )
      }

   }
   return neu;
}

PointSize
GLES::queryPointSize()
{
   PointSize state;
   //GLfloat ps_now;
   //::glGetFloatv( GL_ALPOINT_SIZE, &ps_now ); GL_VALIDATE
   state.m_Now = 1.0f;

   GLfloat ps_range[2];
   ::glGetFloatv( GL_ALIASED_POINT_SIZE_RANGE, ps_range ); GL_VALIDATE
   state.m_Min = ps_range[ 0 ];
   state.m_Max = ps_range[ 1 ];
   return state;
}

PointSize
GLES::applyPointSize( PointSize const & alt, PointSize const & neu )
{
   // if ( other == *this )
   // {
      // return;
   // }

   // //::glPointSize( other.m_Now );
   // std::cout << "Changed PointSize from(" << this->toString() << " ) to( " << other.toString() << " )\n";
   // m_Now = other.m_Now;
   return neu;
}

State
GLES::queryState()
{
   State state;
   //state.viewport = queryViewport();
   //state.scissor = queryScissor();
   //state.clear = queryClear();
   state.culling = queryCulling();
   state.depth = queryDepth();
   state.stencil = queryStencil();
   state.blend = queryBlend();
   state.pointSize = queryPointSize();
   state.lineWidth = queryLineWidth();
   state.rasterizerDiscard = queryRasterizerDiscard();
   state.polygonOffset = queryPolygonOffset();
   return state;
}

State
GLES::applyState( State const & alt, State const & neu )
{
   State state;
   // Dont apply viewport
   // Dont apply scissor
   // Dont apply clear
   //state.depthRange = applyCulling( alt.depthRange, neu.depthRange );
   state.culling = applyCulling( alt.culling, neu.culling );
   state.depth = applyDepth( alt.depth, neu.depth );
   state.stencil = applyStencil( alt.stencil, neu.stencil );
   state.blend = applyBlend( alt.blend, neu.blend );
   state.pointSize = applyPointSize( alt.pointSize, neu.pointSize );
   state.lineWidth = applyLineWidth( alt.lineWidth, neu.lineWidth );
   state.rasterizerDiscard = applyRasterizerDiscard( alt.rasterizerDiscard, neu.rasterizerDiscard );
   state.polygonOffset = applyPolygonOffset( alt.polygonOffset, neu.polygonOffset );

   return state;
}

// ============================================================================
// SO
// ============================================================================
void
GLES::setSamplerOptions2D( SO const & so )
{
   // [Magnify Filter]
   GLint magMode = GL_NEAREST; // Low quality is default.
   if ( so.mag == SO::MagLinear ) { magMode = GL_LINEAR; }// High quality on demand.
   ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magMode );
   GL_VALIDATE

   // [Minify Filter]
   GLint minMode = GL_NEAREST;// Low quality is default.
   switch ( so.min )
   {
      // Higher qualities on demand.
      case SO::MinLinear: minMode = GL_LINEAR; break;
      case SO::MinMipmapNearestNearest: minMode = GL_NEAREST_MIPMAP_NEAREST; break;
      case SO::MinMipmapNearestLinear: minMode = GL_NEAREST_MIPMAP_LINEAR; break;
      case SO::MinMipmapLinearNearest: minMode = GL_LINEAR_MIPMAP_NEAREST; break;
      case SO::MinMipmapLinearLinear: minMode = GL_LINEAR_MIPMAP_LINEAR; break;
      default: break;
   }
   ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minMode );
   GL_VALIDATE

   // [WrapS]
   GLint wrapMode = GL_CLAMP_TO_EDGE; // GL_REPEAT;
   switch ( so.wrapS )
   {
      case SO::Repeat: wrapMode = GL_REPEAT; break;
      case SO::RepeatMirrored: wrapMode = GL_MIRRORED_REPEAT; break;
      //case SO::ClampToEdge: wrapMode = GL_CLAMP_TO_EDGE; break;
      #ifdef GL_ES_VERSION_3_2
      case SO::ClampToBorder: wrapMode = GL_CLAMP_TO_BORDER; break;
      #endif
      default: break;
   }
   ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode );
   GL_VALIDATE

   // [WrapT]
   wrapMode = GL_CLAMP_TO_EDGE; // GL_REPEAT
   switch ( so.wrapT )
   {
      case SO::Repeat: wrapMode = GL_REPEAT; break;
      case SO::RepeatMirrored: wrapMode = GL_MIRRORED_REPEAT; break;
      //case SO::ClampToEdge: wrapMode = GL_CLAMP_TO_EDGE; break;
      #ifdef GL_ES_VERSION_3_2
      case SO::ClampToBorder: wrapMode = GL_CLAMP_TO_BORDER; break;
      #endif
      default: break;
   }
   ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
   GL_VALIDATE

   // [WrapR] Not used so far.

   // [Anisotropic Filtering] AF | needs GL_EXT_texture_filter_anisotropic ( >= 3.0 ) so its there.
   if ( so.anisotropicFilter > 0 )
   {
      ::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, float( so.anisotropicFilter ) );
      GL_VALIDATE
   }
}

// ============================================================================
// Texture
// ============================================================================
uint32_t
GLES::createTexture()
{
   uint32_t texId = 0;
   ::glGenTextures( 1, &texId );
   GL_VALIDATE
   return texId;
}

// ============================================================================
// Texture 2D
// ============================================================================
uint32_t
GLES::createTexture2D( int32_t w, int32_t h, SO const & so )
{
   uint32_t texId = 0;
   ::glGenTextures(1, &texId);
   GL_VALIDATE
   ::glBindTexture(GL_TEXTURE_2D, texId );
   GL_VALIDATE
   ::glTexImage2D(GL_TEXTURE_2D,
               0,             // level
               GL_RGBA,       // internalFormat
               GLsizei( w ),  // width
               GLsizei( h ),  // height
               0,             // border
               GL_RGBA,       // format
               GL_UNSIGNED_BYTE, // type
               nullptr        // pixels
   );
   GL_VALIDATE
   setSamplerOptions2D( so );

   // requires mipmaps. Generate them automatically.
   if ( so.hasMipmaps() )
   {
      ::glGenerateMipmap( GL_TEXTURE_2D );
      GL_VALIDATE
   }
   return texId;
}

// static
uint32_t
GLES::createTexture2D( Image const & img, SO const & so )
{
   ColorFormat::EColorFormat fmt = img.getFormat();

   if ( ( fmt != ColorFormat::RGBA8888 ) &&
        ( fmt != ColorFormat::RGB888 ) )
   {
      DE_ERROR("Unsupported image format ", img.toString() )
      return 0;
   }

   GLuint texId;
   ::glGenTextures( 1, &texId );
   bool ok = GL_VALIDATE;
   if ( !ok )
   {
      DE_ERROR("Cant gen texId" )
      return 0;
   }

   if ( texId < 1 )
   {
      DE_ERROR("No gen texture" )
      return 0;
   }

   ::glBindTexture( GL_TEXTURE_2D, texId );
   ok = GL_VALIDATE;
   if ( !ok )
   {
      DE_ERROR("Cant bind texId(", texId, ")" )
      return texId;
   }

   GLenum texInternal = GL_RGBA;
   GLenum texFormat = GL_RGBA;
   GLenum texType = GL_UNSIGNED_BYTE;

   if ( fmt == ColorFormat::RGB888 )
   {
      DE_WARN("[Benni] Possible unsupported! ", img.toString() )
      texInternal = GL_RGB;
      texFormat = GL_RGB;
      texType = GL_UNSIGNED_BYTE;
   }

   // Poor filtering, or ...
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); GL_VALIDATE
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); GL_VALIDATE

   setSamplerOptions2D( so );

   // Give the image to OpenGL
   ::glTexImage2D( GL_TEXTURE_2D,   // GLenum target
                  0,                // GLint level
                  texInternal,          // GLenum internalFormat
                  GLsizei( img.m_Width ),     // GLsizei width
                  GLsizei( img.m_Height ),     // GLsizei height
                  0,                // GLint border
                  texFormat,        // GLenum format
                  texType,          // GLenum type
                  img.readPtr< void >() ); // void const * pixels
   ok = GL_VALIDATE;
   if ( !ok )
   {
      DE_ERROR("Cant upload texId(", texId, ")" )
      return texId;
   }

   // requires mipmaps. Generate them automatically.
   if ( so.hasMipmaps() )
   {
      glGenerateMipmap( GL_TEXTURE_2D );
      GL_VALIDATE
   }

   glBindTexture(GL_TEXTURE_2D, 0);
   GL_VALIDATE

   return texId;
}

void
GLES::deleteTexture( uint32_t & texId )
{
   if ( texId )
   {
      glDeleteTextures( 1, &texId );
      GL_VALIDATE
      texId = 0;
   }
}

bool
GLES::bindTexture2D( uint32_t texId )
{
   if ( texId < 1 )
   {
      DE_ERROR("Nothing to bind")
      return false;
   }

   glBindTexture( GL_TEXTURE_2D, texId );
   bool ok = GL_VALIDATE
   if ( !ok )
   {
      DE_ERROR("Cant bind texture2D ", texId )
      return false;
   }

   DE_DEBUG("Bind texture2D ", texId)
   return true;
}

// ============================================================================
//    FBO
// ============================================================================
uint32_t
GLES::createFBO()
{
   uint32_t fbo = 0;
   glGenFramebuffers( 1, &fbo );
   GL_VALIDATE
   glBindFramebuffer( GL_FRAMEBUFFER, fbo );
   GL_VALIDATE
   // glDrawBuffer();
   return fbo;
}

bool
GLES::bindFBO( uint32_t fbo )
{
   if ( fbo < 1 ) { DE_ERROR("No fbo") return false; }

   glBindFramebuffer( GL_FRAMEBUFFER, uint32_t( fbo ) );
   bool ok = GL_VALIDATE
   if ( !ok )
   {
      DE_ERROR("No bind fbo")
      return false;
   }

   // glDrawBuffer();
   return true;
}

void
GLES::deleteFBO( uint32_t & fbo )
{
   if ( fbo < 1 ) return;
   uint32_t id = uint32_t( fbo );
   glDeleteFramebuffers( 1, &id ); GL_VALIDATE
   fbo = 0;
}

// ============================================================================
// RBO
// ============================================================================
void
GLES::deleteRenderbuffer( uint32_t & rbo )
{
   if ( rbo )
   {
      glDeleteRenderbuffers( 1, &rbo );
      GL_VALIDATE
      rbo = 0;
   }
}

uint32_t
GLES::createDepthRenderBuffer( int32_t w, int32_t h )
{
   uint32_t rbo = 0;
   glGenRenderbuffers( 1, &rbo );
   bool ok = GL_VALIDATE
   if ( !ok )
   {
      DE_ERROR("Cannot gen depth render buffer(", w, ",", h, ")")
      return 0;
   }

   glBindRenderbuffer( GL_RENDERBUFFER, rbo );
   ok = GL_VALIDATE
   if ( !ok )
   {
      DE_ERROR("Cannot bind depth render buffer(", w, ",", h, ")")
      return 0;
   }

   glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h );
   ok = GL_VALIDATE
   if ( !ok )
   {
      DE_ERROR("Cannot set depth render buffer(", w, ",", h, ")")
      return 0;
   }
   return rbo;
}


//    internalFormat          format            type
// ============================================================================
//GL_RGB                GL_RGB               GL_UNSIGNED_BYTE 8/8/8 RGB 24-bit
//GL_RGB                GL_RGB               GL_UNSIGNED_SHORT_5_6_5 5/6/5 RGB 16-bit
//GL_RGBA               GL_RGBA              GL_UNSIGNED_BYTE 8/8/8/8 RGBA 32-bit
//GL_RGBA               GL_RGBA              GL_UNSIGNED_SHORT_4_4_4_4 4/4/4/4 RGBA 16-bit
//GL_RGBA               GL_RGBA              GL_UNSIGNED_SHORT_5_5_5_1 5/5/5/1 RGBA 16-bit
//GL_LUMINANCE_ALPHA    GL_LUMINANCE_ALPHA   GL_UNSIGNED_BYTE 8/8 LA 16-bit
//GL_LUMINANCE          GL_LUMINANCE         GL_UNSIGNED_BYTE 8L 8-bit
//GL_ALPHA              GL_ALPHA             GL_UNSIGNED_BYTE 8A 8-bit
// ============================================================================
// Integer formats
//GL_R8UI               GL_RED_INTEGER       GL_UNSIGNED_BYTE 8-bit Red (unsigned int)
//GL_R8I                GL_RED_INTEGER       GL_BYTE 8-bit Red (signed int)
//GL_R16UI              GL_RED_INTEGER       GL_UNSIGNED_SHORT 16-bit Red (unsigned int)
//GL_R16I               GL_RED_INTEGER       GL_SHORT 16-bit Red (signed int)
//GL_R32UI              GL_RED_INTEGER       GL_UNSIGNED_INT 32-bit Red (unsigned int)
//GL_R32I               GL_RED_INTEGER       GL_INT 32-bit Red (signed int)
//GL_RG8UI              GL_RG_INTEGER        GL_UNSIGNED_BYTE 8/8 RG (unsigned int)
//GL_RG8I               GL_RG_INTEGER        GL_BYTE 8/8 RG (signed int)
//GL_RG16UI             GL_RG_INTEGER        GL_UNSIGNED_SHORT 16/16 RG (unsigned int)
//GL_RG16I              GL_RG_INTEGER        GL_SHORT 16/16 RG (signed int)
//GL_RG32UI             GL_RG_INTEGER        GL_UNSIGNED_INT 32/32 RG (unsigned int)
//GL_RG32I              GL_RG_INTEGER        GL_INT 32/32 RG (signed int)
//GL_RGBAUI             GL_RGBA_INTEGER      GL_UNSIGNED_BYTE 8/8/8/8 RGBA(unsigned int)
//GL_RGBAI              GL_RGBA_INTEGER      GL_BYTE 8/8/8/8 RGBA (signed int)
//GL_RGB8UI             GL_RGB_INTEGER       GL_UNSIGNED_BYTE 8/8/8 RGB (unsigned int)
//GL_RGB8I              GL_RGB_INTEGER       GL_BYTE 8/8/8 RGB (signed int)
//GL_RGB16UI            GL_RGB_INTEGER       GL_UNSIGNED_SHORT 16/16/16 RGB (unsigned int)
//GL_RGB16I             GL_RGB_INTEGER       GL_SHORT 16/16/16 RGB (signed int)
//GL_RGB32UI            GL_RGB_INTEGER       GL_UNSIGNED_INT 32/32/32 RGB (unsigned int)
//GL_RGB32I             GL_RGB_INTEGER       GL_INT 32/32/32 RG (signed int)
//GL_RG32I              GL_RG_INTEGER        GL_INT 32/32 RG (signed int)
//GL_RGB10_A2_UI        GL_RGBA_INTEGER      GL_UNSIGNED_INT_2_10_10_10_REV 10/10/10/2 RGBA (unsigned int)
//GL_RGBA16UI           GL_RGBA_INTEGER      GL_UNSIGNED_SHORT 16/16/16/16 RGBA (unsigned int)
//GL_RGBA16I            GL_RGBA_INTEGER      GL_SHORT 16/16/16/16 RGBA (signed int)
//GL_RGBA32UI           GL_RGBA_INTEGER      GL_UNSIGNED_INT 32/32/32/32 R/G/B/A (unsigned int)
//GL_RGBA32I            GL_RGBA_INTEGER      GL_INT 32/32/32/32 R/G/B/A (signed int)
// ============================================================================
//GL_DEPTH_COMPONENT16  GL_DEPTH_COMPONENT   GL_UNSIGNED_SHORT
//GL_DEPTH_COMPONENT16  GL_DEPTH_COMPONENT   GL_UNSIGNED_INT
//GL_DEPTH_COMPONENT24  GL_DEPTH_COMPONENT   GL_UNSIGNED_INT
//GL_DEPTH_COMPONENT32F GL_DEPTH_COMPONENT   GL_FLOAT
//GL_DEPTH24_STENCIL8   GL_DEPTH_STENCIL     GL_UNSIGNED_INT_24_8
//GL_DEPTH32F_STENCIL8  GL_DEPTH_STENCIL     GL_FLOAT_32_UNSIGNED_INT_24_8_REV

uint32_t
GLES::createTextureDepth24( int32_t w, int32_t h, SO const & so )
{
   uint32_t texId = 0; glGenTextures( 1, &texId ); GL_VALIDATE
   glBindTexture( GL_TEXTURE_2D, texId ); GL_VALIDATE
   glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, w, h, 0,
                  GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, nullptr ); GL_VALIDATE
   setSamplerOptions2D( so );
   return texId;
}

uint32_t
GLES::createTextureDepth32f( int32_t w, int32_t h, SO const & so )
{
   uint32_t texId = 0; glGenTextures( 1, &texId ); GL_VALIDATE
   glBindTexture( GL_TEXTURE_2D, texId ); GL_VALIDATE
   glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, w, h, 0,
                  GL_DEPTH_COMPONENT, GL_FLOAT, nullptr ); GL_VALIDATE
   setSamplerOptions2D( so );
   return texId;
}

uint32_t
GLES::createTextureDepth24Stencil8( int32_t w, int32_t h, SO const & so )
{
   uint32_t texId = 0; glGenTextures( 1, &texId ); GL_VALIDATE
   glBindTexture( GL_TEXTURE_2D, texId ); GL_VALIDATE
   glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, w, h, 0,
                  GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr ); GL_VALIDATE
   setSamplerOptions2D( so );
   return texId;
}

uint32_t
GLES::createTextureDepth32fStencil8( int32_t w, int32_t h, SO const & so )
{
   uint32_t texId = 0; glGenTextures( 1, &texId ); GL_VALIDATE
   glBindTexture( GL_TEXTURE_2D, texId ); GL_VALIDATE
   glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, w, h, 0, GL_DEPTH_STENCIL,
                  GL_FLOAT_32_UNSIGNED_INT_24_8_REV, nullptr ); GL_VALIDATE
   setSamplerOptions2D( so );
   return texId;
}





} // end namespace gpu.
} // end namespace de.
