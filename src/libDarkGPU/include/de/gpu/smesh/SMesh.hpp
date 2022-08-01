#ifndef DE_GPU_SMESH_SMESH_HPP
#define DE_GPU_SMESH_SMESH_HPP

#include <de/gpu/smesh/SMeshBuffer.hpp>
#include <map>

namespace de {
namespace gpu {

// ===========================================================================
struct IMesh
// ===========================================================================
{
   virtual ~IMesh() = default;

   virtual void
   clear() = 0;

   virtual void
   upload() = 0;

   virtual glm::dmat4 const &
   getModelMatrix() const = 0;

   virtual void
   setModelMatrix( glm::dmat4 const & modelMat ) = 0;

   virtual std::string
   getName() const = 0;

   virtual void
   setName( std::string const & name ) = 0;

   virtual bool
   isVisible() const = 0;

   virtual void
   setVisible( bool visible ) = 0;

   virtual Box3f const &
   getBoundingBox() const = 0;

   virtual void
   setBoundingBox( Box3f const & box ) = 0;

   virtual int
   getMaterialCount() const = 0;

   virtual SMaterial const &
   getMaterial( int i ) const = 0;

   virtual SMaterial &
   getMaterial( size_t i ) = 0;

   virtual std::string
   getPrimTypesStr() const = 0;

};

// ===========================================================================
struct SMesh : public IMesh
// ===========================================================================
{
   typedef std::shared_ptr< SMesh > SharedPtr;
   typedef std::unique_ptr< SMesh > UniquePtr;
   typedef std::weak_ptr< SMesh > WeakPtr;

   DE_CREATE_LOGGER("de.gpu.SMesh")
   glm::dmat4 ModelMat;
   Box3f BoundingBox;//! Bounding box of this meshbuffer.
   std::string Name;
   std::vector< SMeshBuffer > Buffers;   //! Geometry buffers for this mesh
   bool Visible;

public:
   static SharedPtr
   createShared( std::string const & name = "" )
   {
      return std::make_shared< SMesh >( name );
   }

   static SharedPtr
   createShared( SMeshBuffer const & buf )
   {
      return std::make_shared< SMesh >( buf );
   }

   SMesh( std::string const & name = "" );
   SMesh( SMeshBuffer const & buf );
   void upload() override;
   void clear() override;

   glm::dmat4 const &
   getModelMatrix() const override { return ModelMat; }
   void
   setModelMatrix( glm::dmat4 const & modelMat ) override { ModelMat = modelMat; }

   std::string
   getName() const override { return Name; }
   void
   setName( std::string const & name ) override { Name = name; }

   bool
   isVisible() const override { return Visible; }
   void
   setVisible( bool visible ) override { Visible = visible; }

   Box3f const &
   getBoundingBox() const override { return BoundingBox; }
   void
   setBoundingBox( Box3f const & bbox ) override { BoundingBox = bbox; }

   int getMaterialCount() const;
   SMaterial const & getMaterial( int i ) const;
   SMaterial & getMaterial( size_t i );

   void addEmptyBuffer();
   void addMeshBuffer( SMeshBuffer const & buf );
   void addMesh( SMesh const & other );

   std::string
   getPrimTypesStr() const override;

   int getMeshBufferCount() const;
   SMeshBuffer const & getMeshBuffer( int i ) const;
   SMeshBuffer & getMeshBuffer( int i );

   void removeMeshBuffer( int i );
   //void shiftMeshBuffer( int shifts );
   void recalculateNormals();
   void recalculateBoundingBox( bool fully = false );
   bool isIndex( int i ) const;

   std::string toString( bool printDetails = false ) const;
   size_t getVertexCount() const;
   size_t getIndexCount() const;
   size_t getByteCount() const;

   int getLighting() const;
   void setLighting( int illum );
   void setWireframe( bool enable );
   void setFogEnable( bool enable );
   void setCulling( Culling cull );
   void setCulling( bool enable );
   void setDepth( Depth depth );
   void setStencil( Stencil stencil );
   void setBlend( Blend blend );
   void setTexture( int stage, Tex* tex );
   void setTexture( int stage, TexRef ref );

   //std::vector< Triangle3f > createCollisionTriangles() const;
   void flipVertexPosYZ();
   void transformVertices( glm::dmat4 const & m );
   void translateVertices( glm::vec3 const & offset );
   void translateVertices( float x, float y, float z );
   void rotateVertices( float a, float b, float c );
   void colorVertices( uint32_t color );
   void centerVertices();
   void scaleTexture( float u, float v );
   void scaleVertices( float scale = 1.01f );
   void scaleVertices( float sx, float sy, float sz );

   template < typename T >
   bool intersectRay(
         Ray3< T > const & ray,
         glm::tvec3< T >* hitPosition = nullptr,
         uint32_t* bufferIndex = nullptr,
         uint32_t* triangleIndex = nullptr ) const
   {
      auto hitDist = std::numeric_limits< T >::max();
      auto hitPos = glm::tvec3< T >( T(0), T(0), T(0) );

      uint32_t bufIndex = 0;
      uint32_t triIndex = 0;
      bool doesIntersect = false;

      for ( size_t i = 0; i < getMeshBufferCount(); ++i )
      {
         auto const & p = getMeshBuffer( i );
         if ( !p.isVisible() ) continue; // nothing todo

         glm::tvec3< T > tmpPos;
         uint32_t tmpIndex = 0;
         if ( p.intersectRay( ray, &tmpPos, &tmpIndex ) )
         {
            doesIntersect = true;
            auto dist = glm::length( hitPos - ray.getPos() );
            if ( hitDist > dist )
            {
               hitDist = dist;
               hitPos = hitPos;
               bufIndex = uint32_t( i );
               triIndex = tmpIndex;
            }
         }
      }

      if ( doesIntersect )
      {
         if ( hitPosition ) { *hitPosition = hitPos; }
         if ( bufferIndex ) { *bufferIndex = bufIndex; }
         if ( triangleIndex ) { *triangleIndex = triIndex; }
      }

      return doesIntersect;
   }
};


// ===========================================================================
struct SMeshTool
// ===========================================================================
{
   DE_CREATE_LOGGER("de.gpu.SMeshTool")

   static void
   flipX( SMeshBuffer & o )
   {
      for ( auto & vertex : o.Vertices )
      {
         auto & pos = vertex.pos;
         pos.x = -pos.x;
      }
      o.recalculateBoundingBox();
   }
   static void
   flipY( SMeshBuffer & o )
   {
      for ( auto & vertex : o.Vertices )
      {
         auto & pos = vertex.pos;
         pos.y = -pos.y;
      }
      o.recalculateBoundingBox();
   }
   static void
   flipZ( SMeshBuffer & o )
   {
      for ( auto & vertex : o.Vertices )
      {
         auto & pos = vertex.pos;
         pos.z = -pos.z;
      }
      o.recalculateBoundingBox();
   }

   static void
   flipX( SMesh & o )
   {
      for ( SMeshBuffer & buf : o.Buffers )
      {
         flipX( buf );
      }
      o.recalculateBoundingBox();
   }
   static void
   flipY( SMesh & o )
   {
      for ( SMeshBuffer & buf : o.Buffers )
      {
         flipY( buf );
      }
      o.recalculateBoundingBox();
   }
   static void
   flipZ( SMesh & o )
   {
      for ( SMeshBuffer & buf : o.Buffers )
      {
         flipZ( buf );
      }
      o.recalculateBoundingBox();
   }


   static void
   flipXY( SMeshBuffer & o )
   {
      for ( auto & vertex : o.Vertices )
      {
         auto & pos = vertex.pos;
         std::swap( pos.x, pos.y );
      }
      o.recalculateBoundingBox();
   }
   static void
   flipYZ( SMeshBuffer & o )
   {
      for ( auto & vertex : o.Vertices )
      {
         auto & pos = vertex.pos;
         std::swap( pos.y, pos.z );
      }
      o.recalculateBoundingBox();
   }

   static void
   flipXZ( SMeshBuffer & o )
   {
      for ( auto & vertex : o.Vertices )
      {
         auto & pos = vertex.pos;
         std::swap( pos.x, pos.z );
      }
      o.recalculateBoundingBox();
   }

   static void
   flipXY( SMesh & o )
   {
      for ( SMeshBuffer & buf : o.Buffers )
      {
         flipXY( buf );
      }
      o.recalculateBoundingBox();
   }
   static void
   flipYZ( SMesh & o )
   {
      for ( SMeshBuffer & buf : o.Buffers )
      {
         flipYZ( buf );
      }
      o.recalculateBoundingBox();
   }
   static void
   flipXZ( SMesh & o )
   {
      for ( SMeshBuffer & buf : o.Buffers )
      {
         flipXZ( buf );
      }
      o.recalculateBoundingBox();
   }


   static void
   colorVertices( SMesh & o, uint32_t color )
   {
      for ( SMeshBuffer & buf : o.Buffers )
      {
         SMeshBufferTool::colorVertices( buf, color );
      }
   }

   static void
   translateVertices( SMesh & o, glm::vec3 const & offset )
   {
      for ( SMeshBuffer & buf : o.Buffers )
      {
         for ( size_t j = 0; j < buf.getVertexCount(); ++j )
         {
            buf.Vertices[ j ].pos += offset;
         }
         buf.recalculateBoundingBox();
      }

      o.recalculateBoundingBox();
   }

   static void
   translateVertices( SMesh & o, float x, float y, float z )
   {
      translateVertices( o, glm::vec3(x,y,z) );
   }

   static void
   rotateVertices( SMesh & o, float a, float b, float c )
   {
      for ( SMeshBuffer & buf : o.Buffers )
      {
         SMeshBufferTool::rotateVertices( buf, a,b,c );
      }

      o.recalculateBoundingBox();
   }

   static void
   rotateVertices( SMesh & o, glm::vec3 const & degrees )
   {
      rotateVertices( o, degrees.x, degrees.y, degrees.z );
   }

   static void
   scaleVertices( SMesh & o, glm::vec3 const & scale )
   {
      for ( SMeshBuffer & buf : o.Buffers )
      {
         for ( auto & vertex : buf.Vertices )
         {
            vertex.pos *= scale;
         }
         buf.recalculateBoundingBox();
      }

      o.recalculateBoundingBox();
   }

   static void
   scaleVertices( SMesh & o, float x, float y, float z )
   {
      scaleVertices( o, glm::vec3(x,y,z) );
   }

   static void
   centerVertices( SMesh & o )
   {
      o.recalculateBoundingBox();
      translateVertices( o, -o.getBoundingBox().getCenter() );
   }

   static void
   recalculateNormals( SMesh & o )
   {
      o.recalculateNormals();
   }

   static void
   fitVertices( SMesh & o, float maxSize = 1000.0f )
   {
      o.recalculateBoundingBox();
      float x = o.getBoundingBox().getCenter().x;
      float z = o.getBoundingBox().getCenter().z;
      float y = o.getBoundingBox().getMin().y;
      translateVertices( o, -glm::vec3(x,y,z) );

      glm::vec3 const s = o.getBoundingBox().getSize();
      float s_max = std::max( std::max( s.x, s.y ), s.z );
      float f = maxSize / s_max;
      scaleVertices( o, glm::vec3(f,f,f) );
   }

   static void flipNormals( SMeshBuffer & o )
   {
      for ( S3DVertex & v : o.Vertices )
      {
         v.normal = -v.normal;
      }
   }

   static void flipNormals( SMesh & o )
   {
      for ( SMeshBuffer & buf : o.Buffers )
      {
         flipNormals( buf );
      }
   }
};


} // end namespace gpu.
} // end namespace de.

#endif

