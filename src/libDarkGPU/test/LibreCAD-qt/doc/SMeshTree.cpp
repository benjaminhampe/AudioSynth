#include "SMeshTree.hpp"

/*
#include <QDebug>
#include <QVBoxLayout>
#include <QSplitter>
#include <QHeaderView>

#include <QTabWidget>

SMeshTree::SMeshTree( QWidget* parent )
   : QWidget( parent )
   , m_Mesh( nullptr )
{
   setContentsMargins( 0,0,0,0 );

   QTabWidget* tabs = new QTabWidget( this );

   QWidget* tabMesh = new QWidget( this );
   tabs->addTab( tabMesh, QIcon("../../media-cad/icon.ico"), "SMeshInfo" );

   //setBackgroundRole( QPalette::NoRole );
   //setMinimumSize( 320, 240 );
   //setFocusPolicy( Qt::StrongFocus );
   //setMouseTracking( true );

// [SceneTree]
   m_SceneTree = new QTreeWidget( this );
   m_SceneTree->setContentsMargins( 0,0,0,0 );
   m_SceneTree->setColumnCount( 5 );
   m_SceneTree->headerItem()->setText( 0, "Type");
   m_SceneTree->headerItem()->setText( 1, "Vis");
   m_SceneTree->headerItem()->setText( 2, "Name");
   m_SceneTree->headerItem()->setText( 3, "Contents");
   m_SceneTree->headerItem()->setText( 4, "Bytes");
   m_SceneTree->setColumnWidth(0,160);
   m_SceneTree->setColumnWidth(1,20);
   m_SceneTree->setColumnWidth(2,120);
   m_SceneTree->setColumnWidth(3,100);
   m_SceneTree->setColumnWidth(4,20);

// [BufferTable]
   m_BufferTable = new QTableWidget( this );
   m_BufferTable->setContentsMargins( 0,0,0,0 );
   m_BufferTable->setColumnCount( 4 );
   QTableWidgetItem* name = new QTableWidgetItem();
   QTableWidgetItem* vis = new QTableWidgetItem();
   QTableWidgetItem* primType = new QTableWidgetItem();
   QTableWidgetItem* primCount = new QTableWidgetItem();
   name->setText( "Buffer-Name:" );
   vis->setText( "Vis" );
   primType->setText( "PrimType" );
   primCount->setText( "PrimCount" );
   m_BufferTable->setHorizontalHeaderItem( 0, name );
   m_BufferTable->setHorizontalHeaderItem( 1, vis );
   m_BufferTable->setHorizontalHeaderItem( 2, primType );
   m_BufferTable->setHorizontalHeaderItem( 3, primCount );
   m_BufferTable->setColumnWidth(0,140);
   m_BufferTable->setColumnWidth(1,40);
   m_BufferTable->setColumnWidth(2,80);
   m_BufferTable->setColumnWidth(3,80);

// [VertexTable]
   m_VertexTable = new QTableWidget( this );
   m_VertexTable->setContentsMargins( 0,0,0,0 );
   m_VertexTable->setColumnCount( 9 );
   QTableWidgetItem* x = new QTableWidgetItem();
   QTableWidgetItem* y = new QTableWidgetItem();
   QTableWidgetItem* z = new QTableWidgetItem();
   QTableWidgetItem* nx = new QTableWidgetItem();
   QTableWidgetItem* ny = new QTableWidgetItem();
   QTableWidgetItem* nz = new QTableWidgetItem();
   QTableWidgetItem* color = new QTableWidgetItem();
   QTableWidgetItem* u = new QTableWidgetItem();
   QTableWidgetItem* v = new QTableWidgetItem();
   x->setText( "X" );
   y->setText( "Y" );
   z->setText( "Z" );
   nx->setText( "NX" );
   ny->setText( "NY" );
   nz->setText( "NZ" );
   color->setText( "RGBA" );
   u->setText( "U" );
   v->setText( "V" );
   m_VertexTable->setHorizontalHeaderItem( 0, x );
   m_VertexTable->setHorizontalHeaderItem( 1, y );
   m_VertexTable->setHorizontalHeaderItem( 2, z );
   m_VertexTable->setHorizontalHeaderItem( 3, nx );
   m_VertexTable->setHorizontalHeaderItem( 4, ny );
   m_VertexTable->setHorizontalHeaderItem( 5, nz );
   m_VertexTable->setHorizontalHeaderItem( 6, color );
   m_VertexTable->setHorizontalHeaderItem( 7, u );
   m_VertexTable->setHorizontalHeaderItem( 8, v );
   m_VertexTable->setColumnWidth(0,40);
   m_VertexTable->setColumnWidth(1,40);
   m_VertexTable->setColumnWidth(2,40);
   m_VertexTable->setColumnWidth(3,40);
   m_VertexTable->setColumnWidth(4,40);
   m_VertexTable->setColumnWidth(5,40);
   m_VertexTable->setColumnWidth(6,100);
   m_VertexTable->setColumnWidth(7,40);
   m_VertexTable->setColumnWidth(8,40);

// [VertexTable]
   m_S3DVertexTable = new S3DVertexTable( this );

// [IndexFaceTable]
//   m_tvInds = new QTableWidget( this );
//   m_tvInds->setColumnCount( 5 );
//   QTableWidgetItem* i0 = new QTableWidgetItem();
//   i0->setText( "Primitive" );
//   m_tvInds->setHorizontalHeaderItem( 0, i0 );

//   QTableWidgetItem* iA = new QTableWidgetItem();
//   iA->setText( "iA" );
//   m_tvInds->setHorizontalHeaderItem( 1, iA );

//   QTableWidgetItem* iB = new QTableWidgetItem();
//   iB->setText( "iB" );
//   m_tvInds->setHorizontalHeaderItem( 2, iB );

//   QTableWidgetItem* iC = new QTableWidgetItem();
//   iC->setText( "iC" );
//   m_tvInds->setHorizontalHeaderItem( 3, iC );

   QSplitter* splitter = new QSplitter( Qt::Vertical );
   splitter->setContentsMargins( 0,0,0,0 );
   splitter->addWidget( m_SceneTree );
   splitter->addWidget( m_BufferTable );
   splitter->addWidget( m_VertexTable );
   splitter->addWidget( m_S3DVertexTable );

   QVBoxLayout* vbox = new QVBoxLayout( this );
   vbox->setContentsMargins( 0,0,0,0 );
   vbox->addWidget( splitter,1 );
   tabMesh->setLayout( vbox );

   QVBoxLayout* vTabs = new QVBoxLayout( this );
   vTabs->setContentsMargins( 0,0,0,0 );
   vTabs->addWidget( tabs,1 );
   setLayout( vTabs );

//   connect( m_VertexTable, SIGNAL(windowTitleChanged(QString)),
//            this, SLOT(onTableCellChanged41(int,int)));
//   connect( m_VertexTable, &QTableView::doubleClicked,
//            this, &SMeshTree::onTableCellDoubleClicked );
   connect( m_BufferTable, &QTableWidget::cellEntered,
            this, &SMeshTree::onBufferTableCellActivated );

   connect( m_VertexTable, &QTableWidget::cellChanged,
            this, &SMeshTree::onTableCellChanged );

//   connect( m_VertexTable, &QTableWidget::cellActivated,
//            this, &SMeshTree::onTableCellChanged41);
//   connect( m_VertexTable, &QTableWidget::cellClicked,
//            this, &SMeshTree::onTableCellChanged41);
//   connect( m_VertexTable, &QTableWidget::cellEntered,
//            this, &SMeshTree::onTableCellChanged41);
//   connect( m_VertexTable, &QTableWidget::itemChanged,
//            this, &SMeshTree::onTableCellChanged);
//   connect( m_VertexTable, &QTableWidget::itemClicked,
//            this, &SMeshTree::onTableCellChanged);

   connect( m_SceneTree, &QTreeWidget::itemChanged,
            this, &SMeshTree::onTreeItemChanged);
//   connect( m_SceneTree, &QTreeWidget::itemClicked,
//            this, &SMeshTree::onTreeItemChanged);

}

SMeshTree::~SMeshTree()
{

}

void
SMeshTree::onTreeItemChanged(QTreeWidgetItem* item, int col )
{
   qDebug() << __func__;
   DE_DEBUG("col = ", col)
}

void
SMeshTree::onBufferTableCellActivated( int row, int col )
{
   qDebug() << __func__;
   DE_DEBUG("row = ",row,", col = ", col)

   if ( m_Mesh && m_Mesh->Buffers.size() > row && row > -1 )
   {
      populateVertexTable( &m_Mesh->Buffers[ row ] );
      m_S3DVertexTable->setData( &m_Mesh->Buffers[ row ].Vertices );
   }

}

void
SMeshTree::onTableCellChanged( int row, int col )
{
   qDebug() << __func__;
   DE_DEBUG("row = ",row,", col = ", col)
}

//void
//SMeshTree::onTableCellDoubleClicked( const QModelIndex &index )
//{
//   qDebug() << __func__;
//   int r = index.row();
//   int c = index.column();
//   DE_DEBUG("row = ",r,", col = ", c)
//}
void
SMeshTree::setMesh( de::gpu::SMesh* mesh )
{
   m_Mesh = mesh;

   populateTreeMesh();
   populateBufferTable();

   if ( m_Mesh )
   {
      if ( m_Mesh->Buffers.size() )
      {
         //m_VertexTableModel->setMeshBuffer( &m_Mesh->Buffers[ 0 ] );
         populateS3DVertexTable( &m_Mesh->Buffers[ 0 ] );
         populateVertexTable( &m_Mesh->Buffers[ 0 ] );
      }
   }


}

void
SMeshTree::populateTreeMesh()
{
   qDebug() << __func__;
   //DE_DEBUG("BufferCount = ", m_Mesh.Buffers.size())

   m_SceneTree->setEnabled( false );
//   connect( m_SceneTree, &QTreeWidget::itemChanged,
//            this, &SMeshTree::onTreeItemChanged);

   m_SceneTree->clear();
   m_SceneTree->setContentsMargins(0,0,0,0);
   if ( !m_Mesh )
   {
      return;
   }

   QTreeWidgetItem* tiMesh = new QTreeWidgetItem();
   tiMesh->setText(0, QString("SMesh[%1]").arg(0) );
   tiMesh->setData(0, Qt::UserRole, QVariant(0));

   //tiMesh->setFlags( Qt::ItemIsEditable );

   tiMesh->setFlags(tiMesh->flags() | Qt::ItemIsEditable | Qt::ItemIsSelectable );
   tiMesh->setText(1, m_Mesh->isVisible() ? "yes" : "no" );
   tiMesh->setData(1, Qt::UserRole, QVariant(m_Mesh->isVisible()));

   QString name = QString::fromStdString( m_Mesh->Name );
   tiMesh->setText(2, name );
   tiMesh->setData(2, Qt::UserRole, QVariant(name));

   for ( size_t i = 0; i < m_Mesh->Buffers.size(); ++i )
   {
      auto const & mb = m_Mesh->Buffers[ i ];

      QTreeWidgetItem* tiBuffer = new QTreeWidgetItem();

      //tiBuffer->setFlags( Qt::ItemIsEditable );
      tiBuffer->setFlags(tiBuffer->flags() | Qt::ItemIsEditable | Qt::ItemIsSelectable );

      tiBuffer->setText(0, QString("SMeshBuffer[%1]").arg(i));
      tiBuffer->setData(0, Qt::UserRole, QVariant(i));
      tiBuffer->setText(1, mb.isVisible() ? "yes" : "no" );
      tiBuffer->setData(1, Qt::UserRole, QVariant(mb.isVisible()));
      QString name = QString::fromStdString( mb.Name );
      tiBuffer->setText(2, name);
      tiBuffer->setData(2, Qt::UserRole, QVariant(name));
      QString primType = QString::fromStdString( mb.getPrimitiveTypeStr() );
      tiBuffer->setText(3, primType);
      tiBuffer->setData(3, Qt::UserRole, QVariant(primType));

      QTreeWidgetItem* tiVertices = new QTreeWidgetItem();
      tiVertices->setText(0, QString("Vertices = %1").arg(mb.getVertexCount()) );
      tiVertices->setData(0, Qt::UserRole, QVariant(mb.getVertexCount()));
      tiBuffer->addChild( tiVertices );

      QTreeWidgetItem* tiIndices = new QTreeWidgetItem();
      tiIndices->setText(0, QString("Indices = %1").arg(mb.getIndexCount()) );
      tiIndices->setData(0, Qt::UserRole, QVariant(mb.getIndexCount()));
      tiBuffer->addChild( tiIndices );

      tiMesh->addChild( tiBuffer );

   }

   m_SceneTree->addTopLevelItem( tiMesh );

   m_SceneTree->setEnabled( true );
}

void
SMeshTree::populateBufferTable()
{
   qDebug() << __func__;
   //DE_DEBUG("BufferCount = ", m_Mesh.Buffers.size())

   m_BufferTable->blockSignals( true );
   m_BufferTable->clearContents();
   m_BufferTable->setContentsMargins(0,0,0,0);

   auto font = m_BufferTable->font();
   font.setPointSize( 8 );
   m_BufferTable->setFont( font );
   m_BufferTable->setColumnCount( 4 );

   if ( m_Mesh )
   {
      m_BufferTable->setRowCount( m_Mesh->Buffers.size() );

      for ( size_t i = 0; i < m_Mesh->Buffers.size(); ++i )
      {
         de::gpu::SMeshBuffer const & buffer = m_Mesh->Buffers[ i ];

         QTableWidgetItem* ti = nullptr;

         ti = new QTableWidgetItem();
         ti->setText( QString::fromStdString( buffer.Name ) );
         m_BufferTable->setItem( i, 0, ti );

         ti = new QTableWidgetItem();
         ti->setText( buffer.isVisible() ? "yes" : "no" );
         m_BufferTable->setItem( i, 1, ti );

         ti = new QTableWidgetItem();
         DE_DEBUG("i=",i, ", ", buffer.getPrimitiveTypeStr())
         ti->setText( QString::fromStdString( buffer.getPrimitiveTypeStr() ) );
         m_BufferTable->setItem( i, 2, ti );

         ti = new QTableWidgetItem();
         DE_DEBUG("i=",i, ", ", buffer.getPrimitiveCount())
         ti->setText( QString::number( buffer.getPrimitiveCount() ) );
         m_BufferTable->setItem( i, 3, ti );


      }

   }


   m_BufferTable->blockSignals( false );
}

void
SMeshTree::populateS3DVertexTable( de::gpu::SMeshBuffer* mbPtr )
{
   if ( mbPtr )
   {
      m_S3DVertexTable->setData( &mbPtr->Vertices );
   }
}

void
SMeshTree::populateVertexTable( de::gpu::SMeshBuffer* mbPtr )
{
   m_VertexTable->blockSignals( true );

   m_VertexTable->clearContents();
   m_VertexTable->setContentsMargins(0,0,0,0);
   auto font = m_VertexTable->font();
   font.setPointSize( 8 );
   m_VertexTable->setFont( font );
   m_VertexTable->setColumnCount( 9 );

   if ( mbPtr )
   {
      de::gpu::SMeshBuffer const & mb = *mbPtr;

      uint32_t maxIndex = std::min( uint32_t(mb.Vertices.size()), uint32_t(10000) );

      m_VertexTable->setRowCount( maxIndex );

      for ( size_t k = 0; k < maxIndex; ++k )
      {
         de::gpu::S3DVertex const & vertex = mb.Vertices[ k ];

         QTableWidgetItem* ti = nullptr;
         //ti->setFlags(ti->flags() | Qt::ItemIsEditable | Qt::ItemIsSelectable );
   //      ti->setFlags( Qt::ItemIsEditable );
   //      ti->setText( QString("S3DVertex[%1]").arg(i) );
   //      m_VertexTable->setItem( k, 0, ti );

         ti = new QTableWidgetItem();
         ti->setText( QString::number(vertex.pos.x ) );
         m_VertexTable->setItem( k, 0, ti );

         ti = new QTableWidgetItem();
         ti->setText( QString::number(vertex.pos.y ) );
         m_VertexTable->setItem( k, 1, ti );

         ti = new QTableWidgetItem();
         ti->setText( QString::number(vertex.pos.z ) );
         m_VertexTable->setItem( k, 2, ti );

         ti = new QTableWidgetItem();
         ti->setText( QString::number(vertex.normal.x ) );
         m_VertexTable->setItem( k, 3, ti );

         ti = new QTableWidgetItem();
         ti->setText( QString::number(vertex.normal.y ) );
         m_VertexTable->setItem( k, 4, ti );

         ti = new QTableWidgetItem();
         ti->setText( QString::number(vertex.normal.z ) );
         m_VertexTable->setItem( k, 5, ti );

         ti = new QTableWidgetItem();
         ti->setText( QString::number(vertex.color, 16 ).toUpper() );
         m_VertexTable->setItem( k, 6, ti );

         ti = new QTableWidgetItem();
         ti->setText( QString::number(vertex.tex.x ) );
         m_VertexTable->setItem( k, 7, ti );

         ti = new QTableWidgetItem();
         ti->setText( QString::number(vertex.tex.y ) );
         m_VertexTable->setItem( k, 8, ti );
         m_VertexTable->setRowHeight( k, 10 );

      }

   }
   m_VertexTable->blockSignals( false );
}

void
SMeshTree::populateFaceTable( de::gpu::SMeshBuffer* mbPtr )
{
   //m_tvInds->clear();

   //de::gpu::SMeshBuffer const & mb = *mbPtr;
}

*/

