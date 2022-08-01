#ifndef DE_GPU_SMESH_CONE_HPP
#define DE_GPU_SMESH_CONE_HPP

#include <de/gpu/smesh/SMesh.hpp>

namespace de {
namespace gpu {
namespace smesh {

// @brief Benni book example 2021
// =========================================================================
struct Generic3DCone
// =========================================================================
{
   // uint32_t getVertexCount();
   typedef std::function<uint32_t()> FN_GET_VERTEXCOUNT;
   // void addVertex( float x, float y, float z, float nx, float ny, float nz, uint32_t color, float u, float v) ;
   typedef std::function<void(float, float, float, float, float, float, uint32_t, float, float )> FN_ADD_VERTEX;
   // void addIndex( uint32_t index );
   typedef std::function<void(uint32_t)> FN_ADD_INDEX;

   static void
   addTrianglesXZ(
      float rx, float h, float rz,
      uint32_t innerColor,
      uint32_t outerColor,
      int32_t tessCircle,
      int32_t tessHeight,
      FN_GET_VERTEXCOUNT const & getVertexCount,
      FN_ADD_VERTEX const & addVertex,
      FN_ADD_INDEX const & addIndex
   )
   {
      std::vector< glm::vec3 > m_Pos;
      std::vector< glm::vec2 > m_Tex;
      m_Pos.reserve( tessCircle + 1 );
      m_Pos.emplace_back( 0.0f, h, 0.0f );
      m_Tex.reserve( tessCircle + 1 );
      m_Tex.emplace_back( 0.5f, 0.5f );

      float const angleStep = float( ( 2.0 * M_PI ) / double( tessCircle ) );
      for ( uint32_t i = 0; i < tessCircle; ++i )
      {
         float const phi = angleStep * float( i );
         float const c = ::cosf( phi );
         float const s = ::sinf( phi );
         m_Pos.emplace_back( rx*c, 0.0f, rz*s );
         m_Tex.emplace_back( 0.5f*c+0.5f, 0.5f*s+0.5f );
      }

      auto addTriangle = [&] ( uint32_t iA, uint32_t iB )
      {
         uint32_t const v = getVertexCount();
         glm::vec3 m = m_Pos[ 0 ];
         glm::vec3 a = m_Pos[ iA ];
         glm::vec3 b = m_Pos[ iB ];
         glm::vec3 n = Math::getNormal3D( m, a, b );
         addVertex( m.x, m.y, m.z, n.x,n.y,n.z, innerColor, 0.5f, 0.5f );
         addVertex( a.x, a.y, a.z, n.x,n.y,n.z, outerColor, m_Tex[iA].x, m_Tex[iA].y );
         addVertex( b.x, b.y, b.z, n.x,n.y,n.z, outerColor, m_Tex[iB].x, m_Tex[iB].y );
         addIndex( v + 0 );   // ABC (cw) = ACB (ccw)
         addIndex( v + 2 );
         addIndex( v + 1 );
      };

      for ( uint32_t i = 0; i < tessCircle; ++i )
      {
         if ( i == tessCircle - 1 )
         {
            uint32_t A = 1 + i;
            uint32_t B = 1;
            addTriangle( A, B );
         }
         else
         {
            uint32_t A = 1 + i;
            uint32_t B = 2 + i;
            addTriangle( A, B );
         }
      }

   }

};

// =========================================================================
struct Cone
// =========================================================================
{
   static void
   addXZ( SMeshBuffer & o, glm::vec3 const & siz,
         uint32_t iColor = 0xAFFFFFFF, uint32_t oColor = 0xFFFFFFFF,
         int32_t tessCircle = 12,
         int32_t tessHeight = 1 )
   {
      float rx = siz.x * 0.5f;
      float h = siz.y * 0.5f;
      float rz = siz.z * 0.5f;
      auto getVertexCount = [&] () { return o.getVertexCount(); };
      auto addIndex = [&] ( uint32_t index ) { o.addIndex( index ); };
      auto addVertex = [&] ( float px, float py, float pz,
                             float nx, float ny, float nz,
                             uint32_t color, float u, float v )
      {
         o.addVertex( S3DVertex( px,py,pz, nx,ny,nz, color, u,v ) );
      };
      Generic3DCone::addTrianglesXZ( rx,h,rz, iColor,oColor,
         tessCircle, tessHeight, getVertexCount, addVertex, addIndex );
   }
};

} // end namespace smesh.
} // end namespace gpu.
} // end namespace de.

#endif

