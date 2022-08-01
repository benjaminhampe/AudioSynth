#include <irrExt/AutoMeshBuffer.hpp>
#include <iostream>

void SceneNode_setPosition( irr::scene::ISceneNode* node, glm::vec3 const & pos )
{
   if ( !node ) return;
   node->setPosition( irr::core::vector3df( pos.x, pos.y, pos.z ) );
}

void SceneNode_setRotation( irr::scene::ISceneNode* node, glm::vec3 const & degrees )
{
   if ( !node ) return;
   node->setRotation( irr::core::vector3df( degrees.x, degrees.y, degrees.z ) );
}

/*
void
enumerateMesh( irr::scene::SMesh & p )
{
    std::cout << "VertexCount = " << p.Vertices.size() << "\n";
    for ( uint32_t i = 0; i < p.Vertices.size(); ++i )
    {
        std::cout << "Vertex[" << i << "]" << toString( p.Vertices[ i ] ) << "\n";
    }

    std::cout << "IndexCount = " << p.Indices.size() << "\n";

    for ( uint32_t i = 0; i < p.Indices.size(); ++i )
    {
        std::cout << "Index[" << i << "]" << p.Indices[ i ] << "\n";
    }

    std::cout << "Material = " << p.Vertices.size() << "\n";
    std::cout << "BoundingBox = " << p.Vertices.size() << "\n";
}
*/


void
enumerateMeshBuffer( irr::scene::SMeshBuffer & p )
{
    std::cout << "VertexCount = " << p.Vertices.size() << "\n";
    for ( uint32_t i = 0; i < p.Vertices.size(); ++i )
    {
        std::cout << "Vertex[" << i << "]" << p.Vertices[ i ] << "\n";
    }

    std::cout << "IndexCount = " << p.Indices.size() << "\n";

    for ( uint32_t i = 0; i < p.Indices.size(); ++i )
    {
        std::cout << "Index[" << i << "]" << p.Indices[ i ] << "\n";
    }

    std::cout << "Material = " << p.Vertices.size() << "\n";
    std::cout << "BoundingBox = " << p.Vertices.size() << "\n";
}

//=======================================================================================
//
// CLASS: AutoMeshBuffer
//
//=======================================================================================

AutoMeshBuffer::AutoMeshBuffer()
    : PrimitiveType( irr::scene::EPT_POINTS ) // This works for every mesh type ( aka fallback )
    , VertexType( irr::video::EVT_STANDARD )	// Standard Vertex: FVF_POSITION | FVF_NORMAL | FVF_COLOR32 | FVF_TEXCOORD0
    , IndexType( irr::video::EIT_16BIT )		// Standard up to 65535 Vertices
{
    MeshBuffer.Material.MaterialType = irr::video::EMT_SOLID;
    MeshBuffer.Material.Lighting = false;
    MeshBuffer.Material.FogEnable = false;
}

AutoMeshBuffer::AutoMeshBuffer( irr::scene::E_PRIMITIVE_TYPE primType, irr::video::E_VERTEX_TYPE vertexType, irr::video::E_INDEX_TYPE indexType )
    : PrimitiveType( primType )
    , VertexType( vertexType )
    , IndexType( indexType )
{
    MeshBuffer.Material.MaterialType = irr::video::EMT_SOLID;
    MeshBuffer.Material.Lighting = false;
    MeshBuffer.Material.FogEnable = false;
}

AutoMeshBuffer::~AutoMeshBuffer()
{}

// static
uint32_t
AutoMeshBuffer::getPrimitiveCount( irr::scene::E_PRIMITIVE_TYPE primitiveType, uint32_t indexCount )
{
    switch( primitiveType )
    {
        case irr::scene::EPT_POINTS: return indexCount;
        case irr::scene::EPT_LINES: return indexCount / 2;
        case irr::scene::EPT_LINE_LOOP: return indexCount - 1;
        case irr::scene::EPT_TRIANGLES: return indexCount / 3;
        case irr::scene::EPT_POLYGON: return indexCount;
        case irr::scene::EPT_QUADS: return indexCount / 4;
        default: return 0;
    }
}

