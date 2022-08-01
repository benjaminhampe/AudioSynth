#ifndef DE_LIVE_APP_HPP
#define DE_LIVE_APP_HPP

#include "LiveSkin.hpp"

#include <de/audio/engine/MixerStream.hpp>
#include "BeatGrid.hpp"
#include "Clip.hpp"
#include "Track.hpp"
#include "TrackList.hpp"

#include <de/audio/plugin/GetPluginInfoList.hpp>

// ============================================================================
struct App
// ============================================================================
{
   App();
   ~App();
   void reset();
   DE_CREATE_LOGGER("App")
   LiveSkin m_skin;
   BeatGrid m_beatGrid;
   de::audio::MixerStream m_audioEndPoint;

   TrackList m_trackList;

   de::audio::PluginInfoList m_pluginDb; // plugin database

   //   static App &
   //   get()
   //   {
   //      static App s_instance;
   //      return s_instance;
   //   }
};

#endif // G_LOPASS1_HPP
