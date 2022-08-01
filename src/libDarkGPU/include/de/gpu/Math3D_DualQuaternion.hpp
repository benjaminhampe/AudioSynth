#ifndef BENNI_GPU_DUALQUATERNION_HPP
#define BENNI_GPU_DUALQUATERNION_HPP

#include <de/gpu/Math3D_Quaternion.hpp>

namespace de {
namespace gpu {

// =======================================================================
// DualQuat = DualNumber< Quat >
// =======================================================================

// DualNumber< T > bringt neue Konstante epsilon mit e^2 = 0.

   // DualQuat = Duale Zahl( Quat a, Quat b )
   //
   // mit Arithmetik: e^2 = 0, i^2=j^2=k^2=ijk = -1
   //
   // mit d1 = r1 + t1*e    r1 - primary part ( rotation1 )
   //                       t1 - secondary/dual part ( translation1 )
   //     d2 = r2 + t2*e
   //
// DualNumber< T > Arithmetik  mit e^2 = 0 oder e^n = 0 mit n > 1.

// DualNumber< T > Addition: d1 + d2 = r1 + r2 + e*( t1 + t2 )

// DualNumber< T > Multiplikation: d1 * d2 = r1*r2 + e*(r1*t2 + r2*t1)
//        = (r1 + e*t1) *( r2 + e*t2 )
//        = r1*( r2 + e*t2 ) + e*t1 *( r2 + e*t2 )
//        = r1*r2 + e(r1*t2 + t1*r2)

// DualNumber< T > Division:         d3 = d1 / d2 = (r1 + e*t1 ) / ( r2 + e*t2 )

   // Division:   = [(r1 + e*t1 )( r2 + e*t2 )] / [( r2 + e*t2 )( r2 + e*t2 )]
   // Division:   = [r1*r2 + e*(r1*t2 + r2*t1)] / [r2*r2 + e*(2*r2*t2)]
   // Division:   = [r1*r2 + e*(r1*t2 + r2*t1)] / [r2*r2 + e*(2*r2*t2)]
   // Division:   = (p + e*q)/(u + e*v)
   // Division:   = (p + qe)(u - ve) / (u + ve)(u - ve)
   // Division:   = (pu - pve + que - qvee) / (uu - uve + uve - vvee)
   // Division:   = (pu - pve + que - qvee) / uu
   // Division:   = (pu + (qu - pv)e) / uu

template < typename T, glm::precision P = glm::defaultp >
struct DualQuat
{
   DE_CREATE_LOGGER("de.DualQuat")
   typedef glm::tmat3x3< T,P > M3;
   typedef glm::tmat4x4< T,P > M4;
   typedef glm::tvec2< T,P > V2;
   typedef glm::tvec3< T,P > V3;
   typedef glm::tvec4< T,P > V4;
   Quat< T > r; // primary part of dual-quaternion
   Quat< T > t; // dual part of dual-quaternion

   // Construction: Default.
   DualQuat()
      : r(1,0,0,0), t(0,0,0,0)
   {}

   // Construction: By 2 raw quaternions (p,q) = p + epsilon*q.
   DualQuat( Quat< T > const & p,
             Quat< T > const & q = Quat< T >() )
      : r( p ), t( q )
   {}

   // Construction: By 8 raw quaternion components ( a,b,c,d ) + epsilon*( e,f,g,h )
   DualQuat( T a, T b, T c, T d,
             T e = T(0), T f = T(0), T g = T(0), T h = T(0) )
      : r( a,b,c,d ), t( e,f,g,h )
   {}

   // Construction: By copying another quaternion 'o'.
   DualQuat( DualQuat const & o )
      : r( o.r ), t( o.t )
   {}

   // Construction: by a point ( encoded in secondary part )
   static DualQuat
   fromPoint( T x, T y, T z ) { return DualQuat< T >( 1,0,0,0, 0,x,y,z ); }

   // Construction: by a point ( encoded in secondary part )
   static DualQuat
   fromPoint( V3 const & pos ) { return fromPoint( pos.x, pos.y, pos.z ); }

   // Construction: by a translation-vector ( encoded as half in secondary part )
   // Since translation and rotation act twice on v' = qvq', we half each of them.
   static DualQuat
   fromTranslation( T x, T y, T z )
   {
      return DualQuat< T >( 1,0,0,0, 0,T(.5)*x,T(.5)*y,T(.5)*z );
   }