uint32_t
AutoMeshBuffer::getPrimitiveCount() const
{
    return getPrimitiveCount( PrimitiveType, MeshBuffer.getIndexCount() );
}

void
AutoMeshBuffer::render( irr::video::IVideoDriver* driver )
{
    driver->setMaterial( MeshBuffer.Material );

    driver->drawVertexPrimitiveList(
        MeshBuffer.getVertices(),
        MeshBuffer.getVertexCount(),
        MeshBuffer.getIndices(),
        this->getPrimitiveCount(),
        VertexType,
        PrimitiveType,
        IndexType
    );
}

bool
AutoMeshBuffer::hasTransparentVertexColor() const
{
    for ( uint32_t i = 0; i < MeshBuffer.Vertices.size(); ++i )
    {
        if ( MeshBuffer.Vertices[ i ].Color.getAlpha() < 255 )
        {
            return true;
        }
    }
    return false;
}

void
AutoMeshBuffer::setDefaultMaterialType()
{
    MeshBuffer.Material.MaterialType = irr::video::EMT_SOLID;

    if ( hasTransparentVertexColor() )
    {
        MeshBuffer.Material.MaterialType = irr::video::EMT_TRANSPARENT_VERTEX_ALPHA;
    }
}

void
AutoMeshBuffer::setDefaultMaterialType( irr::video::SColor const & color )
{
    MeshBuffer.Material.MaterialType = irr::video::EMT_SOLID;

    if ( color.getAlpha() < 255 )
    {
        MeshBuffer.Material.MaterialType = irr::video::EMT_TRANSPARENT_VERTEX_ALPHA;
    }
}


bool
AutoMeshBuffer::getIntersectionWithLine( irr::core::line3df const & ray, irr::core::vector3df & hitPosition ) const
{
    if ( PrimitiveType == irr::scene::EPT_TRIANGLES )
    {
        uint32_t const vertexCount = MeshBuffer.Vertices.size();
        uint32_t const indexCount = MeshBuffer.Indices.size();

//            std::cout << __FUNCTION__ << "(" << toString( ray.start ) << ", " << toString( ray.getVector() ) << ")"
//                << " v:" << vertexCount << ", i:" << indexCount << "\n";

        for ( uint32_t i = 0; i < indexCount/3; ++i )
        {
            uint32_t const indexA = MeshBuffer.Indices[ 3*i ];
            uint32_t const indexB = MeshBuffer.Indices[ 3*i+1 ];
            uint32_t const indexC = MeshBuffer.Indices[ 3*i+2 ];
            if (   (indexA < vertexCount) && (indexB < vertexCount) && (indexC < vertexCount)   )
            {
                irr::core::vector3df const & A = MeshBuffer.Vertices[ indexA ].Pos;
                irr::core::vector3df const & B = MeshBuffer.Vertices[ indexB ].Pos;
                irr::core::vector3df const & C = MeshBuffer.Vertices[ indexC ].Pos;
                irr::core::triangle3df const tri( A, B, C );
                if ( tri.getIntersectionWithLine( ray.start, ray.getVector(), hitPosition ) )
                {
                    return true;
                }
            }
            else
            {
                std::cout << "[WARN] " << __FUNCTION__ << "(" << " v:" << vertexCount << ", i:" << indexCount << ")"
                    << "iA:" << indexA << ", iB:" << indexB << ", iC:" << indexC << " -> Broken mesh\n";
            }
        }
    }

    return false;
}

//=======================================================================================
//
// CLASS: AutoMesh
//
//=======================================================================================

AutoMesh::AutoMesh()
{
    #ifdef _DEBUG
    setDebugName("AutoMesh");
    #endif
    DefaultMaterial.Wireframe = false;
    DefaultMaterial.Lighting = false;
    DefaultMaterial.MaterialType = irr::video::EMT_SOLID;
}

