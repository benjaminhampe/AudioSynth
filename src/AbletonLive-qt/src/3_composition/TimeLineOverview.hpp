#ifndef DE_LIVE3_TIMELINE_OVERVIEW_HPP
#define DE_LIVE3_TIMELINE_OVERVIEW_HPP

#include "App.hpp"

// TrackListOverview
// ============================================================================
class TimeLineOverview : public QWidget
// ============================================================================
{
   DE_CREATE_LOGGER("TimeLineOverview")
   Q_OBJECT
   App & m_app;
   bool m_isHighlighted;
   int m_lineHeight;

public:
   TimeLineOverview( App & app, QWidget* parent = 0 );
   ~TimeLineOverview();

   void reset()
   {
      m_isHighlighted = false;
   }

signals:
public slots:
protected slots:
protected:
   void updateFromTrackList();

   //void resizeEvent( QResizeEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;
};

#endif // G_LOPASS1_HPP
