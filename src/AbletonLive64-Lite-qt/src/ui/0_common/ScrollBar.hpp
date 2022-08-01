#ifndef DE_LIVE_SCROLLBAR_HPP
#define DE_LIVE_SCROLLBAR_HPP

#include "LiveData.hpp"

struct App;

#include "ImageButton.hpp"

// ============================================================================
class ScrollHandle : public QWidget
// ============================================================================
{
   Q_OBJECT
   App & m_app;
   bool m_isHovered;
   ImageButton* m_btnShowArrangement;
   ImageButton* m_btnShowSessionButton;

public:
   ScrollHandle( App & app, QWidget* parent = 0 );
   ~ScrollHandle() override {}
signals:
public slots:
protected:
   //void resizeEvent( QResizeEvent* event ) override;
   void enterEvent( QEvent* event ) override;
   void leaveEvent( QEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;
};

// ============================================================================
class ScrollBar : public QWidget
// ============================================================================
{
   Q_OBJECT
   App & m_app;
   bool m_isHovered;
   ImageButton* m_btnUp;
   ImageButton* m_btnDown;
   ScrollHandle* m_handle;
public:
   ScrollBar( App & app, QWidget* parent = 0 );
   ~ScrollBar() override {}
signals:
public slots:
protected:
   void enterEvent( QEvent* event ) override;
   void leaveEvent( QEvent* event ) override;

protected:

   ImageButton* createScrollUpButton();
   ImageButton* createScrollDownButton();
};

#endif // G_LOPASS1_HPP
