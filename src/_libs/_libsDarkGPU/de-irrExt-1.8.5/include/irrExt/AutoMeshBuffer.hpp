#ifndef DE_ALPHASONIC_AUTO_MESH_BUFFER_HPP
#define DE_ALPHASONIC_AUTO_MESH_BUFFER_HPP

#include <irrExt/_common.hpp>

void SceneNode_setPosition( irr::scene::ISceneNode* node, glm::vec3 const & pos );
void SceneNode_setRotation( irr::scene::ISceneNode* node, glm::vec3 const & degrees );

void
enumerateMeshBuffer( irr::scene::SMeshBuffer & p );

//=======================================================================================
//
// CLASS: AutoMeshBuffer
//
//=======================================================================================

struct AutoMeshBuffer : public irr::IReferenceCounted
{
   irr::scene::E_PRIMITIVE_TYPE PrimitiveType;	// We need this to enable auto rendering
   irr::video::E_VERTEX_TYPE VertexType;		// We need this to enable auto rendering
   irr::video::E_INDEX_TYPE IndexType;			// We need this to enable auto rendering
   irr::scene::SMeshBuffer MeshBuffer;			// The traditional irrlicht meshbuffer ( aka non-auto, aka broken )

   static uint32_t
   getPrimitiveCount( irr::scene::E_PRIMITIVE_TYPE primitiveType, uint32_t indexCount );

   uint32_t getPrimitiveCount() const;

   AutoMeshBuffer();
   AutoMeshBuffer( irr::scene::E_PRIMITIVE_TYPE primType,
                    irr::video::E_VERTEX_TYPE vertexType = irr::video::EVT_STANDARD,
                    irr::video::E_INDEX_TYPE indexType = irr::video::EIT_16BIT );

   virtual ~AutoMeshBuffer();

   void render( irr::video::IVideoDriver* driver );

   /// interface: IMeshBuffer

   uint32_t getVertexCount() const { return MeshBuffer.Vertices.size(); }
   irr::core::array< irr::video::S3DVertex > const & getVertices() const { return MeshBuffer.Vertices; }
   irr::core::array< irr::video::S3DVertex > & getVertices() { return MeshBuffer.Vertices; }

   uint32_t getIndexCount() const { return MeshBuffer.Indices.size(); }
   irr::core::array< irr::u16 > const & getIndices() const { return MeshBuffer.Indices; }
   irr::core::array< irr::u16 > & getIndices() { return MeshBuffer.Indices; }

   irr::core::aabbox3d< irr::f32 > const & getBoundingBox() const { return MeshBuffer.BoundingBox; }
   irr::core::aabbox3d< irr::f32 > & getBoundingBox() { return MeshBuffer.BoundingBox; }
   void setBoundingBox( irr::core::aabbox3df const & box ){ MeshBuffer.BoundingBox = box; }

   irr::video::SMaterial const & getMaterial() const { return MeshBuffer.Material; }
   irr::video::SMaterial & getMaterial() { return MeshBuffer.Material; }
   void setTexture( uint32_t stage, irr::video::ITexture* tex ){ MeshBuffer.Material.setTexture( stage, tex ); }
   void setWireframe( bool enable ) { MeshBuffer.Material.Wireframe = enable; }
   bool hasTransparentVertexColor() const;
   void setDefaultMaterialType();
   void setDefaultMaterialType( irr::video::SColor const & color );

   /// @brief Add collision detection
   bool getIntersectionWithLine( irr::core::line3df const & ray, irr::core::vector3df & hitPosition ) const;

};

//=======================================================================================
//
// CLASS: AutoMesh
//
//=======================================================================================

class AutoMesh : public irr::scene::IMesh
{
public:
    //! constructor
    AutoMesh();

    //! destructor
    ~AutoMesh() override;

    //! clean mesh
    void clear();

    virtual irr::u32 getMaterialCount() const;

    virtual irr::video::SMaterial & getMaterial( irr::u32 i );

    //! returns amount of mesh buffers.
    virtual irr::u32 getMeshBufferCount() const override;

    //! returns pointer to a mesh buffer
    virtual irr::scene::IMeshBuffer* getMeshBuffer( irr::u32 i ) const override;

    //! returns a meshbuffer which fits a material
    /** reverse search */
    virtual irr::scene::IMeshBuffer* getMeshBuffer( irr::video::SMaterial const & material ) const override;

    //! returns an axis aligned bounding box
    virtual irr::core::aabbox3d<irr::f32> const & getBoundingBox() const override;

