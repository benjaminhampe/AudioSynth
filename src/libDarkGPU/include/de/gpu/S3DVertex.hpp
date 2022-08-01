#ifndef DE_GPU_S3DVERTEX_HPP
#define DE_GPU_S3DVERTEX_HPP

#include <de/gpu/FVF.hpp>

namespace de {
namespace gpu {

#pragma pack( push )
#pragma pack( 1 )

// ===========================================================================
struct S3DVertex
// ===========================================================================
{
   glm::vec3 pos;
   glm::vec3 normal;
   uint32_t color;
   glm::vec2 tex;    // 12 + 12 + 4 + 8 = 36 Bytes if packed

public:
   S3DVertex()
      : pos(), normal( 0, 0, 1 ), color( 0xFFFF00FF ), tex()
   {}

   S3DVertex( float x, float y, float z, float nx, float ny, float nz, uint32_t crgba, float u, float v )
      : pos( x,y,z ), normal( nx,ny,nz ), color( crgba ), tex( u,v )
   {}

   S3DVertex( glm::vec3 const & p, glm::vec3 const & nrm, uint32_t crgba, glm::vec2 const & tex0 )
      : pos( p ), normal( nrm ), color( crgba ), tex( tex0 )
   {}

   void
   flipV()
   {
      tex.y = glm::clamp( 1.0f - tex.y, 0.f, 1.0f ); // Invert v for OpenGL
   }

   // STANDARD_FVF = POS_XYZ | NORMAL_XYZ | COLOR_RGBA | TEX0_UV = 36 Bytes.
   static FVF const &
   getFVF()
   {
      static FVF const fvf( sizeof( S3DVertex ),
         {  VertexAttrib( VertexAttribType::F32, 3, VertexAttribSemantic::Pos ),
            VertexAttrib( VertexAttribType::F32, 3, VertexAttribSemantic::Normal ),
            VertexAttrib( VertexAttribType::U8,  4, VertexAttribSemantic::Color ),
            VertexAttrib( VertexAttribType::F32, 2, VertexAttribSemantic::Tex )
         }
      );
      return fvf;
   }

   glm::vec3 const & getPos() const { return pos; }
   glm::vec3 & getPos() { return pos; }
   void setPos( glm::vec3 const & position ) { pos = position; }

   glm::vec3 const & getNormal() const { return normal; }
   uint32_t const & getColor() const { return color; }
   glm::vec2 const & getTexCoord( int stage = 0 ) const { return tex; }
   glm::vec3 getTangent( glm::vec3 const & ) { return {}; }
   glm::vec3 getBiTangent( glm::vec3 const & ) { return {}; }


   void setNormal( glm::vec3 const & nrm ) { this->normal = nrm; }
   void setTangent( glm::vec3 const & ) {}
   void setBiTangent( glm::vec3 const & ) {}
   void setColor( uint32_t const & colorRGBA ) { this->color = colorRGBA; }
   void setTexCoord( glm::vec2 const & texCoord, int stage = 0 ) { this->tex = texCoord; }

//   bool
//   load( std::string uri )
//   {
//      return false;
//   }

//   bool
//   save( std::string uri ) const
//   {
//      std::stringstream s;
//      s << "Saved Benni Mesh\n";
//      s << "fileName: " << m_Name << "\n";
//      s << "bufferCount: " << m_MeshBuffers << "\n";

//      for ( size_t i = 0; i < m_MeshBuffers.size(); ++i )
//      {
//         SMeshBuffer const * const p = m_MeshBuffers[ i ];
//         if ( !p ) continue;
//         s << "buffer[" << i << "] " << p->getName() << "\n";

//         for ( size_t v = 0; v < m_MeshBuffers.Vertsize(); ++i )
//         {
//            SMeshBuffer const * const p = m_MeshBuffers[ i ];
//            if ( !p ) continue;
//            s << "buffer[" << i << "] " << p->getName() << "\n";

//            p->Material.wireframe = enable;
//         }

//         p->Material.wireframe = enable;
//      }

//      s << "[material-count] " << m_MeshBuffers << "\n";
//      s << " )";
//      return false;
//   }

};
#pragma pack( pop )


} // end namespace gpu.
} // end namespace de.

#endif

