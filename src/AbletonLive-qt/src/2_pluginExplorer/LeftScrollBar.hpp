#ifndef DE_LIVE2_LEFT_SCROLLBAR_HPP
#define DE_LIVE2_LEFT_SCROLLBAR_HPP

#include <App.hpp>

// ============================================================================
class LeftScrollBar : public QWidget
// ============================================================================
{
   Q_OBJECT
   App & m_app;
   bool m_isHovered;
public:
   LeftScrollBar( App & app, QWidget* parent = 0 );
   ~LeftScrollBar();
signals:
public slots:
protected:
   //void resizeEvent( QResizeEvent* event ) override;
   void enterEvent( QEvent* event ) override;
   void leaveEvent( QEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;

protected:
};

#endif // G_LOPASS1_HPP
