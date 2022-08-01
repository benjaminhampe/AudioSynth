/// (c) 2017 - 20180 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef DE_LIVE3_PANEL_HPP
#define DE_LIVE3_PANEL_HPP

#include "App.hpp"
#include "Arrangement.hpp"
#include "ComposeBar.hpp"
#include "TimeLineOverview.hpp"

class Live3_Panel : public QWidget
{
   Q_OBJECT
   App & m_app;
   TimeLineOverview* m_overview;
   Arrangement* m_arrangement;
   Live0_Panel* m_arrangePanel;
   ComposeBar* m_composeBar;
public:
   Live3_Panel( App & app, QWidget* parent = nullptr );
   ~Live3_Panel() override;
};

#endif
