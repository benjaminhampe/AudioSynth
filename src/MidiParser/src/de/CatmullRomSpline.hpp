#ifndef DE_MATH_SPLINE_CATMULL_ROM_HPP
#define DE_MATH_SPLINE_CATMULL_ROM_HPP

#include <de_glm.hpp>
#include <de/Logger.hpp>

namespace de
{
   /// @brief A Catmull-Rom spline is cubic polynom ( polynom of 3rd grade )
   /// A general (1D)polynom of 3rd grade looks like: p( x ) = c0 + c1*x + c2*x^2 + c3*x^3
   /// The trick for 2D seems to use 2 cubic polynoms, one describes x(t) the second y(t).
   template < typename T, glm::precision P = glm::defaultp >
   struct CatmullRomSpline
   {
      typedef glm::tvec2< T, P > P2;

      T c0, c1, c2, c3;

      CatmullRomSpline()
         : c0( T(0) ), c1( T(0) ), c2( T(0) ), c3( T(0) )
      {}

      CatmullRomSpline( T coeff0, T coeff1, T coeff2, T coeff3 )
         : c0( coeff0 ), c1( coeff1 ), c2( coeff2 ), c3( coeff3 )
      {}

      T
      eval( T t ) const
      {
         T tt = t*t;
         T ttt = tt * t;
         return c0 + c1*t + c2*tt + c3*ttt;
      }

      /// @brief Compute coefficients for a cubic polynomial
      /// p(0) = x0, p(1) = x1  and   p'(0) = t0, p'(1) = t1.
      template < typename B >
      void initPolynom( B x0, B x1, B t0, B t1 )
      {
         c0 = T( x0 );
         c1 = T( t0 );
         c2 =-T(3)*T( x0 ) + T(3)*T( x1 ) - T(2)*T( t0 ) - T( t1 );
         c3 = T(2)*T( x0 ) - T(2)*T( x1 ) + T( t0 ) + T( t1 );
      }

      // standard Catmull-Rom spline: interpolate between x1 and x2 with previous/following points x0/x3
      // (we don't need this here, but it's for illustration)
      template < typename B >
      void initCatmullRom( B x0, B x1, B x2, B x3 )
      {
         // Catmull-Rom with tension 0.5
         initPolynom( x1, x2, T(0.5)*( x2 - x0 ), T(0.5)*( x3 - x1 ) );
      }

      // compute coefficients for a nonuniform Catmull-Rom spline
      template < typename B >
      void initNonUniformCatmullRom( B x0, B x1, B x2, B x3, B dt0, B dt1, B dt2 )
      {
         // compute tangents when parameterized in [t1,t2]
         // assert( );
         B t1 = (x1 - x0) / dt0 - (x2 - x0) / (dt0 + dt1) + (x2 - x1) / dt1;
         B t2 = (x2 - x1) / dt1 - (x3 - x1) / (dt1 + dt2) + (x3 - x2) / dt2;

         // rescale tangents for parametrization in [0,1]
         t1 *= dt1;
         t2 *= dt1;

         initPolynom( x1, x2, t1, t2 );
      }

      static void
      initCentripetalCR( P2 const & p0, P2 const & p1, P2 const & p2, P2 const & p3,
         CatmullRomSpline & px, CatmullRomSpline & py )
      {
         T dt0 = std::pow( glm::length2( p1-p0 ), T(0.25) );
         T dt1 = std::pow( glm::length2( p2-p1 ), T(0.25) );
         T dt2 = std::pow( glm::length2( p2-p2 ), T(0.25) );

         // safety check for repeated points
         if ( dt1 < T(1e-9) ) dt1 = T(1);
         if ( dt0 < T(1e-9) ) dt0 = dt1;
         if ( dt2 < T(1e-9) ) dt2 = dt1;

         px.initNonUniformCatmullRom( p0.x, p1.x, p2.x, p3.x, dt0, dt1, dt2 );
         py.initNonUniformCatmullRom( p0.y, p1.y, p2.y, p3.y, dt0, dt1, dt2 );
      }


   };

   struct CatmullRomSplineTest
   {
      DE_CREATE_LOGGER("de.CatmullRomSplineTest")

      static void
      test()
      {
         glm::vec2 p0(0,0), p1(1,1), p2(1.1,1), p3(2,0);
         CatmullRomSpline< float > px, py;
         CatmullRomSpline< float >::initCentripetalCR( p0, p1, p2, p3, px, py );

         // We should draw a picture now.
         for ( int i = 0; i <= 10; ++i )
         {
            DE_DEBUG( "Point2(", px.eval( 0.1f*i ), ",", py.eval( 0.1f*i ), ")" )
         }
      }
   };

} // end namespace de


#endif
