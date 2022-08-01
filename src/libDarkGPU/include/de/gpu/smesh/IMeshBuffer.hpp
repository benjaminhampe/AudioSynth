#ifndef DE_GPU_IMESHBUFFER_HPP
#define DE_GPU_IMESHBUFFER_HPP

#include <de/gpu/SMaterial.hpp>
#include <de/gpu/GpuBufferTools.hpp>
#include <functional>

namespace de {
namespace gpu {

// =======================================================================
struct IMeshBuffer
// =======================================================================
{
   virtual ~IMeshBuffer() = default;

   // VAO - VertexArrayObject ( could be not supported )
   virtual uint32_t &            getVAO() = 0;
   // VBO - VertexBufferObject ( could also even be not supported )
   virtual uint32_t &            getVBO() = 0;
   // IBO - IndexBufferObject ( could also even be not supported )
   virtual uint32_t &            getIBO() = 0;

   virtual bool upload( int mode = 0 )
   {
      if ( mode == 0 )
      {
         return true;
      }
      else
      {
         return GpuBufferTools::uploadGpuBuffer(
                  getName(),
                  getVBO(),
                  getIBO(),
                  getVAO(),
                  getPrimitiveType(),
                  getVertices(),
                  getVertexCount(),
                  getFVF(),
                  getIndices(),
                  getIndexCount(),
                  getIndexType() );
      }

   }

   virtual void destroy()
   {
      GpuBufferTools::destroyGpuBuffer(
         getName(),
         getVBO(),
         getIBO(),
         getVAO() );
   }

   virtual void draw( int mode = 0 )
   {
      if ( mode == 0 )
      {
         GpuBufferTools::drawImmediate(
            getName(),
            getPrimitiveType(),
            getVertices(),
            getVertexCount(),
            getFVF(),
            getIndices(),
            getIndexCount(),
            getIndexType() );
      }
      else
      {
         GpuBufferTools::drawGpuBuffer(
            getName(),
            getVBO(),
            getIBO(),
            getVAO(),
            getPrimitiveType(),
            getVertices(),
            getVertexCount(),
            getFVF(),
            getIndices(),
            getIndexCount(),
            getIndexType() );
      }
   }

   virtual bool                  isVisible() const = 0;
   virtual void                  setVisible( bool visible ) = 0;

   virtual std::string const &   getName() const = 0;
   virtual void                  setName( std::string const & name ) = 0;

   virtual SMaterial const &     getMaterial() const = 0;
   virtual SMaterial &           getMaterial() = 0;
   virtual void                  setMaterial( SMaterial const & material ) = 0;

   virtual PrimitiveType::EType  getPrimitiveType() const = 0;
   virtual void                  setPrimitiveType( PrimitiveType::EType type ) = 0;

   virtual Box3f const &         getBoundingBox() const = 0;
   virtual Box3f &               getBoundingBox() = 0;
   virtual void                  setBoundingBox( Box3f const & bbox ) = 0;

   virtual FVF const &           getFVF() const = 0;
   virtual FVF &                 getFVF() = 0;
   virtual void                  setFVF( FVF const & fvf ) = 0;

   virtual glm::dmat4 const &    getModelMatrix() const = 0;
   virtual void                  setModelMatrix( glm::dmat4 const & modelMat ) = 0;



   virtual size_t                getVertexCount() const = 0;
   virtual size_t                getVertexSize() const = 0;
   virtual size_t                getVerticesByteCount() const { return getVertexCount() * getVertexSize(); }
   virtual void const *          getVertices() const = 0;
   virtual void *                getVertices() = 0;
   virtual size_t                getVertexCapacity() const = 0;
   //virtual glm::vec3           getVertexPos( size_t i ) const = 0;
   virtual glm::vec3 const &     getPos( uint32_t i ) const = 0;
   virtual glm::vec3 &           getPos( uint32_t i ) = 0;
   virtual void                  setPos( uint32_t i, glm::vec3 const & v ) = 0;

   virtual uint32_t              getPrimitveCount() const { return PrimitiveType::getPrimitiveCount( getPrimitiveType(), getIndexCount() == 0 ? getVertexCount() : getIndexCount() ); }
   virtual uint32_t              getFaceCount() const { return getPrimitiveCount(); }
   virtual uint32_t              getEdgeCount() const { return 0; } // PrimitiveType::getEdgeCount( getPrimitiveType(), getIndexCount() == 0 ? getVertexCount() : getIndexCount() ); }

   virtual IndexType::EType      getIndexType() const = 0;
   virtual std::string           getIndexTypeStr() const { return IndexType::getString( getIndexType() ); }
   virtual uint32_t              getIndexSize() const = 0;
   virtual void const *          getIndices() const = 0;
   virtual void*                 getIndices() = 0;
   virtual uint32_t              getIndexCount() const = 0;
   virtual uint32_t              getIndexCapacity() const = 0;
   virtual uint32_t              getIndex( uint32_t i ) const = 0;
   virtual uint32_t &            getIndex( uint32_t i ) = 0;

   virtual size_t                getByteCount() const = 0;
   virtual void                  clear() = 0;
   // helper, combines 2 calls into one
   virtual void                  clear( PrimitiveType::EType primType ) = 0;

   virtual void                  clearIndices() = 0;
   virtual void                  clearVertices() = 0;
   virtual void                  moreIndices( size_t n_additional ) = 0;
   virtual void                  moreVertices( size_t n_additional ) = 0;
   virtual void                  addIndex( uint32_t index ) = 0;
// virtual void                  addIndexedLine( uint32_t a, uint32_t b ) = 0;
// virtual void                  addIndexedLineTriangle( uint32_t a, uint32_t b, uint32_t c ) = 0;
// virtual void                  addIndexedLineQuad( uint32_t a, uint32_t b, uint32_t c, uint32_t d ) = 0;
// virtual void                  addIndexedLineBox( uint32_t a, uint32_t b, uint32_t c, uint32_t d,
//                                                  uint32_t e, uint32_t f, uint32_t g, uint32_t h ) = 0;
// virtual void                  addIndexedTriangle( uint32_t a, uint32_t b, uint32_t c ) = 0;
// virtual void                  addIndexedQuad( uint32_t a, uint32_t b, uint32_t c, uint32_t d ) = 0;
// virtual void                  recalculateBoundingBox() = 0; // needs atleast 'a_pos' vertex attribute.
// virtual void                  translateVertices( glm::vec3 const & offsetPos ) = 0;
// virtual void                  normalizeNormals() = 0; // needs a_pos + a_normal
   virtual std::string           toString( bool withFVF = false ) const = 0;

   // inline stuff:
   virtual void setLighting( int illum ) { getMaterial().setLighting( illum ); }
   virtual void setFogEnable( bool enable ) { getMaterial().setFog( enable ); }
   virtual void setWireframe( bool enable ) { getMaterial().setWireframe( enable ); }
   virtual void setCulling( bool enable ) { getMaterial().setCulling( enable ); }
   virtual void setDepth( bool enable ) { getMaterial().setDepth( enable ); }
   virtual void setStencil( bool enable ) { getMaterial().setStencil( enable ); }
   virtual void setBlend( bool enable ) { getMaterial().setBlend( enable ); }

   virtual void setCulling( Culling cull ) { getMaterial().setCulling( cull ); }
   virtual void setDepth( Depth depth ) { getMaterial().setDepth( depth ); }
   virtual void setStencil( Stencil blend ) { getMaterial().setStencil( blend ); }
   virtual void setBlend( Blend blend ) { getMaterial().setBlend( blend ); }

   virtual void setPointSize( float ps ) { getMaterial().state.pointSize.m_Now = ps; }
   virtual void setLineWidth( float lw ) { getMaterial().state.lineWidth.m_Now = lw; }

   /// @brief Recalculates the bounding box. Should be called if the mesh changed.
   virtual void recalculateBoundingBox() = 0;

   virtual void rotate( glm::vec3 new_nrm, glm::vec3 old_nrm = glm::vec3(0.0f,0.0f,-1.0f) )
   {
      if ( old_nrm != new_nrm )
      {
         recalculateBoundingBox();
         Box3f bb = getBoundingBox();
         auto cc = bb.getCenter();
         Quatf qq = Quatf::from2Vectors( old_nrm, new_nrm );
         for ( size_t i = 0; i < getVertexCount(); ++i )
         {
            auto v1 = getPos( i ) - cc; // dir from center to pos
            auto l1 = glm::length( v1 ); // length
            auto v2 = qq.rotate( v1 ); // rotated dir with length one.
            auto p2 = cc + v2 * l1; // pos = center + rotated dir with length len1.
            setPos( i, p2 );
         }
      }
   }

//   virtual void setTexName( int stage, std::string const & name )
//   { getMaterial().setTexName( stage, name ); }

   virtual void setTexture( int stage, Tex* tex )
   { getMaterial().setTexture( stage, TexRef( tex ) ); }

   virtual void setTexture( int stage, TexRef const & ref )
   { getMaterial().setTexture( stage, ref ); }

   /// @brief PrimitiveType
   virtual uint32_t
   getPrimitiveCount() const { return PrimitiveType::getPrimitiveCount(
      getPrimitiveType(), getIndexCount() > 0 ? getIndexCount() : getVertexCount() ); }

   virtual std::string
   getPrimitiveTypeStr() const { return PrimitiveType::getString( getPrimitiveType() ); }

   virtual std::vector< Triangle3f >
   createCollisionTriangles() const
   {
      if ( getPrimitiveType() != PrimitiveType::Triangles ) return {};

      std::vector< Triangle3f > tris;

      if ( getIndexCount() > 0 )
      {
         tris.reserve( tris.size() + getIndexCount()/3 );
         for ( size_t i = 0; i < getIndexCount()/3; ++i )
         {
            tris.emplace_back( getPos( getIndex( 3*i   ) ),
                               getPos( getIndex( 3*i+1 ) ),
                               getPos( getIndex( 3*i+2 ) ));
         }
      }
      else
      {
         tris.reserve( tris.size() + getVertexCount()/3 );
         for ( size_t i = 0; i < getVertexCount()/3; ++i )
         {
            tris.emplace_back( getPos( 3*i ),
                               getPos( 3*i+1 ),
                               getPos( 3*i+2 ) );
         }
      }

      return tris;
   }

   virtual void
   forAllTriangles(
         std::function< void(uint32_t, Triangle3f const &)> const & func ) const
   {
      // [Triangles]
      if ( getPrimitiveType() == PrimitiveType::Triangles )
      {
         if ( getVertexCount() < 3 )
         {
            return; // no triangles
         }

         if ( getIndexCount() > 0 )
         {
            if ( getIndexCount() < 3 ) { return; }
            for ( size_t i = 0; i < getIndexCount()/3; i++ )
            {
               auto const & a = getPos( getIndex( 3*i ) );
               auto const & b = getPos( getIndex( 3*i+1 ) );
               auto const & c = getPos( getIndex( 3*i+2 ) );
               func( i, Triangle3f(a,b,c) );
            }
         }
         else
         {
            for ( size_t i = 0; i < getVertexCount()/3; i++ )
            {
               auto const & a = getPos( 3*i );
               auto const & b = getPos( 3*i+1 );
               auto const & c = getPos( 3*i+2 );
               func( i, Triangle3f(a,b,c) );
            }
         }
      }

      // [TriangleStrip]
      else if ( getPrimitiveType() == PrimitiveType::TriangleStrip )
      {
         if ( getVertexCount() < 3 )
         {
            return; // no triangle strip
         }

         if ( getIndexCount() > 0 )
         {
            if ( getVertexCount() < 3 ) { return; }
            if ( getIndexCount() < 3 ) { return; }

            for ( size_t i = 2; i < getIndexCount(); i++ )
            {
               auto const & a = getPos( getIndex( i-2 ) );
               auto const & b = getPos( getIndex( i-1 ) );
               auto const & c = getPos( getIndex( i ) );
               func( i-2, Triangle3f(a,b,c) );
            }
         }
         else
         {
            if ( getVertexCount() < 3 ) { return; }

            for ( size_t i = 2; i < getVertexCount(); i++ )
            {
               auto const & a = getPos( i-2 );
               auto const & b = getPos( i-1 );
               auto const & c = getPos( i );
               func( i-2, Triangle3f(a,b,c) );
            }
         }
      }

      // [BenniQuads]
      else if ( getPrimitiveType() == PrimitiveType::Quads )
      {
         if ( getVertexCount() < 4 )
         {
            return; // no quads
         }

         if ( getIndexCount() > 0 )
         {
            for ( size_t i = 0; i < getIndexCount()/4; i++ )
            {
               auto const & a = getPos( getIndex( 4*i ) );
               auto const & b = getPos( getIndex( 4*i+1 ) );
               auto const & c = getPos( getIndex( 4*i+2 ) );
               auto const & d = getPos( getIndex( 4*i+3 ) );
               // process quad, aka 2 triangles from 4 indexed vertices
               func( 2*i, Triangle3f(a,b,c) );
               func( 2*i+1, Triangle3f(a,c,d) );
            }
         }
         else
         {
            for ( size_t i = 0; i < getVertexCount()/4; i++ )
            {
               auto const & a = getPos( 4*i );
               auto const & b = getPos( 4*i+1 );
               auto const & c = getPos( 4*i+2 );
               auto const & d = getPos( 4*i+3 );
               // process quad, aka 2 triangles from 4 indexed vertices
               func( 2*i, Triangle3f(a,b,c) );
               func( 2*i+1, Triangle3f(a,c,d) );
            }
         }

      }
      else
      {
         DEM_ERROR("Unsupported PrimType = ", getPrimitiveTypeStr())
      }
   }

   template < typename T >
   bool
   intersectRay(
         Ray3< T > const & ray,
         glm::tvec3< T >* hitPosition = nullptr,
         uint32_t* triangleIndex = nullptr ) const
   {
      bool doesIntersect = false;
      uint32_t hitIndex = 0;
      auto const distMax = std::numeric_limits< T >::max();
      auto dist = distMax;
      glm::tvec3< T > hitPos;

      forAllTriangles(
         [&] ( uint32_t triIndex, Triangle3< T > const & triangle )
         {
            glm::tvec3< T > tmpPos;
            if ( triangle.intersectRay( ray.getPos(), ray.getDir(), tmpPos ) )
            {
               doesIntersect = true;
               auto d = glm::length( ray.getPos() - tmpPos );
               if ( dist == distMax || dist >= d )
               {
                  dist = d;
                  hitPos = tmpPos;
                  hitIndex = triIndex;
               }
            }
         }
      );

      if ( doesIntersect )
      {
         if ( hitPosition ) { *hitPosition = hitPos; }
         if ( triangleIndex ) { *triangleIndex = hitIndex; }
      }
      return doesIntersect;
   }

};

} // end namespace gpu.
} // end namespace de.

#endif

