#ifndef DE_GPU_TRS_HPP
#define DE_GPU_TRS_HPP

#include <de/gpu/Math3D.hpp>
#include <de/gpu/Math3D_Quaternion.hpp>
#include <de/gpu/Math3D_DualQuaternion.hpp>

namespace de {
namespace gpu {

template < typename T >
// =======================================================================
struct TRS
// =======================================================================
{
   // typedef double T;
   typedef glm::tmat4x4< T > M4;
   typedef glm::tvec2< T > V2;
   typedef glm::tvec3< T > V3;
   typedef glm::tvec4< T > V4;

   DE_CREATE_LOGGER("de.gpu.TRS")
   V3 m_T; // Relative translation of the scene node.
   V3 m_R; // Relative rotation (euler angles) of the scene node.
   V3 m_S; // Relative scale of the scene node.

   M4 m_TRS;

   TRS()
      : m_T(0,0,0), m_R(0,0,0), m_S(1,1,1), m_TRS( T(1) ) // All identity
   {}

   M4 const & getModelMatrix() const { return m_TRS; }
   M4 const & getTRS() const { return m_TRS; }

   V3 const & getT() const { return m_T; }
   T const & getTX() const { return m_T.x; }
   T const & getTY() const { return m_T.y; }
   T const & getTZ() const { return m_T.z; }

   V3 const & getR() const { return m_R; }
   T const & getRX() const { return m_R.x; }
   T const & getRY() const { return m_R.y; }
   T const & getRZ() const { return m_R.z; }

   V3 const & getS() const { return m_S; }
   T const & getSX() const { return m_S.x; }
   T const & getSY() const { return m_S.y; }
   T const & getSZ() const { return m_S.z; }

   void setT( T x, T y, T z ) { m_T = V3( x,y,z ); updateTRS(); }
   void setTX( T x ) { m_T.x = x; updateTRS(); }
   void setTY( T y ) { m_T.y = y; updateTRS(); }
   void setTZ( T z ) { m_T.z = z; updateTRS(); }

   void setR( T x, T y, T z ) { m_R = V3( x,y,z ); updateTRS(); }
   void setRX( T x ) { m_R.x = x; updateTRS(); }
   void setRY( T y ) { m_R.y = y; updateTRS(); }
   void setRZ( T z ) { m_R.z = z; updateTRS(); }

   void setS( T x, T y, T z ) { m_S = V3( x,y,z ); updateTRS(); }
   void setSX( T x ) { m_S.x = x; updateTRS(); }
   void setSY( T y ) { m_S.y = y; updateTRS(); }
   void setSZ( T z ) { m_S.z = z; updateTRS(); }

   void addT( T x, T y, T z ) { m_T += V3( x,y,z ); updateTRS(); }
   void addR( T x, T y, T z ) { m_R += V3( x,y,z ); updateTRS(); }
   void addS( T x, T y, T z ) { m_S += V3( x,y,z ); updateTRS(); }

   void updateTRS()
   {
      // if (m_S != V3(1,1,1) )
      // {

      // }
      // if (m_R != V3(0,0,0) )
      // {
      M4 t = glm::translate( M4( T(1) ), m_T );
      M4 rx = glm::rotate( M4( T(1) ), glm::radians( m_R.x ), { 1., 0., 0. } );
      M4 ry = glm::rotate( M4( T(1) ), glm::radians( m_R.y ), { 0., 1., 0. } );
      M4 rz = glm::rotate( M4( T(1) ), glm::radians( m_R.z ), { 0., 0., 1. } );
      M4 r = rz * ry * rx;
      M4 s = glm::scale( M4(1), m_S );
      m_TRS = t * (r * s);

      //decomposeT( m_TRS );
   }

   //Box3f
   //getAbsoluteBoundingBox() const { return getBoundingBox().transformBox( m_AbsoluteTransform ); }

   // Gets the absolute position of the node in world coordinates.
   static V3
   decomposeT( M4 const & trs )
   {
      T const * const m = glm::value_ptr( trs );
      T x = m[ 12 ];
      T y = m[ 13 ];
      T z = m[ 14 ];
      //DE_DEBUG("AbsolutePosition = (",x,",",y,",",z,"), m_Translation")
      return { x, y, z };
   }

};

typedef TRS< float > TRSf;
typedef TRS< double > TRSd;

} // end namespace gpu.
} // end namespace de.

#endif

