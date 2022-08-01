#include <de/audio/Engine.hpp>

namespace de {
namespace audio {

namespace {
   struct RtUtils
   {
      DE_CREATE_LOGGER("de.audio.RtUtils")

      static int
      getOutputDeviceCount( RtAudio* dac )
      {
         if ( !dac ) return 0;
         int n = 0;
         for ( size_t i = 0; i < dac->getDeviceCount(); i++ )
         {
            if ( dac->getDeviceInfo( i ).outputChannels ) { n++; }
         }
         return n;
      }

      static int
      getInputDeviceCount( RtAudio* dac )
      {
         if ( !dac ) return 0;
         int n = 0;
         for ( size_t i = 0; i < dac->getDeviceCount(); i++ )
         {
            if ( dac->getDeviceInfo( i ).inputChannels ) { n++; }
         }
         return n;
      }

      static int
      getDefaultOutputDevice( RtAudio* dac )
      {
         for ( size_t i = 0; i < dac->getDeviceCount(); i++ )
         {
            if ( dac->getDeviceInfo( i ).isDefaultOutput )
            {
               return i;
            }
         }
         return -1;
      }

      static int
      getDefaultInputDevice( RtAudio* dac )
      {
         for ( size_t i = 0; i < dac->getDeviceCount(); i++ )
         {
            if ( dac->getDeviceInfo( i ).isDefaultInput )
            {
               return i;
            }
         }
         return -1;
      }

