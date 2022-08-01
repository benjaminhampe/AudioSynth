#include "MeshTool.hpp"
#include <QGridLayout>
#include <QDebug>

QPushButton*
createButton( QString name, QString uri = "../../media-cad/FlipX.png", QWidget* parent = nullptr )
{
   QPushButton* btn = new QPushButton( name, parent );
   btn->setIcon( QIcon( uri ) );
   btn->setIconSize( QSize(32,32) );
   btn->setMinimumSize(32,32);
   return btn;
}

MeshTool::MeshTool( QWidget* parent )
   : QWidget( parent )
   , m_Canvas( nullptr )
   , m_Node( nullptr )
   , m_Mesh( nullptr )
   , m_FpsDisplayUpdateTimerId( 0 )
   , m_FrameTimeEdit( nullptr )
   , m_ScreenShotButton( nullptr )
{
   setContentsMargins( 4,0,0,0 );

   //m_Edit3DCylinder = new Edit3DSphere( nullptr );
   //m_Edit3DCylinder->hide();

   //m_Edit3DTube = new Edit3DSphere( nullptr );
   //m_Edit3DTube->hide();

   m_BtnSelMesh = createButton( "", "../../media-cad/SelMesh.png", this );
   m_BtnSelFace = createButton( "", "../../media-cad/SelFace.png", this );
   m_BtnSelVertex = createButton( "", "../../media-cad/SelVertex.png", this );

   m_BtnTranslate = createButton( "", "../../media-cad/TRS_T.png", this );
   m_BtnRotate = createButton( "", "../../media-cad/TRS_R.png", this );
   m_BtnScale = createButton( "", "../../media-cad/TRS_S.png", this );

   m_BtnAddPoint = createButton( "", "../../media-cad/AddPoint.png", this );
   m_BtnAddLine = createButton( "", "../../media-cad/AddLine.png", this );
   m_BtnAddTriangle = createButton( "", "../../media-cad/AddTriangle.png", this );
   m_BtnAddQuad = createButton( "", "../../media-cad/AddQuad.png", this );
   m_BtnAddCircle = createButton( "", "../../media-cad/AddCircle.png", this );
   m_BtnAddRing = createButton( "", "../../media-cad/AddRing.png", this );
   m_BtnAddRoundRect = createButton( "", "../../media-cad/AddRoundRect.png", this );
   m_BtnAddHexagon = createButton( "", "../../media-cad/AddHexagon.png", this );
   m_BtnAddPath = createButton( "", "../../media-cad/AddPath.png", this );

   m_BtnAdd3DBox = createButton( "", "../../media-cad/AddCube.png", this );
   m_BtnAddSphere = createButton( "", "../../media-cad/AddSphere.png", this );
   m_BtnAddCylinder = createButton( "", "../../media-cad/AddCylinder.png", this );
   m_BtnAddCone  = createButton( "", "../../media-cad/AddCone.png", this );
   m_BtnAddTube  = createButton( "", "../../media-cad/AddTube.png", this );
   m_BtnAddTorus = createButton( "", "../../media-cad/AddTorus.png", this );

   m_BtnFlipX = createButton( "", "../../media-cad/FlipX.png", this );
   m_BtnFlipY = createButton( "", "../../media-cad/FlipY.png", this );
   m_BtnFlipZ = createButton( "", "../../media-cad/FlipZ.png", this );
   m_BtnFlipXY = createButton( "", "../../media-cad/FlipXY.png", this );
   m_BtnFlipYZ = createButton( "", "../../media-cad/FlipYZ.png", this );
   m_BtnFlipXZ = createButton( "", "../../media-cad/FlipXZ.png", this );
   m_BtnFlipNormals = createButton( "", "../../media-cad/FlipNormals.png", this );

   m_Edit3DBox = new Edit3DBox( nullptr );
   m_Edit3DBox->hide();
   m_BtnAdd3DBox->setContextMenuPolicy( Qt::CustomContextMenu );

   m_Edit3DSphere = new Edit3DSphere( nullptr );
   m_Edit3DSphere->hide();
   m_BtnAddSphere->setContextMenuPolicy( Qt::CustomContextMenu );

   QGridLayout* toolayout = new QGridLayout( this );
   toolayout->setContentsMargins( 0,0,0,0 );
   toolayout->setHorizontalSpacing(0);
   toolayout->setVerticalSpacing(0);

   int k=0;
   toolayout->addWidget( new QLabel("Select Modes:"), k, 0, 1, 3 ); k++;
   toolayout->addWidget( m_BtnSelMesh, k, 0, 1, 1 );
   toolayout->addWidget( m_BtnSelFace, k, 1, 1, 1 );
   toolayout->addWidget( m_BtnSelVertex, k, 2, 1, 1 ); k++;

   toolayout->addWidget( new QLabel("TRS:"), k, 0, 1, 3 ); k++;
   toolayout->addWidget( m_BtnTranslate, k, 0, 1, 1 );
   toolayout->addWidget( m_BtnRotate, k, 1, 1, 1 );
   toolayout->addWidget( m_BtnScale, k, 2, 1, 1 ); k++;

   toolayout->addWidget( new QLabel("2D Shapes:"), k, 0, 1, 3 ); k++;
   toolayout->addWidget( m_BtnAddPoint, k, 0, 1, 1 );
   toolayout->addWidget( m_BtnAddLine, k, 1, 1, 1 );
   toolayout->addWidget( m_BtnAddTriangle, k, 2, 1, 1 ); k++;
   toolayout->addWidget( m_BtnAddQuad, k, 0, 1, 1 );
   toolayout->addWidget( m_BtnAddCircle, k, 1, 1, 1 );
   toolayout->addWidget( m_BtnAddRing, k, 2, 1, 1 ); k++;
   toolayout->addWidget( m_BtnAddRoundRect, k, 0, 1, 1 );
   toolayout->addWidget( m_BtnAddHexagon, k, 1, 1, 1 );
   toolayout->addWidget( m_BtnAddPath, k, 2, 1, 1 ); k++;

   toolayout->addWidget( new QLabel("3D Shapes:"), k, 0, 1, 3 ); k++;
   toolayout->addWidget( m_BtnAdd3DBox, k, 0, 1, 1 );
   toolayout->addWidget( m_BtnAddSphere, k, 1, 1, 1 );
   toolayout->addWidget( m_BtnAddCylinder, k, 2, 1, 1 ); k++;
   toolayout->addWidget( m_BtnAddCone, k, 0, 1, 1 );
   toolayout->addWidget( m_BtnAddTube, k, 1, 1, 1 );
   toolayout->addWidget( m_BtnAddTorus, k, 2, 1, 1 ); k++;

   toolayout->addWidget( new QLabel("Flip and Rotate 90:"), k, 0, 1, 3 ); k++;
   toolayout->addWidget( m_BtnFlipNormals, k, 0, 1, 3 ); k++;
   toolayout->addWidget( m_BtnFlipX, k, 0, 1, 1 );
   toolayout->addWidget( m_BtnFlipY, k, 1, 1, 1 );
   toolayout->addWidget( m_BtnFlipZ, k, 2, 1, 1 ); k++;
   toolayout->addWidget( m_BtnFlipXY, k, 0, 1, 1 );
   toolayout->addWidget( m_BtnFlipYZ, k, 1, 1, 1 );
   toolayout->addWidget( m_BtnFlipXZ, k, 2, 1, 1 ); k++;

   QWidget* toolPanel = new QWidget( this );
   toolPanel->setContentsMargins( 0,0,0,0 );
   toolPanel->setLayout( toolayout );

   m_FpsDisplay = new QLabel("0", this);
   m_FrameTimeEdit = new QSpinBox( this );
   m_FrameTimeEdit->setMinimumWidth( 32 );
   m_FrameTimeEdit->setMinimum( 1 );
   m_FrameTimeEdit->setMaximum( 20000 );
   m_FrameTimeEdit->setValue( 30 );
   m_ScreenShotButton = new QPushButton( "Take it!", this );

   int row = 0;
   QGridLayout* layout = new QGridLayout( this );
   layout->setContentsMargins( 0,0,0,0 );
   layout->addWidget( toolPanel, row, 0, 3, 2 ); row++; row++; row++;
   layout->addWidget( new QLabel( "FPS:", this ), row, 0, 1, 1 );
   layout->addWidget( m_FpsDisplay, row, 1, 1, 1 ); row++;
   layout->addWidget( new QLabel( "Timer [ms]:", this ), row, 0, 1, 1 );
   layout->addWidget( m_FrameTimeEdit, row, 1, 1, 1 ); row++;
   layout->addWidget( new QLabel( "Screen:", this ), row, 0, 1, 1 );
   layout->addWidget( m_ScreenShotButton, row, 1, 1, 1 ); row++;
   setLayout( layout );

   connect( m_FrameTimeEdit, SIGNAL(valueChanged(int)), this, SLOT(onSetFpsTimer(int)) );
   m_FpsDisplayUpdateTimerId = startTimer( 200 );

   connect( m_BtnSelMesh, &QPushButton::clicked, this, &MeshTool::onBtnSelMesh );
   connect( m_BtnSelFace, &QPushButton::clicked, this, &MeshTool::onBtnSelFace );
   connect( m_BtnSelVertex, &QPushButton::clicked, this, &MeshTool::onBtnSelVertex );

   connect( m_BtnTranslate, &QPushButton::clicked, this, &MeshTool::onBtnTRS_Move );
   connect( m_BtnRotate, &QPushButton::clicked, this, &MeshTool::onBtnTRS_Rotate );
   connect( m_BtnScale, &QPushButton::clicked, this, &MeshTool::onBtnTRS_Scale );

   connect( m_BtnAddCircle, &QPushButton::clicked, this, &MeshTool::onBtnAddCircle );
   connect( m_BtnAddCone, &QPushButton::clicked, this, &MeshTool::onBtnAddCone );
   connect( m_BtnAdd3DBox, &QPushButton::clicked, this, &MeshTool::onBtnAddBox );

   connect( m_BtnAddCylinder, &QPushButton::clicked, this, &MeshTool::onBtnAddCylinder );
   connect( m_BtnAddHexagon, &QPushButton::clicked, this, &MeshTool::onBtnAddHexagon );
   connect( m_BtnAddLine, &QPushButton::clicked, this, &MeshTool::onBtnAddLine );
   connect( m_BtnAddPath, &QPushButton::clicked, this, &MeshTool::onBtnAddPath );
   connect( m_BtnAddPoint, &QPushButton::clicked, this, &MeshTool::onBtnAddPoint );
   connect( m_BtnAddQuad, &QPushButton::clicked, this, &MeshTool::onBtnAddQuad );
   connect( m_BtnAddRing, &QPushButton::clicked, this, &MeshTool::onBtnAddRing );
   connect( m_BtnAddRoundRect, &QPushButton::clicked, this, &MeshTool::onBtnAddRoundRect );
   connect( m_BtnAddSphere, &QPushButton::clicked, this, &MeshTool::onBtnAddSphere );
   connect( m_BtnAddTorus, &QPushButton::clicked, this, &MeshTool::onBtnAddTorus );
   connect( m_BtnAddTriangle, &QPushButton::clicked, this, &MeshTool::onBtnAddTriangle );
   connect( m_BtnAddTube, &QPushButton::clicked, this, &MeshTool::onBtnAddTube );

   connect( m_BtnFlipNormals, &QPushButton::clicked,this, &MeshTool::onBtnFlipMeshNormals );
   connect( m_BtnFlipX, &QPushButton::clicked,this, &MeshTool::onBtnFlipMeshX );
   connect( m_BtnFlipY, &QPushButton::clicked,this, &MeshTool::onBtnFlipMeshY );
   connect( m_BtnFlipZ, &QPushButton::clicked,this, &MeshTool::onBtnFlipMeshZ );
   connect( m_BtnFlipXY, &QPushButton::clicked,this, &MeshTool::onBtnFlipMeshXY );
   connect( m_BtnFlipXZ, &QPushButton::clicked,this, &MeshTool::onBtnFlipMeshXZ );
   connect( m_BtnFlipYZ, &QPushButton::clicked,this, &MeshTool::onBtnFlipMeshYZ );

   connect( m_BtnAdd3DBox, &QPushButton::customContextMenuRequested,
                     this, &MeshTool::ps_Box3D_customContextMenuRequested );

   connect( m_BtnAddSphere, &QPushButton::customContextMenuRequested,
                     this, &MeshTool::ps_Sphere3D_customContextMenuRequested );

   connect( m_Edit3DBox, &Edit3DBox::meshCreated, this, &MeshTool::emit_meshCreated );
   connect( m_Edit3DSphere, &Edit3DSphere::meshCreated, this, &MeshTool::emit_meshCreated );

}

