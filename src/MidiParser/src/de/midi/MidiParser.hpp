#ifndef DE_MIDI_FILE_PARSER_HPP
#define DE_MIDI_FILE_PARSER_HPP

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

// =======================================================================
struct MidiTools
// =======================================================================
{
   DE_CREATE_LOGGER("de.MidiTools")

   //
   // Convert a MIDI keynote to a string
   //

   static std::string
   noteStr( int key )
   {
      std::stringstream s;

      int semi = key % 12;
      int octave = key / 12 - 1;
      switch ( semi )
      {
         case  0: s << "C";  break;
         case  1: s << "C#"; break;
         case  2: s << "D";  break;
         case  3: s << "D#"; break;
         case  4: s << "E";  break;
         case  5: s << "F";  break;
         case  6: s << "F#"; break;
         case  7: s << "G";  break;
         case  8: s << "G#"; break;
         case  9: s << "A";  break;
         case 10: s << "A#"; break;
         case 11: s << "B";  break;
      }
      s << octave;

      return s.str();
   }

   inline static size_t
   readU16( uint8_t const* beg, uint8_t const* end, uint16_t & value ) noexcept
   {
      if ( end - beg < 2 ) { return 0; }
      auto src = beg;
      uint_fast16_t r = *src++;
      uint_fast16_t g = *src++;
      value = (g << 8) | r;
      return src - beg;
   }

   inline static size_t
   readU16_be( uint8_t const* beg, uint8_t const* end, uint16_t & value ) noexcept
   {
      if ( end - beg < 2 ) { return 0; }
      auto src = beg;
      uint_fast16_t r = *src++;
      uint_fast16_t g = *src++;
      value = (r << 8) | g;
      return src - beg;
   }

   inline static size_t
   readU32( uint8_t const* beg, uint8_t const* end, uint32_t & value ) noexcept
   {
      if ( end - beg < 2 ) { return 0; }
      auto src = beg;
      uint_fast32_t r = *src++;
      uint_fast32_t g = *src++;
      uint_fast32_t b = *src++;
      uint_fast32_t a = *src++;
      value = (a << 24) | (b << 8) | (g << 8) | r;
      return src - beg;
   }

   inline static size_t
   readU32_be( uint8_t const* beg, uint8_t const* end, uint32_t & value ) noexcept
   {
      if ( end - beg < 2 ) { return 0; }
      auto src = beg;
      uint_fast32_t r = *src++;
      uint_fast32_t g = *src++;
      uint_fast32_t b = *src++;
      uint_fast32_t a = *src++;
      value = (r << 24) | (g << 8) | (b << 8) | a;
      return src - beg;
   }

   //
   // VLQ - Variable-Length Quantity
   //

   static uint32_t
   parseVLQ( uint8_t const* beg, uint8_t const* end, uint32_t & value )
   {
      if ( beg == end ) return 0;
      auto it = beg;     // Store original pointer to compute delta.
      uint8_t c = *it++; // Read 1st byte, hopefully the last.

      uint32_t r = c & 0x7F;
      while ( c >= 0x80 && ( it != end ) )
      {
         c = *it++;      // Read next byte, hopefully the last.
         r = ( r << 7 ) | ( 0x7F & c );
      }

      //DE_DEBUG("[",hexStr(uint64_t(src)),"] Parsed VLQ = ",retVal," after ",n," bytes")
      value = r;
      return it - beg;
   }

   static void
   testVLQ( int k, std::vector< uint8_t > dat, uint32_t n, uint32_t expectedBytes, uint32_t expectedValue )
   {
      uint8_t const* beg = dat.data();
      uint8_t const* end = dat.data() + dat.size();
      uint32_t returnedValue = 0;
      uint32_t returnedBytes = parseVLQ( beg, end, returnedValue );

      std::stringstream s;
      s << "Test[" << k << "] :: Data(" << dat.size() << ")";
      s << "[";
      for ( size_t i = 0; i < dat.size(); ++i )
      {
         s << "0x" << hexStr( dat[i] );
         if ( i < dat.size() - 1 )
         {
            s << ", ";
         }
      }
      s << "] ";
      s << "expect( " << expectedBytes << "B, " << hexStr(expectedValue) << " = " << expectedValue << " ), ";
      s << "return( " << returnedBytes << "B, " << hexStr(returnedValue) << " = " << returnedValue << " ), ";

      bool ok = true;
      ok &= (expectedBytes == returnedBytes);
      ok &= (expectedValue == returnedValue);

      if ( ok )
      {
         s << " OK";
         DE_DEBUG( s.str() )
      }
      else
      {
         s << " FAIL";
         DE_ERROR( s.str() )
         throw std::runtime_error( s.str() );
      }
   }

   // NUMBER   VARIABLE LENGTH QUANTITY
   // 00000000 00
   // 00000040 40
   // 0000007F 7F
   // 00000080 81 00
   // 00002000 C0 00
   // 00003FFF FF 7F
   // 00004000 81 80 00
   // 00100000 C0 80 00
   // 001FFFFF FF FF 7F
   // 00200000 81 80 80 00
   // 08000000 C0 80 80 00
   // 0FFFFFFF FF FF FF 7F

   static void test()
   {
      testVLQ( 1, { 0x00 }, 1, 1, 0 );
      testVLQ( 2, { 0x40 }, 1, 1, 64 );
      testVLQ( 3, { 0x7F }, 1, 1, 127 );

      testVLQ( 4, { 0x81, 0x00 }, 2, 2, 0x80 ); // 128
      testVLQ( 5, { 0xC0, 0x00 }, 2, 2, 0x2000 ); // 8*1024
      testVLQ( 6, { 0xFF, 0x7F }, 2, 2, 0x3FFF );

      testVLQ( 7, { 0x81, 0x80, 0x00 }, 3, 3, 0x00004000 );
      testVLQ( 8, { 0xC0, 0x80, 0x00 }, 3, 3, 0x00100000 );
      testVLQ( 9, { 0xFF, 0xFF, 0x7F }, 3, 3, 0x001FFFFF );

      testVLQ( 10, { 0x81, 0x80, 0x80, 0x00 }, 4, 4, 0x00200000 );
      testVLQ( 11, { 0xC0, 0x80, 0x80, 0x00 }, 4, 4, 0x08000000 );
      testVLQ( 12, { 0xFF, 0xFF, 0xFF, 0x7F }, 4, 4, 0x0FFFFFFF );

      // Test with one more byte
      testVLQ( 13, { 0x00, 0x00 }, 2, 1, 0 );
      testVLQ( 14, { 0x40, 0x00 }, 2, 1, 64 );
      testVLQ( 15, { 0x7F, 0x00 }, 2, 1, 127 );

      testVLQ( 16, { 0x81, 0x00, 0x00 }, 3, 2, 0x80 ); // 128
      testVLQ( 17, { 0xC0, 0x00, 0x00 }, 3, 2, 0x2000 ); // 8*1024
      testVLQ( 18, { 0xFF, 0x7F, 0x00 }, 3, 2, 0x3FFF );

      testVLQ( 19, { 0x81, 0x80, 0x00, 0x00 }, 4, 3, 0x00004000 );
      testVLQ( 20, { 0xC0, 0x80, 0x00, 0x00 }, 4, 3, 0x00100000 );
      testVLQ( 21, { 0xFF, 0xFF, 0x7F, 0x00 }, 4, 3, 0x001FFFFF );

      testVLQ( 22, { 0x81, 0x80, 0x80, 0x00, 0x00 }, 5, 4, 0x00200000 );
      testVLQ( 23, { 0xC0, 0x80, 0x80, 0x00, 0x00 }, 5, 4, 0x08000000 );
      testVLQ( 24, { 0xFF, 0xFF, 0xFF, 0x7F, 0x00 }, 5, 4, 0x0FFFFFFF );
   }

   static uint32_t
   getChannelColor( int channel )
   {
      switch ( channel )
      {
         case 0: return 0xFF0000FF;    // red
         case 1: return 0xFF0080FF;    // orange
         case 2: return 0xFF00AF00;
         case 3: return 0xFFFFAF00;
         case 4: return 0xFFAF0000;
         case 5: return 0xFFFF0040;
         case 6: return 0xFF00FFFF;
         case 7: return 0xFFFFFFFF;
         case 8: return 0xFFFFC08F;
         case 9: return 0xFF000000;    // Drums = black
         case 10: return 0xFFFF00FF;
         case 11: return 0xFF00FF00;
         case 12: return 0xFF8F008F;
         case 13: return 0xFF3FF03F;
         case 14: return 0xFF7F40DF;
         case 15: return 0xFF3F808F;
         default: return 0xFFFFFFFF;
      }
   }


};