AutoMesh::~AutoMesh()
{
    for ( irr::u32 i = 0; i < MeshBuffers.size(); ++i )
    {
        AutoMeshBuffer* p = MeshBuffers[ i ];
        if ( p )
        {
            p->drop();
        }
    }
}

void
AutoMesh::clear()
{
    for ( irr::u32 i = 0; i < MeshBuffers.size(); ++i )
    {
        AutoMeshBuffer* p = MeshBuffers[ i ];
        if ( p )
        {
            p->drop();
        }
    }
    MeshBuffers.clear();
    BoundingBox.reset ( 0.f, 0.f, 0.f );
}

irr::u32
AutoMesh::getMaterialCount() const
{
    return getMeshBufferCount();
}

irr::video::SMaterial &
AutoMesh::getMaterial( irr::u32 i )
{
    if ( i >= getMaterialCount() )
    {
        std::cout << __FUNCTION__ << "(" << i << ") :: [Error] Invalid material index.\n";
        return DefaultMaterial;
    }

    AutoMeshBuffer* p = MeshBuffers[ i ];
    if ( !p )
    {
        std::cout << __FUNCTION__ << "(" << i << ") :: [Error] Invalid pointer to AutoMeshBuffer\n";
        return DefaultMaterial;
    }

    return p->MeshBuffer.Material;
}

irr::u32
AutoMesh::getMeshBufferCount() const
{
    return static_cast< irr::u32 >( MeshBuffers.size() );
}

irr::scene::IMeshBuffer*
AutoMesh::getMeshBuffer( irr::u32 i ) const
{
    AutoMeshBuffer* p = MeshBuffers[ i ];
    if ( !p )
    {
        std::cout << __FUNCTION__ << "(" << i << ") :: [Error] Invalid pointer to AutoMeshBuffer\n";
        return nullptr;
    }
    return &(p->MeshBuffer);
}

irr::scene::IMeshBuffer*
AutoMesh::getMeshBuffer( irr::video::SMaterial const & material ) const
{
    for ( irr::s32 i = (irr::s32)MeshBuffers.size() - 1; i >= 0; --i )
    {
        AutoMeshBuffer* p = MeshBuffers[ i ];
        if ( p && p->MeshBuffer.getMaterial() == material )
        {
            return &(p->MeshBuffer);
        }
    }

    return nullptr;
}

irr::core::aabbox3d<irr::f32> const &
AutoMesh::getBoundingBox() const
{
    return BoundingBox;
}

void
AutoMesh::setBoundingBox( irr::core::aabbox3df const & box )
{
    BoundingBox = box;
}

void
AutoMesh::recalculateBoundingBox()
{
    if (MeshBuffers.size())
    {
        BoundingBox = MeshBuffers[ 0 ]->getBoundingBox();
        for ( irr::u32 i = 1; i < MeshBuffers.size(); ++i )
        {
            BoundingBox.addInternalBox( MeshBuffers[i]->getBoundingBox() );
        }
    }
    else
        BoundingBox.reset(0.0f, 0.0f, 0.0f);
}

void
AutoMesh::addMeshBuffer( irr::scene::IMeshBuffer* buf )
{
    std::cout << "[Warn] " << __FUNCTION__ << "() :: Should not be called\n";
}

void
AutoMesh::addAutoMeshBuffer( AutoMeshBuffer* buf, bool dropAfter )
{
    if ( !buf )
    {
        std::cout << "[Warn] " << __FUNCTION__ << " :: Cannot add nullptr\n";
        return;
    }

    buf->grab();

    if ( MeshBuffers.size() == 0 )
    {
        BoundingBox.reset( buf->getBoundingBox() );
    }
    else
    {
        BoundingBox.addInternalBox( buf->getBoundingBox() );
    }

    MeshBuffers.emplace_back( buf );

    if ( dropAfter )
    {
        buf->drop(); // remove old owner from ref count
    }
}

