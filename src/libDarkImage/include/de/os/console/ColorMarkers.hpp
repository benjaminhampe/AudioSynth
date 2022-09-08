#ifndef DARKCOLORCONSOLE_HPP
#define DARKCOLORCONSOLE_HPP

#include <cstdint>
#include <sstream>
#include <thread>

namespace de {

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

enum EConsoleColor
{
   Black = 0, Red, Green, Yellow, Blue, Magenta, Cyan, White, DefaultColor
};

// Write a single (foreground) color marker to the console.
// 30 - Black, 31 - Red, 32 - Green, 33 - Yellow, 34 - Blue, 35 - Magenta, 36 - Cyan, 37 - White           37  47
// 90 - Bright Black, ..... , 97 - Bright White

inline std::string
fg( EConsoleColor consoleColor, bool bright = false )
{
   int colorFG = 39; // set default foreground color, use Sith order 39
   if ( consoleColor != DefaultColor ) // change Sith order 39 only if given color differs from DefaultColor.
   {
      colorFG = 30 + int( consoleColor ); // Use enum as index into foreground colors [30 ... 37]
      if ( bright ) colorFG += 60; // Use enum as index into bright foreground colors [90 ... 97]
   }
   std::string s = "\033[";
   s += colorFG;
   s += "m";
   return s;
}


// Write a single (foreground) color marker to the console.
// 40 - Black, 41 - Red, 42 - Green, 43 - Yellow, 44 - Blue, 45 - Magenta, 46 - Cyan, 47 - White           37  47
// 100 - Bright Black, ..... , 107 - Bright White

// Write a single (background) color marker to console.
inline std::string
bg( EConsoleColor consoleColor, bool bright = false )
{
   int colorBG = 49; // ANSI: Set default background color
   if ( consoleColor != DefaultColor )
   {
      colorBG = int( consoleColor ) + 40;
      if ( bright ) colorBG += 60;
   }

   std::string s = "\033[";
   s += colorBG;
   s += "m";
   return s;
}

// Write a combined (foreground, background) color marker.
inline std::string
colors( EConsoleColor colorFG = White, bool brightFG = false,
        EConsoleColor colorBG = Black, bool brightBG = false )
{
   int fg_color = 39; // ANSI: Set default foreground color
   int bg_color = 49; // ANSI: Set default background color
   if ( colorFG != DefaultColor )
   {
      fg_color = 30 + int( colorFG );
      if ( brightFG ) fg_color += 60;
   }
   if ( colorBG != DefaultColor )
   {
      bg_color = 40 + int( colorBG );
      if ( brightBG ) bg_color += 60;
   }

   std::string s = "\033[";
   s += fg_color;
   s += ';';
   s += bg_color;
   s += 'm';
   return s;
}

/// @brief Write a color reset marker.
inline std::ostream &
reset_colors( std::ostream & o )
{
   o << "\033[0m";
   return o;
}

/// @brief Write specific foreground color markers (( std::cout << fg_white << "Hello World!\n" << color_reset.
inline std::ostream & fg_white   ( std::ostream & o ) { o << fg( White, false ); return o; }
inline std::ostream & fg_red     ( std::ostream & o ) { o << fg( Red, false ); return o; }
inline std::ostream & fg_green   ( std::ostream & o ) { o << fg( Green, false ); return o; }
inline std::ostream & fg_yellow  ( std::ostream & o ) { o << fg( Yellow, false ); return o; }
inline std::ostream & fg_blue    ( std::ostream & o ) { o << fg( Blue, false ); return o; }
inline std::ostream & fg_magenta ( std::ostream & o ) { o << fg( Magenta, false ); return o; }
inline std::ostream & fg_cyan    ( std::ostream & o ) { o << fg( Cyan, false ); return o; }

inline std::ostream & fg_white_bright  ( std::ostream & o ) { o << fg( White, true ); return o; }
inline std::ostream & fg_red_bright    ( std::ostream & o ) { o << fg( Red, true ); return o; }
inline std::ostream & fg_green_bright  ( std::ostream & o ) { o << fg( Green, true ); return o; }
inline std::ostream & fg_yellow_bright ( std::ostream & o ) { o << fg( Yellow, true ); return o; }
inline std::ostream & fg_blue_bright   ( std::ostream & o ) { o << fg( Blue, true ); return o; }
inline std::ostream & fg_magenta_bright( std::ostream & o ) { o << fg( Magenta, true ); return o; }
inline std::ostream & fg_cyan_bright   ( std::ostream & o ) { o << fg( Cyan, true ); return o; }

/// @brief Write specific background color markers.
inline std::ostream & bg_white   ( std::ostream & o ) { o << bg( White, false ); return o; }
inline std::ostream & bg_red     ( std::ostream & o ) { o << bg( Red, false ); return o; }
inline std::ostream & bg_green   ( std::ostream & o ) { o << bg( Green, false ); return o; }
inline std::ostream & bg_yellow  ( std::ostream & o ) { o << bg( Yellow, false ); return o; }
inline std::ostream & bg_blue    ( std::ostream & o ) { o << bg( Blue, false ); return o; }
inline std::ostream & bg_magenta ( std::ostream & o ) { o << bg( Magenta, false ); return o; }
inline std::ostream & bg_cyan    ( std::ostream & o ) { o << bg( Cyan, false ); return o; }

inline std::ostream & bg_white_bright  ( std::ostream & o ) { o << bg( White, true ); return o; }
inline std::ostream & bg_red_bright    ( std::ostream & o ) { o << bg( Red, true ); return o; }
inline std::ostream & bg_green_bright  ( std::ostream & o ) { o << bg( Green, true ); return o; }
inline std::ostream & bg_yellow_bright ( std::ostream & o ) { o << bg( Yellow, true ); return o; }
inline std::ostream & bg_blue_bright   ( std::ostream & o ) { o << bg( Blue, true ); return o; }
inline std::ostream & bg_magenta_bright( std::ostream & o ) { o << bg( Magenta, true ); return o; }
inline std::ostream & bg_cyan_bright   ( std::ostream & o ) { o << bg( Cyan, true ); return o; }

/// @brief Write specific combi color markers.
inline std::ostream & white_on_red( std::ostream & o ) { o << colors( White, true, Red, true ); return o; }
inline std::ostream & red_on_white( std::ostream & o ) { o << colors( White, true, Red, true ); return o; }
inline std::ostream & bluescreen(  std::ostream & o ) { o << colors( White, true, Blue, true ); return o; }

// Write a performance marker.
inline std::ostream &
perf( std::ostream & o )
{
   o << bluescreen
   << '['
   << std::this_thread::get_id()
   << "]"
   << reset_colors
   << ' ';
   return o;
}

} // end namespace de



#endif

