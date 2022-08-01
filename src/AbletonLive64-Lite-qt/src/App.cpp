#include "App.hpp"
#include <cassert>

App::App()
{
   appName = "AbletonLive64 Lite (c) 2022 <benjaminhampe@gmx.de>";

   m_hoverPanel = eLivePanelCount;
   m_focusPanel = eLivePanelQuickHelp;

   m_body = nullptr;
   m_arrangement = nullptr;

   // TrackManager
   m_pluginId = -1;
   m_plugin = nullptr;
   m_trackId = -1;
   m_track = nullptr;
   m_masterTrack = nullptr;

   // DeviceBar
   m_btnShowExplorer = nullptr;
   m_btnShowInternDevices = nullptr;
   m_btnShowExternDevices = nullptr;
   m_btnShowExplorer1 = nullptr;
   m_btnShowExplorer2 = nullptr;
   m_btnShowExplorer3 = nullptr;
   m_explorerScrollBar = nullptr;
   m_btnShowGrooves = nullptr;
   // PluginExplorer
   m_pluginExplorer = nullptr;
   // PluginScanner
   m_pluginScanner = nullptr;
   m_pluginDirTree = nullptr;
   m_btnShowPluginDirs = nullptr;
   m_pluginInfoTree = nullptr;
   // Footer
   m_btnShowQuickHelpPanel = nullptr;
   m_longText = nullptr;
   m_btnShowMidiKeyboard = nullptr;
   m_btnClipOverview = nullptr;
   m_btnTrackOverview = nullptr;
   m_btnShowDetailPanel = nullptr;
   // Footer II
   m_midiMasterKeyboard = nullptr;

   m_isOverSplitV = false;
   m_isOverSplitH = false;
   m_isExplorerVisible = true;
   m_isDetailVisible = true;
   m_isQuickHelpVisible = true;
   m_isClipEdVisible = false;
   m_isHelpVisible = false;

   m_deviceBarWidth = 23; // fix width
   m_explorerWidthMin = 20; // fix width
   m_hsplitterPos = 200; // splitter h pos

   m_spurEditorHeight = 190 + 12; // fix height
   m_clipEditorHeight = 300; // var height by splitter
   m_vsplitterPos = m_spurEditorHeight; // splitter h pos
   m_quickHelpWidth = 220; // fix width

   addFontAwesome463();

//   QString const appPath = qApp->applicationDirPath() + "/";
//   QString const stylePath( appPath + "themes/darcula.css" );
//   app.setStyleSheet( "file:///" + stylePath );
//   dbAddFontFamily( "fontawesome.ttf" );
//   dbAddFontFamily( "la-regular-400.ttf" );
//   dbAddFontFamily( "la-brands-400.ttf" );
//   dbAddFontFamily( "la-solid-900.ttf" );
//   QString const appPath = qApp->applicationDirPath() + "/";
//   QString const cssPath( appPath + "themes/darcula.css" );
//   app.setStyleSheet( "file:///" + cssPath );

   // Create Window
   App & m_app = *this;

   m_window = new Window( m_app );

   // Create Window children
   m_body = new Body( m_app, m_window );
   m_midiMasterKeyboard = new MidiMasterKeyboard( m_app, m_window );

   // Create Body children
   m_headerPanel = new Header( m_app, m_body );
   m_footerPanel = new FooterPanel( m_app, m_body );

   // Create DetailPanel children
   m_clipContent = new ClipContent( m_app, m_body );
   m_clipContent->setVisible( false );

   // Layout MainWindow
   auto v = createVBox();
   v->addWidget( m_body, 1 );
   v->addWidget( m_midiMasterKeyboard );
   auto m_central = new QWidget( m_window );
   m_central->setLayout( v );
   m_window->setCentralWidget( m_central );
   m_window->setWindowTitle( appName );

   // Load XML or createDefault

   m_mediaDirMB = "../../media/";
   QString mediaDir = QString::fromStdString( m_mediaDirMB );
   m_synthIcon = QIcon( mediaDir + "synthIcon.png" );
   m_effectIcon = QIcon( mediaDir + "effectIcon.png" );

//   std::cout << "Load PluginDB ... " << std::endl;
//   m_pluginDb.loadXml( m_mediaDirMB + "data/plugin_db.xml" );
//   std::cout << "Loaded " << m_pluginDb.size() << " Plugins" << std::endl;

//   int id = 0;

   QString persistRoot = QStandardPaths::standardLocations( QStandardPaths::AppDataLocation ).at(0);
   QString persistDir = persistRoot + "/";
   m_roamingDir = persistDir.toStdString();
   m_pluginDbXml = m_roamingDir + "data/plugin_info_database.xml";
   m_trackListXml = m_roamingDir + "data/tracks.xml";

   dbMakeDirectory( m_roamingDir + "data" );

   m_window->show();

   load();

   // STOP AUDIO NOW
//   bool wasPlaying = isAudioMasterPlaying();
//   stopAudioMasterBlocking();

   updateDspChain();

   playAudioMaster();

   // (RE)START AUDIO, if any
   //setBypassed( wasBypassed );
   //m_userMixer.m_isBypassed = false;
//   if ( wasPlaying )
//   {
//      playAudioMaster();
//   }

   //updateDspChain();
   //load( m_app );


}

