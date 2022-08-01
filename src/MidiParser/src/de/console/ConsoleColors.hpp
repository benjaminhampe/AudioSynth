#ifndef DARKCOLORCONSOLE_HPP
#define DARKCOLORCONSOLE_HPP

#include <cstdint>
#include <functional>
#include <cstdlib>
#include <sstream>
#include <thread>

namespace de {
namespace console {

//  i.e. \033[bg;fgm
//	\033[m Text wieder auf normal setzten
// 	\033[0m Text wieder auf normal setzten
// 	\033[1m Fetter Text
// 	\033[4m Unterstrichener Text
// 	\033[30m Schrift: Schwarz
// 	\033[31m Schrift: Rot
// 	\033[32m Schrift: Grün
// 	\033[33m Schrift: Gelb
// 	\033[34m Schrift: Blau
// 	\033[35m Schrift: Violett
// 	\033[36m Schrift: Türkis
// 	\033[37m Schrift: Weiss
// 	\033[40m Schrifthintergrund: Schwarz
// 	\033[41m Schrifthintergrund: Rot
// 	\033[42m Schrifthintergrund: Grün
// 	\033[43m Schrifthintergrund: Gelb
// 	\033[44m Schrifthintergrund: Blau
// 	\033[45m Schrifthintergrund: Violett
// 	\033[46m Schrifthintergrund: Türkis
// 	\033[47m Schrifthintergrund: Weiss
// 	\033[G Positioniert den Cursor in Spalte
// 	\033[H Positioniert den Cursor in Zeile
// 	\007 Gibt einen Signalton aus

//void color(unsigned bg, unsigned fg)
//{
//	// i.e. \033[bg;fgm
//	//printf("%c%c%02d%c%02d%c", ESC, '[', bg, ';', fg, 'm');
//}

//
// WebRef: ANSI Codes
//
//╔══════════╦════════════════════════════════╦═════════════════════════════════════════════════════════════════════════╗
//║  Code    ║             Effect             ║                                   Note                                  ║
//╠══════════╬════════════════════════════════╬═════════════════════════════════════════════════════════════════════════╣
//║ 0        ║  Reset / Normal                ║  all attributes off                                                     ║
//║ 1        ║  Bold or increased intensity   ║                                                                         ║
//║ 2        ║  Faint (decreased intensity)   ║  Not widely supported.                                                  ║
//║ 3        ║  Italic                        ║  Not widely supported. Sometimes treated as inverse.                    ║
//║ 4        ║  Underline                     ║                                                                         ║
//║ 5        ║  Slow Blink                    ║  less than 150 per minute                                               ║
//║ 6        ║  Rapid Blink                   ║  MS-DOS ANSI.SYS; 150+ per minute; not widely supported                 ║
//║ 7        ║  [[reverse video]]             ║  swap foreground and background colors                                  ║
//║ 8        ║  Conceal                       ║  Not widely supported.                                                  ║
//║ 9        ║  Crossed-out                   ║  Characters legible, but marked for deletion.  Not widely supported.    ║
//║ 10       ║  Primary(default) font         ║                                                                         ║
//║ 11–19    ║  Alternate font                ║  Select alternate font `n-10`                                           ║
//║ 20       ║  Fraktur                       ║  hardly ever supported                                                  ║
//║ 21       ║  Bold off or Double Underline  ║  Bold off not widely supported; double underline hardly ever supported. ║
//║ 22       ║  Normal color or intensity     ║  Neither bold nor faint                                                 ║
//║ 23       ║  Not italic, not Fraktur       ║                                                                         ║
//║ 24       ║  Underline off                 ║  Not singly or doubly underlined                                        ║
//║ 25       ║  Blink off                     ║                                                                         ║
//║ 27       ║  Inverse off                   ║                                                                         ║
//║ 28       ║  Reveal                        ║  conceal off                                                            ║
//║ 29       ║  Not crossed out               ║                                                                         ║
//║ 30–37    ║  Set foreground color          ║  See color table below                                                  ║
//║ 38       ║  Set foreground color          ║  Next arguments are `5;<n>` or `2;<r>;<g>;<b>`, see below               ║
//║ 39       ║  Default foreground color      ║  implementation defined (according to standard)                         ║
//║ 40–47    ║  Set background color          ║  See color table below                                                  ║
//║ 48       ║  Set background color          ║  Next arguments are `5;<n>` or `2;<r>;<g>;<b>`, see below               ║
//║ 49       ║  Default background color      ║  implementation defined (according to standard)                         ║
//║ 51       ║  Framed                        ║                                                                         ║
//║ 52       ║  Encircled                     ║                                                                         ║
//║ 53       ║  Overlined                     ║                                                                         ║
//║ 54       ║  Not framed or encircled       ║                                                                         ║
//║ 55       ║  Not overlined                 ║                                                                         ║
//║ 60       ║  ideogram underline            ║  hardly ever supported                                                  ║
//║ 61       ║  ideogram double underline     ║  hardly ever supported                                                  ║
//║ 62       ║  ideogram overline             ║  hardly ever supported                                                  ║
//║ 63       ║  ideogram double overline      ║  hardly ever supported                                                  ║
//║ 64       ║  ideogram stress marking       ║  hardly ever supported                                                  ║
//║ 65       ║  ideogram attributes off       ║  reset the effects of all of 60-64                                      ║
//║ 90–97    ║  Set bright foreground color   ║  aixterm (not in standard)                                              ║
//║ 100–107  ║  Set bright background color   ║  aixterm (not in standard)                                              ║
//╚══════════╩════════════════════════════════╩═════════════════════════════════════════════════════════════════════════╝


// Name            FG  BG
// Black           30  40
// Red             31  41
// Green           32  42
// Yellow          33  43
// Blue            34  44
// Magenta         35  45
// Cyan            36  46
// White           37  47
// Bright Black    90  100
// Bright Red      91  101
// Bright Green    92  102
// Bright Yellow   93  103
// Bright Blue     94  104
// Bright Magenta  95  105
// Bright Cyan     96  106
// Bright White    97  107

struct ColorReset
{
   std::string
   toString() const { return "\033[0m"; }
};

struct Color
{
   enum EColor
   {
      Black = 0, Red, Green, Yellow, Blue, Magenta, Cyan, White, Default
   };