// =======================================================================
struct IMidiParserListener
// =======================================================================
{
   virtual ~IMidiParserListener() = default;

// I.:

   // Used for debugging ( diff analysis ), etc...
   virtual void
   mpDebugStart( uint8_t const* beg, uint8_t const* end, std::string const & uri ) {}

   // Used for advanced parsers, debug, diff analysis, etc...
   virtual void
   mpDebugToken( uint8_t const* beg, uint8_t const* end, std::string const & comment ) {}

// II.:

   virtual void
   mpTrack( uint8_t const* beg, uint8_t const* end ) {}

   virtual void
   mpNote( uint32_t pts, int channel, int midinote, int velocity ) {}

   virtual void
   mpProgramChange( uint32_t pts, int channel, int program ) {}

   virtual void
   mpPolyAftertouch( uint32_t pts, int channel, int value ) {}

   virtual void
   mpPitchBend( uint32_t pts, int channel, int value ) {}

   virtual void
   mpControlChange( uint32_t pts, int channel, int cc, int value ) {}

//   virtual void
//   mpBendWheel( uint32_t pts, int channel, int value ) {}

// III.: Even more Simple API

   virtual void
   mpSetTempo( uint32_t pts, float bpm ) {}

   // Always sent before a track event.
//   virtual void
//   mpClock( int deltaTime ) {}

//   virtual void
//   mpNoteOn( double pts, int channel, int midinote, int velocity ) {}

//   virtual void
//   mpNoteOff( double pts, int channel, int midinote, int velocity ) {}

   virtual void
   mpTimeSignature( uint32_t pts, int num, int den, int bpc, int n32rd_per_qn ) {}

/*
   // Get full tokens, Midi header, Track header, TrackEvent
   virtual void
   mpToken( uint8_t const* beg, uint8_t const* end ) {}

   virtual void
   mpTextEvent( int typ, std::string const & txt ) {}


   virtual void
   mpKeySignature( int accidental, int minor ) {}

   // Get all NoteOn 9x and NoteOff 8x events
   //
   virtual void
   mpNoteEvent( int trackIndex = 0,
                int channel = 0,
                int program = 0,
                int dt = 100,
                int midiNote = 60,
                bool noteOn = false,
                uint8_t velocity = 0 ) {}

   // Get all ProgramChange 8x events
   //
*/

//   virtual void
//   mpClockStart( int deltaTime ) {}


//   virtual void
//   mpChannelAfterTouch( int channel, int program ) {}
};


