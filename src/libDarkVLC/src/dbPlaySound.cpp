#include <dbPlaySound.hpp>
#include <de/audio/RtUtils.hpp>
//#include <de/audio/Sound.hpp>
//#include <de/audio/Mixer.hpp>
#include <de/audio/SampleBuffer.hpp>
#include <de/audio/SampleBufferIO.hpp>
#include <de/audio/SampleBufferDraw.hpp>
#include <de/audio/ResamplerST.hpp>
#include <de/audio/ResamplerFF.hpp>

#include <thread>
#include <condition_variable>


namespace de {
namespace audio {

// =========================================================
struct Play_v3_Sound
// =========================================================
{
   DE_CREATE_LOGGER("de.audio.Play_v3_Sound")
   SampleBuffer m_SampleBuffer;
   uint64_t m_FrameIndex;
   std::string m_Uri;
   int m_Id;
   int m_Volume;   // between 0,1
   int m_StereoMode; // 0 - Stereo, 1 - Mono
   bool m_IsLoaded;
   bool m_IsPlaying;
   bool m_IsPaused;
   bool m_IsLooping;

   Play_v3_Sound()
      : m_FrameIndex( 0 )
      , m_Id( 0 )
      , m_Volume( 50 )
      , m_StereoMode( 0 )
      , m_IsLoaded( false )
      , m_IsPlaying( false )
      , m_IsPaused( false )
      , m_IsLooping( false )
   {

   }

   ~Play_v3_Sound()
   {

   }

   bool
   is_loaded() const { return m_IsLoaded; }

   bool
   is_playing() const { return m_IsPlaying; }

   bool
   is_paused() const { return m_IsPaused; }


   void
   play()
   {
      if ( !m_IsLoaded ) { DE_ERROR("Not loaded") return; }
      if ( !m_IsPaused ) { m_FrameIndex = 0; }
      else { m_IsPaused = false; }
      m_IsPlaying = true;
   }

   void
   stop()
   {
      if ( !m_IsLoaded ) { DE_ERROR("Not loaded") return; }
      m_IsPlaying = false;
      m_IsPaused = false;
   }

   void
   pause()
   {
      if ( !m_IsLoaded ) { DE_ERROR("Not loaded") return; }
      m_IsPaused = true;
   }

   int
   getVolume() const { return m_Volume; }

   void
   setVolume( int vol_in_percent )
   {
      vol_in_percent = Math::clampi( vol_in_percent, 0, 200 );
      m_Volume = vol_in_percent;
   }

   uint64_t
   readSamples( SampleBuffer & dst, double pts )
   {
      if ( !m_IsLoaded ) { DE_ERROR("Not loaded") return 0; }
      if ( m_IsPaused ) { DE_ERROR("Paused soundId") return 0; }

      uint64_t n = dst.getFrameCount();
      uint64_t m = m_SampleBuffer.getFrameCount();
      if ( m_FrameIndex + n >= m )
      {
         DE_DEBUG( "m_FrameIndex(",m_FrameIndex,") + n(",n,") >= m(",m,")" )
         m_FrameIndex = 0; // rewind dirty hack
         m_IsPlaying = false;
         m_IsPaused = false;
         //return 0;
      }
      uint64_t k = m_SampleBuffer.read( m_FrameIndex,
                                   dst.getFrameCount(),
                                   dst.getChannelCount(),
                                   dst.data(),
                                   dst.getSampleType() );

      //DE_DEBUG( "k = ",k,", dst.getChannelCount() = ",dst.getChannelCount() )
      m_FrameIndex += k;
      return k * dst.getChannelCount();
   }
};

// =========================================================
struct Play_v3
// =========================================================
{
   DE_CREATE_LOGGER("de.audio.Play_v3")

   std::thread* m_ThreadPtr;
   std::atomic< bool > m_IsPlaying;
   std::atomic< bool > m_IsPaused;
   int m_Volume;
   //mutable std::condition_variable m_BlockSignalReady;

   std::vector< Play_v3_Sound > m_Sounds;
   std::mutex m_SoundsMutex;

   RtAudio* m_dac;
   int32_t m_dacSampleRate;
   int32_t m_dacFrames;
   int32_t m_outDevIndex;
   int32_t m_outDevChannels;
   int32_t m_outDevFirstChannel;
//   uint64_t m_FrameIndex;
//   uint64_t m_FrameCount;
   SampleBuffer m_MixBuffer; // float32 accumulation ( sum of all sources )
   SampleBuffer m_SrcBuffer; // float32 buffer for reading single source ( before mix )

   static Play_v3 &
   getInstance()
   {
      static Play_v3 s_Instance;
      return s_Instance;
   }

   Play_v3()
      : m_ThreadPtr( nullptr )
      , m_IsPlaying( false )
      , m_IsPaused( false )
      , m_Volume( 50 )
      , m_dac( nullptr )
      , m_dacSampleRate( 44000 )
      , m_dacFrames( 256 )
      , m_outDevIndex( 0 )
      , m_outDevChannels( 2 )
      , m_outDevFirstChannel( 0 )
//      , m_FrameIndex( 0 )
//      , m_FrameCount( 0 )
   {
      //m_Uri = uri;
      m_ThreadPtr = new std::thread( &Play_v3::run, this );
      //blockUntilReady();
      // std::mutex m;
      // std::unique_lock< std::mutex > guard( m );
      // m_BlockSignalReady.wait( guard, [ this ] () { return m_IsReady; } );
   }

   ~Play_v3()
   {
      // std::mutex m;
      // std::unique_lock< std::mutex > guard( m );
      // m_BlockSignalReady.wait( guard, [ this ] () { return !m_IsReady; } );

      m_IsPlaying = false;
      m_IsPaused = false;


      if ( m_dac )
      {
         DE_DEBUG("Stop")
         //m_IsPlaying = false;
         m_dac->stopStream();
         m_dac->closeStream();
         delete m_dac;
         m_dac = nullptr;
      }

      if ( m_ThreadPtr )
      {
         m_ThreadPtr->join();
         delete m_ThreadPtr;
         m_ThreadPtr = nullptr;
      }

      clearSounds();
   }

   void clearSounds()
   {
      std::unique_lock< std::mutex > lg( m_SoundsMutex );
      m_Sounds.clear();
   }

//   int addInput( IAudioSource* source )
//   {
//      if ( !source ) return 0;

//      std::unique_lock< std::mutex > lg( m_SoundsMutex );
//      m_Sounds.emplace_back( source );
//      return int( m_Sounds.size() );
//   }

   int loadSound( std::string uri )
   {
      auto it = std::find_if( m_Sounds.begin(), m_Sounds.end(),
         [&] ( Play_v3_Sound const & cached ) { return cached.m_Uri == uri; } );

      if ( it != m_Sounds.end() )
      {
         return it->m_Id;
      }

      // int id = 1;

//      for ( auto & sound : m_Sounds )
//      {
//         if ( sound.getI)
//      }

      static int createCounterId = 1;
      int soundId = createCounterId++;

      m_Sounds.emplace_back();
      Play_v3_Sound & sound = m_Sounds.back();
      sound.m_Uri = uri;
      sound.m_Id = soundId;
      sound.m_Volume = 100;
      sound.m_IsLoaded = SampleBufferIO::load( sound.m_SampleBuffer, uri );
      sound.m_IsPlaying = false;
      sound.m_IsPaused = false;
      sound.m_IsLooping = false;

      if ( !sound.is_loaded() )
      {
         DE_ERROR("Cant load ",uri)
         return 0;
      }

      ESampleType srcType = sound.m_SampleBuffer.getSampleType();
      ESampleType dstType = ST_F32I;
      if ( srcType != dstType )
      {
         DE_ERROR("Convert srcType(",ST_toString(srcType),") to dstType(",ST_toString(dstType),")" )
         auto tmp = sound.m_SampleBuffer; // input buffer with input sample rate
         tmp.convert( sound.m_SampleBuffer, ST_F32I );
      }

      int srcRate = sound.m_SampleBuffer.getSampleRate();
      int dstRate = m_dacSampleRate;
      if ( srcRate != dstRate )
      {
         DE_ERROR("Resample srcRate(",srcRate,"Hz) to dstRate(",dstRate,"Hz)" )
         DE_FLUSH
         #if 1
         auto tmp = sound.m_SampleBuffer; // input buffer with input sample rate
         //auto tmpResample = sound.m_SampleBuffer; // input buffer with input sample rate
         sound.m_SampleBuffer.setSampleRate( m_dacSampleRate ); // output sample rate
         //ResamplerST::resample( tmp, sound.m_SampleBuffer );
         ResamplerFF::resample( tmp, sound.m_SampleBuffer );

         auto saveUri = dbGetFileName( uri ) + ".resample";
         saveUri += std::to_string( srcRate );
         saveUri += "Hz_to_";
         saveUri += std::to_string( dstRate );
         saveUri += "Hz.wav";
         SampleBufferIO::save( sound.m_SampleBuffer, saveUri );
         #endif
      }
      return soundId;
   }

   void deleteSound( int soundId )
   {
      if ( !soundId ) return;
      auto it = std::find_if( m_Sounds.begin(), m_Sounds.end(),
         [&] ( Play_v3_Sound const & cached ) { return cached.m_Id == soundId; } );

      if ( it != m_Sounds.end() )
      {
         m_Sounds.erase( it );
         DE_DEBUG("Deleted soundId ",soundId)
      }
   }

   void playSound( int soundId, int loops = 0 )
   {
      if ( !soundId ) return;
      auto it = std::find_if( m_Sounds.begin(), m_Sounds.end(),
         [&] ( Play_v3_Sound const & cached ) { return cached.m_Id == soundId; } );

      if ( it == m_Sounds.end() )
      {
         DE_ERROR("No soundId ",soundId)
         return;
      }

      DE_DEBUG("Play soundId ",soundId, " Hz(", it->m_SampleBuffer.getSampleRate(),"), dacHz(",m_dacSampleRate,")")
      it->play();
   }

   void pauseSound( int soundId )
   {
      if ( !soundId ) return;
      auto it = std::find_if( m_Sounds.begin(), m_Sounds.end(),
         [&] ( Play_v3_Sound const & cached ) { return cached.m_Id == soundId; } );

      if ( it == m_Sounds.end() )
      {
         DE_ERROR("No soundId ",soundId)
         return;
      }

      DE_DEBUG("Pause soundId ",soundId)
      it->pause();
   }

   void stopSound( int soundId )
   {
      if ( !soundId ) return;
      auto it = std::find_if( m_Sounds.begin(), m_Sounds.end(),
         [&] ( Play_v3_Sound const & cached ) { return cached.m_Id == soundId; } );

      if ( it == m_Sounds.end() )
      {
         DE_ERROR("No soundId ",soundId)
         return;
      }

      DE_DEBUG("Stop soundId ",soundId)
      it->stop();
   }

   int
   getSoundVolume( int soundId ) const
   {
      if ( !soundId ) return 0;
      auto it = std::find_if( m_Sounds.begin(), m_Sounds.end(),
         [&] ( Play_v3_Sound const & cached ) { return cached.m_Id == soundId; } );

      if ( it == m_Sounds.end() )
      {
         DE_ERROR("No soundId ",soundId)
         return 0;
      }

      return it->getVolume();
   }

   void
   setSoundVolume( int soundId, int vol_in_percent )
   {
      if ( !soundId ) return;
      auto it = std::find_if( m_Sounds.begin(), m_Sounds.end(),
         [&] ( Play_v3_Sound const & cached ) { return cached.m_Id == soundId; } );

      if ( it == m_Sounds.end() )
      {
         DE_ERROR("No soundId ",soundId)
         return;
      }

      it->setVolume( vol_in_percent );
      DE_DEBUG("setVolume(",vol_in_percent,"%)")
   }


   void playAudio()
   {
      m_IsPaused = false;
   }

   void pauseAudio()
   {
      m_IsPaused = true;
   }

   void stopAudio()
   {
      //m_IsPlaying = false;
      m_IsPaused = false;

      for ( Play_v3_Sound & sound : m_Sounds )
      {
         sound.stop();
      }
   }

   void run()
   {
      m_IsPlaying = false;

      DE_DEBUG("New thread to play Audio with a fresh WASAPI stream m_dacSampleRate(",m_dacSampleRate,")")

      RtAudio* m_dac = new RtAudio( RtAudio::WINDOWS_WASAPI );
      RtAudio::DeviceInfo oDev = m_dac->getDeviceInfo(m_outDevIndex);

      m_dacSampleRate = oDev.preferredSampleRate; //m_SampleBuffer.getSampleRate();
      if ( m_dacSampleRate != oDev.preferredSampleRate )
      {
         DE_DEBUG("m_dacSampleRate(",m_dacSampleRate,") != oDev.preferredSampleRate(",oDev.preferredSampleRate,")")
      }
      m_outDevIndex = m_dac->getDefaultOutputDevice();
      m_outDevChannels = std::min( int32_t(2), int32_t(oDev.outputChannels) );
      m_MixBuffer.setFormat( ST_F32I, m_outDevChannels, float( m_dacSampleRate ) );
      m_MixBuffer.resize( m_dacFrames );
      m_SrcBuffer.setFormat( ST_F32I, m_outDevChannels, float( m_dacSampleRate ) );
      m_SrcBuffer.resize( m_dacFrames );
      try
      {
         RtAudio::StreamParameters o_params;
         o_params.deviceId = m_outDevIndex;
         o_params.firstChannel = m_outDevFirstChannel;
         o_params.nChannels = m_outDevChannels;
         RtAudio::StreamParameters* in_params = nullptr;
         m_IsPlaying = true;
         uint32_t bufferSize = m_dacFrames;
         m_dac->openStream( &o_params, in_params, RTAUDIO_FLOAT32, m_dacSampleRate,
                   &bufferSize, rt_audio_callback, reinterpret_cast< void* >( this ) );
         if ( m_dacFrames != bufferSize )
         {
            DE_WARN("m_dacFrames(",bufferSize,") != bufferSize(",bufferSize,")")
         }

         m_dac->startStream();
      }
      catch ( RtAudioError& e )
      {
         e.printMessage();
         m_IsPlaying = false;
      }

      while ( m_IsPlaying )
      {
         std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
      }

      DE_DEBUG("Exit play Audio with a fresh WASAPI stream m_dacSampleRate(",m_dacSampleRate,")")
      DE_FLUSH
   }

   static int
   rt_audio_callback(
      void* outputBuffer,
      void* inputBuffer,
      unsigned int frameCount,
      double pts,
      RtAudioStreamStatus status,
      void* userData )
   {
      if ( status == RTAUDIO_OUTPUT_UNDERFLOW ) { DE_WARN("Underflow!") return int( status ); }
      else if ( status == RTAUDIO_INPUT_OVERFLOW ) { DE_WARN("Overflow!") return int( status ); }

      auto play = reinterpret_cast< Play_v3* >( userData );
      if ( !play ) { DE_WARN("No audio!") return 1; }
      if ( !play->m_IsPlaying ) { DE_WARN("No audio playing!") return 1; } // not enough data, eof.

      auto & mixBuffer = play->m_MixBuffer;
      if ( mixBuffer.getFrameCount() < frameCount )
      {
         DE_WARN("Resize mixBuffer")
         mixBuffer.resize( frameCount );
      }

      auto & srcBuffer = play->m_SrcBuffer;
      if ( srcBuffer.getFrameCount() < frameCount )
      {
         DE_WARN("Resize srcBuffer(",srcBuffer.getFrameCount(),") < frameCount(",frameCount,")")
         srcBuffer.resize( frameCount );
      }

      // ZeroMemory
      memset( mixBuffer.data(), 0, mixBuffer.size() );
      memset( srcBuffer.data(), 0, srcBuffer.size() );

      float* mixStart = mixBuffer.writePtr< float >();
      float* srcStart = srcBuffer.writePtr< float >();

   #if 1
      // Accumulate/Add Sounds into MixBuffer ...
      if ( !play->m_IsPaused )
      {
         std::unique_lock< std::mutex > lg( play->m_SoundsMutex );

         // Accumulate/Add Sounds into MixBuffer ...

         for ( Play_v3_Sound & sound : play->m_Sounds )
         {
            if ( !sound.is_playing() )
            {
               continue;
            }

            if ( sound.is_paused() )
            {
               continue;
            }

            // By calling readSamples the source's position will be updated automatically.
            float sourceVolume = 0.01f * sound.getVolume();

            auto nSamples = sound.readSamples( srcBuffer, pts );
            if ( nSamples != srcBuffer.getSampleCount() )
            {
               DE_ERROR("nSamples(",nSamples,") != srcSamples(",srcBuffer.getSampleCount(),")");
               continue; // If source is incomplete, add nothing, goto next source.
            }

            // Loop all samples, works for both ( interleaved + planar ) memory layouts.
            float* pSrc = srcStart;
            float* pMix = mixStart;
            for ( size_t frame = 0; frame < mixBuffer.getFrameCount(); ++frame )
            {
               for ( size_t channel = 0; channel < mixBuffer.getChannelCount(); ++channel )
               {
                  *pMix += (*pSrc) * sourceVolume; // Accumulate +=
                  pMix++;
                  pSrc++;
               }
            }
         }

         lg.unlock();
      }
   #endif

      // Write to interleaved audio stream RTAUDIO_FLOAT32
      float* dst = reinterpret_cast< float* >( outputBuffer );
      float mixerVolume = 0.01f * play->m_Volume;

//      // eof
//      int64_t availFrames = play->m_SampleBuffer.getFrameCount() - play->m_FrameIndex;
//      if ( frameCount >= availFrames )
//      {
//         DE_DEBUG( "m_FrameIndex(",play->m_FrameIndex,") + frameCount(",frameCount,")"
//                   " >= m_SampleBuffer.getFrameCount(", play->m_SampleBuffer.getFrameCount(),")" )

         //play->m_FrameIndex = 0; // rewind dirty hack

      for ( size_t i = 0; i < mixBuffer.getFrameCount(); ++i )
      {
         for ( size_t ch = 0; ch < mixBuffer.getChannelCount(); ++ch )
         {
            float sample = 0.0f;
            if ( mixBuffer.getSample( i, ch, &sample, ST_F32I ) )
            {
               *dst = std::clamp( mixerVolume * sample, -1.0f, 1.0f ); // With Benni Limiter.
            }
            else
            {
               DE_WARN("i(",i,"), ch(",ch,") no sample")
               *dst = 0.f;
            }

            dst++;
         }
      }
   }

   int
   getVolume() const { return m_Volume; }

   void
   setVolume( int vol_in_percent )
   {
      vol_in_percent = Math::clampi( vol_in_percent, 0, 200 );
      m_Volume = vol_in_percent;
      DE_DEBUG("setVolume(",vol_in_percent,"%)")
   }

};

} // end namespace audio
} // end namespace de