    //! set user axis aligned bounding box
    virtual void setBoundingBox( irr::core::aabbox3df const & box ) override;

    //! recalculates the bounding box
    virtual void recalculateBoundingBox(); // override

    //! adds a MeshBuffer
    /** The bounding box is not updated automatically. */
    virtual void addMeshBuffer( irr::scene::IMeshBuffer* buf ); // override

    //! adds a MeshBuffer
    /** The bounding box is not updated automatically. */
    virtual void addAutoMeshBuffer( AutoMeshBuffer* buf, bool dropAfter = false ); // override

    //! sets a flag of all contained materials to a new value
    virtual void setMaterialFlag( irr::video::E_MATERIAL_FLAG flag, bool newvalue) override;

    //! set the hardware mapping hint, for driver
    virtual void setHardwareMappingHint( irr::scene::E_HARDWARE_MAPPING newMappingHint, irr::scene::E_BUFFER_TYPE buffer = irr::scene::EBT_VERTEX_AND_INDEX ) override;

    //! flags the meshbuffer as changed, reloads hardware buffers
    virtual void setDirty( irr::scene::E_BUFFER_TYPE buffer = irr::scene::EBT_VERTEX_AND_INDEX ) override;

    //! The meshbuffers of this mesh
    std::vector< AutoMeshBuffer* > MeshBuffers;

    //! The bounding box of this mesh
    irr::core::aabbox3df BoundingBox;

    //! What we return on invalid index
    irr::video::SMaterial DefaultMaterial;

    /// @brief Add collision detection
    bool getIntersectionWithLine( irr::core::line3df const & ray, irr::core::vector3df & hitPosition ) const;
};

//=======================================================================================
//
// CLASS: AutoSceneNode
//
//=======================================================================================

class AutoSceneNode : public irr::scene::ISceneNode
{
public:
    AutoSceneNode( irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent, int id = -1 );

    virtual ~AutoSceneNode();

    void OnRegisterSceneNode() override;

    void render() override;

    irr::u32 getMaterialCount() const override;

    irr::video::SMaterial& getMaterial( irr::u32 i ) override;

    irr::core::aabbox3d<irr::f32> const & getBoundingBox() const override;

    void clear();

    void addAutoMeshBuffer( AutoMeshBuffer * mb, bool dropAfterAdd = false );

//    void addMeshBuffer(
//            irr::scene::SMeshBuffer && mb,
//            irr::scene::E_PRIMITIVE_TYPE primType = irr::scene::EPT_TRIANGLES,
//            irr::video::E_VERTEX_TYPE vertexType = irr::video::EVT_STANDARD,
//            irr::video::E_INDEX_TYPE indexType = irr::video::EIT_16BIT );

    irr::u32 getMeshBufferCount() const;

    AutoMeshBuffer* getAutoMeshBuffer( irr::u32 i );

    irr::scene::SMeshBuffer* getMeshBuffer( irr::u32 i );

    /// @brief Add collision detection
    bool getIntersectionWithLine( irr::core::line3df const & ray, irr::core::vector3df & hitPosition ) const;

    //void setPosition( float32_t x, float32_t y, float32_t z );

    AutoMesh * getMesh() { return &m_Mesh; }

protected:
    irr::core::aabbox3d<irr::f32> m_BoundingBox;

    AutoMesh m_Mesh;

    // std::vector< AutoMeshBuffer* > m_MeshBuffer;
};


//=======================================================================================
//
// CLASS: AutoTriangleSelector
//
//=======================================================================================

#if 0
//! Interface to return triangles with specific properties.
class AutoTriangleSelector : public virtual irr::scene::ITriangleSelector
{
    irr::scene::ISceneNode* m_SceneNode;
    std::vector< irr::core::triangle3df > m_Triangles;
    irr::core::aabbox3df m_BoundingBox;
public:
    void clear() { m_Triangles.clear(); m_SceneNode = nullptr; }

    //! Get amount of all available triangles in this selector
    void setSceneNode( irr::scene::ISceneNode* node ) { m_SceneNode = node; }

    void addTriangle( irr::core::triangle3df const & triangle ) { m_Triangles.emplace_back( std::move( triangle ) ); }