void AutoMesh::setMaterialFlag( irr::video::E_MATERIAL_FLAG flag, bool newvalue)
{
    for ( irr::u32 i=0; i<MeshBuffers.size(); ++i )
    {
        getMeshBuffer( i )->getMaterial().setFlag( flag, newvalue );
    }
}

void AutoMesh::setHardwareMappingHint( irr::scene::E_HARDWARE_MAPPING newMappingHint, irr::scene::E_BUFFER_TYPE buffer )
{
    for ( irr::u32 i=0; i<MeshBuffers.size(); ++i )
    {
        getMeshBuffer( i )->setHardwareMappingHint( newMappingHint, buffer);
    }
}

void AutoMesh::setDirty( irr::scene::E_BUFFER_TYPE buffer )
{
    for (irr::u32 i=0; i<MeshBuffers.size(); ++i)
    {
        getMeshBuffer( i )->setDirty(buffer);
    }
}

/// @brief Add collision detection
bool
AutoMesh::getIntersectionWithLine( irr::core::line3df const & ray, irr::core::vector3df & hitPosition ) const
{
    for ( uint32_t i = 0; i < MeshBuffers.size(); ++i )
    {
        AutoMeshBuffer* p = MeshBuffers[ i ];
        if ( p && p->getIntersectionWithLine( ray, hitPosition ) )
        {
            return true;
        }
    }

    return false;
}


//=======================================================================================
//
// CLASS: AutoSceneNode
//
//=======================================================================================

// #define ENABLE_AUTOSCENENODE_DEBUG_LOGGING

#ifdef ENABLE_AUTOSCENENODE_DEBUG_LOGGING
    #define AUTOSCENENODE_LOG_SIMPLE { std::cout << __FUNCTION__ << "()\n"; }
#else
    #define AUTOSCENENODE_LOG_SIMPLE
#endif

AutoSceneNode::AutoSceneNode( irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent, int id )
    : irr::scene::ISceneNode( parent, smgr, id )
{
    AUTOSCENENODE_LOG_SIMPLE
    //setAutomaticCulling( irr::scene::EAC_OFF );
}

AutoSceneNode::~AutoSceneNode()
{
    AUTOSCENENODE_LOG_SIMPLE
    clear();
}

//! This method is called just before the rendering process of the whole scene.
void
AutoSceneNode::OnRegisterSceneNode()
{
    if (IsVisible)
        SceneManager->registerNodeForRendering(this);

    ISceneNode::OnRegisterSceneNode();
}


void
AutoSceneNode::render()
{
    AUTOSCENENODE_LOG_SIMPLE

    irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();

    assert( driver );

    driver->setTransform( irr::video::ETS_WORLD, getAbsoluteTransformation() );

    for ( size_t i = 0; i < m_Mesh.MeshBuffers.size(); ++i )
    {
        AutoMeshBuffer* p = m_Mesh.MeshBuffers[ i ];
        if ( p )
        {
            p->render( driver );
        }
    }

    if ( DebugDataVisible & irr::scene::EDS_BBOX )
    {
        driver->draw3DBox( m_BoundingBox );
    }

    if ( DebugDataVisible & irr::scene::EDS_BBOX_BUFFERS )
    {
        for ( size_t i = 0; i < m_Mesh.MeshBuffers.size(); ++i )
        {
            AutoMeshBuffer* p = m_Mesh.MeshBuffers[ i ];
            if ( p )
            {
                driver->draw3DBox( p->MeshBuffer.BoundingBox, 0xFFEFEFEF );
            }
        }
    }
    // irr::scene::ISceneNode::render();
}

void
AutoSceneNode::clear()
{
    AUTOSCENENODE_LOG_SIMPLE
    m_Mesh.clear();
}

void
AutoSceneNode::addAutoMeshBuffer( AutoMeshBuffer * mb, bool dropAfterAdd )
{
    AUTOSCENENODE_LOG_SIMPLE
    m_Mesh.addAutoMeshBuffer( mb, dropAfterAdd );
}

