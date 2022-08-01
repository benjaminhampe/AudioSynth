#include "SceneEditor.hpp"
#include <QDebug>
#include <QVBoxLayout>
#include <QSplitter>
#include <QHeaderView>

#include <QTabWidget>
#include <QPushButton>

QString
toQString( glm::vec3 const & v )
{
   return QString( "%1,%2,%3" ).arg( v.x ).arg( v.y ).arg( v.z );
}

QTableWidgetItem*
createTableItem( QTableWidget* table, int col, QString name, int width )
{
   QTableWidgetItem* item = new QTableWidgetItem();
   item->setText( name );
   table->setHorizontalHeaderItem( col, item );
   table->setColumnWidth( col, width );
   return item;
}

SceneEditor::SceneEditor( QWidget* parent )
   : QWidget( parent )
   //, m_SceneTable( nullptr
   , m_SceneManager( nullptr )
   , m_MeshTable( nullptr )
   , m_BufferTable( nullptr )
   , m_Node( nullptr )
   , m_Mesh( nullptr )
   , m_Buffer( nullptr )
{
   setContentsMargins( 0,0,0,0 );

   QTabWidget* tabs = new QTabWidget( this );

   QWidget* tabMesh = new QWidget( this );
   tabs->addTab( tabMesh, QIcon("../../media-cad/icon.ico"), "SMeshInfo" );

   //setBackgroundRole( QPalette::NoRole );
   //setMinimumSize( 320, 240 );
   //setFocusPolicy( Qt::StrongFocus );
   //setMouseTracking( true );

// [SceneNodes]
   auto m_BtnAddSceneNode = new QPushButton( "+", this );

   m_SceneTree = new QTreeWidget( this );
   m_SceneTree->setContentsMargins( 0,0,0,0 );
   m_SceneTree->setColumnCount( 11 );
   m_SceneTree->headerItem()->setText( 0, "Node");
   m_SceneTree->headerItem()->setText( 1, "Vis");
   m_SceneTree->headerItem()->setText( 2, "Buf");
   m_SceneTree->headerItem()->setText( 3, "Vertices");
   m_SceneTree->headerItem()->setText( 4, "Indices");
   m_SceneTree->headerItem()->setText( 5, "PrimTypes");
   m_SceneTree->headerItem()->setText( 6, "PrimCount");
   m_SceneTree->headerItem()->setText( 7, "BBoxSize");
   m_SceneTree->headerItem()->setText( 8, "BBoxMin");
   m_SceneTree->headerItem()->setText( 9, "BBoxMax");
   m_SceneTree->headerItem()->setText( 10, "FullName");

   m_SceneTree->setColumnWidth(0,120);
   m_SceneTree->setColumnWidth(1,30);
   m_SceneTree->setColumnWidth(2,30);
   m_SceneTree->setColumnWidth(3,40);
   m_SceneTree->setColumnWidth(4,40);
   m_SceneTree->setColumnWidth(5,50);
   m_SceneTree->setColumnWidth(6,30);
   m_SceneTree->setColumnWidth(7,60);
   m_SceneTree->setColumnWidth(8,60);
   m_SceneTree->setColumnWidth(9,60);
   m_SceneTree->setColumnWidth(10,250);

   m_T = new V3Edit( "Position", this );
   m_R = new V3Edit( "Rotation", this );
   m_S = new V3Edit( "Scale", this );

   m_T->setDecimals( 3 );
   m_R->setDecimals( 3 );
   m_S->setDecimals( 9 );

   m_T->setMinimum( 0.0 );
   m_R->setMinimum( 0.0 );
   m_S->setMinimum( 0.0 );

   m_T->setMaximum( 1.0e9 );
   m_R->setMaximum( 1.0e9 );
   m_S->setMaximum( 1.0e9 );

   m_T->setValue( 0.0 );
   m_R->setValue( 0.0 );
   m_S->setValue( 1.0 );

   m_T->setSingleStep( 10 );
   m_R->setSingleStep( 5 );
   m_S->setSingleStep( 0.1 );

   QVBoxLayout* trsLayout = new QVBoxLayout();
   trsLayout->setContentsMargins( 0,0,0,0 );
   trsLayout->setSpacing( 0 );
   trsLayout->addWidget( m_T );
   trsLayout->addWidget( m_R );
   trsLayout->addWidget( m_S );

//   m_SceneTable = new QTableWidget( this );
//   m_SceneTable->setContentsMargins( 0,0,0,0 );
//   m_SceneTable->setColumnCount( 6 );
//   createTableItem( m_SceneTable, 0, "NodeType", 140 );
//   createTableItem( m_SceneTable, 1, "Vis", 40 );
//   createTableItem( m_SceneTable, 2, "Name", 80 );
//   createTableItem( m_SceneTable, 3, "#v", 80 );
//   createTableItem( m_SceneTable, 4, "#i", 80 );
//   createTableItem( m_SceneTable, 5, "Bytes", 80 );

   QHBoxLayout* sceneLayoutH = new QHBoxLayout();
   sceneLayoutH->setContentsMargins( 0,0,0,0 );
   sceneLayoutH->addWidget( m_BtnAddSceneNode );

   QVBoxLayout* sceneLayoutV = new QVBoxLayout();
   sceneLayoutV->setContentsMargins( 0,0,0,0 );
   sceneLayoutV->addLayout( sceneLayoutH );
   sceneLayoutV->addWidget( m_SceneTree,1 );
   sceneLayoutV->addLayout( trsLayout );

   QWidget* sceneLayoutWidget = new QWidget( this );
   sceneLayoutWidget->setContentsMargins(0,0,0,0);
   sceneLayoutWidget->setLayout( sceneLayoutV );

// [MeshTable]
//   m_MeshTable = new QTableWidget( this );
//   m_MeshTable->setContentsMargins( 0,0,0,0 );
//   m_MeshTable->setColumnCount( 9 );
//   createTableItem( m_MeshTable, 0, "Name", 120 );
//   createTableItem( m_MeshTable, 1, "Vis", 40 );
//   createTableItem( m_MeshTable, 2, "Buffers", 40 );
//   createTableItem( m_MeshTable, 3, "bboxSize", 80 );
//   createTableItem( m_MeshTable, 4, "bboxMin", 80 );
//   createTableItem( m_MeshTable, 5, "bboxMax", 80 );
//   createTableItem( m_MeshTable, 6, "Vertices", 60 );
//   createTableItem( m_MeshTable, 7, "Indices", 60 );
//   createTableItem( m_MeshTable, 8, "Bytes", 60 );

// [BufferTable]
//   m_BufferTable = new QTableWidget( this );
//   m_BufferTable->setContentsMargins( 0,0,0,0 );
//   m_BufferTable->setColumnCount( 16 );
//   createTableItem( m_BufferTable, 0, "Buffer-Name:", 140 );
//   createTableItem( m_BufferTable, 1, "Vis", 40 );
//   createTableItem( m_BufferTable, 2, "bboxSize", 80 );
//   createTableItem( m_BufferTable, 3, "bboxMin", 80 );
//   createTableItem( m_BufferTable, 4, "bboxMax", 80 );
//   createTableItem( m_BufferTable, 5, "PrimType", 80 );
//   createTableItem( m_BufferTable, 6, "PrimCount", 80 );
//   createTableItem( m_BufferTable, 7, "Vertices", 80 );
//   createTableItem( m_BufferTable, 8, "Indices", 80 );
//   createTableItem( m_BufferTable, 9, "Bytes", 80 );
//   createTableItem( m_BufferTable, 10, "TexCount", 80 );
//   createTableItem( m_BufferTable, 11, "Tex0", 80 );
//   createTableItem( m_BufferTable, 12, "Tex1", 80 );
//   createTableItem( m_BufferTable, 13, "Tex2", 80 );
//   createTableItem( m_BufferTable, 14, "Tex3", 80 );
//   createTableItem( m_BufferTable, 15, "Ambient", 80 );

// [VertexTable]
   m_VertexTable = new S3DVertexTable( this );
   m_VertexTable->setContentsMargins( 0,0,0,0 );

   QSplitter* splitter = new QSplitter( Qt::Vertical );
   splitter->setContentsMargins( 0,0,0,0 );
   splitter->addWidget( sceneLayoutWidget );
   //splitter->addWidget( m_MeshTable );
   //splitter->addWidget( m_BufferTable );
   splitter->addWidget( m_VertexTable );

   QVBoxLayout* vbox = new QVBoxLayout( this );
   vbox->setContentsMargins( 0,0,0,0 );
   vbox->addWidget( splitter,1 );
   tabMesh->setLayout( vbox );

   QVBoxLayout* vTabs = new QVBoxLayout( this );
   vTabs->setContentsMargins( 0,0,0,0 );
   vTabs->addWidget( tabs,1 );
   setLayout( vTabs );

   connect( m_SceneTree, &QTreeWidget::itemClicked,
            this, &SceneEditor::onNodeClicked);

   connect( m_T, &V3Edit::valueChanged, this, &SceneEditor::onChangedTRS_T);
   connect( m_R, &V3Edit::valueChanged, this, &SceneEditor::onChangedTRS_R);
   connect( m_S, &V3Edit::valueChanged, this, &SceneEditor::onChangedTRS_S);

//   connect( m_MeshTable, &QTableWidget::cellClicked,
//            this, &SceneEditor::onMeshClicked );

//   connect( m_MeshTable, &QTableWidget::cellDoubleClicked,
//            this, &SceneEditor::onMeshDoubleClick );

//   connect( m_MeshTable, &QTableWidget::cellChanged,
//            this, &SceneEditor::onMeshEdited );

//   connect( m_BufferTable, &QTableWidget::cellClicked,
//            this, &SceneEditor::onBufferClicked );


//   connect( this, &SceneEditor::activatedMeshBuffer,
//            m_VertexTable, &S3DVertexTable::setData );
}