   // Construction: by a translation-vector ( encoded as half in secondary part )
   // Since translation and rotation act twice on v' = qvq', we half each of them.
   static DualQuat
   fromTranslation( V3 const & pos ) { return fromTranslation( pos.x, pos.y, pos.z ); }

   // Construction
   // Create a DualQuat from a rotation
   static DualQuat
   fromRotation( AngleAxis< T > const & in ) // yaw (Z), pitch (Y), roll (X)
   {
      DualQuat< T > s( Quat< T >::fromRotation( in ), Quat< T >(0,0,0,0 ) );
      return s;
   }

   // Construction from a rotation and a translation
   static void
   fromDirectionAndPosition( V3 const & dir, V3 const & pos )
   {
      return compose( AngleAxis< T >( T(180 ), dir ), pos );
   }

   // Construction from a rotation and a translation
   static void
   compose( AngleAxis< T > const & rot, V3 const & pos )
   {
      DualQuat< T > t = DualQuat< T >::fromTranslation( pos );
      DualQuat< T > r = DualQuat< T >::fromRotation( rot );
      return t * r;
   }

   // Construction from a rotation and a translation
   static void
   compose( T radians, V3 const & axis, V3 const & pos )
   {
      return compose( AngleAxis< T >( radians, axis ), pos );
   }

   void
   decompose( AngleAxis< T > & rot, V3 & pos ) const
   {
      Quat< T > p = r; // .toAngleAxis();
      Quat< T > q = t; //
      rot = p.toAngleAxis();
      pos = q.toPoint();
   }

   // Arithmetik:
   V3
   toPoint() const
   {
      if ( r.a != 1 )
      {
         DE_WARN("Translation cant be extracted directly, because rotation != 0°")
      }
      return t.toPoint(); // must be DualQuat(1,0,0,0,0,x,y,z)
   }

   // Copy operator
   DualQuat&
   operator=( DualQuat const & o ) {  r = o.r; t = o.t; return *this; }

   // Algebra: Equality operator
   bool
   operator==( DualQuat const & o ) const { return ( r == o.r ) && ( t == o.t ); }

   // Algebra: Inequality operator
   bool
   operator!=( DualQuat const & o ) const { return !(*this == o); }


//   // Build quaternion from 'axis and angle', angle in radians.
//   static Quaternion
//   fromAngleAxis( T angle, V3 axis )
//   {
//      axis = glm::normalize( axis );
//      T const h = angle * T( 0.5 );
//      T const s = dbSin( h );
//      T const c = dbCos( h );
//      Quaternion quat;
//      quat.a = c;
//      quat.b = axis.x * s;
//      quat.c = axis.y * s;
//      quat.d = axis.z * s;
//      return quat;
//   }

   // Arithmetik:
   T
   getLengthSquared() const { return r.getLengthSquared() + t.getLengthSquared(); }

   // Arithmetik:
   T
   getLength() const { return dbSqrt( getLengthSquared() ); }

   // Formatting:
   std::string
   toString() const {
      std::stringstream s;
      std::string p = r.toString();
      std::string q = t.toString();

      if ( p.empty() && q.empty() )
      {
         return "0";
      }
      else
      {
         if ( !p.empty() )
         {
            s << r.toString();
         }

         if ( !(p.empty() || q.empty()) )
         {
            s << " + ";
         }

         if ( !q.empty() )
         {
            s << "e(" << t.toString() << ")";
         }
      }

      return s.str();
   }

   // Algebra:
   /// @brief Conjugate q' or q* of a dual quaternion q = p + e*s
   // q' = r' - e*s' = (a,u)' - e*(b,v)' = (a,-u) + e*(-b,v)
   // (a,-b,-c,-d) - e(a,-b,-c,-d)
   DualQuat
   conj() const { return DualQuat( r.a, -r.b, -r.c, -r.d, -t.a, t.b, t.c, t.d ); }

   // Algebra: Skalare Addition: d + skalar
   DualQuat
   operator+( T const & s ) const { return DualQuat( r+s, t+s ); }

   // Algebra: Skalare Substraction: d - skalar
   DualQuat
   operator-( T const & s ) const { return DualQuat( r-s, t-s ); }

   // Algebra: Skalare Multiplikation: d * skalar
   DualQuat
   operator*( T const & s ) const { return DualQuat( r*s, t*s ); }

   // Algebra: Skalare Division: d / skalar
   DualQuat
   operator/( T const & s ) const { return DualQuat( r/s, t/s ); }

   // Algebra: Duale Addition: q1 + q2
   DualQuat
   operator+( DualQuat const & o ) const { return DualQuat( r+o.r, t+o.t ); }

