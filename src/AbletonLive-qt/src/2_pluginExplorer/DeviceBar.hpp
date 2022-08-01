/// (c) 2017 - 20180 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef DE_LIVE2_DEVICEBAR_HPP
#define DE_LIVE2_DEVICEBAR_HPP

#include <App.hpp>
#include <Button.hpp>
#include <LeftScrollBar.hpp>

class DeviceBar : public QWidget
{
   Q_OBJECT
   App & m_app;
public: // Connected by Body
   Button* m_btnShowExplorer;
   Button* m_btnInternDevice;
   Button* m_btnExternDevice;
   Button* m_btnExplorer1;
   Button* m_btnExplorer2;
   Button* m_btnExplorer3;
   LeftScrollBar* m_leftScrollBar;
   Button* m_btnQuickLibrary;

public:
   DeviceBar( App & app, QWidget* parent = nullptr );
   ~DeviceBar() override;
signals:
protected slots:
protected:
   //void updateImage();
   //void resizeEvent( QResizeEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;

};

#endif
