#ifndef DE_GPU_V3_QWIDGET_HPP
#define DE_GPU_V3_QWIDGET_HPP

#include <DarkGPU.hpp>
#include <QWidget>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>

struct V3Edit : public QWidget
{
   Q_OBJECT
public:
   V3Edit( QString txt, QWidget* parent );
   ~V3Edit() override;
   double x() const { return m_X->value(); }
   double y() const { return m_Y->value(); }
   double z() const { return m_Z->value(); }
   glm::dvec3 value() const { return glm::dvec3( x(),y(),z() ); }

signals:
   void valueChanged( glm::dvec3 value );

public slots:
   void setX( double x );
   void setY( double y );
   void setZ( double z );

   void setValue( glm::dvec3 const & value )
   {
      setX( value.x );
      setY( value.y );
      setZ( value.z );
   }
   void setValue( double value )
   {
      setX( value );
      setY( value );
      setZ( value );
   }

   void setSingleStep( double value )
   {
      m_X->setSingleStep( value );
      m_Y->setSingleStep( value );
      m_Z->setSingleStep( value );
   }


   void setDecimals( int value )
   {
      m_X->setDecimals( value );
      m_Y->setDecimals( value );
      m_Z->setDecimals( value );
   }

   void setMinimumX( double x ) { m_X->setMinimum(x); }
   void setMinimumY( double y ) { m_Y->setMinimum(y); }
   void setMinimumZ( double z ) { m_Z->setMinimum(z); }
   void setMinimum( double value )
   {
      setMinimumX( value );
      setMinimumY( value );
      setMinimumZ( value );
   }
   void setMinimum( glm::dvec3 const & value )
   {
      setMinimumX( value.x );
      setMinimumY( value.y );
      setMinimumZ( value.z );
   }

   void setMaximumX( double x ) { m_X->setMaximum(x); }
   void setMaximumY( double y ) { m_Y->setMaximum(y); }
   void setMaximumZ( double z ) { m_Z->setMaximum(z); }
   void setMaximum( double value )
   {
      setMaximumX( value );
      setMaximumY( value );
      setMaximumZ( value );
   }

   void setMaximum( glm::dvec3 const & value )
   {
      setMaximumX( value.x );
      setMaximumY( value.y );
      setMaximumZ( value.z );
   }


   void setSceneNode( de::gpu::ISceneNode* node ) { m_SceneNode = node; }


private slots:
   void onSpinX( double value );
   void onSpinY( double value );
   void onSpinZ( double value );
   void onBtnResetClicked( bool clicked = false );

protected:
   void focusInEvent( QFocusEvent* event) override;
   void focusOutEvent( QFocusEvent* event) override;
   void enterEvent( QEvent* event) override;
   void leaveEvent( QEvent* event) override;
   void keyPressEvent( QKeyEvent* event ) override;
   void keyReleaseEvent( QKeyEvent* event ) override;
   void wheelEvent( QWheelEvent* event ) override;

   //void timerEvent( QTimerEvent* event) override;
   //void paintEvent( QPaintEvent* event ) override;
   //void resizeEvent( QResizeEvent* event ) override;
   void mousePressEvent( QMouseEvent* event ) override;
   void mouseReleaseEvent( QMouseEvent* event ) override;
   void mouseMoveEvent( QMouseEvent* event ) override;

protected:
   DE_CREATE_LOGGER("V3Edit")

   QLabel* m_Label;
   QDoubleSpinBox* m_X;
   QDoubleSpinBox* m_Y;
   QDoubleSpinBox* m_Z;
   QPushButton* m_Reset;

   bool m_IsHovered = false;
   bool m_IsFocused = false;

   de::gpu::ISceneNode* m_SceneNode;
};

#endif
