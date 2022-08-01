#ifndef G_COMPRESSOR_HPP
#define G_COMPRESSOR_HPP

#include <QWidget>
#include <de/audio/dsp/IDspChainElement.hpp>
#include "GSliderV.hpp"
#include "GGroupV.hpp"

namespace de {
namespace audio {

struct randja_compressor : public IDspChainElement
{
   DE_CREATE_LOGGER("randja_compressor")
   bool m_isBypassed;
   uint32_t m_sampleRate;
   float m_threshold;
   float m_attack, m_release, m_envelope_decay;
   float m_output;
   float m_transfer_A, m_transfer_B;
   float m_env, m_gain;
   IDspChainElement* m_inputSignal;

   randja_compressor()
   {
      m_isBypassed = false;
      m_sampleRate = 48000;
      m_threshold = 1.f;
      m_attack = m_release = m_envelope_decay = 0.f;
      m_output = 1.f;
      m_transfer_A = 0.f;
      m_transfer_B = 1.f;
      m_env = 0.f;
      m_gain = 1.f;
      m_inputSignal = nullptr;
      setThreshold( 0.9f );
      setRatio( 2.0f ); // 2:1
      setAttackTime( 5 );  // 5 ms
      setReleaseTime( 5 ); // 5 ms
      setOutputGain( 1.0f ); // 0dB gain
   }
   void reset()
   {
      m_env = 0.f; m_gain = 1.f;
   }
   void setSampleRate(uint32_t value)
   {
      m_sampleRate = value;
   }
   void setThreshold(float value)
   {
      m_threshold = std::clamp( value, 0.01f, 1.0f );
      m_transfer_B = m_output *pow(m_threshold,-m_transfer_A);
   }

   // Must be in range [0.0; 1.0] with 0.0
   // representing a oo:1 ratio, 0.5 a 2:1 ratio; 1.0 a 1:1 ratio and so on
   void setRatio(float ratio)
   {
      if ( ratio < 1.0f ) ratio = 1.0f;
      m_transfer_A = std::clamp( (1.0f / ratio)-1.f, 0.01f, 1.0f );
      m_transfer_B = m_output * pow(m_threshold,-m_transfer_A);
   }
   void setAttackTime(float t_in_ms)
   {
      float frameCount = 0.001f * t_in_ms * float(m_sampleRate);
      m_attack = exp( -1.f / frameCount );
   }
   void setReleaseTime(float t_in_ms)
   {
      float frameCount = 0.001f * t_in_ms * float(m_sampleRate);
      m_release = exp( -1.f / frameCount );
      m_envelope_decay = exp( -4.f / frameCount );
   }
   void setOutputGain(float value)
   {
      m_output = value;
      m_transfer_B = m_output * pow( m_threshold, -m_transfer_A );
   }

   bool isBypassed() const { return m_isBypassed; }
   void setBypassed( bool enabled ) { m_isBypassed = enabled; }
   void clearInputSignals() { m_inputSignal = nullptr; }
   void setInputSignal( int i, de::audio::IDspChainElement* input ) { m_inputSignal = input; }

   uint64_t
   readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
   {
      auto dstSamples = dstFrames * dstChannels;

      if ( m_inputSignal )
      {
         m_inputSignal->readSamples( pts, dst, dstFrames, dstChannels, dstRate );
      }
      else
      {
         DSP_FILLZERO( dst, dstSamples );
      }

      if ( m_isBypassed )
      {
         return dstSamples;
      }

//      if ( m_sampleRate != dstRate )
//      {
//         m_sampleRate = dstRate;
//         float frameCount = 0.001f * t_in_ms * float(m_sampleRate);
//         m_attack = exp( -1.f / frameCount );
//      }

      if ( dstChannels == 2 )
      {
         auto pSrc = dst;
         auto pDst = dst;
         for ( size_t i = 0; i < dstFrames; i++ )
         {
            float L = *pSrc++;
            float R = *pSrc++;
            float det = std::max( fabs( L ), fabs( R ) );
            det += 10e-30f; // add tiny DC offset (-600dB) to prevent denormals
            m_env = det >= m_env ? det : det+m_envelope_decay * (m_env-det);
            float transfer_gain = m_env > m_threshold ? pow(m_env,m_transfer_A) * m_transfer_B : m_output;
            m_gain = transfer_gain < m_gain
                     ? transfer_gain + m_attack * (m_gain-transfer_gain)
                     : transfer_gain + m_release * (m_gain-transfer_gain);
            *pDst++ = L * m_gain;
            *pDst++ = R * m_gain;
         }
      }
      else
      {
         DE_ERROR("Not implemented for channels != 2")
      }
      return dstSamples;
   }



   // __forceinline void process(
   // float* input_left, float* input_right,
   // float* output_left, float* output_right, int frames )
   //   {
   //      float det, transfer_gain;
   //      for ( int i = 0; i < frames; i++)
   //      {
   //         det = max(fabs(input_left[i]),fabs(input_right[i]));
   //         det += 10e-30f; // add tiny DC offset (-600dB) to prevent denormals
   //         m_env = det >= m_env ? det : det+m_envelope_decay * (m_env-det);
   //         transfer_gain = m_env > m_threshold ? pow(m_env,m_transfer_A) * m_transfer_B : m_output;
   //         m_gain = transfer_gain < m_gain
   //                  ? transfer_gain + m_attack * (m_gain-transfer_gain)
   //                  : transfer_gain + m_release * (m_gain-transfer_gain);
   //         output_left[i] = input_left[i] * m_gain;
   //         output_right[i] = input_right[i] * m_gain;
   //      }
   //   }
};

} // end namespace audio
} // end namespace de

// Placed in the DSP chain.
// Controls volume of one incoming (stereo) audio signal only.
// ============================================================================
class GCompressor : public QWidget, public de::audio::IDspChainElement
// ============================================================================
{
   Q_OBJECT
public:
   GCompressor( QWidget* parent = 0 );
   ~GCompressor();

   uint64_t
   readSamples( double pts, float* dst, uint32_t dstFrames,
                uint32_t dstChannels, uint32_t dstRate ) override;
signals:
//   void newSamples( float* samples, uint32_t frameCount, uint32_t channels );

public slots:
   void clearInputSignals();
   void setInputSignal( int i, de::audio::IDspChainElement* input );
   void setBypassed( bool bypassed );
private slots:
   void on_menuButton_bypass(bool);
   void on_menuButton_more(bool);
   void on_menuButton_hideAll(bool);
   void on_thresholdKnob( int v );
   void on_ratioKnob( int v );
   void on_attackKnob( int v );
   void on_releaseKnob( int v );
   void on_gainKnob( int v );

protected:
   //void timerEvent( QTimerEvent* event) override;
   //void paintEvent( QPaintEvent* event ) override;

private:
   DE_CREATE_LOGGER("GCompressor")
   de::audio::randja_compressor m_compressor;

   GGroupV* m_menu;
   GSliderV* m_threshKnob;
   GSliderV* m_ratioKnob;
   GSliderV* m_attackKnob;
   GSliderV* m_releaseKnob;
   GSliderV* m_gainKnob;


   //GSliderV* m_trebleKnob;
   //GSliderV* m_midKnob;
   //GSliderV* m_bassKnob;
   //GSliderV* m_reverbKnob;



};

#endif // G_OSZILLOSKOP_H
