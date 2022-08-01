/// ==========================================================================
/// @file GLES.hpp
/// @brief DarkEngine header offering OpenGL ES 3.x Benni extensions
/// @author 2018 Benjamin Hampe <benjaminhampe@gmx.de>
/// @copyright Free open source software
/// to encapsulate much overheader.
/// ==========================================================================

#ifndef BENNI_MATH3D_HPP
#define BENNI_MATH3D_HPP

#include <de/gpu/CompileConfigDarkGPU.hpp>

namespace de {
namespace gpu {

// ============================================================================
template <class T>
struct TSinTable
// ============================================================================
{
public:
   explicit TSinTable(  uint32_t count = 36,
                        T radian_angle_start = T( 0.0 ),
                        T radian_angle_end = T( 2.0*M_PI ) )
   {
      count = (count>2)?count:2;
      m_table.resize( count );
      m_angle_start = radian_angle_start;
      m_angle_end = radian_angle_end;
      m_angle_step = (m_angle_end - m_angle_start) / T( count-1 );
      if ( sizeof(T) < 8 )
      {
         for ( size_t i = 0; i < m_table.size(); ++i )
         {
            m_table[i] = sinf( m_angle_start + m_angle_step * i );
         }
      }
      else
      {
         for ( size_t i = 0; i < m_table.size(); ++i )
         {
            m_table[i] = sin( m_angle_start + m_angle_step * i );
         }
      }
   }

   T& operator[] ( uint32_t index ) { return m_table[ index % m_table.size() ]; }
   T const & operator[] ( uint32_t index ) const { return m_table[ index % m_table.size() ]; }
   T const & getAngleStepRad() const { return m_angle_step; }
   T const & getAngleStartRad() const { return m_angle_start; }
   T const & getAngleEndRad() const { return m_angle_end; }
private:
   std::vector< T > m_table;
   T m_angle_step;
   T m_angle_start;
   T m_angle_end;

}; // end class TSinTable



// ============================================================================
template <class T>
struct TCosTable
// ============================================================================
{
   explicit TCosTable(  uint32_t count,
                        T radian_angle_start = T(0),
                        T radian_angle_end = T( 2.0*M_PI ) )
   {
      count = (count>2)?count:2;
      m_table.resize( count );
      m_angle_start = radian_angle_start;
      m_angle_end = radian_angle_end;
      m_angle_step = (m_angle_end - m_angle_start) / T(count-1);

      if ( sizeof(T) < 8 )
      {
         for ( size_t i = 0; i < m_table.size(); ++i )
         {
            m_table[i] = cosf( m_angle_start + m_angle_step * i );
         }
      }
      else
      {
         for ( size_t i = 0; i < m_table.size(); ++i )
         {
            m_table[i] = cos( m_angle_start + m_angle_step * i );
         }
      }
   }

   T& operator[] ( uint32_t index ) { return m_table[ index % m_table.size() ]; }
   T const & operator[] ( uint32_t index ) const { return m_table[ index % m_table.size() ]; }
   T const & getAngleStepRad() const { return m_angle_step; }
   T const & getAngleStartRad() const { return m_angle_start; }
   T const & getAngleEndRad() const { return m_angle_end; }

private:
   std::vector< T > m_table;
   T m_angle_step;
   T m_angle_start;
   T m_angle_end;

}; // end class TCosTable


// ============================================================================
template <class T>
struct TSinCosTable // Full circle table (360°)
// ============================================================================
{
   struct SPair { T s, c; };

   explicit TSinCosTable(  uint32_t count,
                           T rad_start = T( 0.0 ),
                           T rad_end = T( 2.0*M_PI ) )
   {
      count = (count>2)?count:2;
      m_table.resize( count + 1 );
      m_start = rad_start;
      m_end = rad_end;
      m_step = (m_end - m_start) / T(count);

      T angle = m_start;
      if (sizeof(T) < 8)
      {
         for ( size_t i = 0; i < m_table.size(); ++i )
         {
            m_table[ i ].s = sinf( angle );
            m_table[ i ].c = cosf( angle );
            angle += m_step;
         }
      }
      else
      {
         for ( size_t i = 0; i < m_table.size(); ++i )
         {
            m_table[ i ].s = sin( angle );
            m_table[ i ].c = cos( angle );
            angle += m_step;
         }
      }
   }

   uint32_t getTableSize() const { return m_table.size(); }
   SPair& operator[] ( uint32_t index ) { return m_table[ index % m_table.size() ]; }
   SPair const & operator[] ( uint32_t index ) const { return m_table[ index % m_table.size() ]; }
   T const & getAngleStepRad() const { return m_step; }
   T const & getAngleStartRad() const { return m_start; }
   T const & getAngleEndRad() const { return m_end; }

private:
   std::vector< SPair > m_table;
   T m_step;
   T m_start;
   T m_end;

}; // end class TSinCosTable



//=============================================================================
/// @brief Turn2
//=============================================================================
template< typename T, glm::precision P = glm::defaultp >
struct Turn2
{
   typedef glm::tvec2< T,P > V2;
   typedef glm::tvec3< T,P > V3;

