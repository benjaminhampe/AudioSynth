#include "V4ColorEdit.hpp"
#include <QDebug>
#include <QVBoxLayout>
#include <QWheelEvent>

V4ColorEdit::V4ColorEdit( QString txt, QWidget* parent )
   : QWidget( parent )
   , m_IsHovered( false )
   , m_IsFocused( false )
{
   setContentsMargins( 0,0,0,0 );
   setMouseTracking( true );
   m_Btn = new QPushButton( txt, this );
   m_R = new QSpinBox( this );
   m_G = new QSpinBox( this );
   m_B = new QSpinBox( this );
   m_A = new QSpinBox( this );

   m_Btn->setMinimumSize( 16,16 );
   m_R->setMinimumSize( 32,16 );
   m_G->setMinimumSize( 32,16 );
   m_B->setMinimumSize( 32,16 );
   m_A->setMinimumSize( 32,16 );
   // m_Reset->setMaximumWidth( 48 );

   m_R->setMinimum( 0 );
   m_G->setMinimum( 0 );
   m_B->setMinimum( 0 );
   m_A->setMinimum( 0 );

   m_R->setMaximum( 255 );
   m_G->setMaximum( 255 );
   m_B->setMaximum( 255 );
   m_A->setMaximum( 255 );

   m_R->setValue( 255 );
   m_G->setValue( 255 );
   m_B->setValue( 255 );
   m_A->setValue( 255 );

   QHBoxLayout* h = new QHBoxLayout();
   h->setContentsMargins( 5,0,5,0 );
   h->setSpacing(0);
   h->addWidget( m_Btn );
   h->addWidget( m_R,3 );
   h->addWidget( m_G,3 );
   h->addWidget( m_B,3 );
   h->addWidget( m_A,3 );

   setLayout( h );

   connect( m_R, SIGNAL(valueChanged(int)), this, SLOT(onSpinR(int)) );
   connect( m_G, SIGNAL(valueChanged(int)), this, SLOT(onSpinG(int)) );
   connect( m_B, SIGNAL(valueChanged(int)), this, SLOT(onSpinB(int)) );
   connect( m_A, SIGNAL(valueChanged(int)), this, SLOT(onSpinA(int)) );
   connect( m_Btn, &QPushButton::clicked, this, &V4ColorEdit::onBtnClick );
}

V4ColorEdit::~V4ColorEdit()
{
}

void
V4ColorEdit::onSpinR( int n )
{
   emit colorChanged( value() );
}

void
V4ColorEdit::onSpinG( int n )
{
   emit colorChanged( value() );
}

void
V4ColorEdit::onSpinB( int n )
{
   emit colorChanged( value() );
}

void
V4ColorEdit::onSpinA( int n )
{
   emit colorChanged( value() );
}

void
V4ColorEdit::onBtnClick( bool checked )
{
   DE_DEBUG("")
}

//void
//V4ColorEdit::onMeshDoubleClick( int row, int col )
//{

//}

//void
//V4ColorEdit::resizeEvent( QResizeEvent* event )
//{
//   QWidget::resizeEvent( event );
//}



void
V4ColorEdit::focusInEvent( QFocusEvent* event)
{
   m_IsFocused = true;
   update();
}

void
V4ColorEdit::focusOutEvent( QFocusEvent* event)
{
   m_IsFocused = false;
   update();
}

void
V4ColorEdit::enterEvent( QEvent* event)
{
   m_IsHovered = true;
   update();
}

void
V4ColorEdit::leaveEvent( QEvent* event)
{
   m_IsHovered = false;
   update();
}

void
V4ColorEdit::keyPressEvent( QKeyEvent* event )
{
   QWidget::keyPressEvent( event );
}

void
V4ColorEdit::keyReleaseEvent( QKeyEvent* event )
{
   QWidget::keyPressEvent( event );
}
void
V4ColorEdit::wheelEvent( QWheelEvent* event )
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
V4ColorEdit::mouseMoveEvent( QMouseEvent* event )
{
   QWidget::mouseMoveEvent( event );
}

void
V4ColorEdit::mousePressEvent( QMouseEvent* event )
{
   QWidget::mousePressEvent( event );
}

void
V4ColorEdit::mouseReleaseEvent( QMouseEvent* event )
{
   QWidget::mouseReleaseEvent( event );
}

