#ifndef DE_LIVE3_ARRANGEMENT_HPP
#define DE_LIVE3_ARRANGEMENT_HPP

#include "App.hpp"

// ============================================================================
class Arrangement : public QWidget
// ============================================================================
{
   DE_CREATE_LOGGER("Arrangement")
   Q_OBJECT
   App & m_app;
   int m_focusedTrack;

   int m_headHeight;
   int m_setHeight;
   int m_pinHeight;
   int m_trackHeight;
   int m_trackSpace;
   int m_footHeight;

public:
   Arrangement( App & app, QWidget* parent = 0 );
   ~Arrangement();
   void reset();

signals:
public slots:
protected slots:
protected:
   //void resizeEvent( QResizeEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;
};

#endif // G_LOPASS1_HPP