   enum ETurn
   {
      NoTurn = 0,
      // Turn180,
      TurnLeft,
      TurnRight,
      ConvexTurn = TurnLeft // ( if winding is 'ccw' )
   };

   /// @brief 2D cross product.
   /// @param a Vektor A.
   /// @param b Vektor B.
   /// @return 2D cross product AxB.
   static T
   cross2D( V2 const & a, V2 const & b )
   {
      return a.x * b.y - b.x * a.y;
   }

   /// @brief Calculate sign (left|right) of turn ABC. ( Relation of point C to line AB )
   /// @param a Point A.
   /// @param b Point B.
   /// @param c Point C.
   /// @return < 0 : Relation of point 'C to line 'AB' is 'Left turn'.
   ///         = 0 : Relation of point 'C to line 'AB' is 'Lies on AB'.
   ///         > 0 : Relation of point 'C to line 'AB' is 'Right turn'.
   static T
   computeSign( V2 const & a, V2 const & b, V2 const & c )
   {
      // improve numerical stability
      if( glm::length2( b - c ) > glm::length2( c - a ) )
      {
         return cross2D( c - a, b - a );
      }
      else
      {
         return cross2D( b - c, a - b );
      }
   }

   /// @brief Compute turn enum of turn ABC.
   /// @param a Point A.
   /// @param b Point B.
   /// @param c Point C.
   /// @return Turn of a, b, c.
   static ETurn
   computeTurn( V2 const & a, V2 const & b, V2 const & c )
   {
      auto const signum = computeSign( a, b, c );
      if ( signum < T( 0 ) )
      {
         return TurnLeft;
      }
      else if ( signum > T( 0 ) )
      {
         return TurnRight;
      }
      else // if ( signum == T( 0 ) )
      {
         return NoTurn; // numerically unlikely, but we like to have it.
      }
   }

   /// @brief Are 3 points ABC forming a LeftTurn?
   /// @param a Point A.
   /// @param b Point B.
   /// @param c Point C.
   /// @return True if turn ABC forms a LeftTurn.
   static bool
   isLeftTurn( V2 const & a, V2 const & b, V2 const & c )
   {
      return computeTurn( a,b,c ) == TurnLeft;
   }

   /// @brief Are 3 points ABC forming a RightTurn?
   /// @param a Point A.
   /// @param b Point B.
   /// @param c Point C.
   /// @return True if turn ABC forms a RightTurn.
   static bool
   isRightTurn( V2 const & a, V2 const & b, V2 const & c )
   {
      return computeTurn( a,b,c ) == TurnRight;
   }