// =======================================================================
struct MidiTempo
// =======================================================================
{
   uint32_t m_ptsBeg = 0;
   uint32_t m_ptsEnd = 0;
   float m_bpm = 0;     // bar tempo
   double m_timeBeg = 0;
   double m_timeEnd = 0;
};

// =======================================================================
struct MidiTimeSignature
// =======================================================================
{
   uint32_t m_ptsBeg = 0;
   uint32_t m_ptsEnd = 0;
   int m_clockNum = 4;  // bar tempo - numerator
   int m_clockDen = 4;  // bar tempo - denominator
   int m_clocksPerBeat = 24;         // bar tempo - clocks per beat
   int m_32n = 8;          // bar tempo - 32nd per bar = 8ths per beat
   int m_upb = 646000;     // bar tempo - microseconds per beat
};

// =======================================================================
struct MidiTempoMap
// =======================================================================
{
   std::vector< MidiTempo > m_tempos;
   std::vector< MidiTimeSignature > m_timesigs;

   void addTimeSignature( uint32_t pts, int num, int den, int cpb, int n32rd_per_qn )
   {
      if ( m_timesigs.size() > 0 )
      {
         m_timesigs.back().m_ptsEnd = pts;
      }

      m_timesigs.emplace_back();
      auto & t = m_timesigs.back();
      t.m_ptsBeg = pts;
      t.m_ptsEnd = pts;
      t.m_clockNum = num;
      t.m_clockDen = den;
      t.m_clocksPerBeat = cpb;
      t.m_32n = n32rd_per_qn;
   }

   void addTempo( uint32_t pts, float bpm )
   {
      if ( m_tempos.size() > 0 )
      {
         m_tempos.back().m_ptsEnd = pts;
      }

      m_tempos.emplace_back();
      auto & t = m_tempos.back();
      t.m_ptsBeg = pts;
      t.m_ptsEnd = pts;
      t.m_bpm = bpm;
   }

};

