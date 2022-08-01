#ifndef DE_AUDIO_DSP_IDSPINPUT_HPP
#define DE_AUDIO_DSP_IDSPINPUT_HPP

#include <de/audio/dsp/ShiftBuffer.hpp>
#include <functional>

namespace de {
namespace audio {

// IStereoSignal
//template < typename T >
//struct IDSP
//{
//   /// @brief Caller presents a buffer to be filled with samples to build backward DSP chains.
//   /// @param pts The presentation time stamp in [s]
//   /// @param dst The destination buffer to be filled with dstSampleCount = dstFrames * dstChannels.
//   /// @return Should return the num expected samples in 'dst' buffer = dstFrames * dstChannels.
//   ///         If returns less then caller has to react, fade out, fill with zeroes or so.
//   ///         But the source is expected to deliver zeroes or something else on its own in error cases.
//   ///         So normally we expect the callee to fill everything and we dont have to care in most cases.
//   uint64_t
//   readSamples( T pts, T* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate );

//   /// @brief Caller presents a buffer to be filled with samples to build backward DSP chains.
//   /// @param pts The presentation time stamp in [s]
//   /// @param dst The destination buffer to be filled with dstSampleCount = dstFrames * dstChannels.
//   void
//   initDsp( uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate );

//   bool
//   isBypassed() const;
//   void
//   setBypassed( bool enabled );

//   void
//   clearInputSignals();

//   void
//   setInputSignal( int i, IDSP* input );

//   uint32_t
//   getSampleRate() const;
//};

//typedef IDSP< float > IDSP32;
//typedef IDSP< double > IDSP64;

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

struct IDspChainElement // The name is stupid but easy to read and find in code.
{
   virtual ~IDspChainElement() {}

   /// @brief Caller presents a buffer to be filled with samples to build backward DSP chains.
   /// @param pts The presentation time stamp in [s]
   /// @param dst The destination buffer to be filled with dstSampleCount = dstFrames * dstChannels.
   /// @return Should return the num expected samples in 'dst' buffer = dstFrames * dstChannels.
   ///         If returns less then caller has to react, fade out, fill with zeroes or so.
   ///         But the source is expected to deliver zeroes or something else on its own in error cases.
   ///         So normally we expect the callee to fill everything and we dont have to care in most cases.
   virtual uint64_t
   readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate ) = 0;

   /// @brief Caller presents a buffer to be filled with samples to build backward DSP chains.
   /// @param pts The presentation time stamp in [s]
   /// @param dst The destination buffer to be filled with dstSampleCount = dstFrames * dstChannels.
   virtual void
   setupDspElement( uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate ) {}


//   virtual bool
//   isBypassed() const { return false; }
//   virtual void
//   setBypassed( bool enabled ) {}

//   virtual void
//   clearInputSignals() {}
//   virtual void
//   setInputSignal( int i, de::audio::IDspChainElement* input ) {}


   virtual uint32_t
   getSampleRate() const { return 44100; }
};

/*
   struct IMultiChannelFrequencySignal
   {
      virtual ~IMultiChannelFrequencySignal() {}

      /// @brief Caller presents a buffer to be filled with samples to build backward DSP chains.
      /// @param pts The presentation time stamp in [s]
      /// @param dst The destination buffer to be filled with dstSampleCount = dstFrames * dstChannels.
      virtual uint64_t
      readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate ) = 0;

   };

   /// @brief Is the source playing? Asked by caller like the SoundCard
   virtual bool
   is_playing() const { return true; }

   /// @brief Starts from begin if was stopped or resumes at last pos when was paused.
   virtual void
   dsp_play() {}

   /// @brief Wishes to stop with fade out or at next beat grid border?
   virtual void
   dsp_stop() {}

   /// @brief Is the source paused? Not asked by SoundCard. Only some info bar?
   virtual bool
   dsp_is_paused() const { return false; }

   /// @brief Wishes to stop with fade out and staying at same position.
   ///        The source advances while fade out a bit, so you dont resume at the same pos you pressed the button
   virtual void
   dsp_pause() {}

   /// @brief Is the source looping?
   virtual bool
   dsp_is_looping() const { return false; }

   virtual int32_t
   dsp_loops() const { return 0; }

   virtual void
   dsp_loop( int32_t loops ) {}

   virtual void
   dsp_set_loops( int32_t loops ) {}
*/

} // end namespace audio
} // end namespace de

#endif