App::~App()
{
   //m_masterStream.stop();
   //clearTracks();
}


void App::hideEditorWindows()
{
   for ( auto trk : m_tracks )
   {
      if ( trk )
      {
         for ( auto plugin : trk->m_audioShells )
         {
            if ( plugin )
            {
               plugin->hideEditor();
            }
         }
      }
   }
}

void
App::removeTrack( Track* track )
{
   if ( !track )
   {
      return;
   }

   if ( track->m_trackInfo.m_type == eTrackMaster )
   {
      return;
   }

   bool wasPlaying = isAudioMasterPlaying();
   stopAudioMasterBlocking();

//   m_trackId = -1;
//   m_track = nullptr;
//   m_masterTrack = nullptr;

   if ( track->m_trackInfo.m_type == eTrackReturn )
   {
      auto it = std::find_if( m_returnTracks.begin(), m_returnTracks.end(), [&] ( Track* cached ) { return cached == track; } );
      if ( it != m_returnTracks.end() ) { m_returnTracks.erase( it ); }

      auto it2 = std::find_if( m_arrangement->m_returns.begin(), m_arrangement->m_returns.end(), [&] ( ItemV* cached ) { return cached->m_track == track; } );
      if ( it2 != m_arrangement->m_returns.end() ) { delete *it2; m_arrangement->m_returns.erase( it2 ); }
   }
   else if ( track->m_trackInfo.m_type == eTrackAudio )
   {
      auto it = std::find_if( m_userTracks.begin(), m_userTracks.end(), [&] ( Track* cached ) { return cached == track; } );
      if ( it != m_userTracks.end() ) { m_userTracks.erase( it ); }

      auto it2 = std::find_if( m_userAudoTracks.begin(), m_userAudoTracks.end(), [&] ( Track* cached ) { return cached == track; } );
      if ( it2 != m_userAudoTracks.end() ) { m_userAudoTracks.erase( it2 ); }

      auto it3 = std::find_if( m_arrangement->m_user.begin(), m_arrangement->m_user.end(), [&] ( ItemV* cached ) { return cached->m_track == track; } );
      if ( it3 != m_arrangement->m_user.end() ) { delete *it3; m_arrangement->m_user.erase( it3 ); }

   }
   else if ( track->m_trackInfo.m_type == eTrackMidi )
   {
      auto it = std::find_if( m_userTracks.begin(), m_userTracks.end(), [&] ( Track* cached ) { return cached == track; } );
      if ( it != m_userTracks.end() ) { m_userTracks.erase( it ); }

      auto it2 = std::find_if( m_userMidiTracks.begin(), m_userMidiTracks.end(), [&] ( Track* cached ) { return cached == track; } );
      if ( it2 != m_userMidiTracks.end() ) { m_userMidiTracks.erase( it2 ); }

      auto it3 = std::find_if( m_arrangement->m_user.begin(), m_arrangement->m_user.end(), [&] ( ItemV* cached ) { return cached->m_track == track; } );
      if ( it3 != m_arrangement->m_user.end() ) { delete *it3; m_arrangement->m_user.erase( it3 ); }
   }

   for ( int i = 0; i < m_detailStack->count(); i++ )
   {
      auto w = m_detailStack->widget( i );
      if ( w == track )
      {
         m_detailStack->removeWidget( w );
         //m_detailStack->setCurrentIndex( 0 );
      }
   }

   auto it = std::find_if( m_tracks.begin(), m_tracks.end(), [&] ( Track* cached ) { return cached == track; } );
   if ( it != m_tracks.end() ) { m_tracks.erase( it ); }

   delete track;

   updateDspChain();

   m_arrangement->updateLayout();

   if ( wasPlaying )
   {
      playAudioMaster();
   }
}

