#ifndef DE_GPU_SCENETREE_QWIDGET_HPP
#define DE_GPU_SCENETREE_QWIDGET_HPP

#include <DarkGPU.hpp>
#include <QWidget>
#include <QTreeWidget>
#include <QTableWidget>
#include <S3DVertexTable.hpp>
#include <V3Edit.hpp>

struct SceneEditor : public QWidget
{
   Q_OBJECT
public:
   SceneEditor( QWidget* parent );
   ~SceneEditor() override;

signals:
   void activatedNode( de::gpu::ISceneNode* );
   void activatedMesh( de::gpu::SMesh::SharedPtr );
   void activatedBuffer( de::gpu::SMeshBuffer* );

public slots:
   void setSceneManager( de::gpu::ISceneManager* smgr );
   void activateNode( de::gpu::ISceneNode* );
   //void activateMesh( de::gpu::SMesh* );
   //void activateBuffer( de::gpu::SMeshBuffer* );
   void addSceneNode( de::gpu::ISceneNode* );
   void addMesh( de::gpu::SMesh::SharedPtr mesh );
   //void removeMesh( std::string name );

private slots:
   void onChangedTRS_T( glm::dvec3 const & value );
   void onChangedTRS_R( glm::dvec3 const & value );
   void onChangedTRS_S( glm::dvec3 const & value );

protected:
   void populateSceneTree();
   void populateMeshTable();
   void populateBufferTable();



   void onNodeClicked( QTreeWidgetItem* item, int col );
   void onMeshClicked( int row, int col );
   void onMeshDoubleClick( int row, int col );
   void onMeshEdited( int row, int col );
   void onBufferClicked( int row, int col );

   DE_CREATE_LOGGER("SceneEditor")

   de::gpu::ISceneManager* m_SceneManager;
   QTreeWidget* m_SceneTree;

   V3Edit* m_T;
   V3Edit* m_R;
   V3Edit* m_S;

   QTableWidget* m_SceneTable;
   QTableWidget* m_MeshTable;
   QTableWidget* m_BufferTable;
   S3DVertexTable* m_VertexTable;

   de::gpu::ISceneNode* m_Node; //current
   de::gpu::SMesh::SharedPtr m_Mesh; //current
   de::gpu::SMeshBuffer* m_Buffer;  //current
   //QTableWidget* m_VertexTable;
   //QTableWidget* m_IndexTable;
   //QTableWidget* m_FaceTable;
   //QTableWidget* m_PrimTable;
   //QTableWidget* m_Material;
};

#endif