   EColor m_fg_color;
   bool m_fg_bright;
   EColor m_bg_color;
   bool m_bg_bright;


   static Color error() { return Color( Color::White, true, Color::Red, true ); }
   static Color bluescreen() { return Color( Color::White, true, Color::Blue, true ); }
   static Color warn() { return Color( Color::Yellow, true, Color::Default, false ); }
   static Color debug() { return Color( Color::White, true, Color::Green, false ); }
   static Color trace() { return Color( Color::White, true, Color::Magenta, false ); }

   Color()
      : m_fg_color( Default )
      , m_fg_bright( false )
      , m_bg_color( Default )
      , m_bg_bright( false )
   {}

   Color( EColor fg_color, bool fg_bright, EColor bg_color, bool bg_bright )
      : m_fg_color( fg_color )
      , m_fg_bright( fg_bright )
      , m_bg_color( bg_color )
      , m_bg_bright( bg_bright )
   {}

   Color( int fg, int bg )
   {
      fg = fg % 16;
      bg = bg % 16;

      bool fg_bright = fg > 7;
      bool bg_bright = bg > 7;
      int fg_color = fg_bright ? fg - 8 : fg;
      int bg_color = bg_bright ? bg - 8 : bg;
      set( EColor(fg_color), fg_bright, EColor(bg_color), bg_bright );
   }

   void
   set( EColor fg_color, bool fg_bright, EColor bg_color, bool bg_bright )
   {
      m_fg_color = fg_color;
      m_fg_bright = fg_bright;
      m_bg_color = bg_color;
      m_bg_bright = bg_bright;
   }

   static Color
   randomColor()
   {
      int fg = (1 + rand() % 7) * ( 1 + rand() % 2 ); // No black, 14 colors left.
      return Color( fg, Default );
   }

   // Write a combined (foreground, background) color marker.
   inline std::string
   toString() const
   {
      int fg_color = 39; // ANSI: Set default foreground color
      int bg_color = 49; // ANSI: Set default background color

      if ( m_fg_color != Default )
      {
         fg_color = 30 + int( m_fg_color );
         if ( m_fg_bright ) fg_color += 60;
      }
      if ( m_bg_color != Default )
      {
         bg_color = 40 + int( m_bg_color );
         if ( m_bg_bright ) bg_color += 60;
      }

      std::ostringstream s;
      s << "\033[" << fg_color << ';' << bg_color << 'm';
      return s.str();
   }
};

//// Write a combined (foreground, background) color marker.
//inline std::string
//color( Color const & color )
//{
//   return color.toString();
//}

///// @brief Write specific combi color markers.
//inline std::ostream &
//color_error( std::ostream & o ) { o << Color::error(); return o; }

//inline std::ostream &
//color_fatal( std::ostream & o ) { o << Color::bluescreen(); return o; }

//inline std::ostream &
//color_warn( std::ostream & o ) { o << Color::warn(); return o; }

//inline std::ostream &
//color_debug( std::ostream & o ) { o << Color::debug(); return o; }

//inline std::ostream &
//color_trace( std::ostream & o ) { o << Color::trace(); return o; }

///// @brief Write a color reset marker.
//inline std::ostream &
//color_reset( std::ostream & o )
//{
//   o << "\033[0m";
//   return o;
//}

//// Write a performance marker.
//inline std::ostream &
//perf( std::ostream & o )
//{
//   o << color_fatal
//   << '['
//   << std::this_thread::get_id()
//   << "]"
//   << color_reset
//   << ' ';
//   return o;
//}

} // end namespace console
} // end namespace de

// Write a console color.
inline std::ostream &
operator<< ( std::ostream & o, de::console::Color const & consoleColor )
{
   o << consoleColor.toString();
   return o;
}

// Write a console color reset.
inline std::ostream &
operator<< ( std::ostream & o, de::console::ColorReset const & reset )
{
   o << reset.toString();
   return o;
}

#endif

