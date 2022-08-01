#include <de/audio/play3/SoundPlayer.hpp>

//#include <de/audio/Sound.hpp>
//#include <de/audio/Mixer.hpp>
#include <de/audio/SampleBuffer.hpp>
#include <de/audio/SampleBufferIO.hpp>
#include <de/audio/SampleBufferDraw.hpp>
#include <de/audio/ResamplerST.hpp>
#include <de/audio/ResamplerFF.hpp>

namespace de {
namespace audio {
namespace play3 {

// =========================================================
SoundPlayer::SoundPlayer()
// =========================================================
   : m_IsPlaying( false )
   , m_IsPaused( false )
   , m_Volume( 50 )
   , m_dac( nullptr )
   , m_dacSampleRate( 48000 )
   , m_dacFrames( 1024 )
   , m_outDevIndex( 0 )
   , m_outDevChannels( 2 )
   , m_outDevFirstChannel( 0 )
//      , m_FrameIndex( 0 )
//      , m_FrameCount( 0 )

//   , m_ThreadPtr( nullptr )
{
   //m_Uri = uri;
   //m_ThreadPtr = new std::thread( &SoundPlayer::run, this );
   //blockUntilReady();
   // std::mutex m;
   // std::unique_lock< std::mutex > guard( m );
   // m_BlockSignalReady.wait( guard, [ this ] () { return m_IsReady; } );
}

SoundPlayer::~SoundPlayer()
{
   stop();
   clearSounds();
}

void
SoundPlayer::clearSounds()
{
   //std::unique_lock< std::mutex > lg( m_SoundsMutex );
   m_Sounds.clear();
}

SoundPlayer &
SoundPlayer::getInstance()
{
   static SoundPlayer s_Instance;
   return s_Instance;
}

bool
SoundPlayer::is_playing() const { return m_IsPlaying; }

void
SoundPlayer::play()
{
   stop();

   m_dac = new RtAudio( RtAudio::WINDOWS_WASAPI );
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

//      std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );

//      if ( !m_dac->isStreamRunning() )
//      {
//         stop();
//      }
   }
   catch ( RtAudioError& e )
   {
      m_IsPlaying = false;
      e.printMessage();
   }

   DE_DEBUG("Started AudioStream "
               "m_dacFrames(", m_dacFrames,"), "
               "m_dacSampleRate(",m_dacSampleRate,")")
}

void
SoundPlayer::stop()
{
   m_IsPlaying = false;
   m_IsPaused = false;

   for ( Sound & sound : m_Sounds )
   {
      sound.stop();
   }

   if ( m_dac )
   {
      DE_DEBUG("Stop")
      //m_IsPlaying = false;
      m_dac->stopStream();
      m_dac->closeStream();
      delete m_dac;
      m_dac = nullptr;
   }


}


int
SoundPlayer::rt_audio_callback(
   void* outputBuffer,
   void* inputBuffer,
   unsigned int frameCount,
   double pts,
   RtAudioStreamStatus status,
   void* userData )
{
   if ( status == RTAUDIO_OUTPUT_UNDERFLOW ) { DE_WARN("Underflow!") return int( status ); }
   else if ( status == RTAUDIO_INPUT_OVERFLOW ) { DE_WARN("Overflow!") return int( status ); }

   auto play = reinterpret_cast< SoundPlayer* >( userData );
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

   for ( Sound & sound : play->m_Sounds )
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

      auto nFrames = sound.readSamples( srcBuffer, pts );
      if ( nFrames != srcBuffer.getFrameCount() )
      {
         //DE_ERROR("nSamples(",nSamples,") != srcSamples(",srcBuffer.getSampleCount(),")");
         //continue; // If source is incomplete, add nothing, goto next source.
      }

      // Loop all samples, works for both ( interleaved + planar ) memory layouts.
      float* pSrc = srcStart;
      float* pMix = mixStart;
      for ( size_t frame = 0; frame < nFrames; ++frame )
      {
         for ( size_t channel = 0; channel < mixBuffer.getChannelCount(); ++channel )
         {
            *pMix += (*pSrc) * sourceVolume; // Accumulate +=
            pMix++;
            pSrc++;
         }
      }
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
            *dst = std::clamp( mixerVolume * sample, -1.0f, 1.0f ); // Limiter.
         }
         else
         {
            //DE_WARN("i(",i,"), ch(",ch,") no sample")
            *dst = 0.f;
         }

         dst++;
      }
   }
}


int
SoundPlayer::getVolume() const { return m_Volume; }

void
SoundPlayer::setVolume( int vol_in_percent )
{
   vol_in_percent = Math::clampi( vol_in_percent, 0, 200 );
   m_Volume = vol_in_percent;
   DE_DEBUG("setVolume(",vol_in_percent,"%)")
}

