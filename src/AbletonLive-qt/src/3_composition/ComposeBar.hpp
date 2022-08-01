/// (c) 2017 - 20180 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef DE_LIVE3_COMPOSEBAR_HPP
#define DE_LIVE3_COMPOSEBAR_HPP

#include <App.hpp>
#include <Button.hpp>

class ComposeBar : public QWidget
{
   Q_OBJECT
   App & m_app;
   Button* m_btnShowArrangement;
   Button* m_btnShowSession;
   Button* m_btnQuickLibrary;
public:
   ComposeBar( App & m_app, QWidget* parent = nullptr );
   ~ComposeBar() override;
signals:
   void showArrangement( bool checked );
   void showSession( bool checked );
protected slots:
   void on_btnShowArrangement( bool checked );
   void on_btnShowSession( bool checked );
protected:
protected:
   //void updateImage();
   //void resizeEvent( QResizeEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;

};

#endif
