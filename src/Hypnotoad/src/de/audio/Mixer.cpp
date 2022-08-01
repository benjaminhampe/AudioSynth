#include <de/audio/Mixer.hpp>

namespace de {
namespace audio {

// ===========================================================================
Mixer::Mixer( size_t n )
   : m_mixerVolume(100)
{
   setInputSignalCount( n );
   for ( auto & p : m_inputSignals ) { p = nullptr; }
}
Mixer::~Mixer()
{
   clearInputSignals();
}

/// @brief Called by user to resize mixer
void
Mixer::setInputSignalCount( uint32_t mixerItems )
{
   DSP_RESIZE( m_inputSignals, mixerItems );
}

void
Mixer::setInputSignal( int i, IDspElement* input )
{
   if ( i < 0 || i >= m_inputSignals.size() ) return;
   m_inputSignals[ i ] = input;
}

IDspElement*
Mixer::getInputSignal( uint32_t i ) const
{
   if ( i < 0 || i >= m_inputSignals.size() ) return nullptr;
   return m_inputSignals[ i ];
}

uint32_t
Mixer::getInputSignalCount()
{
   return m_inputSignals.size();
}

void
Mixer::clearInputSignals()
{
   for ( auto & p : m_inputSignals )
   {
      p = nullptr;
   }
}

void
Mixer::setMixerVolume( int vol_in_pc )
{
   m_mixerVolume = std::clamp( vol_in_pc, 0, 200 );
   DE_DEBUG("m_mixerVolume = ",m_mixerVolume," %")
}

int
Mixer::getMixerVolume() const
{
   return m_mixerVolume;
}

/// @brief Called by engine, dsp-element should setup internal buffers now.
void
Mixer::onAudioEngineAboutToStart( uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
{
   uint64_t dstSamples = dstFrames * dstChannels;
   DSP_RESIZE( m_inputBuffer, dstSamples );
   DSP_RESIZE( m_mixBuffer, dstSamples );
   IDspElement::onAudioEngineAboutToStart( dstFrames, dstChannels, dstRate );
}

/// @brief Called by engine, dsp-element can react with fade out or so.
void
Mixer::onAudioEngineAboutToStop( int32_t errorCode )
{
   IDspElement::onAudioEngineAboutToStop( errorCode );
}

uint64_t
Mixer::readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
{
   uint64_t dstSamples = dstFrames * dstChannels;
   DSP_RESIZE( m_inputBuffer, dstSamples );
   DSP_RESIZE( m_mixBuffer, dstSamples );

   size_t n = 0;
   for ( auto & signal : m_inputSignals )
   {
      if ( !signal ) continue;
      DSP_FILLZERO( m_inputBuffer, dstSamples );
      signal->readSamples( pts, m_inputBuffer.data(), dstFrames, dstChannels, dstRate );
      DSP_ADD( m_inputBuffer.data(), m_mixBuffer.data(), dstSamples );
      n++;
   }

   float masterVolume = 1.0e-4f * (m_mixerVolume * m_mixerVolume);
   DSP_COPY_SCALED( m_mixBuffer.data(), dst, dstSamples, masterVolume / float( n ) );
   return dstSamples;
}


} // end namespace gpu.
} // end namespace de.
