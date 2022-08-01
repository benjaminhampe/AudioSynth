#ifndef DE_AUDIO_MUSIC_PLAYER_HPP
#define DE_AUDIO_MUSIC_PLAYER_HPP

#include <de/audio/FileStreamThread.hpp>
#include <de/audio/IDspElement.hpp>

namespace de {
namespace audio {

struct Music : public IDspElement
{
   DE_CREATE_LOGGER("de.audio.Music")

   std::unique_ptr< FileStreamThread > m_fileStream;
   uint64_t m_frameCount;
   uint32_t m_channelCount;
   uint32_t m_sampleRate;
   int32_t m_streamIndex;
   std::atomic< bool > m_isPlaying;
   //bool m_isLooping = false;
   double m_time;
   double m_timeStart;
   std::string m_uri;
   std::vector< float > m_inputBuffer;

   Music()
      : m_isPlaying( false )
      //, m_isLooping( false )
   {
      DE_DEBUG("Constructor")
      m_inputBuffer.resize( 8*1024 );
   }

   ~Music()
   {
      DE_DEBUG("Destructor")
      close();
   }

   void close()
   {
      stop();
      DE_DEBUG("Closing")
      m_fileStream.reset();
   }

   bool open( std::string uri, int streamIndex = -1 )
   {
      if ( m_fileStream ) { DE_ERROR("Already open") return true; }
      if ( uri.empty() ) { DE_ERROR("Empty uri") return false; } // Prevent dead locks.

      m_fileStream.reset( new FileStreamThread() );
      bool ok = m_fileStream->open( uri, streamIndex );
      if ( !ok )
      {
         DE_ERROR("Cant open uri ",uri )
         m_fileStream.reset();
      }
      return ok;
   }

   bool is_open() const { return m_fileStream.get() != nullptr; }
   bool is_playing() const  { return m_isPlaying; }

   void play()
   {
      stop();
      m_timeStart = dbSeconds();
      m_time = 0.0;
      m_isPlaying = true;
      DE_DEBUG("Play ", m_uri)
   }

   void stop()
   {
      if ( m_isPlaying )
      {
         m_isPlaying = false;
         DE_DEBUG("Stop")
      }
   }

   uint64_t
   readSamples( double pts,
                float* dst,
                uint32_t dstFrames,
                uint32_t dstChannels,
                uint32_t dstRate ) override
   {
      size_t dstSamples = dstFrames * dstChannels;
      DSP_RESIZE( m_inputBuffer, dstSamples );
      DSP_FILLZERO( m_inputBuffer, dstSamples );

      // Return something else than zeroes only if (playing + data)
      if ( m_fileStream )
      {
         size_t r = m_fileStream->readSamples( pts, m_inputBuffer.data(), dstFrames, dstChannels, dstRate );
         if ( r < dstSamples )
         {
            DE_WARN("EOF r(",r,") < dstSamples(",dstSamples,")")
         }
      }

      DSP_COPY( m_inputBuffer.data(), dst, dstSamples );
      return dstSamples;
   }


};


} // end namespace audio
} // end namespace de

#endif

