#ifndef PAINT_COLORBUTTON_HPP
#define PAINT_COLORBUTTON_HPP

#include <QPushButton>
#include <QString>
#include <QWidget>
#include <QMouseEvent>
#include <QColor>

class ColorButton : public QPushButton
{
   Q_OBJECT
public:
   ColorButton( QString name, uint32_t colorRGBA, QWidget * parent = nullptr );
   ~ColorButton() override;
   QColor const & getColor() const { return m_Color; }
signals:
   void fgcolorChanged( QColor );
   void bgcolorChanged( QColor );

public slots:
   void emit_fgcolor();
   void emit_bgcolor();
   void setColor( QColor color );
   void setColorRGBA( uint32_t colorRGBA );
protected:
   void updateButtonImage();
   void mouseDoubleClickEvent( QMouseEvent * me ) override;
   void mousePressEvent( QMouseEvent * me ) override;
   void mouseReleaseEvent( QMouseEvent * me ) override;
private:
   QColor m_Color;
   uint32_t m_Index;
};

#endif