SceneEditor::~SceneEditor()
{
}


void
SceneEditor::onChangedTRS_T( glm::dvec3 const & pos )
{
   if ( !m_Node ) return;
   m_Node->setPosition( pos );
}

void
SceneEditor::onChangedTRS_R( glm::dvec3 const & euler )
{
   if ( !m_Node ) return;
   m_Node->setRotation( euler );
}

void
SceneEditor::onChangedTRS_S( glm::dvec3 const & scale )
{
   if ( !m_Node ) return;
   m_Node->setScale( scale );
}

void
SceneEditor::setSceneManager( de::gpu::ISceneManager* smgr )
{
   m_SceneManager = smgr;
}


void
SceneEditor::addSceneNode( de::gpu::ISceneNode* node )
{
   if ( !node ) { DE_ERROR("No node") return; }

   if ( !m_SceneManager ) { DE_ERROR("No scene manager") return; }

   m_SceneManager->addSceneNode( node );

   populateSceneTree();
   activateNode( node );
}

void
SceneEditor::activateNode( de::gpu::ISceneNode* node )
{
   if ( m_Node == node ) { return; }
   m_Node = node;
   if ( m_Node )
   {
      m_T->setValue( m_Node->getPosition() );
      m_R->setValue( m_Node->getRotation() );
      m_S->setValue( m_Node->getScale() );

      emit activatedNode( node );

      if ( m_Node->getNodeType() == de::gpu::SceneNodeType::Mesh )
      {
         de::gpu::SMeshSceneNode* snode =
            reinterpret_cast< de::gpu::SMeshSceneNode* >( node );
         if ( snode )
         {
            auto mesh = snode->getMesh();
            if ( mesh )
            {
               emit activatedMesh( mesh );

               DE_DEBUG("Activated SMeshSceneNode ",snode->getName())

               if ( mesh->Buffers.size() )
               {
                  emit activatedBuffer( &mesh->Buffers[ 0 ] );

                  m_VertexTable->setData( &mesh->Buffers[ 0 ] );
               }
            }
         }
      }
   }
}