   /// @brief Is given point 'p' inside given triangle 'ABC'.
   /// @param p The test Point P.
   /// @param a Triangle Point A.
   /// @param b Triangle Point B.
   /// @param c Triangle Point C.
   /// @return True, if 'p' is inside triangle. ( TODO check also border )
   static bool
   insideTriangle2D( V2 const & p, V2 const & a, V2 const & b, V2 const & c )
   {
      return ( computeTurn( a,b,p ) == ConvexTurn )
          && ( computeTurn( b,c,p ) == ConvexTurn )
          && ( computeTurn( c,a,p ) == ConvexTurn );
   }

};



//=============================================================================
template < typename T, glm::precision P = glm::defaultp >
struct Line3
//=============================================================================
{
   constexpr static T const s_Invalid = std::numeric_limits<T>::infinity();

   typedef glm::tvec3< T, P > V3;

   Line3()
      : m_A( 0,0,0 )
      , m_B( 0,0,0 )
   {}

   Line3( V3 const & a, V3 const & b )
      : m_A( a )
      , m_B( b )
   {}

   /// @brief Get vector AB (difference of 2 points)
   V3 delta() const { return m_B - m_A; }

   /// @brief Get normalized direction vector d = AB/|AB|
   V3 dir() const { return glm::normalize( m_B - m_A ); }

   /// @brief Get squared length (fast) of line AB ( use to compare distances fast )
   T len2() const { V3 d = m_B - m_A; return d.x*d.x + d.y*d.y + d.z*d.z; }

   /// @brief Get length (slow) of line AB
   T len() const { return dbSqrt( len() ); }

   /// @brief Get length (slow) of line AB
   //T distToPoint() const { return dbSqrt( len() ); }

public:
   V3 m_A;
   V3 m_B;
};


//=============================================================================
template < typename T, glm::precision P = glm::defaultp >
struct Ray3
//=============================================================================
{
   constexpr static T const NoInit = std::numeric_limits<T>::infinity();
   typedef glm::tmat4x4< T, P > M4;
   typedef glm::tvec3< T, P > V3;
   typedef glm::tvec4< T, P > V4;

   V3 m_Pos;
   V3 m_Dir;

   Ray3() : m_Pos( 0, 0, 0 ), m_Dir( 0, 0, 1 ) {}
   Ray3( V3 const & pos, V3 const & dir ) : m_Pos( pos ), m_Dir( dir ) {}
   void setPos( V3 const & pos ) { m_Pos = pos; }
   void setDir( V3 const & dir ) { m_Dir = dir; }
   V3 const & getPos() const { return m_Pos; }
   V3 const & getDir() const { return m_Dir; }


   template < typename B >
   Ray3< B >
   convert() const
   {
      return Ray3< B >( m_Pos, m_Dir );
   }

   std::string
   toString() const
   {
      std::stringstream s; s << "pos:" << m_Pos << ", dir:" << m_Dir;
      return s.str();
   }

   bool
   isValid() const
   {
      if ( std::isinf( m_Pos.x ) || std::isnan( m_Pos.x ) ) return false;
      if ( std::isinf( m_Pos.y ) || std::isnan( m_Pos.y ) ) return false;
      if ( std::isinf( m_Pos.z ) || std::isnan( m_Pos.z ) ) return false;
      if ( std::isinf( m_Dir.x ) || std::isnan( m_Dir.x ) ) return false;
      if ( std::isinf( m_Dir.y ) || std::isnan( m_Dir.y ) ) return false;
      if ( std::isinf( m_Dir.z ) || std::isnan( m_Dir.z ) ) return false;
      return true;
   }

   void reset()
   {
      // We always have bird/top view from north direction as default view.
      m_Pos = V3( 0, 0, 0 ); // origin, earth-plane is image-plane (x,y), z is height above x,y ground.
      m_Dir = V3( 0, 0, -1 ); // -z, looks from heaven (+z) to ground (0 and below)
   }

   struct HitResult
   {
      std::vector< V3 > hitPositions;
      bool empty() const { return hitPositions.empty(); }
   };


   /// @author Benjamin Hampe <benjaminhampe@gmx.de>
   /// @brief Picking something at mousePos(mx,my) on screen into 3D World space
   // Most efficient:
   // ViewProjectionMatrix is already given as inverse
   /// @param mx The mouse pos x on screen
   /// @param my The mouse pos y on screen
   /// @param w The screen width ( of this camera )
   /// @param h The screen height ( of this camera )
   /// @param Inverse camera ViewProjectionMatrix (VP_inv).
   static Ray3
   compute3DWorldRayFrom2DScreen( int mx, int my, int w, int h, M4 const & viewProjMatInv )
   {
      // MousePos to NDC
      T x = T( mx ) / T( w ) * 2.0 - 1.0;
      T y = T( h - 1 - my ) / T( h ) * 2.0 - 1.0; // flips y for OpenGL ES
      // Make a ray that goes through entire frustum from z=near to far.
      V4 ndcA( x, y, -1.0, 1.0 ); // z=-1 means ray starts at near plane
      V4 ndcB( x, y,  1.0, 1.0 ); // z=1 means ray ends at far plane
      // NDC to World, perspective correction done by (div by w)
      V4 tmpA = viewProjMatInv * ndcA;
      V4 tmpB = viewProjMatInv * ndcB;
      // After the perspective division xyz/w, worldspace pos is correct.
      V3 posA = V3( tmpA ) / tmpA.w;
      V3 posB = V3( tmpB ) / tmpB.w;
      return Ray3( posA, glm::normalize( posB - posA ) );
   }

};


//=============================================================================
template < typename T, glm::precision P = glm::defaultp >
struct Plane3
//=============================================================================
{
   DE_CREATE_LOGGER("de.gpu.Plane3")

   typedef glm::tvec3< T, P > V3;
   typedef glm::tvec4< T, P > V4;
   typedef glm::tmat4x4< T, P > M4;

   V3 n; // x=nx,y=ny,z=nz,w=distance to origin
   T dist;

   Plane3( ) : n( 0,0,1 ), dist( 0 ) {}
   Plane3( T x, T y, T z, T distToOrigin = T( 0.0 ) ) : n( x,y,z ), dist( distToOrigin ) {}
   Plane3( V3 const & nrm = V3(0,0,1), T distToOrigin = T( 0.0 ) ) : n( nrm), dist( distToOrigin ) {}

   V3 getNormal() const { return n; }

   // double p =  glm::dot( rayDir, n );
   // Ebene: (x-A)*nABC = 0
   // Ebene: A = 0 + dist * n;
   // Ray: rayStart + r * rayDir
   // R in E:

   //(dist * n - rayStart - r * rayDir)o n = 0;
   //((dist * n) o n - rayStart o n - r * rayDir o n = 0;
   //((dist * n) o n - rayStart o n = r * rayDir o n;
   // r = (dist * (n,n) - rayStart*n) / (rayDir*n);
   bool
   intersectRay( V3 rayStart, V3 rayDir, V3 & hitPosition, T* out_r = nullptr ) const
   {
      T r = ( (glm::dot(n,n) * dist) - glm::dot( rayStart, n ) ) / glm::dot( rayDir, n );
      if ( std::isinf( r ) ) { return false; }
      if ( std::isnan( r ) ) { return false; }
      if ( out_r ) { *out_r = r; }
      hitPosition = rayStart + rayDir * r;
      return true;
   }

   std::string
   toString() const
   {
      std::stringstream s;
      s << n << "," << dist;
      return s.str();
   }
};


//=============================================================================
/// @brief Triangle3
//=============================================================================
template < typename T, glm::precision P = glm::defaultp >
struct Triangle3
{
   typedef glm::tvec3< T,P > V3;
   typedef glm::tvec4< T,P > V4;
   typedef glm::tmat4x4< T,P > M4;
   DE_CREATE_LOGGER("de.math.Triangle3")
   V3 A,B,C;

