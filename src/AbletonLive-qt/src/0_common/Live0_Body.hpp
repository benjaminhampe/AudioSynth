/// (c) 2017 - 20180 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef DE_LIVE_BODY_HPP
#define DE_LIVE_BODY_HPP

#include "App.hpp"
#include "Live1_Panel.hpp"
#include "Live2_Panel.hpp"
#include "Live3_Panel.hpp"
//#include "Live4_Panel.hpp"
#include "Live5_Panel.hpp"
//#include "Live6_Panel.hpp"
#include "Live7_Panel.hpp"

// ============================================================================
class Splitter : public QSplitter
// ============================================================================
{
   Q_OBJECT
   App & m_app;
public:
   Splitter( App & app, Qt::Orientation orientation, QWidget* parent = 0 );
   ~Splitter() {}

signals:
public slots:
protected:
   //void updateImage();
   //void resizeEvent( QResizeEvent* event ) override;
//   void enterEvent( QEvent* event ) override { m_isHovered = true; QPushButton::enterEvent( event ); }
//   void leaveEvent( QEvent* event ) override { m_isHovered = false; QPushButton::leaveEvent( event ); }
   void paintEvent( QPaintEvent* event ) override;

protected:
};

// ============================================================================
class Live0_Body : public QWidget
// ============================================================================
{
   Q_OBJECT
   App m_app;
   Live1_Panel* m_enginePanel;

   Splitter* m_hSplit;
   Live2_Panel* m_pluginPanel;
   Live3_Panel* m_arrangePanel;

   Splitter* m_vSplit;
   Live5_Panel* m_trackPanel;
   Live7_Panel* m_footerPanel;


/*
   Live1_AudioEngine* m_enginePanel;

   GMidiTrackEditor* m_compo;
   Live0_Panel* m_compoPanel;

   GPluginList* m_helpPanel;
   Live0_Panel* m_helpPanelPanel;

   GMidiTrackEditor* m_trackEditor;
   Live0_Panel* m_trackEditorPanel;

*/
public:
   Live0_Body( QWidget* parent = nullptr );
   ~Live0_Body() override;

protected slots:
   void on_showExplorer( bool checked );
   void on_showHelpPanel( bool checked );
   void on_showClipAndFxPanel( bool checked );
protected:
   //void updateImage();
   //void resizeEvent( QResizeEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;



};

#endif
