#ifndef DE_AUDIO_DETUNE_SYNTH_HPP
#define DE_AUDIO_DETUNE_SYNTH_HPP

#include <de/audio/dsp/IDspChainElement.hpp>
//#include <de/audio/dsp/IVoice.hpp>
#include <de/audio/dsp/ADSR.hpp>
#include <de/audio/dsp/MultiOSC.hpp>
#include <array>
#include <vector>
#include <mutex>

namespace de {
namespace audio {

// ============================================================================
class DetuneSynth : public IDspChainElement
// ============================================================================
{
   DE_CREATE_LOGGER("DetuneSynth")
public:
   DetuneSynth();
   ~DetuneSynth();
   bool
   isBypassed() const;
   void
   setBypassed( bool bypass );
   uint64_t
   readSamples(
      double pts,
      float* dst,
      uint32_t dstFrames,
      uint32_t dstChannels,
      uint32_t dstRate ) override;

   bool
   isSynth() const override { return true; }

   void
   sendNote( Note const & note ) override;

   static void
   setOscType( MultiOSCf & osc, int v )
   {
      if ( v == 0 )
      {
         osc.setFunction( MultiOSCf::Sine );
      }
      else if ( v == 1 )
      {
         osc.setFunction( MultiOSCf::Triangle );
      }
      else if ( v == 2 )
      {
         osc.setFunction( MultiOSCf::Rect );
      }
      else if ( v == 3 )
      {
         osc.setFunction( MultiOSCf::Ramp );
      }
      else if ( v == 4 )
      {
         osc.setFunction( MultiOSCf::Saw );
      }
      else if ( v == 5 )
      {
         osc.setFunction( MultiOSCf::Noise );
      }
   }
   bool m_isBypassed;
//   int m_volume;
   //std::vector< float > m_inputBuffer;
   //std::vector< float > m_mixerBuffer;

   MultiOSCf m_osc;
   MultiOSCf m_lfoPhase;
   MultiOSCf m_lfoPulseWidth;
   MultiOSCf m_lfoDetune;
   ADSR m_adsr;


   // Each command triggers a note ( no release event needed, yet, more like single shots )
   // =========================================================
   struct NoteSlot
   // =========================================================
   {
      bool m_isPlaying; // default: true, so it auto starts playing
      bool m_wasPlaying;// default: false, used for sync
      Note m_note;
      MultiOSCf m_osc;
      MultiOSCf m_lfoPhase;
      MultiOSCf m_lfoPulseWidth;
      MultiOSCf m_lfoDetune;

      NoteSlot()
         : m_isPlaying( true )
         , m_wasPlaying( false ) // false triggers a sync with audio thread stream time,
      {}

      float getDuration() const { return m_note.getDuration(); }
   };
   std::array< NoteSlot, 256 > m_NoteSlots;
   std::mutex m_NoteSlotsMutex; // std::atomic< bool >
};

} // end namespace audio
} // end namespace de

#endif // G_OSZILLOSKOP_H
