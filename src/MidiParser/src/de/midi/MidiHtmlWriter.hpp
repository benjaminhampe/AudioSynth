#ifndef DE_MIDI_2_HTML_WRITER_HPP
#define DE_MIDI_2_HTML_WRITER_HPP

#include <de/midi/MidiParser.hpp>
#include <de/HtmlWriter.hpp>

namespace de {
namespace midi {


inline uint32_t randomHtmlColor()
{
   return de::RGBA( rand()%200, rand()%200, rand()%200 );
}


// =======================================================================
struct MidiToken
// =======================================================================
{
   uint64_t m_address;
   uint32_t m_fg;
   uint32_t m_bg;
   std::string m_comment;
   std::vector< uint8_t > m_data;

   MidiToken()
         : m_address( 0 )
         , m_fg( randomHtmlColor() )
         , m_bg( 0xFFFFFFFF )
   {}

   MidiToken( uint64_t debugAddress, uint8_t const* beg, uint8_t const* end, std::string const & comment )
         : m_address( debugAddress )
         , m_fg( randomHtmlColor() )
         , m_bg( 0xFFFFFFFF )
         , m_comment( comment )
   {
      m_data.assign( beg, end );
   }

   size_t
   size() const { return m_data.size(); }

   std::string
   dataString( size_t maxBytesPerRow = 0 ) const
   {
      uint8_t const* beg = m_data.data();
      uint8_t const* end = m_data.data() + m_data.size();
      if ( maxBytesPerRow < 1 )
         return hexStr( beg, end );
      else
         return hexStr( beg, end, maxBytesPerRow );
   }

   std::string
   toString() const
   {
      return addressString() + " " + dataString();
   }

   std::string
   addressString() const
   {
      std::ostringstream s;
      s << "(0x" << hexStr( uint32_t(m_address) ) << ")";
      return s.str();
   }
};


// =======================================================================
struct MidiHtmlWriter : public IMidiParserListener
// =======================================================================
{
   DE_CREATE_LOGGER("de.MidiHtmlWriter")

   uint8_t const* m_startAddress;
   uint8_t const* m_endAddress;
   std::vector< MidiToken > m_tokens;

   MidiHtmlWriter()
      : m_startAddress( nullptr )
      , m_endAddress( nullptr )
   {
      DE_DEBUG("Constructor")
   }

   ~MidiHtmlWriter()
   {
      DE_DEBUG("Destructor")
   }

   void
   mpDebugStart( uint8_t const* beg, uint8_t const* end, std::string const & uri ) override
   {
      m_startAddress = beg;
      m_endAddress = end;
      m_tokens.clear();
   }

   void
   mpDebugToken( uint8_t const* beg, uint8_t const* end, std::string const & comment ) override
   {
      m_tokens.emplace_back( uint64_t( beg - m_startAddress ), beg, end, comment );
   }

   void
   save( std::string saveUri, std::string title, std::string mediaDir = "" )
   {
      std::ostringstream o;

      o << "<html>\n"
      "<title>Midi2Html: " << saveUri << "</title>\n"
      "<style type=\"text/css\">\n"
      "*    { margin:0; border:0; padding:0; }\n"
      "body { width:100%; height:100%; color:rgb(0,0,0); background-color:rgb(255,255,255); font-family:monospace; font-size:16px; }\n"
      //"caption { font-weight:bold; font-size:24px; line-height:50px; vertical-align:middle;}\n"
      "content { background-color:white; color:black; position:absolute; top:50px; padding-top:20px; white-space:nowrap; z-index:1;}\n"
      // "img { border:5px; border-color:black; padding:5px; margin:5px;}\n"
      // "div12 { width:100%; height:12px; line-height:12px; display-style:inline; }\n"
      // "h1 { color:white; background-color:rgb(0   ,0   ,0  ); font-size:64px; vertical-align:middle; font-weight:900; }\n"
      // "h2 { color:white; background-color:rgb(127 ,192 ,255); font-size:32px; vertical-align:middle; font-weight:750; }\n"
      // "h3 { color:white; background-color:rgb(192 ,200 ,255); font-size:24px; vertical-align:middle; font-weight:600; }\n"
      // "h4 { color:white; background-color:rgb(200 ,200 ,255); font-size:24px; vertical-align:middle; font-weight:600; }\n"
      // "h5 { color:black; background-color:rgb(255 ,255 ,255); font-size:16px; vertical-align:middle; font-weight:400; }\n"
      // "h6 { color:white; background-color:rgb(64  ,64  ,192); font-size:12px; vertical-align:middle; font-weight:400; }\n"
      // "hr { color:black; background-color:rgb(255 ,255 ,0); height:10px; line-height:5px; vertical-align:middle; }\n"
      // "date { color:#FF00FF; font-weight:bold; margin-left:20px;}\n"
      // "line { color:#207F20; font-weight:bold; margin-left:20px;}\n"
      // "time { color:#4040DF; font-weight:bold; margin-left:20px;}\n"
      // "trace {color:white; background-color:#00AF00; margin-left:20px;}\n"
      // "debug {color:white; background-color:#FF00FF; margin-left:20px;}\n"
      // "info { margin-left:20px; }\n"
      // "warn {color:black; background-color:yellow; margin-left:20px;}\n"
      // "error {color:white; background-color:red; margin-left:20px;}\n"
      "</style>\n</head>\n"
      "<body>\n"

      // ---------------------- title (=loadUri) ----------------------------------
      "<panel style=\"background-color:blue; position:fixed; top:0px; left:0px; width:100%; height:50px; z-index:2;\">\n"
      "<table width=\"100%\" height=\"100%\">\n"
      "<tr valign=\"middle\">"
      "<td align=\"center\">"
      "<caption style=\"color:white; font-weight:bold; font-size:16px; line-height:50px; vertical-align:middle;\">" << title << "</caption></td>\n"
      "</tr>\n"
      "</table>\n"
      "</panel>\n";

      // ---------------------- content start ----------------------------------
      o << "<content>\n";

      // ---------------------- big table ----------------------------------
      o <<
      "<table cellspacing=\"5\" cellpadding=\"5\">\n"
      "<tr>\n"
         "<th><h3>Original MIDI</h3></th>"
         "<th><h3>Parsed and recombined MIDI</h3></th>"
      "</tr>\n"
      "<tr>\n"
      "<td>\n";

      // ---------------------- original hex table ----------------------------------

      o <<
      "<table cellspacing=\"5\" cellpadding=\"5\">\n"
      "<tr>\n"
      "<th>Line</th>"
      "<th>Dec</th>"
      "<th>Hex Address</th>";

      size_t cols = 16; // hex bytes per row ( used for compare hex tables )

      for ( size_t i = 0; i < cols; ++i )
      {
         o << "<th>" << i << "</th>";
      }

      o << "</tr>\n";

      //size_t n = m_endAddress - m_startAddress;
      size_t lineCount = 0;
      size_t byteIndex = 0;
      size_t col = 0;

      auto beg = m_startAddress;
      auto end = m_endAddress;
      auto src = beg;

      o << "<tr>"
            "<th>" << lineCount<< "</th>"
            "<td>" << byteIndex << "</td>"
            "<td>0x" << hexStr( uint32_t(byteIndex) ) << "</td>";

      while ( src < end )
      {
         std::string byte = hexStr( *src++ );
         byteIndex++;
         col++;

         o << "<td>" << byte << "</td>";

         if ( col >= cols )
         {
            col -= cols;
            lineCount++;
            o <<
            "</tr>\n"
            "<tr>"
            "<th>" << lineCount<< "</th>"
            "<td>" << byteIndex << "</td>"
            "<td>0x" << hexStr( uint32_t(byteIndex) ) << "</td>";
         }
      }

      o <<
      "</tr>\n"
      "</table>\n";

   // ---------------------- big table ----------------------------------

      o <<
      "</td>"
      "<td>\n";

   // ---------------------- token hex table ----------------------------------
   {
      struct ColorByte
      {
         uint32_t m_fg;
         uint32_t m_bg;
         uint8_t m_byte;

         ColorByte()
            : m_fg( 0xFF000000 ), m_bg( 0xFFFFFFFF ), m_byte( 0 )
         {}

         ColorByte( uint8_t byte, uint32_t fg = 0xFF000000, uint32_t bg = 0xFFFFFFFF )
            : m_fg( fg ), m_bg( bg ), m_byte( byte )
         {}
      };

      std::vector< ColorByte > colorBytes;
      for ( size_t i = 0; i < m_tokens.size(); ++i )
      {
         auto const & token = m_tokens[ i ];
         auto const & bytes = token.m_data;
         for ( size_t j = 0; j < token.m_data.size(); ++j )
         {
            colorBytes.emplace_back( token.m_data[ j ], token.m_fg, token.m_bg );
         }
      }

      o <<
      "<table cellspacing=\"5\" cellpadding=\"5\">\n"
      "<tr>\n"
      "<th>Line</th>"
      "<th>Dec</th>"
      "<th>Hex Address</th>";

      for ( size_t i = 0; i < cols; ++i )
      {
         o << "<th>" << i << "</th>";
      }

      o << "</tr>\n";

      //size_t n = m_endAddress - m_startAddress;
      lineCount = 0;
      byteIndex = 0;
      col = 0;

      auto beg2 = colorBytes.data();
      auto end2 = colorBytes.data() + colorBytes.size();
      auto src2 = beg2;
      auto src = beg;

      o <<
      "<tr>"
      "<th>" << lineCount<< "</th>"
      "<td>" << byteIndex << "</td>"
      "<td>0x" << hexStr( uint32_t(byteIndex) ) << "</td>";

      while ( src < end && src2 < end2 )
      {
         ColorByte const & colorByte = *src2++;
         uint8_t oldByte = *src++;
         uint8_t newByte = colorByte.m_byte;
         std::string byte = hexStr( colorByte.m_byte );

         int r = de::RGBA_R( colorByte.m_fg );
         int g = de::RGBA_G( colorByte.m_fg );
         int b = de::RGBA_B( colorByte.m_fg );
         std::string color = dbStrJoin( "color:rgb(",r,",",g,",",b,"); " );

         // diff
         if ( oldByte != newByte)
         {
            color += dbStrJoin( "background-color:rgb(255,150,150); " );
         }

         auto style = dbStrJoin( " style=\"",color,"\"" );

         byteIndex++;
         col++;

         o << "<td " << style << ">" << byte << "</td>";

         if ( col >= cols )
         {
            col -= cols;
            lineCount++;
            o <<
            "</tr>\n"
            "<tr>"
            "<th>" << lineCount<< "</th>"
            "<td>" << byteIndex << "</td>"
            "<td>0x" << hexStr( uint32_t(byteIndex) ) << "</td>";
         }
      }

      o <<
      "</tr>\n"
      "</table>\n";
   }

   // ---------------------- big table end -------------------------------

      o <<
      "</td>\n"
      "</tr>\n"
      "</table>\n";

   // ---------------------------- print colored midiTokens ----------------------------------

      o <<
      "<table cellspacing=\"5\" cellpadding=\"5\">\n"
      "<tr>\n"
         "<th>Index</th>\n"
         "<th>Token Size</th>\n"
         "<th>Address</th>\n"
         "<th>Token Data</th>\n"
         "<th>Midi Parser Comments</th>\n"
      "</tr>\n";

      for ( size_t i = 0; i < m_tokens.size(); ++i )
      {
         auto const & t = m_tokens[ i ];

         //DE_DEBUG( "Token[",i,"] ", t.toString(), " :: ", t.m_comment )

         int r = de::RGBA_R( t.m_fg );
         int g = de::RGBA_G( t.m_fg );
         int b = de::RGBA_B( t.m_fg );

         auto css = dbStrJoin( " style=\"color:rgb(",r,",",g,",",b,")\"" );

         o <<
        "<tr>\n"
         "<td" << css << " align=\"right\">" << i << "</td>"
         "<td" << css << " align=\"left\">" << t.size() << "</td>"
         "<td" << css << " align=\"left\">" << t.addressString() << "</td>"
         "<td" << css << " align=\"left\"><pre>" << t.dataString( 8 ) << "</pre></td>"
         "<td" << css << " align=\"left\">" << t.m_comment << "</td>"
        "</tr>\n";
      }

      // ---------------------------- end content ----------------------------------

      o <<
      "</table>\n"
      "<br />"
      "<br />"
      "<br />"
      "<br />"
      "<br />"
      "</content>\n"

      // ---------------------------- footer ----------------------------------

      "<footer style=\"background-color:blue; color:white; position:fixed; bottom:0px; left:0px; width:100%; height:50px; line-height:50px; z-index:3;\">\n"
      "<table width=\"100%\" height=\"100%\">\n"
      "<tr valign=\"middle\"><td align=\"center\"><h3 style=\"color:white;\">log-file produced by class HtmlWriter</h3></td></tr>\n"
      "<tr valign=\"middle\"><td align=\"center\"><h6 style=\"color:white;\">(c) 2022 by BenjaminHampe@gmx.de</h6></td></tr>\n"
      "</table>\n"
      "</footer>\n"
      "</body>\n"
      "</html>\n";

      std::ofstream file( saveUri.c_str() );
      if ( file.is_open() )
      {
         std::string htmlStr = o.str();
         file << htmlStr;
         file.close();

         DE_DEBUG("Saved HTML ", saveUri, " with ", htmlStr.size(), " bytes.")
      }
   }


};


} // end namespace midi
} // end namespace de

#endif
