#ifndef DE_GPU_SMESHTREE_QWIDGET_HPP
#define DE_GPU_SMESHTREE_QWIDGET_HPP

/*
#include <DarkGPU.hpp>

#include <QWidget>
#include <QTreeWidget>
#include <QTableWidget>
//#include <VertexTableModel.hpp>
#include <FastTableWidget.hpp>
#include <S3DVertexTable.hpp>

struct SMeshTree : public QWidget
{
   Q_OBJECT
public:
   SMeshTree( QWidget* parent );
   ~SMeshTree() override;

signals:
public slots:
   void setMesh( de::gpu::SMesh* mesh );

protected:
   void onTreeItemChanged( QTreeWidgetItem* item, int col );

   void onBufferTableCellActivated( int row, int col );

   void onTableCellChanged( int row, int col );
   //void onTableCellDoubleClicked( const QModelIndex &index );

   void populateTreeMesh();
   void populateBufferTable();

   void populateS3DVertexTable( de::gpu::SMeshBuffer* mb );
   void populateVertexTable( de::gpu::SMeshBuffer* mb );
   void populateFaceTable( de::gpu::SMeshBuffer* mb );

   DE_CREATE_LOGGER("SMeshTree")

   de::gpu::ISceneManager* m_Scene;

   QTreeWidget* m_SceneTree;

   //VertexTableModel* m_VertexTableModel;
   //QTableView* m_VertexTableView;
   QTableWidget* m_BufferTable;
   QTableWidget* m_VertexTable;
   QTableWidget* m_IndexTable;
   QTableWidget* m_FaceTable;

   S3DVertexTable* m_S3DVertexTable;

};
*/

#endif