   Triangle3() : A( 0, 0, 0 ), B( 0, 1, 0 ), C( 1, 1, 0 )
   {}
   Triangle3( V3 a, V3 b, V3 c ) : A( a ), B( b ), C( c )
   {}
   std::string
   toString() const
   {
      std::stringstream s;
      s << "A(" << A << "), B(" << B << "), C(" << C << ")";
      return s.str();
   }

   // nABC = (AB x AC) / |AB x AC|.
   V3
   computeNormal() const
   {
      return glm::normalize( glm::cross( B - A, C - B ) );
   }

   T
   computeDistToOrigin() const
   {
      T d = std::min( std::min( glm::length2( A ), glm::length2( B ) ), glm::length2( C ) );
      return dbSqrt( d );
   }

   Plane3< T >
   computePlane() const
   {
      V3 n = computeNormal();
      T d = computeDistToOrigin();
      return Plane3< T >( n, d );
   }

   Triangle3 &
   transform( M4 const & m )
   {
      A = V3( m * V4( A, T(1) ) );
      B = V3( m * V4( B, T(1) ) );
      C = V3( m * V4( C, T(1) ) );
      return *this;
   }

   // Benni - Mueller - Trumbore algorithm
   bool
   intersectRay( V3 rayStart, V3 rayDir, V3 & hitPosition ) const
   {
      T const EPSILON = T( 0.0000001 );
      V3 AB = B - A;
      V3 AC = C - A;
      // alias R = rayStart, D = rayDir
      V3 nDAC = glm::cross( rayDir, AC );
      // 'a' is projection of 'AB' on 'nDAC'
      T a = glm::dot( AB, nDAC );
      // if 'a' == 0, then ray is parallel to this triangle.
      if ( std::abs( a ) < EPSILON )
      {
//         DE_TRACE("ray is parallel, a = ", a, ", "
//                  "rayStart(", rayStart, "), rayDir(", rayDir, "), "
//                  "A =", A, ", B =", B,", C =", C )
         return false;
      }

      T f = T( 1.0 ) / a;

      if ( std::isinf( f ) )
      {
//         DE_DEBUG("f is inf, a = ", a, ", "
//                  "rayStart(", rayStart, "), rayDir(", rayDir, "), "
//                  "A =", A, ", B =", B,", C =", C )
         return false;
      }

      if ( std::isnan( f ) )
      {
//         DE_DEBUG("f is nan, a = ", a, ", "
//                  "rayStart(", rayStart, "), rayDir(", rayDir, "), "
//                  "A =", A, ", B =", B,", C =", C )
         return false;
      }

      V3 AR = rayStart - A;
      T u = f * glm::dot( AR, nDAC );
      if ( u < 0.0 || u > 1.0 )
      {
//         DE_DEBUG("u = ", u, ", a = ", a, ", "
//                  "rayStart(", rayStart, "), rayDir(", rayDir, "), "
//                  "A =", A, ", B =", B,", C =", C )
         return false;
      }

      V3 nARxAB = glm::cross( AR, AB );
      T v = f * glm::dot( rayDir, nARxAB );
      if ( v < 0.0 || u + v > 1.0 )
      {
//         DE_DEBUG("u = ", u, ", v = ", v, ", a = ", a, ", "
//                  "rayStart(", rayStart, "), rayDir(", rayDir, "), "
//                  "A =", A, ", B =", B,", C =", C )
         return false;
      }

      // At this stage we can compute t to find out where the intersection point is on the line.
      T t = f * glm::dot( AC, nARxAB );
      if ( t > EPSILON ) // ray intersection
      {
         hitPosition = rayStart + rayDir * t;
//         DE_DEBUG("Got intersection, hitPos = ", hitPosition, ", "
//                  "rayStart(", rayStart, "), rayDir(", rayDir, "), "
//                  "A =", A, ", B =", B,", C =", C )
         return true;
      }
      else
      {
         return false;
      }
   }
};



//=============================================================================
/// @brief Triangle3
//=============================================================================
template < typename T, glm::precision P = glm::defaultp >
struct TriangleList
{
   static void
   transform( std::vector< Triangle3< T > > & out,
              glm::tmat4x4< T,P > const & m )
   {
      for ( size_t i = 0; i < out.size(); ++i )
      {
         out[ i ].transform( m );
      }
   }

