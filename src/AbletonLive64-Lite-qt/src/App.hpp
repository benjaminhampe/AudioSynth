#ifndef DE_LIVE_APP_HPP
#define DE_LIVE_APP_HPP

#include "LiveSkin.hpp"
#include "Track.hpp"
#include "ImageButton.hpp"
#include "ScrollBar.hpp"

#include "Window.hpp"
#include "Body.hpp"
#include "Header.hpp"

// ExplorerPanel
#include "PluginDirTree.hpp"
#include "PluginScanner.hpp"
#include "PluginExplorer.hpp"
#include "PluginInfoTree.hpp"

#include "ArrangeOverview.hpp"
#include "ArrangeContent.hpp"
#include "ClipContent.hpp"
#include "Track.hpp"
#include "FooterPanel.hpp"
#include "LongText.hpp"
#include "ClipOverview.hpp"
#include "TrackOverview.hpp"

#include "MidiMasterKeyboard.hpp"

#include <de/audio/dsp/IDspChainElement.hpp>

// --- audio engine ---
#include <de/audio/stream/Stream.hpp>
#include <de/audio/dsp/Mixer.hpp>
//#include <de/audio/dsp/Mixer2.hpp>
#include <de/audio/plugin/PluginInfoDb.hpp>

enum eLivePanel
{
   eLivePanelExplorer = 0,
   eLivePanelArrange,
   eLivePanelDetails,
   eLivePanelQuickHelp,
   eLivePanelCount,
};

