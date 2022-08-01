#ifndef DE_LIVE3_TRACKLIST_HPP
#define DE_LIVE3_TRACKLIST_HPP

#include <Track.hpp>

// ============================================================================
struct TrackList //: public ITrack
// ============================================================================
{
   DE_CREATE_LOGGER("Track")
   std::vector< Track > m_tracks;

//   /// Pre effect chain (midi) sent to m_synth.
//   std::vector< de::audio::IMidiChainElement* > m_midiEffects;
//   /// Between MIDI and AUDIO chain, produces audio from midi or audio ( synth or player )
//   de::audio::IDspChainElement* m_synth;
//   /// Last AUDIO chain element, for convenience to auto-connect end with a mixer
//   de::audio::IDspChainElement* m_last;
//   std::vector< de::audio::IDspChainElement* > m_effects; // the rest of the audio chain is a series of effects.

   TrackList() { reset(); }
   ~TrackList() {}

   void reset()
   {
      m_tracks.clear();
//      m_midiEffects.clear();;
//      m_synth = nullptr;
//      m_last = nullptr;
//      m_effects.clear();
   }

   size_t size() const { return m_tracks.size(); }
   size_t trackCount() const { return m_tracks.size(); }
   Track const & track( size_t i ) const { return m_tracks[ i ]; }
   Track & track( size_t i ) { return m_tracks[ i ]; }
   std::vector< Track > const & tracks() const { return m_tracks; }
   std::vector< Track > & tracks() { return m_tracks; }

   void addTrack( Track track )
   {
      m_tracks.emplace_back( std::move( track ) );
   }

//   bool isAudioTrack() const override { return m_isAudioTrack; }
//   int getClipCount() const override { return m_clips.size(); }
//   Clip const & getClip( int i ) const override { return m_clips[ i ]; }
//   Clip & getClip( int i ) override { return m_clips[ i ]; }
//   int getEffectCount() const { return m_effects.size(); }

//   int addPlugin( de::audio::IDspChainElement* plugin, int pos = 0 )
//   {
//      if ( !plugin )
//      int n = m_effects.size();
//      m_effects.emplace_back( plugin );
//      return n;
//   }
};

#endif // G_LOPASS1_HPP
