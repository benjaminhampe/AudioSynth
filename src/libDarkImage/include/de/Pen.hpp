#ifndef DE_PEN_HPP
#define DE_PEN_HPP

#include <de/Math.hpp>

namespace de {

// =======================================================================
// LineCaps for Start- and EndSegment of a polyline by SVG standard
// =======================================================================
struct LineCap
{
    enum ECap
    {
        NONE = 0, // SVG lineCap = "butt"
        ROUND,    // SVG lineCap = "round"
        SQUARE,   // SVG lineCap = "square"
        ENUM_END
    };

    LineCap() : Value( NONE ) {}
    LineCap( ECap value ) : Value( value ) {}
    operator ECap () const { return Value; }
    ECap Value; ///< Member Field
};

// =======================================================================
// LineJoins between two segments of a polyline by SVG standard
// =======================================================================
struct LineJoin
{
   enum EJoin
   {
      NONE = 0,   // No visible line-join
      BEVEL,      // Add one or two triangles == AUTO_DOUBLE_BEVEL
      ROUND,      // For now a circular, not elliptic shape, because lineWidth is const for all segments ( for now )
      MITER,      // Not used, produces too large and ugly triangles. TODO: Miter should reuse already computed intersection point
      ENUM_END    // Indicates invalid state, also counts the enum-items before
   };
   LineJoin() : Value( ROUND ) {}
   LineJoin( EJoin value ) : Value( value ) {}
   operator EJoin () const { return Value; }
   EJoin Value; ///< Member Field
};

// =======================================================================
// LineFilter - AntiAliasing or Benni 3x3 filtering ( like blur )
// =======================================================================
struct LineFilter
{
   enum EFilter
   {
      NONE = 0,// Default = Aliased ( most ugly ), fastest and easiest to implement.
      AA2X,
      AA3X,
      AA4X,
      COUNT,
      ALIASED = NONE,
      DEFAULT = NONE
   };

   LineFilter() : value( NONE ) {}
   LineFilter( EFilter filter ) : value( filter ) {}
   operator uint32_t() const { return value; }

   EFilter value; ///< Member Field
};


// =======================================================================
// Pen | LineStyle
// =======================================================================
struct Pen
{
   uint32_t color;   ///< Line color
   float width;      ///< Line width in px
   LineCap cap;      ///< Line cap
   LineJoin join;    ///< Line join
   uint16_t stipple; ///< Line stipple pattern, emulates GL line stipple pattern here
   int32_t offset;   ///< Line offset in px, emulates GL line stipple offset here
   LineFilter filter;

   Pen()
      : color( 0xFF000000 ) // Default is = black pencil on white paper background.
      , width( 1.0f )       // Default is a simple
      , cap()
      , join()
      , stipple( 0xFFFF )
      , offset( 0 )
   {}

   Pen( uint32_t lineColor, float lineWidth = 1.0f, uint16_t lineStipple = 0xFFFF, int32_t lineOffset = 0 )
      : color( lineColor )
      , width( lineWidth )
      , cap()
      , join()
      , stipple( lineStipple )
      , offset( lineOffset )
   {}
};

typedef Pen LineStyle;

} // end namespace de.

#endif
