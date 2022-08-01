#ifndef DE_GPU_V4_COLOR_QWIDGET_HPP
#define DE_GPU_V4_COLOR_QWIDGET_HPP

#include <DarkGPU.hpp>
#include <QWidget>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>

struct V4ColorEdit : public QWidget
{
   Q_OBJECT
public:
   V4ColorEdit( QString txt, QWidget* parent );
   ~V4ColorEdit() override;

   int r() const { return m_R->value(); }
   int g() const { return m_G->value(); }
   int b() const { return m_B->value(); }
   int a() const { return m_A->value(); }
   uint32_t value() const { return de::RGBA( r(),g(),b(),a() ); }

signals:
   void colorChanged( uint32_t color );

public slots:

   void setValue( int r, int g, int b, int a )
   {
      m_R->setValue(r);
      m_G->setValue(g);
      m_B->setValue(b);
      m_A->setValue(a);
   }
   void setValue( uint32_t color )
   {
      setValue( de::RGBA_R( color ),
                de::RGBA_G( color ),
                de::RGBA_B( color ),
                de::RGBA_A( color ) );
   }

private slots:
   void onSpinR( int );
   void onSpinG( int );
   void onSpinB( int );
   void onSpinA( int );
   void onBtnClick( bool );

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
   DE_CREATE_LOGGER("V4ColorEdit")

   QPushButton* m_Btn;
   QSpinBox* m_R;
   QSpinBox* m_G;
   QSpinBox* m_B;
   QSpinBox* m_A;

   bool m_IsHovered = false;
   bool m_IsFocused = false;

};

#endif