// =======================================================================
struct MidiNote
// =======================================================================
{
   int m_channel = 0;
   int m_midinote = 0;
   int m_velocity = 0;
   uint32_t m_ppqBeg = 0;
   uint32_t m_ppqEnd = 0;
   uint32_t m_ticksPerQuarterNote = 96;
   uint32_t m_microsPerQuarterNote = 606000;
//   uint32_t m_noteNum = 1; // For a
//   uint32_t m_noteDen = 4; // quarter note in 4/4
//   int m_clockNum = 4;  // bar tempo - numerator
//   int m_clockDen = 4;  // bar tempo - denominator
//   float m_bpm = 0;     // bar tempo
//   int m_cpb = 24;         // bar tempo - clocks per beat
//   int m_32n = 8;          // bar tempo - 32nd per bar = 8ths per beat
//   int m_upb = 646000;     // bar tempo - microseconds per beat

   uint32_t duration() const { return m_ppqEnd - m_ppqBeg; }


   std::string
   toString() const
   {
      std::stringstream s;
      s << MidiTools::noteStr( m_midinote ) << ", "
            "ch(" << m_channel << "), "
            "velocity(" << m_velocity << "), "
            "ppqBeg(" << m_ppqBeg << "), "
            "ppqEnd(" << m_ppqEnd << "), "
            "duration(" << duration() << "), "
            "ticksPerQuarterNote(" << m_ticksPerQuarterNote << ")";

      return s.str();
   }

};

// =======================================================================
struct MidiTrackChannel
// =======================================================================
{
   int m_id = 0;     // bar tempo
   int m_channel = 0;
   int m_instrument = 0;
   uint32_t m_ppqBeg = 0;
   uint32_t m_ppqEnd = 0;
   std::vector< MidiNote > m_notes;

   uint32_t duration() const { return m_ppqEnd - m_ppqBeg; }

   std::string
   toString() const
   {
      std::stringstream s;
      s << "Channel(" << m_channel << "), "
            "instrument(" << EMidiGM_toString(m_instrument) << "), "
            "ppqBeg(" << m_ppqBeg << "), "
            "ppqEnd(" << m_ppqEnd << "), "
            "duration(" << duration() << "), "
            "notes(" << m_notes.size() << ")";

      for ( size_t i = 0; i < m_notes.size(); ++i )
      {
         s << "Ch[" << m_channel << "].Note[" << i << "] " << m_notes[i].toString() << "\n";
      }

      return s.str();
   }
};

