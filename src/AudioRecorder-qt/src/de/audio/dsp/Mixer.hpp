#ifndef DE_AUDIO_DSP_ELEMENT_MIXER_N_ADDER_HPP
#define DE_AUDIO_DSP_ELEMENT_MIXER_N_ADDER_HPP

#include <de/audio/dsp/IDspChainElement.hpp>
#include <thread>
#include <atomic>
#include <condition_variable>

namespace de {
namespace audio {

// ===========================================================================
struct Mixer : public IDspChainElement
// ===========================================================================
{
   DE_CREATE_LOGGER("de.audio.Mixer")
   bool m_isBypassed;
   int m_mixerVolume;
   //std::array< int, N > m_inputVolumes;   // Record buffer for microphone
   std::vector< IDspChainElement* > m_inputSignals;   // Record buffer for microphone
   std::vector< float > m_inputBuffer; // Single input buffer?
   //std::vector< float > m_mixBuffer;   // Mix buffers

   Mixer( size_t n = 16 );
   ~Mixer();
   bool isBypassed() const;
   void setBypassed( bool bypassed );

   /// @brief Called by audio callback.
   void setupDspElement(
      uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate ) override;

   /// @brief Called by audio callback.
   uint64_t readSamples( double pts, float* dst,
      uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate ) override;

   /// @brief Disconnect all input signals.
   void clearInputSignals() override;

   /// @brief Make this DSP element a processor by accepting input signals.
   void setInputSignal( int i, IDspChainElement* input ) override;

   /// @brief Return maximal possible number of input slots, a mixer/adder has 2-128 or so, most elements have one input and are relays/processors.
   uint32_t getInputSignalCount() const override;

   /// @brief Return input slot, a mixer has 8 or so, most elements have one input.
   IDspChainElement*
   getInputSignal( uint32_t i ) const override;

   /// @brief Called by user to resize mixer
   void setInputSignalCount( uint32_t mixerItems );

   /// @brief Mixer volume quadratic 50% means 0.25 and 200% means 4.0
   int getVolume() const;
   void setVolume( int vol_in_pc );


   /// @brief Called by engine, dsp-element can react with fade out or so.
   //void
   //audioEngineAboutToStop( int32_t errorCode ) override;

};

} // end namespace audio.
} // end namespace de.

#endif
