#include <irrExt/createRoundRect.hpp>
#include <irrExt/sinCosTable.hpp>

AutoMeshBuffer*
    createRoundRect(
    glm::vec3 pos,
    glm::vec2 size,
    glm::vec2 r,
    uint32_t tesselation,
    uint32_t color )
{
    AutoMeshBuffer* p = new AutoMeshBuffer( irr::scene::EPT_TRIANGLES );

    // bbox
    p->MeshBuffer.BoundingBox = irr::core::aabbox3df( pos.x-0.5f*size.x, pos.y-0.5f*size.y, pos.z,
                                                      pos.x+0.5f*size.x, pos.y+0.5f*size.y, pos.z);

    // material
    p->setDefaultMaterialType( color );

    // vertices = 12 + 4*tesselation

    float const fw = 1.0f/(float)size.x;
    float const fh = 1.0f/(float)size.y;
    float const frw = (float)r.x*fw;
    float const frh = (float)r.y*fh;
    float const fwrw = (float)(size.x-r.x)*fw;
    float const fhrh = (float)(size.y-r.y)*fh;
    float const x = -0.5f*size.x;
    float const y = -0.5f*size.y;

    glm::vec3 const A ( x+r.x,       y+r.y,0 );
    glm::vec3 const B ( x+r.x,       y-r.y+size.y,0 );
    glm::vec3 const C ( x+size.x-r.x,y-r.y+size.y,0 );
    glm::vec3 const D ( x+size.x-r.x,y+r.y,0 );
    glm::vec3 const E ( x+r.x,       y,0 );
    glm::vec3 const F ( x+size.x-r.x,y,0 );
    glm::vec3 const G ( x+r.x,       y+size.y,0 );
    glm::vec3 const H ( x+size.x-r.x,y+size.y,0);
    glm::vec3 const I ( x+0,        y+r.y,0);
    glm::vec3 const J ( x+0,        y-r.y+size.y,0);
    glm::vec3 const K ( x+size.x,   y-r.y+size.y,0);
    glm::vec3 const L ( x+size.x,   y+r.y,0);

    // vertices = 12 + 4 * tesselation
    p->MeshBuffer.Vertices.reallocate(12 + 4*tesselation);
    p->MeshBuffer.Vertices.set_used(0);

    auto addVertex = [ p, &pos, color ] ( glm::vec3 const & P, float32_t u, float32_t v )
    {
        p->MeshBuffer.Vertices.push_back( irr::video::S3DVertex( pos.x + P.x, pos.y + P.y, pos.z + P.z, 0, 0, -1, color, u, v ) );
    };

    addVertex( A, frw, fhrh ); // A=0
    addVertex( B, frw,frh ); // B=1
    addVertex( C, fwrw,frh ); // C=2
    addVertex( D, fwrw,fhrh ); // D=3
    addVertex( E, frw,1 ); // E=4
    addVertex( F, fwrw,1 ); // F=5
    addVertex( G, frw,0 ); // G=6
    addVertex( H, fwrw,0 ); // H=7
    addVertex( I, 0,fhrh ); // I=8
    addVertex( J, 0,frh ); // J=9
    addVertex( K, 1,frh ); // K=A
    addVertex( L, 1,fhrh ); // L=B

    // indices += 30 + 4*3*(tesselation+1)
    p->MeshBuffer.Indices.reallocate(30 + 4*3*(tesselation+1));
    p->MeshBuffer.Indices.set_used(0);

    auto addTriangle = [ p ] ( uint16_t a, uint16_t b, uint16_t c )
    {
        p->MeshBuffer.Indices.push_back( a );
        p->MeshBuffer.Indices.push_back( b );
        p->MeshBuffer.Indices.push_back( c );
    };

    addTriangle( 0, 1, 2 ); 	// ABC = 012
    addTriangle( 0, 2, 3 ); 	// ACD = 023
    addTriangle( 1, 6, 7 );		// BGH = 167
    addTriangle( 1, 7, 2 );		// BHC = 172
    addTriangle( 4, 0, 3 );		// EAD = 403
    addTriangle( 4, 3, 5 );		// EDF = 435
    addTriangle( 8, 9, 1 );		// IJB = 891
    addTriangle( 8, 1, 0 );		// IBA = 810
    addTriangle( 3, 2, 10 );	// DCK = 32A
    addTriangle( 3, 10, 11 );	// DKL = 3AB

    uint32_t const INDEX_A = 0;
    uint32_t const INDEX_B = 1;
    uint32_t const INDEX_C = 2;
    uint32_t const INDEX_D = 3;
    uint32_t const INDEX_E = 4;
    uint32_t const INDEX_F = 5;
    uint32_t const INDEX_G = 6;
    uint32_t const INDEX_H = 7;
    uint32_t const INDEX_I = 8;
    uint32_t const INDEX_J = 9;
    uint32_t const INDEX_K = 10;
    uint32_t const INDEX_L = 11;

    SinCosTablef const table( tesselation, 0.0f, 0.5f * M_PI );

    //! topLeft BJG quarter circle
    // vertices += tesselation
    uint32_t k = p->MeshBuffer.Vertices.size();
    for ( uint32_t i = 0; i < tesselation; ++i )
    {
        float u = (float)r.x*(1.0f-table[ i ].c)/(float)size.x;
        float v = (float)r.y*(1.0f-table[ i ].s)/(float)size.y;
        addVertex( glm::vec3( B.x - r.x*table[ i ].c, B.y + r.y*table[i].s, B.z), u, v );
    }
    // indices += 3*(tesselation+1)
    for ( uint32_t i = 0; i < tesselation + 1; ++i )
    {
        uint32_t b,c;
        if (i==0) 					{ b = INDEX_J; c = k; }
        else if (i==tesselation) 	{ b = k+i-1; c = INDEX_G; }
        else 						{ b = k+i-1; c = k+i; }
        addTriangle( INDEX_B, b, c );
    }

    //! topRight CHK quarter circle
    // vertices += tesselation
    k = p->MeshBuffer.Vertices.size();
    for ( uint32_t i = 0; i < tesselation; ++i )
    {
        float u = 1.0f-(float)r.x*(1.0f-table[ i ].c)/(float)size.x;
        float v = (float)r.y*(1.0f-table[ i ].s)/(float)size.y;
        addVertex( glm::vec3( C.x + r.x*table[ i ].c, C.y + r.y*table[i].s, C.z), u, v );
    }
    // indices += 3*(tesselation+1)
    for ( uint32_t i = 0; i < tesselation + 1; ++i )
    {
        uint32_t b,c;
        if (i==0) 					{ b=k; c=INDEX_K; }
        else if (i==tesselation) 	{ b=INDEX_H; c=k+i-1; }
        else 						{ b=k+i; c=k+i-1; }
        addTriangle( INDEX_C,b,c );
    }

    //! bottomRight DLF quarter circle
    // vertices += tesselation
    k = p->MeshBuffer.Vertices.size();
    for ( uint32_t i = 0; i < tesselation; ++i )
    {
        float u = 1.0f-(float)r.x*(1.0f-table[ i ].c)/(float)size.x;
        float v = 1.0f-(float)r.y*(1.0f-table[ i ].s)/(float)size.y;
        addVertex( glm::vec3( D.x + r.x*table[ i ].c, D.y - r.y*table[i].s, D.z), u, v );
    }
    // indices += 3*(tesselation+1)
    for ( uint32_t i = 0; i < tesselation + 1; ++i )
    {
        uint32_t b,c;
        if (i==0) 					{ b=INDEX_L; c=k; }
        else if (i==tesselation) 	{ b=k+i-1; c=INDEX_F; }
        else 						{ b=k+i-1; c=k+i; }
        addTriangle( INDEX_D,b,c );
    }

    //! bottomLeft AEI quarter circle
    // vertices += tesselation
    k = p->MeshBuffer.Vertices.size();
    for ( uint32_t i = 0; i < tesselation; ++i )
    {
        float u = (float)r.x*(1.0f-table[ i ].c)/(float)size.x;
        float v = 1.0f-(float)r.y*(1.0f-table[ i ].s)/(float)size.y;
        addVertex( glm::vec3( A.x - r.x*table[ i ].c, A.y - r.y*table[i].s, A.z), u, v );
    }
    // indices += 3*(tesselation+1)
    for ( uint32_t i = 0; i < tesselation + 1; ++i )
    {
        uint32_t b,c;
        if (i==0) 					{ b=k; c=INDEX_I; }
        else if (i==tesselation) 	{ b=INDEX_E; c=k+i-1; }
        else 						{ b=k+i; c=k+i-1; }
        addTriangle( INDEX_A,b,c );
    }

    return p;
}


