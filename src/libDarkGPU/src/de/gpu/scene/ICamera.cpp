#include <de/gpu/scene/ICamera.hpp>

namespace de {
namespace gpu {

ICamera::ICamera()
   : m_Pos( 0, 60, -100 )
   , m_Target( 0, 50, 10 )
   , m_Up( 0,1,0 )
   //, m_Right( 1,0,0 )
   //, m_Dir( glm::normalize( V3( 0,0,100) - m_Pos ) )
   //, m_Ang( 0,0,0 )
   , m_IsDirty( true )
   , m_IsDirtyView( true )
   , m_IsDirtyProjection( true )
   , m_IsOrtho( false )
   , m_IsInputEnabled( true )
   , m_ViewMatrix( 1.0 )
   , m_ProjectionMatrix( 1.0 )
   , m_VP( 1.0 )
   , m_Viewport( 0, 0, 640, 480 )
   , m_ZNear( 0.2f )
   , m_ZFar( 38000.0f )
   , m_Fovy( 90.0f )
   , m_MouseX(0)
   , m_MouseY(0)
   , m_MouseMoveX(0)
   , m_MouseMoveY(0)
   , m_DragStartX(0)
   , m_DragStartY(0)
   , m_DragMoveX(0)
   , m_DragMoveY(0)
   , m_IsDragging( false )
   , m_IsLeftPressed( false )
   , m_IsMiddlePressed( false )
   , m_IsRightPressed( false )
   , m_Time( 0.0 )
   , m_LastTime( 0.0 )
   , m_MoveSpeed( 100.0f )
   , m_StrafeSpeed( 100.0f )
   , m_UpSpeed( 100.0f )
   , m_TurnSpeed( 1.0f )

{
   update();

   for ( auto & key : m_Keys )
   {
      key = false;
   }

}

// ICamera::~ICamera()
// {}

void
ICamera::setPos( V3 pos )
{
   auto delta = pos - m_Pos;
   m_Pos = pos;
   m_Target += delta;
   m_IsDirtyView = true;
   update();
}

void
ICamera::setDir( V3 dir )
{
   dir = glm::normalize( dir );
   T len = glm::length( m_Target - m_Pos );
   if ( len < T(1) ) len = T(1);
   m_Target = m_Pos + dir * len;
   m_IsDirtyView = true;
   update();
}

void
ICamera::setUp( V3 up )
{
   m_Up = up; // glm::normalize( up );
   m_IsDirtyView = true;
   update();
}


void
ICamera::onAnimate( double seconds )
{
   double dt = seconds-m_LastTime;
   m_LastTime = m_Time;
   m_Time = seconds;

   if ( m_IsDragging )
   {
      m_DragMoveX = m_MouseX.curr - m_DragStartX;
      m_DragMoveY = m_MouseY.curr - m_DragStartY;

      //DE_DEBUG("Mouse(",m_MouseX.curr,",",m_MouseY.curr,"), dt(",dt,")")
      //DE_DEBUG("DragMove(",m_DragMoveX,",",m_DragMoveY,"), dt(",dt,")")
      //DE_DEBUG("DragStart(",m_DragStartX,",",m_DragStartY,"), dt(",dt,")")

      if ( m_DragMoveX != 0 )
      {
         yaw( m_TurnSpeed * m_DragMoveX );
         m_DragMoveX = 0;
         m_DragStartX = m_MouseX.curr;
      }

      if ( m_DragMoveY != 0 )
      {
         pitch( m_TurnSpeed * m_DragMoveY );
         m_DragMoveY = 0;
         m_DragStartY = m_MouseY.curr;
      }
   }

   if ( m_Keys[ KEY_W ] )         { move( m_MoveSpeed * dt ); }
   if ( m_Keys[ KEY_A ] )         { strafe( -m_StrafeSpeed * dt ); }
   if ( m_Keys[ KEY_S ] )         { move( -m_MoveSpeed * dt ); }
   if ( m_Keys[ KEY_D ] )         { strafe( m_StrafeSpeed * dt ); }
   if ( m_Keys[ KEY_UP ] )        { move( m_MoveSpeed * dt ); }
   if ( m_Keys[ KEY_LEFT ] )      { strafe( -m_StrafeSpeed * dt ); }
   if ( m_Keys[ KEY_DOWN ] )      { move( -m_MoveSpeed * dt ); }
   if ( m_Keys[ KEY_RIGHT ] )     { strafe( m_StrafeSpeed * dt ); }
   if ( m_Keys[ KEY_PAGE_UP ] )   { elevate( m_UpSpeed * dt ); }
   if ( m_Keys[ KEY_Y ] )         { elevate( m_UpSpeed * dt ); }
   if ( m_Keys[ KEY_PAGE_DOWN ])  { elevate( -m_UpSpeed * dt ); }
   if ( m_Keys[ KEY_X ] )         { elevate( -m_UpSpeed * dt ); }

}

void
ICamera::onEvent( SEvent const & event )
{
   if ( !m_IsInputEnabled ) return;

   if ( event.type == EET_MOUSE_EVENT )
   {
      m_MouseX.curr = event.mouseEvent.x();
      m_MouseY.curr = event.mouseEvent.y();
      m_MouseMoveX = m_MouseX.curr - m_MouseX.last;
      m_MouseMoveY = m_MouseY.curr - m_MouseY.last;
      m_MouseX.last = m_MouseX.curr;
      m_MouseY.last = m_MouseY.curr;

      //m_IsLeftPressed = event.mouseEvent.isLeftPressed();
      //m_IsRightPressed = event.mouseEvent.isRightPressed();
      m_IsMiddlePressed = event.mouseEvent.isMiddlePressed();

      //DE_DEBUG("MouseEvent(",m_MouseX.curr,",",m_MouseY.curr,")")

      if ( event.mouseEvent.m_Flags & SMouseEvent::Wheel )
      {
         float wheel = 0.25f * event.mouseEvent.wheel();
         if ( wheel > 0.f )
         {
            move( 5.0f );
         }
         else
         {
            move( -5.0f );
         }
      }

      if ( event.mouseEvent.isLeft() )
      {
         m_IsLeftPressed = event.mouseEvent.isPressed();
      }

      if ( event.mouseEvent.isRight() )
      {
         if ( event.mouseEvent.isPressed() )
         {
            //DE_DEBUG("RMB Down(", m_MouseX.curr, ",",m_MouseY.curr,")")
            if ( !m_IsDragging )
            {
               //DE_DEBUG("DragStart(", m_DragStartX, ",", m_DragStartY,")")
               m_DragStartX = event.mouseEvent.x();
               m_DragStartY = event.mouseEvent.y();
               m_IsDragging = true;
            }
         }
         else
         {
            //DE_DEBUG("RMB Up(", m_MouseX.curr, ",",m_MouseY.curr,")")
            if ( m_IsDragging )
            {
               //DE_DEBUG("DragEnd(", m_DragStartX, ",", m_DragStartY,")")
               m_IsDragging = false;
            }
         }
      }
   }
   else if ( event.type == EET_KEY_EVENT )
   {
      m_Keys[ event.keyEvent.Key ] = event.keyEvent.pressed();
   }

}

// ViewMatrix
void
ICamera::move( T speed )
{
   auto delta = getDir() * speed;
   m_Pos += delta;
   m_Target += delta;
   m_IsDirtyView = true;
   update();
}
// ViewMatrix
void
ICamera::strafe( T speed )
{
   auto delta = getRight() * speed;
   m_Pos += delta;
   m_Target += delta;
   m_IsDirtyView = true;
   update();
}
// ViewMatrix
void
ICamera::elevate( T speed )
{
   auto delta = getUp() * speed;
   m_Pos += delta;
   m_Target += delta;
   m_IsDirtyView = true;
   update();
}
// ViewMatrix
void
ICamera::yaw( T degrees )
{
   Quat< T > rot = Quat< T >::fromAngleAxis( toRad( degrees ), Quat<T>::V3(0,1,0) );
   auto dir = m_Target-m_Pos;
   auto len = glm::length( dir );
   dir = rot.rotate( dir );
   m_Target = m_Pos + dir * len;
   m_Up = rot.rotate( m_Up );
   m_IsDirtyView = true;
   update();
}
// ViewMatrix
void
ICamera::pitch( T degrees )
{
   Quat< T > rot = Quat< T >::fromAngleAxis( toRad( degrees ), getRight() );
   auto dir = m_Target-m_Pos;
   auto len = glm::length( dir );
   dir = rot.rotate( dir );
   m_Target = m_Pos + dir * len;
   m_Up = rot.rotate( m_Up );
   m_IsDirtyView = true;
   update();
}
// ViewMatrix
void
ICamera::roll( T degrees )
{
   auto dir = m_Target-m_Pos;
   auto len = glm::length( dir );
   dir = glm::normalize( dir );

   Quat< T > rot = Quat< T >::fromAngleAxis( toRad( degrees ), dir );
   dir = rot.rotate( dir );

   m_Target = m_Pos + dir * len;
   m_Up = rot.rotate( m_Up );
   m_IsDirtyView = true;
   update();
}

// Matrices
void
ICamera::update()
{
   if ( m_IsDirtyView )
   {
      //m_ViewMatrix = de::Math::buildLookAtLH( m_Pos, m_Dir, m_Up );
      auto dir = glm::normalize( m_Target - m_Pos );
      auto up = glm::normalize( m_Up );
      auto left = glm::normalize( glm::cross( up, dir ) );
      m_Up = glm::normalize( glm::cross( dir,left ) );
      //up = glm::normalize( glm::cross( dir, left ) ); // recompute normalized
      m_ViewMatrix = M4( T(1) );
      T* m = glm::value_ptr( m_ViewMatrix );
      m[ 0 ] = left.x;
      m[ 4 ] = left.y;
      m[ 8 ] = left.z;
      m[ 1 ] = m_Up.x;
      m[ 5 ] = m_Up.y;
      m[ 9 ] = m_Up.z;
      m[ 2 ] = dir.x;
      m[ 6 ] = dir.y;
      m[ 10 ] = dir.z;
      m[ 12 ] = -glm::dot( left, m_Pos );
      m[ 13 ] = -glm::dot( m_Up, m_Pos );
      m[ 14 ] = -glm::dot( dir, m_Pos );
      //DE_DEBUG("pos(",pos,",) dir(",dir,"), up(",up,", left(",left,")")
      //m_Right = glm::normalize( glm::cross( m_Dir, m_Up ) ); // RH
      //m_Right = glm::normalize( glm::cross( m_Up, m_Dir ) ); // LH
      m_IsDirtyView = false;
      m_IsDirty = true;
      // DE_DEBUG( "[View] Pos(", m_Pos, "), Dir(", m_Dir, "), Up(", m_Up, "), Right(", m_Right, ") = ViewMat(", m_ViewMatrix, ")" )
   }

   if ( m_IsDirtyProjection )
   {
      double x( getX() );
      double y( getY() );
      double w( getWidth() );
      double h( getHeight() );
      if ( m_IsOrtho )
      {
         m_ProjectionMatrix = glm::orthoLH( T(x), T(x + w), T(y), T(y + h), m_ZNear, m_ZFar );
      }
      else
      {
         m_ProjectionMatrix = glm::perspectiveLH( de::Math::toRad( m_Fovy ), T(w)/T(h), m_ZNear, m_ZFar );
      }
      m_IsDirtyProjection = false;
      m_IsDirty = true;
      // DE_DEBUG( "[Proj] Pos(", m_Pos, "), Dir(", m_Dir, "), Up(", m_Up, "), Right(", m_Right, "), Matrix(", m_ViewMatrix, ")" )
   }

   if ( m_IsDirty )
   {
      m_VP = m_ProjectionMatrix * m_ViewMatrix;
      m_IsDirty = false;
   }
}

// ViewMatrix
void
ICamera::lookAt( V3 pos, V3 target, V3 up )
{
   m_Pos = pos;
   m_Target = target;
   m_Up = up;
//   m_Dir = glm::normalize( m_Target - m_Pos );
//   auto left = glm::normalize( glm::cross( up, m_Dir ) );
//   m_Up = glm::normalize( glm::cross( dir,left ) );
//   //m_Dir = glm::normalize( m_Target - m_Pos );
   m_IsDirtyView = true;
   update();
}

// Viewport
void
ICamera::setViewport( Recti const & viewport )
{
   if ( m_Viewport == viewport ) { return; }
   m_Viewport = viewport;
   m_IsDirtyProjection = true;
   update();
}

// ProjectionMatrix
void
ICamera::setProjectionMode( bool ortho )
{
   if ( m_IsOrtho == ortho ) return;
   m_IsOrtho = ortho;
   m_IsDirtyProjection = true;
   update();
}

void
ICamera::setNearValue( T nearValue )
{
   if ( m_ZNear == nearValue ) return;
   m_ZNear = nearValue;
   m_IsDirtyProjection = true;
   update();
}

void
ICamera::setFarValue( T farValue )
{
   if ( m_ZFar == farValue ) return;
   m_ZFar = farValue;
   m_IsDirtyProjection = true;
   update();
}

void
ICamera::setFOV( T fovy_degrees )
{
   if ( fovy_degrees < 1.0f ) fovy_degrees = 1.0f;
   if ( fovy_degrees > 179.0f ) fovy_degrees = 179.0f;
   if ( m_Fovy == fovy_degrees ) return;
   m_Fovy = fovy_degrees;
   m_IsDirtyProjection = true;
   update();
}



/*
///@brief DirectionVector to EulerAngles
///@return A rotation vector containing the Y (pitch) and Z (yaw) rotations (in rad) that when applied to a
/// direction vector(0,0,1) would make it point in the same direction as this vector.
/// The X (roll) rotation is always 0, since two Euler rotations are sufficient to any given direction.
void
setRotation( V3 const & euler )
{
   m_Dir = Direction3< T >::toEulerAngles(::computeDirFromEulerAngles( eulerRad );
   m_Right = glm::normalize( glm::cross( m_Up, m_Dir ) );
   m_IsDirty = true;
   // DE_DEBUG( "eulerAngles(", eulerRad * Math::RAD2DEG, ") -> direction(", m_Dir, ")" )
}

void
setEulerAnglesDEG( glm::vec3 const & eulerDeg )
{
   setEulerAngles( eulerDeg * Math::DEG2RAD );
}

///@brief DirectionVector to EulerAngles
///@return A rotation vector containing the Y (pitch) and Z (yaw) rotations (in rad) that when applied to a
/// direction vector(0,0,1) would make it point in the same direction as this vector.
/// The X (roll) rotation is always 0, since two Euler rotations are sufficient to any given direction.
V3
getRotation() const
{
   V3 euler( 0,0,0 );
   V3 d( glm::normalize( m_Dir ) );
   euler.x = ::atan2( std::sqrt( d.x*d.x + d.z*d.z ), d.y ) - T(Math::HALF_PI64);
   euler.y = ::atan2( d.x, d.z );
//      if ( euler.x < 0.0f ) euler.x += Math::TWOPI;
//      if ( euler.x >= Math::TWOPI ) euler.x -= Math::TWOPI;
//      if ( euler.y < -Math::HALF_PI+0.0001f ) euler.y = -Math::HALF_PI+0.0001f;
//      if ( euler.y > Math::HALF_PI-0.0001f ) euler.y = Math::HALF_PI-0.0001f;

   // DE_DEBUG( "direction(", m_Dir, ") -> eulerAngles(", euler * Math::RAD2DEG, ")" )
   return euler;
}

V3
getRotationDEG() const { return getRotation() * T(Math::RAD2DEG64); }
*/


} // end namespace gpu.
} // end namespace de.

