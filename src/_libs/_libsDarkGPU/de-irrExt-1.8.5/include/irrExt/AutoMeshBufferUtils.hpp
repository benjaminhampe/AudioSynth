#ifndef DE_ALPHASONIC_AUTO_MESH_BUFFER_UTILS_HPP
#define DE_ALPHASONIC_AUTO_MESH_BUFFER_UTILS_HPP

#include <irrExt/AutoMeshBuffer.hpp>
#include <irrExt/sinCosTable.hpp>

glm::vec3
getNormal( glm::vec3 A, glm::vec3 B, glm::vec3 C );

/// @brief Translate all vertices by a vector
void
translate( AutoMeshBuffer & p, glm::vec3 const & offset );

/// @brief Rotate all vertices by a vector
void
rotate( AutoMeshBuffer & p, glm::vec3 const & degrees );

AutoMeshBuffer*
createRect( glm::vec3 pos, glm::vec2 size, uint32_t color );

/// @brief Create a box with one repeating texture over all 6 planes
AutoMeshBuffer *
createBox( glm::vec3 const & pos, glm::vec3 const & size, uint32_t color );

/// @brief Create a box with one repeating texture over all 6 planes
AutoMeshBuffer *
createRotatedBox( glm::vec3 const & pos, glm::vec3 const & size, glm::vec3 const & rot, uint32_t color );

/// @brief Create a dice mesh ( box ) with 6 mesh-buffers and 6 textures
AutoMesh *
createDice( glm::vec3 const & size, uint32_t color );

/// Hexagon ( ver_2018 ):
/**
                 D        triangles: ABF, BCE, BEF, CDE
                / \
              C/   \ E    M (x=0,y=0,z=0,u=0.5,v=0.5)
               |----|     F( 0.5, 0, -.25, 1, 0.25 )
               |  M |     E( 0.5, 0, 0.25, 1, 0.75 )
               |----|     D( 0.0, 0, 0.50, 0.5, 1 )
              B \  / F    C( -.5, 0, 0.25, 0, 0.75 )
                 \/       B( -.5, 0, -.25, 0, 0.25 )
                  A       A( 0, 0, -.5, .5, 0 )
*/

glm::vec3
getHexagonPoint( float w, float h, int i );

AutoMeshBuffer*
createHexagon( float w, float h );

AutoMeshBuffer*
createHafenXZ( float w, float h, int i );

AutoMeshBuffer*
createCircle( glm::vec3 const & pos, float r, uint32_t segments = 36, uint32_t color = 0xFFFFFFFF );

AutoMeshBuffer*
createCircleXZ( glm::vec3 const & pos, float r, uint32_t segments );

AutoMeshBuffer*
createCylinderHull(
    glm::vec3 pos,
    float radius,
    float height,
    uint32_t color,
    uint32_t tessCircle = 33,
    uint32_t tessHull = 1);

void
addVertex( AutoMeshBuffer & buffer, glm::vec3 p, glm::vec3 n, uint32_t color, glm::vec2 uv, bool flipV = true );

void
addTriangle( AutoMeshBuffer & buffer, glm::vec3 A, glm::vec3 B, glm::vec3 C, uint32_t color, glm::vec2 uvA, glm::vec2 uvB, glm::vec2 uvC, bool flipV = true );

void
addTriangle( AutoMeshBuffer & buffer, glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 n, uint32_t color, glm::vec2 uvA, glm::vec2 uvB, glm::vec2 uvC, bool flipV = true );

void
addQuad( AutoMeshBuffer & buffer, glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 D, uint32_t color, bool flipV = true );

/*
void
addRect( AutoMeshBuffer* buffer, glm::vec3 pos, glm::vec2 size, uint32_t color);

/// @brief Add a rect to a meshbuffer.
inline void addRect(
        irr::scene::SMeshBuffer* buffer,
        float32_t width,
        float32_t height,
        irr::video::SColor const & bottomLeftColor = 0xffffffff,
        irr::video::SColor const & topLeftColor = 0xffffffff,
        irr::video::SColor const & topRightColor = 0xffffffff,
        irr::video::SColor const & bottomRightColor = 0xffffffff )
{
    // vertices:
    irr::core::vector3df const n( 0,0,-1 );
    float const w = 0.5f * width;
    float const h = 0.5f * height;
    irr::core::vector3df const A( -w,-h,0 );
    irr::core::vector3df const B( -w, h,0 );
    irr::core::vector3df const C(  w, h,0 );
    irr::core::vector3df const D(  w,-h,0 );
    irr::core::vector2df const uvA( 0,1 );
    irr::core::vector2df const uvB( 0,0 );
    irr::core::vector2df const uvC( 1,0 );
    irr::core::vector2df const uvD( 1,1 );
    buffer->Vertices.push_back( irr::video::S3DVertex( A,n,bottomLeftColor,uvA ) );
    buffer->Vertices.push_back( irr::video::S3DVertex( B,n,topLeftColor,uvB ) );
    buffer->Vertices.push_back( irr::video::S3DVertex( C,n,topRightColor,uvC ) );
    buffer->Vertices.push_back( irr::video::S3DVertex( D,n,bottomRightColor,uvD ) );
    buffer->BoundingBox.addInternalPoint( A );
    buffer->BoundingBox.addInternalPoint( B );
    buffer->BoundingBox.addInternalPoint( C );
    buffer->BoundingBox.addInternalPoint( D );
    buffer->Indices.push_back( 0 );
    buffer->Indices.push_back( 1 );
    buffer->Indices.push_back( 2 );
    buffer->Indices.push_back( 0 );
    buffer->Indices.push_back( 2 );
    buffer->Indices.push_back( 3 );
}
*/

//  F  G
// /E /H
// B--C
// |/ |
// A--D

/// @brief Create a box with one repeating texture over all 6 planes
void
addBox( AutoMeshBuffer & p, glm::vec3 const & pos, glm::vec3 const & size, uint32_t color );

/// @brief Create a box with one repeating texture over all 6 planes
void
addBoxPlane( AutoMeshBuffer & p, int i, glm::vec3 const & pos, glm::vec3 const & size, uint32_t color );

/// @brief Create a box
void
addBoxWithoutTop( AutoMeshBuffer & p, glm::vec3 const & pos, glm::vec3 const & size, uint32_t color );

/// @brief Create a box
void
addBoxWithoutTopAndBottom( AutoMeshBuffer & p, glm::vec3 const & pos, glm::vec3 const & size, uint32_t color );

#endif // DE_ALPHASONIC_AUTO_MESH_BUFFER_HPP
