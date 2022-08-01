/// ==========================================================================
/// @file GLES.hpp
/// @author 2018 Benjamin Hampe <benjaminhampe@gmx.de>
/// @copyright Free open source software
/// ==========================================================================

#ifndef DE_GPU_GLES_HPP
#define DE_GPU_GLES_HPP

#include <de/gpu/Tex.hpp>
#include <de/gpu/Shader.hpp>
#include <de_opengl.hpp>

namespace de {
namespace gpu {

// ===========================================================================
struct GLES
// ===========================================================================
{
   DE_CREATE_LOGGER("de.gpu.GLES")

   static Uniform::EType
   toEUniformType( GLenum const uniformType );

   static GLenum
   fromEUniformType( Uniform::EType const uniformType );

//   static bool
//   setActiveShader( IShader* shader );

//   static bool
//   setActiveTexture( int stage, ITexture* tex );

   // [Anisotropic Filtering] AF
   static void
   setAF( float af );

   static float
   getAF();

   // =======================================================
   // FVFUtils
   // =======================================================

   // I. converter to OpenGL backend
   static uint32_t
   fromVertexAttribType( VertexAttribType::EType type );

   // I. converter from OpenGL backend
   static VertexAttribType::EType
   toVertexAttribType( uint32_t type );

   static uint32_t
   fromPrimitiveType( PrimitiveType::EType const primitiveType );

   static PrimitiveType::EType
   toPrimitiveType( uint32_t const primitiveType );

   static uint32_t
   fromIndexType( IndexType::EType const indexType );

   static bool
   enableAttributesGL( FVF const & fvf, uint8_t const * vertices );

   static bool
   disableAttributesGL( FVF const & fvf );

   static bool
   drawPrimitiveList( std::string const & callerName,
      PrimitiveType::EType primitiveType,
      void const* vertices,
      uint32_t vertexCount,
      FVF const & fvf,
      void const* indices = nullptr,
      uint32_t indexCount = 0,
      IndexType::EType indexType = IndexType::U32,
      bool debug = false );

#ifdef USE_SMESH_GEOMETRY

   static bool
   drawPrimitiveList( std::string const & caller,
      IMeshBuffer const & buf );

#endif

   // ===========================================================================
   // StateUtils
   // ===========================================================================
   static uint32_t   toClearMask( Clear const & clear );
   static Clear      queryClear();
   static Clear      applyClear( Clear const & old_state, Clear const & new_state );
   static DepthRange queryDepthRange();
   static DepthRange applyDepthRange( DepthRange const & old_state, DepthRange const & new_state );
   static Viewport   queryViewport();
   static Viewport   applyViewport( Viewport const & old_state, Viewport const & new_state );
   static Scissor    queryScissor();
   static Scissor    applyScissor( Scissor const & old_state, Scissor const & new_state );
   static Culling    queryCulling();
   static Culling    applyCulling( Culling const & old_state, Culling const & new_state );
   static Depth      queryDepth();
   static Depth      applyDepth( Depth const & old_state, Depth const & new_state );
   static Stencil    queryStencil();
   static Stencil    applyStencil( Stencil const & old_state, Stencil const & new_state );
   static Blend      queryBlend();
   static Blend      applyBlend( Blend const & old_state, Blend const & new_state );
   static LineWidth  queryLineWidth();
   static LineWidth  applyLineWidth( LineWidth const & old_state, LineWidth const & new_state );
   static PointSize  queryPointSize();
   static PointSize  applyPointSize( PointSize const & old_state, PointSize const & new_state );
   static PolygonOffset queryPolygonOffset();
   static PolygonOffset applyPolygonOffset( PolygonOffset const & old_state, PolygonOffset const & new_state );
   static RasterizerDiscard   queryRasterizerDiscard();
   static RasterizerDiscard   applyRasterizerDiscard( RasterizerDiscard const & old_state, RasterizerDiscard const & new_state );
   // Combined render states.
   static State      queryState();
   static State      applyState( State const & old_state, State const & new_state );

   // =======================================================================
   // Helpers that can be removed by inlining
   // =======================================================================

   static Depth::EFunc
   toDepthFunction( GLint value );
   static uint32_t
   fromDepthFunction( Depth::EFunc value );

   static Blend::EEquation
   toBlendEquation( GLint value );
   static uint32_t
   fromBlendEquation( Blend::EEquation value );
   static Blend::EFunction
   toBlendFunction( GLint value );
   static uint32_t
   fromBlendFunction( Blend::EFunction value );


   // =======================================================================
   // ShaderUtils
   // =======================================================================
   static uint32_t
   fromShaderType( ShaderType::EType const shaderType );
   static uint32_t
   compileShader( ShaderType::EType shaderType, std::string const & source, bool debug = false );
   static uint32_t
   linkProgram( std::string const & name, std::string const & vs, std::string const & fs, bool debug = false );
   static void
   deleteProgram( uint32_t & program );

   static GLint
   getProgramValue( uint32_t program, uint32_t query );
   static bool
   setUniformValue( uint32_t programId, std::string const & u_name, int u_value );
   static bool
   setUniformValue( uint32_t programId, std::string const & u_name, uint32_t u_colorRGBA );
   static bool
   setUniformValue( uint32_t programId, std::string const & u_name, glm::vec3 const & u_value );
   static bool
   setUniformValue( uint32_t programId, std::string const & u_name, glm::mat4 const & u_matrix );

   static std::string
   printShaderWithLineNumbers( std::string const & src );
   static std::string
   getShaderInfoString( uint32_t shader );
   static std::string
   getProgramInfoString( uint32_t program );

   // void glGetActiveAttrib( uint32_t program, uint32_t index, GLsizei bufsize, GLsizei *length, GLenum *type, GLint *size, GLchar *name);
   //   program - name of a program object that was successfully linked previously.
   //   index - specifies the vertex attribute to query and will be a value between 0 and GL_ACTIVE_ATTRIBUTES – 1. The value of GL_ACTIVE_ATTRIBUTES is determined with glGetProgramiv .
   //   bufsize - specifies the maximum number of characters that may be written into name , including the null terminator.
   //   length - returns the number of characters written into name, excluding the null terminator, if length is not NULL .
   //   type - returns the type of the attribute. Valid values are GL_FLOAT, GL_FLOAT_VEC2, GL_FLOAT_VEC3, GL_FLOAT_VEC4, GL_FLOAT_MAT2, GL_FLOAT_MAT3, GL_FLOAT_MAT4, GL_FLOAT_MAT2x3, GL_FLOAT_MAT2x4, GL_FLOAT_MAT3x2, GL_FLOAT_MAT3x4, GL_FLOAT_MAT4x2, GL_FLOAT_MAT_4x3, GL_INT, GL_INT_VEC2, GL_INT_VEC3, GL_INT_VEC4, GL_UNSIGNED_INT, GL_UNSIGNED_INT_VEC2, GL_UNSIGNED_INT_VEC3, GL_UNSIGNED_INT_VEC4
   //   size - returns the size of the attribute. This is specified in units of the type returned by type . If the variable is not an array, size will always be 1. If the variable is an array, then size returns the size of the array.
   //   name - name of the attribute variable as declared in the vertex shader.

   static std::string
   getActiveAttribString( uint32_t program );
   static GLint
   getUniformCount( uint32_t program );
   static GLint
   getUniformLenMax( uint32_t program );
   static std::string
   getUniformString( uint32_t program );

   // ===========================================================================
   // TextureUtils ( mostly for testing, real stuff is done in Texture classes )
   // ===========================================================================
   constexpr static uint32_t const InvalidTexture = 0xFFFFFFFF;

   // =========================================================================
   //    RBO - RenderBufferObject
   // =========================================================================
   static void
   deleteRenderbuffer( uint32_t & rbo );
   static uint32_t
   createDepthRenderBuffer( int32_t w, int32_t h );
   // =========================================================================
   //    FBO - FrameBufferObject
   // =========================================================================
   static void
   deleteFBO( uint32_t & id );
   static uint32_t
   createFBO();
   static bool
   bindFBO( uint32_t fbo );
/*
   static uint32_t createColorTextureAttachment( int32_t w, int32_t h, SO const & so = SO() );
   static uint32_t createDepthTextureAttachment( int32_t w, int32_t h, SO const & so = SO() );
   static uint32_t createDepthBufferAttachment( int32_t w, int32_t h );
*/
   // =========================================================================
   //    Textures, color, depth, stencil
   // =========================================================================
   static void
   deleteTexture( uint32_t & texId );
   static uint32_t
   createTexture();

   static bool
   bindTexture2D( uint32_t texId );
   static void
   setSamplerOptions2D( SO const & so );

   static uint32_t
   createTexture2D( int32_t w, int32_t h, SO const & so );
   static uint32_t
   createTexture2D( Image const & img, SO const & so ); // Should be most efficient this way.
// New:
   static uint32_t
   createTextureDepth24( int32_t w, int32_t h, SO const & so );
   static uint32_t
   createTextureDepth32f( int32_t w, int32_t h, SO const & so );
   static uint32_t
   createTextureDepth24Stencil8( int32_t w, int32_t h, SO const & so );
   static uint32_t
   createTextureDepth32fStencil8( int32_t w, int32_t h, SO const & so );
};


} // end namespace gpu.
} // end namespace de.

#endif

