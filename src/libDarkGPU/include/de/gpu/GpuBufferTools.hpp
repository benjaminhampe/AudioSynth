#ifndef DE_GPU_BUFFERTOOLS_HPP
#define DE_GPU_BUFFERTOOLS_HPP

#include <de/gpu/FVF.hpp>

namespace de {
namespace gpu {

#ifndef USE_VAO
#define USE_VAO
#endif

enum EGpuBufferUsage
{
   GBU_Static = 0,
   GBU_Dynamic
};

// Manages the GpuSide of a high level mesh.
// Internally creates a VAO with VBO and IBO if needed.
// VAO is supported since ES 3.0
// VBO and IBO supported since ES2.0
// Immediate drawing supported since ES 1.0 ( no gpu buffer, but can be fast for dynamic things )
// ===========================================================================
struct GpuBufferTools
// ===========================================================================
{
   DE_CREATE_LOGGER("de.gpu.GpuBufferTools")

   // Tools for all users
   static void
   destroyGpuBuffer(
      std::string const & caller,
      uint32_t & vbo, uint32_t & ibo, uint32_t & vao );

   static bool
   uploadGpuBuffer(
      std::string const & caller,
      uint32_t & vbo, uint32_t & ibo, uint32_t & vao,
      PrimitiveType::EType primType,
      void const* vertices,
      uint32_t vCount,
      FVF const & fvf,
      void const* indices,
      uint32_t iCount,
      IndexType::EType indexType );

   static void
   drawGpuBuffer(
      std::string const & caller,
      uint32_t & vbo, uint32_t & ibo, uint32_t & vao,
      PrimitiveType::EType primType,
      void const* vertices, uint32_t vCount, FVF const & fvf,
      void const* indices, uint32_t iCount, IndexType::EType indexType );

   static bool
   drawImmediate(
      std::string const & caller,
      PrimitiveType::EType primType,
      void const* vertices, uint32_t vCount, FVF const & fvf,
      void const* indices, uint32_t iCount, IndexType::EType indexType );

// ToolKit for more advanced users

   // ES 2.0
   static void
   destroyBuffer( uint32_t & vbo );

   // ES 2.0
   static bool
   createBuffer( uint32_t & vbo );

   // ES 3.0
   static bool
   createVAO( uint32_t & vao );

   // ES 3.0
   static void
   destroyVAO( uint32_t & vao );
};

} // end namespace gpu.
} // end namespace de.

#endif