   static void
   append( std::vector< Triangle3< T > > & out,
           std::vector< Triangle3< T > > const & other )
   {
      out.insert( out.end(), other.begin(), other.end() );
   }

};


//=============================================================================
/// @brief BoundingBox3
//=============================================================================
//
//        F-------G (Max)           tex atlas infos         +----+----+---------+
//       /|      /|                                         |    |    |         |
//      / |  +Y / |                                         |    | +Z |         |
//     /  |    /  |                                         |    |    |         |
//    /   E---/---H   lookat = -X for plane PX              +----+----+----+----+
//   /-X /   / X /    <-------P  EyePos is +X for plane PX  |    |    |    |    |
//  /   /   / + /                                           | -X | -Y | +X | +Y |
// B-------C   /     +z                                     |    |    |    |    |
// |  /    |  /      |  +y                                  +----+----+----+----+
// | / -Y  | /       | /                                    |    |    |         |
// |/      |/        |/                                     |    | -Z |         |
// A-------D         0------> +x                            |    |    |         |
// Min:
template < typename T, glm::precision P = glm::defaultp >
struct Box3
{
   DE_CREATE_LOGGER("de.gpu.Box3")

   typedef glm::tvec3< T,P > V3;
   typedef glm::tvec4< T,P > V4;
   typedef glm::tmat4x4< T,P > M4;

