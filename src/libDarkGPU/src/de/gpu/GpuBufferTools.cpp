#include <de/gpu/GpuBufferTools.hpp>
#include <de/gpu/GLESv3.hpp>

namespace de {
namespace gpu {

bool
GpuBufferTools::drawImmediate(
   std::string const & caller,
   PrimitiveType::EType primType,
   void const* vertices,
   uint32_t vCount,
   FVF const & fvf,
   void const* indices,
   uint32_t iCount,
   IndexType::EType indexType )
{
   return GLES::drawPrimitiveList( caller, primType,
                                   vertices, vCount, fvf,
                                   indices, iCount, indexType );
}

void
GpuBufferTools::destroyBuffer( GLuint & vbo )
{
   if ( vbo < 1 ) { /*DE_WARN("Already destroyed")*/ return; }
   ::glDeleteBuffers( 1, &vbo ); GL_VALIDATE
   /*DE_DEBUG("Deleted buffer ",vbo)*/
   vbo = 0;
}

bool
GpuBufferTools::createBuffer( GLuint & vbo )
{
   if ( vbo > 0 ) { /*DE_WARN("Already created ",vbo)*/ return true; }
   ::glGenBuffers( 1, &vbo ); GL_VALIDATE
   if ( vbo == 0 ) { DE_ERROR("No buffer created") return false; }
   /*DE_DEBUG("Created buffer ",vbo)*/
   return true;
}

bool
GpuBufferTools::createVAO( GLuint & vao )
{
   if ( vao != 0 ) { /*DE_WARN("Already created ",vao)*/ return true; }
   ::glGenVertexArrays( 1, &vao ); GL_VALIDATE
   if ( vao == 0 ) { DE_ERROR("No vao created") return false; }
   /*DE_DEBUG("Created vao ",vao)*/
   return true;
}

void
GpuBufferTools::destroyVAO( GLuint & vao )
{
   if ( vao < 1 ) { /*DE_WARN("Already destroyed")*/ return; }
   ::glDeleteVertexArrays( 1, &vao ); GL_VALIDATE
   /*DE_DEBUG("Deleted vao ",vao)*/
   vao = 0;
}

void
GpuBufferTools::destroyGpuBuffer(
   std::string const & caller, uint32_t & vbo, uint32_t & ibo, uint32_t & vao )
{
   // Nothing todo, since no gpu buffer was used.
   // Immediate mode uploads CPU side right before rendering
   // and then nothing needs to be cleaned up on GPU side, CPU mesh stays constant.
   // No gpu buffer ids have to managed or carried around.
   destroyBuffer( vbo );
   destroyBuffer( ibo );
   destroyVAO( vao );
}

bool
GpuBufferTools::uploadGpuBuffer(
   std::string const & caller,
   uint32_t & vbo, uint32_t & ibo, uint32_t & vao,
   PrimitiveType::EType primType,
   void const* vertices,
   uint32_t vCount,
   FVF const & fvf,
   void const* indices,
   uint32_t iCount,
   IndexType::EType indexType )
{
   if ( !vertices ) { DE_WARN("name(",caller,"), es(3) No vertices") return false; }
   if ( !vCount ) { DE_WARN("name(",caller,"), es(3) No vertex count") return false; }
   // CheckPoint
   bool useIndices = indices && iCount > 0;
//   if ( esVersion > 1 )
//   {
//      if ( !vbo ) { DE_ERROR( "name(",caller,"), es(",esVersion,") no vbo given." ) return; }
//      if ( !ibo && useIndices ) { DE_ERROR( "name(",caller,"), es(",esVersion,") no ibo given." ) return; }
//   }
//   if ( esVersion > 2 )
//   {
//      if ( !vao ) { DE_ERROR( "name(",caller,"), es(",esVersion,") no vao given." ) return; }
//   }
   // RestorePoint
   GLint old_vao = 0;
   GLint old_vbo = 0;
   GLint old_ibo = 0;
   ::glGetIntegerv( GL_VERTEX_ARRAY_BINDING, &old_vao ); GL_VALIDATE;
   ::glGetIntegerv( GL_ARRAY_BUFFER_BINDING, &old_vbo ); GL_VALIDATE;
   ::glGetIntegerv( GL_ELEMENT_ARRAY_BUFFER_BINDING, &old_ibo ); GL_VALIDATE;

   //==============================
   // VBO - Upload Vertices ( atleast three vertices must exist to form a triangle, min is one for dot)
   // Possible since OpenGL ES 2.0 ( we love it, it means we store a mesh on graphics card memory GDDR5 )
   //==============================
   if ( !vbo ) { ::glGenBuffers( 1, &vbo ); GL_VALIDATE }
   if ( !vbo ) { DE_ERROR("name(",caller,"), es(3) No vbo") return false; }
   ::glBindBuffer( GL_ARRAY_BUFFER, vbo); GL_VALIDATE
   size_t byteCount = size_t(vCount) * fvf.getByteSize();
   ::glBufferData( GL_ARRAY_BUFFER, byteCount, vertices, GL_STATIC_DRAW ); GL_VALIDATE;

   //==============================
   // IBO - Upload Indices ( optional, can reduce memory of mesh by reusing vertices using indices )
   // Possible since OpenGL ES 2.0 ( we love it, it means we store a mesh on graphics card memory GDDR5 )
   // A 4byte index is still smaller than a 36byte S3DVertex.
   // So i could address 9 vertices instead of storing only one new vertex.
   // Indices can be 8-bit,16-bit,32-bit unsigned integer.
   // Default for me is 4 byte to load 150000000 triangles without problems.
   //==============================
   if ( useIndices )
   {
      if ( !ibo ) { ::glGenBuffers( 1, &ibo ); GL_VALIDATE }
      if ( !ibo ) { DE_ERROR("name(",caller,"), es(3) No ibo") return false; }
      ::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo); GL_VALIDATE
      size_t byteCount = size_t(iCount) * IndexType::getIndexSize(indexType);
      ::glBufferData( GL_ELEMENT_ARRAY_BUFFER, byteCount, indices, GL_STATIC_DRAW ); GL_VALIDATE;
   }
   //==============================
   // [VAO] for reducing calls to Attribute changes, possible since OpenGL ES 3.0 ( we like it )

