#ifndef DE_AUDIO_RTMIDI_ENGINE_HPP
#define DE_AUDIO_RTMIDI_ENGINE_HPP

#include <de/midi/MIDI_TOOLS.hpp>

struct RtMidiEngine
{
   DE_CREATE_LOGGER("RtMidiEngine")

   RtMidiIn* m_MidiIn;
   RtMidiOut* m_MidiOut;

   static void
   handleInput( double pts, std::vector< unsigned char >* msg, void* userData )
   {
      if ( !msg )
      {
         DE_ERROR("No msg")
         return;
      }

      RtMidiEngine* engine = (RtMidiEngine*)userData;
      if ( !engine )
      {
         DE_ERROR("No engine")
         return;
      }

      engine->processInput( pts, *msg );
   }

   void processInput( double pts, std::vector<unsigned char> const & msg )
   {
      DE_DEBUG("pts = ", pts, ", msg.size() = ", msg.size())
   }


   RtMidiEngine()
      : m_MidiIn( nullptr ), m_MidiOut( nullptr )
   {
      m_MidiIn = RtMidiUtils::createMidiIn();
      m_MidiOut = RtMidiUtils::createMidiOut();

      if ( m_MidiOut )
      {
         // Open first available port.
         m_MidiOut->openPort( 0 );

         // Send MIDI messages:

         // Program change: 192, 5
         RtMidiUtils::send2Bytes( m_MidiOut, 192, 5 );

         // Control Change: 176, 7, 100 (volume)
         RtMidiUtils::send3Bytes( m_MidiOut, 176, 7, 30 );
      }

      RtMidiUtils::dump();
   }

   ~RtMidiEngine()
   {
      if ( m_MidiIn ) { delete m_MidiIn; m_MidiIn = nullptr; }
      if ( m_MidiOut ) { delete m_MidiOut; m_MidiOut = nullptr; }
   }

   void allNotesOff()
   {
      RtMidiUtils::allNotesOff( m_MidiOut );
   }

   void noteOff( uint8_t key, uint8_t vel = 80, uint8_t channel = 0 )
   {
      RtMidiUtils::noteOff( m_MidiOut, key, vel, channel );
   }

   void noteOn( uint8_t key, uint8_t vel = 80, uint8_t channel = 0 )
   {
      RtMidiUtils::noteOn( m_MidiOut, key, vel, channel );
   }


};

#endif
