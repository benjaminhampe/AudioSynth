#include <de/audio/SampleBufferDraw.hpp>

namespace de {
namespace audio {

void
SampleBufferDraw::drawChannel( SampleBuffer const & src, uint32_t channel,
                               Image & img, Recti pos, uint32_t color )
{
   if ( src.getFrameCount() < 2 )
   {
      DE_DEBUG("Nothing to draw")
      return; // DRAWING...
   }

   int w = pos.getWidth();
   int h = pos.getHeight();
   double n = double( src.getFrameCount() ) / double( w );
   if ( n < 1. ) n = 1.;

   DE_DEBUG("n = ", n )
   DE_FLUSH

   int a = 255;
   uint64_t frameIndex = 0;

   for ( size_t k = 0; k < w; ++k )
   {
      double energy = 0.0;
      for ( size_t i = 0; i < size_t(n); ++i )
      {
         float sample = 0.0f;
         if ( src.getSample( frameIndex + i, 0, &sample, ST_F32 ) )
         {
            energy += double( sample ) * double( sample );
         }
      }

      energy /= double( size_t(n) );

      frameIndex += size_t(n);

      // compute average sample energy in dB.
      int64_t db = 20.0 * ::log10( double( energy ) );

      int x = k;
      int y1 = int( float(h/2) - float(h/2) * db / 120.0f );
      int y2 = int( float(h/2) + float(h/2) * db / 120.0f );

      ImagePainter::drawLine( img, x,y1,x,y2, RGBA(255,255,255,a) );
   }

#if 0
   int64_t s_min = std::numeric_limits< int64_t >::max();
   int64_t s_max = std::numeric_limits< int64_t >::lowest();
   int64_t e_min = std::numeric_limits< int64_t >::max();
   int64_t e_max = std::numeric_limits< int64_t >::lowest();
   int64_t db_min = std::numeric_limits< int64_t >::max();
   int64_t db_max = std::numeric_limits< int64_t >::lowest();
   for ( size_t i = 0; i < src.getFrameCount(); ++i )
   {
      int16_t sample = 0;
      src.getSample( i, channel, &sample, ST_S16 );
      if ( s_min > sample ) s_min = sample; // db_min = std::min( db_min, db );
      if ( s_max < sample ) s_max = sample; // db_max = std::max( db_max, db );

      int64_t energy = int64_t(sample)*sample;
      if ( e_min > energy ) e_min = energy;
      if ( e_max < energy ) e_max = energy;

      // decibel in [-0,1]    -> image rect x in [0,w-1]
      int64_t db = 20.0 * ::log10( double( energy ) ); // compute average sample energy in dB.
      if ( db_min > db ) db_min = db; // db_min = std::min( db_min, db );
      if ( db_max < db ) db_max = db; // db_max = std::max( db_max, db );
   }
   DE_DEBUG("Global.Sample.Min = ", s_min )
   DE_DEBUG("Global.Sample.Max = ", s_max )
   DE_DEBUG("Global.Energy.Min = ", e_min )
   DE_DEBUG("Global.Energy.Max = ", e_max )
   DE_DEBUG("Global.Decibel.Min = ", db_min )
   DE_DEBUG("Global.Decibel.Max = ", db_max )


// DRAWING...

   int w = pos.getWidth();
   int h = pos.getHeight();
   int64_t fc = src.getFrameCount();
   int64_t framesPerPixel = fc / w;

   int a = 2;

   float scale_x = float( w ) / float( fc );
   float scale_y = float( h ) / 360.0f;   // h/2/180

   int16_t sampleA = 0;
   int32_t energyA = 0;
   int32_t dbA = 0;


   src.getSample( 0, channel, &sampleA, ST_S16 );
   energyA = int32_t(sampleA)*sampleA;
   dbA = std::max( int32_t(0), int32_t( 20.0 * ::log10( double( energyA ) ) ) ); // compute average bin energy in dB.

   for ( size_t i = 1; i < src.getFrameCount(); ++i )
   {
      int16_t sampleB = 0;
      int32_t energyB = 0;
      int32_t dbB = 0;

      src.getSample( i, channel, &sampleB, ST_S16 );
      energyB = int32_t(sampleB)*sampleB;
      dbB = std::max( int32_t(0), int32_t( 20.0 * ::log10( double( energyB ) ) ) ); // compute average bin energy in dB.

      float x1 = scale_x * (i-1);
      float x2 = scale_x * (i);

      float y1 = h/2 - scale_y * dbA;
      float y2 = h/2 + scale_y * dbB;

      ImagePainter::drawLine( img, x1,y1,x2,y2, RGBA(255,255,255,a) );

      sampleA = sampleB;
      energyA = energyB;
      dbA = dbB;
   }

   DE_DEBUG("Bin.framesPerPixel = ", framesPerPixel )
   DE_DEBUG("Bin.alphaChannel = ", a )

#endif
}

void
SampleBufferDraw::draw( SampleBuffer const & src, Image & img, Recti pos, uint32_t color )
{
   int w = pos.getWidth();
   int h = pos.getHeight() / src.getChannelCount();

   for ( int i = 0; i < src.getChannelCount(); ++i )
   {
      drawChannel( src, i, img, Recti(pos.x,pos.y+i*h,w,h), color );
   }
}

Image
SampleBufferDraw::createImage( SampleBuffer const & src, int w, int h, uint32_t color )
{
   Image img( w,h );
   img.fillZero();
   draw( src, img, img.getRect(), color );
   return img;
}

bool
SampleBufferDraw::saveImage( SampleBuffer const & src, std::string uri, int w, int h, uint32_t color )
{
   Image img( w,h );
   img.fillZero();
   draw( src, img, img.getRect(), color );
   return dbSaveImage( img, uri );
}


} // end namespace audio
} // end namespace de