MeshTool::~MeshTool()
{
   if ( m_FpsDisplayUpdateTimerId != 0 )
   {
      killTimer( m_FpsDisplayUpdateTimerId );
      m_FpsDisplayUpdateTimerId = 0;
   }

   delete m_Edit3DBox;
   delete m_Edit3DSphere;
}

void
MeshTool::emit_meshCreated( de::gpu::SMesh::SharedPtr mesh )
{
   emit meshCreated( mesh );
}

void
MeshTool::ps_Box3D_customContextMenuRequested( QPoint const & pos )
{
   m_Edit3DBox->show();
   m_Edit3DBox->move( dbDesktopWidth()/2, dbDesktopHeight()/2 );
   m_Edit3DBox->raise();
   m_Edit3DBox->activateWindow();
}

void
MeshTool::ps_Sphere3D_customContextMenuRequested( QPoint const & pos )
{
   m_Edit3DSphere->show();
   m_Edit3DSphere->move( dbDesktopWidth()/2, dbDesktopHeight()/2 );
   m_Edit3DSphere->raise();
   m_Edit3DSphere->activateWindow();
}

void
MeshTool::timerEvent( QTimerEvent* e )
{
   if ( e->timerId() == m_FpsDisplayUpdateTimerId )
   {
      updateFpsLabel();
   }
}