bool
App::addTrack( TrackInfo const & ti, bool trace )
{
   bool wasPlaying = isAudioMasterPlaying();
   stopAudioMasterBlocking();

   DE_DEBUG("Add track ti ", ti.toString() )

//   auto cached found = findTrack( ti.id() );
//   if (found > -1)
//   {
//      DE_ERROR("Track id = ", ti.id()," already exist!")
//      return false;
//   }

   auto track = new Track( *this, m_detailStack );
   track->m_trackInfo = ti;
   track->m_dropTarget->setAudioOnly( ti.isAudioOnly() );

   m_tracks.emplace_back( track );

   // 0 = master, 1 = return, 2 = audio-only, 3 = midi+audio
   auto itemV = new ItemV( *this, m_arrangement );
   itemV->setTrack( track );

   if ( ti.m_type == eTrackMaster )
   {
      m_masterTrack = track;
      m_arrangement->m_master = itemV;
   }
   else if ( ti.m_type == eTrackReturn )
   {
      m_returnTracks.emplace_back( track );
      m_arrangement->m_returns.emplace_back( itemV );
   }
   else if ( ti.m_type == eTrackAudio )
   {
      m_userTracks.emplace_back( track );
      m_userAudoTracks.emplace_back( track );
      m_arrangement->m_user.emplace_back( itemV );
   }
   else if ( ti.m_type == eTrackMidi )
   {
      m_userTracks.emplace_back( track );
      m_userMidiTracks.emplace_back( track );
      m_arrangement->m_user.emplace_back( itemV );
   }

   //auto chain = m_app.m_engine.m_network.m_chains[ i ];
   m_detailStack->addWidget( track ); // Qt controls spur deletion now!
   //track->m_ownedAndDeletedByQt = true;

   setActiveTrack( track );

   if ( trace )
   {
      int c = ti.m_plugins.size();
      DE_DEBUG("Add track ",ti.id()," plugins ",c)
      for ( int i = 0; i < c; ++i )
      {
         de::audio::PluginInfo const & pi = ti.m_plugins[ i ];
         addPlugin( pi, false );
         DE_DEBUG("Add plugin[",i,"] = ",dbStr(pi.m_name) )
      }
   }
   else
   {
      for ( de::audio::PluginInfo const & pi : ti.m_plugins )
      {
         addPlugin( pi, false );
      }
   }

   if ( m_arrangement ) m_arrangement->updateLayout();

   if ( wasPlaying )
   {
      playAudioMaster();
   }

   return true;
}


void
App::clearTracks()
{
   m_trackId = -1;
   m_track = nullptr;
   m_masterTrack = nullptr;
   m_returnTracks.clear();
   m_userTracks.clear();
   m_userAudoTracks.clear();
   m_userMidiTracks.clear();

   for ( size_t i = 0; i < m_tracks.size(); ++i )
   {
      auto p = m_tracks[ i ];
      if ( p ) delete p;
   }
   m_tracks.clear();

   m_arrangement->m_master = nullptr;
   m_arrangement->m_returns.clear();
   m_arrangement->m_user.clear();

   int c = m_detailStack->count();
   for ( int i = 0; i < c; i++ )
   {
      auto w = m_detailStack->widget( i );
      if ( w )
      {
         m_detailStack->removeWidget( w );
         delete w;
      }
   }

   m_detailStack->setCurrentIndex( -1 );
}


//int
//App::findTrack( int id ) const
//{
//   for ( size_t i = 0; i < m_tracks.size(); ++i )
//   {
//      auto p = m_tracks[ i ];
//      if ( p && p->id() == id )
//      {
//         //DE_DEBUG("found id = ",id, " at index = ",i)
//         return i;
//      }
//   }