// ============================================================================
struct App : public QObject
// ============================================================================
{
   Q_OBJECT
public:
   App();
   ~App();

   // Audio Master Stream
   void stopAudioMaster() { m_masterStream.stop(); }

   void stopAudioMasterBlocking()
   {
      stopAudioMaster();
      while ( isAudioMasterPlaying() ) { std::cout << "Wait... AudioMaster stop..." << std::endl; }
   }

   void playAudioMaster() { m_masterStream.play(); }
   bool isAudioMasterPlaying() const { return m_masterStream.isPlaying(); }

   // Plugin Info Database
   de::audio::PluginInfoDb const & pluginDb() const { return m_pluginDb; }
   de::audio::PluginInfoDb & pluginDb() { return m_pluginDb; }
   de::audio::PluginInfo const* getPluginInfo( std::wstring const & uri ) const { return m_pluginDb.getPlugin( uri ); }

   // Plugin of current
   //Plugin const* plugin( int id ) const { return m_pluginDb; }
   IPlugin* getPlugin( int id );

   // TRACK MANAGER

   typedef std::vector< Track* > TrackVector;

   size_t size() const { return m_tracks.size(); }

   typedef TrackVector::const_iterator const_iterator;
   const_iterator begin() const { return m_tracks.begin(); }
   const_iterator end() const { return m_tracks.end(); }

   typedef TrackVector::iterator iterator;
   iterator begin() { return m_tracks.begin(); }
   iterator end() { return m_tracks.end(); }

   //std::string const & uri() const { return m_uri; }
   Track* masterTrack() { return m_masterTrack; }
   //int currentTrackId() const { return m_currentTrackId; }
   //Track const* currentTrack() const { return m_currentTrackId > -1 ? m_tracks[ m_currentTrackId ] : nullptr; }
   //Track* currentTrack() { return m_currentTrackId > -1 ? m_tracks[ m_currentTrackId ] : nullptr; }

   //std::string const & uri() const { return m_uri; }
   int trackId() const { return m_trackId; }

   Track const* track() const { return m_track; }
   Track* track() { return m_track; }

//   bool addTrack( int id = -1, std::string name = "UnnamedTrack", eTrackType type = eTrackTypeCount );
//   bool addTrack( Track* Track );
//   void updateDspChain();

   //IPlugin* addPlugin( std::wstring uri );

   //int findTrack( int id ) const;
   //bool hasTrack( int id ) const;
   Track* getTrack( int id );

signals:
   void activatedPluginId( int id );
   void activatedPlugin( IPlugin* plugin );

   void activatedTrackId( int trackId );
   void activatedTrack( Track* track );

public slots:
   bool addTrack( TrackInfo const & ti, bool trace = false );
   bool addTrack( int id = -1, std::string name = "UnnamedTrack", eTrackType type = eTrackTypeCount, QColor color = QColor(255,255,255) );

   bool addPlugin( de::audio::PluginInfo const & pluginInfo, bool stopAudio = false );
   bool addPlugin( std::wstring const & uri, bool stopAudio = false );

   bool save( bool saveLiveSet );
   bool load();

   //void setActiveTrackId( int trackId );
   void setActiveTrack( Track* track );
   void clearTracks();
   void removeTrack( Track* track );

   bool loadTracks( std::string const & uri );
   bool saveTracks( std::string const & uri ) const;

   void setActivePluginId( int id );
   void setActivePlugin( IPlugin* plugin );

   void updateDspChain();

   void setLongText( QString txt )
   {
      if ( m_longText ) m_longText->setLongText( txt );
   }

   void showSpurEditor();
   void showClipEditor();

   void allNotesOff()
   {
      for ( auto track : m_userMidiTracks )
      {
         if ( track ) track->allNotesOff();
      }
   }

   void sendNote( de::audio::Note const & note )
   {
      for ( auto track : m_userMidiTracks )
      {
         if ( track ) track->sendNote( note );
      }
   }

   void updateLayout();


   void hideEditorWindows();
public:

//   int countSynthesizer() const
//   {
//      return m_engine.countSynthesizer();
//   }

//   void updateSynthesizer()
//   {
//      m_engine.updateSynthesizer();
//   }


public:
   DE_CREATE_LOGGER("LiveApp")
   //QString m_persistDir;
   std::string m_roamingDir;
   std::string m_pluginDbXml;
   std::string m_trackListXml;

   LiveSkin m_skin;

   //std::vector< Plugin* > m_plugins;
   de::audio::PluginInfoDb m_pluginDb;

   BeatGrid m_beatGrid;
   std::string m_mediaDirMB;
   QIcon m_synthIcon;
   QIcon m_effectIcon;

   int m_pluginId;   // current plugin id ( of current track )
   IPlugin* m_plugin; // current direct pointer

   int m_trackId;    // current track id ( of master mixer )
   Track* m_track;   // current track

   TrackVector m_tracks;         // all tracks managed ( Audio + Midi )
   Track* m_masterTrack;         // one master track ( Audio-Only )
   TrackVector m_returnTracks;   // all return tracks ( Audio-Only )
   TrackVector m_userTracks;     // all user tracks ( Audio + Midi )
   TrackVector m_userAudoTracks; // all user audio-only tracks ( Audio-Only )
   TrackVector m_userMidiTracks; // all user midi tracks ( Midi )

   //IDspChainElement* m_inputSignal;
   de::audio::Mixer m_userMixer;
   de::audio::Mixer m_returnMixer;
   de::audio::Mixer m_masterMixer;

//   std::string m_pluginDbXml;
//   std::string m_networkXml;
/*
   // DSP network ( signal mixing )
   Network m_networkInfo;
   Network m_network1;
   Network m_network2;

   bool m_wantNetworkSwitching;
   Network* m_readNetwork;
   Network* m_writeNetwork;
*/
   // Audio Engine
   int m_dspRateInHz; // 48000
   int m_dspFrames;   // 256, sw buffer latency then ca. 256 / 48000 = 5,333 ms

//   // SoundCard DeviceList
//   std::vector< de::audio::Stream > m_streams;
//   // SoundCard DeviceList
//   std::vector< DriverInfo > m_drivers;
//   // SoundCard Master Output Device
//   DriverInfo* m_master;
//   // SoundCard Monitor Output Device
//   DriverInfo* m_monitor;
//   // SoundCard Input Device ( Microphone )
//   DriverInfo* m_microphone;
   de::audio::Stream m_masterStream;   // SoundCard Master Output Device

   // Synthesizer a MidiKeyboard sends notes to.
   //std::vector< de::audio::IDspChainElement* > m_synths;

   // Spur* get( int trackId );
   // int find( int trackId ) const;
   // std::vector< Spur* > const & tracks() const { return m_all; }
   // std::vector< Spur* > & tracks() { return m_all; }
   // size_t size() const { return m_all.size(); }
   // size_t trackCount() const { return m_user.size(); }
   // Spur const* operator[] ( size_t i ) const { return m_all[ i ]; }
   // Spur* operator[] ( size_t i ) { return m_all[ i ]; }

   QString appName;
   bool m_isOverSplitV;
   bool m_isOverSplitH;
   //bool m_isExplorerVisible;
   //bool m_isQuickHelpVisible;
   bool m_isExplorerVisible;
   bool m_isQuickHelpVisible;
   bool m_isDetailVisible;
   bool m_isClipEdVisible;
   bool m_isHelpVisible;

   eLivePanel m_hoverPanel;
   eLivePanel m_focusPanel;

   int m_deviceBarWidth; // fix width
   int m_explorerWidthMin; // fix width
   int m_hsplitterPos; // splitter h pos
   // Bottom Rect Contents
   int m_spurEditorHeight; // fix height
   int m_clipEditorHeight; // var height by splitter
   int m_vsplitterPos; // splitter h pos
   int m_quickHelpWidth; // fix width

   // ::: All Widgets :::
   Window* m_window;
   Body* m_body;
   MidiMasterKeyboard* m_midiMasterKeyboard;
   Header* m_headerPanel;   // Header
   // TopPanel::Left::ExplorerPanel::DeviceBar
   ImageButton* m_btnShowExplorer;
   ImageButton* m_btnShowInternDevices;
   ImageButton* m_btnShowExternDevices;
   ImageButton* m_btnShowExplorer1;
   ImageButton* m_btnShowExplorer2;
   ImageButton* m_btnShowExplorer3;
   ScrollBar* m_explorerScrollBar;
   ImageButton* m_btnShowGrooves;
   // TopPanel::Left::ExplorerPanel
   PluginExplorer* m_pluginExplorer;
   PluginScanner* m_pluginScanner;
   PluginDirTree* m_pluginDirTree;
   ImageButton* m_btnShowPluginDirs;
   PluginInfoTree* m_pluginInfoTree;
   // TopPanel::Right::ArrangePanel
   Arrangement* m_arrangement;
   ImageButton* m_btnShowArrangement;
   ImageButton* m_btnShowSession;
   ScrollBar* m_arrangeScrollBar;
   ImageButton* m_btnComposeIO;
   ImageButton* m_btnComposeR;
   ImageButton* m_btnComposeM;
   ImageButton* m_btnComposeD;
   TimeLineOverview* m_timeLineOverview;
   // DetailPanel
   ClipContent* m_clipContent;
   QStackedWidget* m_detailStack; // all tracks
   // Footer
   FooterPanel* m_footerPanel;
   ImageButton* m_btnShowQuickHelpPanel;
   LongText* m_longText;
   ImageButton* m_btnShowMidiKeyboard;
   ClipOverview* m_btnClipOverview;
   TrackOverview* m_btnTrackOverview;
   ImageButton* m_btnShowDetailPanel;

};

#endif // G_LOPASS1_HPP