   // Same as ECubeFace
   enum EPlane { NEG_X = 0, NEG_Y, NEG_Z, POS_X, POS_Y, POS_Z, EPlaneCount };

   V3 m_Min;
   V3 m_Max;

   Box3()
      : m_Min( T(0), T(0), T(0) )
      , m_Max( T(0), T(0), T(0) )
   {}

   Box3( T minX, T minY, T minZ, T maxX, T maxY, T maxZ )
      : m_Min( minX, minY, minZ )
      , m_Max( maxX, maxY, maxZ )
   {}

   Box3( V3 const & minEdge, V3 const & maxEdge )
      : m_Min( minEdge )
      , m_Max( maxEdge )
   {}

   V3 getA() const { return { m_Min.x, m_Min.y, m_Min.z }; }
   V3 getB() const { return { m_Min.x, m_Max.y, m_Min.z }; }
   V3 getC() const { return { m_Max.x, m_Max.y, m_Min.z }; }
   V3 getD() const { return { m_Max.x, m_Min.y, m_Min.z }; }
   V3 getE() const { return { m_Min.x, m_Min.y, m_Max.z }; }
   V3 getF() const { return { m_Min.x, m_Max.y, m_Max.z }; }
   V3 getG() const { return { m_Max.x, m_Max.y, m_Max.z }; }
   V3 getH() const { return { m_Max.x, m_Min.y, m_Max.z }; }


   Triangle3< T >
   getTriangle( int face, bool b ) const
   {
      if ( b )
      {
         switch( face )
         {
            case NEG_Z: return { getC(), getD(), getA() }; // CDA back 2
            case POS_Z: return { getF(), getE(), getH() }; // FEH front
            case NEG_X: return { getB(), getE(), getF() }; // EFBA left
            case POS_X: return { getG(), getH(), getD() }; // DCGH right
            case NEG_Y: return { getE(), getA(), getD() }; // DHEA bottom
            case POS_Y: return { getG(), getC(), getB() }; // BFGC top
            default: return { V3(0,0,0), V3(0,0,0), V3(0,0,0) };
         }
      }
      else
      {
         switch( face )
         {
            case NEG_Z: return { getA(), getB(), getC() }; // ABC back 1
            case POS_Z: return { getH(), getG(), getF() }; // HGF front 1
            case NEG_X: return { getE(), getF(), getB() }; // EFBA left
            case POS_X: return { getD(), getC(), getG() }; // DCGH right
            case NEG_Y: return { getD(), getH(), getE() }; // DHEA bottom
            case POS_Y: return { getB(), getF(), getG() }; // BFGC top
            default: return { V3(0,0,0), V3(0,0,0), V3(0,0,0) };
         }
      }
   }

   std::string
   toString( bool withCorners = false ) const
   {
      std::stringstream s;
      s << "size:" << getSize() << ", ";
      s << "min:" << m_Min << ", ";
      s << "max:" << m_Max;
      return s.str();
   }

   V3 const & getMin() const { return m_Min; }
   V3 const & getMax() const { return m_Max; }
   V3 & getMin() { return m_Min; }
   V3 & getMax() { return m_Max; }
   void setMin( V3 const & min ) { m_Min = min; }
   void setMin( T const & x, T const & y, T const & z ) { m_Min = V3(x,y,z); }
   void setMax( V3 const & max ) { m_Max = max; }
   void setMax( T const & x, T const & y, T const & z ) { m_Max = V3(x,y,z); }
   V3 getSize() const
   {
      //if ( !isValid() ) return V3();
      return m_Max - m_Min;
   }
   bool isValid() const
   {
      if ( std::isinf( m_Min.x ) || std::isnan( m_Min.x ) ) return false;
      if ( std::isinf( m_Min.y ) || std::isnan( m_Min.y ) ) return false;
      if ( std::isinf( m_Min.z ) || std::isnan( m_Min.z ) ) return false;
      if ( std::isinf( m_Max.x ) || std::isnan( m_Max.x ) ) return false;
      if ( std::isinf( m_Max.y ) || std::isnan( m_Max.y ) ) return false;
      if ( std::isinf( m_Max.z ) || std::isnan( m_Max.z ) ) return false;
      return true;
   }
   V3 getCenter() const
   {
      if ( !isValid() ) return V3();
      return m_Min + ( getSize() * 0.5f );
   }
   Box3 operator+( V3 const & value ) const
   {
      Box3 box( *this );
      box.m_Min += value;
      box.m_Max += value;
      return box;
   }
   Box3 operator-( V3 const & value ) const
   {
      Box3 box( *this );
      box.m_Min -= value;
      box.m_Max -= value;
      return box;
   }
   void reset() { m_Min = m_Max = V3( 0, 0, 0 ); }
   void reset( V3 const & pos ) { m_Min = m_Max = pos; }
   void reset( Box3 const & box ) { m_Min = box.m_Min; m_Max = box.m_Max; }
   void reset( T const & x, T const & y, T const & z ) { m_Min = m_Max = V3( x, y, z ); }