void
SceneEditor::onNodeClicked( QTreeWidgetItem* item, int col )
{
   if ( !item ) { DE_ERROR("No item") return; }

   //DE_DEBUG("col = ",col)
   uint64_t value = item->data( 0, Qt::UserRole ).toULongLong();
   auto node = (de::gpu::ISceneNode*)value;

   if ( !node ) { DE_ERROR("No node, col = ",col) }

   activateNode( node );

   if ( col == 1 )
   {
      //DE_DEBUG("Got col = 1")

      if ( node->getNodeType() == de::gpu::SceneNodeType::Mesh )
      {
         //DE_DEBUG("Got node-type == mesh")
         auto snode = reinterpret_cast< de::gpu::SMeshSceneNode* >( node );
         if ( snode )
         {
            DE_DEBUG("Got snode and col = 1")

            bool visible = true;

            int32_t bufferIndex = item->data( 1, Qt::UserRole ).toInt();
            if ( bufferIndex > -1 )
            {
               auto mesh = snode->getMesh();
               if ( mesh && bufferIndex < mesh->Buffers.size() )
               {
                  auto & buffer = mesh->Buffers[ bufferIndex ];

                  buffer.setVisible( !buffer.isVisible() );
                  visible = buffer.isVisible();
               }
            }
            else
            {
               snode->setVisible( !snode->isVisible() );
               visible = snode->isVisible();
            }

            if ( visible )
            {
               item->setText( col, "1");
            }
            else
            {
               item->setText( col, "0");
            }
         }
      }
   }
}

