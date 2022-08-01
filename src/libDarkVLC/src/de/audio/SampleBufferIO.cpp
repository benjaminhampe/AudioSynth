#include <de/audio/SampleBufferIO.hpp>
#include <de/audio/io/SampleBufferWAV.hpp>
#include <de/audio/io/SampleBufferFFMPEG.hpp>
#include <de/audio/io/SampleBufferLAME.hpp>
#include <de/audio/io/SampleBufferSND.hpp>

namespace de {
namespace audio {

bool
SampleBufferIO::load( SampleBuffer & dst, std::string uri, int streamIndex,
                        ESampleType convertType, int sampleRate )
{
   PerformanceTimer perf;
   perf.start();

   bool ok = false;

   if ( uri.empty() )
   {
      DE_ERROR("Empty uri, ", uri )
      return ok;
   }

   std::string suffix = FileSystem::fileSuffix( uri );
   if ( suffix.empty() )
   {
      DE_ERROR("Empty suffix, ", uri )
      return ok;
   }

//   if ( suffix == "wav" )
//   {
//      try
//      {
//         ok = io::SampleBufferWAV::load( dst, uri );
//      }
//      catch ( std::exception & e ) // Input file is corrupt or not a WAV file
//      {
//         DE_ERROR("SampleBufferWAV::load() got exception ",e.what()," for uri ", uri )
//         ok = false;
//      }
//   }

//#ifdef USE_LIBSNDFILE
//   if ( !ok )
//   {
//      ok = io::SampleBufferSND::load( dst, uri );
//   }
//#endif

   if ( !ok )
   {
      ok = io::SampleBufferFFMPEG::load( dst, uri, streamIndex, convertType, sampleRate );
   }

   perf.stop();

   if ( ok )
   {
      DE_DEBUG("[Load] time(", perf.ms(),"ms), uri(", uri, "), dst(", dst.toString(),")" )
   }
   else
   {
      DE_ERROR("[Load] time(", perf.ms(),"ms), uri(", uri, "), dst(", dst.toString(),")" )
   }

   return ok;
}

bool
SampleBufferIO::save( SampleBuffer const & src, std::string uri )
{
   PerformanceTimer perf;
   perf.start();

   bool ok = false;

   if ( uri.empty() )
   {
      DE_ERROR("Empty uri, ", uri )
      return ok;
   }

   std::string suffix = FileSystem::fileSuffix( uri );
   if ( suffix.empty() )
   {
      DE_ERROR("Empty suffix, ", uri )
      return ok;
   }

   if ( suffix == "wav" )
   {
      ok = io::SampleBufferWAV::save( src, uri );
   }

#ifdef USE_LIBSNDFILE

   if ( !ok || suffix == "ogg" || suffix == "flac" )
   {
      ok = io::SampleBufferSND::save( src, uri );
   }

#endif

   if ( !ok || suffix == "mp3" )
   {
      ok = io::SampleBufferLAME::save( src, uri );
   }

   if ( !ok )
   {
      ok = io::SampleBufferFFMPEG::save( src, uri );
   }

   perf.stop();

   if ( ok )
   {
      DE_DEBUG("[Save] time(", perf.ms(),"ms), uri(", uri, "), src(", src.toString(),")" )
   }
   else
   {
      DE_ERROR("[Save] time(", perf.ms(),"ms), uri(", uri, "), src(", src.toString(),")" )
   }

   return ok;
}




} // end namespace audio
} // end namespace de











