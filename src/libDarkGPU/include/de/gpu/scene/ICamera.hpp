#ifndef BENNI_GPU_ICAMERA_HPP
#define BENNI_GPU_ICAMERA_HPP

#include <de/gpu/State.hpp> // SM2<T>
#include <de/gpu/ui/EventReceiver.hpp>
#include <de/gpu/Math3D.hpp>
#include <de/gpu/Math3D_Quaternion.hpp>

namespace de {
namespace gpu {

// =======================================================================
struct ICamera
// =======================================================================
{
   DE_CREATE_LOGGER("de.gpu.Camera")
   typedef double T;
   typedef glm::tmat4x4< T > M4;
   typedef glm::tvec2< T > V2;
   typedef glm::tvec3< T > V3;
   typedef glm::tvec4< T > V4;

   constexpr static double F64_RAD2DEG = 180.0 / M_PI;
   constexpr static double F64_DEG2RAD = M_PI / 180.0;
   constexpr static double toDeg ( double radians ) { return radians * F64_RAD2DEG; }
   constexpr static double toRad ( double degrees ) { return degrees * F64_DEG2RAD; }

   constexpr static float F32_RAD2DEG = float( F64_RAD2DEG );
   constexpr static float F32_DEG2RAD = float( F64_DEG2RAD );
   constexpr static float toDeg ( float radians ) { return radians * F32_RAD2DEG; }
   constexpr static float toRad ( float degrees ) { return degrees * F32_DEG2RAD; }

   // FpsCamera is a 3d line ( direction ) in atleast 3d space
   // [ModelViewMatrix]
   V3 m_Pos;      // (0,0,1) P - Start source pos, Eye
   V3 m_Target;   // (0,0,1) Q - Destination pos, LookAt
   V3 m_Up;       // (0,1,0) Up vector on unit sphere around src pos
   //V3 m_Right;    // (1,0,0) Side vector on unit sphere around src pos
   //V3 m_Dir;      // (0,0,1) Front vector on unit sphere around src pos
   //V3 m_Ang;      // Something computed

   // [Flags]
   bool m_IsDirty;
   bool m_IsDirtyView;
   bool m_IsDirtyProjection;
   bool m_IsOrtho;
   bool m_IsInputEnabled;
   // bool m_IsRightHanded;

   // [Matrices]
   M4 m_ViewMatrix;
   M4 m_ProjectionMatrix;
   M4 m_VP;

   // [ProjectionMatrix]
   Recti m_Viewport;
   T m_ZNear;
   T m_ZFar;
   T m_Fovy;

   // [Name]
   std::string m_Name;

   // Camera control
   std::array< bool, 1024 > m_Keys;

   SM3< int32_t > m_MouseX;   // Mouse Pos X
   SM3< int32_t > m_MouseY;   // Mouse Pos Y
   int32_t m_MouseMoveX;
   int32_t m_MouseMoveY;
   int32_t m_DragStartX;// Mouse Pos X at right click start
   int32_t m_DragStartY;// Mouse Pos Y at right click start
   int32_t m_DragMoveX; // Mouse Move X at right click start
   int32_t m_DragMoveY; // Mouse Move Y at right click start

   bool m_IsDragging;
   bool m_IsLeftPressed; // left mb pressed
   bool m_IsMiddlePressed;
   bool m_IsRightPressed;

   double m_Time;
   double m_LastTime;

   float m_MoveSpeed;
   float m_StrafeSpeed;
   float m_UpSpeed;
   float m_TurnSpeed;

   ICamera();
   virtual ~ICamera() = default;
   virtual void onAnimate( double seconds );
   virtual void onEvent( SEvent const & event );

   // ViewMatrix
   virtual void move( T speed );
   virtual void strafe( T speed );
   virtual void elevate( T speed );
   virtual void yaw( T degrees );
   virtual void pitch( T degrees );
   virtual void roll( T degrees );
   virtual void lookAt( V3 pos, V3 target, V3 up = V3( 0, 1, 0) );

   // Name
   virtual std::string const & getName() const { return m_Name; }
   virtual void setName( std::string const & name ) { m_Name = name; }

   // IsInputEnabled
   virtual bool isInputEnabled() const { return m_IsInputEnabled; }
   virtual void setInputEnabled( bool enable ) { m_IsInputEnabled = enable; }

   // Matrices
   virtual void update();
   virtual M4 const & getViewProjectionMatrix() const { return m_VP; }
   virtual M4 const & getViewMatrix() const { return m_ViewMatrix; }
   virtual M4 const & getProjectionMatrix() const { return m_ProjectionMatrix; }

   // Viewport
   virtual void setViewport( Recti const & viewport );
   virtual int32_t getWidth() const { return getViewport().getWidth(); }    // Alias used by my shaders
   virtual int32_t getHeight() const { return getViewport().getHeight(); }  // Alias used by my shaders
   virtual int32_t getX() const { return getViewport().getX(); }
   virtual int32_t getY() const { return getViewport().getY(); }
   virtual void setScreenSize( int32_t w, int32_t h ) { setViewport( {0,0,w,h} ); }
   virtual void setScreenSize( glm::ivec2 const & size ) { setViewport( {0,0, size.x, size.y} ); }
   virtual Recti const & getViewport() const { return m_Viewport; }
   virtual void setViewport( int32_t x, int32_t y, int32_t w, int32_t h ) { setViewport( {x,y,w,h} ); }
   virtual glm::ivec2 getScreenSize() const { return { getWidth(), getHeight() }; }
   virtual T getAspectRatio() const { return ( getHeight() < 1 ) ? 1.0f : T( getWidth() ) / T( getHeight() ); }

   // ProjectionMatrix
   virtual T getNearValue() const { return m_ZNear; }
   virtual T getFarValue() const { return m_ZFar; }
   virtual T getFOV() const { return m_Fovy; }
   virtual bool isOrtho() const { return m_IsOrtho; }
   virtual void setProjectionMode( bool ortho = false );
   virtual void setNearValue( T nearValue );
   virtual void setFarValue( T farValue );
   virtual void setFOV( T fovy_degrees );

   // ViewMatrix
   virtual V3 getPos() const { return m_Pos; }
   virtual V3 getTarget() const { return m_Target; }
   virtual V3 getUp() const { return m_Up; }
   virtual V3 getRight() const
   {
      return glm::normalize( glm::cross( getUp(), getDir() ) );
   }
   virtual V3 getDir() const
   {
      return glm::normalize( m_Target - m_Pos );
   }
   virtual V3 getAng() const
   {
      return Direction3< T >( getDir() ).toEulerAngles();
      //return Quat< T >::fromDirection( getDir() ).to();
      //return Quat< T >::fromDirection( getDir() ).toEulerAngles().toDegrees();
   }

   virtual void setPos( V3 pos );
   virtual void setDir( V3 dir );
   virtual void setUp( V3 up );

   virtual void setPos( T x, T y, T z ) { setPos( {x,y,z} ); }
   virtual void setDir( T x, T y, T z ) { setDir( {x,y,z} ); }
   virtual void setUp(  T x, T y, T z ) { setUp( {x,y,z} ); }

   virtual bool isRightHanded() const
   {
      return glm::dot( glm::cross( getRight(), getUp() ), getDir() ) >= T( 0 );
   }

   virtual Ray3d computeRay( int mouse_x, int mouse_y )
   {
      M4 viewProjectionInv = glm::inverse( getViewProjectionMatrix() );
      return Ray3d::compute3DWorldRayFrom2DScreen( mouse_x, mouse_y,
         getWidth(), getHeight(), viewProjectionInv );
   }

   virtual Ray3f computeRayf( int mouse_x, int mouse_y )
   {
      Ray3d r3d = computeRay( mouse_x, mouse_y );
      return r3d.convert< float >();
   }
};

} // end namespace gpu.
} // end namespace de.

#endif

