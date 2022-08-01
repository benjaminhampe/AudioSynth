#ifndef DE_GPU_SCENEMANAGER_HPP
#define DE_GPU_SCENEMANAGER_HPP

#include <de/gpu/scene/ICamera.hpp>
#include <de/gpu/scene/SceneNode.hpp>

namespace de {
namespace gpu {

template < typename T >
// =======================================================================
struct ScenePickResult
// =======================================================================
{
   ISceneNode* node = nullptr;
   glm::tvec3< T > pos;
   uint32_t triangleIndex = 0;
   uint32_t nodeIndex = 0;
   T const distMax = std::numeric_limits< T >::max();
   T dist = distMax;
   bool doesIntersect = false;
};


// =======================================================================
struct ISceneManager // : public ISceneManager
// =======================================================================
{
   ISceneManager( IVideoDriver* driver );
   ~ISceneManager();
   void init( int w, int h );
   void clear();
   void resize( int w, int h );
   void render();
   void onFrameBegin();
   void onFrameEnd();
   void postEvent( SEvent event );

   IVideoDriver*
   getVideoDriver() { return m_Driver; }

   ISceneNode*
   getRootSceneNode() { return m_RootNode; }

   //virtual void
   //clearDebugData();

   virtual void
   setDebugData( VisualDebugData const & debugData );

   virtual std::vector< ISceneNode* > const &
   getSceneNodes() const { return m_TopLevel; }

   virtual std::vector< ISceneNode* > &
   getSceneNodes() { return m_TopLevel; }

   virtual ISceneNode*
   getSceneNode( std::string const & name );

   virtual void
   addSceneNode( ISceneNode* node );

   virtual SMeshSceneNode*
   addMeshSceneNode( SMesh::SharedPtr const & mesh, ISceneNode* parent = nullptr, int id = -1 );

   virtual void
   removeSceneNode( ISceneNode* node );

   // ================
   // Collision control
   // ================

   typedef double T;

   virtual ISceneNode*
   pickSceneNode( Ray3< T > const & ray, ScenePickResult< T >* pickResult = nullptr );

   virtual ISceneNode*
   getHoveredSceneNode() { return m_Hovered; }

   virtual ISceneNode*
   getPickedSceneNode() { return m_Picked; }


   // =====================
   // ### CameraManager ###
   // =====================
   // ICamera* m_Camera;
   // std::vector< ICamera* > m_Cameras;

   // void clearCameras();
   // int32_t getCameraCount() const;

   // int32_t findCamera( ICamera* camera ) const;
   // int32_t findCamera( std::string const & name ) const;

   // ICamera* getCamera( int32_t index ) const;
   // ICamera* getCamera( std::string const & name ) const;

   // ICamera* addCamera( std::string const & name, bool makeActive = true );
   // void addCamera( ICamera* camera, bool makeActive = true );

   // ICamera* getActiveCamera();
   // ICamera const* getActiveCamera() const;
   // bool setActiveCamera( ICamera* camera );

   // ###########################
   // ###                     ###
   // ###     MeshManager     ###
   // ###                     ###
   // ###########################
   virtual bool
   hasMesh( std::string const & name ) const;

   virtual void
   removeMesh( std::string const & name );

   virtual SMesh::SharedPtr
   addEmptyMesh( std::string name );

   virtual bool
   addMesh( SMesh::SharedPtr mesh );

   virtual SMesh::SharedPtr
   getMesh( std::string const & name );

   virtual uint32_t
   getMeshCount() const;

   virtual std::unordered_map< std::string, SMesh::SharedPtr > const &
   getMeshCache() const { return m_MeshLut; }

   virtual uint32_t
   getBufferCount( int mode = 0 ) const;

   virtual uint64_t
   getVertexCount( int mode = 0 ) const;

   virtual uint64_t
   getIndexCount( int mode = 0 ) const;

   virtual uint64_t
   getByteCount( int mode = 0 ) const;



protected:
   DE_CREATE_LOGGER("de.gpu.SceneManager")
   // ########################
   // ### SceneNodeManager ###
   // ########################
   IVideoDriver* m_Driver;
   std::vector< ISceneNode* > m_TopLevel;
   std::vector< ISceneNode* > m_TrashBin;
   ISceneNode* m_RootNode;
   // ########################
   // ### CollisionManager ###
   // ########################
   ISceneNode* m_Hovered;
   ISceneNode* m_Picked;
   // ###################
   // ### MeshManager ###
   // ###################
   std::unordered_map< std::string, SMesh::SharedPtr > m_MeshLut;
};

} // end namespace gpu.
} // end namespace de.