   // Algebra: Duale Subtraction: q1 - q2 = q1 + (-q2)
   DualQuat
   operator-( DualQuat const & o ) const { return DualQuat( r-o.r, t-o.t ); }

   // Algebra: Duale Multiplikation: d1 * d2  =  r1 * r2  +  e*( r1*t2 + r2*t1 )
   DualQuat
   operator*( DualQuat const & o ) const
   {
      DualQuat const & a = *this;
      DualQuat const & b = o;
      //DE_DEBUG( "q1 = ", a )
      //DE_DEBUG( "q2 = ", b )
      //DE_DEBUG( "Mul(q1,q2) = ", (a*b) )
      //d1*d2 = r1*r2 + e(r1*t2 + r2*t1)
      return DualQuat( a.r*b.r, a.r*b.t + a.t*b.r );
   }
   //
   // Algebra: Duale Division: d1 / d2  =  ( a1 + b1*i + c1*j + d1*k ) / ( a2 + b2*i + c2*j + d2*k )
   //
   // Duale Division: d2.conj() = ( a2 - b2*i - c2*j - d2*k )
   //
   // Duale Division: d1*d2.conj() = ( a1 + b1*i + c1*j + d1*k ) * ( a2 - b2*i - c2*j - d2*k )
   // Duale Division: d1*d2.conj() = a1*a2 + b1*i + c1*j + d1*k ) * ( a2 - b2*i - c2*j - d2*k )

   // Duale Division: d2*d2.conj() = ( a2 + b2*i + c2*j + d2*k ) * ( a2 - b2*i - c2*j - d2*k )
   // Duale Division: d2*d2.conj() = a2*a2 + b2*b2 + c2c2 + d2d2

   // Duale Division: d1*d2.conj() =  ( r2 + e* t2 )*( r2.conj() - e* t2.conj())
   // Duale Division: d2*d2.conj() =  ( r2 + e* t2 )*( r2.conj() - e* t2.conj())

   // Duale Division: d1*d2.conj() =  ( r2 + e* t2 )*( r2.conj() - e* t2.conj())
   // Duale Division: d2*d2.conj() =  ( r2 + e* t2 )*( r2.conj() - e* t2.conj())
   //
   DualQuat
   operator/( DualQuat const & other ) const
   {
      return DualQuat( other ); // * inversed( r );
   }

   // Algebra: Transform another quat 'o' with this quat.
   DualQuat
   transform( DualQuat const & q ) const
   {
      DualQuat o = (q * *this) * q.conj();
//      DE_DEBUG( "q = ", q )
//      DE_DEBUG( "q'= ", q.conj() )
//      DE_DEBUG( "v = ", *this )
//      DE_DEBUG( "v'= q*v*q' = ", o )
      return o;
   }