//   //DE_DEBUG("No id = ",id, " found")
//   return -1;
//}

//bool
//App::hasTrack( int id ) const
//{
//   bool ok = findTrack( id );
//   if ( ok )
//   {
//   }

//   //DE_DEBUG("id = ",id, " has = ",ok)
//   return getTrack();
//}

Track*
App::getTrack( int id )
{
   auto it = std::find_if( m_tracks.begin(), m_tracks.end(), [&]
                  (Track* cached) { return cached && cached->id() == id; } );
   if ( it != m_tracks.end() )
   {
      return *it;
   }

   DE_ERROR("id ",id," not found")
   return nullptr;
}

bool
App::addTrack( int id, std::string name, eTrackType tt, QColor color )
{
   DE_DEBUG("Add track id = ",id,", name = ",name,", tt = ",int(tt)," :: START")
   TrackInfo ti;
   ti.m_id = id;
   ti.m_name = name;
   ti.m_type = tt;
   ti.m_color = color;
   return addTrack( ti );
}


bool
App::addPlugin( std::wstring const & uri, bool stopAudio )
{
   if ( !m_track )
   {
      std::wcout << "App::addPlugin() - No m_track " << uri << std::endl;
      return false;
   }

   // FIND PLUGIN uri IN DATABASE
   auto pluginInfo = getPluginInfo( uri );
   if ( !pluginInfo )
   {
      std::wcout << "App::addPlugin() - No plugin info found in db " << uri << std::endl;
      return false;
   }

   return addPlugin( *pluginInfo, stopAudio );
}

bool
App::addPlugin( de::audio::PluginInfo const & pluginInfo, bool stopAudio )
{
   std::string const uri = dbStr(pluginInfo.m_uri);

   if ( !m_track )
   {
      DE_ERROR("No m_track, uri = ", uri )
      return false;
   }

   bool wasPlaying = isAudioMasterPlaying();

   if ( !m_track->addPlugin( pluginInfo, stopAudio ) )
   {
      DE_ERROR("Cant add shell, uri = ", uri )
      //return false;
   }

   updateDspChain();

   // RESTART AUDIO, if any
   if ( wasPlaying )
   {
      playAudioMaster();
   }

   // UPDATE selected
   if ( m_arrangement->m_selected )
   {
      m_arrangement->m_selected->updateFromTrack();
      m_arrangement->update();
   }

   m_detailStack->update();

   return true;
}

void
App::updateDspChain()
{
   if ( !m_masterTrack )
   {
      DE_ERROR( "No m_masterTrack" )
      return;
   }

   // CONNECT user-tracks TO m_userMixer
   size_t k = 0;

   m_userMixer.clearInputSignals();
   m_userMixer.resize( m_userTracks.size() );

   k = 0;
   for ( auto & track : m_userTracks )
   {
      if ( !track ) continue;
      m_userMixer.setInputSignal( k, track );
      k++;
   }

   // CONNECT return-tracks TO m_returnTracks

   m_returnMixer.clearInputSignals();
   m_returnMixer.resize( m_returnTracks.size() );

//   auto nReturn = m_returnTracks.size();
//   if ( m_returnMixer.getInputSignalCount() < nReturn )
//   {
//      std::cout << "App::updateDspChain() :: nReturn = " << nReturn << std::endl;
//      m_returnMixer.resize( nReturn );
//   }

   k = 0;
   for ( auto & track : m_returnTracks )
   {
      if ( !track ) continue;
      m_returnMixer.setInputSignal( k, track );
      k++;
   }

   // CONNECT mixer TO master audio endpoint ( Benni : RtAudioStream )
   m_masterMixer.resize( 2 );
   m_masterMixer.setInputSignal( 0, &m_userMixer );
   m_masterMixer.setInputSignal( 1, &m_returnMixer );

   m_masterTrack->setInputSignal( 0, &m_masterMixer );
   m_masterTrack->updateDspChain();

   m_masterStream.setInputSignal( 0, m_masterTrack );
}

