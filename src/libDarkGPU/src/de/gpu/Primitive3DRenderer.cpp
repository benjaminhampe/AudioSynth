#include <de/gpu/Primitive3DRenderer.hpp>
#include <de/gpu/IVideoDriver.hpp>

namespace de {
namespace gpu {

namespace
{
   constexpr float const CONST_Z_INIT = 0.90f; // At 1.0 it disappears, not inside frustum.
   constexpr float const CONST_Z_STEP = -0.00001f; // enough for 200.000 elements, TODO: test more.
}

Primitive3DRenderer::Primitive3DRenderer( IVideoDriver* driver )
   : m_Driver( driver )
   , m_CurrentBuffer( nullptr )
{

}

Primitive3DRenderer::~Primitive3DRenderer()
{

}

void
Primitive3DRenderer::clear()
{
   m_Mesh.clear();
   m_CurrentBuffer = nullptr;
}

void
Primitive3DRenderer::setDriver( IVideoDriver* driver )
{
   m_Driver = driver;
}

void
Primitive3DRenderer::begin( PrimitiveType::EType primType, TexRef const & ref )
{
   // The AI: 3 conditions before adding a new buffer
   if ( !m_CurrentBuffer // if no current work mesh
      || m_CurrentBuffer->PrimType != primType // or work mesh differs in primtype
      || m_CurrentBuffer->Material.getTexture(0).tex != ref.tex ) // or work mesh differs in Tex
   {
      // ... then add buffer to mesh, STL vector is public, all standards can be used by end-user.
      m_Mesh.Buffers.emplace_back();
      m_CurrentBuffer = &m_Mesh.Buffers.back();

      // Prepare buffer...
      m_CurrentBuffer->Name = "PrimitiveRenderBuf" + std::to_string( m_Mesh.Buffers.size() );
      m_CurrentBuffer->PrimType = primType;
      if ( ref.tex )
      {
         m_CurrentBuffer->Material.setTexture( 0, ref );
      }

      if ( primType >= PrimitiveType::LineLoop )
      {
         m_CurrentBuffer->Material.setLighting( 1 );
      }
      else
      {
         m_CurrentBuffer->Material.setLighting( 0 );
      }
   }
}

void
Primitive3DRenderer::end()
{
   if ( m_CurrentBuffer )
   {
      m_CurrentBuffer->upload();
   }
}

void
Primitive3DRenderer::add3DLine( glm::vec3 a, glm::vec3 b, uint32_t colorA, uint32_t colorB )
{
   begin( PrimitiveType::Lines );
   if ( !m_CurrentBuffer ) { DE_ERROR("No current buffer") return; }
   //add( *m_CurrentBuffer, pos, colorA, colorB, colorC, colorD, ref );
   m_CurrentBuffer->addVertex( S3DVertex( a.x, a.y, a.z, 0.f, 0.f, 0.f, colorA, 0,0 ) ); // A
   m_CurrentBuffer->addVertex( S3DVertex( b.x, b.y, b.z, 0.f, 0.f, 0.f, colorB, 0,0 ) ); // B
   m_CurrentBuffer->addIndexedLine();
   end();
}

void
Primitive3DRenderer::add3DTriangleOutline( glm::vec3 a, glm::vec3 b, glm::vec3 c, uint32_t colorA, uint32_t colorB, uint32_t colorC )
{
   begin( PrimitiveType::Lines );
   if ( !m_CurrentBuffer ) { DE_ERROR("No current buffer") return; }
   //add( *m_CurrentBuffer, pos, colorA, colorB, colorC, colorD, ref );
   S3DVertex A( a.x, a.y, a.z, 0.f, 0.f, 0.f, colorA, 0,0 );
   S3DVertex B( b.x, b.y, b.z, 0.f, 0.f, 0.f, colorB, 0,0 );
   S3DVertex C( c.x, c.y, c.z, 0.f, 0.f, 0.f, colorC, 0,0 );
   m_CurrentBuffer->addVertex( A );
   m_CurrentBuffer->addVertex( B );
   m_CurrentBuffer->addVertex( C );
   m_CurrentBuffer->addIndexedLineTriangle();
   end();
}

void
Primitive3DRenderer::add3DRectOutline( glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, uint32_t colorA, uint32_t colorB, uint32_t colorC, uint32_t colorD )
{
   begin( PrimitiveType::Lines );
   if ( !m_CurrentBuffer ) { DE_ERROR("No current buffer") return; }
   S3DVertex A( a.x, a.y, a.z, 0.f, 0.f, 0.f, colorA, 0,0 );
   S3DVertex B( b.x, b.y, b.z, 0.f, 0.f, 0.f, colorB, 0,0 );
   S3DVertex C( c.x, c.y, c.z, 0.f, 0.f, 0.f, colorC, 0,0 );
   S3DVertex D( d.x, d.y, d.z, 0.f, 0.f, 0.f, colorD, 0,0 );
   m_CurrentBuffer->addVertex( A );
   m_CurrentBuffer->addVertex( B );
   m_CurrentBuffer->addVertex( C );
   m_CurrentBuffer->addVertex( D );
   m_CurrentBuffer->addIndexedLineQuad();
   end();
}

void
Primitive3DRenderer::render()
{
   if (!m_Driver) { DE_ERROR("No driver") return; }

   //DE_DEBUG("m_Mesh.Buffers.size() = ", m_Mesh.Buffers.size())
   m_Driver->resetModelMatrix();
   m_Driver->draw3D( m_Mesh );
}


} // end namespace gpu.
} // end namespace de.
