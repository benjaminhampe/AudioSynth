#include <de/gpu/scene/SceneManager.hpp>
#include <de/gpu/IVideoDriver.hpp>

namespace de {
namespace gpu {

ISceneManager::ISceneManager( IVideoDriver* driver )
   : m_Driver( driver )
   , m_RootNode( nullptr )
   , m_Hovered( nullptr )
   , m_Picked( nullptr )
//   , m_LastHoveredNode( nullptr )
//   , m_SelectedNode( nullptr )
//   , m_LastSelectedNode( nullptr )
{
   //DE_DEBUG("")
}

ISceneManager::~ISceneManager()
{
   clear();
}

void
ISceneManager::init( int w, int h )
{
   DE_DEBUG("w(",w,"), h(",h,")")
}

void
ISceneManager::clear()
{
   //DE_DEBUG("")

   m_RootNode = nullptr;
   m_Hovered = nullptr;
   m_Picked = nullptr;

   for ( size_t i = 0; i < m_TopLevel.size(); ++i )
   {
      ISceneNode* trash = m_TopLevel[ i ];
      if ( trash )
      {
         auto it = std::find_if( m_TrashBin.begin(), m_TrashBin.end(),
            [&] ( ISceneNode const * const cached ) { return cached == trash; } );
         if ( it == m_TrashBin.end() )
         {
            m_TrashBin.emplace_back( trash );
         }
      }
   }

   m_TopLevel.clear();

   //clearCameras();

   if ( m_TrashBin.empty() ) { return; }

   //DE_DEBUG("Trash ", m_TrashBin.size(), " SceneNodes")

   for ( size_t i = 0; i < m_TrashBin.size(); ++i )
   {
      SAFE_DELETE( m_TrashBin[ i ] );
   }
   m_TrashBin.clear();

   // CLEAR MESHCACHE
   m_MeshLut.clear();
}

void
ISceneManager::render()
{
   if ( !m_Driver ) { DE_ERROR("No driver") return; }

   for ( size_t i = 0; i < m_TopLevel.size(); ++i )
   {
      ISceneNode* node = m_TopLevel[ i ];
      if ( !node ) continue;
      if ( !node->isVisible() ) continue;
      node->render();
   }
}


void
ISceneManager::onFrameBegin()
{
   if ( !m_Driver ) { DE_ERROR("No driver") return; }

   auto pts = m_Driver->getTimer();

   for ( size_t i = 0; i < m_TopLevel.size(); ++i )
   {
      ISceneNode* node = m_TopLevel[ i ];
      if ( node )
      {
         node->onAnimate( pts );
      }
   }

   auto camera = m_Driver->getActiveCamera();
   if ( !camera ) { DEM_ERROR("No camera") return; }

   int mx = m_Driver->getMouseX();
   int my = m_Driver->getMouseY();
   de::gpu::Ray3d ray = camera->computeRay( mx, my );

   ScenePickResult< double > pickResult;
   auto picked = pickSceneNode( ray, &pickResult );
   if ( m_Picked != picked )
   {
      // emit changedPickResult()
      //DE_DEBUG("Changed pick from(",(void*)m_Picked,") to(",(void*)picked,")")
   }
   m_Picked = picked;
   m_Hovered = picked;
}

void
ISceneManager::onFrameEnd()
{

}

ISceneNode*
ISceneManager::pickSceneNode( Ray3< T > const & ray, ScenePickResult< T >* pickResult )
{
   ISceneNode* pickNode = nullptr;
   if ( m_TopLevel.size() < 1 ) return pickNode;
   glm::tvec3< T > pickPos;
   uint32_t pickTriangleIndex = 0;
   uint32_t pickNodeIndex = 0;
   T const distMax = std::numeric_limits< T >::max();
   T pickDist = distMax;
   bool doesIntersect = false;

   for ( size_t i = 0; i < m_TopLevel.size(); ++i )
   {
      ISceneNode* node = m_TopLevel[ i ];
      if ( !node ) { continue; }
      if ( !node->isCollisionEnabled() ) { continue; }

      glm::tvec3< T > tmpPos;
      uint32_t tmpBufferIndex;
      uint32_t tmpTriangleIndex;

      if ( node->intersectRayWithBoundingBox( ray, &tmpPos, &tmpBufferIndex ) )
      {
         T d = glm::length( ray.getPos() - tmpPos );

         //DE_DEBUG("Node[",i,"](", node->getName(),").Triangle[",t,"].Hit(", tmpPosition,"), "
         //   "d(", d, "), dist(", dist, "), tri(", tri.toString(), ")")

         if ( pickDist == distMax || pickDist >= d )
         {
            pickDist = d;
            pickNode = node;
            pickNodeIndex = i;
            pickPos = tmpPos;
            pickTriangleIndex = tmpTriangleIndex;
         }

         doesIntersect = true;
      }
/*
      if ( node->intersectRay( ray, &tmpPos, &tmpBufferIndex, &tmpTriangleIndex ) )
      {
         auto d = glm::length( ray.getPos() - tmpPos );

         //DE_DEBUG("Node[",i,"](", node->getName(),").Triangle[",t,"].Hit(", tmpPosition,"), "
         //   "d(", d, "), dist(", dist, "), tri(", tri.toString(), ")")

         if ( pickDist == distMax || pickDist >= d )
         {
            pickDist = d;
            pickNode = node;
            pickNodeIndex = i;
            pickPos = tmpPos;
            pickTriangleIndex = tmpTriangleIndex;
         }

         doesIntersect = true;
      }



      if ( node->intersectRay( rayStart, rayDir, &tmpPos, &tmpBufferIndex, &tmpTriangleIndex ) )
      {
         auto d = glm::length( rayStart - tmpPos );

         //DE_DEBUG("Node[",i,"](", node->getName(),").Triangle[",t,"].Hit(", tmpPosition,"), "
         //   "d(", d, "), dist(", dist, "), tri(", tri.toString(), ")")

         if ( pickDist == distMax || pickDist >= d )
         {
            pickDist = d;
            pickNode = node;
            pickNodeIndex = i;
            pickPos = tmpPos;
            pickTriangleIndex = tmpTriangleIndex;
         }

         doesIntersect = true;
      }
*/
   }

   if ( pickResult )
   {
      pickResult->doesIntersect = doesIntersect;
      pickResult->pos = pickPos;
      pickResult->dist = pickDist;
      pickResult->node = pickNode;
      pickResult->nodeIndex = pickNodeIndex;
      pickResult->triangleIndex = pickTriangleIndex;
   }

   if ( doesIntersect )
   {
      //DE_DEBUG("pickNode", pickNode ,".Triangle[", pickTriangleIndex, "] "
      //         "pickPos(", pickPos, "), name(", pickNode->getName(), ")" )
   }

   return pickNode;

}

/*
ISceneNode*
ISceneManager::pickSceneNode( glm::dvec3 const & rayStart, glm::vec3 const & rayDir,
   ScenePickResult* pickResult )
{
   ISceneNode* pickNode = nullptr;

   if ( m_TopLevel.size() < 1 ) return pickNode;

   glm::vec3 pickPos;
   uint32_t pickTriangleIndex = 0;
   uint32_t pickNodeIndex = 0;
   float const distMax = std::numeric_limits< float >::max();
   float pickDist = distMax;

   bool doesIntersect = false;

   for ( size_t i = 0; i < m_TopLevel.size(); ++i )
   {
      ISceneNode* node = m_TopLevel[ i ];
      if ( !node ) { continue; }
      if ( !node->isCollisionEnabled() ) { continue; }

      glm::vec3 tmpPos;
      uint32_t tmpBufferIndex;
      uint32_t tmpTriangleIndex;
      if ( node->intersectRay( rayStart, rayDir, &tmpPos, &tmpBufferIndex, &tmpTriangleIndex ) )
      {
         float d = glm::length( rayStart - tmpPos );

         //DE_DEBUG("Node[",i,"](", node->getName(),").Triangle[",t,"].Hit(", tmpPosition,"), "
         //   "d(", d, "), dist(", dist, "), tri(", tri.toString(), ")")

         if ( pickDist == distMax || pickDist >= d )
         {
            pickDist = d;
            pickNode = node;
            pickNodeIndex = i;
            pickPos = tmpPos;
            pickTriangleIndex = tmpTriangleIndex;
         }

         doesIntersect = true;
      }
   }

   if ( pickResult )
   {
      pickResult->doesIntersect = doesIntersect;
      pickResult->pos = pickPos;
      pickResult->dist = pickDist;
      pickResult->node = pickNode;
      pickResult->nodeIndex = pickNodeIndex;
      pickResult->triangleIndex = pickTriangleIndex;
   }

   if ( doesIntersect )
   {
      //DE_DEBUG("Pick.NodeIndex = ", iNode ,"].Triangle[", iTriangle, "] "
        //       "hitPos(", hitPosition, "), name(", pick->getName(), "), "
          //     "hitTriangle(", hitTriangle.toString(), ")" )
   }

   return pickNode;

}


std::vector< Triangle3f >
ISceneManager::getCollisionTriangles() const
{
   std::vector< Triangle3f > container;

   for ( size_t i = 0; i < m_TopLevel.size(); ++i )
   {
      ISceneNode* node = m_TopLevel[ i ];
      if ( !node ) continue;
      if ( node->getNodeType() != NodeType::Mesh ) continue;

      // DE_DEBUG("Test MeshSceneNode")

      SMeshSceneNode* snode = dynamic_cast< SMeshSceneNode* >( node );
      if ( !snode )
      {
         DE_ERROR("Cast failed") continue;
      }

      std::vector< Triangle3f > const & colltris = snode->getCollisionTriangles();

      container.insert( container.end(), colltris.begin(), colltris.end() );
   }

   return container;
}
*/

// ###########################
// ###                     ###
// ###     MeshManager     ###
// ###                     ###
// ###########################
/*
void
ISceneManager::clearMeshes()
{
   for ( size_t i = 0; i < m_Meshes.size(); ++i )
   {
      if ( m_Meshes[ i ] )
      {
         delete m_Meshes[ i ];
      }
   }
   m_Meshes.clear();
}

int32_t
ISceneManager::findMesh( std::string const & name ) const
{
   auto it = std::find_if( m_Meshes.begin(), m_Meshes.end(), [&]
      ( SMesh const * const m ) { return m && m->getName() == name; } );
   if ( it == m_Meshes.end() ) { return -1; }
   return std::distance( m_Meshes.begin(), it );
}

SMesh*
ISceneManager::getMesh( std::string const & name )
{
   auto it = std::find_if( m_Meshes.begin(), m_Meshes.end(), [&]
      ( SMesh const * const m ) { return m && m->getName() == name; } );
   if ( it == m_Meshes.end() ) { return nullptr; }
   return *it;
}

SMesh*
ISceneManager::addMesh( std::string const & name, std::string const & uri )
{
   SMesh* mesh = new SMesh( name );
   if ( SMeshIO::load( *mesh, uri, this ) )
   {
      m_Meshes.push_back( mesh );
      return m_Meshes.back();
   }

   delete mesh;
   return nullptr;
}
*/

// ###########################
// ###                     ###
// ###     MeshManager     ###
// ###                     ###
// ###########################

bool
ISceneManager::hasMesh( std::string const & name ) const
{
   auto it = m_MeshLut.find( name );
   if (it == m_MeshLut.end())
   {
      return false;
   }
   return true;
}

void
ISceneManager::removeMesh( std::string const & name )
{
   auto it = m_MeshLut.find( name );
   if (it != m_MeshLut.end())
   {
      DE_ERROR("Removing mesh ", name)
      m_MeshLut.erase( it );
   }
}

SMesh::SharedPtr
ISceneManager::addEmptyMesh( std::string name )
{
   if ( name.empty() )
   {
      DE_ERROR("No name")
      return nullptr;
   }

   auto it = m_MeshLut.find( name );
   if (it != m_MeshLut.end())
   {
      DE_ERROR("Mesh already exist, ",name)
      return nullptr;
   }

   DE_DEBUG("Adding Empty mesh ",name)
   auto mesh = SMesh::createShared( name );
   m_MeshLut[ name ] = mesh;
   //m_Meshes.emplace_back( mesh );
   return mesh;
}

bool
ISceneManager::addMesh( SMesh::SharedPtr mesh )
{
   if ( !mesh )
   {
      DE_ERROR("No mesh")
      return false;
   }

//   if ( mesh->Name.empty() )
//   {
//      DE_WARN("No name")
//      return false;
//   }

//   auto it = m_MeshLut.find( mesh->Name );
//   if (it != m_MeshLut.end())
//   {
//      DE_ERROR("Mesh already exist, ",mesh->Name)
//      return false;
//   }

//   auto it = std::find_if( m_Meshes.begin(), m_Meshes.end(),
//      [&] ( SMesh const * const cached ) { return cached == mesh; } );
//   if ( it != m_Meshes.end() )
//   {
//      DE_ERROR("Mesh ",mesh->Name, " already added.")
//      return false;
//   }

//   DE_DEBUG("+ Added mesh ",mesh->Name)
////   m_MeshLut[ mesh->Name ] = mesh;
//   m_Meshes.emplace_back( mesh );
   return true;
}

uint32_t
ISceneManager::getMeshCount() const { return m_MeshLut.size(); }

SMesh::SharedPtr
ISceneManager::getMesh( std::string const & name )
{
   if ( name.empty() )
   {
      DE_ERROR("Empty name")
      return nullptr;
   }

   auto it = m_MeshLut.find( name );
   if (it != m_MeshLut.end())
   {
      return it->second; // Cache hit
   }

   // Heavy load work
   if ( !dbExistFile( name ) )
   {
      DE_ERROR("No 3d model file exist with name ", name)
      return nullptr;
   }

   // Heavy load work
   auto mesh = SMesh::createShared( name );

   // Heavy load work
   if ( !SMeshIO::load( *mesh, name, m_Driver ) )
   {
      DE_ERROR("Cant decode 3d model file ", name, ", empty mesh.")
      return nullptr;
   }

   // Heavy load work
   m_MeshLut[ name ] = mesh;
   DE_DEBUG("Loaded 3d model file ", name)
   return mesh;
}


//SMesh*
//ISceneManager::loadMesh( std::string const & uri )
//{
//   if ( uri.empty() )
//   {
//      throw std::runtime_error( "No mesh can have an empty uri.");
//   }

//   auto it = m_MeshLut.find( uri );
//   if ( it != m_MeshLut.end() )
//   {
//      throw std::runtime_error( "Mesh name(" + uri + ") already exist" );
//   }

//   SMesh* mesh = new SMesh( uri );
//   if ( !SMeshIO::load( *mesh, uri, this ) )
//   {
//      // throw std::runtime_error( "No mesh created.");
//   }

//   m_MeshLut[ uri ] = mesh;
//   return mesh;
//}



//   void removeMesh( SMesh* mesh );
//   void addMesh( SMesh* buffer )
//   {

//   }


//   int32_t findMeshByIndex( std::string const & name ) const;
//   SMesh* getMesh( std::string const & name );
//   SMesh* addMesh( std::string const & name, std::string const & uri );
//   void addMesh( std::string const & name, SMesh* mesh );



void
ISceneManager::postEvent( SEvent event )
{
   for ( size_t i = 0; i < m_TopLevel.size(); ++i )
   {
      ISceneNode* node = m_TopLevel[ i ];
      if ( node )
      {
         node->onEvent( event );
      }
   }
}


//void
//ISceneManager::clearDebugData()
//{
//   for ( size_t i = 0; i < m_TopLevel.size(); ++i )
//   {
//      ISceneNode * node = m_TopLevel[ i ];
//      if ( node )
//      {
//         node->setDebugData( VisualDebugData::NONE );
//      }
//   }
//}

void
ISceneManager::setDebugData( VisualDebugData const & debugData )
{
   for ( size_t i = 0; i < m_TopLevel.size(); ++i )
   {
      ISceneNode * node = m_TopLevel[ i ];
      if ( node )
      {
         node->setDebugData( debugData );
      }
   }
}

void
ISceneManager::removeSceneNode( ISceneNode* node )
{
   if ( !node ) { DE_ERROR("No node") return; }

   // Search pointer in cache
   auto it = std::find_if( m_TopLevel.begin(), m_TopLevel.end(),
      [&] ( ISceneNode* cached ) { return cached == node; } );

   // Add only if pointer is unique
   if ( it != m_TopLevel.end() )
   {
      ISceneNode* ptr = *it;
      if ( ptr )
      {
         DE_DEBUG("Mark SceneNode as obsolete(", ptr, ")")
         ptr->setVisible( false );  // Dont render anymore
         ptr->setParent( nullptr ); // Detach from tree
         m_TrashBin.emplace_back( ptr );
      }
      m_TopLevel.erase( it );
   }
}

ISceneNode*
ISceneManager::getSceneNode( std::string const & name )
{
   if ( name.empty() ) { DE_ERROR("No name") return nullptr; }

   for ( ISceneNode* node : m_TopLevel )
   {
      if ( node && node->getName() == name )
      {
         return node;
      }
   }

   return nullptr;
}

void
ISceneManager::addSceneNode( ISceneNode* node )
{
   if ( !node ) { DE_ERROR("No node") return; }

   // Search pointer in cache
   auto it = std::find_if( m_TopLevel.begin(), m_TopLevel.end(),
      [&] ( ISceneNode* cached ) { return cached == node; } );

   // Add only if pointer is unique
   if ( it == m_TopLevel.end() )
   {
      DE_ERROR("Add SceneNode ", m_TopLevel.size())
      m_TopLevel.emplace_back( node );
   }
   else
   {
      DE_WARN("SceneNode already added(", node, ")")
   }
}

SMeshSceneNode*
ISceneManager::addMeshSceneNode( SMesh::SharedPtr const & mesh, ISceneNode* parent, int id )
{
   if ( !mesh ) { DE_ERROR("No mesh") return nullptr; }
   if ( mesh->Name.empty() ) { DE_WARN("No mesh name") }
   if ( !parent ) { parent = getRootSceneNode(); }
   SMeshSceneNode* node = new SMeshSceneNode( mesh, this, parent, id );
   if ( !node ) { DE_ERROR("No node") return nullptr; }
   m_TopLevel.push_back( node );
   return node;
}

//SMeshSceneNode*
//ISceneManager::addEmptyMeshSceneNode( ISceneNode* parent, int id )
//{
//   if ( !parent ) { parent = getRootSceneNode(); }
//   SMeshSceneNode* node = new SMeshSceneNode( this, parent, id );
//   if ( !node ) { DE_ERROR("No node") return nullptr; }
//   m_TopLevel.push_back( node );
//   return node;
//}

//SMeshSceneNode*
//ISceneManager::loadMeshSceneNode(
//   std::string const & uri, ISceneNode* parent, int id )
//{
//   if ( uri.empty() ) { return nullptr; }

//   auto mesh = new de::gpu::SMesh( uri );
//   if ( !de::gpu::SMeshIO::load( *mesh, uri, m_Driver ) )
//   {
//      DEM_ERROR("Cant load mesh uri ", uri)
//      delete mesh;
//      return nullptr;
//   }

//   //static int createCounter = 0;
//   //createCounter++;
//   //int id = createCounter;

//   return addMeshSceneNode( mesh, parent, id );
//}

//SMeshSceneNode*
//ISceneManager::addNode( SMeshBuffer const & buf, ISceneNode* parent, int id )
//{
//   if ( !parent ) { parent = getRootSceneNode(); }

//   SMeshSceneNode* node = new SMeshSceneNode( this, parent, id );
//   if ( !node ) { DE_ERROR("No node") return nullptr; }
//   if ( !node->getMesh() )  { DE_ERROR("No node mesh") return nullptr; }

//   node->getMesh()->addMeshBuffer( buf );

//   m_TopLevel.push_back( node );
//   return node;
//}

uint32_t
ISceneManager::getBufferCount( int mode ) const
{
   size_t n = 0;
   for ( ISceneNode const * const p : m_TopLevel )
   {
      if ( p ) n += p->getBufferCount( mode );
   }
   return n;
}


uint64_t
ISceneManager::getVertexCount( int mode ) const
{
   uint64_t n = 0;
   for ( ISceneNode const * const p : m_TopLevel )
   {
      if ( p ) n += p->getVertexCount( mode );
   }
   return n;
}

uint64_t
ISceneManager::getIndexCount( int mode ) const
{
   uint64_t n = 0;
   for ( ISceneNode const * const p : m_TopLevel )
   {
      if ( p ) n += p->getIndexCount( mode );
   }
   return n;
}

uint64_t
ISceneManager::getByteCount( int mode ) const
{
   uint64_t n = sizeof( *this );
   for ( ISceneNode const * const p : m_TopLevel )
   {
      if ( p ) n += p->getByteCount( mode );
   }
   return n;
}

void
ISceneManager::resize( int w, int h )
{
//   for ( ICamera* camera : m_Cameras )
//   {
//      if ( camera )
//      {
//         camera->setScreenSize( w, h );
//      }
//   }
}



/*

SMeshSceneNode*
ISceneManager::add3DRectZ( float w, float h, ITexture* tex, uint32_t color )
{
   SMeshSceneNode* node = new SMeshSceneNode( this, getRootSceneNode(), -1 );

   SMeshBuffer* quad = new SMeshBuffer( PrimitiveType::Triangles );
   quad->setTexture( 0, tex );
   quad->setLighting( false );
   quad->setFogEnable( false );
   mesh::Quad::addXY( *quad, w, h, color );

   node->getMesh()->addMeshBuffer( quad );

   addSceneNode( node );

   node->setDebugData( VisualDebugData::ALL );

   return node;
}
// =====================
// ### CameraManager ###
// =====================
// void clearCameras();
// int32_t findCamera( ICamera* camera ) const;
// int32_t findCamera( std::string const & name ) const;
// ICamera* getCamera( std::string const & name ) const;
// ICamera* addCamera( std::string const & name, bool makeActive = true );
// int32_t addCamera( ICamera* camera );

ICamera*
ISceneManager::addCamera( std::string const & name, bool makeActive )
{
   ICamera* camera = new ICamera();
   camera->setName( name );
   addCamera( camera, makeActive );
   return camera;
}

void
ISceneManager::addCamera( ICamera* camera, bool makeActive )
{
   if ( !camera ) return;
   int32_t found = findCamera( camera );
   if ( found > -1 ) { return; } // Already cached

   m_Cameras.emplace_back( camera );
   if ( makeActive )
   {
      setActiveCamera( camera );
   }
}

void
ISceneManager::clearCameras()
{
   m_Camera = nullptr;
   for ( ICamera* camera : m_Cameras )
   {
      if ( camera ) delete camera;
   }
   m_Cameras.clear();
}

int32_t
ISceneManager::getCameraCount() const
{
   return m_Cameras.size();
}

int32_t
ISceneManager::findCamera( ICamera* camera ) const
{
   if ( !camera ) return -1;
   for ( size_t i = 0; i < m_Cameras.size(); ++i )
   {
      ICamera const * const cached = m_Cameras[ i ];
      if ( cached == camera )
      {
         return int32_t( i );
      }
   }
   return -1;
}

int32_t
ISceneManager::findCamera( std::string const & name ) const
{
   if ( name.empty() ) return -1;
   for ( size_t i = 0; i < m_Cameras.size(); ++i )
   {
      ICamera const * const cached = m_Cameras[ i ];
      if ( cached && cached->getName() == name )
      {
         return int32_t( i );
      }
   }
   return -1;
}

ICamera*
ISceneManager::getCamera( int32_t index ) const
{
   return m_Cameras[ index ];
}

ICamera*
ISceneManager::getCamera( std::string const & name ) const
{
   int32_t found = findCamera( name );
   if ( found < 0 ) return nullptr;
   return m_Cameras[ found ];
}



// ICamera* getActiveCamera();
// ICamera const* getActiveCamera() const;
// bool setActiveCamera( ICamera* camera );
ICamera const*
ISceneManager::getActiveCamera() const { return m_Camera; }

ICamera*
ISceneManager::getActiveCamera() { return m_Camera; }

bool
ISceneManager::setActiveCamera( ICamera* camera )
{
   m_Camera = camera;
   if ( m_Camera )
   {
      m_Camera->setScreenSize( m_Driver->getScreenWidth(),
                               m_Driver->getScreenHeight() );
      m_Camera->update();
   }
   return true;
}


SkyboxSceneNode*
ISceneManager::addSkyboxSceneNode( ISceneNode* parent, int id )
{
   SkyboxSceneNode* node = new SkyboxSceneNode( this, parent, id );
   this->addSceneNode( node );
   return node;
}

CubeSceneNode*
ISceneManager::addCubeMeshSceneNode( ISceneNode* parent, int id )
{
   CubeSceneNode* node = new CubeSceneNode( this, parent, id );
   this->addSceneNode( node );
   return node;
}

PlaneSceneNode*
ISceneManager::addPlaneMeshSceneNode( ISceneNode* parent, int id )
{
   PlaneSceneNode* node = new PlaneSceneNode( this, parent, id );
   this->addSceneNode( node );
   return node;
}

TerrainSceneNode*
ISceneManager::addTerrainMeshSceneNode( ISceneNode* parent, int id )
{
   TerrainSceneNode* node = new TerrainSceneNode( this, parent, id );
   this->addSceneNode( node );
   return node;
}



*/




} // end namespace gpu.
} // end namespace de.