int
SoundPlayer::addSound( std::string uri, SampleBuffer const & buffer )
{
   auto it = std::find_if( m_Sounds.begin(), m_Sounds.end(),
      [&] ( Sound const & cached ) { return cached.getUri() == uri; } );

   if ( it != m_Sounds.end() )
   {
      return it->getId();
   }

   static int createCounterId = 1;
   int soundId = createCounterId++;

   m_Sounds.emplace_back();
   Sound & sound = m_Sounds.back();
   sound.m_Uri = uri;
   sound.m_Id = soundId;
   sound.m_Volume = 100;
   sound.m_SampleBuffer = buffer;
   sound.m_IsLoaded = true;
   sound.m_IsPlaying = false;
   sound.m_IsPaused = false;
   sound.m_IsLooping = false;

   auto saveUri = uri + "." + std::to_string( soundId ) + ".loaded.wav";
   SampleBufferIO::save( sound.m_SampleBuffer, saveUri );

   return soundId;
}

int
SoundPlayer::loadSound( std::string uri )
{
   auto it = std::find_if( m_Sounds.begin(), m_Sounds.end(),
      [&] ( Sound const & cached ) { return cached.getUri() == uri; } );

   if ( it != m_Sounds.end() )
   {
      return it->getId();
   }

   // int id = 1;

//      for ( auto & sound : m_Sounds )
//      {
//         if ( sound.getI)
//      }

   static int createCounterId = 1;
   int soundId = createCounterId++;

   m_Sounds.emplace_back();
   Sound & sound = m_Sounds.back();
   sound.m_Uri = uri;
   sound.m_Id = soundId;
   sound.m_Volume = 100;
   sound.m_IsLoaded = SampleBufferIO::load( sound.m_SampleBuffer, uri, -1, ST_F32I, m_dacSampleRate );
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
      DE_ERROR("No ST_F32I" )
   }

   int srcRate = sound.m_SampleBuffer.getSampleRate();
   int dstRate = m_dacSampleRate;
   if ( srcRate != dstRate )
   {
      DE_ERROR("Resample srcRate(",srcRate,"Hz) != dstRate(",dstRate,"Hz)" )
   }

   auto saveUri = uri + ".1.loaded.wav";
   SampleBufferIO::save( sound.m_SampleBuffer, saveUri );

   return soundId;
}

void
SoundPlayer::deleteSound( int soundId )
{
   if ( !soundId ) return;
   auto it = std::find_if( m_Sounds.begin(), m_Sounds.end(),
      [&] ( Sound const & cached ) { return cached.m_Id == soundId; } );

   if ( it != m_Sounds.end() )
   {
      m_Sounds.erase( it );
      DE_DEBUG("Deleted soundId ",soundId)
   }
}

void
SoundPlayer::playSound( int soundId, int loops )
{
   if ( !soundId ) return;
   auto it = std::find_if( m_Sounds.begin(), m_Sounds.end(),
      [&] ( Sound const & cached ) { return cached.m_Id == soundId; } );

   if ( it == m_Sounds.end() )
   {
      DE_ERROR("No soundId ",soundId)
      return;
   }

   DE_DEBUG("Play soundId ",soundId, " Hz(", it->m_SampleBuffer.getSampleRate(),"), dacHz(",m_dacSampleRate,")")
   it->play();

   if ( !is_playing() )
   {
      play();
   }
}

void
SoundPlayer::pauseSound( int soundId )
{
   if ( !soundId ) return;
   auto it = std::find_if( m_Sounds.begin(), m_Sounds.end(),
      [&] ( Sound const & cached ) { return cached.getId() == soundId; } );

   if ( it == m_Sounds.end() )
   {
      DE_ERROR("No soundId ",soundId)
      return;
   }

   DE_DEBUG("Pause soundId ",soundId)
   it->pause();

   if ( !is_playing() )
   {
      play();
   }
}

void
SoundPlayer::stopSound( int soundId )
{
   if ( !soundId ) return;
   auto it = std::find_if( m_Sounds.begin(), m_Sounds.end(),
      [&] ( Sound const & cached ) { return cached.getId() == soundId; } );

   if ( it == m_Sounds.end() )
   {
      DE_ERROR("No soundId ",soundId)
      return;
   }

   DE_DEBUG("Stop soundId ",soundId)
   it->stop();

   if ( !is_playing() )
   {
      play();
   }
}

int
SoundPlayer::getSoundVolume( int soundId ) const
{
   if ( !soundId ) return 0;
   auto it = std::find_if( m_Sounds.begin(), m_Sounds.end(),
      [&] ( Sound const & cached ) { return cached.getId() == soundId; } );

   if ( it == m_Sounds.end() )
   {
      DE_ERROR("No soundId ",soundId)
      return 0;
   }

   return it->getVolume();
}

void
SoundPlayer::setSoundVolume( int soundId, int vol_in_percent )
{
   if ( !soundId ) return;
   auto it = std::find_if( m_Sounds.begin(), m_Sounds.end(),
      [&] ( Sound const & cached ) { return cached.getId() == soundId; } );

   if ( it == m_Sounds.end() )
   {
      DE_ERROR("No soundId ",soundId)
      return;
   }

   it->setVolume( vol_in_percent );
   DE_DEBUG("setVolume(",vol_in_percent,"%)")
}


} // end namespace play3
} // end namespace audio
} // end namespace de