// =======================================================================
struct MidiTrack
// =======================================================================
{
   int m_id = 0;
   int m_trackIndex = 0;
   int m_sequenceNumber = 0;
   int m_ticksPerQuarterNote = 0;
   std::string m_uri;
   std::string m_name;
   MidiTempoMap m_tempoMap;
   std::vector< MidiTrackChannel > m_channels;

   // Track Stats
   uint32_t m_ppqBeg = 0;
   uint32_t m_ppqEnd = 0;

   uint64_t m_nEvents = 0;
   uint64_t m_nMetaEvents = 0;
   uint64_t m_nNoteOn = 0;
   uint64_t m_nNoteOff = 0;
   uint64_t m_nSetTempo = 0;
   uint64_t m_nProgramChanges = 0;
   uint64_t m_nControlChanges = 0;
   uint64_t m_nSustainPedalCount = 0;
   uint64_t m_nSoftPedalCount = 0;

   void addNote( uint32_t ppq, int channel, int midinote, int velocity )
   {
      // New note
      if ( velocity > 0 )
      {
         m_nNoteOn++;

         // Add new note
         MidiTrackChannel & c = getChannel( channel );
         c.m_notes.emplace_back();
         MidiNote & note = c.m_notes.back();
         note.m_midinote = midinote;
         note.m_velocity = velocity;
         note.m_ppqBeg = ppq;
         note.m_ppqEnd = ppq;
      }
      // Stop note
      else
      {
         m_nNoteOff++;

         // Find last note and stop it.
         MidiNote* note = getLastNote( channel, midinote );
         if ( note )
         {
            note->m_ppqEnd = ppq;
         }
      }
   }


   void addPolyAftertouch( uint32_t pts, int channel, int value )
   {

   }

   void addPitchWheel( uint32_t pts, int channel, int value )
   {

   }

   void addBendWheel( uint32_t pts, int channel, int value )
   {

   }

   void addProgramChange( uint32_t pts, int channel, int program )
   {

   }


   void addControlChange( uint32_t pts, int channel, int cc, int value )
   {

   }

   void addTimeSignature( uint32_t pts, int num, int den, int cpb, int n32rd_per_qn )
   {
      m_tempoMap.addTimeSignature( pts, num, den, cpb, n32rd_per_qn );
   }

   void addTempo( uint32_t pts, float bpm, uint32_t usPerQN = 606000 )
   {
      m_tempoMap.addTempo( pts, bpm );
   }

   int
   findChannel( int channel ) const
   {
      for ( size_t i = 0; i < m_channels.size(); ++i )
      {
         if ( m_channels[ i ].m_channel == channel )
         {
            return int( i );
         }
      }
      return -1;
   }

   MidiTrackChannel &
   getChannel( int channel )
   {
      int found = findChannel( channel );
      if ( found > -1 )
      {
         return m_channels[ found ];
      }
      m_channels.emplace_back();
      auto & item = m_channels.back();
      item.m_channel = channel;
      item.m_instrument = 0;
      //item.m_tickBeg = m_currTick;
      return item;
   }

   int
   findLastNote( int channel, int midiNote )
   {
      MidiTrackChannel const & c = getChannel( channel );
      for ( size_t i = 0; i < c.m_notes.size(); ++i )
      {
         size_t k = c.m_notes.size() - 1 - i;
         if ( c.m_notes[ k ].m_midinote == midiNote )
         {
            return k;
         }
      }
      return -1;
   }

   MidiNote*
   getLastNote( int channel, int midiNote )
   {
      int found = findLastNote( channel, midiNote );
      if ( found > -1 )
      {
         return &getChannel( channel ).m_notes[ found ];
      }
      return nullptr;
   }

   std::string
   toString() const
   {
      std::stringstream s;
      s << "Track(" << m_trackIndex << "), "
           "#(" << m_sequenceNumber << ")"
            "ticksPerQuarterNote(" << m_ticksPerQuarterNote << "), "
            "m_uri(" << m_uri << "), "
            "m_name(" << m_name << "), "
            "m_tempoMap(" << m_tempoMap.m_tempos.size() << "), "
            "m_channels(" << m_channels.size() << ")";

      for ( size_t i = 0; i < m_channels.size(); ++i )
      {
         s << m_channels[i].toString() << "\n";
      }

      //   // Track Stats
      //   uint32_t m_nTicks = 0;
      //   uint64_t m_nEvents = 0;
      //   uint64_t m_nMetaEvents = 0;
      //   uint64_t m_nNoteOn = 0;
      //   uint64_t m_nNoteOff = 0;
      //   uint64_t m_nSetTempo = 0;
      //   uint64_t m_nProgramChanges = 0;
      //   uint64_t m_nControlChanges = 0;
      //   uint64_t m_nSustainPedalCount = 0;
      //   uint64_t m_nSoftPedalCount = 0;


      return s.str();
   }
};

