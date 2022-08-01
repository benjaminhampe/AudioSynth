#ifndef DE_AUDIO_I_DSP_ELEMENT_HPP
#define DE_AUDIO_I_DSP_ELEMENT_HPP

#include <cstdint>
#include <sstream>
#include <vector>
#include <array>
#include <functional>
#include <de/Logger.hpp>

namespace de {
namespace audio {

// Reverse/Backward DSP chain. Mixer starts calling end of DSP chain and streams then to SoundCard.
// Must implement readSamples() to take part in DSP chain
// Must deliver samples:
//                       a synth or player
// or must passthrough samples:
//                       implements addInputSignal( int i, IDspChainElement* elem )
//                       for any effect like mixer, fx, lfo, bassboost, etc...

// Samples must be:
//            - 32bit floats, (maybe 64bit doubles in the future)
//            - in interleaved order [ch0,chN,ch0,chN,etc...]
//              not planar order [ch0,ch0,chN,chN,etc...]!!!
//            - Also little endian i guess?
//
// Return must be dstSamples = dstFrames * dstChannels or error is indicated.
//
// SampleRate is given for special effects like SoundTouch, reverb, RIAA filter or so on.
// Most effects dont react on sampleRate, so ignore it if not used.
// In callback you should resize memory only when absolutely neccessary ( once at first call, better would be in ctr or before audio starts )
// AudioCallback should inform when bufferSize changed. -> resize dsp chain, recalc latency
// AudioCallback should inform when sampleRate changed. -> recalc dsp chain, recalc latency
// AudioCallback should inform when connection lost.    -> todo what?
// AudioCallback should inform when connection reopened.-> todo what?

struct IDspElement // The name is stupid but easy to read and find in code.
{
   virtual ~IDspElement() {}

   /// @brief Caller presents a buffer to be filled with samples to build backward DSP chains.
   /// @param pts The presentation time stamp in [s]
   /// @param dst The destination buffer to be filled with dstSampleCount = dstFrames * dstChannels.
   /// @return Should return the num expected samples in 'dst' buffer = dstFrames * dstChannels.
   ///         If returns less then caller has to react, fade out, fill with zeroes or so.
   ///         But the source is expected to deliver zeroes or something else on its own in error cases.
   ///         So normally we expect the callee to fill everything and we dont have to care in most cases.
   virtual uint64_t
   readSamples(
      double pts,
      float* dst,
      uint32_t dstFrames,
      uint32_t dstChannels,
      uint32_t dstRate ) = 0;

   /// [INPUTS] For processors and effects that process an input.

   /// @brief Return maximal possible number of input slots, a mixer has 8 or so, most elements have one input.
   virtual IDspElement*
   getInputSignal( uint32_t i ) const { return nullptr; } // empty for pure audio sources like players or synths.

   /// @brief Return maximal possible number of input slots, a mixer/adder has 2-128 or so, most elements have one input and are relays/processors.
   virtual uint32_t
   getInputSignalCount() { return 0; } // empty for pure audio sources like players or synths.

   /// @brief Disconnect all input signals.
   virtual void
   clearInputSignals() {} // empty for pure audio sources like players or synths.

   /// @brief Make this DSP element a processor by accepting input signals.
   virtual void
   setInputSignal( int index, IDspElement* elem ) {} // empty for pure audio sources like players or synths.


   /// [OUTPUTS] for something like recorder or feedback loops

   /// @brief Return maximal possible number of input slots, a mixer/adder has 2-128 or so, most elements have one input and are relays/processors.
   virtual uint32_t
   getOutputSignalCount() { return 0; } // empty for pure audio sources like players or synths.

   /// @brief Disconnect all output signals.
   virtual void
   clearOutputSignals() {} // empty for pure audio sources like players or synths.

   /// @brief Make this DSP element a processor by accepting input signals.
   virtual void
   setOutputSignal( int index, IDspElement* elem ) {} // empty for pure audio sources like players or synths.

   /// @brief Get sampleRate of this dsp-element, if any,
   /// Give your next plugin ( foreward element ) a chance by telling it your sample-rate is the idea.
   /// A player should return here the sample-rate of the currently loaded audio file ( inputSide ).
   /// A synth should return here its sample-rate ( inputSide ).
   /// A processor like SoundTouch should return the sampleRate of the input side ( inputSide, backward side of dsp-chain )
   /// A mixer has to choose one rate for all varying input sources, a mixer returns the common rate here.
   /// A simple relay just returns the rate from its one and only input source.
   virtual uint32_t
   getSampleRate() const
   {
      return getInputSignal(0) ? getInputSignal(0)->getSampleRate() : 0;
   }