//void
//AutoSceneNode::addMeshBuffer(
//        irr::scene::SMeshBuffer && mb,
//        irr::scene::E_PRIMITIVE_TYPE primType,
//        irr::video::E_VERTEX_TYPE vertexType,
//        irr::video::E_INDEX_TYPE indexType )
//{
//    AUTOSCENENODE_LOG_SIMPLE

//    m_MeshBuffer.emplace_back( new AutoMeshBuffer( std::move( mb ), primType, vertexType, indexType ) );
//}

irr::u32
AutoSceneNode::getMeshBufferCount() const
{
    return m_Mesh.getMeshBufferCount();
}

AutoMeshBuffer*
AutoSceneNode::getAutoMeshBuffer( irr::u32 i )
{
    assert( getMeshBufferCount() > i );
    return m_Mesh.MeshBuffers[ i ];
}

irr::scene::SMeshBuffer*
AutoSceneNode::getMeshBuffer( irr::u32 i )
{
    assert( getMeshBufferCount() > i );
    return static_cast< irr::scene::SMeshBuffer* >( m_Mesh.getMeshBuffer( i ) );
}

irr::u32
AutoSceneNode::getMaterialCount() const
{
    return m_Mesh.getMaterialCount();
}

irr::video::SMaterial&
AutoSceneNode::getMaterial( irr::u32 i )
{
    assert( getMaterialCount() > i );
    return m_Mesh.getMaterial( i );
}

irr::core::aabbox3d<irr::f32> const &
AutoSceneNode::getBoundingBox() const
{
    return m_BoundingBox;
}

/// @brief Add collision detection
/*
bool
AutoSceneNode::getIntersectionWithLine( irr::core::line3df const & ray, irr::core::vector3df & hitPosition ) const
{
    return m_Mesh.getIntersectionWithLine( ray, hitPosition );
}
*/

bool
AutoSceneNode::getIntersectionWithLine( irr::core::line3df const & ray, irr::core::vector3df & hitPosition ) const
{
    irr::core::matrix4 mat = this->getAbsoluteTransformation();

    for ( uint32_t b = 0; b < m_Mesh.MeshBuffers.size(); ++b )
    {
        AutoMeshBuffer * p = m_Mesh.MeshBuffers[ b ];
        if ( p && p->PrimitiveType == irr::scene::EPT_TRIANGLES )
        {
            uint32_t const vertexCount = p->MeshBuffer.Vertices.size();
            uint32_t const indexCount = p->MeshBuffer.Indices.size();

            for ( uint32_t i = 0; i < indexCount/3; ++i )
            {
                uint32_t const indexA = p->MeshBuffer.Indices[ 3*i ];
                uint32_t const indexB = p->MeshBuffer.Indices[ 3*i+1 ];
                uint32_t const indexC = p->MeshBuffer.Indices[ 3*i+2 ];
                if (   (indexA < vertexCount) && (indexB < vertexCount) && (indexC < vertexCount)   )
                {
                    irr::core::vector3df A = p->MeshBuffer.Vertices[ indexA ].Pos;
                    irr::core::vector3df B = p->MeshBuffer.Vertices[ indexB ].Pos;
                    irr::core::vector3df C = p->MeshBuffer.Vertices[ indexC ].Pos;
                    mat.transformVect( A );
                    mat.transformVect( B );
                    mat.transformVect( C );
                    irr::core::triangle3df const tri( A, B, C );
                    if ( tri.getIntersectionWithLine( ray.start, ray.getVector(), hitPosition ) )
                    {
                        return true;
                    }
                }
                else
                {
                    std::cout << "[WARN] " << __FUNCTION__ << "(" << " v:" << vertexCount << ", i:" << indexCount << ")"
                        << "iA:" << indexA << ", iB:" << indexB << ", iC:" << indexC << " -> Broken mesh\n";
                }
            }
        }
    }
    return false;
}


//void
//AutoSceneNode::setPosition( float32_t x, float32_t y, float32_t z )
//{
//    irr::scene::ISceneNode::setPosition( irr::core::vector3df( x, y, z ) );
//}