// =======================================================================
struct MidiFile
// =======================================================================
{
   int m_id = 0;

   int m_fileFormat = 0;

   int m_trackCount = 0;

   int m_ticksPerQuarterNote = 0;

   std::string m_uri;

   std::string m_name;

   MidiTempoMap m_tempoMap;

   std::vector< MidiTrack > m_tracks;

   // File Stats
   uint32_t m_nMaxTicks = 0;
   uint64_t m_nEvents = 0;
   uint64_t m_nMetaEvents = 0;
   uint64_t m_nNoteOn = 0;
   uint64_t m_nNoteOff = 0;
   uint64_t m_nSetTempo = 0;
   uint64_t m_nProgramChanges = 0;
   uint64_t m_nControlChanges = 0;
   uint64_t m_nSustainPedalCount = 0;
   uint64_t m_nSoftPedalCount = 0;

   std::string
   toString() const
   {
      std::stringstream s;
      s << "MidiFile(" << m_uri << "), "
           "trackCount(" << m_trackCount << "), "
            "ticksPerQuarterNote(" << m_ticksPerQuarterNote << ")\n";

      for ( size_t i = 0; i < m_tracks.size(); ++i )
      {
         s << m_tracks[i].toString() << "\n";
      }

      //   // Track Stats
      //   uint32_t m_nTicks = 0;
      //   uint64_t m_nEvents = 0;
      //   uint64_t m_nMetaEvents = 0;
      //   uint64_t m_nNoteOn = 0;
      //   uint64_t m_nNoteOff = 0;
      //   uint64_t m_nSetTempo = 0;
      //   uint64_t m_nProgramChanges = 0;
      //   uint64_t m_nControlChanges = 0;
      //   uint64_t m_nSustainPedalCount = 0;
      //   uint64_t m_nSoftPedalCount = 0;


      return s.str();
   }
};

