#ifndef G_SLIDER_HPP
#define G_SLIDER_HPP

#include <QWidget>
#include <QImage>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QThread>
#include <QSlider>
#include <QDial>
#include <QLabel>

#include <QFont5x8.hpp>

/*
enum class eParamType // like amsynth now
{
   Linear = 0,
   Exponetial,
   Pow,
   eParamTypeCount
};

template< typename T >
struct Param
{
   typedef T value_type;

   Param()
      : m_type( eParamType::Linear )
      , m_id( -1 )
      , m_value( T(0.5) )
      , m_default( T(0.5) )
      , m_min( T(0) )
      , m_max( T(1) )
      , m_step( T(0.0001) )
   {}

   Param( std::string name, eParamType type, T defaultValue, T min = T(0), T max = T(1), std::string unit = "", T step = T(0.0001))
      : m_type( type )
      , m_id( -1 )
      , m_value( defaultValue )
      , m_default( defaultValue )
      , m_min( min )
      , m_max( max )
      , m_step( step )
      , m_name( name )
   {}

   T value() const { return m_value; }
   T min() const { return m_min; }
   T max() const { return m_max; }
   T range() const { return m_max - m_min; }
   T defaultValue() const { return m_default; }
   eParamType type() const { return m_type; }
   std::string const & name() const { return m_name; }

   void setValue( T value ) { m_value = value; }

   eParamType m_type;
   int m_id;
   T m_value;
   T m_default;
   T m_min;
   T m_max;
   T m_step;
   std::string m_name;
   std::string m_unit;
   std::string m_desc; // Tooltip Info

};

typedef Param< float > Paramf;
typedef Param< double > Paramd;
*/

// ============================================================================
class GSliderV : public QWidget
// ============================================================================
{
   Q_OBJECT
   DE_CREATE_LOGGER("GSliderV")
public:
   explicit
   GSliderV( QString name, int value, int min = 0, int max = 1, QString unit = "", int step = 1, QWidget* parent = 0 );
   ~GSliderV();

   int value() const { return m_slider->value(); }
   int minimum() const { return m_slider->minimum(); }
   int maximum() const { return m_slider->maximum(); }
   int singleStep() const { return m_slider->singleStep(); }
   QString name() const { return m_title->text(); }
   QString unit() const { return m_unit; }

signals:
   void valueChanged( int value );

public slots:
   void setValue(int v) { m_slider->setValue( v ); }
   void setMinimum(int v) { m_slider->setMinimum( v ); }
   void setMaximum(int v) { m_slider->setMaximum( v ); }
   void setSingleStep(int v) { m_slider->setSingleStep( v ); }
   void setName(QString title) { m_title->setText( title ); }
   void setUnit(QString unit) { m_unit = unit; }

private slots:
   void on_sliderChanged( int value );
   // void on_sliderPressed();
   // void on_sliderReleased();

protected:
   void timerEvent( QTimerEvent* event) override;
   void paintEvent( QPaintEvent* event ) override;
   void resizeEvent( QResizeEvent* event ) override;
   void keyPressEvent( QKeyEvent* event ) override;
   void keyReleaseEvent( QKeyEvent* event ) override;
   void mousePressEvent( QMouseEvent* event ) override;
   void mouseReleaseEvent( QMouseEvent* event ) override;
   void mouseMoveEvent( QMouseEvent* event ) override;
   void wheelEvent( QWheelEvent* event ) override;

private:
   QLabel* m_title;
   QSlider* m_slider;
   QLabel* m_amount;
   int m_default;
   QString m_unit;
   QString m_desc; // Tooltip Info
};

#endif // G_SLIDER_HPP
