#ifndef DE_Q_VERTICAL_TEXT_HPP
#define DE_Q_VERTICAL_TEXT_HPP

#include "QImageWidget.hpp"
#include <QPushButton>
#include <QFont5x8.hpp>
// ============================================================================
class GVerticalText : public QImageWidget
// ============================================================================
{
   Q_OBJECT
public:
   GVerticalText( QString name, QWidget* parent = 0 );
   ~GVerticalText();

signals:
   //void toggled(bool);
public slots:

public:
   QPushButton* m_bypass;
   QFont5x8 m_font;
};

#endif // DE_Q_IMAGEWIDGET_HPP
