#include <de/gpu/smesh/SMesh.hpp>

namespace de {
namespace gpu {

// ===========================================================================
SMesh::SMesh( std::string const & name )
   : ModelMat( 1.0 )
   , BoundingBox()
   , Name( name )
   , Buffers()
   , Visible( true )
{}

SMesh::SMesh( SMeshBuffer const & buf )
   : SMesh()
{
   addMeshBuffer( buf );
}

std::string
SMesh::getPrimTypesStr() const
{
   std::map< PrimitiveType::EType, uint32_t > primMap;

   for ( auto const & buffer : Buffers )
   {
      auto it = primMap.find( buffer.getPrimitiveType() );
      if ( it == primMap.end() )
      {
         primMap[ buffer.getPrimitiveType() ] = 1;
      }
      else
      {
         primMap[ buffer.getPrimitiveType() ]++;
      }
   }

   int k=0;
   std::stringstream s;

   auto it = primMap.begin();
   auto itEnd = primMap.end();
   while ( it != itEnd )
   {
      if ( k > 0 ) s << "+";
      std::string t = PrimitiveType::getString( it->first );
      s << t;
      k++;
      it++;
   }

   return s.str();
}


void
SMesh::upload()
{
   for ( auto & buf : Buffers )
   {
      buf.upload();
   }
}

void
SMesh::addEmptyBuffer()
{
   //auto n = Buffers.size();
   Buffers.emplace_back();
   //Buffers.back().Name = Name + std::to_string( n );
}

void
SMesh::addMeshBuffer( SMeshBuffer const & buf )
{
   if ( Buffers.empty() )
   {
      BoundingBox.reset( buf.getBoundingBox() );
   }
   else
   {
      BoundingBox.addInternalBox( buf.getBoundingBox() );
   }

   if ( Name.empty() )
   {
      Name = buf.Name;
   }

   Buffers.emplace_back( std::move( buf ) );

//   if ( Buffers.back().Name.empty() )
//   {
//      auto n = Buffers.size()-1;
//      Buffers.back().Name = Name + std::to_string( n );
//   }
}

void
SMesh::addMesh( SMesh const & other )
{
   for ( int i = 0; i < other.getMeshBufferCount(); ++i )
   {
      addMeshBuffer( other.getMeshBuffer( i ) );
   }
}

void
SMesh::clear()
{
   BoundingBox.reset();
   Buffers.clear();
}

bool
SMesh::isIndex( int i ) const
{
   if ( size_t( i ) >= Buffers.size() ) return false;
   return true;
}

int
SMesh::getMeshBufferCount() const { return Buffers.size(); }

SMeshBuffer const &
SMesh::getMeshBuffer( int i ) const
{
   if ( !isIndex( i ) )
   {
      std::ostringstream s;
      s << __FILE__ << ":" << __LINE__ << " invalid meshbuffer index " << i;
      throw std::runtime_error( s.str() );
   }
   return Buffers[ i ];
}

SMeshBuffer &
SMesh::getMeshBuffer( int i )
{
   if ( !isIndex( i ) )
   {
      std::ostringstream s;
      s << __FILE__ << ":" << __LINE__ << " invalid meshbuffer index " << i;
      throw std::runtime_error( s.str() );
   }
   return Buffers[ i ];
}

void
SMesh::removeMeshBuffer( int i )
{
   if ( !isIndex( i ) )
   {
      return;
   }
   Buffers.erase( Buffers.begin() + i );
   recalculateBoundingBox();
}

//   void
//   shiftMeshBuffer( int shifts )
//   {
//      printf( "shiftMeshBuffer( shifts = %d, buffercount = %d )\n", shifts, getMeshBufferCount() );
//      if ( shifts < 1 ) return;
//      uint32_t bufferCount = getMeshBufferCount();
//      for ( uint32_t b = 0; b < bufferCount; ++b )
//      {
//         uint32_t k = ( uint32_t(shifts) + b ) % bufferCount; // looks like no neg shifts, yet.
//         SMeshBuffer* tmp = Buffers[ k ];          // save value at target position
//         Buffers[ k ] = Buffers[ b ];// overwrite target position with new value
//         Buffers[ b ] = tmp;// overwrite source position with save row-data
//      }
//   }



void
SMesh::recalculateNormals()
{
   for ( auto & buf : Buffers )
   {
      SMeshBufferTool::computeNormals( buf );
   }
}

void
SMesh::recalculateBoundingBox( bool fully )
{
   if ( Buffers.empty() )
   {
      BoundingBox.reset();
   }
   else
   {
      if ( fully )
      {
         Buffers[ 0 ].recalculateBoundingBox();
      }

      BoundingBox.reset( Buffers[ 0 ].getBoundingBox() );

      for ( size_t i = 1; i < Buffers.size(); ++i )
      {
         SMeshBuffer & buf = Buffers[ i ];
         if ( fully )
         {
            buf.recalculateBoundingBox();
         }

         BoundingBox.addInternalBox( buf.getBoundingBox() );
      }
   }
}

//std::vector< Triangle3f >
//SMesh::createCollisionTriangles() const
//{
//   std::vector< Triangle3f > tris;

//   for ( SMeshBuffer const & p : Buffers )
//   {
//      std::vector< Triangle3f > tmp = SMeshBufferTool::createCollisionTriangles( p );
//      if ( tmp.size() > 0)
//      {
//         tris.insert( tris.end(), tmp.begin(), tmp.end() );
//      }
//   }

//   return tris;
//}

void
SMesh::flipVertexPosYZ()
{
   for ( SMeshBuffer & p : Buffers )
   {
      SMeshBufferTool::flipVertexPosYZ( p );
   }
   std::swap( BoundingBox.m_Min.y, BoundingBox.m_Min.z );
   std::swap( BoundingBox.m_Max.y, BoundingBox.m_Max.z );
}

void
SMesh::transformVertices( glm::dmat4 const & m )
{
   for ( SMeshBuffer & p : Buffers )
   {
      for ( size_t b = 0; b < p.Vertices.size(); ++b )
      {
         glm::dvec4 dp( p.Vertices[ b ].pos, 1.0 );
         glm::dvec4 dn( p.Vertices[ b ].normal, 1.0 );
         p.Vertices[ b ].pos = glm::dvec3( m * dp );
         p.Vertices[ b ].normal = glm::dvec3( m * dn );
      }
   }
}

void
SMesh::translateVertices( glm::vec3 const & offset )
{
   for ( SMeshBuffer & p : Buffers )
   {
      SMeshBufferTool::translateVertices( p, offset );
   }
}

void
SMesh::translateVertices( float x, float y, float z )
{
   translateVertices( glm::vec3( x,y,z ) );
}

void
SMesh::rotateVertices( float a, float b, float c )
{
   for ( SMeshBuffer & p : Buffers )
   {
      SMeshBufferTool::rotateVertices( p, a, b, c );
   }
   recalculateBoundingBox();
}

void
SMesh::colorVertices( uint32_t color )
{
   for ( SMeshBuffer & p : Buffers )
   {
      SMeshBufferTool::colorVertices( p, color );
   }
}

void
SMesh::centerVertices()
{
   recalculateBoundingBox();
   glm::vec3 const offset = -getBoundingBox().getCenter();

   for ( SMeshBuffer & p : Buffers )
   {
      SMeshBufferTool::translateVertices( p, offset );
      p.recalculateBoundingBox();
   }

   recalculateBoundingBox();
}

void
SMesh::scaleTexture( float u, float v )
{
   for ( SMeshBuffer & p : Buffers )
   {
      SMeshBufferTool::scaleTexture( p, u, v );
   }
}

void
SMesh::scaleVertices( float scale )
{
   for ( SMeshBuffer & p : Buffers )
   {
      SMeshBufferTool::scaleVertices( p, scale );
   }
   recalculateBoundingBox();
}

void
SMesh::scaleVertices( float sx, float sy, float sz )
{
   for ( SMeshBuffer & p : Buffers )
   {
      SMeshBufferTool::scaleVertices( p, sx,sy,sz );
   }
   recalculateBoundingBox();
}

std::string
SMesh::toString( bool printDetails ) const
{
   std::ostringstream s;
   s << "Mesh '" << dbGetFileName( Name ) << "' has " << Buffers.size() << " limbs";
   s << ", BBox(" << getBoundingBox().getSize() << ")";
   s << ", V(" << getVertexCount() << ")";
   s << ", I(" << getIndexCount() << ")";
   //s << ", T(" << getTextureCount() << ")";
   //s << "\n";

//      for ( size_t i = 0; i < Buffers.size(); ++i )
//      {
//         IMeshBuffer const * const p = Buffers[ i ];
//         if ( !p ) continue;
//         s << "Limb[" << i << "] " << p->toString() << "\n";
//      }
   return s.str();
}

size_t
SMesh::getVertexCount() const
{
   size_t n = 0;
   for ( SMeshBuffer const & p : Buffers )
   {
      n += p.getVertexCount();
   }
   return n;
}

size_t
SMesh::getIndexCount() const
{
   size_t n = 0;
   for ( SMeshBuffer const & p : Buffers )
   {
      n += p.getIndexCount();
   }
   return n;
}

size_t
SMesh::getByteCount() const
{
   size_t byteCount = sizeof( *this );
   for ( SMeshBuffer const & p : Buffers )
   {
      byteCount += p.getByteCount();
   }
   return byteCount;
}

int
SMesh::getMaterialCount() const
{
   return getMeshBufferCount();
}

SMaterial const &
SMesh::getMaterial( int i ) const
{
   if ( !isIndex( i ) )
   {
      std::ostringstream s;
      s << __FILE__ << ":" << __LINE__ << " invalid meshbuffer index " << i;
      throw std::runtime_error( s.str() );
   }
   return getMeshBuffer( i ).getMaterial();
}

SMaterial &
SMesh::getMaterial( size_t i )
{
   if ( !isIndex( i ) )
   {
      std::ostringstream s;
      s << __FILE__ << ":" << __LINE__ << " invalid meshbuffer index " << i;
      throw std::runtime_error( s.str() );
   }
   return getMeshBuffer( i ).getMaterial();
}

void
SMesh::setCulling( Culling cull )
{
   for ( SMeshBuffer & p : Buffers ) { p.setCulling( cull ); }
}

void
SMesh::setCulling( bool enable )
{
   for ( SMeshBuffer & p : Buffers ) { p.setCulling( enable ); }
}

void
SMesh::setDepth( Depth depth )
{
   for ( SMeshBuffer & p : Buffers ) { p.setDepth( depth ); }
}

void
SMesh::setStencil( Stencil stencil )
{
   for ( SMeshBuffer & p : Buffers ) { p.setStencil( stencil ); }
}

void
SMesh::setBlend( Blend blend )
{
   for ( SMeshBuffer & p : Buffers ) { p.setBlend( blend ); }
}


void
SMesh::setTexture( int stage, Tex* tex )
{
   for ( SMeshBuffer & p : Buffers ) { p.setTexture( stage, tex ); }
}

void
SMesh::setTexture( int stage, TexRef ref )
{
   for ( SMeshBuffer & p : Buffers ) { p.setTexture( stage, ref ); }
}

void
SMesh::setLighting( int illum )
{
   for ( SMeshBuffer & p : Buffers ) { p.setLighting( illum ); }
}

int
SMesh::getLighting() const
{
   int illum = 0;
   int count = int( Buffers.size() );
   if ( count > 0 )
   {
      for ( SMeshBuffer const & p : Buffers )
      {
         illum += p.Material.Lighting;
      }
      illum /= count;
   }

   return illum;
}

void
SMesh::setFogEnable( bool enable )
{
   for ( SMeshBuffer & p : Buffers ) { p.setFogEnable( enable ); }
}

void
SMesh::setWireframe( bool enable )
{
   for ( SMeshBuffer & p : Buffers ) { p.setWireframe( enable ); }
}
//   void
//   shiftMeshBuffer( int shifts )
//   {
//      printf( "shiftMeshBuffer( shifts = %d, buffercount = %d )\n", shifts, getMeshBufferCount() );
//      if ( shifts < 1 ) return;
//      uint32_t bufferCount = getMeshBufferCount();
//      for ( uint32_t b = 0; b < bufferCount; ++b )
//      {
//         uint32_t k = ( uint32_t(shifts) + b ) % bufferCount; // looks like no neg shifts, yet.
//         SMeshBuffer* tmp = Buffers[ k ];          // save value at target position
//         Buffers[ k ] = Buffers[ b ];// overwrite target position with new value
//         Buffers[ b ] = tmp;// overwrite source position with save row-data
//      }
//   }

/*
static bool
loadMeshBufferXML( std::string uri, SMeshBuffer & buffer )
{
   return false;
}

static bool
saveMeshBufferXML( std::string uri, SMeshBuffer const & buffer )
{
   tinyxml2::XMLDocument doc;
   tinyxml2::XMLElement* limb = doc.NewElement( "SMeshBuffer" );
   limb->SetAttribute( "n", buffer.getName().c_str() );
   limb->SetAttribute( "p", buffer.getPrimitiveTypeStr().c_str() );
   limb->SetAttribute( "v", buffer.getVertexCount() );
   limb->SetAttribute( "i", buffer.getIndexCount() );

   // Write Material:

   tinyxml2::XMLElement* matNode = doc.NewElement( "SMaterial" );
   matNode->SetAttribute( "n", buffer.Material.name.c_str() );

   for ( size_t i = 0; i < 4; ++i )
   {
      auto const & tex = buffer.Material.getTexture( i );
      if ( !tex.empty() )
      {
         tinyxml2::XMLElement* texNode = doc.NewElement( "Tex" );
         texNode->SetAttribute("stage", i );
         texNode->SetText( tex.tex->getName().c_str() );
         matNode->InsertEndChild( texNode );
      }
   }

   limb->InsertEndChild( matNode );

   // Write Vertices:

   for ( size_t vi=0; vi < buffer.Vertices.size(); ++vi )
   {
      S3DVertex const & v = buffer.Vertices[ vi ];
      tinyxml2::XMLElement* vlem = doc.NewElement( "v" );
      if ( v.pos.x != 0.f ) { vlem->SetAttribute( "x", v.pos.x ); }
      if ( v.pos.y != 0.f ) { vlem->SetAttribute( "y", v.pos.y ); }
      if ( v.pos.z != 0.f ) { vlem->SetAttribute( "z", v.pos.z ); }
      if ( v.normal.x != 0.f ) { vlem->SetAttribute( "nx", v.normal.x ); }
      if ( v.normal.y != 0.f ) { vlem->SetAttribute( "ny", v.normal.y ); }
      if ( v.normal.z != 0.f ) { vlem->SetAttribute( "nz", v.normal.z ); }
      if ( v.color != 0xFFFFFFFF ) { vlem->SetAttribute( "color", v.color ); }
      if ( v.tex.x != 0.f ) { vlem->SetAttribute( "u", v.tex.x ); }
      if ( v.tex.y != 0.f ) { vlem->SetAttribute( "v", v.tex.y ); }

      limb->InsertEndChild( vlem );
   }

   // Write Indices:

   if ( buffer.Indices.size() > 0 )
   {
      tinyxml2::XMLElement* ilem = doc.NewElement( "i" );

      std::ostringstream oss;
      oss << buffer.Indices[ 0 ];

      for ( size_t k = 1; k < buffer.Indices.size(); ++k )
      {
         oss << "," << buffer.Indices[ k ];
      }
      ilem->SetText( oss.str().c_str() );
      limb->InsertEndChild( ilem );
   }

   doc.InsertFirstChild( limb );

   tinyxml2::XMLError err = doc.SaveFile( uri.c_str() );
   if ( err != tinyxml2::XML_SUCCESS )
   {
      DE_ERROR("Cant save");
      return false;
   }

   return true;
}
*/

} // end namespace gpu.
} // end namespace de.