void
App::setActiveTrack( Track* track )
{
   if ( track == m_track )
   {
      DE_DEBUG("Same track")
      //return;
   }

   m_track = track;
   //m_track = getTrack( m_trackId );
   if ( !m_track )
   {
      m_trackId = -1;
      emit activatedTrackId( -1 );
      emit activatedTrack( nullptr );

   }
   else
   {
      m_trackId = m_track->id();
      emit activatedTrackId( m_trackId );
      emit activatedTrack( m_track );

      if ( m_detailStack )
      {
         int found = -1;
         for ( size_t i = 0; i < m_detailStack->count(); ++i )
         {
            auto stackWidget = m_detailStack->widget( i );
            auto stackTrack = reinterpret_cast< Track* >( stackWidget );
            if ( stackTrack == m_track )
            {
               found = int( i );
               break;
            }
         }
         m_detailStack->setCurrentIndex( found );
         auto currentTrack = reinterpret_cast< Track* >( m_detailStack->currentWidget() );
         if ( currentTrack ) currentTrack->updateLayout();
      }


   }
}

IPlugin*
App::getPlugin( int id )
{
   IPlugin* plugin = nullptr;

   if ( !m_track )
   {
      DE_ERROR("No plugin id ",id, ", No current track")
      return plugin;
   }

   // Search/find plugin in current track.
   auto & v = m_track->m_audioShells;
   auto it = std::find_if( v.begin(), v.end(), [&] ( IPlugin* p ) { return p && p->id() == id; } );
   if ( it != v.end() )
   {
      plugin = *it;
   }

   if ( plugin )
   {
      DE_DEBUG("OK plugin id ",id," in current track ", m_track->id() )
   }
   else
   {
      DE_ERROR("No plugin id ",id," in current track ", m_track->id() )
   }

   return plugin;
}


void
App::setActivePluginId( int id )
{
   DE_DEBUG("Active track id = ",id)

   IPlugin* plugin = getPlugin( id );
   if ( plugin )
   {
      m_pluginId = plugin->id();
      m_plugin = plugin;

      emit activatedPluginId( m_pluginId );
      emit activatedPlugin( m_plugin );
   }
}

void
App::setActivePlugin( IPlugin* plugin )
{
   if ( plugin )
   {
      setActivePluginId( plugin->id() );
   }
   else
   {
      setActivePluginId( -1 );
   }
}

bool
App::load()
{
   DE_DEBUG("Load pluginDb ",m_pluginDbXml)
   bool ok = m_pluginDb.load( m_pluginDbXml );
   DE_DEBUG("Database loaded ",m_pluginDb.m_all.m_plugins.size()," plugin(s)")

   m_pluginDirTree->updateFromPluginDb();
   m_pluginInfoTree->updateFromPluginDb();

   DE_DEBUG("Load trackList ",m_trackListXml)
   if ( loadTracks( m_trackListXml ) )
   {
      ok = true;
   }
   else
   {
      ok = false;
      addTrack( 0, "Master", eTrackMaster );
      addTrack( 1000, "A - Return", eTrackReturn );
      addTrack( 1001, "B - Return", eTrackReturn );
      addTrack( 2000, "1 - Audio", eTrackAudio );
      addTrack( 2001, "2 - Audio", eTrackAudio );
      addTrack( 3000, "3 - Midi", eTrackMidi );
      addTrack( 3001, "4 - Midi", eTrackMidi );
      addTrack( 3002, "5 - Midi", eTrackMidi );
      addTrack( 3003, "6 - Midi", eTrackMidi );
   }

   setActiveTrack( getTrack( 0 ) );

   return ok;
}

bool
App::save( bool saveLiveSet )
{
   bool ok = true;

   DE_DEBUG("Save pluginDb")
   ok &= m_pluginDb.save( m_pluginDbXml );

   if ( saveLiveSet )
   {
      ok &= saveTracks( m_trackListXml );
      DE_DEBUG("Save tracks")
   }
   else
   {
      DE_DEBUG("No tracks saved")
   }

   return ok;
}