   // Algebra: Inverse
   void
   inverse()
   {
//      conjugate();

//      T betrag_inv = T( 1 ) / getLength();

//      if ( !dbIsInf( betrag_inv ) )
//      {
//         DE_ERROR( "Invalid betrag_inv(", betrag_inv, ")" )
//      }
//      else
//      {
//         a *= betrag_inv;
//         b *= betrag_inv;
//         c *= betrag_inv;
//         d *= betrag_inv;
//      }
   }

//   static Quaternion
//   inversed( Quaternion const & o )
//   {
//      Quaternion q( o );
//      q.inverse();
//      return q;
//   }
//   VIP 2:
//   void
//   normalize()
//   {
//      T betrag_inv = T( 1 ) / getLength();

//      if ( dbIsInf( betrag_inv ) )
//      {
//         DE_ERROR( "Invalid betrag_inv(", betrag_inv, ")" )
//      }
//      else
//      {
//         a *= betrag_inv;
//         b *= betrag_inv;
//         c *= betrag_inv;
//         d *= betrag_inv;
//      }
//   }




//   V3
//   rotateVector( V3 vec ) const
//   {
//      DE_DEBUG( "vec = ", vec )
//      Quaternion param( T(0), vec.x, vec.y, vec.z );
//      Quaternion result = rotate( param );
//      V3 out( result.b, result.c, result.d );
//      DE_DEBUG( "out = ", out )
//      return out;
//   }


// Converters

//   // Negate vector part, skalar part stays const.
//   static Quaternion
//   fromEulerAngles( EulerAngles< T > const & euler ) // yaw (Z), pitch (Y), roll (X)
//   {
//      // Abbreviations for the various angular functions
//      T cy = dbCos( euler.yaw * 0.5 );
//      T sy = dbSin( euler.yaw * 0.5 );
//      T cp = dbCos( euler.pitch * 0.5 );
//      T sp = dbSin( euler.pitch * 0.5 );
//      T cr = dbCos( euler.roll * 0.5 );
//      T sr = dbSin( euler.roll * 0.5 );

//      Quaternion< T > q;
//      q.a = cr * cp * cy + sr * sp * sy;
//      q.b = sr * cp * cy - cr * sp * sy;
//      q.c = cr * sp * cy + sr * cp * sy;
//      q.d = cr * cp * sy - sr * sp * cy;
//      return q;
//   }

//   // Negate vector part, skalar part stays const.
//   EulerAngles< T > // yaw (Z), pitch (Y), roll (X)
//   toEulerAngles() const
//   {
//      EulerAngles< T > euler;

//      // roll (x-axis rotation)
//      T pcos_rsin = T(2) * (a * b + c * d);
//      T pcos_rcos = T(1) - ( T(2) * (b * b + c * c) );
//      euler.roll = dbAtan2( pcos_rsin, pcos_rcos );

//      // pitch (y-axis rotation)
//      T sinp = T(2) * (a * c - d * b);
//      if ( dbAbs( sinp ) >= T(1) )
//      {
//         euler.pitch = std::copysign( M_PI / 2, sinp); // use 90 degrees if out of range
//      }
//      else
//      {
//         euler.pitch = dbAsin( sinp );
//      }

//      // yaw (z-axis rotation)
//      T pcos_ysin = T(2) * (a * d + b * c);
//      T pcos_ycos = T(1) - ( T(2) * (c * c + d * d) );
//      euler.yaw = dbAtan2( pcos_ysin, pcos_ycos );

//      return euler;
//   }


//   AngleAxis< T >
//   toAngleAxis() const
//   {
//      AngleAxis< T > out;
//      T t = T( 1 ) /  dbSqrt( T( 1 ) - ( a * a ) );

//      if ( dbIsInf( t ) )
//      {
//         DE_ERROR( "t is inf, ", toString())
//      }
//      else
//      {
//         out.angle = T( 2 ) * dbAcos( a ); // Angle
//         out.axis.x = b * t;  // Rotation axis x
//         out.axis.y = c * t;  // Rotation axis y
//         out.axis.z = d * t;  // Rotation axis z
//      }
//      return out;
//   }

//   // Build quaternion from 'axis and angle', angle in radians.
//   static Quaternion
//   fromAngleAxis( T angle, V3 axis )
//   {
//      axis = glm::normalize( axis );
//      T const h = angle * T( 0.5 );
//      T const s = dbSin( h );
//      T const c = dbCos( h );
//      Quaternion quat;
//      quat.a = c;
//      quat.b = axis.x * s;
//      quat.c = axis.y * s;
//      quat.d = axis.z * s;
//      return quat;
//   }

//   // Build quaternion from 'axis and angle', angle in radians.
//   static Quaternion
//   fromAngleAxis( AngleAxis< T > in )
//   {
//      return fromAngleAxis( in.angle, in.axis );
//   }

//   // Build quaternion from 'axis and angle', angle in radians.
//   static Quaternion
//   fromAngleAxis( T angle, T x, T y, T z )
//   {
//      return fromAngleAxis( angle, V3(x,y,z) );
//   }

//   /// @brief Create Rotation Matrix 4x4 benni|row-major.
//   /// skalar a = w;
//   /// vector b = x; c = y; d = z;
//   ///
//   //( a^2 + b^2 - c^2 - d^2     2(bc - ad)               2(bd + ac)      )
//   //(      2(bc + ad)       a^2 - b^2 + c^2 - d^2        2(cd + ab)      )
//   //(      2(bd - ac)           2(cd + ab)         a^2 - b^2 - c^2 + d^2 )
//   M3
//   toMat3() const
//   {
//      M3 out( T(1) );
//      T * m = glm::value_ptr( out );
//      m[0] = a*a + b*b - c*c - d*d;    // M11
//      m[1] = T( 2 ) * ( b*c - a*d);    // M12
//      m[2] = T( 2 ) * ( b*d + a*c);    // M13
//      m[3] = T( 2 ) * ( b*c + a*d);    // M21
//      m[4] = a*a - b*b + c*c - d*d;    // M22
//      m[5] = T( 2 ) * ( c*d - a*b);    // M23
//      m[6] = T( 2 ) * ( b*d - a*c);    // M31
//      m[7] = T( 2 ) * ( c*d + a*b);    // M32
//      m[8] = a*a - b*b - c*c + d*d;    // M33
//      return m; // benni row-major
//   }

//   M4
//   toMat4() const
//   {
//      M4 out( T(1) );
//      T * m = glm::value_ptr( out );
//      m[0] = a*a + b*b - c*c - d*d;    // M11
//      m[1] = T( 2 ) * ( b*c - a*d);    // M12
//      m[2] = T( 2 ) * ( b*d + a*c);    // M13
//      m[4] = T( 2 ) * ( b*c + a*d);    // M21
//      m[5] = a*a - b*b + c*c - d*d;    // M22
//      m[6] = T( 2 ) * ( c*d - a*b);    // M23
//      m[8] = T( 2 ) * ( b*d - a*c);    // M31
//      m[9] = T( 2 ) * ( c*d + a*b);    // M32
//      m[10] = a*a - b*b - c*c + d*d;    // M33
//      return m; // benni row-major
//   }