/*
// =======================================================================
struct ISceneManager
// =======================================================================
{
   virtual ~ISceneManager() = default;

   virtual IVideoDriver* getVideoDriver() = 0;
   virtual ISceneNode* getRootSceneNode() = 0;

   virtual void render() = 0;
   virtual void onFrameBegin() = 0; // Does animate() and intersection test?
   virtual void onFrameEnd() = 0;

   //virtual void clearDebugData() = 0;
   //virtual void setDebugData( VisualDebugData const & debugData ) = 0;
   //virtual ISceneNode* getSceneNode( std::string const & name ) = 0;

   // =====================
   // ### NodeManager ###
   // =====================
   virtual void
   clear() = 0;

   virtual void
   resize( int w, int h ) = 0;  // for cameras and render targets, called by driver.stop().

   virtual void
   addSceneNode( ISceneNode* node ) = 0;

   virtual SMeshSceneNode*
   addEmptyMeshSceneNode( ISceneNode* parent = nullptr, int id = -1 ) = 0;

   virtual SMeshSceneNode*
   addMeshSceneNode( SMesh* mesh, ISceneNode* parent = nullptr, int id = -1 ) = 0;

   virtual SMeshSceneNode*
   addMeshSceneNode( SMeshBuffer const & buffer, ISceneNode* parent = nullptr, int id = -1 ) = 0;

   virtual void
   removeSceneNode( ISceneNode* node ) = 0;

   virtual ISceneNode*
   loadSceneNode( std::string uri, ISceneNode* parent = nullptr, int id = -1 ) = 0;

   virtual bool
   saveSceneNode( ISceneNode* node, std::string uri ) = 0;

   virtual bool
   loadScene( std::string uri ) = 0;

   virtual bool
   saveScene( std::string uri ) = 0;

   // virtual ISceneNode* getHoveredSceneNode() = 0;

   virtual ISceneNode*
   pickSceneNode( glm::vec3 const & rayStart, glm::vec3 const & rayDir, ScenePickResult* pickResult = nullptr ) = 0;

   //   virtual uint32_t getVertexCount( int mode ) const = 0;
   //   virtual uint32_t getIndexCount( int mode ) const = 0;
   //   virtual uint32_t getByteCount() const = 0;

   //   virtual std::vector< Triangle3f > getCollisionTriangles() const = 0;

   // =====================
   // ### CameraManager ###
   // =====================
   // virtual void clearCameras() = 0;
   // virtual int32_t getCameraCount() const = 0;

   // virtual int32_t findCamera( ICamera* camera ) const = 0;
   // virtual int32_t findCamera( std::string const & name ) const = 0;

   // virtual ICamera* getCamera( int32_t index ) const = 0;
   // virtual ICamera* getCamera( std::string const & name ) const = 0;

   // virtual ICamera* addCamera( std::string const & name, bool makeActive = true ) = 0;
   // virtual void addCamera( ICamera* camera, bool makeActive = true ) = 0;

   // virtual ICamera* getActiveCamera() = 0;
   // virtual ICamera const* getActiveCamera() const = 0;
   // virtual bool setActiveCamera( ICamera* camera ) = 0;
};
*/

#endif // DE_SCENEMANAGER_HPP