// =======================================================================
struct MidiParser
// =======================================================================
{
   DE_CREATE_LOGGER("de.MidiParser")
   std::vector< IMidiParserListener* > m_listeners;

   void addListener( IMidiParserListener* listener )
   {
      if ( !listener ) return;
      m_listeners.emplace_back( listener );
   }

   void emitDebugStart( uint8_t const* beg, uint8_t const* end, std::string const & uri )
   {
      for ( auto listener : m_listeners )
      {
         if ( listener ) listener->mpDebugStart( beg, end, uri );
      }
   }

   void emitTrack( uint8_t const* beg, uint8_t const* end )
   {
      for ( auto listener : m_listeners )
      {
         if ( listener ) listener->mpTrack( beg, end );
      }
   }

   void emitToken( uint8_t const* beg, uint8_t const* end, std::string const & comment )
   {
      for ( auto listener : m_listeners )
      {
         if ( listener ) listener->mpDebugToken( beg, end, comment );
      }
   }

   void emitNote( uint32_t pts, int channel, int midinote, int velocity )
   {
      m_curTrack.addNote( m_currTick, channel, midinote, velocity );
      for ( auto listener : m_listeners )
      {
         if ( listener ) listener->mpNote( pts, channel, midinote, velocity );
      }
   }

   void emitPolyAftertouch( uint32_t pts, int channel, int value )
   {
      m_curTrack.addPolyAftertouch( m_currTick, channel, value );
      for ( auto listener : m_listeners )
      {
         if ( listener ) listener->mpPolyAftertouch( pts, channel, value );
      }
   }

   void emitPitchBend( uint32_t pts, int channel, int value )
   {
      m_curTrack.addPitchWheel( m_currTick, channel, value );
      for ( auto listener : m_listeners )
      {
         if ( listener ) listener->mpPitchBend( pts, channel, value );
      }
   }

//   void emitBendWheel( uint32_t pts, int channel, int value )
//   {
//      m_curTrack.addBendWheel( m_currTick, channel, value );
//      for ( auto listener : m_listeners )
//      {
//         if ( listener ) listener->mpBendWheel( pts, channel, value );
//      }
//   }

   void emitProgramChange( uint32_t pts, int channel, int program )
   {
      m_curTrack.addProgramChange( pts, channel, program );
      for ( auto listener : m_listeners )
      {
         if ( listener ) listener->mpProgramChange( pts, channel, program );
      }
   }

   void emitControlChange( uint32_t pts, int channel, int cc, int value )
   {
      m_curTrack.addControlChange( pts, channel, cc, value );
      if ( cc == CC_64_SustainPedal )
      {
         m_curTrack.m_nSustainPedalCount++;
      }
      if ( cc == CC_67_SoftPedal )
      {
         m_curTrack.m_nSoftPedalCount++;
      }

      for ( auto listener : m_listeners )
      {
         if ( listener ) listener->mpControlChange( pts, channel, cc, value );
      }
   }

   void emitTimeSignature( uint32_t pts, int num, int den, int bpc, int n32rd_per_qn )
   {
      m_curTrack.addTimeSignature( pts, num, den, bpc, n32rd_per_qn );
      for ( auto listener : m_listeners )
      {
         if ( listener ) listener->mpTimeSignature( pts, num, den, bpc, n32rd_per_qn );
      }
   }
   //0-the file contains a single multi-channel track
   //1-the file contains one or more simultaneous tracks (or MIDI outputs) of a sequence
   //2-the file contains one or more sequentially independent single-track patterns
   uint16_t m_fmt;
   // <ntrks> the number of tracks. Always 1 for a format 0 file.
   uint16_t m_trackCount;
   //15| bits 14 thru 8            | bits 7 thru 0
   //F | E | D | C | B | A | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
   //0 | ticks per quarter-note bits 14 - 0 = 0...2^14 = 16 * 1024 |
   //1 | negative SMPTE format     | ticks per frame               |
   uint16_t m_resolution = 0;// ticks per quarter-note

   uint16_t m_trackIndex = 0;

   uint8_t m_runStatus = 0;

   uint32_t m_ticksPerQuarterNote = 96;

   uint64_t m_currTick = 0;
   uint32_t m_currChannel = 0;
   uint32_t m_currProgram = 0;
   float m_currBpm = 0;  // bar tempo
   int m_currTempoNum = 4;            // bar tempo - numerator
   int m_currTempoDen = 4;            // bar tempo - denominator
   int m_currTempoCPB = 24;         // bar tempo - clocks per beat
   int m_currTempo32n = 8;          // bar tempo - 32nd per bar = 8ths per beat
   int m_currTempoUPB = 646000;     // bar tempo - microseconds per beat


   MidiFile m_curFile;
   MidiTrack m_curTrack;


   std::string m_uri;

   MidiParser();
   ~MidiParser();

   void
   reset();

   uint32_t
   parse( uint8_t const* beg, uint8_t const* end, std::string const & uri );

   // Must always return 14 bytes.
   uint32_t
   parseFileHeader( uint8_t const* beg, uint8_t const* end );

   // Must always return 8 bytes.
   uint32_t
   parseTrackHeader( uint8_t const* beg, uint8_t const* end, uint32_t & dataSize );

   uint64_t
   parseTrackData( uint8_t const* const beg, uint8_t const* const end );

   uint32_t
   parseEvent( uint8_t const* beg, uint8_t const* end );

   uint32_t
   parseMetaEvent(
      uint8_t const* evt, // Event start ( time ), needed for full token extract
      uint8_t const* beg, // Current pos (after reading byte metatype).
      uint8_t const* end, // End of string
      uint32_t deltaTime, // Already parsed dt injected here
      uint8_t metatype    // Already parsed metatype injected here
   );

   std::string
   parseMetaText( uint8_t const* beg, uint8_t const* end );

//   bool
//   loadFromFile( std::string uri )
//   {
//      return m_file.loadFromFile( uri );
//   }

//   bool
//   loadFromData( uint8_t const* src, uint64_t nBytes )
//   {
//      return m_file.loadFromData( src, nBytes );
//   }

//   static MidiParser
//   createFromFile( std::string uri )
//   {
//      MidiParser m;
//      m.loadFromFile( uri );
//      m.parse();
//      return m;
//   }

//   static MidiParser
//   createFromData( uint8_t const* src, uint64_t nBytes )
//   {
//      MidiParser m;
//      m.loadFromData( src, nBytes );
//      m.parse();
//      return m;
//   }

};


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