   /// @brief Create Quaternion by extracting Axis and Angle from Matrix3x3
   ///
   // [( m11 m12 m13 )            ( 1 0 0 )]   ( x )   ( 0 )
   // [( m21 m22 m23 ) - lambda * ( 0 1 0 )] * ( y ) = ( 0 )
   // [( m31 m32 m33 )            ( 0 0 1 )]   ( z )   ( 0 )
   //
   // But we know that one Eigenwert (lambda) is always 1 for a rotation matrix
   //
   // [( m11 m12 m13 )   ( 1 0 0 )]   ( x )   ( 0 )
   // [( m21 m22 m23 ) - ( 0 1 0 )] * ( y ) = ( 0 )
   // [( m31 m32 m33 )   ( 0 0 1 )]   ( z )   ( 0 )
   //
   // Ax = b = 0
   // ( m11-1 m12   m13   )   ( x )   ( 0 )
   // ( m21   m22-1 m23   ) * ( y ) = ( 0 )
   // ( m31   m32   m33-1 )   ( z )   ( 0 )

   // -> build inverse A'
   // use inverse
   //
   // (AA')x = (A')b
   // (I)x = A'b
   // x = A'b
/*
   Quaternion
   fromMat3( M3 const & m33 ) const
   {
      // M = ( R t )
      //     ( 0 1 )
      // Angle =

      T const * const m = glm::value_ptr( m33 );

      // Spur( m33 ) = sum(mii) = 1 + 2*cos( ges_angle );
      // Axis = Eigenwert of m33 is the axis vector
      // -> aka. the vector that stays const after a rotation.
      // ( m33 - lambda*I33 )*x = 0
      m[0] = a*a + b*b - c*c - d*d;    // M11
      m[1] = T( 2 ) * ( b*c - a*d);    // M12
      m[2] = T( 2 ) * ( b*d + a*c);    // M13
      m[3] = T( 2 ) * ( b*c + a*d);    // M21
      m[4] = a*a - b*b + c*c - d*d;    // M22
      m[5] = T( 2 ) * ( c*d - a*b);    // M23
      m[6] = T( 2 ) * ( b*d - a*c);    // M31
      m[7] = T( 2 ) * ( c*d + a*b);    // M32
      m[8] = a*a - b*b - c*c + d*d;    // M33
      return m; // benni row-major
   }
*/


};

typedef DualQuat< float > DualQuatf;
typedef DualQuat< double > DualQuatd;



// =======================================================================
// DualQuatTest
// =======================================================================
struct DualQuatTest
{
   DE_CREATE_LOGGER("de.DualQuatTest")