   void addInternalPoint( V3 const & p )
   {
      m_Min.x = std::min( m_Min.x, p.x );
      m_Min.y = std::min( m_Min.y, p.y );
      m_Min.z = std::min( m_Min.z, p.z );
      m_Max.x = std::max( m_Max.x, p.x );
      m_Max.y = std::max( m_Max.y, p.y );
      m_Max.z = std::max( m_Max.z, p.z );
   }

   void
   addInternalPoint( T x, T y, T z )
   {
      addInternalPoint( {x, y, z} );
   }

   void
   addInternalBox( Box3 const & o )
   {
      addInternalPoint( o.getMin() );
      addInternalPoint( o.getMax() );
   }

   V3
   getCorner( int corner ) const
   {
      switch ( corner )
      {
         case 0: return { m_Min.x, m_Min.y, m_Min.z };
         case 1: return { m_Min.x, m_Max.y, m_Min.z };
         case 2: return { m_Max.x, m_Max.y, m_Min.z };
         case 3: return { m_Max.x, m_Min.y, m_Min.z };
         case 4: return { m_Min.x, m_Min.y, m_Max.z };
         case 5: return { m_Min.x, m_Max.y, m_Max.z };
         case 6: return { m_Max.x, m_Max.y, m_Max.z };
         case 7: return { m_Max.x, m_Min.y, m_Max.z };
         default: break;
      }
   }

   Box3
   transformBox( M4 const & modelMatrix ) const
   {
      V3 const A = V3{ modelMatrix * V4( getA(), T(1.0) ) };
      V3 const B = V3{ modelMatrix * V4( getB(), T(1.0) ) };
      V3 const C = V3{ modelMatrix * V4( getC(), T(1.0) ) };
      V3 const D = V3{ modelMatrix * V4( getD(), T(1.0) ) };
      V3 const E = V3{ modelMatrix * V4( getE(), T(1.0) ) };
      V3 const F = V3{ modelMatrix * V4( getF(), T(1.0) ) };
      V3 const G = V3{ modelMatrix * V4( getG(), T(1.0) ) };
      V3 const H = V3{ modelMatrix * V4( getH(), T(1.0) ) };

      // No perspective divide by w wanted here,
      // so pls never use this very function with a projection matrix.
      // If you need to transform a unit-cube in ndc/clipSpace to a
      // camera world frustum = invViewProj * unitcube;
      // unitcube = camera frustum in ndc space.
      // So any frustum ( sheared box = Spat ) will always become
      // a unit-cube in normalized device coords in range [-1,-1,-1] to [1,1,1].
      // Thats what a projectionMatrix does - transform a camera frustum to ndc unit-cube.

      Box3 bbox( A,A);
      //bbox.addInternalPoint( A );
      bbox.addInternalPoint( B );
      bbox.addInternalPoint( C );
      bbox.addInternalPoint( D );
      bbox.addInternalPoint( E );
      bbox.addInternalPoint( F );
      bbox.addInternalPoint( G );
      bbox.addInternalPoint( H );
      return bbox;
   }

   Box3
   scale( T skalar ) const { return Box3( m_Min * skalar, m_Max * skalar ); }

   Box3
   widen( T skalar ) const
   {
      T const t = T(0.5) * skalar;
      return Box3( m_Min - t, m_Max + t );
   }

   Box3
   widenZ( float skalar ) const
   {
      T const t = T(0.5) * skalar;
      V3 m( m_Min ); m.z -= t;
      V3 n( m_Max ); n.z += t;
      return Box3( m, n );
   }