void
MeshTool::setSceneNode(de::gpu::ISceneNode* node)
{
   m_Node = node;
   if ( !node ) return;
   if ( node->getNodeType() != de::gpu::SceneNodeType::Mesh ) return;
   auto snode = reinterpret_cast< de::gpu::SMeshSceneNode* >( node );

}

void
MeshTool::setMesh( de::gpu::SMesh::SharedPtr mesh )
{
   m_Mesh = mesh;
}

void
MeshTool::showEditorAddBox3D()
{
   m_Edit3DBox->show();
}

void
MeshTool::onBtnSelMesh( bool checked )
{
   DE_DEBUG("")
//   if ( !m_Mesh ) { DE_ERROR("No mesh") return; }
//   de::gpu::SMeshTool::translateVertices( *m_Mesh, 100, 0,0);
//   m_Mesh->upload();
}
void
MeshTool::onBtnSelFace( bool checked )
{
   DE_DEBUG("")
//   if ( !m_Mesh ) { DE_ERROR("No mesh") return; }
//   de::gpu::SMeshTool::translateVertices( *m_Mesh, 100, 0,0);
//   m_Mesh->upload();
}
void
MeshTool::onBtnSelVertex( bool checked )
{
   DE_DEBUG("")
//   if ( !m_Mesh ) { DE_ERROR("No mesh") return; }
//   de::gpu::SMeshTool::translateVertices( *m_Mesh, 100, 0,0);
//   m_Mesh->upload();
}

