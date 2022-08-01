/// (c) 2017 - 20180 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef DE_LIVE0_PANEL_HPP
#define DE_LIVE0_PANEL_HPP

#include "App.hpp"

class Live0_Panel : public QWidget
{
   Q_OBJECT
public:
   App & m_app;
   bool m_hasFocus;
public:
   Live0_Panel( App & app, QWidget* parent = nullptr );
   ~Live0_Panel() override;
signals:
   void focusChanged( bool focused );
protected:
   //void updateImage();
   //void resizeEvent( QResizeEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;
   void focusInEvent( QFocusEvent* event ) override;
   void focusOutEvent( QFocusEvent* event ) override;
};

#endif
