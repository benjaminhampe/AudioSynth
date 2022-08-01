#ifndef MESHLOADER_HPP
#define MESHLOADER_HPP

#include <irrExt/_common.hpp>

uint32_t MeshBuffer_getVertexCount( irr::scene::IMeshBuffer* meshBuffer );
uint32_t MeshBuffer_getIndexCount( irr::scene::IMeshBuffer* meshBuffer );
glm::vec3 MeshBuffer_getSize( irr::scene::IMeshBuffer* meshBuffer );
uint32_t Mesh_getVertexCount( irr::scene::IMesh* mesh );
uint32_t Mesh_getIndexCount( irr::scene::IMesh* mesh );
glm::vec3 Mesh_getSize( irr::scene::IMesh* mesh );
glm::vec3 SceneNode_getSize( irr::scene::ISceneNode* node );
int32_t SceneNode_getFreeId( irr::scene::ISceneManager* smgr );
irr::scene::IAnimatedMeshSceneNode*
loadObject( irr::IrrlichtDevice* device, std::string const & fileName );

#endif // MESHLOADER_HPP