bool
App::saveTracks( std::string const & uri ) const
{
   dbRemoveFile( uri );

   tinyxml2::XMLDocument doc;
   tinyxml2::XMLElement* root = doc.NewElement( "tracks" );

   int c = int( m_tracks.size() );

   root->SetAttribute( "count", c );

   DE_DEBUG("Saving ",c," track(s)")

   for ( int i = 0; i < c; ++i )
   {
      Track* track = m_tracks[ i ];
      if ( !track )
      {
         DE_ERROR("Cant save track ",c," track(s)")
         continue;
      }

      auto & ti = track->m_trackInfo;
      //ti.m_isBypassed = track->isBypassed();
      //ti.m_volume = track->getVolume();

      for ( int k = 0; k < track->m_audioShells.size(); ++k )
      {
         auto shell = track->m_audioShells[ k ];
         if ( shell )
         {
            if ( ti.m_plugins.size() > k + 1  )
            {
               ti.m_plugins[ k + 1 ] = *shell->pluginInfo();
            }
            else
            {
               DE_ERROR("Plugin shell index error k = ",k)
            }
         }
      }
#if 0
      if ( track->m_audioSynth )
      {
         if ( ti.m_plugins.size() > 0 && ti.m_plugins[ 0 ].isSynth() )
         {
            ti.m_plugins[ 0 ] = *track->m_audioSynth->pluginInfo();
         }
         else
         {
            DE_ERROR("Invalid synth plugin index 0")
         }
      }

      for ( int k = 0; k < track->m_audioEffects.size(); ++k )
      {
         auto fx = track->m_audioEffects[ k ];
         if ( fx )
         {
            if ( ti.m_plugins.size() > k + 1  )
            {
               ti.m_plugins[ k + 1 ] = *fx->pluginInfo();
            }
            else {
               DE_ERROR("Plugin index error k = ",k)
            }
         }
      }
#endif

      DE_DEBUG("Save track ",i+1," with id = ",ti.id()," and ",ti.m_plugins.size()," plugin(s)")
      ti.writeXML( doc, root );
   }

   doc.InsertEndChild( root );

   auto e = doc.SaveFile( uri.c_str() );
   if ( e != tinyxml2::XML_SUCCESS )
   {
      std::cout << "Cant save <tracks> xml " << uri << ", e = " << int(e) << std::endl;
      return false;
   }

   auto n = m_tracks.size();
   std::cout <<"Saved <tracks> xml " << uri << " with " << n << " tracks" << std::endl;
   return true;
}

bool
App::loadTracks( std::string const & uri )
{
   tinyxml2::XMLDocument doc;
   auto e = doc.LoadFile( uri.c_str() );
   if ( e != tinyxml2::XML_SUCCESS )
   {
      std::cout << "Cant load <tracks> xml " << uri << std::endl;
      return false;
   }

   tinyxml2::XMLElement* tracks = doc.FirstChildElement( "tracks" );
   if ( !tracks )
   {
      std::cout << "No <tracks> tag in xml " << uri << std::endl;
      return false;
   }

   int m_checkSum = tracks->IntAttribute( "count" );

   clearTracks();

   int k = 0;

   // Read first child <PluginInfo> of parent <PluginInfoList>
   tinyxml2::XMLElement* trackNode = tracks->FirstChildElement( "track" );
   if ( !trackNode )
   {
      std::cout << "No <track> in <tracks> xml " << uri << std::endl;
      return false;
   }
   else
   {
      // Read all next children <PluginInfo> of parent <PluginInfoList>
      while ( trackNode )
      {
         TrackInfo trackInfo;
         if ( trackInfo.readXML( k, trackNode ) )
         {
            bool ok = addTrack( trackInfo );
            if ( !ok )
            {
               DE_ERROR("No <track> added")
            }
            else
            {
               k++;
            }
         }
         else
         {
            DE_ERROR("Got <track> read error")
         }
         trackNode = trackNode->NextSiblingElement( "track" );
      }
   }

   std::cout <<"<tracks> OK loaded. " << uri << std::endl;
   //std::cout << "[loadXml] dir = " << m_vstDirMB << std::endl;
   std::cout << "<tracks> expect <track> tags = " << m_checkSum << std::endl;
   std::cout << "<tracks> loaded <track> tags = " << k << std::endl;

   return true;
}





void
App::showSpurEditor()
{
   m_isClipEdVisible = false;
   updateLayout();
}