void
SceneEditor::addMesh( de::gpu::SMesh::SharedPtr mesh )
{
   if ( !m_SceneManager ) { DE_ERROR("No scene") return; }
   if ( !mesh ) { DE_ERROR("No mesh") return; }

   DE_DEBUG("Add SMesh ",(void*)mesh.get())

   auto node = m_SceneManager->addMeshSceneNode( mesh, nullptr, -1 );

   populateSceneTree();

   activateNode( node );
}



void
addBufferTreeItem( QTreeWidgetItem* parent, int i, de::gpu::SMeshBuffer* o, uint64_t nodeAddress )
{
   if ( !o ) return;

   QTreeWidgetItem* item = new QTreeWidgetItem();
   item->setFlags(item->flags() | Qt::ItemIsEditable | Qt::ItemIsSelectable );

   QString name = QString::fromStdString( dbGetFileName( o->Name ) );
   if ( name.isEmpty() )
   {
      name = QString("SMeshBuffer[%1]").arg(i);
   }

   item->setData(0, Qt::UserRole, QVariant(nodeAddress) );
   item->setData(1, Qt::UserRole, QVariant(i) );

   item->setText(0, name );
   item->setText(1, o->isVisible() ? "1" : "0" );
   item->setText(2, QString::number( i ) );
   item->setText(3, QString::number( o->getVertexCount() ) );
   item->setText(4, QString::number( o->getIndexCount() ) );
   item->setText(5, "?" ); // QString::fromStdString( o->getPrimitiveTypeStr() ));
   item->setText(6, QString("?") );
   auto bb = o->getBoundingBox();
   item->setText(7, toQString(bb.getSize()) );
   item->setText(8, toQString(bb.getMin()) );
   item->setText(9, toQString(bb.getMax()) );
   item->setText(10, QString::fromStdString( o->getName() ) );

   parent->addChild( item );
}

void
SceneEditor::populateSceneTree()
{
   auto & nodes = m_SceneManager->getSceneNodes();

   DE_DEBUG("SceneNode.Count = ", nodes.size())

   m_SceneTree->blockSignals( true );
   m_SceneTree->clear();
   m_SceneTree->setContentsMargins(0,0,0,0);

   for ( size_t n = 0; n < nodes.size(); ++n )
   {
      auto node = nodes[ n ];
      if ( !node ) continue;

      QString name = QString::fromStdString( dbGetFileName(node->getName()) );
      uint64_t nodeAddress = uint64_t( (void*)node );
      QTreeWidgetItem* tvNode = new QTreeWidgetItem();
      tvNode->setText(0, name );
      tvNode->setData(0, Qt::UserRole, QVariant( nodeAddress ) ); // Node address
      tvNode->setText(1, node->isVisible() ? "1" : "0" );
      tvNode->setData(1, Qt::UserRole, QVariant( -1 ) ); // Buffer index into SMesh

      //auto flags = tvNode->flags() | Qt::ItemIsEditable | Qt::ItemIsSelectable;
      //tvNode->setFlags( flags );

      if ( node->getNodeType() == de::gpu::SceneNodeType::Mesh )
      {
         auto snode = reinterpret_cast< de::gpu::SMeshSceneNode* >( node );
         if ( snode )
         {
            auto mesh = snode->getMesh();
            if ( mesh )
            {

               tvNode->setText(2, QString::number( mesh->getMeshBufferCount() ) );
               tvNode->setText(3, QString::number( mesh->getVertexCount() ) );
               tvNode->setText(4, QString::number( mesh->getIndexCount() ) );
               tvNode->setText(5, QString::fromStdString( mesh->getPrimTypesStr() ) );
               tvNode->setText(6, QString("0") );

               auto bb = mesh->getBoundingBox();
               tvNode->setText(7, toQString(bb.getSize()) );
               tvNode->setText(8, toQString(bb.getMin()) );
               tvNode->setText(9, toQString(bb.getMax()) );
               tvNode->setText(10, QString::fromStdString( mesh->getName() ) );

               for ( size_t i = 0; i < mesh->Buffers.size(); ++i )
               {
                  auto & buffer = mesh->Buffers[ i ];
                  addBufferTreeItem( tvNode, i, &buffer, nodeAddress );
               }
            }
         }
      }

      m_SceneTree->addTopLevelItem( tvNode );
   }

   m_SceneTree->blockSignals( false );

}




