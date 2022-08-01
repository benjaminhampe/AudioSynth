#ifndef DE_LIVE4_HELP_VIEWER_HPP
#define DE_LIVE4_HELP_VIEWER_HPP

#include "Live0_Panel.hpp"

// ============================================================================
class HelpViewer : public QWidget
// ============================================================================
{
   DE_CREATE_LOGGER("HelpViewer")
   Q_OBJECT
   App & m_app;
public:
   HelpViewer( App & app, QWidget* parent = 0 );
   ~HelpViewer();

signals:
public slots:

protected slots:
protected:
   //void resizeEvent( QResizeEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;
};

#endif // G_LOPASS1_HPP
