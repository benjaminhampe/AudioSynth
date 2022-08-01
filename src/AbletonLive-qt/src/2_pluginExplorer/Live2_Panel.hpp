/// (c) 2017 - 20180 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef DE_LIVE2_PANEL_HPP
#define DE_LIVE2_PANEL_HPP

#include "App.hpp"
#include "Live0_Panel.hpp"
#include "PluginList.hpp"
#include "DeviceBar.hpp"

class Live2_Panel : public QWidget
{
   Q_OBJECT
public:
   DeviceBar* m_deviceBar;
   Live0_Panel* m_pluginPanel;
   PluginList* m_pluginList;
public:
   Live2_Panel( App & app, QWidget* parent )
      : QWidget( parent )
   {
      m_deviceBar = new DeviceBar( app, this );
      m_pluginPanel = new Live0_Panel( app, this );
      m_pluginList = new PluginList( app, this );

      auto hPanel = createHBox();
      hPanel->addWidget( m_pluginList );
      m_pluginPanel->setLayout( hPanel );

      auto h = createHBox();
      h->addWidget( m_deviceBar );
      h->addWidget( m_pluginPanel );
      h->addSpacing( 5 );
      setLayout( h );
   }

   ~Live2_Panel() override
   {

   }
};

#endif
