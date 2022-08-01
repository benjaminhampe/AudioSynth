#include <de/Rectf.hpp>

namespace de {

Rectf::Rectf() : x( 0 ), y( 0 ), w( 0 ), h( 0 )
{}

Rectf::Rectf( T x_, T y_, T w_, T h_ ) : x( x_ ), y( y_ ), w( w_ ), h( h_ )
{}

Rectf::Rectf( Rectf const & r ) : x( r.x ), y( r.y ), w( r.w ), h( r.h )
{}

std::string
Rectf::toString() const
{
   std::ostringstream s;
   s << x << "," << y << "," << w << "," << h;
   return s.str();
}

void
Rectf::zero() { x = y = w = h = T(0); }

Rectf
Rectf::identity()
{
   return Rectf( T(0), T(0), T(1), T(1) );
}

Rectf
Rectf::fromRecti( Recti const & pos, int w, int h )
{
   int x1 = pos.getX1();
   int y1 = pos.getY1();
   int x2 = pos.getX2();
   int y2 = pos.getY2();
   T u1 = (T(x1)+0.5f) / T( w );
   T v1 = (T(y1)+0.5f) / T( h );
   T u2 = (T(x2)+0.5f) / T( w );
   T v2 = (T(y2)+0.5f) / T( h );

//   T u1 = (T(x1) + T(0.5)) / T( w );
//   T v1 = (T(y1) + T(0.5)) / T( h );
//   T u2 = (T(pw)) / T( w );
//   T v2 = (T(ph)) / T( h );
   return Rectf( u1, v1, u2-u1, v2-v1 );

//   T u1 = (T(x1)+0.5f) / T( w );
//   T v1 = (T(y1)+0.5f) / T( h );
//   T u2 = (T(x2)) / T( w );
//   T v2 = (T(y2)) / T( h );
//   T tw = (T(pos.getWidth())-0.5f) / T( w );
//   T th = (T(pos.getHeight())-0.5f) / T( h );
//   return Rectf( u1, v1, tw, th );

}

Recti
Rectf::toRecti( int scr_w, int scr_h ) const
{
   int ix = Math::round32( x * scr_w );
   int iy = Math::round32( y * scr_h );
   int iw = Math::round32( w * scr_w );
   int ih = Math::round32( h * scr_h );
   return Recti( ix,iy,iw,ih );
}

void
Rectf::addInternalPoint( glm::tvec2< T > const & point )
{
   addInternalPoint( point.x, point.y );
}

void
Rectf::addInternalPoint( T _x, T _y )
{
   // handle _x
   if ( _x < x )  // _x is lower, reposition rect, increase rect size
   {
      int32_t delta = x - _x; // groesser - kleiner
      x = _x;
      w += delta;
   }
   else  // _x is inside rect ( nothing todo ), or right from it ( increase rect size )
   {
      int32_t x2 = x + w - 1;
      if ( _x > x2 ) // _x is right from rectangle ( must increase rect size, no pos change )
      {
         int32_t delta = _x - x2; // groesser - kleiner
         w += delta; // increase w by portion that was not covered from x2 to _x
      }
      // else  // _x is inside rect ( nothing todo )
   }

   // handle _y
   if ( _y < y )
   {
      int32_t delta = y - _y; // groesser - kleiner
      y = _y;
      h += delta;
   }
   else // _y is inside rect ( nothing todo ), or below from it ( increase rect size )
   {
      int32_t y2 = y + h - 1;
      if ( _y > y2 ) // _y is below rectangle ( must increase rect size, no pos change )
      {
         int32_t delta = _y - y2; // groesser - kleiner
         h += delta; // increase h by portion that was not covered from y2 to _y
      }
      // else  // _y is inside rect ( nothing todo )
   }
}

} // end namespace de.

/*
   static T
   computeU1( int32_t x1, int32_t w )
   {
      if ( w < 2 ) return T(0);
      return (T(x1) + T(0.5)) / T( w ); // ( T(0.5) + T(x1) ) / T( w );
   }

   static T
   computeV1( int32_t y1, int32_t h )
   {
      if ( h < 2 ) return T(0);
      return (T(y1) + T(0.5)) / T( h ); // return ( T(0.5) + T(y1) ) / T( h );
   }

   static T
   computeU2( int32_t x2, int32_t w )
   {
      if ( w < 2 ) return T(1);
      return (T(x2) + T(0.5)) / T( w ); // return ( T(0.5) + T(x2) ) / T( w );
   }

   static T
   computeV2( int32_t y2, int32_t h )
   {
      if ( h < 2 ) return T(1);
      return (T(y2) + T(0.5)) / T( h ); // return ( T(0.5) + T(y2) ) / T( h );
   }

   static T
   computeDU( int32_t w2, int32_t w )
   {
      if ( w < 1 ) return T(0);
      return (T(w2) - T(1)) / (T( w )- T(1));
   }

   static T
   computeDV( int32_t h2, int32_t h )
   {
      if ( h < 1 ) return T(0);
      return (T(h2) - T(1)) / (T( h )- T(1));
   }
*/
