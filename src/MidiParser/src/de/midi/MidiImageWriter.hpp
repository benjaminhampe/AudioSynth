#ifndef DE_MIDI_IMAGE_WRITER_HPP
#define DE_MIDI_IMAGE_WRITER_HPP

#include <de/midi/MidiParser.hpp>
#include <DarkImage.hpp>

namespace de {
namespace midi {

// =======================================================================
struct MidiImageWriter : public IMidiParserListener
// =======================================================================
{
   DE_CREATE_LOGGER("de.MidiImageWriter")

   struct Note
   {
      int m_channel = 0;
      int m_midinote = 0;
      int m_velocity = 0;
      uint32_t m_ptsBeg = 0;
      uint32_t m_ptsEnd = 0;

      uint32_t tickDuration() const { return m_ptsEnd - m_ptsBeg; }

      float m_bpm = 0;  // bar tempo
      int m_num = 1;            // bar tempo - numerator
      int m_den = 1;            // bar tempo - denominator
      int m_cpb = 24;         // bar tempo - clocks per beat
      int m_32n = 8;          // bar tempo - 32nd per bar = 8ths per beat
      int m_upb = 646000;     // bar tempo - microseconds per beat
   };

//   struct Bar // Ein Takt mit eigenem Tempo, wie gehen Wiederholungen?
//   {
//      int m_id = 0;     // bar tempo
//      float m_bpm = 0;  // bar tempo
//      int m_z = 1;            // bar tempo - numerator
//      int m_n = 1;            // bar tempo - denominator
//      int m_cpb = 24;         // bar tempo - clocks per beat
//      int m_32n = 8;          // bar tempo - 32nd per bar = 8ths per beat
//      int m_upb = 646000;     // bar tempo - microseconds per beat
//      uint32_t m_tickBeg = 0; // in 96th per beat
//      uint32_t m_tickEnd = 0; // in 96th per beat
//      uint32_t tickDuration() const { return m_tickEnd - m_tickBeg; }

//      int m_intrument = 0;
//      int m_channel = 0;
//      uint32_t m_color = 0xFFFF00FF;

//      std::vector< Note > m_notes;
//   };

   struct Channel
   {
      int m_channel = 0;
      int m_instrument = 0;

      int m_currNote = 0;
      int m_lastNote = 0;
      int m_velocity = 0;

      uint32_t tickBeg = 0; // Duration in
      uint32_t tickEnd = 0; // Duration in

      uint32_t color = 0xFFFF00FF;

      //std::vector< Bar > m_bars;
      std::vector< Note > m_notes;
   };

   uint32_t m_currTick = 0;
   uint32_t m_currChannel = 0;
   uint32_t m_currProgram = 0;
   float m_currBpm = 0;  // bar tempo
   int m_currTempoNum = 4;            // bar tempo - numerator
   int m_currTempoDen = 4;            // bar tempo - denominator
   int m_currTempoCPB = 24;         // bar tempo - clocks per beat
   int m_currTempo32n = 8;          // bar tempo - 32nd per bar = 8ths per beat
   int m_currTempoUPB = 646000;     // bar tempo - microseconds per beat

   //std::vector< Note > m_notes;

   std::vector< Channel > m_channels;
   //std::vector< Image > m_imgs;

   std::string m_uri;
   uint8_t const* m_beg;
   uint8_t const* m_end;
   Image m_img;
   int m_imgIndex = 0;

   // int m_pixelPerNote = 160;

   int m_x = 10;


   MidiImageWriter()
   {
      DE_DEBUG("Constructor")
      m_img.resize( 16000, 512 );
      m_img.fill( 0xFFFFFFFF );
   }

   ~MidiImageWriter()
   {
      dbSaveImage( m_img, m_uri + std::to_string( m_imgIndex++ ) + ".png" );
      DE_DEBUG("Destructor")
   }

// II.: More Simpler API

   void
   mpDebugStart( uint8_t const* beg, uint8_t const* end, std::string const & uri ) override
   {
      m_beg = beg;
      m_end = end;
      m_uri = uri;

      if ( m_uri.empty() )
      {
         m_uri = "MidiUnknown.mid";
      }
      //DE_DEBUG("NewTrack(",track,"), count(",count,"), dataSize(",dataSize,"), name(",name,")")
   }

   void
   mpSetTempo( uint32_t pts, float bpm ) override
   {
      m_currBpm = bpm;
      // find last bar

      //if ( m_)
      //m_bars
//      Bar bar;
//      bar.m_bpm = m_currBpm;
//      bar.m_channel = m_currChannel;

//      bar.m_tickBeg = m_currTick;
//      bar.m_tickEnd = m_currTick;
      //note.timeBeg = m_time;
      //note.timeEnd = m_time;

      //DE_DEBUG("t(",m_tick,"), ch(",m_channel,"), SetTempo(",bpm,")")
   }

   // Always sent before a track event.

   void
   mpProgramChange( uint32_t pts, int channel, int program ) override
   {
      m_currProgram = program;
      m_currChannel = channel;

      Channel & c = getChannel( channel );
      c.m_instrument = m_currProgram;
      //DE_DEBUG("t(",m_tick,"), ch(",m_channel,"), ProgramChange(",program,"), value(",value,")")
   }

   void
   mpControlChange( uint32_t pts, int channel, int cc, int value ) override
   {
      m_currChannel = channel;
      //ChannelData & data = getChannel( channel );
      //data.instrument = program;
      //DE_DEBUG("t(",m_tick,"), ch(",m_channel,"), CC(",cc,"), value(",value,")")
   }

   virtual void
   mpNote( uint32_t pts, int channel, int midinote, int velocity ) override
   {
      // New note
      if ( velocity > 0 )
      {
         Channel & c = getChannel( channel );
         c.m_notes.emplace_back();
         Note & note = c.m_notes.back();
         note.m_channel = channel;
         note.m_midinote = midinote;
         note.m_velocity = velocity;
         note.m_ptsBeg = pts;
         note.m_ptsEnd = pts;
         note.m_bpm = m_currBpm;      // bar tempo
         note.m_num = m_currTempoNum; // bar tempo - numerator
         note.m_den = m_currTempoDen; // bar tempo - denominator
         note.m_cpb = m_currTempoCPB; // bar tempo - clocks per beat
         note.m_32n = m_currTempo32n; // bar tempo - 32nd per bar = 8ths per beat
         note.m_upb = m_currTempoUPB; // bar tempo - microseconds per beat
      }
      // Stop note
      else
      {
         Note* note = getLastNote( channel, midinote );
         if ( note )
         {
            note->m_ptsEnd = pts;

            int nw = 12;
            int nh = 9;
            int x = m_x;
            int y = 230 - (note->m_midinote - 60) * nh;
            ImagePainter::drawCircle( m_img, Recti( x,y,nw,nh ), 0xFF00D200 );
            ImagePainter::drawCircleBorder( m_img, Recti( x,y,nw,nh ), 0xFF000000 );
            ImagePainter::drawLine( m_img, x,y-40,x,y+nh/2, 0xFF000000 );
            m_x += nw + 4;

         }

      }
   }

   static void
   test()
   {
      DE_DEBUG("Test")

      Image img;
      img.resize( 600, 600 );
      img.fill( 0xFF202020 );

      ImagePainter::drawCircle( img, Recti(1,1,13,8), 0xFFFF0000 );
      ImagePainter::drawCircleBorder( img, Recti(14,1,13,8), 0xFF00FFFF );

      ImagePainter::drawCircle( img, Recti(1,10,12,6), 0xFFFF00FF );
      ImagePainter::drawCircleBorder( img, Recti(13,10,12,6), 0xFF00FFFF );

      ImagePainter::drawCircle( img, Recti(1,20,11,7), 0xFFFF00FF );
      ImagePainter::drawCircleBorder( img, Recti(12,20,11,7), 0xFF00FFFF );
      dbSaveImage( img, "MidiImageWriter_Ellipse.png");
   }

   void
   save( std::string saveUri, std::string title, std::string mediaDir = "" )
   {
//      std::ostringstream o;
//      std::ofstream file( saveUri.c_str() );
//      if ( file.is_open() )
//      {
//         std::string htmlStr = o.str();
//         file << htmlStr;
//         file.close();

//         DE_DEBUG("Saved HTML ", saveUri, " with ", htmlStr.size(), " bytes.")
//      }
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

   Channel &
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
      Channel & c = getChannel( channel );
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

   Note*
   getLastNote( int channel, int midiNote )
   {
      int found = findLastNote( channel, midiNote );
      if ( found > -1 )
      {
         return &getChannel( channel ).m_notes[ found ];
      }
      return nullptr;
   }

/*

*/
};

} // end namespace midi
} // end namespace de

#endif
