#ifndef DE_AUDIO_RTMIDI_UTILS_HPP
#define DE_AUDIO_RTMIDI_UTILS_HPP

#include <DarkImage.hpp>

// #ifdef USE_LIBRTMIDI
#include <cstdint>
#include <iostream>

#include <RtMidi/RtMidi.h>


struct RtMidiUtils
{
   static RtMidiIn* createMidiIn()
   {
      RtMidiIn* ptr = nullptr;
      try
      {
         ptr = new RtMidiIn();
      }
      catch ( RtMidiError & error )
      {
         error.printMessage();
         ptr = nullptr;
      }
      return ptr;
   }

   static RtMidiOut* createMidiOut()
   {
      RtMidiOut* ptr = nullptr;
      try
      {
         ptr = new RtMidiOut();
      }
      catch ( RtMidiError &error )
      {
         error.printMessage();
         ptr = nullptr;
      }
      return ptr;
   }


   static void dumpInputs()
   {
      std::cout << "===============================================\n";
      std::cout << "MIDI input dump\n";
      std::cout << "===============================================\n";

      RtMidiIn* midiin = RtMidiUtils::createMidiIn();
      if ( !midiin )
      {
         std::cout << "No MIDI inputs\n";
         return;
      }

      auto nPorts = midiin->getPortCount();
      std::cout << "MIDI inputs = " << nPorts << "\n";
      std::string portName;
      for ( uint32_t i=0; i<nPorts; i++ )
      {
         try
         {
            portName = midiin->getPortName(i);
         }
         catch ( RtMidiError &error )
         {
            error.printMessage();
         }
         std::cout << "MIDI.input[" << i << "] " << portName << '\n';
      }
      delete midiin;
   }

   static void dumpOutputs()
   {
      std::cout << "===============================================\n";
      std::cout << "MIDI output dump\n";
      std::cout << "===============================================\n";

      RtMidiOut* midiout = RtMidiUtils::createMidiOut();
      if ( !midiout )
      {
         std::cout << "No MIDI output found.\n";
         return;
      }

      auto nPorts = midiout->getPortCount();
      std::cout << "MIDI outputs = " << nPorts << "\n";
      std::string portName;
      for ( unsigned int i=0; i<nPorts; i++ )
      {
         try
         {
            portName = midiout->getPortName(i);
         }
         catch (RtMidiError &error)
         {
            error.printMessage();
         }
         std::cout << "MIDI.output[" << i << "] " << portName << '\n';
      }
      delete midiout;
   }

   static void dump()
   {
      dumpInputs();
      dumpOutputs();
   }

   enum EPianoKey
   {
      Key_C = 0,                       // 0
      Key_Db, // = C# = Des oder Cis   // 1
      Key_D,                           // 2
      Key_Eb, // = D# = Es oder Dis    // 3
      Key_E,                           // 4
      Key_F,                           // 5
      Key_Gb, // = F# = Ges oder Fis   // 6
      Key_G,                           // 7
      Key_Ab, // = G# = As oder Gis    // 8
      Key_A,                           // 9
      Key_B,  // = A# = B|b oder Ais   // 10
      Key_H,                           // 11
      EPianoKeyCount                   // 12
   };

   static uint32_t toMidi( EPianoKey key, int octave = 0 )
   {
      return 60 + uint32_t( key ) + octave * 12;
   }

   static void send1Byte( RtMidiOut* out, uint8_t r )
   {
      if ( !out ) return;
      std::vector< unsigned char > message { r };
      out->sendMessage( &message );
   }

   static void send2Bytes( RtMidiOut* out, uint8_t r, uint8_t g )
   {
      if ( !out ) return;
      std::vector< unsigned char > message { r,g };
      out->sendMessage( &message );
   }

   static void send3Bytes( RtMidiOut* out, uint8_t r, uint8_t g, uint8_t b = 0 )
   {
      if ( !out ) return;
      std::vector< unsigned char > message { r,g,b };
      out->sendMessage( &message );
   }

   static void noteOff( RtMidiOut* out, uint8_t key = 60, uint8_t vel = 80, uint8_t channel = 0 )
   {
      send3Bytes( out, 0x80 + channel, key, vel );
   }

   static void noteOn( RtMidiOut* out, uint8_t key = 60, uint8_t vel = 80, uint8_t channel = 0 )
   {
      send3Bytes( out, 0x90 + channel, key, vel );
   }

   static void noteOff( RtMidiOut* out, EPianoKey key, uint8_t octave, uint8_t vel = 80, uint8_t channel = 0 )
   {
      send3Bytes( out, 0x80 + channel, toMidi( key, octave ), vel );
   }

   static void noteOn( RtMidiOut* out, EPianoKey key, uint8_t octave, uint8_t vel = 80, uint8_t channel = 0 )
   {
      send3Bytes( out, 0x90 + channel, toMidi( key, octave ), vel );
   }

   static void allNotesOff( RtMidiOut* out )
   {
      send3Bytes( out, 0x7B, 0, 0 );
   }


   // midiout.cpp
   static void testOutput()
   {
      dump();

      std::cout << "===============================================\n";
      std::cout << "MIDI OUTPUT TEST\n";
      std::cout << "===============================================\n";
      RtMidiOut* midiout = createMidiOut();
      if ( !midiout )
      {
         std::cout << "No MIDI output\n";
         return;
      }

      auto nPorts = midiout->getPortCount();
      if ( nPorts == 0 )
      {
         std::cout << "No midi output ports available!\n";
      }
      else
      {
         // takt 4/4
         // takt 500 / 4 = 125 ms / viertel note.
         uint32_t baseTime = 500 / 10;
         // Open first available port.
         midiout->openPort( 0 );

         // Send MIDI messages:

         // Program change: 192, 5
         send2Bytes( midiout, 192, 5 );

         // Control Change: 176, 7, 100 (volume)
         send3Bytes( midiout, 176, 7, 30 );

         for ( int i = 0; i < 4; ++i )
         {
            // Note On: C2
            noteOn( midiout, 48, 90 );
            dbSleep( 2*baseTime );
            noteOn( midiout, 48, 90 );
            dbSleep( 2*baseTime );

            // Note On:
            noteOn( midiout, 55, 90 );
            dbSleep( 2*baseTime );
            noteOn( midiout, 55, 90 );
            dbSleep( 2*baseTime );

            // Note On: C3
            noteOn( midiout, 60, 90 );
            dbSleep( 2*baseTime );
            noteOn( midiout, 60, 90 );
            dbSleep( 2*baseTime );

            // Note on:
            noteOn( midiout, 67, 90 );
            dbSleep( 2*baseTime );
            noteOn( midiout, 67, 90 );
            dbSleep( 2*baseTime );

            // Note on: C4
            noteOn( midiout, 72, 90 );
            dbSleep( 2*baseTime );
            noteOn( midiout, 72, 90 );
            dbSleep( 2*baseTime );

            // Note on:
            noteOn( midiout, 67, 90 );
            dbSleep( 2*baseTime );
            noteOn( midiout, 67, 90 );
            dbSleep( 2*baseTime );
         }

         allNotesOff( midiout );
      }
      delete midiout;
   }


};





#endif