void
MeshTool::onBtnTRS_Move( bool checked )
{
   if ( !m_Node ) { DE_ERROR("No node") return; }
   auto pos = m_Node->getPosition();
   pos.x += 100.0;
   pos.z += 100.0;
   m_Node->setPosition( pos );

//   if ( !node ) return;
//   if ( node->getNodeType() != de::gpu::SceneNodeType::Mesh ) return;
//   auto snode = reinterpret_cast< de::gpu::SMeshSceneNode* >( node );

//   if ( !m_Mesh ) { DE_ERROR("No mesh") return; }
//   de::gpu::SMeshTool::translateVertices( *m_Mesh, 100, 0,0);
//   m_Mesh->upload();

}
void
MeshTool::onBtnTRS_Rotate( bool checked )
{
   if ( !m_Node ) { DE_ERROR("No node") return; }
   auto euler = m_Node->getRotation();
   euler.x += 30.0;
   if ( euler.x > 360.0 ) euler.x -= 360.0;
   m_Node->setRotation( euler );

//   if ( !node ) return;
//   if ( node->getNodeType() != de::gpu::SceneNodeType::Mesh ) return;
//   auto snode = reinterpret_cast< de::gpu::SMeshSceneNode* >( node );

//   if ( !m_Mesh ) { DE_ERROR("No mesh") return; }
//   de::gpu::SMeshTool::rotateVertices( *m_Mesh, 10,0,0);
//   m_Mesh->upload();
}
void
MeshTool::onBtnTRS_Scale( bool checked )
{
   if ( !m_Node ) { DE_ERROR("No node") return; }
   m_Node->setScale( m_Node->getScale() * 1.5 );

//   if ( !m_Mesh ) { DE_ERROR("No mesh") return; }
//   de::gpu::SMeshTool::rotateVertices( *m_Mesh, 1.1,1.1,1.1);
//   m_Mesh->upload();
}


