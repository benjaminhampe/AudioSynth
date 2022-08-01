#ifndef DE_AUDIO_BETTERSYNTH_HPP
#define DE_AUDIO_BETTERSYNTH_HPP

#include <de/audio/dsp/IDspChainElement.hpp>
#include <de/audio/dsp/MultiOSC.hpp>
#include <de/audio/dsp/ADSR.hpp>
#include <array>
#include <vector>
#include <mutex>

namespace de {
namespace audio {

// ============================================================================
struct BetterVoice
// ============================================================================
{
   DE_CREATE_LOGGER("BetterVoice")
   bool m_isBypassed;
   //int m_volume;
   int m_midiNote; // base 0..127 midi note offset
   int m_fineTune; // 0..64 = -100 .. 0 cents
   float m_basePhase;

   MultiOSCf m_osc;
   MultiOSCf m_lfoPhase;
   MultiOSCf m_lfoPulseWidth;
   MultiOSCf m_lfoDetune;
   ADSR m_adsr;

   BetterVoice()
      : m_isBypassed( false )
      //, m_volume( 100 )
      , m_midiNote( 0 ) // offset midiNote not absolute
      , m_fineTune( 0 ) // in cents
      , m_basePhase( 0 ) // in [0,1]
   {
      m_lfoPhase.setBypassed( true );
      m_lfoPhase.setBypassValue( 0.0f );

      m_lfoPulseWidth.setBypassed( true );
      m_lfoPulseWidth.setBypassValue( 0.5f );

      m_lfoDetune.setBypassed( true );
      m_lfoDetune.setBypassValue( 0.0f );
   }

   ~BetterVoice()
   {}

   bool isBypassed() const { return m_isBypassed; }
   void setBypassed( bool bypass ) { m_isBypassed = bypass; }

   void setOscType( int v ) { m_osc.setFunction( v ); }

   void setPhaseLfoType( int v ) { m_lfoPhase.setFunction( v ); }
   void setPulseWidthLfoType( int v ) { m_lfoPulseWidth.setFunction( v ); }
   void setDetuneLfoType( int v ) { m_lfoDetune.setFunction( v ); }
   void setDetune( int v ) { m_midiNote = v; }
   void setFineDetune( int v ) { m_fineTune = v; }
   void setVolume( int v ) { m_osc.setVolume( v ); }
   //void setVelocity( int v ) { m_velocity = v; }
   // Play the OSC using rudimentary midi data:
   void // uint64_t
   addSamples(
      int midiNote, int velocity,
      double t, double dt, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
   {
      if ( m_isBypassed ) return;

      float volume = velocity * float(1.0 / 127.0);

      //uint64_t dstSamples = dstFrames * dstChannels;
      //double dt = 1.0 / double( dstRate );
      auto pDst = dst;

      float fNoteBase = float(midiNote + m_midiNote) + 0.01f * (m_fineTune);
      float fMidiNote = fNoteBase + m_lfoDetune.computeSample( t );
      float freq = getFrequencyFromMidiNote( fMidiNote );

      for ( size_t i = 0; i < dstFrames; i++ )
      {
         float phase = m_lfoPhase.computeSample( t );
         float pw = m_lfoPulseWidth.computeSample( t );
         float adsr = m_adsr.computeSample( t );
         m_osc.setFrequency( freq );
         m_osc.setPhase( phase );
         m_osc.setPulseWidth( pw );
         float sample = m_osc.computeSample( t ) * adsr * volume;

         for ( size_t c = 0; c < dstChannels; c++ )
         {
            *pDst += sample;
            pDst++;
         }
         t += dt;
      }

      //return dstSamples;
   }
};

// ============================================================================
struct BetterSynth : public IDspChainElement
// ============================================================================
{
   DE_CREATE_LOGGER("BetterSynth")
public:
   BetterSynth();
   ~BetterSynth();

   bool isBypassed() const;
   void setBypassed( bool bypass );

   uint64_t
   readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate ) override;

   bool
   isSynth() const override { return true; }

   void
   sendNote( Note const & note ) override;

   bool m_isBypassed;
//   int m_volume;
   //std::vector< float > m_inputBuffer;
   //std::vector< float > m_mixerBuffer;

   // Each command triggers a note ( no release event needed, yet, more like single shots )
   // =========================================================
   struct NoteSlot
   // =========================================================
   {
      bool m_isPlaying; // default: true, so it auto starts playing
      bool m_wasPlaying;// default: false, used for sync
      float m_duration;
      Note m_note;
      std::array< BetterVoice, 8 > m_voices;

      NoteSlot()
         : m_isPlaying( true )
         , m_wasPlaying( false ) // false triggers a sync with audio thread stream time,
         , m_duration( 0 )
      {}

      float getDuration() const { return m_duration; }

      void computeDuration()
      {
         m_duration = 0.0f;
         for ( auto & voice : m_voices )
         {
            if ( voice.isBypassed() ) continue;
            m_duration = std::max( m_duration, voice.m_adsr.getDuration() );
         }
      }
   };

   std::array< BetterVoice, 8 > m_voices;
   std::array< NoteSlot, 32 > m_notes;

   std::mutex m_mutex; // std::atomic< bool >
};

} // end namespace audio
} // end namespace de


#endif // G_OSZILLOSKOP_H