   /// @brief Called by engine, dsp-element should setup internal buffers now.
   virtual void
   onAudioEngineAboutToStart( uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
   {
      for ( size_t i = 0; i < getInputSignalCount(); ++i )
      {
         auto p = getInputSignal( i );
         if ( p ) p->onAudioEngineAboutToStart( dstFrames, dstChannels, dstRate );
      }
   }

   /// @brief Called by engine, dsp-element can react with fade out or so.
   virtual void
   onAudioEngineAboutToStop( int32_t errorCode )
   {
      for ( size_t i = 0; i < getInputSignalCount(); ++i )
      {
         auto p = getInputSignal( i );
         if ( p ) p->onAudioEngineAboutToStop( errorCode );
      }
   }

};

// IAudioSource
// IAudioProducer
// ISoundProducer
// ISoundProcessor
// IFunElement
// IMusiker
// IKrachProducer
// ITierschreck
// IHundepfeife
// IDogWhistler
// IOhrenReiniger
// INeighborVisitProducer
// IBeeingMurderedEnhancer
// IDanceEnhancer
// IKaraoke
// IEntarteteKunst
// ISonicPollution
// IEarStress
// IEarPain
// IMultiChannelAudioSignalChainElement
// IBackwardDspChainElement
// IDSP,
// IDspElem
// IAudioSignal,
// ITimeSignal ( compared to complex frequency signals after fft )
// IMultiChannelAudioTimeSignal
// IStereoSignal


template < typename T >
inline void
DSP_RESIZE( std::vector< T > & dst, uint64_t dstSamples )
{
   if ( dstSamples != dst.size() ) dst.resize( dstSamples );
}

template < typename T >
inline void
DSP_FILLZERO( std::vector< T > & dst )
{
   auto p = dst.data();
   for ( size_t i = 0; i < dst.size(); ++i ) { *p = T(0); p++; }
}

inline void
DSP_FILLZERO( std::vector< float > & dst, uint64_t sampleCount )
{
   auto p = dst.data();
   sampleCount = std::min( sampleCount, dst.size() );
   for ( size_t i = 0; i < sampleCount; ++i ) { *p = 0.0f; p++; }
}

inline void
DSP_FILLZERO( float* dst, uint64_t sampleCount )
{
   if ( !dst ) return;
   if ( sampleCount < 1 ) return;
   for ( size_t i = 0; i < sampleCount; ++i ) { *dst = 0.0f; dst++; }
}

inline void
DSP_FILLZERO( double* dst, uint64_t sampleCount )
{
   if ( !dst ) return;
   if ( sampleCount < 1 ) return;
   for ( size_t i = 0; i < sampleCount; ++i ) { *dst = 0.0; dst++; }
}

inline void
DSP_GET_CHANNEL(
   float* dst, uint64_t dstFrames,
   float const* src, uint64_t srcFrames, uint32_t srcChannel, uint32_t srcChannels )
{
   float* pDst = dst;
   float const* pSrc = src + srcChannel;
   auto n = std::min( srcFrames, dstFrames );
   for ( size_t i = 0; i < n; i++ )
   {
      *pDst = (*pSrc);
      pDst++;
      pSrc += srcChannels; // jump to next sample of srcChannel.
   }
}

inline void
DSP_COPY( float const* src, float* dst, uint64_t sampleCount )
{
   for ( size_t i = 0; i < sampleCount; ++i )
   {
      *dst = (*src);
      dst++;
      src++;
   }
}

inline void
DSP_ADD( float const* src, float* dst, uint64_t sampleCount )
{
   for ( size_t i = 0; i < sampleCount; ++i )
   {
      *dst += (*src);
      dst++;
      src++;
   }
}

inline void
DSP_COPY_LIMITED( float const* src, float* dst, uint64_t sampleCount )
{
   for ( size_t i = 0; i < sampleCount; ++i )
   {
      *dst = std::clamp( *src, -1.0f, 1.0f ); // Limiter.
      dst++;
      src++;
   }
}

inline void
DSP_COPY_SCALED( float const* src, float* dst, uint64_t sampleCount, float scale )
{
   for ( size_t i = 0; i < sampleCount; ++i )
   {
      *dst = (*src) * scale;
      dst++;
      src++;
   }
}

inline void
DSP_COPY_SCALED_LIMITED( float const* src, float* dst, uint64_t sampleCount, float scale )
{
   for ( size_t i = 0; i < sampleCount; ++i )
   {
      *dst = std::clamp( (*src) * scale, -1.0f, 1.0f ); // Limiter.
      dst++;
      src++;
   }
}

inline void
DSP_ADD( float* dst, uint64_t sampleCount, float noise )
{
   for ( size_t i = 0; i < sampleCount; ++i )
   {
      *dst += noise;
      dst++;

   }
}

//inline void
//DSP_PROC( float const* src, float* dst, uint32_t sampleCount, std::function<float(float,float)> const & manipSample )
//{
//   for ( size_t i = 0; i < sampleCount; ++i )
//   {
//      *dst = manipSample(*src,*dst);
//      dst++;
//      src++;
//   }
//}

} // end namespace audio
} // end namespace de

#endif