   int
   intersectRay( V3 const & rayStart, V3 const & rayDir, V3 & hitPosition ) const
   {
      T dx = T(0.5) * getSize().x;
      T dy = T(0.5) * getSize().y;
      T dz = T(0.5) * getSize().z;

      bool nx = Plane3< T >( 1,0,0,-dx ).intersectRay( rayStart, rayDir, hitPosition );
      bool ny = Plane3< T >( 0,1,0,-dy ).intersectRay( rayStart, rayDir, hitPosition );
      bool nz = Plane3< T >( 0,0,1,-dz ).intersectRay( rayStart, rayDir, hitPosition );
      bool px = Plane3< T >( -1,0,0,dx ).intersectRay( rayStart, rayDir, hitPosition );
      bool py = Plane3< T >( 0,-1,0,dy ).intersectRay( rayStart, rayDir, hitPosition );
      bool pz = Plane3< T >( 0,0,-1,dz ).intersectRay( rayStart, rayDir, hitPosition );

      int n = 0;
      if ( nx ) n++;
      if ( ny ) n++;
      if ( nz ) n++;
      if ( px ) n++;
      if ( py ) n++;
      if ( pz ) n++;

      DE_DEBUG( "BoundingBox.IntersectCount = ",n,")" )
      return n;
   }

   T
   minDistanceTo( V3 const & pos ) const
   {
      T d = glm::length2( pos - getA() );
      d = std::min( d, glm::length2( pos - getB() ) );
      d = std::min( d, glm::length2( pos - getC() ) );
      d = std::min( d, glm::length2( pos - getD() ) );
      d = std::min( d, glm::length2( pos - getE() ) );
      d = std::min( d, glm::length2( pos - getF() ) );
      d = std::min( d, glm::length2( pos - getG() ) );
      d = std::min( d, glm::length2( pos - getH() ) );
      return dbSqrt( d );
   }

   T
   maxDistanceTo( V3 const & pos ) const
   {
      T d = glm::length2( pos - getA() );
      d = std::max( d, glm::length2( pos - getB() ) );
      d = std::max( d, glm::length2( pos - getC() ) );
      d = std::max( d, glm::length2( pos - getD() ) );
      d = std::max( d, glm::length2( pos - getE() ) );
      d = std::max( d, glm::length2( pos - getF() ) );
      d = std::max( d, glm::length2( pos - getG() ) );
      d = std::max( d, glm::length2( pos - getH() ) );
      return dbSqrt( d );
   }

/*
   // getTriangle( )
   // back:  B,A,D,C
   // front: G,H,E,F
   // left:  F,E,A,B
   // front: G,H,E,F
   // right: C,D,H,G
   // bottom:H,D,A,E
   // top:   B,C,G,F
*/
   constexpr uint32_t getColorA() const { return 0xFF0000FF; }
   constexpr uint32_t getColorB() const { return 0xFFFFFFFF; }
   constexpr uint32_t getColorC() const { return 0xFFFFFFFF; }
   constexpr uint32_t getColorD() const { return 0xFFFFFFFF; }
   constexpr uint32_t getColorE() const { return 0xFF00FFFF; }
   constexpr uint32_t getColorF() const { return 0xFFFFFFFF; }
   constexpr uint32_t getColorG() const { return 0xFFFFFFFF; }
   constexpr uint32_t getColorH() const { return 0xFFFFFFFF; }

};


typedef TSinTable< float > SinTablef;
typedef TSinTable< double > SinTabled;
typedef TCosTable< float > CosTablef;
typedef TCosTable< double > CosTabled;
typedef TSinCosTable< float > SinCosTablef;
typedef TSinCosTable< double > SinCosTabled;
typedef Turn2< float > Turn2f;
typedef Turn2< double > Turn2d;
typedef Line3< float > Line3f;
typedef Line3< double > Line3d;
typedef Ray3< float > Ray3f;
typedef Ray3< double > Ray3d;
typedef Plane3< float > Plane3f;
typedef Plane3< double > Plane3d;
typedef Triangle3< float > Triangle3f;
typedef Triangle3< double > Triangle3d;
typedef Box3< float > Box3f;
typedef Box3< double > Box3d;




} // end namespace gpu.
} // end namespace de.

#endif