      static void
      enumerateDevices( RtAudio* dac )
      {
         DE_DEBUG("OutputDeviceCount = ",getOutputDeviceCount(dac))
         DE_DEBUG("InputDeviceCount = ",getInputDeviceCount(dac))
         DE_DEBUG("DefaultOutputDevice = ",getDefaultOutputDevice(dac))
         DE_DEBUG("DefaultInputDevice = ",getDefaultInputDevice(dac))

         for ( size_t i = 0; i < dac->getDeviceCount(); i++ )
         {
            auto d = dac->getDeviceInfo( i );
            std::stringstream s;

            s << "[" << i << "] name(" << d.name << "), "
               "sampleRate(" << d.preferredSampleRate << "), ";

            if ( d.duplexChannels > 0 )
            {
               s << "duplex(" << d.duplexChannels << "), ";
            }
            else if ( d.outputChannels > 0 )
            {
               s << "output(" << d.outputChannels << "), ";
            }
            else if ( d.inputChannels > 0 )
            {
               s << "inputs(" << d.inputChannels << "), ";
            }

            if ( d.isDefaultOutput || d.isDefaultInput )
            {
               s << "isDefault";
            }

            DE_DEBUG(s.str())
         }
      }

   };
} // end namespace audio

Engine::Engine()
   : m_isPlaying( false )
   , m_isPaused( false )
   , m_dac( nullptr )
   , m_dacStreamTime( 0.0 )
   , m_outSampleRate( 48000 )
   , m_outBufferFrames( 1024 ) // 256 = 5.333 ms buffer latency
   , m_outDeviceIndex( 0 )
   , m_outChannels( 2 )
   , m_outFirstChannel( 0 )
   , m_inSampleRate( 48000 )
   , m_inBufferFrames( 1024 ) // = 5.333 ms buffer latency
   , m_inDeviceIndex( 0 )
   , m_inChannels( 2 )
   , m_inFirstChannel( 0 )
   , m_player( nullptr )
   , m_recorder( nullptr )
{
   //ShiftBuffer< float >::test();
}

Engine::~Engine()
{
   stopAudio();
}

void
Engine::stopAudio()
{
   m_isPlaying = false;
   //m_isPaused = false;
   if ( m_dac )
   {
      DE_DEBUG("Stop")
      //m_isPlaying = false;
      m_dac->stopStream();
      m_dac->closeStream();
      delete m_dac;
      m_dac = nullptr;
   }
}

void
Engine::playAudio()
{
   stopAudio();
   m_dac = new RtAudio( RtAudio::WINDOWS_WASAPI );
   RtUtils::enumerateDevices( m_dac );
   // RtAudio::DeviceInfo inDev = m_dac->getDeviceInfo( m_inDeviceIndex );
   // m_inSampleRate = iDev.preferredSampleRate; //m_SampleBuffer.getSampleRate();
   // if ( m_outSampleRate != oDev.preferredSampleRate )
   // {
      // DE_DEBUG("m_outSampleRate(",m_outSampleRate,") != oDev.preferredSampleRate(",oDev.preferredSampleRate,")")
   // }

   //m_outDeviceIndex = RtUtils::getDefaultOutputDevice( m_dac );
   m_inDeviceIndex = RtUtils::getDefaultInputDevice( m_dac );
   m_outDeviceIndex = m_dac->getDefaultOutputDevice();
   RtAudio::DeviceInfo oDev = m_dac->getDeviceInfo( m_outDeviceIndex );
   m_outSampleRate = oDev.preferredSampleRate; //m_SampleBuffer.getSampleRate();
   m_outChannels = std::min( uint32_t(2), uint32_t(oDev.outputChannels) );
   // if ( m_outSampleRate != oDev.preferredSampleRate )
   // {
      // DE_DEBUG("m_outSampleRate(",m_outSampleRate,") != oDev.preferredSampleRate(",oDev.preferredSampleRate,")")
   // }
   try
   {
      RtAudio::StreamParameters o_params;
      o_params.deviceId = m_outDeviceIndex;
      o_params.firstChannel = m_outFirstChannel;
      o_params.nChannels = m_outChannels;
      RtAudio::StreamParameters* in_params = nullptr;

      m_isPlaying = true;

      uint32_t bufferSize = m_outBufferFrames;
      m_dac->openStream( &o_params, in_params, RTAUDIO_FLOAT32, m_outSampleRate, &bufferSize, rt_audio_callback, reinterpret_cast< void* >( this ) );
      if ( m_outBufferFrames != bufferSize )
      {
         DE_WARN("m_outBufferFrames(",m_outBufferFrames,") != bufferSize(",bufferSize,")")
         m_outBufferFrames = bufferSize;
      }

      // uint32_t dacSamples = m_outBufferFrames * m_outChannels;
      // ResizeMemory( m_MixBuffer, dacSamples );
      // ResizeMemory( m_TmpBuffer, dacSamples );

//      if ( m_player )
//      {
//         m_player->onAudioEngineAboutToStart( m_outBufferFrames, m_outChannels, m_outSampleRate );
//      }

      m_dac->startStream();
      m_dacStreamTime = dbSeconds();
   }
   catch ( RtAudioError& e )
   {
      m_isPlaying = false;
      e.printMessage();
   }

   double latency = 1000.0 * double( m_outBufferFrames ) / double( m_outSampleRate ); // in [ms]
   DE_DEBUG("PlayAudio Engine "
               "dacFrames(", m_outBufferFrames,"), "
               "dacSampleRate(",m_outSampleRate,"), "
               "bufferLatency(",latency," ms)")
}

int
Engine::rt_audio_callback(
   void* outputBuffer,
   void* inputBuffer,
   unsigned int frameCount,
   double pts,
   RtAudioStreamStatus status,
   void* userData )
{
   if ( status == RTAUDIO_OUTPUT_UNDERFLOW ) { DE_WARN("Underflow!") return int( status ); }
   else if ( status == RTAUDIO_INPUT_OVERFLOW ) { DE_WARN("Overflow!") return int( status ); }

   auto engine = reinterpret_cast< Engine* >( userData );
   if ( !engine ) { DE_WARN("No engine") return 1; }
   if ( !engine->m_isPlaying ) { DE_WARN("Not playing!") return 1; }

   // Output ( MasterOut )
   uint32_t dstRate = engine->m_outSampleRate;
   uint32_t dstChannels = engine->m_outChannels;
   uint64_t dstSamples = frameCount * dstChannels;
   auto & dstBuffer = engine->m_playerBuffer;
   DSP_RESIZE( dstBuffer, dstSamples );
   DSP_FILLZERO( dstBuffer );
   if ( engine->m_player )
   {
      engine->m_player->readSamples( pts, dstBuffer.data(), frameCount, dstChannels, dstRate );
   }

   // Output ( MonitorOut )

   // Input ( MicrophoneIn )
   uint32_t phoneChannels = engine->m_inChannels;
   uint64_t phoneSamples = frameCount * phoneChannels;
   auto & phoneBuffer = engine->m_recordBuffer;
   DSP_RESIZE( phoneBuffer, phoneSamples );
   DSP_FILLZERO( phoneBuffer );

   // PLAY DEBUG NOIZE
//   float* play = reinterpret_cast< float* >( dstBuffer.data() );
//   for ( size_t i = 0; i < playSamples; ++i )
//   {
//      *play += 0.14f*(-1.0f + float(rand()%65536)/32768.0f);
//      play++;
//   }

   float* dst = reinterpret_cast< float* >( outputBuffer );
   DSP_COPY_SCALED_LIMITED( dstBuffer.data(), dst, dstSamples, 0.1f );


   return 0; // OK
}

} // end namespace audio.
} // end namespace de.
