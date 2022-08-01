﻿#ifndef DE_LIVE_GTRACK_HORIZONTAL_HPP
#define DE_LIVE_GTRACK_HORIZONTAL_HPP

#include "LiveSkin.hpp"
#include "AudioMeter.hpp"
#include <QSlider>

struct App;
struct Track;

// ============================================================================
class ItemV : public QWidget
// ============================================================================
{
   Q_OBJECT
public:
   DE_CREATE_LOGGER("ItemV")
   App & m_app;
   bool m_hasFocus;
   bool m_isHovered;
   bool m_isSelected;   // Not same as hasFocus
   Track* m_track;

   bool m_isAudioOnly;

   QColor m_color;

   QWidget* m_optMaster;

   QWidget* m_timeGrid;

   QLabel* m_lblName;
   QLabel* m_lblType;

//   QString m_trackName;
//   QString m_trackDesc;

   AudioMeter2* m_audioMeter;
   QSlider* m_volumeSlider;

public:
   ItemV( App & app, QWidget* parent = 0 );
   ~ItemV() override {}

signals:
public slots:
   void setTrack( Track* track );
   void updateFromTrack();
   void updateLayout();
protected slots:
   void onCustomContextMenuRequested( QPoint const & pos );
protected:
   void resizeEvent( QResizeEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;
   void dropEvent( QDropEvent* event );
   void dragEnterEvent( QDragEnterEvent* event );
   void dragLeaveEvent( QDragLeaveEvent* event );
   void dragMoveEvent(QDragMoveEvent* event );
   void enterEvent( QEvent* event ) override;
   void leaveEvent( QEvent* event ) override;
   void focusInEvent( QFocusEvent* event ) override;
   void focusOutEvent( QFocusEvent* event ) override;
//   void hideEvent( QHideEvent* event ) override;
//   void showEvent( QShowEvent* event ) override;

   void mousePressEvent( QMouseEvent* event ) override;
   void mouseReleaseEvent( QMouseEvent* event ) override;
   void mouseMoveEvent( QMouseEvent* event ) override;
   void wheelEvent( QWheelEvent* event ) override;

   void keyPressEvent( QKeyEvent* event ) override;
   void keyReleaseEvent( QKeyEvent* event ) override;
};

#endif // G_LOPASS1_HPP