   //==============================
#ifdef USE_VAO
   if ( !vao )
   {
      ::glGenVertexArrays( 1, &vao ); GL_VALIDATE
   }
   if ( !vao ) { DE_ERROR("name(",caller,"), es(3) No vao") return false; }

   ::glBindVertexArray( vao ); GL_VALIDATE
   ::glBindBuffer( GL_ARRAY_BUFFER, vbo); GL_VALIDATE
   if ( useIndices )
   {
      ::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo); GL_VALIDATE
   }

   GLuint k = 0;
   size_t offset = 0;
   for ( size_t i = 0 ; i < fvf.m_Data.size(); ++i )
   {
      VertexAttrib const & attr = fvf.m_Data[ i ];
      if ( !attr.m_Enabled ) continue;
      GLint const aCount = GLint( attr.m_Count );
      GLenum const aType = GLES::fromVertexAttribType( attr.m_Type );
      GLsizei const aStride = GLsizei( fvf.m_Stride );
      GLboolean const shouldNormalize = attr.m_Normalize ? GL_TRUE : GL_FALSE;
      ::glEnableVertexAttribArray( k ); GL_VALIDATE
      ::glVertexAttribPointer( k, aCount, aType, shouldNormalize, aStride, ( const void * )offset ); GL_VALIDATE
      offset += attr.m_ByteSize; // Advance ptr to next item (attribute).
      k++;
   }

   // Restore RestorePoint
   ::glBindVertexArray ( old_vao );
   ::glBindBuffer( GL_ARRAY_BUFFER, old_vbo );
   ::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, old_ibo );
   //::glBindVertexArray ( 0 );
//   k = 0;
//   for ( size_t i = 0 ; i < fvf.m_Data.size(); ++i )
//   {
//      VertexAttrib const & attr = fvf.m_Data[ i ];
//      if ( !attr.m_Enabled ) continue;
//      ::glDisableVertexAttribArray( GLuint( k ) ); GL_VALIDATE
//      k++;
//   }

#endif

   return true;
}

