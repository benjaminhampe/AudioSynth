#include <de/audio/SampleBufferIO.hpp>
#include <de/audio/SpektrumFFTW3.hpp>

namespace de {
namespace audio {

struct audio2image
{
   DE_CREATE_LOGGER("audio2image")

   static void
   drawChannelWaveform( SampleBuffer const & src, uint32_t channel, Image & img, Recti pos, uint32_t color )
   {
      auto srcFrames = src.getFrameCount();
      if ( srcFrames < 2 ) { return; }

      auto w = pos.getWidth();
      auto h = pos.getHeight();
      auto dx = float( w ) / float( src.getFrameCount() );
      auto dy = float( h ) / float( 65536.0f );
      DE_ERROR("dx(",dx,"), dy(",dy,")" )

      int16_t sampleA = 0;
      src.getSample( 0, channel, &sampleA, ST_S16 );
      float xA = pos.x;
      float yA = pos.y + int(0.5f * float( h ) - ( dy * sampleA ));
      for ( size_t i = 1; i < srcFrames; ++i )
      {
         int16_t sampleB = 0;
         src.getSample( i, channel, &sampleB, ST_S16 );
         float xB = pos.x + dx*i;
         float yB = pos.y + int(0.5f * float( h ) - ( dy * sampleB ));
         ImagePainter::drawLine( img,
                              Math::round32(xA),
                              Math::round32(yA),
                              Math::round32(xB),
                              Math::round32(yB), color, true );
         xA = xB;
         yA = yB;
         //sampleA = sampleB;
      }
   }

   static void
   drawChannelWaveforms( SampleBuffer const & src, Image & img, Recti pos, uint32_t color )
   {
      int srcChannels = src.getChannelCount();
      DE_ERROR("srcChannels(",srcChannels,")")

      if ( srcChannels < 1 )
      {
         return;
      }

      int w = pos.getWidth();
      int h = pos.getHeight() / srcChannels;

      for ( int i = 0; i < srcChannels; ++i )
      {
         drawChannelWaveform( src, i, img, Recti(pos.x,pos.y+i*h,w,h), color );
      }
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

      if ( !dbExistFile(loadUri) )
      {
         DE_ERROR("Empty loadUri, program exits now... Bye bye")
         DE_FLUSH
         return 0;
      }

      SampleBuffer srcBuffer;

      // Easy and heavy to RAM
      if ( !SampleBufferIO::load( srcBuffer, loadUri, -1, ST_S16I ) )
      {
         DE_ERROR("Cant load uri ",loadUri)
         DE_FLUSH
         return 0;
      }

      DE_DEBUG("WAV ", srcBuffer.toString( true ) )

      std::string mp3Uri = loadUri + ".waveform.mp3";
      if ( de::audio::SampleBufferIO::save( srcBuffer, mp3Uri ) )
      {
         DE_DEBUG("Saved MP3 ",mp3Uri )
      }
      else
      {
         DE_ERROR("Can't save waveform ",mp3Uri )
      }

      std::string saveUri = loadUri + ".waveform.png";
      if ( argc > 2 )
      {
         saveUri = argv[ 2 ];
      }

      de::Image img( 2*4096,2*512 );
      img.fill( 0x00000000 ); // 0xFFFFFFFF

      float fa = 4.0f*255.0f * float( img.getWidth() ) / float( srcBuffer.getFrameCount() );
      DE_DEBUG( "fa = ",fa )
      int a = std::clamp( int(fa), 1, 255 );

      drawChannelWaveforms( srcBuffer, img, img.getRect(), RGBA(255,255,255,a) ); // 0x01000000

      if ( dbSaveImage( img, saveUri ) )
      {
         DE_DEBUG("Save waveform ",saveUri )
      }
      else
      {
         DE_ERROR("Can't save waveform ",saveUri )
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
   int retVal = de::audio::audio2image::run( argc, argv );
   return 0;
}

