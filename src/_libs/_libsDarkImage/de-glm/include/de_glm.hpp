#ifndef DE_LIB_GLM_HPP
#define DE_LIB_GLM_HPP

// Use this header instead of including raw glm directly.

#include <cstdint>
// #include <cstdlib>
#include <cmath>
#include <limits>
#include <sstream>
#include <functional>
#include <algorithm>

typedef float float32_t;
typedef double float64_t;

// GLM headers
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#ifndef GLM_FORCE_AVX2
#define GLM_FORCE_AVX2
#endif

#ifndef GLM_FORCE_INLINE
#define GLM_FORCE_INLINE
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // value_ptr
#include <glm/gtc/matrix_transform.hpp> // ortho, lookAt
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

// ------------------------------------------------------------
// --- For my developer convenience
//
// Print GLM types using ostream << operator
//
// ------------------------------------------------------------
//
// ostream
//
template <typename T, glm::precision P = glm::defaultp>
std::ostream& operator<< ( std::ostream& o, glm::tvec1<T,P> const & v )
{
   o << v.x;
   return o;
}

template <typename T, glm::precision P = glm::defaultp>
std::ostream& operator<< ( std::ostream& o, glm::tvec2<T,P> const & v )
{
   o << v.x << "," << v.y;
   return o;
}

template <typename T, glm::precision P = glm::defaultp>
std::ostream& operator<< ( std::ostream& o, glm::tvec3<T,P> const & v )
{
   if ( std::abs( v.x ) > T(1.e-14) ) { o << v.x; }
   else { o << "0"; }
   o << ",";
   if ( std::abs( v.y ) > T(1.e-14) ) { o << v.y; }
   else { o << "0"; }
   o << ",";
   if ( std::abs( v.z ) > T(1.e-14) ) { o << v.z; }
   else { o << "0"; }
   return o;
}

template <typename T, glm::precision P = glm::defaultp>
std::ostream& operator<< ( std::ostream& o, glm::tvec4<T,P> const & v )
{
   o << v.x << "," << v.y << "," << v.z << "," << v.w;
   return o;
}

template <typename T, glm::precision P = glm::defaultp>
std::ostream& operator<< ( std::ostream& o, glm::tquat<T,P> const & q )
{
   o << q.x << "," << q.y << "," << q.z << "," << q.w;
   return o;
}

template <typename T, glm::precision P = glm::defaultp>
std::ostream& operator<< ( std::ostream& o, glm::tmat3x3<T,P> const & m3 )
{
   auto toS = [] ( T v ) { return ( std::abs( v ) > T(1.e-12) ) ? v : T(0); };
   T const * m = glm::value_ptr( m3 );
   o << toS(m[0]) << "," << toS(m[1]) << "," << toS(m[2]) << ", "
     << toS(m[3]) << ", "<< toS(m[4]) << "," << toS(m[5]) << ", "
     << toS(m[6]) << "," << toS(m[7]) << "," << toS(m[8]);
   return o;
}

template <typename T, glm::precision P = glm::defaultp>
std::ostream& operator<< ( std::ostream& o, glm::tmat4x4<T,P> const & m4 )
{
   auto toS = [] ( T v ) { return ( std::abs( v ) > T(1.e-12) ) ? v : T(0); };
   T const * m = glm::value_ptr( m4 );
   o << toS(m[0]) << "," << toS(m[1]) << "," << toS(m[2]) << "," << toS(m[3]) << ", ";
   o << toS(m[4]) << "," << toS(m[5]) << "," << toS(m[6]) << "," << toS(m[7]) << ", ";
   o << toS(m[8]) << "," << toS(m[9]) << "," << toS(m[10])<< "," << toS(m[11])<< ", ";
   o << toS(m[12])<< "," << toS(m[13])<< "," << toS(m[14])<< "," << toS(m[15]);
   return o;
}







// wostream



template <typename T, glm::precision P = glm::defaultp>
std::wostream& operator<< ( std::wostream& o, glm::tvec1<T,P> const & v )
{
   o << v.x;
   return o;
}

template <typename T, glm::precision P = glm::defaultp>
std::wostream& operator<< ( std::wostream& o, glm::tvec2<T,P> const & v )
{
   o << v.x << "," << v.y;
   return o;
}

template <typename T, glm::precision P = glm::defaultp>
std::wostream& operator<< ( std::wostream& o, glm::tvec3<T,P> const & v )
{
   if ( std::abs( v.x ) > T(1.e-14) ) { o << v.x; }
   else { o << L"0"; }
   o << L",";
   if ( std::abs( v.y ) > T(1.e-14) ) { o << v.y; }
   else { o << L"0"; }
   o << L",";
   if ( std::abs( v.z ) > T(1.e-14) ) { o << v.z; }
   else { o << L"0"; }
   return o;
}

template <typename T, glm::precision P = glm::defaultp>
std::wostream& operator<< ( std::wostream& o, glm::tvec4<T,P> const & v )
{
   o << v.x << "," << v.y << "," << v.z << "," << v.w;
   return o;
}

template <typename T, glm::precision P = glm::defaultp>
std::wostream& operator<< ( std::wostream& o, glm::tquat<T,P> const & q )
{
   o << q.x << "," << q.y << "," << q.z << "," << q.w;
   return o;
}

template <typename T, glm::precision P = glm::defaultp>
std::wostream& operator<< ( std::wostream& o, glm::tmat3x3<T,P> const & m3 )
{
   T const * m = glm::value_ptr( m3 );
   o << m[0] << "," << m[1] << "," << m[2] << ", "
     << m[3] << ", "<< m[4] << "," << m[5] << ", "
     << m[6] << "," << m[7] << "," << m[8];
   return o;
}

template <typename T, glm::precision P = glm::defaultp>
std::wostream& operator<< ( std::wostream& o, glm::tmat4x4<T,P> const & m4 )
{
   T const * m = glm::value_ptr( m4 );
   o << m[0] << "," << m[1] << "," << m[2] << "," << m[3] << ", ";
   o << m[4] << "," << m[5] << "," << m[6] << "," << m[7] << ", ";
   o << m[8] << "," << m[9] << "," << m[10]<< "," << m[11]<< ", ";
   o << m[12]<< "," << m[13]<< "," << m[14]<< "," << m[15];
   return o;
}









//template <typename T, glm::precision P = glm::defaultp>
//std::wostream& operator<< ( std::wostream& o, glm::tvec1<T,P> const & v ) { o << v.x; return o; }

//template <typename T, glm::precision P = glm::defaultp>
//std::wostream& operator<< ( std::wostream& o, glm::tvec2<T,P> const & v ) { o << v.x << "," << v.y; return o; }

//template <typename T, glm::precision P = glm::defaultp>
//std::wostream& operator<< ( std::wostream& o, glm::tvec3<T,P> const & v ) { o << v.x << "," << v.y << "," << v.z; return o; }

//template <typename T, glm::precision P = glm::defaultp>
//std::wostream& operator<< ( std::wostream& o, glm::tvec4<T,P> const & v ) { o << v.x << "," << v.y << "," << v.z << "," << v.w; return o; }

//template <typename T, glm::precision P = glm::defaultp>
//std::wostream& operator<< ( std::wostream& o, glm::tmat4x4<T,P> const & m )
//{
//   float32_t const * p = glm::value_ptr( m );
//   for ( size_t i = 0; i < 16; ++i )
//   { o << (*p) << " "; p++; }
//   return o;
//}

#endif
