#ifndef DE_LIVE_ARRANGE_TRACK_DROP_TARGET_DUMMY_HPP
#define DE_LIVE_ARRANGE_TRACK_DROP_TARGET_DUMMY_HPP

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

// ============================================================================
class ArrangeDropTarget : public QWidget
// ============================================================================
{
   Q_OBJECT
   //DE_CREATE_LOGGER("ArrangeDropTarget")
   App & m_app;
   //bool m_isDirty;
   //bool m_isAudioOnly;
   //bool m_hasFocus;
public:
   ArrangeDropTarget( App & app, QWidget* parent = 0 );
   ~ArrangeDropTarget() override {}
   //bool hasFocus() const { return m_hasFocus; }
signals:
public slots:
   //void setAudioOnly( bool b );
protected:
   //void updateImage();
   //void resizeEvent( QResizeEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;
   void dropEvent( QDropEvent* event ) override;
   void dragEnterEvent( QDragEnterEvent* event ) override;
   void dragLeaveEvent( QDragLeaveEvent* event ) override;
   void dragMoveEvent(QDragMoveEvent* event ) override;
   void enterEvent( QEvent* event ) override;
   void leaveEvent( QEvent* event ) override;
   void focusInEvent( QFocusEvent* event ) override;
   void focusOutEvent( QFocusEvent* event ) override;
};

#endif // DE_Q_IMAGEWIDGET_HPP
