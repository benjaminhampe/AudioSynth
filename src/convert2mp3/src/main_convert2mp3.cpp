#include <de/audio/decoder/DecoderFFMPEG.hpp>
#include <de/audio/encoder/EncoderLAME.hpp>
#include <de/FileSystem.hpp>
#include <de/os/CommonDialogs.hpp>
#include <de/audio/buffer/BufferIO.hpp>

namespace de {
namespace audio {

using namespace decoder;
using namespace encoder;

struct convert2mp3
{
   DE_CREATE_LOGGER("convert2mp3")

   static int
   runW( int argc, char** argv )
   {
      std::wstring loadUri;
//      if ( argc > 1 )
//      {
//         loadUri = argv[1];
//      }

      if ( !dbExistFile(loadUri) )
      {
         loadUri = dbOpenFileDlgW( L"Load/open any file that contains an audio stream (mp3,wav,mkv,avi,mp4,etc..) that is supported by libAVCodec ( vlc )" );
      }

      DE_DEBUG("Got loadUri = ", dbStr(loadUri))

      if ( !dbExistFile(loadUri) )
      {
         DE_ERROR("Empty loadUri or not a file, program exits now... Bye bye.")
         DE_FLUSH
         return 0;
      }

      std::wstring saveUri = loadUri + L".convert2mp3.mp3";
//      if ( argc > 2 )
//      {
//         saveUri = argv[ 2 ];
//      }
//      saveUri = loadUri + ".convert2mp3.mp3";

      DE_DEBUG("Got saveUri = ", dbStr(saveUri))

      //DE_DEBUG("[LoadMP3] loadUri = ", loadUri )
      //Buffer workBuffer;

      de::PerformanceTimer perf;
      perf.start();

      DecoderCtx ds;
      DecoderTools::initDecoder( ds );
      if ( !DecoderTools::openDecoder( ds, dbStr(loadUri), -1, true ) )
      {
         DE_ERROR("No decoder open, ",loadUri)
         return false;
      }

      EncoderLAME mp3;
      if ( !mp3.open( dbStr(saveUri), ds.channelCount, ds.sampleRate, 192 ) )
      {
         DE_ERROR("No encoder open, saveUri", dbStr(saveUri) )
         return false;
      }


      size_t totalBytes = 0;
      size_t printBytes = 0;
      size_t totalChunks = 0;
      double totalDuration = 0.0;

      auto onPayload = [&]( AudioPayload const & payload )
      {
         size_t n = payload.frameCount; // * sizeof( float );
         if ( n > 0 )
         {
            mp3.writeSamples( payload.samples.data(), n );
            totalBytes += payload.byteCount();
            printBytes += payload.byteCount();
            totalDuration += payload.duration();
         }
         totalChunks++;
      };

      while ( AVERROR_EOF != DecoderTools::readFrame( ds, onPayload ) )
      {
         // while loop body
         if ( printBytes >= 256 * 1024 * 1024 )
         {
            printBytes -= 256 * 1024 * 1024;

            std::cout << "[FFMPEG] "
            "Bytes(" << dbStrByteCount( totalBytes ) << "), "
            "Duration(" << dbSecondsToString( totalDuration ) << "), "
            "Chunks(" << totalChunks << ")" << std::endl;
         }
      }

      mp3.close();

      perf.stop();
      DE_DEBUG("[JoinMP3] needed ", perf.ms(), " ms, uri = ", dbStr(loadUri) )
      return true;
   }


   static int
   run( int argc, char** argv )
   {
      std::string loadUri;
      if ( argc > 1 )
      {
         loadUri = argv[1];
      }

      if ( !dbExistFile(loadUri) )
      {
         loadUri = dbOpenFileDlg("Load/open any file that contains an audio stream (mp3,wav,mkv,avi,mp4,etc..) that is supported by libAVCodec ( vlc )");
      }

      DE_DEBUG("Got loadUri = ", loadUri)

      if ( !dbExistFile(loadUri) )
      {
         DE_ERROR("Empty loadUri or not a file, program exits now... Bye bye.")
         DE_FLUSH
         return 0;
      }

      std::string saveUri;
      if ( argc > 2 )
      {
         saveUri = argv[ 2 ];
      }
      saveUri = loadUri + ".convert2mp3.mp3";

      //DE_DEBUG("[LoadMP3] loadUri = ", loadUri )
      //Buffer workBuffer;

      de::PerformanceTimer perf;
      perf.start();

      DecoderCtx ds;
      DecoderTools::initDecoder( ds );
      if ( !DecoderTools::openDecoder( ds, loadUri, -1, true ) )
      {
         DE_ERROR("No decoder open, ",loadUri)
         return false;
      }

      EncoderLAME mp3;
      if ( !mp3.open( saveUri, ds.channelCount, ds.sampleRate, 192 ) )
      {
         DE_ERROR("No encoder open, ",loadUri)
         return false;
      }


      size_t totalBytes = 0;
      size_t printBytes = 0;
      size_t totalChunks = 0;
      double totalDuration = 0.0;

      auto onPayload = [&]( AudioPayload const & payload )
      {
         size_t n = payload.frameCount; // * sizeof( float );
         if ( n > 0 )
         {
            mp3.writeSamples( payload.samples.data(), n );
            totalBytes += payload.byteCount();
            printBytes += payload.byteCount();
            totalDuration += payload.duration();
         }
         totalChunks++;
      };

      while ( AVERROR_EOF != DecoderTools::readFrame( ds, onPayload ) )
      {
         // while loop body
         if ( printBytes >= 256 * 1024 * 1024 )
         {
            printBytes -= 256 * 1024 * 1024;

            std::cout << "[FFMPEG] "
            "Bytes(" << dbStrByteCount( totalBytes ) << "), "
            "Duration(" << dbSecondsToString( totalDuration ) << "), "
            "Chunks(" << totalChunks << ")" << std::endl;
         }
      }

      mp3.close();

      perf.stop();
      DE_DEBUG("[JoinMP3] needed ", perf.ms(), " ms, uri = ", loadUri )
      return true;
   }

};

} // end namespace audio
} // end namespace de


//========================================================================
int main(int argc, char** argv)
//========================================================================
{
   bool ok = de::audio::convert2mp3::runW( argc, argv );
   return 0;
}

