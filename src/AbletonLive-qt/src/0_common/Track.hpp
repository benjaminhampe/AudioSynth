#ifndef DE_LIVE3_TRACK_HPP
#define DE_LIVE3_TRACK_HPP

#include <Clip.hpp>

/*
// ============================================================================
struct ITrack : public de::audio::IDspChainElement
// ============================================================================
{
   virtual ~ITrack() = default;

   virtual bool isAudioTrack() const = 0;

   virtual int getClipCount() const = 0;
   virtual Clip const & getClip( int i ) const = 0;
   virtual Clip & getClip( int i ) = 0;

   virtual int getEffectCount() const = 0;

};
*/
// A track manages one Audio DSP chain and several Midi Clips on a timeline.
// ============================================================================
struct Track //: public ITrack
// ============================================================================
{
   DE_CREATE_LOGGER("Track")
   bool m_isAudioTrack;
   // MidiProducer < only for MidiTrack > .
   std::vector< Clip > m_clips;
   // MidiEffectProcessors < only for MidiTrack >
   std::vector< de::audio::IMidiChainElement* > m_midiEffects;

   // AudioProducer ( player for AudioTrack and synth for MidiTrack ).
   de::audio::IDspChainElement* m_audioBeg;
   // AudioChainEnd, for convenience to auto-connect end with a mixer
   de::audio::IDspChainElement* m_audioEnd;
   std::vector< de::audio::IDspChainElement* > m_audioEffects; // the rest of the audio chain is a series of effects.

   Track() { reset(); }
   ~Track() {}

   void reset()
   {
      m_isAudioTrack = false;
      m_clips.clear();
      m_audioBeg = nullptr;
      m_audioEnd = nullptr;
//      m_midiEffects.clear();;
//      m_synth = nullptr;
//      m_last = nullptr;
//      m_effects.clear();
   }

   size_t size() const { return m_clips.size(); }
   size_t clipCount() const { return m_clips.size(); }
   Clip const & clip( size_t i ) const { return m_clips[ i ]; }
   Clip & clip( size_t i ) { return m_clips[ i ]; }
   std::vector< Clip > const & clips() const { return m_clips; }
   std::vector< Clip > & clips() { return m_clips; }

   void addClip( Clip clip )
   {
      m_clips.emplace_back( std::move( clip ) );
   }

   void addClip( double timeBeg, double timeEnd, std::string name = "Clip" )
   {
      Clip clip;
      clip.m_timeBeg = timeBeg;
      clip.m_timeEnd = timeEnd;
      clip.m_name = name;
      addClip( clip );
   }
//   bool isAudioTrack() const override { return m_isAudioTrack; }
//   int getClipCount() const override { return m_clips.size(); }
//   Clip const & getClip( int i ) const override { return m_clips[ i ]; }
//   Clip & getClip( int i ) override { return m_clips[ i ]; }
//   int getEffectCount() const { return m_effects.size(); }

   void
   addAudioPlugin( de::audio::IDspChainElement* plugin, bool reconnect = true )
   {
      if ( !plugin ) return;

      if ( plugin->isSynth() )
      {
         if ( m_audioBeg )
         {
            delete m_audioBeg;
         }

         m_audioBeg = plugin;

         std::cout << "Set Audio Synth" << std::endl;
      }
      else
      {
         m_audioEffects.emplace_back( plugin );
         std::cout << "Set Audio Effect " << m_audioEffects.size() << std::endl;
      }

      if ( reconnect ) reconnectDspChain();
   }

   void reconnectDspChain()
   {
      m_audioEnd = nullptr;

      if ( !m_audioBeg )
      {
         std::cout << "No synth!" << std::endl;
         return;
      }

      // Connect dsp chain
      m_audioEnd = m_audioBeg;
      for ( int i = 0; i < m_audioEffects.size(); ++i )
      {
         auto fx = m_audioEffects[ i ];
         fx -> setInputSignal( 0, m_audioEnd );
         m_audioEnd = fx;
      }

   }

};

#endif // G_LOPASS1_HPP