void
MeshTool::onBtnFlipMeshNormals( bool checked )
{
//   DE_DEBUG("")
//   if ( !m_Mesh ) { DE_ERROR("No mesh") return; }
//   de::gpu::SMeshTool::flipNormals( *m_Mesh );
//   m_Mesh->upload();
}

void
MeshTool::onBtnFlipMeshX( bool checked )
{
   DE_DEBUG("")
   if ( !m_Node ) { DE_ERROR("No node") return; }
   auto scale = m_Node->getScale();
   scale.x = -scale.x;
   m_Node->setScale( scale );
//   if ( !m_Mesh ) { DE_ERROR("No mesh") return; }
//   de::gpu::SMeshTool::flipMeshX( *m_Mesh );
//   m_Mesh->upload();
}

void
MeshTool::onBtnFlipMeshY( bool checked )
{
   if ( !m_Node ) { DE_ERROR("No node") return; }
   auto scale = m_Node->getScale();
   scale.y = -scale.y;
   m_Node->setScale( scale );
//   if ( !m_Mesh ) { DE_ERROR("No mesh") return; }
//   de::gpu::SMeshTool::flipMeshY( *m_Mesh );
//   m_Mesh->upload();
}

void
MeshTool::onBtnFlipMeshZ( bool checked )
{
   if ( !m_Node ) { DE_ERROR("No node") return; }
   auto scale = m_Node->getScale();
   scale.x = -scale.x;
   m_Node->setScale( scale );
//   if ( !m_Mesh ) { DE_ERROR("No mesh") return; }
//   de::gpu::SMeshTool::flipMeshZ( *m_Mesh );
//   m_Mesh->upload();
}

void
MeshTool::onBtnFlipMeshXY( bool checked )
{
   if ( !m_Node ) { DE_ERROR("No node") return; }
   auto euler = m_Node->getRotation();
   euler.z += 90.0f;
   if ( euler.z > 360.0f ) euler.z -= 360.0f;
   m_Node->setRotation( euler );

//   if ( !m_Mesh ) { DE_ERROR("No mesh") return; }
//   de::gpu::SMeshTool::flipMeshXY( *m_Mesh );
//   m_Mesh->upload();
}

void
MeshTool::onBtnFlipMeshYZ( bool checked )
{
   if ( !m_Node ) { DE_ERROR("No node") return; }
   auto euler = m_Node->getRotation();
   euler.x += 90.0f;
   if ( euler.x > 360.0f ) euler.x -= 360.0f;
   m_Node->setRotation( euler );

//   if ( !m_Mesh ) { DE_ERROR("No mesh") return; }
//   de::gpu::SMeshTool::flipMeshYZ( *m_Mesh );
//   m_Mesh->upload();
}

void
MeshTool::onBtnFlipMeshXZ( bool checked )
{
   if ( !m_Node ) { DE_ERROR("No node") return; }
   auto euler = m_Node->getRotation();
   euler.y += 90.0f;
   if ( euler.y > 360.0f ) euler.y -= 360.0f;
   m_Node->setRotation( euler );

//   if ( !m_Mesh ) { DE_ERROR("No mesh") return; }
//   de::gpu::SMeshTool::flipMeshXZ( *m_Mesh );
//   m_Mesh->upload();
}