    void addMeshBuffer( irr::scene::IMeshBuffer* p, irr::scene::E_PRIMITIVE_TYPE primType = irr::scene::EPT_TRIANGLES )
    {
        if ( primType != irr::scene::EPT_TRIANGLES )
        {
            std::cout << "[Warn] " << __FUNCTION__ << " :: Adding incompatible MeshBuffer of primType(" << primType << ")\n";
        }

        uint32_t triCount = p->getIndexCount() / 3;
        for ( uint32_t i = 0; i < triCount; ++i )
        {
            irr::video::S3DVertex* vertices = (irr::video::S3DVertex*)p->getVertices();
            irr::core::vector3df const & A = vertices[ 3*i + 0 ].Pos;
            irr::core::vector3df const & B = vertices[ 3*i + 1 ].Pos;
            irr::core::vector3df const & C = vertices[ 3*i + 2 ].Pos;
            m_Triangles.emplace_back( irr::core::triangle3df( A, B, C ) );
        }
    }
    //! Get amount of all available triangles in this selector
    virtual irr::s32 getTriangleCount() const { return m_Triangles.size(); }

    //! Get scene node associated with a given triangle.
    virtual irr::scene::ISceneNode* getSceneNodeForTriangle( irr::u32 /*triangleIndex*/ ) const { return m_SceneNode; }

    //! Get number of TriangleSelectors that are part of this one
    virtual irr::u32 getSelectorCount() const { return 1; }

    //! Get TriangleSelector based on index based on getSelectorCount
    virtual ITriangleSelector* getSelector( irr::u32 index ) { return nullptr; }

    //! Get TriangleSelector based on index based on getSelectorCount
    virtual const ITriangleSelector* getSelector( irr::u32 index ) const { return nullptr; }

    //! Gets the triangles for one associated node.
    virtual void getTriangles( irr::core::triangle3df* triangles, irr::s32 arraySize,
        irr::s32& outTriangleCount, const irr::core::matrix4* transform=0 ) const override
    {
        irr::u32 cnt = m_Triangles.size();
        if (cnt > (irr::u32)arraySize)
            cnt = (irr::u32)arraySize;

        irr::core::matrix4  mat;
        if (transform)      mat = *transform;
        if (m_SceneNode)    mat *= m_SceneNode->getAbsoluteTransformation();

        for ( irr::u32 i = 0; i < cnt; ++i )
        {
            mat.transformVect( triangles[i].pointA, m_Triangles[i].pointA );
            mat.transformVect( triangles[i].pointB, m_Triangles[i].pointB );
            mat.transformVect( triangles[i].pointC, m_Triangles[i].pointC );
        }

        outTriangleCount = cnt;
    }


//! Gets all triangles which lie within a specific bounding box.
    virtual void getTriangles( irr::core::triangle3df* triangles,
                    irr::s32 arraySize, irr::s32 & outTriangleCount,
                    irr::core::aabbox3d<irr::f32> const & box,
                    irr::core::matrix4 const * transform) const override
    {
        irr::core::matrix4 mat( irr::core::matrix4::EM4CONST_NOTHING );
        irr::core::aabbox3df tBox(box);

        if (m_SceneNode)
        {
            m_SceneNode->getAbsoluteTransformation().getInverse(mat);
            mat.transformBoxEx( tBox );
        }
        if ( transform )    mat = *transform;
        else                mat.makeIdentity();
        if (m_SceneNode)    mat *= m_SceneNode->getAbsoluteTransformation();

        outTriangleCount = 0;

        if ( !tBox.intersectsWithBox( m_BoundingBox ) ) return;

        irr::s32 triangleCount = 0;
        irr::u32 const cnt = m_Triangles.size();
        for ( irr::u32 i = 0; i < cnt; ++i )
        {
            // This isn't an accurate test, but it's fast, and the
            // API contract doesn't guarantee complete accuracy.
            if ( m_Triangles[i].isTotalOutsideBox(tBox) )
               continue;

            triangles[ triangleCount ] = m_Triangles[ i ];
            mat.transformVect( triangles[ triangleCount ].pointA );
            mat.transformVect( triangles[ triangleCount ].pointB );
            mat.transformVect( triangles[ triangleCount ].pointC );

            ++triangleCount;

            if (triangleCount == arraySize)
                break;
        }

        outTriangleCount = triangleCount;
    }


    //! Gets all triangles which have or may have contact with a 3d line.
    virtual void getTriangles( irr::core::triangle3df* triangles,
                    irr::s32 arraySize,
                    irr::s32 & outTriangleCount,
                    irr::core::line3d<irr::f32> const & line,
                    irr::core::matrix4 const * transform) const override
    {
        irr::core::aabbox3d<irr::f32> box( line.start );
        box.addInternalPoint( line.end );

        // TODO: Could be optimized for line a little bit more.
        getTriangles( triangles, arraySize, outTriangleCount, box, transform );
    }
};

#endif // 0

#endif // AUTOMESH_HPP