void
addTableItem( QTableWidget* table, int row, int col, QString text )
{
   QTableWidgetItem* item = new QTableWidgetItem();
   item->setText( text );
   table->setItem( row, col, item );
   //item->setFlags(item->flags() | Qt::ItemIsEditable | Qt::ItemIsSelectable );
   //item->setData(1, Qt::UserRole, QVariant(m_Mesh->isVisible()));
}


void
SceneEditor::populateMeshTable()
{
   if ( !m_SceneManager ) { DE_ERROR("No scene manager") return; }

   auto & meshCache = m_SceneManager->getMeshCache();

   DE_DEBUG("MeshCache.Size = ", meshCache.size())

   m_MeshTable->blockSignals( true );
   m_MeshTable->clearContents();
   m_MeshTable->setContentsMargins(0,0,0,0);

   auto font = m_MeshTable->font();
   font.setPointSize( 8 );
   m_MeshTable->setFont( font );
   m_MeshTable->setColumnCount( 9 );
   m_MeshTable->setRowCount( meshCache.size() );

   auto it = meshCache.begin();

   for ( size_t i = 0; i < meshCache.size(); ++i )
   {
      de::gpu::SMesh::SharedPtr spMesh = it->second;
      if ( !spMesh ) continue;
      de::gpu::SMesh const & mesh = *spMesh;
      // QString("SMesh[%1]").arg(0)
      addTableItem( m_MeshTable, i,0, QString::fromStdString( mesh.Name ) );
      addTableItem( m_MeshTable, i,1, QString::number( mesh.Visible ) );
      addTableItem( m_MeshTable, i,2, QString::number( mesh.Buffers.size() ) );
      auto bb = mesh.getBoundingBox();
      addTableItem( m_MeshTable, i,3, toQString(bb.getSize()) );
      addTableItem( m_MeshTable, i,4, toQString(bb.getMin()) );
      addTableItem( m_MeshTable, i,5, toQString(bb.getMax()) );

      addTableItem( m_MeshTable, i,6, QString::number( mesh.getVertexCount() ) );
      addTableItem( m_MeshTable, i,7, QString::number( mesh.getIndexCount() ) );
      addTableItem( m_MeshTable, i,8, QString::number( mesh.getByteCount() ) );

      it++;
   }

   m_MeshTable->blockSignals( false );
}

void
SceneEditor::populateBufferTable()
{
   m_BufferTable->blockSignals( true );
   m_BufferTable->clearContents();
   m_BufferTable->setContentsMargins(0,0,0,0);

   if ( m_Mesh )
   {
      DE_DEBUG("BufferCount = ", m_Mesh->Buffers.size())
      auto font = m_BufferTable->font();
      font.setPointSize( 8 );
      m_BufferTable->setFont( font );
      m_BufferTable->setColumnCount( 4 );

      m_BufferTable->setRowCount( m_Mesh->Buffers.size() );

      for ( size_t i = 0; i < m_Mesh->Buffers.size(); ++i )
      {
         de::gpu::SMeshBuffer const & buffer = m_Mesh->Buffers[ i ];
         addTableItem( m_BufferTable, i,0, QString::fromStdString( buffer.Name ) );
         addTableItem( m_BufferTable, i,1, QString::number( buffer.isVisible() ) );
         auto bb = buffer.getBoundingBox();
         addTableItem( m_BufferTable, i,3, toQString(bb.getSize()) );
         addTableItem( m_BufferTable, i,4, toQString(bb.getMin()) );
         addTableItem( m_BufferTable, i,5, toQString(bb.getMax()) );

         addTableItem( m_BufferTable, i,5, QString::fromStdString( buffer.getPrimitiveTypeStr() ) );
         addTableItem( m_BufferTable, i,6, QString::number( buffer.getPrimitiveCount() ) );
         addTableItem( m_BufferTable, i,7, QString::number( buffer.getVertexCount() ) );
         addTableItem( m_BufferTable, i,8, QString::number( buffer.getIndexCount() ) );
         addTableItem( m_BufferTable, i,9, QString::number( buffer.getByteCount() ) );
         addTableItem( m_BufferTable, i,10,QString::number( buffer.Material.getTextureCount() ) );
      }
   }

   m_BufferTable->blockSignals( false );
}



//void
//SceneEditor::removeMesh( std::string name )
//{
//   if ( !hasMesh( name ) ) { return; }

//   auto it = std::find_if( m_Meshes.begin(), m_Meshes.end(),
//         [&] ( de::gpu::SMesh const * const cached ) {
//            return cached && cached->getName() == name; } );

//   if ( it == m_Meshes.end() ) { return; }

//   m_Meshes.erase( it );
//}