void
MeshTool::onBtnAddBox( bool checked )
{
   DE_DEBUG("")
   auto mesh = de::gpu::SMesh::createShared("Box");
   mesh->addEmptyBuffer();
   de::gpu::smesh::Box::add( mesh->Buffers[0], glm::vec3(100,100,100) );
   mesh->recalculateBoundingBox();
   mesh->setLighting( 1 );
   setMesh( mesh );
   emit meshCreated( mesh );
}
void
MeshTool::onBtnAddSphere( bool checked )
{
   DE_DEBUG("")
   auto mesh = de::gpu::SMesh::createShared("Sphere");
   mesh->addEmptyBuffer();
   de::gpu::smesh::Sphere::add( mesh->Buffers[0], glm::vec3(100,100,100), 24,24 );
   mesh->recalculateBoundingBox();
   mesh->setLighting( 1 );
   setMesh( mesh );
   emit meshCreated( mesh );
}
void
MeshTool::onBtnAddCone( bool checked )
{
   DE_DEBUG("")
   auto mesh = de::gpu::SMesh::createShared("Cone");
   mesh->addEmptyBuffer();
   de::gpu::smesh::Cone::addXZ( mesh->Buffers[0], glm::vec3(60,100,60), 0xFFFFFFFF, 0xFFFFFFFF, 12,3  );
   mesh->recalculateBoundingBox();
   mesh->setLighting( 1 );
   setMesh( mesh );
   emit meshCreated( mesh );
}
void
MeshTool::onBtnAddCylinder( bool checked )
{
   DE_DEBUG("")
   auto mesh = de::gpu::SMesh::createShared("Cylinder");
   mesh->addEmptyBuffer();
   de::gpu::smesh::Cylinder::addXZ( mesh->Buffers[0], 60,100,60 );
   mesh->recalculateBoundingBox();
   mesh->setLighting( 1 );
   setMesh( mesh );
   emit meshCreated( mesh );
}
void
MeshTool::onBtnAddPyramide( bool checked )
{
   DE_DEBUG("")
   auto mesh = de::gpu::SMesh::createShared("Pyramide");
   mesh->addEmptyBuffer();
   de::gpu::smesh::Cylinder::addXZ( mesh->Buffers[0], 60,100,60 );
   mesh->recalculateBoundingBox();
   mesh->setLighting( 1 );
   setMesh( mesh );
   emit meshCreated( mesh );
}
void
MeshTool::onBtnAddTube( bool checked )
{
   DE_DEBUG("")
   auto mesh = de::gpu::SMesh::createShared("Tube");
   mesh->addEmptyBuffer();
   de::gpu::smesh::Tube::add( mesh->Buffers[0], 100,100,100,20,20 );
   mesh->recalculateBoundingBox();
   mesh->setLighting( 1 );
   setMesh( mesh );
   emit meshCreated( mesh );

}
void
MeshTool::onBtnAddTorus( bool checked )
{
   DE_DEBUG("")
   auto mesh = de::gpu::SMesh::createShared("Torus");
   mesh->addEmptyBuffer();
   de::gpu::smesh::Cylinder::addXZ( mesh->Buffers[0], 60,100,60 );
   mesh->recalculateBoundingBox();
   mesh->setLighting( 1 );
   setMesh( mesh );
   emit meshCreated( mesh );
}


void
MeshTool::onBtnAddPoint( bool checked )
{
   auto mesh = de::gpu::SMesh::createShared("Dot");
   mesh->addEmptyBuffer();
   de::gpu::smesh::Line::add( mesh->Buffers[0], {60,120,60}, {260,180,20}, 0xFF00DFDF, 0xFF007FDF );
   emit meshCreated( mesh );
}

void
MeshTool::onBtnAddLine( bool checked )
{
   DE_DEBUG("")
   auto mesh = de::gpu::SMesh::createShared("Line");
   mesh->addEmptyBuffer();
   de::gpu::smesh::Line::add( mesh->Buffers[0], {60,120,60}, {260,180,20}, 0xFF00DFDF, 0xFF007FDF );
   emit meshCreated( mesh );
}

void
MeshTool::onBtnAddTriangle( bool checked )
{
   DE_DEBUG("")
   auto mesh = de::gpu::SMesh::createShared("Triangle");
   mesh->addEmptyBuffer();
   de::gpu::smesh::Triangle::add( mesh->Buffers[0], {60,120,60}, {260,180,20}, {240,100,10}, 0xFF00DFDF, 0xFF007FDF, 0xFF7FDF70 );
   emit meshCreated( mesh );
}