#if 0
ISceneNode*
ISceneManager::pickSceneNode( int mx, int my )
{
   glm::vec3 rayStart;
   glm::vec3 rayEnd;

   if ( getCamera() )
   {
      getCamera()->update();
      int w = getCamera()->getWidth();
      int h = getCamera()->getHeight();
      //auto cameraPos = getCamera()->getPos();
      glm::dmat4 viewProj = getCamera()->getViewProjectionMatrix();
      glm::dmat4 viewProjInv = glm::inverse( viewProj );

      // DE_DEBUG("Camera :: ",getCamera()->toString())

      auto ndc_x = 2.0 * ( double( mx ) / double( w ) ) - 1.0;
      auto ndc_y = 2.0 * ( double( my ) / double( h ) ) - 1.0;
      auto drayStart = glm::dvec4( ndc_x, ndc_y, -1.0, 1.0 );
      auto drayEnd = glm::dvec4( ndc_x, ndc_y, 1.0, 1.0 );

//      GLM_FUNC_QUALIFIER vec<3, T, Q>
//      unProject(  vec<3, T, Q> const& win,
//                  mat<4, 4, T, Q> const& model,
//                  mat<4, 4, T, Q> const& proj,
//                  vec<4, U, Q> const& viewport );

      // glm::unProject( glm::dvec3( ) )

      // CameraMouse Ray
      rayStart = glm::dvec3( viewProjInv * drayStart );
      rayEnd = glm::dvec3( viewProjInv * drayEnd );
   }

   return pickSceneNode( rayStart, rayEnd - rayStart );
}


   if ( getCamera() )
   {
      // DE_DEBUG("Camera :: ",getCamera()->toString())

      int w = getCamera()->getWidth();
      int h = getCamera()->getHeight();
      auto cameraPos = getCamera()->getPos();

      auto ndc_x = 2.0 * ( double( mx ) / double( w ) ) - 1.0;
      auto ndc_y = 2.0 * ( double( my ) / double( h ) ) - 1.0;
      auto rayStart = glm::dvec4( ndc_x, ndc_y, -1.0, 1.0 );
      auto rayEnd = glm::dvec4( ndc_x, ndc_y, 1.0, 1.0 );

//      GLM_FUNC_QUALIFIER vec<3, T, Q>
//      unProject(  vec<3, T, Q> const& win,
//                  mat<4, 4, T, Q> const& model,
//                  mat<4, 4, T, Q> const& proj,
//                  vec<4, U, Q> const& viewport );

      // glm::unProject( glm::dvec3( ) )

      // CameraMouse Ray
      glm::dmat4 viewProj = getCamera()->getViewProjectionMatrix();
      glm::dmat4 viewProjInv = glm::inverse( viewProj );
      auto worldStart = glm::dvec3( viewProjInv * rayStart );
      auto worldEnd = glm::dvec3( viewProjInv * rayEnd );

      // m_HoveredNode = nullptr;

      for ( size_t i = 0; i < m_TopLevel.size(); ++i )
      {
         ISceneNode* node = m_TopLevel[ i ];
         if ( !node ) continue;

         auto bbox = node->getTransformedBoundingBox();
         foundIntersection = bbox.intersectRay( glm::dvec3( worldStart ), glm::dvec3( worldEnd ), hitPosition );
         if ( foundIntersection )
         {
            float32_t distance = bbox.minDistanceTo( cameraPos );
            if ( !m_HoveredNode )
            {
               m_HoveredNode = node;
               distanceMin = distance;
               DE_DEBUG( "First intersection at distance(", distance,")")
            }
            else if ( m_HoveredNode && distance < distanceMin )
            {
               DE_DEBUG( "Next smaller distance(", distance, ") to previous(",distanceMin,")")
               m_HoveredNode = node;
            }
         }
      }
   }


   if ( m_HoveredNode )
   {
//      if ( m_LastHoveredNode != m_HoveredNode )
//      {
//         // Restore state
//         if ( m_LastHoveredNode ) m_LastHoveredNode->setDebugFlags( ISceneNode::EDF_DISABLED );
//      }

      m_LastHoveredNode = m_HoveredNode;
//      m_HoveredNode->setDebugFlags( ISceneNode::EDF_BBOX );

      DE_DEBUG("m_HoveredNode = ", m_HoveredNode)
   }

   if ( m_SelectedNode )
   {
//      if ( m_LastSelectedNode != m_SelectedNode )
//      {
//         if ( m_LastSelectedNode ) m_LastSelectedNode->setDebugFlags( ISceneNode::EDF_DISABLED );
//      }

      m_LastSelectedNode = m_SelectedNode;
//       m_SelectedNode->setDebugFlags( ISceneNode::EDF_ALL );

      DE_DEBUG("m_SelectedNode = ", m_SelectedNode)
   }

   // DE_DEBUG("SceneNodeCount = ",m_TopLevel.size())



   if ( !m_Driver )
   {
      DE_ERROR("No driver")
      return nullptr;
   }

   Camera * cam = this->getCamera();
   if ( !cam )
   {
      DE_ERROR("No camera")
      return nullptr;
   }

   auto w = m_Driver->getScreenWidth();
   auto h = m_Driver->getScreenHeight();
   if ( w < 1 || h < 1 )
   {
      DE_ERROR("Invalid screensize")
      return nullptr;
   }

   auto mx = m_Driver->getMouseX();
   auto my = m_Driver->getMouseY();
   auto fx = ( 2.0 * double( mx ) / double( w ) ) - 1.0;
   auto fy = ( 2.0 * double( my ) / double( h ) ) - 1.0;

   // CameraMouse Ray
   //glm::dmat4 viewProj = m_ViewProjectionMatrix;
   glm::dmat4 viewInv = glm::inverse( cam->getViewMatrix() );
   auto rayStart = glm::dvec3( viewInv * glm::dvec4( fx, fy, -1.0, 1.0 ) );
   auto rayEnd = glm::dvec3( viewInv * glm::dvec4( fx, fy, 1.0, 1.0 ) );
   //auto cameraPos = getCamera()->getPos();

   // DE_DEBUG("cameraPos = ", getCamera()->getPos() )
   // DE_DEBUG("viewInv = ", viewInv )
   // DE_DEBUG("m_TopLevel.size() = ", m_TopLevel.size() )

   size_t hits = 0;
   float32_t minDistance = std::numeric_limits< float32_t >::max();
   ISceneNode* hovered = nullptr;

   for ( size_t i = 0; i < m_TopLevel.size(); ++i )
   {
      ISceneNode* node = m_TopLevel[ i ];
      if ( !node ) continue;

      auto bbox = node->getTransformedBoundingBox();

      glm::dvec3 hitPosition;
      bool foundHit = bbox.intersectRay( cam->getPos(),
                                         cam->getDir(),
                                         hitPosition );
      if ( foundHit )
      {
         // DE_DEBUG( "Hit[", hits,"] at distance(", dist,")")
         float32_t dist = bbox.minDistanceTo( cam->getPos() );
         if ( minDistance > dist )
         {
            hovered = node;
            minDistance = dist;
         }
         hits++;
      }
   }

   return hovered;

#endif