   static void
   test()
   {
      DE_INFO("==============================================")
      DE_INFO("[1.] TEST ======== ( KIT EXAMPLE 4.5 ) =======")
      DE_INFO("==============================================")
      DE_INFO("")
      DE_INFO("Gegeben: Ein Punkt P = (3,4,5)")
      DE_INFO("         Eine Translation T = (-4,8,-8)")
      DE_INFO("Ergebnis: Ein Punkt Q = (-1,12,-3)")
      DE_INFO("")
      {
         DualQuatd P = DualQuatd::fromPoint( 3,4,5 );
         DualQuatd T = DualQuatd::fromTranslation( -4., 8., -8. );
         DualQuatd Q = P.transform( T );
         DualQuatd::V3 P1 = Q.toPoint();
//         DE_INFO("Expected P = " )
//         DE_INFO("Expected R = i" )
//         DE_INFO("Expected T = 1+e(2i+j+3k)" )
//         DE_INFO("")
         DE_INFO("")
         DE_INFO("Computed P = ", P )
         DE_INFO("Computed T = ", T )
         DE_INFO("Computed Q = ", Q )
         DE_INFO("Computed P1 = ", P1 )

         if ( P != DualQuatd( 1,0,0,0,0,3,4,5) )
         {
            throw std::runtime_error("DualQuatTest (1) failed. cond: P != DualQuatd( 1,0,0,0,0,3,4,5)");
         }

         if ( T != DualQuatd( 1,0,0,0,0,-2,4,-4 ) )
         {
            throw std::runtime_error("DualQuatTest (1) failed. cond: T != DualQuatd( 1,0,0,0,0,-2,4,-4 )");
         }

         if ( Q != DualQuatd( 1,0,0,0,0,-1,12,-3 ) )
         {
            throw std::runtime_error("DualQuatTest (1) failed. cond: Q != DualQuatd( 1,0,0,0,0,-1,12,-3 )");
         }

         DE_INFO("OK (0)" )
      }

      DE_INFO("==============================================")
      DE_INFO("[2.] TEST ======== ( KIT EXAMPLE 4.5 ) =======")
      DE_INFO("==============================================")
      DE_INFO("")
      DE_INFO("Gegeben: Ein Punkt p = (3,4,5)" )
      DE_INFO("Gesucht: Rotation um Achse a = (1,0,0) mit phi = 180°" )
      DE_INFO("         dann Translation um t = (4,2,6)" )
      DE_INFO("")
      {
         DualQuatd P = DualQuatd::fromPoint( 3,4,5 );
         DualQuatd R = DualQuatd::fromRotation( AngleAxisd( dbRAD(180.), 1,0,0 ) );
         DualQuatd T = DualQuatd::fromTranslation( 4., 2., 6. );
         DualQuatd K = T * R;
         DualQuatd K_conj = K.conj();
         DualQuatd Q = P.transform( K );
         DualQuatd::V3 P1 = Q.toPoint();
         DE_INFO("")
         DE_INFO("Expected P = 1+e(3i+4j+5k)" )
         DE_INFO("Expected R = i" )
         DE_INFO("Expected T = 1+e(2i+j+3k)" )
         DE_INFO("")
         DE_INFO("Computed P = ", P )
         DE_INFO("Computed R = ", R )
         DE_INFO("Computed T = ", T )

         if ( P != DualQuatd( 1,0,0,0,0,3,4,5) )
         {
            throw std::runtime_error("DualQuatTest (1) failed. cond: P != DualQuatd( 1,0,0,0,0,3,4,5)");
         }

         if ( T != DualQuatd( 1,0,0,0,0,2,1,3 ) )
         {
            throw std::runtime_error("DualQuatTest (1) failed. cond: T != DualQuatd( 1,0,0,0,0,2,1,3 )");
         }

         if ( R != DualQuatd( dbCos( dbRAD(90.) ), dbSin( dbRAD(90.) ),0,0,0,0,0,0 ) )
         {
            throw std::runtime_error("DualQuatTest (1) failed. cond: R != DualQuatd( cos(90°), sin(90°),0,0,0,0,0,0 )");
         }

         DE_INFO("")
         DE_INFO("Expected K = i + e(-2 + 3j - k)" )
         DE_INFO("Expected K'= -i + e(2 + 3j - k)" )
         DE_INFO("Expected P'= (7,-2,1)" )
         DE_INFO("")
         DE_INFO("Computed K = ", K )
         DE_INFO("Computed K'= ", K_conj )
         DE_INFO("Computed Q = ", Q )
         DE_INFO("Computed P'= (", P1, ")" )

         if ( !dbEquals( P1, Quatd::V3(7,-2,1) ) )
         {
            throw std::runtime_error("QuaterionTest (1) failed.  P1 != (7,-2,1) ");
         }

         DE_INFO("OK (0)" )
//         DE_INFO("")
//         DE_INFO("Computed K2 = ", K2 )
//         DE_INFO("Computed K2'= ", K2_conj )
//         DE_INFO("Computed Q2 = ", Q2 )
//         DE_INFO("Computed P2 = (", P2, ")" )
      }

   }
};

} // end namespace gpu.
} // end namespace de.


template < typename T >
std::ostream& operator<< ( std::ostream& o, de::gpu::DualQuat< T > const & quat )
{
   o << quat.toString();
   return o;
}


#endif

