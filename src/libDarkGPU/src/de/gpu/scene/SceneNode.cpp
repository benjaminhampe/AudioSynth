#include <de/gpu/scene/SceneNode.hpp>
#include <de/gpu/scene/SceneManager.hpp>
#include <de/gpu/IVideoDriver.hpp>

namespace de {
namespace gpu {

ISceneNode::ISceneNode( ISceneManager* smgr, ISceneNode* parent, int id )
   : m_SceneManager( smgr )
   , m_Parent( parent )
   , m_Id( id )
   , m_IsVisible( true )
   , m_IsDirty( true )
   , m_IsCollisionEnabled( true )
   , m_AbsoluteTransform(1)   // default ctr identity matrix.
   , m_RelativeTranslation(0,0,0)
   , m_RelativeRotation(0,0,0)
   , m_RelativeScale(1,1,1)
   , m_Children()
{
//   if ( m_SceneManager )
//   {
//      m_SceneManager->addSceneNode( this );
//   }

   if ( m_Parent )
   {
      m_Parent->addChild( this );
   }

   updateAbsoluteTransform();
}

ISceneNode::~ISceneNode()
{
   removeAll();
   // delete all animators
   // m_TriangleSelector->drop();
}


ISceneManager*
ISceneNode::getSceneManager() { return m_SceneManager; }

std::string
ISceneNode::getName() const { return m_Name; }
void
ISceneNode::setName( std::string const & name ) { m_Name = name; }

std::string
ISceneNode::toString() const
{
   std::stringstream s;
   s << getName();
   return s.str();
}

bool
ISceneNode::isCollisionEnabled() const { return m_IsCollisionEnabled; }
void
ISceneNode::setCollisionEnabled( bool enable ) { m_IsCollisionEnabled = enable; }

VisualDebugData const &
ISceneNode::getDebugData() const { return m_DebugData; }
VisualDebugData &
ISceneNode::getDebugData() { return m_DebugData; }
void
ISceneNode::setDebugData( VisualDebugData const & debugData ) { m_DebugData = debugData; }

// Get the axis aligned, transformed and animated absolute bounding box of this node.
Box3f
ISceneNode::getAbsoluteBoundingBox() const
{
   return getBoundingBox().transformBox( m_AbsoluteTransform );
}

// Get the absolute transformation of the node. Is recalculated every OnAnimate()-call.
ISceneNode::M4 const &
ISceneNode::getAbsoluteTransform() const { return m_AbsoluteTransform; }

// Gets the absolute position of the node in world coordinates.
ISceneNode::V3
ISceneNode::getAbsolutePosition() const
{
   T const * const m = glm::value_ptr( m_AbsoluteTransform );
   T x = m[ 12 ];
   T y = m[ 13 ];
   T z = m[ 14 ];
   DE_DEBUG("AbsolutePosition = (",x,",",y,",",z,")")
   return { x, y, z };
}

// Updates the absolute position based on the relative and the parents position
void
ISceneNode::updateAbsoluteTransform()
{
   if ( !m_IsDirty ) return;

   if ( m_Parent )
   {
      m_AbsoluteTransform = m_Parent->getAbsoluteTransform() * getRelativeTransform();
   }
   else
   {
      m_AbsoluteTransform = getRelativeTransform();
   }
   m_IsDirty = false;
}

// Returns the relative transformation of the scene node.
ISceneNode::M4
ISceneNode::getRelativeTransform() const
{
   M4 t = glm::translate( M4(1), m_RelativeTranslation );
   M4 rx = glm::rotate( glm::dmat4( 1.0 ), glm::radians( m_RelativeRotation.x ), { 1., 0., 0. } );
   M4 ry = glm::rotate( glm::dmat4( 1.0 ), glm::radians( m_RelativeRotation.y ), { 0., 1., 0. } );
   M4 rz = glm::rotate( glm::dmat4( 1.0 ), glm::radians( m_RelativeRotation.z ), { 0., 0., 1. } );
   M4 r = rz * ry * rx;
   // M4 r = Quat< T >::fromEulerAngles( m_RelativeRotation ).toMat4();
   M4 s = glm::scale( M4(1), m_RelativeScale );
   M4 mat = t * r * s;
   // DE_DEBUG( getName(), ", r(",r,")")
   // DE_DEBUG( getName(), ", t(",t,")")
   // DE_DEBUG( getName(), ", s(",s,")")
   // DE_DEBUG( getName(), ", mat(",mat,")")
   return mat;
}

bool
ISceneNode::isVisible() const { return m_IsVisible; }

bool
ISceneNode::isTrulyVisible() const
{
   if ( !m_IsVisible ) return false;
   if ( !m_Parent ) return true;
   return m_Parent->isTrulyVisible();
}

void
ISceneNode::setVisible( bool isVisible ) { m_IsVisible = isVisible; }

int
ISceneNode::getId() const { return m_Id; }

void
ISceneNode::setId( int id ) { m_Id = id; }

ISceneNode::V3 const &
ISceneNode::getScale() const { return m_RelativeScale; }

ISceneNode::V3 const &
ISceneNode::getRotation() const { return m_RelativeRotation; }

ISceneNode::V3 const &
ISceneNode::getPosition() const { return m_RelativeTranslation; }

void
ISceneNode::setScale( V3 const & scale )
{
   m_RelativeScale = scale;
   m_IsDirty = true;
   updateAbsoluteTransform();
}

void
ISceneNode::setRotation( V3 const & rotation )
{
   m_RelativeRotation = rotation;
   m_IsDirty = true;
   updateAbsoluteTransform();
}

void
ISceneNode::setPosition( V3 const & pos )
{
   m_RelativeTranslation = pos;
   m_IsDirty = true;
   updateAbsoluteTransform();
}

void
ISceneNode::setScale( T x, T y, T z ) { setScale( V3(x,y,z) ); }

void
ISceneNode::setRotation( T x, T y, T z ) { setRotation( V3(x,y,z) ); }

void
ISceneNode::setPosition( T x, T y, T z ) { setPosition( V3(x,y,z) ); }

//   //! Removes this scene node from the scene
//   virtual void
//   removeFromParent()
//   {
//      if ( m_Parent )
//      {
//         m_Parent->removeChild( this );
//         m_Parent = nullptr;
//      }
//   }

void
ISceneNode::removeAll()
{
   for ( size_t i = 0; i < m_Children.size(); ++i )
   {
      ISceneNode* node = m_Children[ i ];
      if ( node )
      {
         node->setParent( nullptr );
         //delete node;
         //node->drop();
      }
   }
   m_Children.clear();
}

bool
ISceneNode::removeChild( ISceneNode* child )
{
   for ( size_t i = 0; i < m_Children.size(); ++i )
   {
      if ( m_Children[ i ] == child )
      {
         m_Children[ i ]->m_Parent = nullptr;
//            m_Children[ i ]->drop();
         m_Children.erase( m_Children.begin() + i );
         return true;
      }
   }
   return false;
}


ISceneNode*
ISceneNode::getParent() const { return m_Parent; }

void
ISceneNode::setParent( ISceneNode* newParent )
{
   //grab();
   //removeFromParent();
   if ( m_Parent )
   {
      m_Parent->removeChild( this );
   }

   m_Parent = newParent;

   if (m_Parent)
   {
      m_Parent->addChild(this);
   }
   //drop();
}

std::vector< ISceneNode* > const &
ISceneNode::getChildren() const { return m_Children; }

void
ISceneNode::addChild( ISceneNode* child )
{
   if ( !child || ( child == this ) ) return;
   //child->grab();
   child->setParent( nullptr ); // remove from old parent
   m_Children.push_back( child );
   child->setParent( this );
}



// ===========================================================================
SMeshSceneNode::SMeshSceneNode(  SMesh::SharedPtr const & mesh,
                                 ISceneManager* smgr,
                                 ISceneNode* parent,
                                 int id )
// ===========================================================================
   : ISceneNode( smgr, parent, id )
   , m_Mesh( mesh )
   , m_IsDirtyDebugMesh( true )
{
   if ( !m_Parent && m_SceneManager )
   {
      m_Parent = m_SceneManager->getRootSceneNode(); // TODO: change m_Driver to m_SceneManager
   }
}

SMeshSceneNode::SMeshSceneNode( ISceneManager* smgr, ISceneNode* parent, int id )
   : ISceneNode( smgr, parent, id )
   , m_Mesh( nullptr )
   , m_IsDirtyDebugMesh( true )
{
   if ( !m_Parent && m_SceneManager )
   {
      m_Parent = m_SceneManager->getRootSceneNode(); // TODO: change m_Driver to m_SceneManager
   }

   m_Mesh = SMesh::createShared("");
}

SMeshSceneNode::~SMeshSceneNode()
{

}


void
SMeshSceneNode::render()
{
   if ( !m_SceneManager ) { /* DE_ERROR("No scene manager") */ return; }
   if ( !m_Mesh ) { /* DE_WARN("No mesh") */ return; }
   if ( !m_IsVisible ) { /* DE_DEBUG("Not visible") */ return; }

   IVideoDriver* driver = m_SceneManager->getVideoDriver();
   if ( !driver ) { /* DE_ERROR("No driver") */ return; }

   updateDebugMesh();
   updateAbsoluteTransform();

   driver->setModelMatrix( m_AbsoluteTransform );
   driver->draw3D( *m_Mesh );

   // [Render] debug geometry
   if ( m_DebugData.flags > 0 && m_DebugMesh.getMeshBufferCount() > 0 )
   {
      for ( SMeshBuffer & p : m_DebugMesh.Buffers )
      {
         driver->draw3D( p );
      }
   }
}

SMesh::SharedPtr const &
SMeshSceneNode::getMesh() const { return m_Mesh; }

SMesh::SharedPtr
SMeshSceneNode::getMesh() { return m_Mesh; }

void
SMeshSceneNode::setMesh( SMesh::SharedPtr const & mesh )
{
   m_Mesh = mesh;
   m_DebugMesh.clear();
   m_IsDirtyDebugMesh = true;

   if ( m_Mesh && m_Name.empty() )
   {
      m_Name = m_Mesh->getName();
   }
   updateDebugMesh();
}

uint32_t
SMeshSceneNode::getBufferCount( int mode ) const
{
   size_t n = 0;
   if ( m_Mesh ) n += m_Mesh->getMeshBufferCount();
   if ( getDebugData().flags > 0 ) n += m_DebugMesh.getVertexCount();
   return n;
}

uint64_t
SMeshSceneNode::getVertexCount( int mode ) const
{
   uint64_t n = 0;
   if ( m_Mesh ) n += m_Mesh->getVertexCount();
   if ( getDebugData().flags > 0 ) n += m_DebugMesh.getVertexCount();
   return n;
}

uint64_t
SMeshSceneNode::getIndexCount( int mode ) const
{
   uint64_t n = 0;
   if ( m_Mesh ) n += m_Mesh->getIndexCount();
   if ( getDebugData().flags > 0 ) n += m_DebugMesh.getIndexCount();
   return n;
}

uint64_t
SMeshSceneNode::getByteCount( int mode ) const
{
   uint64_t n = sizeof( *this );
   if ( m_Mesh ) n += m_Mesh->getByteCount();
   if ( getDebugData().flags > 0 ) n += m_DebugMesh.getByteCount();
   return n;
}


void
SMeshSceneNode::setLighting( int illum ) { if ( m_Mesh ) m_Mesh->setLighting( illum ); }
void
SMeshSceneNode::setFogEnable( bool enable ) { if ( m_Mesh ) m_Mesh->setFogEnable( enable ); }
void
SMeshSceneNode::setWireframe( bool enable ) { if ( m_Mesh ) m_Mesh->setWireframe( enable ); }
void
SMeshSceneNode::setCulling( Culling cull ) { if ( m_Mesh ) m_Mesh->setCulling( cull ); }
void
SMeshSceneNode::setCulling( bool enable ) { if ( m_Mesh ) m_Mesh->setCulling( enable ); }
void
SMeshSceneNode::setDepth( Depth depth ) { if ( m_Mesh ) m_Mesh->setDepth( depth ); }
void
SMeshSceneNode::setTexture( int stage, TexRef const & tex ) { if ( m_Mesh ) m_Mesh->setTexture( stage, tex ); }

void
SMeshSceneNode::setDebugData( VisualDebugData const & debugFlags )
{
   if ( m_DebugData == debugFlags ) return;
   m_DebugData = debugFlags;
   m_IsDirtyDebugMesh = true;
   updateDebugMesh();
}

/*
void
SMeshSceneNode::createCollisionTriangles()
{
   updateAbsoluteTransform();

   m_CollisionTriangles.clear();
   if ( m_Mesh )
   {
      m_CollisionTriangles = m_Mesh->createCollisionTriangles();
   }

   glm::dmat4 const & modelMat = getAbsoluteTransform();
   for ( size_t i = 0; i < m_CollisionTriangles.size(); ++i )
   {
      m_CollisionTriangles[ i ].transform( modelMat );
   }
}
*/

void
SMeshSceneNode::updateDebugMesh()
{
   if ( !m_Mesh ) { return; } // nothing todo
   if ( !m_IsDirtyDebugMesh ) { return; } // nothing todo

   m_DebugMesh.clear();

   // [Debug] Tiny bboxes
   if ( m_DebugData.flags & VisualDebugData::BBOXES )
   {
      for ( SMeshBuffer const & p : m_Mesh->Buffers )
      {
         SMeshBuffer dst( PrimitiveType::Lines );
         dst.setLighting( 0 );
         dst.setBlend( Blend::alphaBlend() );
         SMeshBufferTool::addLineBox( dst, p.getBoundingBox().widen( 0.1f ), m_DebugData.bboxesColor );
         m_DebugMesh.addMeshBuffer( dst );
      }
   }

   // [Debug] Normals (red)
   if ( m_DebugData.flags & VisualDebugData::NORMALS )
   {
      for ( SMeshBuffer const & p : m_Mesh->Buffers )
      {
         SMeshBuffer dst( PrimitiveType::Lines );
         dst.setLighting( 0 );
         dst.setBlend( Blend::alphaBlend() );
         SMeshBufferTool::addLineNormals( dst, p, m_DebugData.normalLength, m_DebugData.normalColor );
         m_DebugMesh.addMeshBuffer( dst );
      }
   }

   // [Debug] Wireframe overlay
   if ( m_DebugData.flags & VisualDebugData::WIREFRAME )
   {
      for ( SMeshBuffer const & p : m_Mesh->Buffers )
      {
         SMeshBuffer dst( PrimitiveType::Lines );
         dst.setLighting( 0 );
         dst.setBlend( Blend::alphaBlend() );
         SMeshBufferTool::addWireframe( dst, p, m_DebugData.wireframeScale, m_DebugData.wireframeColor );
         m_DebugMesh.addMeshBuffer( dst );
      }
   }

   // [Debug] Mesh bbox
   if ( m_DebugData.flags & VisualDebugData::BBOX )
   {
      SMeshBuffer dst( PrimitiveType::Lines );
      dst.setLighting( 0 );
      dst.setBlend( Blend::alphaBlend() );
      SMeshBufferTool::addLineBox( dst, m_Mesh->getBoundingBox().widen( 0.03f ), m_DebugData.bboxColor );
      m_DebugMesh.addMeshBuffer( dst );
   }

   m_IsDirtyDebugMesh = false;
}

bool
SMeshSceneNode::intersectRayWithBoundingBox(
   Ray3< T > const & ray,
   glm::tvec3< T > * outPosition,
   uint32_t* outMeshBufferIndex ) const
{
   if ( !m_Mesh ) return false;
   auto trs = getAbsoluteTransform();
   bool doesIntersect = false;
   T const distMax = std::numeric_limits< T >::max();
   T hitDist = distMax;
   uint32_t hitMeshBufferIndex = 0;
   glm::tvec3< T > hitPos;
   //Triangle3< T > hitTriangle;

   auto const & boxf = m_Mesh->getBoundingBox();
   Box3< T > boxd( boxf.getMin(), boxf.getMax() );

   for ( size_t i = 0; i < 6; ++i )
   {
      for ( size_t j = 0; j < 2; ++j )
      {
         Triangle3< T > const tri = boxd.getTriangle( i, j ).transform( trs );
         glm::tvec3< T > tmpPos;
         if ( tri.intersectRay( ray.getPos(), ray.getDir(), tmpPos ) )
         {
            auto tmpDist = glm::length( ray.getPos() - tmpPos );
            //DE_DEBUG("Node[",i,"](", node->getName(),").Triangle[",t,"].Hit(", tmpPosition,"), "
            //   "d(", d, "), dist(", dist, "), tri(", tri.toString(), ")")
            if ( hitDist == distMax || hitDist >= tmpDist )
            {
               doesIntersect = true;
               hitDist = tmpDist;
               hitPos = tmpPos;
               hitMeshBufferIndex = uint32_t( 0 );
               //hitTriangleIndex = triangleIdx;
               //hitTriangle = triangle;
            }
         }
            //DE_DEBUG("Mesh[",i,"].Triangle[", t, "] = ", tri.toString() )
      }
   }

   if ( doesIntersect )
   {
      if ( outPosition ) { *outPosition = hitPos; }
      if ( outMeshBufferIndex ) { *outMeshBufferIndex = hitMeshBufferIndex; }
      //if ( outTriangleIndex ) { *outTriangleIndex = hitTriangleIndex; }
      //if ( outTriangle ) { *outTriangle = hitTriangle; }
   }
   return doesIntersect;
}


bool
SMeshSceneNode::intersectRay( Ray3< T > const & ray,
              glm::tvec3< T > * outPosition,
              uint32_t* outBufferIndex,
              uint32_t* outTriangleIndex,
              Triangle3< T >* outTriangle ) const
{
   if ( !m_Mesh ) return false;
   glm::tvec3< T > hitPos;
   uint32_t hitBuffer = 0;
   uint32_t hitTriangleIndex = 0;
   bool doesIntersect = false;
   T const distMax = std::numeric_limits< T >::max();
   T hitDist = distMax;
   Triangle3< T > hitTriangle;

   auto trs = getAbsoluteTransform();

   for ( size_t i = 0; i < m_Mesh->Buffers.size(); ++i )
   {
      SMeshBuffer const & buffer = m_Mesh->Buffers[ i ];
      buffer.forAllTriangles(
         [&] ( uint32_t triangleIdx, Triangle3f const & tri )
         {
            glm::tvec3< T > tmpPos;
            auto a = glm::tvec3< T >( trs * glm::tvec4< T >(tri.A,T(1)) );
            auto b = glm::tvec3< T >( trs * glm::tvec4< T >(tri.B,T(1)) );
            auto c = glm::tvec3< T >( trs * glm::tvec4< T >(tri.C,T(1)) );
            Triangle3< T > triangle( a, b, c );
            if ( triangle.intersectRay( ray.getPos(), ray.getDir(), tmpPos ) )
            {
               auto tmpDist = glm::length( ray.getPos() - tmpPos );
               //DE_DEBUG("Node[",i,"](", node->getName(),").Triangle[",t,"].Hit(", tmpPosition,"), "
               //   "d(", d, "), dist(", dist, "), tri(", tri.toString(), ")")
               if ( hitDist == distMax || hitDist >= tmpDist )
               {
                  doesIntersect = true;
                  hitBuffer = uint32_t( i );
                  hitDist = tmpDist;
                  hitPos = tmpPos;
                  hitTriangleIndex = triangleIdx;
                  hitTriangle = triangle;
               }
            }
            //DE_DEBUG("Mesh[",i,"].Triangle[", t, "] = ", tri.toString() )
         }
      );
   }

   if ( doesIntersect )
   {
      if ( outPosition ) { *outPosition = hitPos; }
      if ( outBufferIndex ) { *outBufferIndex = hitBuffer; }
      if ( outTriangleIndex ) { *outTriangleIndex = hitTriangleIndex; }
      if ( outTriangle ) { *outTriangle = hitTriangle; }
   }
   return doesIntersect;
}

} // end namespace gpu.
} // end namespace de.


