/// (c) 2017 - 20180 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef DE_LIVE6_FXPANEL_HPP
#define DE_LIVE6_FXPANEL_HPP

#include "Live0_Panel.hpp"
#include "FxPlugin.hpp"

#ifndef USE_PLUGIN_DRAG_N_DROP
#define USE_PLUGIN_DRAG_N_DROP
#endif

#ifdef USE_PLUGIN_DRAG_N_DROP
   #include <QMimeData>
#endif

// FxPanel:
class FxPanel : public Live0_Panel
{
   Q_OBJECT
   FxPlugin* m_synth;
   FxPlugin* m_last;
   std::vector< FxPlugin* > m_effects;
   QHBoxLayout* m_layout;
public:
   FxPanel( App & app, QWidget* parent = nullptr )
      : Live0_Panel( app, parent )
      , m_synth( nullptr )
      , m_last( nullptr )
   {
   #ifdef USE_PLUGIN_DRAG_N_DROP
      setAcceptDrops( true );
   #endif
      m_layout = createHBox();
      setLayout( m_layout );

      //connect( this, SIGNAL(focusChanged(bool)), m_synth, SLOT(on_focusChanged(bool)) );
      //connect( this, SIGNAL(focusChanged(bool)), m_effects[0], SLOT(on_focusChanged(bool)) );
   }
   ~FxPanel() override {}

   int getEffectCount() const { return m_effects.size(); }

signals:
public slots:
//   int addPlugin( FxPlugin* plugin, int pos = 0 )
//   {
//      int n = m_effects.size();
//      m_effects.emplace_back( plugin );
//      return n;
//   }

protected slots:
protected:

#ifdef USE_PLUGIN_DRAG_N_DROP

   void dragEnterEvent( QDragEnterEvent* event )
   {
      if ( event->mimeData()->hasFormat("text/plain") )
      {
         event->acceptProposedAction();
      }
      std::cout << __func__ << std::endl;
   }

   void dragLeaveEvent( QDragLeaveEvent* event )
   {
      std::cout << __func__ << std::endl;
   }

   void dragMoveEvent(QDragMoveEvent* event )
   {
      std::cout << __func__ << std::endl;
   }

   void dropEvent( QDropEvent* event )
   {
      std::cout << __func__ << std::endl;
      std::cout << event->mimeData()->text().toStdString() << std::endl;

      std::wstring uri = event->mimeData()->text().toStdWString();

      FxPlugin* plugin = new FxPlugin( m_app, this );
      plugin->setUri( uri );

      if ( plugin->openPlugin() )
      {
         bool isSynth = plugin->isSynth();
         std::wcout << "Opened " << (isSynth ? "synth" : "effect")
            << " plugin " << uri << std::endl;

         if ( isSynth )
         {
            if ( m_synth )
            {
               m_layout->replaceWidget( m_synth, plugin );
               delete m_synth;
            }

            m_synth = plugin;
         }
         else
         {
            m_effects.emplace_back( plugin );
         }

         reconnectDspChain();
      }
      else
      {
         std::wcout << "Cant open plugin " << uri << std::endl;
         delete plugin;
      }

      //textBrowser->setPlainText(event->mimeData()->text());
      //mimeTypeCombo->clear();
      //mimeTypeCombo->addItems(event->mimeData()->formats());

      event->acceptProposedAction();
   }

#endif

   void reconnectDspChain()
   {
      if ( m_synth && m_layout ) { m_layout->removeWidget( m_synth ); }

      for ( int i = 0; i < m_effects.size(); ++i )
      {
         auto fx = m_effects[ i ];
         if ( fx && m_layout )
         {
            m_layout->removeWidget( fx );
         }
      }

      if ( m_synth && m_layout ) { m_layout->addWidget( m_synth ); }
      for ( int i = 0; i < m_effects.size(); ++i )
      {
         auto fx = m_effects[ i ];
         if ( fx && m_layout )
         {
            m_layout->addWidget( fx );
         }
      }

      // Connect dsp chain

      m_last = nullptr;

      if ( !m_synth )
      {
         std::wcout << "Error, need synth to connect DSP chain" << std::endl;
         return;
      }

      m_last = m_synth;

      // Connect dsp chain

      for ( int i = 0; i < m_effects.size(); ++i )
      {
         auto fx = m_effects[ i ];

         fx -> setInputSignal( 0, m_last );
         m_last = fx;
      }

   }

};

#endif
