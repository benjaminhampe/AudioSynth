#include "Edit3DBox.hpp"
#include <QDebug>
#include <QVBoxLayout>
#include <QWheelEvent>

Edit3DBox::Edit3DBox( QWidget* parent )
   : QWidget( parent )
   , m_SceneNode( nullptr )
   , m_IsHovered( false )
   , m_IsFocused( false )
{
   setContentsMargins( 0,0,0,0 );
   m_Label = new QLabel( "Create 3D (sky)box", this );
   m_Size = new V3Edit( "Size:", this );
   m_Size->setValue( 10.0 );

   m_Color = new V4ColorEdit("VertexColor:", this );

   m_BtnCreate = new QPushButton( "Create", this );
   m_BtnHide = new QPushButton( "Hide window", this );

   QHBoxLayout* h = new QHBoxLayout();
   h->setContentsMargins( 0,0,0,0 );
   h->setSpacing(0);
   h->addWidget( m_BtnCreate );
   h->addWidget( m_BtnHide );

   QVBoxLayout* v = new QVBoxLayout();
   v->setContentsMargins( 0,0,0,0 );
   v->setSpacing(0);
   v->addWidget( m_Label );
   v->addWidget( m_Size );
   v->addWidget( m_Color );
   v->addLayout( h );
   setLayout( v );

   connect( m_BtnHide, &QPushButton::clicked, this, &Edit3DBox::onBtnHide );
   connect( m_BtnCreate, &QPushButton::clicked, this, &Edit3DBox::onBtnCreate );
   //connect( this, &QPushButton::clicked, this, &Edit3DBox::onBtnCreate );
   //connect( m_Size, SIGNAL(valueChanged(glm::dvec3))()), this, SLOT(onXChanged(double)) );
   //connect( m_Y, SIGNAL(valueChanged(double)), this, SLOT(onYChanged(double)) );
   //connect( m_Z, SIGNAL(valueChanged(double)), this, SLOT(onZChanged(double)) );
}

Edit3DBox::~Edit3DBox()
{
}

void
Edit3DBox::closeEvent( QCloseEvent* event )
{
   hide();
}

void
Edit3DBox::onBtnHide( bool clicked )
{
   hide();
}

void
Edit3DBox::onBtnCreate( bool clicked )
{
   DE_DEBUG("")
   auto mesh = de::gpu::SMesh::createShared("BoxDlg");
   mesh->addEmptyBuffer();
   de::gpu::smesh::Box::add( mesh->Buffers[0], m_Size->value() );

   de::gpu::SMeshBufferTool::colorVertices( mesh->Buffers[0], m_Color->value() );

   mesh->recalculateBoundingBox();
   mesh->setLighting( 2 );
   emit meshCreated( mesh );
}
