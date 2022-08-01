#ifndef DE_MIDI_MESSAGE_HPP
#define DE_MIDI_MESSAGE_HPP

#include <de/Logger.hpp>
#include <de/midi/MidiDefines.hpp>
#include <de/midi/InputStream.hpp>
#include <fstream>
#include <iomanip>

#include <DarkImage.hpp>

// SMF Syntax - Standard Midi file syntax. ( Aufs Byte genau leider )
// <descriptor:length> means 'length' bytes, MSB first
// <descriptor:v> means variable length argument (special format)
// SMF := <header_chunk> + <track_chunk> [ + <track_chunk> ... ]
// header chunk := "MThd" + <header_length:4> + <format:2> + <num_tracks:2> + <time_division:2>
// track_chunk := "MTrk" + <length:4> + <track_event> [ + <track_event> ... ]
// track_event := <time:v> + [ <midi_event> | <meta_event> | <sysex_event> ]
// midi_event := any MIDI channel message, including running status
// meta_event := 0xFF + <meta_type:1> + <length:v> + <event_data_bytes>
// sysex_event := 0xF0 + <len:1> + <data_bytes> + 0xF7
// sysex_event := 0xF7 + <len:1> + <data_bytes> + 0xF7

namespace de {
namespace midi {

/*
// =======================================================================
struct MidiMessage
// =======================================================================
{
   uint32_t m_ppqBeg;
   uint32_t m_ppqLen;
   std::vector< uint8_t > m_data;

   MidiMessage()
         : m_ppqBeg( 0 )
         , m_ppqLen( 0 )
   {}

   MidiMessage( uint32_t ppqBeg, uint32_t ppqLen, uint8_t const* beg, uint8_t const* end )
         : m_ppqBeg( ppqBeg )
         , m_ppqLen( ppqLen )
   {
      m_data.assign( beg, end );
   }

   size_t
   size() const { return m_data.size(); }

   int
   getCommand() const { m_data.empty() ? 0 : m_data[ 0 ] & 0xF0; }

   int
   getChannel() const { m_data.empty() ? 0 : m_data[ 0 ] & 0x0F; }

   int
   isCC() const { return getCommand() == 0xB0; }

   int
   isNoteOn() const
   {
      return getCommand() == 0x90 && m_data.
   }
};
*/

} // end namespace midi
} // end namespace de


#endif









/*
// =======================================================================
struct MidiTokenListener : public IMidiParserListener
// =======================================================================
{
   DE_CREATE_LOGGER("de.MidiTokenListener")

   MidiTokenListener() {}

   void
   clear() { m_tokens.clear(); }

   size_t
   countTokens() const { return m_tokens.size(); }

   size_t
   countBytes() const
   {
      size_t n = 0;
      for ( auto const & t : m_tokens )
      {
         n += t.size();
      }
      return n;
   }

   size_t
   maxDataString() const
   {
      size_t n = 0;
      for ( auto const & t : m_tokens )
      {
         n = std::max( n, t.toString().size() );
      }
      return n;
   }

   size_t
   maxCommentString() const
   {
      size_t n = 0;
      for ( auto const & t : m_tokens )
      {
         n = std::max( n, t.comment.size() );
      }
      return n;
   }


   std::string
   spaces( size_t n ) const
   {
      std::stringstream s;
      for ( size_t i = 0; i < n; ++i )
      {
         s << " ";
      }
      return s.str();
   }

   virtual void
   onParseStart( uint8_t const* beg, uint8_t const* end )
   {

   }

   virtual void
   onParsedToken( uint8_t const* beg, uint8_t const* end,
                    std::string const & comment = "" ) override
   {
      if ( beg >= end ) return;
      m_tokens.emplace_back();
      MidiToken & token = m_tokens.back();
      token.m_address = uint64_t( beg );
      token.m_data.assign( beg, end );
      token.comment = comment;
   }

   std::string
   toString() const
   {
      std::stringstream s;
      s << "TokenCount = " << countTokens() << " with " << countBytes() << " byte(s)\n";
      //auto m = maxDataString();
      for ( uint32_t i = 0; i < m_tokens.size(); ++i )
      {
         auto const & t = m_tokens[ i ];
         auto const d = t.toString();
         auto const c = d.size();
         auto const p = " \t=>\t "; // spaces( 1 + m - c );
         s << dbStrJoin("Token[",i,"] ",d,p,t.comment,'\n');
      }

      return s.str();
   }

   inline void
   dumpTokens()
   {
      DE_DEBUG( toString() )
   }

   std::vector< MidiToken > m_tokens;
};
*/