void
GpuBufferTools::drawGpuBuffer(
   std::string const & caller,
   uint32_t & vbo, uint32_t & ibo, uint32_t & vao,
   PrimitiveType::EType primType,
   void const* vertices, uint32_t vCount, FVF const & fvf,
   void const* indices, uint32_t iCount, IndexType::EType indexType )
{
   if ( !vertices || vCount < 1 ) { return; } //
   if ( !indices && iCount > 0 ) { DE_ERROR( "name(",caller,"), es(3) gave malformed indices" ) return; }
   bool useIndices = indices && iCount > 0;

//   if ( esVersion > 1 )
//   {
//      if ( !vbo ) { DE_ERROR( "name(",caller,"), es(",esVersion,") no vbo given." ) return; }
//      if ( !ibo && useIndices ) { DE_ERROR( "name(",caller,"), es(",esVersion,") no ibo given." ) return; }
//   }
//   if ( esVersion > 2 )
//   {
//      if ( !vao ) { DE_ERROR( "name(",caller,"), es(",esVersion,") no vao given." ) return; }
//   }

#ifdef USE_VAO_DEBUG
   PerformanceTimer timer;
   timer.start();
#endif

#ifdef USE_VAO
   if ( !vao )
   {
      uploadGpuBuffer( caller, vbo, ibo, vao,
         primType, vertices, vCount, fvf, indices, iCount, indexType );
    }

   if ( !vao )
   {
      DE_ERROR( "name(",caller,"), es(3) No vao" ) return;
   }

   // RestorePoint
//   GLint old_vao = 0;
//   GLint old_vbo = 0;
//   GLint old_ibo = 0;
//   ::glGetIntegerv( GL_VERTEX_ARRAY_BINDING, &old_vao ); GL_VALIDATE;
//   ::glGetIntegerv( GL_ARRAY_BUFFER_BINDING, &old_vbo ); GL_VALIDATE;
//   ::glGetIntegerv( GL_ELEMENT_ARRAY_BUFFER_BINDING, &old_ibo ); GL_VALIDATE;

   ::glBindVertexArray( vao ); GL_VALIDATE

   if ( useIndices )
   {
      ::glDrawElements( GLenum( GLES::fromPrimitiveType( primType ) ), GLsizei( iCount ),
            GLenum( GLES::fromIndexType( indexType ) ), (const void*)0 );
      bool ok = GL_VALIDATE;
      if ( !ok )
      {
         DE_ERROR( "name(",caller,"), es(3)" )
         DE_ERROR( "vao(", vao,"), vbo(", vbo,"), ibo(", ibo,")")
         DE_ERROR( "DrawElements(", vCount,")")
         DE_ERROR( "PrimType(", PrimitiveType::getString( primType ),")")
         DE_ERROR( "iCount(", iCount,")")
         DE_ERROR( "iType(", IndexType::getString( indexType ),")")
         DE_ERROR( "vertices(", vertices,")")
         DE_ERROR( "indices(", indices,")")
      }
   }
   else
   {
      ::glDrawArrays( GLenum( GLES::fromPrimitiveType( primType ) ),
                      0, GLsizei( vCount ) );
      bool ok = GL_VALIDATE;
      if ( !ok )
      {
         DE_ERROR( "name(",caller,"), es(3)" )
         DE_ERROR( "vao(", vao,"), vbo(", vbo,"), ibo(", ibo,")")
         DE_ERROR( "DrawArrays(", vCount,")")
         DE_ERROR( "PrimType(", PrimitiveType::getString( primType ),")")
         DE_ERROR( "iCount(", iCount,")")
         DE_ERROR( "iType(", IndexType::getString( indexType ),")")
         DE_ERROR( "vertices(", vertices,")")
         DE_ERROR( "indices(", indices,")")
      }
   }

   // Restore RestorePoint
//   ::glBindVertexArray ( old_vao );
//   ::glBindBuffer( GL_ARRAY_BUFFER, old_vbo );
//   ::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, old_ibo );
   ::glBindVertexArray ( 0 );

#ifdef USE_VAO_DEBUG
   timer.stop();
   DE_DEBUG( "RenderTime = ", timer.us(), " us." )
   DE_DEBUG( "New vao = ", vao )
   //DE_DEBUG( "Old vao = ", old_vao )
#endif

#else
   // VBO only not implemented
#endif
}

// unsigned int cubeVAO, cubeVBO, cubeIBO;
// glGenBuffers(1, &cubeVBO);
// glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
// glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);

// glGenBuffers(1, &cubeIBO);
// glBindBuffer(GL_ARRAY_BUFFER, cubeIBO);
// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), &cubeIndices, GL_STATIC_DRAW);

// glGenVertexArrays(1, &cubeVAO);
// glBindVertexArray(cubeVAO);
// glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
// glEnableVertexAttribArray(0);
// glEnableVertexAttribArray(1);
// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
// glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

//void
//GpuBufferTools::drawGpuBuffer(
//   std::string const & caller,
//   int esVersion,
//   uint32_t & vbo, uint32_t & ibo, uint32_t & vao,
//   PrimitiveType::EType primType,
//   void const* vertices, uint32_t vCount, FVF const & fvf,
//   void const* indices, uint32_t iCount, IndexType::EType indexType )
//{
//   drawGpuBufferES3( caller, vbo, ibo, vao,
//      primType, vertices, vCount, fvf, indices, iCount, indexType );
//}


} // end namespace gpu.
} // end namespace de.

