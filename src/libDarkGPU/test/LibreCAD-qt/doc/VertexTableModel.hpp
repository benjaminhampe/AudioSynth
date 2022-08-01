#ifndef DE_GPU_QWIDGET_S3DVERTEX_TABLEVIEW_HPP
#define DE_GPU_QWIDGET_S3DVERTEX_TABLEVIEW_HPP

#include <DarkGPU.hpp>
#include <QWidget>
#include <QTableView>
#include <QAbstractTableModel>


struct VertexTableModel : public QAbstractTableModel
{
   Q_OBJECT
public:
   VertexTableModel( QObject *parent = nullptr );
   ~VertexTableModel() override;

   virtual void
   setMeshBuffer( de::gpu::SMeshBuffer* mbPtr )
   {
      m_MeshBuffer = mbPtr;
      if ( m_MeshBuffer )
      {
         emit dataChanged( index(0,0), index(8,m_MeshBuffer->getVertexCount()) );
      }
      else
      {
         emit dataChanged( index(0,0), index(8,0) );
      }

   }

   int
   rowCount( QModelIndex const & parent = QModelIndex()) const override;

   int
   columnCount( QModelIndex const & parent = QModelIndex()) const override;

   QVariant
   data( QModelIndex const & index, int role = Qt::DisplayRole) const override;

   de::gpu::SMeshBuffer* m_MeshBuffer;

};

#endif
