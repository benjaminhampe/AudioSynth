#ifndef DE_MATH_RECTF_HPP
#define DE_MATH_RECTF_HPP

#include <de/Recti.hpp>

namespace de {

//=============================================================================
/// @brief Rectf
//=============================================================================
struct Rectf
{
   DE_CREATE_LOGGER("de.Rectf")
   typedef float T;

   T x,y,w,h;

   Rectf();
   Rectf( T x_, T y_, T w_, T h_ );
   Rectf( Rectf const & r );

   std::string
   toString() const;

   T u1() const { return x; }
   T v1() const { return y; }
   T u2() const { return x+w; }
   T v2() const { return y+h; }
   T du() const { return w; }
   T dv() const { return h; }

   T getU1() const { return x; }
   T getV1() const { return y; }
   T getU2() const { return x+w; }
   T getV2() const { return y+h; }
   T getDU() const { return w; }
   T getDV() const { return h; }

   T getX() const { return x; }
   T getY() const { return y; }
   T getWidth() const { return w; }
   T getHeight() const { return h; }

   T getCenterX() const { return x + T( 0.5 ) * getWidth(); }
   T getCenterY() const { return y + T( 0.5 ) * getHeight(); }

   T getX1() const { return x; }
   T getY1() const { return y; }
   T getX2() const { return x+w; }
   T getY2() const { return y+h; }

   glm::tvec2< T > getCenter() const { return { x + getWidth()/2, y + getHeight()/2 }; }
   glm::tvec2< T > getTopLeft() const { return { getX1(), getY1() }; }
   glm::tvec2< T > getLowerRight() const { return { getX2(), getY2() }; }

   static Rectf
   identity();

   static Rectf
   fromRecti( Recti const & pos, int w, int h );

   Recti
   toRecti( int scr_w, int scr_h ) const;

   void
   zero();

   void
   addInternalPoint( glm::tvec2< T > const & point );

   void
   addInternalPoint( T _x, T _y );

};


} // end namespace de.

//template < typename T >
inline std::ostream &
operator<< ( std::ostream & o, de::Rectf const & rect )
{
   o << rect.toString();
   return o;
}

#endif // DELIB_MATH_HPP
