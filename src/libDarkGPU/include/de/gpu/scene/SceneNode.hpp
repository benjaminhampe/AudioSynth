#ifndef DE_GPU_SCENE_SMESHSCENENODE_HPP
#define DE_GPU_SCENE_SMESHSCENENODE_HPP

#include <de/gpu/smesh/SMesh.hpp>
#include <de/gpu/smesh/Debug.hpp> // uses VisualDebugData
#include <de/gpu/Math3D_Quaternion.hpp>
#include <de/gpu/ui/EventReceiver.hpp>

namespace de {
namespace gpu {

class ISceneManager;
class IVideoDriver;

// =======================================================================
struct SceneNodeType
// =======================================================================
{
   enum EType { User = 0, Mesh, Camera, SkyBox, Cube, Terrain, Plane, Water, ETypeCount };
};

// =======================================================================
struct ISceneNode
// =======================================================================
{
   typedef double T;
   typedef glm::tmat4x4< T > M4;
   typedef glm::tvec2< T > V2;
   typedef glm::tvec3< T > V3;
   typedef glm::tvec4< T > V4;
   typedef std::unique_ptr< ISceneNode > UniquePtr;
   typedef std::shared_ptr< ISceneNode > SharedPtr;
   typedef std::weak_ptr< ISceneNode > WeakPtr;

   ISceneNode( ISceneManager* smgr, ISceneNode* parent, int id = -1 );
   virtual ~ISceneNode();
   virtual Box3f getBoundingBox() const = 0;
   virtual void render() = 0;

   virtual SceneNodeType::EType getNodeType() const { return SceneNodeType::User; }
   virtual void onEvent( SEvent event ) {}
   virtual void onAnimate( double pts ) {}

   virtual ISceneManager* getSceneManager();
   virtual std::string getName() const;
   virtual void setName( std::string const & name );
   virtual uint32_t getVertexCount() const { return 0; }
   virtual uint32_t getIndexCount() const { return 0; }
   virtual uint32_t getByteCount() const { return 0; }

   virtual VisualDebugData const & getDebugData() const;
   virtual VisualDebugData & getDebugData();
   virtual void setDebugData( VisualDebugData const & debugData );

   virtual std::string toString() const;

   // Get the axis aligned, transformed and animated absolute bounding box of this node.
   virtual Box3f getAbsoluteBoundingBox() const;
   // Get the absolute transformation of the node. Is recalculated every OnAnimate()-call.
   virtual M4 const & getAbsoluteTransform() const;
   // Gets the absolute position of the node in world coordinates.
   virtual V3 getAbsolutePosition() const;
   // Updates the absolute position based on the relative and the parents position
   virtual void updateAbsoluteTransform();
   // Returns the relative transformation of the scene node.
   virtual M4 getRelativeTransform() const;

   // Returns whether the node should be visible (if all of its parents are visible).
   virtual bool isVisible() const;
   // Check whether the node is truly visible, taking into accounts its parents' visibility
   virtual bool isTrulyVisible() const;
   // Sets if the node should be visible or not.
   virtual void setVisible( bool isVisible );
   // Get the id of the scene node.
   virtual int getId() const;
   // Sets the id of the scene node.
   virtual void setId( int id );
   // Gets the scale of the scene node relative to its parent.
   virtual V3 const & getScale() const;
   // Gets the rotation of the node relative to its parent in degrees.
   virtual V3 const & getRotation() const;
   // Gets the position of the node relative to its parent.
   virtual V3 const & getPosition() const;
   // Sets the relative scale of the scene node.
   virtual void setScale( V3 const & scale );
   // Sets the rotation of the node relative to its parent in degrees.
   virtual void setRotation( V3 const & rotation );
   // Sets the position of the node relative to its parent.
   virtual void setPosition( V3 const & pos );
   // Sets the relative scale of the scene node.
   virtual void setScale( T x, T y, T z );
   // Sets the relative rotation of the scene node in degrees.
   virtual void setRotation( T x, T y, T z );
   // Sets the relative position of the scene node.
   virtual void setPosition( T x, T y, T z );
   // Count.
   virtual uint32_t getBufferCount( int mode = 0 ) const { return 0; }
   virtual uint64_t getVertexCount( int mode = 0 ) const { return 0; }
   virtual uint64_t getIndexCount( int mode = 0 ) const { return 0; }
   virtual uint64_t getByteCount( int mode = 0 ) const { return 0; }

   // Removes this scene node from the scene
   //virtual void removeFromParent();
   // Get list of all children.
   virtual std::vector< ISceneNode* > const & getChildren() const;
   // Removes all children of this scene node
   virtual void removeAll();
   // Removes a child from this scene node.
   virtual bool removeChild( ISceneNode* child );
   // Returns the parent of this scene node
   virtual ISceneNode* getParent() const;
   // Changes the parent of the scene node.
   virtual void setParent( ISceneNode* newParent );
   // Adds a child to this scene node.
   virtual void addChild( ISceneNode* child );


   virtual bool isCollisionEnabled() const;
   virtual void setCollisionEnabled( bool enable );

   virtual bool
   intersectRay( Ray3< T > const & ray,
                 glm::tvec3< T > * hitPosition = nullptr,
                 uint32_t* hitBufferIndex = nullptr,
                 uint32_t* hitTriangleIndex = nullptr,
                 Triangle3< T >* hitTriangle = nullptr ) const { return false; }

   virtual bool
   intersectRayWithBoundingBox( Ray3< T > const & ray,
                 glm::tvec3< T > * hitPosition = nullptr,
                 uint32_t* hitMeshBufferIndex = nullptr ) const { return false; }

//   std::vector< Triangle3f > const &
//   getCollisionTriangles() const { return m_CollisionTriangles; }
//   void
//   setCollisionTriangles( std::vector< Triangle3f > const & triangles ) { m_CollisionTriangles = triangles; }
//   void
//   createCollisionTriangles() {}

   DE_CREATE_LOGGER("de.gpu.ISceneNode")
   ISceneManager* m_SceneManager;
   ISceneNode* m_Parent;             //! Pointer to the parent
   int m_Id;               //! ID of the node.
   bool m_IsVisible;         //! Is the node visible?
   bool m_IsDirty;        //! Is dirty matrix
   bool m_IsCollisionEnabled;     //! Is enabled for interactivity
   //bool m_IsDebugObject; //! Is debug object?

   M4 m_AbsoluteTransform;   // Absolute TRS transformation of the node.
   V3 m_RelativeTranslation; // Relative Translation of the scene node.
   V3 m_RelativeRotation;    // Relative Rotation (euler angles) of the scene node.
   V3 m_RelativeScale;       // Relative Scale of the scene node.

   //AnimTimer m_AnimTime;
   std::vector< ISceneNode* > m_Children;   //! List of all children of this node

   VisualDebugData m_DebugData;
   std::string m_Name;

   //std::vector< Triangle3f > m_CollisionTriangles;
};


// =======================================================================
struct SMeshSceneNode : public ISceneNode
// =======================================================================
{
   // SMeshSceneNode( SMeshBuffer* mesh, ISceneManager* smgr, ISceneNode* parent = nullptr, int id = -1 );
   SMeshSceneNode( SMesh::SharedPtr const & mesh, ISceneManager* smgr, ISceneNode* parent = nullptr, int id = -1 );
   SMeshSceneNode( ISceneManager* smgr, ISceneNode* parent = nullptr, int id = -1 );
   ~SMeshSceneNode() override;
   void render() override;

   SMesh::SharedPtr const &
   getMesh() const;

   SMesh::SharedPtr
   getMesh();

   void
   setMesh( SMesh::SharedPtr const & mesh );

   bool
   intersectRay( Ray3< T > const & ray,
                 glm::tvec3< T > * outPosition = nullptr,
                 uint32_t* outBufferIndex = nullptr,
                 uint32_t* outTriangleIndex = nullptr,
                 Triangle3< T >* outTriangle = nullptr ) const override;

   bool
   intersectRayWithBoundingBox( Ray3< T > const & ray,
                 glm::tvec3< T > * hitPosition = nullptr,
                 uint32_t* hitMeshBufferIndex = nullptr ) const override;


   SceneNodeType::EType
   getNodeType() const override { return SceneNodeType::Mesh; }
   Box3f
   getBoundingBox() const override { return m_Mesh ? m_Mesh->getBoundingBox() : Box3f(); }

   std::string
   getName() const override { return m_Mesh ? m_Mesh->Name : ""; }
   void
   setName( std::string const & name ) override { if (m_Mesh) m_Mesh->setName( name ); }

   void setLighting( int illum ); // { if ( m_Mesh ) m_Mesh->setLighting( enable ); }
   void setFogEnable( bool enable ); // { if ( m_Mesh ) m_Mesh->setFogEnable( enable ); }
   void setWireframe( bool enable ); // { if ( m_Mesh ) m_Mesh->setWireframe( enable ); }
   void setCulling( Culling cull ); // { if ( m_Mesh ) m_Mesh->setCulling( culling ); }
   void setCulling( bool enable ); // { if ( m_Mesh ) m_Mesh->setCulling( enable ); }
   void setDepth( Depth depth ); // { if ( m_Mesh ) m_Mesh->setDepth( depth ); }
   void setTexture( int stage, TexRef const & tex ); // { if ( m_Mesh ) m_Mesh->setTexture( stage, tex ); }

   void setDebugData( VisualDebugData const & debugFlags ) override;

   uint32_t getBufferCount( int mode = 0 ) const override;
   uint64_t getVertexCount( int mode = 0 ) const override;
   uint64_t getIndexCount( int mode = 0 ) const override;
   uint64_t getByteCount( int mode = 0 ) const override;

   int getLighting() const { return m_Mesh ? m_Mesh->getLighting() : 0; }
//   int getFogEnable( bool enable ) { if ( m_Mesh ) m_Mesh->setFogEnable( enable ); }
//   int getWireframe( bool enable ) { if ( m_Mesh ) m_Mesh->setWireframe( enable ); }
//   int getCulling( Culling cull ) { if ( m_Mesh ) m_Mesh->setCulling( cull ); }
//   int getDepth( Depth depth ) { if ( m_Mesh ) m_Mesh->setDepth( depth ); }
//   int getTexture( int stage, TexRef const & tex ) { if ( m_Mesh ) m_Mesh->setTexture( stage, tex ); }


protected:
   void updateDebugMesh();

   DE_CREATE_LOGGER("de.gpu.SMeshSceneNode")
   SMesh::SharedPtr m_Mesh;
   bool m_IsDirtyDebugMesh;
   SMesh m_DebugMesh;
   //std::vector< Triangle3f > m_CollisionTriangles;
};

} // end namespace gpu.
} // end namespace de.

#endif

