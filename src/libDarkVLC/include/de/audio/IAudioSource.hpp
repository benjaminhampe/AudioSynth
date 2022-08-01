#ifndef DE_AUDIO_IAUDIOSOURCE_HPP
#define DE_AUDIO_IAUDIOSOURCE_HPP

#include <de/audio/SampleBuffer.hpp>

namespace de {
namespace audio {

struct IAudioSourceListener
{
   virtual ~IAudioSourceListener() {}

   /// @brief
   virtual void about2play() = 0;

   /// @brief
   virtual void about2stop() = 0;

   /// @brief
   virtual void about2pause() = 0;

};

struct IAudioSource
{
   virtual ~IAudioSource() {}

   /// @brief Caller presents a buffer to be filled with samples, not reformatted or resized.
   /// @attention Only and most important function of backward DSP chain, thatswhy abstract, must be implemented.
   /// @param dst A writable destination buffer to be filled with samples, not reformatted or resized.
   /// @param pts A presentation time stamp in [s], not used in audio only yet. Maybe important for audio+video sync that run in separate threads.
   virtual uint64_t
   readSamples( SampleBuffer & dst, double pts ) = 0;

   /// @brief Volume in % percent [pc], 0 means 0ff, 100 = def. max., >200 start killing your ears and pets.
   virtual int
   getVolume() const { return 100; }

   /// @brief Volume in % percent [pc], 0 means 0ff, vlc does up to 200 or 400 % for very silent audio recordings needed on YT.
   virtual void
   setVolume( int vol_in_percent ) {}

   /// @brief Return Unique FileName of AudioSource, if any
   virtual std::string
   getUri() const { return "IAudioSource"; }

   /// @brief Is the source loaded and ready to play? Asked by caller like the SoundCard
   virtual bool
   is_loaded() const { return true; }

   /// @brief Is the source playing? Asked by caller like the SoundCard
   virtual bool
   is_playing() const { return true; }

   /// @brief Starts from begin if was stopped or resumes at last pos when was paused.
   virtual void
   play() {}

   /// @brief Wishes to stop with fade out or at next beat grid border?
   virtual void
   stop() {}

   /// @brief Wishes to stop with A.S.A.P. fade out.
   virtual void
   panic() {}

   /// @brief Is the source paused? Not asked by SoundCard. Only some info bar?
   virtual bool
   is_paused() const { return false; }

   /// @brief Wishes to stop with fade out and staying at same position.
   ///        The source advances while fade out a bit, so you dont resume at the same pos you pressed the button
   virtual void
   pause() {}

   /// @brief Is the source looping?
   virtual bool
   is_looping() const { return false; }

   virtual int32_t
   getLoops() const { return 0; }

   virtual void
   loop( int32_t loops ) {}

   virtual void
   setLoops( int32_t loops ) {}

   // virtual bool
   // is_seekable() const = 0;

   // virtual int64_t
   // getDurationInNs() = 0; chrono steady_clock native time type

   // virtual double
   // getDurationInSec() = 0;

   // virtual void
   // setPosition( uint64_t frame = 0, int dir = 0 ) = 0;

   // virtual void
   // seekFrame( uint64_t frame = 0, int dir = 0 ) = 0;

   // virtual uint64_t
   // tellFrame() = 0;

   /// @brief The audio source is a DSP element in an arbitrary long (unidirectional) chain.
   /// The DSP element is expected to write samples to the next element's 'dst' buffer.
   /// The external caller did already format buffer 'dst' in the format the caller expects.
   /// The 'dst' buffer gives all info about expected SampleRate/Type and Frame/ChannelCount.
   /// DONT change format or size of param 'dst', only fill new samples!
   /// DONT expect the given buffer to be zeroed out for feedback effect reasons.
   /// So, your DSP element has to convert its data if needed to the requested output/dest format.
   /// If the DSP element is a Resampler it can convert to correct SampleRate
   /// If the DSP element is a Converter it can convert to correct SampleType
   /// If the DSP element is a FourierTransformer it can filter frequencies.
   ///
   /// @return Number of samples ( nFrames * nChannels ), not number of frames only.
   /// Dont know yet if its better for DSP chains.
   /// Covers and encapsulates supporting calls like:
   /// - getSampleRate()
   /// - getSampleType()
   /// - getFrameCount()
   /// - getChannelCount()
   /// - getDuration()
   /// e.g. You dont really need isInterleaved() anymore,
   /// just access 2d sample buffer like an 2d image(x=frame, y=channel)
   /// using getSamples() or getSampleF32 or getSampleS16 if you have these common sample types.
};

} // end namespace audio
} // end namespace de

#endif