irr::scene::SMeshBuffer*
createRoundRectMeshBuffer(
    glm::vec2 size,
    glm::vec2 r,
    uint32_t tesselation,
    bool bFullyTextured,
    irr::video::SColor const & outerColor,
    irr::video::SColor const & innerColor )
{
    irr::scene::SMeshBuffer* buffer = new irr::scene::SMeshBuffer();
    assert( buffer );

    // tables
    SinCosTablef table( tesselation + 1 );

    // bbox
    buffer->setBoundingBox(
        irr::core::aabbox3df(-0.5f*size.x, -0.5f*size.y, 0.0f, 0.5f*size.x, 0.5f*size.y, 0.0f) );

    // material
    if (outerColor.getAlpha()<255 || innerColor.getAlpha()<255)
        buffer->Material.MaterialType = irr::video::EMT_TRANSPARENT_VERTEX_ALPHA;
    else
        buffer->Material.MaterialType = irr::video::EMT_SOLID;

    float const fw = 1.0f/(float)size.x;
    float const fh = 1.0f/(float)size.y;
    float const frw = (float)r.x*fw;
    float const frh = (float)r.y*fh;
    float const fwrw = (float)(size.x-r.x)*fw;
    float const fhrh = (float)(size.y-r.y)*fh;
    float const x = -0.5f*size.x;
    float const y = -0.5f*size.y;

    // vertices = 12 + 4*tesselation
    buffer->Vertices.reallocate(12 + 4*tesselation);
    buffer->Vertices.set_used(0);
    irr::core::vector3df const A ( x+r.x,       y+r.y,0 );
    irr::core::vector3df const B ( x+r.x,       y-r.y+size.y,0 );
    irr::core::vector3df const C ( x+size.x-r.x,y-r.y+size.y,0 );
    irr::core::vector3df const D ( x+size.x-r.x,y+r.y,0 );
    irr::core::vector3df const E ( x+r.x,       y,0 );
    irr::core::vector3df const F ( x+size.x-r.x,y,0 );
    irr::core::vector3df const G ( x+r.x,       y+size.y,0 );
    irr::core::vector3df const H ( x+size.x-r.x,y+size.y,0);
    irr::core::vector3df const I ( x+0,              y+r.y,0);
    irr::core::vector3df const J ( x+0,              y-r.y+size.y,0);
    irr::core::vector3df const K ( x+size.x,         y-r.y+size.y,0);
    irr::core::vector3df const L ( x+size.x,         y+r.y,0);
    irr::core::vector3df const n ( 0,0,-1);

    // vertices += 12
    buffer->Vertices.push_back( irr::video::S3DVertex( A, n, innerColor, irr::core::vector2df(frw,fhrh)) ); // A=0
    buffer->Vertices.push_back( irr::video::S3DVertex( B, n, innerColor, irr::core::vector2df(frw,frh)) ); 	// B=1
    buffer->Vertices.push_back( irr::video::S3DVertex( C, n, innerColor, irr::core::vector2df(fwrw,frh)) );	// C=2
    buffer->Vertices.push_back( irr::video::S3DVertex( D, n, innerColor, irr::core::vector2df(fwrw,fhrh)) );// D=3
    buffer->Vertices.push_back( irr::video::S3DVertex( E, n, outerColor, irr::core::vector2df(frw,1)) ); 	// E=4
    buffer->Vertices.push_back( irr::video::S3DVertex( F, n, outerColor, irr::core::vector2df(fwrw,1)) ); 	// F=5
    buffer->Vertices.push_back( irr::video::S3DVertex( G, n, outerColor, irr::core::vector2df(frw,0)) ); 	// G=6
    buffer->Vertices.push_back( irr::video::S3DVertex( H, n, outerColor, irr::core::vector2df(fwrw,0)) ); 	// H=7
    buffer->Vertices.push_back( irr::video::S3DVertex( I, n, outerColor, irr::core::vector2df(0,fhrh)) ); 	// I=8
    buffer->Vertices.push_back( irr::video::S3DVertex( J, n, outerColor, irr::core::vector2df(0,frh)) ); 	// J=9
    buffer->Vertices.push_back( irr::video::S3DVertex( K, n, outerColor, irr::core::vector2df(1,frh)) ); 	// K=A
    buffer->Vertices.push_back( irr::video::S3DVertex( L, n, outerColor, irr::core::vector2df(1,fhrh)) ); 	// L=B

    // indices += 30 + 4*3*(tesselation+1)
    buffer->Indices.reallocate(30 + 4*3*(tesselation+1));
    buffer->Indices.set_used(0);

    // indices += 30
    auto addTriangle = [&buffer] ( uint16_t a, uint16_t b, uint16_t c )
    {
        buffer->Indices.push_back( a );
        buffer->Indices.push_back( b );
        buffer->Indices.push_back( c );
    };

    addTriangle( 0, 1, 2 ); 	// ABC = 012
    addTriangle( 0, 2, 3 ); 	// ACD = 023
    addTriangle( 1, 6, 7 );		// BGH = 167
    addTriangle( 1, 7, 2 );		// BHC = 172
    addTriangle( 4, 0, 3 );		// EAD = 403
    addTriangle( 4, 3, 5 );		// EDF = 435
    addTriangle( 8, 9, 1 );		// IJB = 891
    addTriangle( 8, 1, 0 );		// IBA = 810
    addTriangle( 3, 2, 10 );	// DCK = 32A
    addTriangle( 3, 10, 11 );	// DKL = 3AB

    uint32_t const INDEX_A = 0;
    uint32_t const INDEX_B = 1;
    uint32_t const INDEX_C = 2;
    uint32_t const INDEX_D = 3;
    uint32_t const INDEX_E = 4;
    uint32_t const INDEX_F = 5;
    uint32_t const INDEX_G = 6;
    uint32_t const INDEX_H = 7;
    uint32_t const INDEX_I = 8;
    uint32_t const INDEX_J = 9;
    uint32_t const INDEX_K = 10;
    uint32_t const INDEX_L = 11;

    //! topLeft BJG quarter circle
    // vertices += tesselation
    uint32_t k = buffer->Vertices.size();
    for (uint32_t i=0; i<tesselation; i++)
    {
        float x = buffer->Vertices[INDEX_B].Pos.X - r.x*table[ i ].c;
        float y = buffer->Vertices[INDEX_B].Pos.Y + r.y*table[i].s;
        float z = buffer->Vertices[INDEX_B].Pos.Z;
        float u = (float)r.x*(1.0f-table[ i ].c)/(float)size.x;
        float v = (float)r.y*(1.0f-table[ i ].s)/(float)size.y;
        buffer->Vertices.push_back( irr::video::S3DVertex( x, y,	z, 0, 0, -1, outerColor, u, v ));
    }
    // indices += 3*(tesselation+1)
    for (uint32_t i=0; i<tesselation+1; i++)
    {
        uint32_t a,b,c;
        if (i==0) 					{ a = INDEX_B; b = INDEX_J; c = k; }
        else if (i==tesselation) 	{ a = INDEX_B; b = k+i-1; c = INDEX_G; }
        else 						{ a = INDEX_B; b = k+i-1; c = k+i; }
        buffer->Indices.push_back(a);
        buffer->Indices.push_back(b);
        buffer->Indices.push_back(c);
    }

    //! topRight CHK quarter circle
    // vertices += tesselation
    k=buffer->Vertices.size();
    for (uint32_t i=0; i<tesselation; i++)
    {
        float x = buffer->Vertices[INDEX_C].Pos.X + r.x*table[ i ].c;
        float y = buffer->Vertices[INDEX_C].Pos.Y + r.y*table[i].s;
        float z = buffer->Vertices[INDEX_C].Pos.Z;
        float u = 1.0f-(float)r.x*(1.0f-table[ i ].c)/(float)size.x;
        float v = (float)r.y*(1.0f-table[ i ].s)/(float)size.y;
        buffer->Vertices.push_back( irr::video::S3DVertex( x, y, z, 0, 0, -1, outerColor, u, v ));
    }
    // indices += 3*(tesselation+1)
    for (uint32_t i=0; i<tesselation+1; i++)
    {
        uint32_t a,b,c;
        if (i==0) 					{ a=INDEX_C; b=k; c=INDEX_K; }
        else if (i==tesselation) 	{ a=INDEX_C; b=INDEX_H; c=k+i-1; }
        else 						{ a=INDEX_C; b=k+i; c=k+i-1; }
        buffer->Indices.push_back(a);
        buffer->Indices.push_back(b);
        buffer->Indices.push_back(c);
    }

    //! bottomRight DLF quarter circle
    // vertices += tesselation
    k=buffer->Vertices.size();
    for (uint32_t i=0; i<tesselation; i++)
    {
        float x = buffer->Vertices[INDEX_D].Pos.X + r.x*table[ i ].c;
        float y = buffer->Vertices[INDEX_D].Pos.Y - r.y*table[i].s;
        float z = buffer->Vertices[INDEX_D].Pos.Z;
        float u = 1.0f-(float)r.x*(1.0f-table[ i ].c)/(float)size.x;
        float v = 1.0f-(float)r.y*(1.0f-table[ i ].s)/(float)size.y;
        buffer->Vertices.push_back( irr::video::S3DVertex( x, y, z, 0, 0, -1, outerColor, u, v ));
    }
    // indices += 3*(tesselation+1)
    for (uint32_t i=0; i<tesselation+1; i++)
    {
        uint32_t a,b,c;
        if (i==0) 					{ a=INDEX_D; b=INDEX_L; c=k; }
        else if (i==tesselation) 	{ a=INDEX_D; b=k+i-1; c=INDEX_F; }
        else 						{ a=INDEX_D; b=k+i-1; c=k+i; }
        buffer->Indices.push_back(a);
        buffer->Indices.push_back(b);
        buffer->Indices.push_back(c);
    }

    //! bottomLeft AEI quarter circle
    // vertices += tesselation
    k=buffer->Vertices.size();
    for (uint32_t i=0; i<tesselation; i++)
    {
        float x = buffer->Vertices[INDEX_A].Pos.X - r.x*table[ i ].c;
        float y = buffer->Vertices[INDEX_A].Pos.Y - r.y*table[i].s;
        float z = buffer->Vertices[INDEX_A].Pos.Z;
        float u = (float)r.x*(1.0f-table[ i ].c)/(float)size.x;
        float v = 1.0f-(float)r.y*(1.0f-table[ i ].s)/(float)size.y;
        buffer->Vertices.push_back( irr::video::S3DVertex( x, y, z, 0, 0, -1, outerColor, u, v ));
    }
    // indices += 3*(tesselation+1)
    for (uint32_t i=0; i<tesselation+1; i++)
    {
        uint32_t a,b,c;
        if (i==0) 					{ a=INDEX_A; b=k; c=INDEX_I; }
        else if (i==tesselation) 	{ a=INDEX_A; b=INDEX_E; c=k+i-1; }
        else 						{ a=INDEX_A; b=k+i; c=k+i-1; }
        buffer->Indices.push_back(a);
        buffer->Indices.push_back(b);
        buffer->Indices.push_back(c);
    }

    return buffer;
}
