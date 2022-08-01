#include <DarkVLC.hpp>
#include <de/audio/SpektrumFFTW3.hpp>
#include <de/audio/SampleBufferDraw.hpp>

namespace de {
namespace audio {

struct convert2mp3
{
   DE_CREATE_LOGGER("convert2mp3")

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

      if ( !dbExistFile(loadUri) )
      {
         DE_ERROR("Empty loadUri, program exits now... Bye bye")
         DE_FLUSH
         return 0;
      }

      SampleBuffer srcBuffer;

      // Heavy load to RAM for now ( converted to float32, very heavy in RAM )
      if ( !SampleBufferIO::load( srcBuffer, loadUri, -1, ST_F32I ) )
      {
         DE_ERROR("Cant load uri ",loadUri)
         DE_FLUSH
         return 0;
      }

      if ( srcBuffer.getSampleRate() < 44100 )
      {
         DE_ERROR("Resampling from ", srcBuffer.getSampleRate(),"Hz to 44100Hz")
         DE_FLUSH
         SampleBuffer dstBuffer;
         dstBuffer.setFormat( ST_F32I, srcBuffer.getChannelCount(), 44100 );
         ResamplerFF::resample( srcBuffer, dstBuffer ); // Heavy resample for now
         srcBuffer = dstBuffer; // Heavy copy for now
      }

      std::string saveUri;
      if ( argc > 2 )
      {
         saveUri = argv[ 2 ];
      }

      saveUri = loadUri + ".convert2mp3.waveform.png";
      de::Image img( 2048,1440 );
      img.fill( 0x00000000 );
      de::audio::SampleBufferDraw::drawChannel( srcBuffer, 0, img, img.getRect() );
      dbSaveImage( img, saveUri );

      saveUri = loadUri + ".convert2mp3.spektrum.png";
      img.fill( 0x00000000 );
      de::audio::SpektrumTools::draw( srcBuffer, 0, img, img.getRect() );
      dbSaveImage( img, saveUri );

#if 0
      saveUri = loadUri + ".convert2mp3.wav";
      if ( SampleBufferIO::save( srcBuffer, saveUri ) )
      {
         DE_DEBUG("Did save WAV ",saveUri )
         DE_FLUSH
      }
      else
      {
         DE_ERROR("Can't save WAV ",saveUri )
         DE_FLUSH
      }
#endif

      saveUri = loadUri + ".convert2mp3.mp3";
      if ( SampleBufferIO::save( srcBuffer, saveUri ) )
      {
         DE_DEBUG("Did save MP3 ",saveUri )
         DE_FLUSH
      }
      else
      {
         DE_ERROR("Can't save MP3 ",saveUri )
         DE_FLUSH
      }

      return 0;
   }

};

} // end namespace audio
} // end namespace de


//========================================================================
int main(int argc, char** argv)
//========================================================================
{
   de::LinearColorGradient::test();
   int retVal = de::audio::convert2mp3::run( argc, argv );
   return 0;
}

