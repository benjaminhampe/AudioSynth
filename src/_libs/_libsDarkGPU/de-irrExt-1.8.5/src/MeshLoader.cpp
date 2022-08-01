#include <irrExt/MeshLoader.hpp>
#include <iostream>

uint32_t
MeshBuffer_getVertexCount( irr::scene::IMeshBuffer* meshBuffer )
{
   if ( !meshBuffer ) return 0;
   return meshBuffer->getVertexCount();
}

uint32_t
MeshBuffer_getIndexCount( irr::scene::IMeshBuffer* meshBuffer )
{
   if ( !meshBuffer ) return 0;
   return meshBuffer->getIndexCount();
}

glm::vec3
MeshBuffer_getSize( irr::scene::IMeshBuffer* meshBuffer )
{
   if ( !meshBuffer ) return glm::vec3(0,0,0);
   auto ext = meshBuffer->getBoundingBox().getExtent();
   return glm::vec3( ext.X, ext.Y, ext.Z );
}

uint32_t
Mesh_getVertexCount( irr::scene::IMesh* mesh )
{
   uint32_t vertexCount = 0;
   if ( !mesh ) return vertexCount;
   for ( uint32_t i = 0; i < mesh->getMeshBufferCount(); ++i )
   {
      vertexCount += MeshBuffer_getVertexCount( mesh->getMeshBuffer( i ) );
   }
   return vertexCount;
}

uint32_t
Mesh_getIndexCount( irr::scene::IMesh* mesh )
{
   uint32_t indexCount = 0;
   if ( !mesh ) return indexCount;
   for ( uint32_t i = 0; i < mesh->getMeshBufferCount(); ++i )
   {
      indexCount += MeshBuffer_getIndexCount( mesh->getMeshBuffer( i ) );
   }
   return indexCount;
}

glm::vec3
Mesh_getSize( irr::scene::IMesh* mesh )
{
   if ( !mesh ) return glm::vec3(0,0,0);
   auto ext = mesh->getBoundingBox().getExtent();
   return glm::vec3( ext.X, ext.Y, ext.Z );
}

glm::vec3
SceneNode_getSize( irr::scene::ISceneNode* node )
{
   if ( !node ) return glm::vec3(0,0,0);
   auto ext = node->getBoundingBox().getExtent();
   return glm::vec3( ext.X, ext.Y, ext.Z );
}

int32_t
SceneNode_getFreeId( irr::scene::ISceneManager* smgr )
{
   if ( !smgr ) return 0;
   int32_t id = 0;
   irr::core::list< irr::scene::ISceneNode* > const & children = smgr->getRootSceneNode()->getChildren();
   auto it = children.begin();
   for ( ; it != children.end(); it++ )
   {
      irr::scene::ISceneNode* node = *it;
      id = std::max( id, node->getID() );
   }
   id = std::max( id, 1 );
   return id;
}

irr::scene::IAnimatedMeshSceneNode*
loadObject( irr::IrrlichtDevice* device, std::string const & fileName )
{
   if ( !device )
   {
      std::cout << "[Error] " << __FUNCTION__ << " :: Invalid pointer to IrrlichtDevice\n";
      return nullptr;
   }

   irr::scene::ISceneManager* smgr = device->getSceneManager();
   if ( !smgr )
   {
      std::cout << "[Error] " << __FUNCTION__ << " :: Invalid pointer to ISceneManager\n";
      return nullptr;
   }

   uint32_t const texCount0 = smgr->getVideoDriver()->getTextureCount();
   irr::scene::IAnimatedMesh* mesh = smgr->getMesh( fileName.c_str() );
   if ( !mesh )
   {
      std::cout << "[Error] " << __FUNCTION__ << " :: Cannot read file(" << fileName << ")\n";
      return nullptr;
   }

   uint32_t texCount = smgr->getVideoDriver()->getTextureCount() - texCount0; // Delta loaded textures
   uint32_t vertexCount = Mesh_getVertexCount( mesh );
   uint32_t indexCount = Mesh_getIndexCount( mesh );
   glm::vec3 meshSize = Mesh_getSize( mesh );
   int32_t objId = SceneNode_getFreeId( smgr );

   std::cout << "[Ok] " << __FUNCTION__ << " :: Loaded mesh "
         << "id(" << objId << "), "
         << "file(" << fileName << "), "
         << "size(" << meshSize << "), "
         << "tex(" << texCount << "), "
         << "vertex(" << vertexCount << "), "
         << "index(" << indexCount << "), "
         << "\n";

   irr::scene::IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh, smgr->getRootSceneNode(), objId );
   mesh->drop();

   if ( !node )
   {
      std::cout << "[Error] " << __FUNCTION__ << " :: Cannot create scene node(" << fileName << ")\n";
      return nullptr;
   }

   return node;
}