void
App::showClipEditor()
{
   m_isClipEdVisible = true;
   updateLayout();
}

void
App::updateLayout()
{
   if ( m_body )
   {
      m_body->updateLayout();
   }
}

/*

bool
App::loadXml( std::string const & uri )
{

   tinyxml2::XMLDocument doc;
   auto e = doc.LoadFile( uri.c_str() );
   if ( e != tinyxml2::XML_SUCCESS )
   {
      std::cout << "Cant load NetworkData xml " << uri << std::endl;
      return false;
   }

   tinyxml2::XMLElement* root = doc.FirstChildElement( "Network" );
   if ( !root )
   {
      std::cout << "No Network tag in xml " << uri << std::endl;
      return false;
   }

   //m_vstDirMB = root->Attribute( "directory" );
   int m_checkSum = root->IntAttribute( "count" );
   //m_vstDir = QString::fromStdString( m_vstDirMB ).toStdWString();

   m_all.clear();

   // Read first child <PluginInfo> of parent <PluginInfoList>
   tinyxml2::XMLElement* spur = root->FirstChildElement( "Spur" );
   if ( !spur )
   {
      std::cout << "No PluginInfo in xml " << uri << std::endl;
      return true;
   }

   // Read all next children <PluginInfo> of parent <PluginInfoList>
   int k = 0;
   while ( spur )
   {
      SpurData spurData;
      if ( spurData.readXML( k, spur ) )
      {
         m_all.emplace_back( std::move( spurData ) );
         k++;
      }

      spur = spur->NextSiblingElement( "Spur" );
   }

   std::cout <<"Loaded Network XML " << uri << std::endl;
   //std::cout << "[loadXml] dir = " << m_vstDirMB << std::endl;
   std::cout << "[loadXml] expectSpur = " << m_checkSum << std::endl;
   std::cout << "[loadXml] loadedSpur = " << k << std::endl;

   return true;
}

bool
App::saveXml( std::string const & uri ) const
{

   tinyxml2::XMLDocument doc;
   tinyxml2::XMLElement* network = doc.NewElement( "Network" );

   network->SetAttribute( "count", int( m_all.size() ) );

   for ( int i = 0; i < m_all.size(); ++i )
   {
      m_all[ i ].writeXML( doc, network );
   }

   doc.InsertEndChild( network );

   auto e = doc.SaveFile( uri.c_str() );
   if ( e != tinyxml2::XML_SUCCESS )
   {
      std::cout << "Cant save xml " << uri << std::endl;
      return false;
   }

   std::cout <<"Saved NetworkData XML " << uri << std::endl;

   return true;
}

void
createNetwork( TrackInfo const & trackInfo )
{
   int id = trackInfo.m_id;
   int found = findIndex( id );
   if ( found > -1 )
   {
      DE_ERROR("Track id ",id," already exist")
      return nullptr;
   }

   auto track = new Track( m_app );
   track->setTrackInfo( trackInfo );

   m_tracks.emplace_back( track );

   // 0 = master, 1 = return, 2 = audio-only, 3 = midi+audio
   auto type = trackInfo.m_type;
   if ( type == eTrackMaster )
   {
      m_master = track;
   }
   else if ( type == eTrackReturn )
   {
      m_returns.emplace_back( track );
   }
   else if ( type == eTrackAudio )
   {
      m_user.emplace_back( track );
      m_userAudio.emplace_back( track );
   }
   else if ( type == eTrackMidi )
   {
      m_user.emplace_back( track );
      m_userMidi.emplace_back( track );
   }

   return track;
}
void
updateDspChain()
{
   // setBypassed( true );

   if ( m_userMixer.size() < m_user.size() )
   {
      m_userMixer.resize( m_user.size() );
   }

   size_t k = 0;
   for ( auto & track : m_user )
   {
      if ( !track ) continue;
      m_userMixer.setInputSignal( k, track );
      k++;
   }

   k = 0;
   for ( auto & track : m_returns )
   {
      if ( !track ) continue;
      m_returnMixer.setInputSignal( k, track );
      k++;
   }

   m_returnMixer.setInputSignal( k, &m_userMixer );

   m_master->setInputSignal( 0, &m_returnMixer );

   //setBypassed( false );
}

*/

