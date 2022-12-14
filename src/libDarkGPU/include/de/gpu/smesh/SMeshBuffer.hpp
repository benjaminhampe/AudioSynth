#ifndef DE_GPU_SMESHBUFFER_HPP
#define DE_GPU_SMESHBUFFER_HPP

#include <de/gpu/smesh/TMeshBuffer.hpp>
#include <de/gpu/S3DVertex.hpp>

namespace de {
namespace gpu {

// ===========================================================================
// S3DVertex - FVF_POSITION_XYZ | FVF_NORMAL_XYZ | FVF_RGBA | FVF_TEXCOORD0
//
//    The standard Vertex type for this engine's internal material shader.
// ===========================================================================

typedef TMeshBuffer< S3DVertex > SMeshBuffer;



// ===========================================================================
struct SMeshBufferTool
// ===========================================================================
{
   DE_CREATE_LOGGER("de.gpu.SMeshBufferTool")

   static void
   transformPoints( std::vector< glm::vec3 > & points,
      glm::dmat4 const & modelMat );

   static void
   rotatePoints( std::vector< glm::vec3 > & points,
      float a, float b, float c );

   static void
   rotateTexCoords90_onlyQuads( de::gpu::SMeshBuffer & mesh );

   static SMeshBuffer
   tesselate( SMeshBuffer const & src );

   static void
   fuse( SMeshBuffer & dst, SMeshBuffer const & src );


   static std::vector< Triangle3f >
   createCollisionTriangles( SMeshBuffer const & b );

   static void
   computeBoundingBox( SMeshBuffer & o );

   static void
   computeNormals( SMeshBuffer & o );

   static void
   bumpVertices( SMeshBuffer & o, Image* bumpMap, float bumpScale = 1.0f );

   static void
   flipVertexPosYZ( SMeshBuffer & o );

   static void
   translateVertices( SMeshBuffer & o, glm::vec3 const & offset );

   static void
   translateVertices( SMeshBuffer & o, glm::vec3 const & offset, uint32_t vStart );

   static void
   translateVertices( SMeshBuffer & o, float x, float y, float z );

   static void
   transformVertices( SMeshBuffer & o, glm::dmat4 const & modelMat = glm::dmat4(1.0) );

   static void
   randomRadialDisplace( SMeshBuffer & o, float h );

   static void
   rotateVertices( SMeshBuffer & o, float a, float b, float c );

   static void
   scaleVertices( SMeshBuffer & o, float scale = 1.01f );

   static void
   scaleVertices( SMeshBuffer & o, float sx, float sy, float sz );

   static void
   translateNormals( SMeshBuffer & o, glm::vec3 const & offset );

   static void
   translateNormals( SMeshBuffer & o, float nx, float ny, float nz );

   static void
   setNormals( SMeshBuffer & o, glm::vec3 const & normal );

   static void
   setNormals( SMeshBuffer & o, float nx, float ny, float nz );

   static void
   setNormalZ( SMeshBuffer & o, float nz )
   {
      for ( auto & v : o.Vertices )
      {
         v.normal.z = nz;
      }
   }


   static void
   scaleTexture( SMeshBuffer & o, float u, float v );

   static void
   transformTexCoords( SMeshBuffer & o, Recti const & r_atlas, int atlas_w, int atlas_h );

   static void
   colorVertices( SMeshBuffer & o, uint32_t color );

   static void
   centerVertices( SMeshBuffer & o );

/*
   static SMeshBuffer
   createLines();

   static SMeshBuffer
   createTriangles();
*/
   static void
   addVertex( SMeshBuffer & o, S3DVertex const & vertex );

   static void
   addVertex( SMeshBuffer & o, glm::vec3 const & pos, glm::vec3 const & nrm, uint32_t color, glm::vec2 const & tex );

   static void
   addVertex( SMeshBuffer & o, glm::vec3 const & pos, uint32_t color );

   static void
   addLine( SMeshBuffer & o, S3DVertex const & a, S3DVertex const & b );

   static void
   addTriangle( SMeshBuffer & o, S3DVertex const & a, S3DVertex const & b, S3DVertex const & c );

   static void
   addLine( SMeshBuffer & o, glm::vec3 const & a, glm::vec3 const & b, uint32_t color = 0xFFFFFFFF );

   static void
   addLine( SMeshBuffer & o, glm::vec3 const & a, glm::vec3 const & b, uint32_t colorA, uint32_t colorB );

   static void
   addLine( SMeshBuffer & o, float x1, float y1, float x2, float y2, float z = 0.0f, uint32_t color = 0xFFFFFFFF );

   static void
   addLine( SMeshBuffer & o, float x1, float y1, float x2, float y2, float z, uint32_t colorA, uint32_t colorB );

   static void
   addLineTriangle( SMeshBuffer & o, glm::vec3 const & a, glm::vec3 const & b, glm::vec3 const & c, uint32_t color = 0xFFFFFFFF );

   static void
   addLineTriangle( SMeshBuffer & o, glm::vec3 const & a, glm::vec3 const & b, glm::vec3 const & c, uint32_t colorA, uint32_t colorB, uint32_t colorC );

   static void
   addLineQuad( SMeshBuffer & o, glm::vec3 const & a, glm::vec3 const & b, glm::vec3 const & c, glm::vec3 const & d, uint32_t color = 0xFFFFFFFF );

   static void
   addLineQuad( SMeshBuffer & o, glm::vec3 const & a, glm::vec3 const & b, glm::vec3 const & c, glm::vec3 const & d,
                uint32_t colorA, uint32_t colorB, uint32_t colorC = 0xFFFFFFFF, uint32_t colorD = 0xFFFFFFFF );

   static void
   addLineBox( SMeshBuffer & o, Box3f const & box, uint32_t color );

   // =======================================================================
   // DebugGeometry:
   // =======================================================================
   static void
   addLineNormals( SMeshBuffer & o, SMeshBuffer const & src, float n_len, uint32_t color );

   static void
   addWireframe( SMeshBuffer & o, SMeshBuffer const & src, float outScale, uint32_t color );

   static SMeshBuffer
   createWireframe( SMeshBuffer const & src, float scale = 1.0f, uint32_t color = 0 );

   static SMeshBuffer
   createScaled( SMeshBuffer const & src, float scale = 1.01f );

   static SMeshBuffer
   createTranslated( SMeshBuffer const & src, glm::vec3 pos );

   static SMeshBuffer
   createLineBox( Box3f const & box, uint32_t color );

   static SMeshBuffer
   createLineNormals( SMeshBuffer const & src, float normalLen, uint32_t color );

};


} // end namespace gpu.
} // end namespace de.

#endif

