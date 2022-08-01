#include "Mixer.hpp"

namespace de {
namespace audio {

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

bool
Mixer::isBypassed() const { return m_isBypassed; }

void
Mixer::setBypassed( bool bypassed ) { m_isBypassed = bypassed; }

void
Mixer::aboutToStart( uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
{
   uint64_t dstSamples = dstFrames * dstChannels;
   DSP_RESIZE( m_inputBuffer, dstSamples );
}

uint64_t
Mixer::readSamples( double pts,
                    float* dst,
                    uint32_t dstFrames,
                    uint32_t dstChannels,
                    uint32_t dstRate )
{
   uint64_t dstSamples = dstFrames * dstChannels;
   DSP_RESIZE( m_inputBuffer, dstSamples );

   size_t n = 0;
   for ( auto & signal : m_inputSignals )
   {
      if ( !signal ) continue;
      DSP_FILLZERO( m_inputBuffer, dstSamples );
      signal->readSamples( pts, m_inputBuffer.data(), dstFrames, dstChannels, dstRate );
      DSP_ADD( m_inputBuffer.data(), dst, dstSamples );
      n++;
   }

   float masterVolume = 1.0e-4f * (m_mixerVolume * m_mixerVolume);
   DSP_MUL( dst, dstSamples, masterVolume );
   return dstSamples;
}

void
Mixer::clearInputSignals()
{
   for ( auto & p : m_inputSignals ) { p = nullptr; }
}

void
Mixer::setInputSignal( int i, IDspChainElement* input )
{
   if ( i < 0 || i >= int( m_inputSignals.size() ) ) return;
   m_inputSignals[ i ] = input;
}

uint32_t
Mixer::getInputSignalCount() const { return m_inputSignals.size(); }

IDspChainElement*
Mixer::getInputSignal( uint32_t i ) const
{
   if ( i < 0 || i >= m_inputSignals.size() ) return nullptr;
   return m_inputSignals[ i ];
}

void
Mixer::setInputSignalCount( uint32_t mixerItems )
{
   DSP_RESIZE( m_inputSignals, mixerItems );
}

int
Mixer::getVolume() const { return m_mixerVolume; }

void
Mixer::setVolume( int vol_in_pc )
{
   m_mixerVolume = std::clamp( vol_in_pc, 0, 200 );
   DE_DEBUG("m_mixerVolume = ",m_mixerVolume," %")
}

} // end namespace audio.
} // end namespace de.