int
dbLoadSound( std::string uri )
{
   return de::audio::Play_v3::getInstance().loadSound( uri );
}

void
dbDeleteSound( int soundId )
{
   de::audio::Play_v3::getInstance().deleteSound( soundId );
}

void
dbPlaySound( int soundId, int loops )
{
   de::audio::Play_v3::getInstance().playSound( soundId, loops );
}

void
dbStopSound( int soundId )
{
   de::audio::Play_v3::getInstance().stopSound( soundId );
}

void
dbPauseSound( int soundId )
{
   de::audio::Play_v3::getInstance().pauseSound( soundId );
}


void
dbSetSoundVolume( int soundId, int percent0_400 )
{
   de::audio::Play_v3::getInstance().setSoundVolume( soundId, percent0_400 );
}

int
dbGetSoundVolume( int soundId )
{
   return de::audio::Play_v3::getInstance().getSoundVolume( soundId );
}


/*


// [Sound]
struct Play_v3_Sound : public IAudioSource
{
   DE_CREATE_LOGGER("de.audio.Play_v3_Sound")
   SampleBuffer m_SampleBuffer;
   uint64_t m_FrameIndex;
   int m_Id;
   int m_Volume;   // between 0,1
   int m_StereoMode; // 0 - Stereo, 1 - Mono
   bool m_IsPlaying;
   bool m_IsPaused;
   bool m_IsLooping;

   Play_v3_Sound()
      : m_FrameIndex( 0 )
      , m_Id( 0 )
      , m_Volume( 100 )
      , m_StereoMode( 0 )
      , m_IsPlaying( false )
      , m_IsPaused( false )
      , m_IsLooping( false )
   {

   }

   ~Play_v3_Sound()
   {

   }

   bool
   is_playing() const override { return m_IsPlaying; }

   bool
   is_paused() const override { return m_IsPaused; }

   void
   play()
   {
      if ( !m_IsPaused ) { m_FrameIndex = 0; }
      else { m_IsPaused = false; }
      m_IsPlaying = true;
   }

   void
   stop()
   {
      m_IsPlaying = false;
      m_IsPaused = false;
   }

   void
   pause()
   {
      m_IsPaused = true;
   }

   int
   getVolume() const override { return m_Volume; }

   void
   setVolume( int vol_in_percent ) override
   {
      vol_in_percent = Math::clampi( vol_in_percent, 0, 200 );
      m_Volume = vol_in_percent;
   }

   uint64_t
   readSamples( SampleBuffer & dst, double pts ) override
   {
      //      if ( m_SampleBuffer.empty() )
      //      {
      //         DE_DEBUG( "No sample buffer" )
      //         return 0;
      //      }
      uint64_t n = dst.getFrameCount();
      uint64_t m = m_SampleBuffer.getFrameCount();
      if ( m_FrameIndex + n >= m )
      {
         DE_DEBUG( "m_FrameIndex(",m_FrameIndex,") + n(",n,") >= m(",m,")" )
         m_FrameIndex = 0; // rewind dirty hack

         //return 0;
      }
      uint64_t k = m_SampleBuffer.read( m_FrameIndex,
                                   dst.getFrameCount(),
                                   dst.getChannelCount(),
                                   dst.data(),
                                   dst.getSampleType() );

      DE_DEBUG( "k = ",k,", dst.getChannelCount() = ",dst.getChannelCount() )
      m_FrameIndex += k;
      return k * dst.getChannelCount();
   }
};

struct Play_v3
{
   DE_CREATE_LOGGER("de.audio.Play_v3")

   std::thread* m_ThreadPtr;
   std::atomic< bool > m_IsPlaying;
   std::atomic< bool > m_IsPaused;
   int m_Volume;
   //mutable std::condition_variable m_BlockSignalReady;

   std::vector< Play_v3_Sound > m_Sounds;
   std::mutex m_SoundsMutex;

   RtAudio* m_dac;
   int32_t m_dacSampleRate;
   int32_t m_dacFrames;
   int32_t m_outDevIndex;
   int32_t m_outDevChannels;
   int32_t m_outDevFirstChannel;
//   uint64_t m_FrameIndex;
//   uint64_t m_FrameCount;
   SampleBuffer m_MixBuffer; // float32 accumulation ( sum of all sources )
   SampleBuffer m_SrcBuffer; // float32 buffer for reading single source ( before mix )

   static Play_v3 &
   getInstance()
   {
      static Play_v3 s_Instance;
      return s_Instance;
   }

   Play_v3()
      : m_ThreadPtr( nullptr )
      , m_IsPlaying( false )
      , m_IsPaused( false )
      , m_Volume( 100 )
      , m_dac( nullptr )
      , m_dacSampleRate( 44000 )
      , m_dacFrames( 256 )
      , m_outDevIndex( 0 )
      , m_outDevChannels( 2 )
      , m_outDevFirstChannel( 0 )
//      , m_FrameIndex( 0 )
//      , m_FrameCount( 0 )
   {
      //m_Uri = uri;
      m_ThreadPtr = new std::thread( &Play_v3::run, this );
      //blockUntilReady();
      // std::mutex m;
      // std::unique_lock< std::mutex > guard( m );
      // m_BlockSignalReady.wait( guard, [ this ] () { return m_IsReady; } );
   }

   ~Play_v3()
   {
      // std::mutex m;
      // std::unique_lock< std::mutex > guard( m );
      // m_BlockSignalReady.wait( guard, [ this ] () { return !m_IsReady; } );

      m_IsPlaying = false;
      m_IsPaused = false;


      if ( m_dac )
      {
         DE_DEBUG("Stop")
         //m_IsPlaying = false;
         m_dac->stopStream();
         m_dac->closeStream();
         delete m_dac;
         m_dac = nullptr;
      }

      if ( m_ThreadPtr )
      {
         m_ThreadPtr->join();
         delete m_ThreadPtr;
         m_ThreadPtr = nullptr;
      }

      clearSounds();
   }

   void clearSounds()
   {
      std::unique_lock< std::mutex > lg( m_SoundsMutex );

      for ( size_t i = 0; i < m_Sounds.size(); ++i )
      {
         auto p = m_Sounds[ i ];
         if ( p ) delete p;
      }
      m_Sounds.clear();
   }

//   int addInput( IAudioSource* source )
//   {
//      if ( !source ) return 0;

//      std::unique_lock< std::mutex > lg( m_SoundsMutex );
//      m_Sounds.emplace_back( source );
//      return int( m_Sounds.size() );
//   }

   int loadSound( std::string uri )
   {
      int soundId = 0;

      auto it = std::find_if( m_Sounds.begin(), m_Sounds.end(),
         [&] ( Play_v3_Sound const & cached ) { return cached.getUri() == uri; } );

      if ( it != m_Sounds.end() )
      {
         return it->getId();
      }

      // int id = 1;

//      for ( auto & sound : m_Sounds )
//      {
//         if ( sound.getI)
//      }

      static int createCounterId = 0;
      createCounterId++;
//
      m_Sounds.emplace_back();
      Play_v3_Sound & sound = m_Sounds.back();
      sound.m_Id = createCounterId;
      sound.m_Volume = 100;
      sound.m_IsPlaying = false;
      sound.m_IsPaused = false;
      sound.m_IsLooping = false;
      if ( !SampleBufferIO::load( sound.m_SampleBuffer, uri ) )
      {
         DE_ERROR("Cant open ",uri)
         return;
      }
      return soundId;
   }

   void deleteSound( int soundId )
   {
      if ( !soundId ) return;
      auto it = std::find_if( m_Sounds.begin(), m_Sounds.end(),
         [&] ( Play_v3_Sound const & cached ) { return cached.m_Id == soundId; } );

      if ( it != m_Sounds.end() )
      {
         m_Sounds.erase( it );
         DE_DEBUG("Deleted soundId ",soundId)
      }
   }

   void playSound( int soundId, int loops = 0 )
   {
      if ( !soundId ) return;
      auto it = std::find_if( m_Sounds.begin(), m_Sounds.end(),
         [&] ( Play_v3_Sound const & cached ) { return cached.m_Id == soundId; } );

      if ( it == m_Sounds.end() )
      {
         DE_ERROR("No soundId ",soundId)
         return;
      }

      DE_DEBUG("Play soundId ",soundId)
      it->play();
   }

   void pauseSound( int soundId )
   {
      if ( !soundId ) return;
      auto it = std::find_if( m_Sounds.begin(), m_Sounds.end(),
         [&] ( Play_v3_Sound const & cached ) { return cached.m_Id == soundId; } );

      if ( it == m_Sounds.end() )
      {
         DE_ERROR("No soundId ",soundId)
         return;
      }

      DE_DEBUG("Pause soundId ",soundId)
      it->pause();
   }

   void stopSound( int soundId )
   {
      if ( !soundId ) return;
      auto it = std::find_if( m_Sounds.begin(), m_Sounds.end(),
         [&] ( Play_v3_Sound const & cached ) { return cached.m_Id == soundId; } );

      if ( it == m_Sounds.end() )
      {
         DE_ERROR("No soundId ",soundId)
         return;
      }

      DE_DEBUG("Stop soundId ",soundId)
      it->stop();
   }

   int
   getSoundVolume( int soundId ) const
   {
      if ( !soundId ) return 0;
      auto it = std::find_if( m_Sounds.begin(), m_Sounds.end(),
         [&] ( Play_v3_Sound const & cached ) { return cached.m_Id == soundId; } );

      if ( it == m_Sounds.end() )
      {
         DE_ERROR("No soundId ",soundId)
         return 0;
      }

      return it->getVolume();
   }

   void
   setSoundVolume( int soundId, int vol_in_percent )
   {
      if ( !soundId ) return 0;
      auto it = std::find_if( m_Sounds.begin(), m_Sounds.end(),
         [&] ( Play_v3_Sound const & cached ) { return cached.m_Id == soundId; } );

      if ( it == m_Sounds.end() )
      {
         DE_ERROR("No soundId ",soundId)
         return;
      }

      it->setVolume( vol_in_percent );
      DE_DEBUG("setVolume(",vol_in_percent,"%)")
   }


   void playAudio()
   {
      m_IsPaused = false;
   }

   void pauseAudio()
   {
      m_IsPaused = true;
   }

   void stopAudio()
   {
      //m_IsPlaying = false;
      m_IsPaused = false;

      for ( Play_v3_Sound & sound : m_Sounds )
      {
         sound.stop();
      }
   }

   void run()
   {
      m_IsPlaying = false;

      DE_DEBUG("New thread to play Audio with a fresh WASAPI stream with frameCount(",m_FrameCount,")")

      RtAudio* m_dac = new RtAudio( RtAudio::WINDOWS_WASAPI );
      RtAudio::DeviceInfo oDev = m_dac->getDeviceInfo(m_outDevIndex);

      m_dacSampleRate = oDev.preferredSampleRate; //m_SampleBuffer.getSampleRate();
      if ( m_dacSampleRate != oDev.preferredSampleRate )
      {
         DE_DEBUG("m_dacSampleRate(",m_dacSampleRate,") != oDev.preferredSampleRate(",oDev.preferredSampleRate,")")
      }
      m_outDevIndex = m_dac->getDefaultOutputDevice();
      m_outDevChannels = std::min( int32_t(2), int32_t(oDev.outputChannels) );
      m_MixBuffer.setFormat( ST_F32I, m_outDevChannels, float( m_dacSampleRate ) );
      m_MixBuffer.resize( m_dacFrames );
      try
      {
         RtAudio::StreamParameters o_params;
         o_params.deviceId = m_outDevIndex;
         o_params.firstChannel = m_outDevFirstChannel;
         o_params.nChannels = m_outDevChannels;
         RtAudio::StreamParameters* in_params = nullptr;
         m_IsPlaying = true;
         uint32_t bufferSize = m_dacFrames;
         m_dac->openStream( &o_params, in_params, RTAUDIO_FLOAT32, m_dacSampleRate,
                   &bufferSize, rt_audio_callback, reinterpret_cast< void* >( this ) );
         if ( m_dacFrames != bufferSize )
         {
            DE_WARN("m_dacFrames(",bufferSize,") != bufferSize(",bufferSize,")")
         }

         m_dac->startStream();
      }
      catch ( RtAudioError& e )
      {
         e.printMessage();
         m_IsPlaying = false;
      }

      while ( m_IsPlaying )
      {
         std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
      }

      DE_DEBUG("Exit play Audio with a fresh WASAPI stream")
      DE_FLUSH
   }

   static int
   rt_audio_callback(
      void* outputBuffer,
      void* inputBuffer,
      unsigned int frameCount,
      double pts,
      RtAudioStreamStatus status,
      void* userData )
   {
      if ( status == RTAUDIO_OUTPUT_UNDERFLOW ) { DE_WARN("Underflow!") return int( status ); }
      else if ( status == RTAUDIO_INPUT_OVERFLOW ) { DE_WARN("Overflow!") return int( status ); }

      auto play = reinterpret_cast< Play_v3* >( userData );
      if ( !play ) { DE_WARN("No audio!") return 1; }
      if ( !play->m_IsPlaying ) { DE_WARN("No audio playing!") return 1; } // not enough data, eof.

      auto & mixBuffer = play->m_MixBuffer;
      if ( mixBuffer.getFrameCount() < frameCount )
      {
         DE_WARN("Resize mixBuffer")
         mixBuffer.resize( frameCount );
      }

      auto & srcBuffer = play->m_SrcBuffer;
      if ( srcBuffer.getFrameCount() < frameCount )
      {
         DE_WARN("Resize srcBuffer")
         srcBuffer.resize( frameCount );
      }

      // ZeroMemory
      memset( mixBuffer.data(), 0, mixBuffer.size() );
      memset( srcBuffer.data(), 0, srcBuffer.size() );

      float* mixStart = mixBuffer.writePtr< float >();
      float* srcStart = srcBuffer.writePtr< float >();

   #if 1
      // Accumulate/Add Sounds into MixBuffer ...
      if ( !m_IsPaused )
      {
         std::unique_lock< std::mutex > lg( play->m_SoundsMutex );

         // Accumulate/Add Sounds into MixBuffer ...

         for ( Play_v3_Sound & sound : play->m_Sounds )
         {
            if ( !sound.is_playing() )
            {
               continue;
            }

            if ( sound.is_paused() )
            {
               continue;
            }

            // By calling readSamples the source's position will be updated automatically.
            float sourceVolume = 0.01f * sound.getVolume();

            auto nSamples = sound.readSamples( srcBuffer, pts );
            if ( nSamples != srcBuffer.getSampleCount() )
            {
               DE_ERROR("nSamples(",nSamples,") != srcSamples(",srcBuffer.getSampleCount(),")");
               continue; // If source is incomplete, add nothing, goto next source.
            }

            // Loop all samples, works for both ( interleaved + planar ) memory layouts.
            float* pSrc = srcStart;
            float* pMix = mixStart;
            for ( size_t frame = 0; frame < mixBuffer.getFrameCount(); ++frame )
            {
               for ( size_t channel = 0; channel < mixBuffer.getChannelCount(); ++channel )
               {
                  *pMix += (*pSrc) * sourceVolume; // Accumulate +=
                  pMix++;
                  pSrc++;
               }
            }
         }

         lg.unlock();
      }
   #endif

      // Write to interleaved audio stream RTAUDIO_FLOAT32
      float* dst = reinterpret_cast< float* >( outputBuffer );
      float mixerVolume = 0.01f * play->m_Volume;

//      // eof
//      int64_t availFrames = play->m_SampleBuffer.getFrameCount() - play->m_FrameIndex;
//      if ( frameCount >= availFrames )
//      {
//         DE_DEBUG( "m_FrameIndex(",play->m_FrameIndex,") + frameCount(",frameCount,")"
//                   " >= m_SampleBuffer.getFrameCount(", play->m_SampleBuffer.getFrameCount(),")" )

         //play->m_FrameIndex = 0; // rewind dirty hack

      for ( size_t i = 0; i < mixBuffer.getFrameCount(); ++i )
      {
         for ( size_t ch = 0; ch < mixBuffer.getChannelCount(); ++ch )
         {
            float sample = 0.0f;
            if ( mixBuffer.getSample( i, ch, &sample, ST_F32I ) )
            {
               *dst = std::clamp( mixerVolume * sample, -1.0f, 1.0f ); // With Benni Limiter.
            }
            else
            {
               DE_WARN("i(",i,"), ch(",ch,") no sample")
               *dst = 0.f;
            }

            dst++;
         }
      }
   }

   int
   getVolume() const { return m_Volume; }

   void
   setVolume( int vol_in_percent )
   {
      vol_in_percent = Math::clampi( vol_in_percent, 0, 200 );
      m_Volume = vol_in_percent;
      DE_DEBUG("setVolume(",vol_in_percent,"%)")
   }

};

} // end namespace audio
} // end namespace de

int
dbLoadSound( std::string uri )
{
   return de::audio::Play_v3::getInstance().loadSound( uri );
}

void
dbDeleteSound( int soundId )
{
   de::audio::Play_v3::getInstance().deleteSound( soundId );
}


void
dbPlaySound( int soundId, int loops )
{
   de::audio::Play_v3::getInstance().playSound( soundId, loops );
}

void
dbStopSound( int soundId )
{
   de::audio::Play_v3::getInstance().stopSound( soundId );
}

void
dbPauseSound( int soundId )
{
   de::audio::Play_v3::getInstance().pauseSound( soundId );
}


void
dbSetSoundVolume( int soundId, int percent0_400 )
{
   de::audio::Play_v3::getInstance().setSoundVolume( soundId, percent0_400 );
}

int
dbGetSoundVolume( int soundId )
{
   return de::audio::Play_v3::getInstance().getSoundVolume( soundId );
}


void dbPlaySound_V1( std::string uri )
{
   de::audio::Play_v1 play( uri );
}



namespace de {
namespace audio {

struct DB_PLAY
{
   DE_CREATE_LOGGER("DB_PLAY")

   static DB_PLAY &
   getInstance()
   {
      static DB_PLAY s_instance;
      return s_instance;
   }

   DB_PLAY()
   {
      DE_DEBUG("Constructor")
   }

   ~DB_PLAY()
   {

   }

   void clearSounds()
   {
//      for ( size_t i = 0; i < m_Sounds.size(); ++i )
//      {

//         if ( m_Sounds[ i ].)
//      }
   }


   bool
   existSound( int id ) const
   {
      // Search cache for existing id
      auto it = std::find_if( m_Sounds.begin(), m_Sounds.end(),
         [&] ( Sound const & cached ) { return cached.getId() == id; } );
      if ( it == m_Sounds.end() )
      {
         return false;
      }

      return true; // Cache hit
   }

   Sound &
   getSound( int id )
   {
      // Search cache for existing id
      auto it = std::find_if( m_Sounds.begin(), m_Sounds.end(),
         [&] ( Sound const & cached ) { return cached.getId() == id; } );
      if ( it == m_Sounds.end() )
      {
         throw std::runtime_error( std::string("Invalid soundId ") + std::to_string(id) );
      }

      return *it; // Cache hit
   }

   // returns and id stored in IAudioSource or Sound
   int loadSound( std::string uri )
   {
      static int createIdCounter = 0;
      createIdCounter++;

      // Search cache for existing uri
      auto it = std::find_if( m_Sounds.begin(), m_Sounds.end(),
               [&] ( de::audio::Sound const & cached )
               { return cached.getUri() == uri; } );
      if ( it != m_Sounds.end() )
      {
         return it->getId(); // Cache hit
      }

      SampleBuffer sampleBuffer;
      if ( !de::audio::SampleBufferIO::load( sampleBuffer, uri ) )
      {
         DE_ERROR("Cant load sound ", uri)
         return 0;
      }

      SampleBufferDraw::saveImage( sampleBuffer, dbGetFileName(uri) + ".waveform.png", 1024,128 );

      m_Buffers.emplace_back( std::move( sampleBuffer ) );

      int freshId = createIdCounter;

      m_Sounds.emplace_back();
      auto & sound = m_Sounds.back();
      sound.setId( freshId );
      sound.setSampleBuffer( &m_Buffers.back() );
      return sound.getId();
   }

   bool playSound( int soundId )
   {
      // Search cache for existing id
      auto it = std::find_if( m_Sounds.begin(), m_Sounds.end(),
         [&] ( Sound const & cached ) { return cached.getId() == soundId; } );
      if ( it == m_Sounds.end() )
      {
         throw std::runtime_error( std::string("Invalid soundId ") + std::to_string(soundId) );
      }

      Sound & sound = *it;

      m_Mixer.addInput( &sound );

      sound.play();

      if ( !m_Mixer.is_playing() )
      {
         m_Mixer.play();
      }


      return true;
   }
   void setSoundVolume( int soundId, int percent0_400 )
   {

   }
   int getSoundVolume( int soundId )
   {

   }

   std::vector< SampleBuffer > m_Buffers;
   std::vector< Sound > m_Sounds;
   Mixer m_Mixer;
};

} // end namespace audio
} // end namespace de

int dbLoadSound( std::string uri )
{
   return de::audio::DB_PLAY::getInstance().loadSound( uri );
}

void dbPlaySound( int soundId, int loops )
{
   de::audio::DB_PLAY::getInstance().playSound( soundId );
}
void dbSetSoundVolume( int soundId, int percent0_400 )
{
   de::audio::DB_PLAY::getInstance().setSoundVolume( soundId, percent0_400 );
}
int dbSoundVolume( int soundId )
{
   return de::audio::DB_PLAY::getInstance().getSoundVolume( soundId );
}


*/
