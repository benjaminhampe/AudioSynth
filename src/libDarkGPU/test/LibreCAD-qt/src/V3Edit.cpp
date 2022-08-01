#include "V3Edit.hpp"
#include <QDebug>
#include <QVBoxLayout>
#include <QWheelEvent>

V3Edit::V3Edit( QString txt, QWidget* parent )
   : QWidget( parent )
   , m_SceneNode( nullptr )
   , m_IsHovered( false )
   , m_IsFocused( false )
{
   setContentsMargins( 0,0,0,0 );
   setMouseTracking( true );
   m_Label = new QLabel( txt, this );
   m_X = new QDoubleSpinBox( this );
   m_Y = new QDoubleSpinBox( this );
   m_Z = new QDoubleSpinBox( this );
   m_Reset = new QPushButton( "x", this );

   m_Label->setMinimumSize( 50,16 );
   m_X->setMinimumSize( 32,16 );
   m_Y->setMinimumSize( 32,16 );
   m_Z->setMinimumSize( 32,16 );
   m_Reset->setMinimumSize( 12,16 );
   // m_Reset->setMaximumWidth( 48 );

   m_X->setMinimum( 0.000001 );
   m_Y->setMinimum( 0.000001 );
   m_Z->setMinimum( 0.000001 );
   m_X->setMaximum( 1.0e9 );
   m_Y->setMaximum( 1.0e9 );
   m_Z->setMaximum( 1.0e9 );

   QHBoxLayout* h = new QHBoxLayout();
   h->setContentsMargins( 5,0,5,0 );
   h->setSpacing(0);
   h->addWidget( m_Label,0 );
   h->addWidget( m_X,3 );
   h->addWidget( m_Y,3 );
   h->addWidget( m_Z,3 );
   h->addWidget( m_Reset,1 );

   setLayout( h );

   connect( m_X, SIGNAL(valueChanged(double)), this, SLOT(onSpinX(double)) );
   connect( m_Y, SIGNAL(valueChanged(double)), this, SLOT(onSpinY(double)) );
   connect( m_Z, SIGNAL(valueChanged(double)), this, SLOT(onSpinZ(double)) );
   connect( m_Reset, &QPushButton::clicked, this, &V3Edit::onBtnResetClicked );
}

V3Edit::~V3Edit()
{
}

void
V3Edit::setX( double x )
{
   m_X->blockSignals( true );
   m_X->setValue(x);
   m_X->blockSignals( false );
}
void
V3Edit::setY( double y )
{
   m_Y->blockSignals( true );
   m_Y->setValue(y);
   m_Y->blockSignals( false );
}

void
V3Edit::setZ( double z )
{
   m_Z->blockSignals( true );
   m_Z->setValue(z);
   m_Z->blockSignals( false );
}

void
V3Edit::onSpinX( double n )
{
   emit valueChanged( value() );
}

void
V3Edit::onSpinY( double n )
{
   emit valueChanged( value() );
}

void
V3Edit::onSpinZ( double n )
{
   emit valueChanged( value() );
}

void
V3Edit::onBtnResetClicked( bool clicked )
{
   DE_DEBUG("")
}

//void
//V3Edit::onMeshDoubleClick( int row, int col )
//{

//}

//void
//V3Edit::resizeEvent( QResizeEvent* event )
//{
//   QWidget::resizeEvent( event );
//}



void
V3Edit::focusInEvent( QFocusEvent* event)
{
   m_IsFocused = true;
   update();
}

void
V3Edit::focusOutEvent( QFocusEvent* event)
{
   m_IsFocused = false;
   update();
}

void
V3Edit::enterEvent( QEvent* event)
{
   m_IsHovered = true;
   update();
}

void
V3Edit::leaveEvent( QEvent* event)
{
   m_IsHovered = false;
   update();
}

void
V3Edit::keyPressEvent( QKeyEvent* event )
{
   QWidget::keyPressEvent( event );
}

void
V3Edit::keyReleaseEvent( QKeyEvent* event )
{
   QWidget::keyPressEvent( event );
}
void
V3Edit::wheelEvent( QWheelEvent* event )
{

//   if ( event->angleDelta().y() > 0)
//   {
//      int rowStart = getRowStart();
//      rowStart -= (2*m_RowCount) / 3;
//      setRowStart( rowStart );
//      update();
//   }
//   else if ( event->angleDelta().y() < 0)
//   {
//      int rowStart = getRowStart();
//      rowStart += (2*m_RowCount) / 3;
//      setRowStart( rowStart );
//      update();
//   }

   event->accept();
}


void
V3Edit::mouseMoveEvent( QMouseEvent* event )
{
   QWidget::mouseMoveEvent( event );
}

void
V3Edit::mousePressEvent( QMouseEvent* event )
{
   QWidget::mousePressEvent( event );
}

void
V3Edit::mouseReleaseEvent( QMouseEvent* event )
{
   QWidget::mouseReleaseEvent( event );
}

