#ifndef DE_LIVE_CLIP_CONTENT_HPP
#define DE_LIVE_CLIP_CONTENT_HPP

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

struct App;

//#include <BottomLeftPanel.hpp>
//#include <ClipContent.hpp>
#include <ClipEditor.hpp>

// ============================================================================
class ClipContent : public QWidget
// ============================================================================
{
   Q_OBJECT
   //DE_CREATE_LOGGER("ClipContent")
   App & m_app;
   bool m_hasFocus;
   ClipEditor* m_clipEditor;
public:
   ClipContent( App & app, QWidget* parent = 0 );
   ~ClipContent();
   bool hasFocus() const { return m_hasFocus; }
signals:
public slots:
protected:
   void resizeEvent( QResizeEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;
   void enterEvent( QEvent* event ) override;
   void leaveEvent( QEvent* event ) override;
   void focusInEvent( QFocusEvent* event ) override;
   void focusOutEvent( QFocusEvent* event ) override;
private:
};

#endif // DE_Q_IMAGEWIDGET_HPP