void
MeshTool::onBtnAddQuad( bool checked )
{
   DE_DEBUG("")
   auto mesh = de::gpu::SMesh::createShared("Quad");
   mesh->addEmptyBuffer();
   de::gpu::smesh::Quad::add( mesh->Buffers[0], {20,120,20}, {260,180,40}, {240,100,110}, {340,20,10}, 0xFFFFFFFF, 0xFF007FDF, 0xFF7FDF70, 0xFFDF7F70 );
   emit meshCreated( mesh );
}

void
MeshTool::onBtnAddHexagon( bool checked )
{
   DE_DEBUG("")
   auto mesh = de::gpu::SMesh::createShared("Hexagon");
   mesh->addEmptyBuffer();
   de::gpu::smesh::Hexagon::addXZ( mesh->Buffers[0], 100,100, 0xFFFFFFFF );
   emit meshCreated( mesh );
}

void
MeshTool::onBtnAddCircle( bool checked )
{
   DE_DEBUG("")
   auto mesh = de::gpu::SMesh::createShared("Circle");
   mesh->addEmptyBuffer();
   de::gpu::smesh::Circle::addXZ( mesh->Buffers[0], {200,200}, 0xFF7FDF70, 33 );
   emit meshCreated( mesh );
}

void
MeshTool::onBtnAddRing( bool checked )
{
   DE_DEBUG("")
   auto mesh = de::gpu::SMesh::createShared("Ring");
   mesh->addEmptyBuffer();
   de::gpu::smesh::Ring::addXZ( mesh->Buffers[0], 200,200,150,150, 0xFF7FDF70, 0xFF007FDF, 33 );
   emit meshCreated( mesh );
}


void
MeshTool::onBtnAddRoundRect( bool checked )
{
   DE_DEBUG("")
}

void
MeshTool::onBtnAddPath( bool checked )
{
   DE_DEBUG("")
}


void
MeshTool::setCanvas3D( Canvas3D* canvas )
{
   m_Canvas = canvas;
   if ( !m_Canvas ) return;
   connect( m_Canvas, SIGNAL(frameFinished()), this, SLOT(updateFpsLabel()) );

   updateFpsLabel();
}

void
MeshTool::onSetFpsTimer( int time_per_frame_in_ms )
{
   if ( !m_Canvas ) return;
   m_Canvas->setFpsTimer( time_per_frame_in_ms );
}

void
MeshTool::updateFpsLabel()
{
   if ( !m_Canvas ) return;
   de::gpu::IVideoDriver* driver = m_Canvas->getVideoDriver();
   if ( !driver ) return;
   double fps = driver->getFPS();
   m_FpsDisplay->setText( QString::number( fps ) );
}

void
MeshTool::updateCanvasInfoLabel()
{
   if ( !m_Canvas ) return;
   de::gpu::IVideoDriver* driver = m_Canvas->getVideoDriver();
   if ( !driver ) return;

/*
   int w = driver->getScreenWidth();
   int h = driver->getScreenHeight();

   std::stringstream s; s <<
   "Screen(" << w << "," << h << "), "
   "Time(" << driver->getTimer() << "), "
   "FPS(" << driver->getFPS() << "), "
   "FrameCount(" << driver->getFrameCount() << "), "
   "CamPos(" << glm::ivec3( driver->getCameraPos() ) << "), "
   "X(" << driver->getCameraVec1() << "), "
   "Y(" << driver->getCameraVec2() << "), "
   "Z(" << driver->getCameraVec3() << ")"
   //"CamDir(" << cam.getDir() << ")";
   // "CamAng(" << glm::ivec3(cam.getRotationDEG()) << "), ";
   ;
   m_FpsDisplay->setText( QString::fromStdString(s.str()) );
*/
   m_FpsDisplay->setText( QString::number( driver->getFPS() ) );
}

void
MeshTool::saveScreenShot()
{
   if ( !m_Canvas ) return;
   de::gpu::IVideoDriver* driver = m_Canvas->getVideoDriver();
   if ( !driver ) return;

   de::Image screenShot;
   if ( driver->createScreenShot( screenShot ) )
   {
      bool ok = dbSaveImage( screenShot, "MeshTool.png" );
      if ( !ok )
      {
         DE_ERROR("Screenshot not saved ")
      }
   }
}
