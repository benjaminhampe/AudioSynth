#ifndef DE_AUDIO_MULTI_OSC_HPP
#define DE_AUDIO_MULTI_OSC_HPP

#include <de/audio/dsp/OSC_Sine.hpp>
#include <de/audio/dsp/ADSR.hpp>
#include <functional>

namespace de {
namespace audio {

struct MultiOSCf // : public IOSC< float >
{
   enum eType
   {
      Sine = 0,
      Triangle,
      Rect,
      Ramp,
      Saw,
      Noise,
      Organ,
      Piano,
      AnalogRect,
      AnalogSaw,
      eTypeCount
   };

   typedef float T;

   DE_CREATE_LOGGER("de.audio.MultiOSCf")
   bool m_isBypassed;
   eType m_type;
   int m_volume;
   T m_freq;
   T m_period;
   T m_amp;
   T m_phase;
   T m_pulseWidth; // for rect
   T m_bypassValue;
   T m_center;
   T m_range;
   // T t, T freq, T period, T phase = T(0), T pulseWidth = T(0.5)
   std::function< T( T,T,T,T,T ) > m_func;

   static std::string
   getFunctionString( int typ );

   std::string
   getFunctionString() const;

   uint32_t
   getTypeCount() const;

   std::string
   toString() const;

   MultiOSCf();
   bool isBypassed() const;
   void setBypassed( bool bypassed );
   void setBypassValue( T defaultValue );
   void toggleBypassed();
   int getFunction() const;
   void setFunction( int typ );
   // Gain/Volume
   int getVolume() const;
   void setVolume( int volume );

   // Total frequency = getFreqFromMidiNote( midiNote + detune_in_cents/100 ) + lfo + etc...
   T getFrequency() const;
   void setFrequency( T freq );

   // Total phase in range [0,1]
   T getPhase() const;
   void setPhase( T phase );

   // Total pulseWidth in range [0,1], default: 0.5, near 0 or 1 its really impulse or noise like
   T getPulseWidth() const;
   void setPulseWidth( T pulseWidth );

   // Center of oscillator, default: 0.0 = neutral addition
   T getCenter() const;
   void setCenter( float center );

   // Range of oscillator, default: 1.0 = neutral multiplication
   T getRange() const;
   void setRange( float range );

//   |  /|   | ma = m_amp / (pulseWidth * m_period)
//   | / |   | mb = m_amp / ((1.0 - pulseWidth) * m_period)
//  _|/  |   |
//   | a | b/| pulseWidth = a / (a + b), T = 1 / f
//   |   | / |
//   |   |/  |   Like two mirrored ADSR without release phase,
   float
   computeSample( T t ) const;

   inline static float
   functionNoise( T t, T freq, T period, T phase = T(0), T pulseWidth = T(0.5) );

   inline static float
   functionSine( T t, T freq, T period, T phase = T(0), T pulseWidth = T(0.5) );

   inline static float
   functionRect( T t, T freq, T period, T phase = T(0), T pulseWidth = T(0.5) );

   inline static float
   functionTriangle( T t, T freq, T period, T phase = T(0), T pulseWidth = T(0.5) );

   inline static float
   functionRamp( T t, T freq, T period, T phase = T(0), T pulseWidth = T(0.5) );

   inline static float
   functionSaw( T t, T freq, T period, T phase = T(0), T pulseWidth = T(0.5) );

   inline static float
   functionOrgan( T t, T freq, T period, T phase = T(0), T pulseWidth = T(0.1) );

   inline static float
   functionPiano( T t, T freq, T period, T phase = T(0), T pulseWidth = T(0.5) );

   inline static float
   functionAnalogRect( T t, T freq, T period, T phase = T(0), T pulseWidth = T(0.5) );

   inline static float
   functionAnalogSaw( T t, T freq, T period, T phase = T(0), T pulseWidth = T(0.5) );
};

} // end namespace audio.
} // end namespace de.

#endif


