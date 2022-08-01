#ifndef DE_MATH_RECTI_HPP
#define DE_MATH_RECTI_HPP

#include <de/Math.hpp>

namespace de {

struct Recti
{
   int32_t x;
   int32_t y;
   int32_t w;
   int32_t h;
public:
   DE_CREATE_LOGGER("de.Recti")

   constexpr static int32_t const s_Min = std::numeric_limits< int32_t >::lowest();
   constexpr static int32_t const s_Max = std::numeric_limits< int32_t >::max();

   static void test()
   {
      // DE_DEBUG("typename(T) = ", typename(T) )
      DE_DEBUG("s_Min = ", s_Min )
      DE_DEBUG("s_Max = ", s_Max )

      Recti a( 0,0,100,50);
      Recti b( 50,50,33,66);
      Recti c( 33,33,21,123);
      DE_DEBUG("abc_width = ", a.getWidth() + b.getWidth() + c.getWidth() )
      DE_DEBUG("abc_height = ", a.getHeight() + b.getHeight() + c.getHeight() )

//      if ( a.getWidth() + b.getWidth() + c.getWidth() != 239 )
//      {
//         throw std::runtime_error( "Rect< T >.getWidth() failed" );
//      }

//      if ( a.getHeight() + b.getHeight() + c.getHeight() != 239 )
//      {
//         throw std::runtime_error( "Rect< T >.getHeight() failed" );
//      }
   }

   Recti( int dummy = 0 )
      : x( 0 ), y( 0 ), w( 0 ), h( 0 )
   {}

   Recti( int32_t _x, int32_t _y, int32_t _w, int32_t _h )
      : x( 0 ), y( 0 ), w( 0 ), h( 0 )
   {
      set( _x,_y,_w,_h );
   }

   bool operator==( Recti const & o ) const { return x == o.x && y == o.y && w == o.w && h == o.h; }
   bool operator!=( Recti const & o ) const { return !( o == *this ); }

   static Recti fromPoints( int32_t x1, int32_t y1, int32_t x2, int32_t y2 )
   {
      if ( x1 > x2 ) std::swap( x1, x2 );
      if ( y1 > y2 ) std::swap( y1, y2 );
      int w = x2 - x1 + 1;
      int h = y2 - y1 + 1;
      w = std::min( std::max( w, 0 ), 64 * 65536 ); // clamp w in [0,enough]
      h = std::min( std::max( h, 0 ), 64 * 65536 ); // clamp h in [0,enough]
      return Recti( x1, y1, w, h );
   }

   Recti( Recti const & r )
      : x( r.x ), y( r.y ), w( r.w ), h( r.h )
   {}

   void
   reset() { x = y = w = h = 0; }

   void
   setWidth( int32_t _w )
   {
      if ( _w >= 0 )
      {
         w = _w;
      }
      else
      {
         //DE_DEBUG("Not impl")
         w = 0;
      }
   }

   void
   setHeight( int32_t _h )
   {
      if ( _h >= 0 )
      {
         h = _h;
      }
      else
      {
         //DE_DEBUG("Not impl")
         h = 0;
      }
   }

   void
   set( int32_t _x, int32_t _y, int32_t _w, int32_t _h )
   {
      x = _x;
      y = _y;
      setWidth( _w );
      setHeight( _h );
   }

   void
   addInternalPoint( glm::ivec2 const & p )
   {
      addInternalPoint( p.x, p.y );
   }

   void
   addInternalPoint( int32_t _x, int32_t _y )
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

   int32_t getX() const { return x; }
   int32_t getY() const { return y; }
   int32_t getWidth() const { return w; }
   int32_t getHeight() const { return h; }
   int32_t getCenterX() const { return x + getWidth()/2; }
   int32_t getCenterY() const { return y + getHeight()/2; }

   inline int32_t getX1() const { return getX(); }
   inline int32_t getY1() const { return getY(); }
   inline int32_t getX2() const { return w > 1 ? x + w - 1 : x; }
   inline int32_t getY2() const { return h > 1 ? y + h - 1 : y; }

   glm::ivec2 getPos() const { return { x, y }; }
   glm::ivec2 getSize() const { return { getWidth(), getHeight() }; }
   glm::ivec2 getCenter() const { return { x + getWidth()/2, y + getHeight()/2 }; }
   glm::ivec2 getTopLeft() const { return { getX(), getY() }; }
   glm::ivec2 getLowerRight() const { return { getX2(), getY2() }; }

   std::string
   toString() const
   {
      std::ostringstream s;
      s << x << "," << y << "," << w << "," << h;
      return s.str();
   }

//   Recti operator+ ( int32_t v ) const { return Recti( *this ) += v; }
//   Recti operator- ( int32_t v ) const { return Recti( *this ) -= v; }
//   Recti operator* ( int32_t v ) const { return Recti( *this ) *= v; }
//   Recti operator/ ( int32_t v ) const { return Recti( *this ) /= v; }

   // Translate x,y. (w,h) stay untouched.
//   Recti& operator+= ( glm::ivec2 const & p ) { x += p.x; y += p.y; return *this; }
//   Recti& operator-= ( glm::ivec2 const & p ) { x -= p.x; y -= p.y; return *this; }
//   Recti& operator+= ( int32_t v ) { x += v; y1 += v; x2 += v; y2 += v; return *this; }
//   Recti& operator-= ( int32_t v ) { x -= v; y1 -= v; x2 -= v; y2 -= v; return *this; }
//   Recti& operator*= ( int32_t v ) { x *= v; y *= v; x2 *= v; y2 *= v; return *this; }
//   Recti& operator/= ( int32_t v ) { x /= v; y /= v; x2 /= v; y2 /= v; return *this; }

   bool
   contains( int mx, int my, int touchWidth = 0 ) const
   {
      if ( mx < getX() + touchWidth ) return false;
      if ( my < getY() + touchWidth ) return false;
      if ( mx > getX2() - touchWidth ) return false;
      if ( my > getY2() - touchWidth ) return false;
      return true;
   }

   glm::vec4
   toVec4( int atlas_w, int atlas_h ) const
   {
      float fx = float( getX() ) / float( atlas_w-1 );
      float fy = float( getY() ) / float( atlas_h-1 );
      float fw = float( getWidth() ) / float( atlas_w );
      float fh = float( getHeight() ) / float( atlas_h );
      return glm::vec4( fx, fy, fw, fh );
   }

};

} // end namespace de.

inline std::ostream &
operator<< ( std::ostream & o, de::Recti const & rect )
{
   o << rect.toString();
   return o;
}

#endif // DELIB_MATH_HPP
