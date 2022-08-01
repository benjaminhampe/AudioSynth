#ifndef DE_ALIGNMENT_HPP
#define DE_ALIGNMENT_HPP

#include <de/Math.hpp>
#include <de/Rectf.hpp>
#include <de/Recti.hpp>

inline bool
// =======================================================================
dbIsMouseOver( int mx, int my, int x1, int y1, int x2, int y2 )
// =======================================================================
{
   if ( x1 > x2 ) std::swap( x1, x2 );
   if ( y1 > y2 ) std::swap( y1, y2 );
   if ( mx < x1 ) return false;
   if ( my < y1 ) return false;
   if ( mx > x2 ) return false;
   if ( my > y2 ) return false;
   return true;
}

namespace de {

template < typename T >
// =======================================================================
struct BoxTransform2 // Transform between data/view ( linearly )
// =======================================================================
{
   typedef glm::tvec2< T > V2;

   static glm::ivec2
   toView( V2 const & p_data, Rectf const & r_data, Recti const & r_view )
   {
      int32_t x = int32_t( T(0.5) + T( r_view.getWidth() ) * ( p_data.x - r_data.x ) / r_data.getWidth() );
      int32_t y = int32_t( T(0.5) + T( r_view.getHeight() ) * ( p_data.y - r_data.y ) / r_data.getHeight() );
      return { r_view.x + x, r_view.y + y };
   }

   static V2
   toData( glm::ivec2 const & p_view, Recti const & r_view, Rectf const & r_data )
   {
      T x = ( r_data.getWidth() * T( p_view.x - r_view.x ) / T( r_view.getWidth() ) );
      T y = ( r_data.getHeight() * T( p_view.y - r_view.y ) / T( r_view.getHeight() ) );
      return { r_data.x + x, r_data.y + y };
   }

};

typedef BoxTransform2< float > BoxTransform2f;
typedef BoxTransform2< double > BoxTransform2d;

// =======================================================================
struct AspectRatio
// =======================================================================
{
   // Aligns a text around its anchor point inside screen(scr_w,scr_h) rect.
   // textRect.xy = given anchor point
   // textRect.wh = given text size(w,h) in (pixels).
   static glm::ivec2
   fitAspectPreserving( int src_w, int src_h, int max_w, int max_h )
   {
      if ( src_w < 1 ) return { 0,0 };
      if ( src_h < 1 ) return { 0,0 };
      if ( max_w < 1 ) return { 0,0 };
      if ( max_h < 1 ) return { 0,0 };

      float aspect_src = float( src_w ) / float( src_h );
      float aspect_max = float( max_w ) / float( max_h );

      int dst_w = max_w;
      int dst_h = max_h;

      // +----max_w----+
      // |             |
      // |+-----------+|
      // ||           ||max_h       dst_w     src_w     max_w
      // |+-----------+|           ------- = ------- > -------
      // |             |            dst_h     src_h     max_h
      // +-------------+
      if ( aspect_src >= aspect_max )
      {
         dst_w = max_w;
         dst_h = int32_t( std::round( float( dst_w ) / aspect_src ) );
      }
      // +-------------+
      // |   +-----+   |
      // |   |     |   |
      // |   |     |   |max_h
      // |   |     |   |
      // |   +-----+   |
      // +-------------+
      else
      {
         dst_h = max_h;
         dst_w = int32_t( std::round( float( dst_h ) * aspect_src ) );
      }

      return { dst_w, dst_h };
   }

};

// =======================================================================
struct Align
// =======================================================================
{
   enum EAlign
   {
      NoAlign = 0,
      Left = 1,
      Center = 2,
      Right = 4,
      Top = 8,
      Middle = 16,
      Bottom = 32,

      TopLeft = Left | Top,
      TopCenter = Center | Top,
      TopRight = Right | Top,
      MiddleLeft = Left | Middle,
      MiddleCenter = Center | Middle,
      MiddleRight = Right | Middle,
      BottomLeft = Left | Bottom,
      BottomCenter = Center | Bottom,
      BottomRight = Right | Bottom,

      LeftTop = TopLeft,
      CenterTop = TopCenter,
      RightTop = TopRight,
      LeftMiddle = MiddleLeft,
      Centered = Center | Middle,
      RightMiddle = MiddleRight,
      LeftBottom = BottomLeft,
      CenterBottom = BottomCenter,
      RightBottom = BottomRight,

      Default = TopLeft
   };

/*
   My first try, only aligns a text around its anchor point, not aligned to outer screen rect.
   static inline glm::ivec2
   apply( glm::ivec2 const & pos, Align::EAlign align, int textWidth, int textHeight )
   {
      int x = pos.x;
      int y = pos.y;

           if ( align & Align::Center ){ x -= textWidth/2;   }
      else if ( align & Align::Right ) { x -= textWidth;   }

           if ( align & Align::Top )   { y += 3*textHeight/4;   }
      else if ( align & Align::Middle ){ y += textHeight/2 - 2;   }

      return {x,y};
   }
*/

   // Aligns a text around its anchor point inside screen(scr_w,scr_h) rect.
   // textRect.xy = given anchor point
   // textRect.wh = given text size(w,h) in (pixels).
   static Recti
   apply( Recti const & textRect, Align::EAlign align, int scr_w, int scr_h )
   {
      int text_x = textRect.getX();
      int text_y = textRect.getY();
      int text_w = textRect.getWidth();
      int text_h = textRect.getHeight();

      int x = text_x; // Default: left (Align::TopLeft)
      int y = text_y; // Default: top (Align::TopLeft)

      // horizontal align to entire screen
      if ( align & Align::Center )
      {
         x = text_x + (scr_w - text_w)/2;
      }
      else if ( align & Align::Right )
      {
         x = scr_w - 1 - text_w - text_x;
      }

      // vertical align to entire screen
      if ( align & Align::Middle )
      {
         y = text_y + (scr_h - text_h)/2;
      }
      else if ( align & Align::Bottom )
      {
         y = scr_h - 1 - text_h - text_y;
      }

      return Recti( x, y, text_w, text_h ); // actual screen position, used for picking
   }

};


} // end namespace de.

#endif
