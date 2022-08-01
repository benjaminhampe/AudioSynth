#include <de/audio/play3/Sound.hpp>

namespace de {
namespace audio {
namespace play3 {

// =========================================================
Sound::Sound()
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

Sound::~Sound()
{

}

uint64_t
Sound::readSamples( SampleBuffer & dst, double pts )
{
   if ( !m_IsLoaded ) { DE_ERROR("Not loaded") return 0; }
   if ( m_IsPaused ) { DE_ERROR("Paused soundId") return 0; }
   if ( m_SampleBuffer.getSampleType() != ST_F32I ) { DE_ERROR("No src ST_F32I") return 0; }
   if ( dst.getSampleType() != ST_F32I ) { DE_ERROR("No dst ST_F32I") return 0; }

   uint64_t n = dst.getFrameCount();
   uint64_t m = m_SampleBuffer.getFrameCount();
   if ( m_FrameIndex + n >= m )
   {
      //DE_DEBUG( "m_FrameIndex(",m_FrameIndex,") + n(",n,") >= m(",m,")" )
      m_IsPlaying = false;
      m_IsPaused = false;
      m_FrameIndex = 0; // rewind dirty hack
      return 0;
   }
   uint64_t k = m_SampleBuffer.read( m_FrameIndex,
                                dst.getFrameCount(),
                                dst.getChannelCount(),
                                dst.data(),
                                dst.getSampleType() );

   //DE_DEBUG( "k = ",k,", dst.getChannelCount() = ",dst.getChannelCount() )
   m_FrameIndex += k;
   return k;
}

bool
Sound::is_loaded() const { return m_IsLoaded; }

bool
Sound::is_playing() const { return m_IsPlaying; }

bool
Sound::is_paused() const { return m_IsPaused; }

void
Sound::play()
{
   if ( !m_IsLoaded ) { DE_ERROR("Not loaded") return; }
   m_FrameIndex = 0;
   m_IsPlaying = true;
}

void
Sound::stop()
{
   if ( !m_IsLoaded ) { DE_ERROR("Not loaded") return; }
   m_IsPlaying = false;
   m_IsPaused = false;
}

void
Sound::pause()
{
   if ( !m_IsLoaded ) { DE_ERROR("Not loaded") return; }
   m_IsPaused = true;
}

int
Sound::getVolume() const { return m_Volume; }

void
Sound::setVolume( int vol_in_percent )
{
   vol_in_percent = Math::clampi( vol_in_percent, 0, 200 );
   m_Volume = vol_in_percent;
}


} // end namespace play3
} // end namespace audio
} // end namespace de
