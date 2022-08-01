#ifndef DE_MIDI_DOCS_HPP
#define DE_MIDI_DOCS_HPP

namespace de {
namespace midi {

} // end namespace midi
} // end namespace de

//////////////////////////////
//
// MidiFileWrite::writeVLValue -- write a number to the midifile
//    as a variable length value which segments a file into 7-bit
//    values.  Maximum size of aValue is 0x7fffffff
//

//void MidiFileWrite::writeVLValue(long aValue) {
//   uchar bytes[5];
//    bytes[0] = (uchar)((aValue >> 28) & 0x7f);    // most significant 5 bits
//    bytes[1] = (uchar)((aValue >> 21) & 0x7f);    // next largest 7 bits
//    bytes[2] = (uchar)((aValue >> 14) & 0x7f);
//    bytes[3] = (uchar)((aValue >> 7)  & 0x7f);
//    bytes[4] = (uchar)((aValue)       & 0x7f);    // least significant 7 bits

//   int start = 0;
//   while (start<5 && bytes[start] == 0)  start++;

//   for (int i=start; i<4; i++) {
//      writeRaw((uchar)(bytes[i] | 0x80));
//   }
//   writeRaw(bytes[4]);
//}

//////////////////////////////
//
// MidiFileWrite::close
//

//void MidiFileWrite::close(void) {
//   if (midifile == NULL) {
//      openQ = 0;
//      return;
//   }
//   writeRaw(0, 0xff, 0x2f, 0);       // end of track meta event

//   midifile->seekg(18);
//   midifile->writeBigEndian(trackSize);

//   midifile->close();

//   delete midifile;
//   midifile = NULL;
//   openQ = 0;
//}


//void MidiFileWrite::setup(const char* aFilename, int startTime) {
//   if (openQ) {
//      close();
//   }

//   if (midifile != NULL)  delete midifile;
//   midifile = new FileIO;
//   midifile->open(aFilename, ios::out);

//   // write the header chunk
//   *midifile << "MThd";                    // file identification: MIDI file
//   midifile->writeBigEndian(6L);          // size of header (always 6)
//   midifile->writeBigEndian((short)0);    // format: type 0;
//   midifile->writeBigEndian((short)0);    // num of tracks (always 0 for type 0)
//   midifile->writeBigEndian((short)1000); // divisions per quarter note


//   // write the track header
//   *midifile << "MTrk";
//   midifile->writeBigEndian(0xffffL);     // the track size which will
//                                          // be corrected with close()


//   // the midifile stream is now setup for writing
//   // track events

//   openQ = 1;

//   start();  // start can be called later and will behave well
//             // as long as no track events have been written
//}



// microseconds per tick = microseconds per quarter note / ticks per quarter note

// ticks = resolution * (1 / tempo) * 1000 * elapsed_time

// - resolution in ticks/beat (or equivalently ticks/Quarter note). This fixes the smallest time interval to be generated.
// - tempo in microseconds per beat, which determines how many ticks are generated in a set time interval.
// - elapsed_time which provides the fixed timebase for playing the midi events.

// ticks   ticks   beat   1000 us    ms
// ----- = ----- * ---- * ------- * ----
// time    beat     us       ms     time

// - time is the elapsed time between calls to the tick generator. This must be calculated by the tick generator based on the history of the previous call to the tick generator.
// - tempo is the tempo determined by the Set Tempo MIDI event. Note this event only deals in Quarter Notes.
// - resolution is held as TicksPerQuarterNote.

//>>> mido.parse([0x92, 0x10, 0x20])
//Message('note_on', channel=2, note=16, velocity=32, time=0)
//>>> mido.parse_all([0x92, 0x10, 0x20, 0x82, 0x10, 0x20])
//[Message('note_on', channel=2, note=16, velocity=32, time=0),
//Message('note_off', channel=2, note=16, velocity=32, time=0)]

// Toene Oktave Frequenzen
// ================================================================
//     0       1       2       3       4       5       6       7
// C   16,35   32,70   65,41   130,81  261,63  523,25  1046,50 2093,00
// C’  17,32   34,65   69,30   138,59  277,18  554,37  1108,74 2217,46
// D   18,35   36,71   73,42   146,83  293,66  587,33  1174,66 2349,32
// D’  19,45   38,89   77,78   155,56  311,13  622,25  1244,51 2489,02
// E   20,60   41,20   82,41   164,81  329,63  659,26  1328,51 2637,02
// F   21,83   43,65   87,31   174,61  349,23  698,46  1396,91 2793,83
// F’  23,12   46,25   92,50   185,00  369,99  739,99  1479,98 2959,96
// G   24,50   49,00   98,00   196,00  392,00  783,99  1567,98 3135,96
// G’  25,96   51,91   103,83  207,65  415,30  830,61  1661,22 3322,44
// A   27,50   55,00   110,00  220,00  440,00  880,00  1760,00 3520,00
// A’  29,14   58,27   116,54  233,08  466,16  923,33  1864,66 3729,31
// H   30,87   61,74   123,47  246,94  493,88  987,77  1975,53 3951,07

// MIDI Channel Messages:
// ================================================================
// Note Off                8n kk vv kk = key number (0-127) vv = velocity (0-127)
// Note On                 9n kk vv kk = key number (0-127) vv = velocity (1-127, 0 = note off)
// Polyphonic After Touch  An kk tt kk = key number (0-127) tt = after touch (0-127)
// Control Change          Bn cc xx cc = controller (0-121) xx = value (0-127)
// Channel Mode Local Ctl  Bn 7A xx xx = 0 (off), 127 (on)
// All Notes Off           Bn 7B 00
// Omni Mode Off           Bn 7C 00
// Omni Mode On            Bn 7D 00
// Mono Mode On            Bn 7E cc cc = number of channels
// Poly Mode On            Bn 7F 00
// Program Change          Cn pp pp = program (0-127)
// Channel After Touch     Dn tt tt = after touch (0-127)
// Pitch Wheel Change      En ll hh ll = low 7 bits (0-127) hh = high 7 bits (0-127)

// MIDI Defines
// ================================================================
// #define MIDI_NOTEOFF          0x80
// #define MIDI_NOTEON           0x90
// #define MIDI_POLY_AFTERTOUCH  0xA0
// #define MIDI_CC               0xB0
// #define MIDI_PROGRAM_CHANGE   0xC0
// #define MIDI_CAT              0xD0
// #define MIDI_PITCH_BEND       0xE0
// #define MIDI_NOTEOFF_MAX      0x8F
// #define MIDI_NOTEON_MAX       0x9F
// #define MIDI_PAT_MAX          0xAF
// #define MIDI_CC_MAX           0xBF
// #define MIDI_PROGRAM_CHANGE_MAX 0xCF
// #define MIDI_CAT_MAX          0xDF
// #define MIDI_PITCH_BEND_MAX   0xEF
// #define MIDI_CLOCK            0xF8
// #define MIDI_CLOCK_START      0xFA
// #define MIDI_CLOCK_CONTINUE   0xFB
// #define MIDI_CLOCK_STOP       0xFC
// #define MIDI_SYSEX_START      0xF0
// #define MIDI_SYSEX_END        0xF7
// #define MIDI_CHANNEL_BITS     0x0F
// #define MIDI_STATUS_BITS      0xF0

// static int findActualEventLength (const uint8* data, int maxBytes) noexcept
//    {
//        auto byte = (unsigned int) *data;

//        if (byte == 0xf0 || byte == 0xf7)
//        {
//            int i = 1;

//            while (i < maxBytes)
//                if (data[i++] == 0xf7)
//                    break;

//            return i;
//        }

//        if (byte == 0xff)
//        {
//            if (maxBytes == 1)
//                return 1;

//            const auto var = MidiMessage::readVariableLengthValue (data + 1, maxBytes - 1);
//            return jmin (maxBytes, var.value + 2 + var.bytesUsed);
//        }

//        if (byte >= 0x80)
//            return jmin (maxBytes, MidiMessage::getMessageLengthFromFirstByte ((uint8) byte));

//        return 0;
//    }


//////////////////////////////
//
// MidiFile::extractMidiData -- Extract MIDI data from input
//    stream.  Return value is 0 if failure; otherwise, returns 1.
//
/*
int MidiFile::extractMidiData(std::istream& input, std::vector<uchar>& array,
   uchar& runningCommand) {

   int character;
   uchar byte;
   array.clear();
   int runningQ;

   character = input.get();
   if (character == EOF) {
      std::cerr << "Error: unexpected end of file." << std::endl;
      return 0;
   } else {
      byte = (uchar)character;
   }

   if (byte < 0x80) {
      runningQ = 1;
      if (runningCommand == 0) {
         std::cerr << "Error: running command with no previous command" << std::endl;
         return 0;
      }
      if (runningCommand >= 0xf0) {
         std::cerr << "Error: running status not permitted with meta and sysex"
              << " event." << std::endl;
         std::cerr << "Byte is 0x" << std::hex << (int)byte << std::dec << std::endl;
         return 0;
      }
   } else {
      runningCommand = byte;
      runningQ = 0;
   }

   array.push_back(runningCommand);
   if (runningQ) {
      array.push_back(byte);
   }

   switch (runningCommand & 0xf0) {
      case 0x80:        // note off (2 more bytes)
      case 0x90:        // note on (2 more bytes)
      case 0xA0:        // aftertouch (2 more bytes)
      case 0xB0:        // cont. controller (2 more bytes)
      case 0xE0:        // pitch wheel (2 more bytes)
         byte = readByte(input);
         if (!status()) { return m_rwstatus; }
         if (byte > 0x7f) {
            std::cerr << "MIDI data byte too large: " << (int)byte << std::endl;
            m_rwstatus = false; return m_rwstatus;
         }
         array.push_back(byte);
         if (!runningQ) {
            byte = readByte(input);
            if (!status()) { return m_rwstatus; }
            if (byte > 0x7f) {
               std::cerr << "MIDI data byte too large: " << (int)byte << std::endl;
               m_rwstatus = false; return m_rwstatus;
            }
            array.push_back(byte);
         }
         break;
      case 0xC0:        // patch change (1 more byte)
      case 0xD0:        // channel pressure (1 more byte)
         if (!runningQ) {
            byte = readByte(input);
            if (!status()) { return m_rwstatus; }
            if (byte > 0x7f) {
               std::cerr << "MIDI data byte too large: " << (int)byte << std::endl;
               m_rwstatus = false; return m_rwstatus;
            }
            array.push_back(byte);
         }
         break;
      case 0xF0:
         switch (runningCommand) {
            case 0xff:                 // meta event
               {
               if (!runningQ) {
                  byte = readByte(input); // meta type
                  if (!status()) { return m_rwstatus; }
                  array.push_back(byte);
               }
               ulong length = 0;
               uchar byte1 = 0;
               uchar byte2 = 0;
               uchar byte3 = 0;
               uchar byte4 = 0;
               byte1 = readByte(input);
               if (!status()) { return m_rwstatus; }
               array.push_back(byte1);
               if (byte1 >= 0x80) {
                  byte2 = readByte(input);
                  if (!status()) { return m_rwstatus; }
                  array.push_back(byte2);
                  if (byte2 > 0x80) {
                     byte3 = readByte(input);
                     if (!status()) { return m_rwstatus; }
                     array.push_back(byte3);
                     if (byte3 >= 0x80) {
                        byte4 = readByte(input);
                        if (!status()) { return m_rwstatus; }
                        array.push_back(byte4);
                        if (byte4 >= 0x80) {
                           std::cerr << "Error: cannot handle large VLVs" << std::endl;
                           m_rwstatus = false; return m_rwstatus;
                        } else {
                           length = unpackVLV(byte1, byte2, byte3, byte4);
                           if (!m_rwstatus) { return m_rwstatus; }
                        }
                     } else {
                        length = unpackVLV(byte1, byte2, byte3);
                        if (!m_rwstatus) { return m_rwstatus; }
                     }
                  } else {
                     length = unpackVLV(byte1, byte2);
                     if (!m_rwstatus) { return m_rwstatus; }
                  }
               } else {
                  length = byte1;
               }
               for (int j=0; j<(int)length; j++) {
                  byte = readByte(input); // meta type
                  if (!status()) { return m_rwstatus; }
                  array.push_back(byte);
               }
               }
               break;

            // The 0xf0 and 0xf7 meta commands deal with system-exclusive
            // messages. 0xf0 is used to either start a message or to store
            // a complete message.  The 0xf0 is part of the outgoing MIDI
            // bytes.  The 0xf7 message is used to send arbitrary bytes,
            // typically the middle or ends of system exclusive messages.  The
            // 0xf7 byte at the start of the message is not part of the
            // outgoing raw MIDI bytes, but is kept in the MidiFile message
            // to indicate a raw MIDI byte message (typically a partial
            // system exclusive message).
            case 0xf7:   // Raw bytes. 0xf7 is not part of the raw
                         // bytes, but are included to indicate
                         // that this is a raw byte message.
            case 0xf0:   // System Exclusive message
               {         // (complete, or start of message).
               int length = (int)readVLValue(input);
               for (int i=0; i<length; i++) {
                  byte = readByte(input);
                  if (!status()) { return m_rwstatus; }
                  array.push_back(byte);
               }
               }
               break;

            // other "F" MIDI commands are not expected, but can be
            // handled here if they exist.
         }
         break;
      default:
         std::cout << "Error reading midifile" << std::endl;
         std::cout << "Command byte was " << (int)runningCommand << std::endl;
         return 0;
   }
   return 1;
}



//////////////////////////////
//
// MidiFile::readVLValue -- The VLV value is expected to be unpacked into
//   a 4-byte integer no greater than 0x0fffFFFF, so a VLV value up to
//   4-bytes in size (FF FF FF 7F) will only be considered.  Longer
//   VLV values are not allowed in standard MIDI files, so the extract
//   delta time would be truncated and the extra byte(s) will be parsed
//   incorrectly as a MIDI command.
//

ulong MidiFile::readVLValue(std::istream& input) {
   uchar b[5] = {0};

   for (int i=0; i<5; i++) {
      b[i] = readByte(input);
      if (!status()) { return m_rwstatus; }
      if (b[i] < 0x80) {
         break;
      }
   }

   return unpackVLV(b[0], b[1], b[2], b[3], b[4]);
}



//////////////////////////////
//
// MidiFile::unpackVLV -- converts a VLV value to an unsigned long value.
//     The bytes a, b, c, d, e are in big-endian order (the order they would
//     be read out of the MIDI file).
// default values: a = b = c = d = 0;
//

ulong MidiFile::unpackVLV(uchar a, uchar b, uchar c, uchar d, uchar e) {
   uchar bytes[5] = {a, b, c, d, e};
   int count = 0;
   while ((count < 5) && (bytes[count] > 0x7f)) {
      count++;
   }
   count++;
   if (count >= 6) {
      std::cerr << "VLV number is too large" << std::endl;
      m_rwstatus = false;
      return 0;
   }

   ulong output = 0;
   for (int i=0; i<count; i++) {
      output = output << 7;
      output = output | (bytes[i] & 0x7f);
   }

   return output;
}



//////////////////////////////
//
// MidiFile::writeVLValue -- write a number to the midifile
//    as a variable length value which segments a file into 7-bit
//    values and adds a contination bit to each.  Maximum size of input
//    aValue is 0x0FFFffff.
//

void MidiFile::writeVLValue(long aValue, std::vector<uchar>& outdata) {
   uchar bytes[4] = {0};

   if ((unsigned long)aValue >= (1 << 28)) {
      std::cerr << "Error: number too large to convert to VLV" << std::endl;
      aValue = 0x0FFFffff;
   }

   bytes[0] = (uchar)(((ulong)aValue >> 21) & 0x7f);  // most significant 7 bits
   bytes[1] = (uchar)(((ulong)aValue >> 14) & 0x7f);
   bytes[2] = (uchar)(((ulong)aValue >> 7)  & 0x7f);
   bytes[3] = (uchar)(((ulong)aValue)       & 0x7f);  // least significant 7 bits

   int start = 0;
   while ((start<4) && (bytes[start] == 0))  start++;

   for (int i=start; i<3; i++) {
      bytes[i] = bytes[i] | 0x80;
      outdata.push_back(bytes[i]);
   }
   outdata.push_back(bytes[3]);
}


//////////////////////////////
//
// MidiFile::setMillisecondTicks -- set the ticks per quarter note
//   value to milliseconds.  The format for this specification is
//   highest 8-bits: SMPTE Frame rate (as a negative 2's compliment value).
//   lowest 8-bits: divisions per frame (as a positive number).
//   for millisecond resolution, the SMPTE value is -25, and the
//   frame rate is 40 frame per division.  In hexadecimal, these
//   values are: -25 = 1110,0111 = 0xE7 and 40 = 0010,1000 = 0x28
//   So setting the ticks per quarter note value to 0xE728 will cause
//   delta times in the MIDI file to represent milliseconds.  Calling
//   this function will not change any exiting timestamps, it will
//   only change the meaning of the timestamps.
//

void MidiFile::setMillisecondTicks(void) {
   m_ticksPerQuarterNote = 0xE728;
}


//////////////////////////////
//
// MidiFile::addTimeSignature -- Add a time signature meta message
//      (meta #0x58).  The "bottom" parameter must be a power of two;
//      otherwise, it will be set to the next highest power of two.
//
// Default values:
//     clocksPerClick     == 24 (quarter note)
//     num32ndsPerQuarter ==  8 (8 32nds per quarter note)
//
// Time signature of 4/4 would be:
//    top    = 4
//    bottom = 4 (converted to 2 in the MIDI file for 2nd power of 2).
//    clocksPerClick = 24 (2 eighth notes based on num32ndsPerQuarter)
//    num32ndsPerQuarter = 8
//
// Time signature of 6/8 would be:
//    top    = 6
//    bottom = 8 (converted to 3 in the MIDI file for 3rd power of 2).
//    clocksPerClick = 36 (3 eighth notes based on num32ndsPerQuarter)
//    num32ndsPerQuarter = 8
//

MidiEvent* MidiFile::addTimeSignature(int aTrack, int aTick, int top, int bottom,
      int clocksPerClick, int num32ndsPerQuarter) {
   MidiEvent* me = new MidiEvent;
   me->makeTimeSignature(top, bottom, clocksPerClick, num32ndsPerQuarter);
   me->tick = aTick;
   m_events[aTrack]->push_back_no_copy(me);
   return me;
}



//////////////////////////////
//
// MidiFile::addCompoundTimeSignature -- Add a time signature meta message
//      (meta #0x58), where the clocksPerClick parameter is set to three
//      eighth notes for compount meters such as 6/8 which represents
//      two beats per measure.
//
// Default values:
//     clocksPerClick     == 36 (quarter note)
//     num32ndsPerQuarter ==  8 (8 32nds per quarter note)
//

MidiEvent* MidiFile::addCompoundTimeSignature(int aTrack, int aTick, int top,
      int bottom, int clocksPerClick, int num32ndsPerQuarter) {
   return addTimeSignature(aTrack, aTick, top, bottom, clocksPerClick,
      num32ndsPerQuarter);
}



//////////////////////////////
//
// MidiFile::makeVLV --  This function is used to create
//   size byte(s) for meta-messages.  If the size of the data
//   in the meta-message is greater than 127, then the size
//   should (?) be specified as a VLV.
//

int MidiFile::makeVLV(uchar *buffer, int number) {

   unsigned long value = (unsigned long)number;

   if (value >= (1 << 28)) {
      std::cerr << "Error: Meta-message size too large to handle" << std::endl;
      buffer[0] = 0;
      buffer[1] = 0;
      buffer[2] = 0;
      buffer[3] = 0;
      return 1;
   }

   buffer[0] = (value >> 21) & 0x7f;
   buffer[1] = (value >> 14) & 0x7f;
   buffer[2] = (value >>  7) & 0x7f;
   buffer[3] = (value >>  0) & 0x7f;

   int i;
   int flag = 0;
   int length = -1;
   for (i=0; i<3; i++) {
      if (buffer[i] != 0) {
         flag = 1;
      }
      if (flag) {
         buffer[i] |= 0x80;
      }
      if (length == -1 && buffer[i] >= 0x80) {
         length = 4-i;
      }
   }

   if (length == -1) {
      length = 1;
   }

   if (length < 4) {
      for (i=0; i<length; i++) {
         buffer[i] = buffer[4-length+i];
      }
   }

   return length;
}



//////////////////////////////
//
// MidiFile::addPitchBend -- convert  number in the range from -1 to +1
//     into two 7-bit numbers (smallest piece first)
//
//   -1.0 maps to 0 (0x0000)
//    0.0 maps to 8192 (0x2000 --> 0x40 0x00)
//   +1.0 maps to 16383 (0x3FFF --> 0x7F 0x7F)
//

MidiEvent* MidiFile::addPitchBend(int aTrack, int aTick, int aChannel, double amount) {
   m_timemapvalid = 0;
   amount += 1.0;
   int value = int(amount * 8192 + 0.5);

   // prevent any wrap-around in case of round-off errors
   if (value > 0x3fff) {
      value = 0x3fff;
   }
   if (value < 0) {
      value = 0;
   }

   int lsbint = 0x7f & value;
   int msbint = 0x7f & (value  >> 7);

   std::vector<uchar> mididata;
   mididata.resize(3);
   if (aChannel < 0) {
      aChannel = 0;
   } else if (aChannel > 15) {
      aChannel = 15;
   }
   mididata[0] = uchar(0xe0 | aChannel);
   mididata[1] = uchar(lsbint);
   mididata[2] = uchar(msbint);

   return addEvent(aTrack, aTick, mididata);
}



///////////////////////////////////////////////////////////////////////////
//
// RPN convenience functions:
//

//////////////////////////////
//
// MidiFile::setPitchBendRange -- Set the range for the min/max pitch bend
//   alteration of a note.  Default is 2.0 (meaning +/- 2 semitones from given pitch).
//   Fractional values are cents, so 2.5 means a range of two semitones plus 50 cents,
//   which is two semitones plus a quarter tone.
//

void MidiFile::setPitchBendRange(int aTrack, int aTick, int aChannel, double range) {
   if (range < 0.0) {
      range = -range;
   }
   if (range > 24.0) {
      std::cerr << "Warning: pitch bend range is too large: " << range << std::endl;
      std::cerr << "Setting to 24." << std::endl;
      range = 24.0;
   }
   int irange = int(range);
   int cents = int((range - irange) * 100.0 + 0.5);

   // Select pitch bend RPN:
   addController(aTrack, aTick, aChannel, 101, 0);  // RPN selector (byte 1)
   addController(aTrack, aTick, aChannel, 100, 0);  // RPN selector (byte 2)

   // Set the semitone range (will be +/-range above/below a note):
   addController(aTrack, aTick, aChannel,  6,  irange);  // coarse: number of semitones
   addController(aTrack, aTick, aChannel, 38,  cents);   // fine: cents (1/100ths of semitone)
}

//////////////////////////////
//
// MidiFile::getTicksPerQuarterNote -- returns the number of
//   time units that are supposed to occur during a quarternote.
//

int MidiFile::getTicksPerQuarterNote(void) const {
   if (m_ticksPerQuarterNote == 0xE728) {
      // this is a special case which is the SMPTE time code
      // setting for 25 frames a second with 40 subframes
      // which means one tick per millisecond.  When SMPTE is
      // being used, there is no real concept of the quarter note,
      // so presume 60 bpm as a simiplification here.
      // return 1000;
   }
   return m_ticksPerQuarterNote;
}

//
// Alias for getTicksPerQuarterNote:
//

int MidiFile::getTPQ(void) const {
   return getTicksPerQuarterNote();
}



//////////////////////////////
//
// MidiFile::readLittleEndian4Bytes -- Read four bytes which are in
//      little-endian order (smallest byte is first).  Then flip
//      the order of the bytes to create the return value.
//

ulong MidiFile::readLittleEndian4Bytes(std::istream& input) {
   uchar buffer[4] = {0};
   input.read((char*)buffer, 4);
   if (input.eof()) {
      std::cerr << "Error: unexpected end of file." << std::endl;
      return 0;
   }
   return buffer[3] | (buffer[2] << 8) | (buffer[1] << 16) | (buffer[0] << 24);
}



//////////////////////////////
//
// MidiFile::readLittleEndian2Bytes -- Read two bytes which are in
//       little-endian order (smallest byte is first).  Then flip
//       the order of the bytes to create the return value.
//

ushort MidiFile::readLittleEndian2Bytes(std::istream& input) {
   uchar buffer[2] = {0};
   input.read((char*)buffer, 2);
   if (input.eof()) {
      std::cerr << "Error: unexpected end of file." << std::endl;
      return 0;
   }
   return buffer[1] | (buffer[0] << 8);
}



//////////////////////////////
//
// MidiFile::readByte -- Read one byte from input stream.  Set
//     fail status error if there was a problem (calling function
//     has to check this status for an error after reading).
//

uchar MidiFile::readByte(std::istream& input) {
   uchar buffer[1] = {0};
   input.read((char*)buffer, 1);
   if (input.eof()) {
      std::cerr << "Error: unexpected end of file." << std::endl;
      m_rwstatus = false;
      return 0;
   }
   return buffer[0];
}



//////////////////////////////
//
// MidiFile::writeLittleEndianUShort --
//

std::ostream& MidiFile::writeLittleEndianUShort(std::ostream& out, ushort value) {
   union { char bytes[2]; ushort us; } data;
   data.us = value;
   out << data.bytes[0];
   out << data.bytes[1];
   return out;
}

*/

////==========================================================================
////Patch parameter CC numbers
////Osc
//#define PARAM_OSC_SINE_LEVEL 14
//#define PARAM_OSC_TRI_LEVEL 15
//#define PARAM_OSC_SAW_LEVEL 16
//#define PARAM_OSC_PULSE_LEVEL 17
//#define PARAM_OSC_PULSE_AMOUNT 3
//#define PARAM_OSC_SQUARE_LEVEL 18
//#define PARAM_OSC_SINE_NOTE 33
//#define PARAM_OSC_TRI_NOTE 34
//#define PARAM_OSC_SAW_NOTE 35
//#define PARAM_OSC_PULSE_NOTE 36
//#define PARAM_OSC_SQUARE_NOTE 37
//#define PARAM_OSC_PHASE_SPREAD 20
////Filter
//#define PARAM_FILTER_FREQ 74
//#define PARAM_FILTER_RESO 19
//#define PARAM_FILTER_LP_MIX 26
//#define PARAM_FILTER_BP_MIX 27
//#define PARAM_FILTER_HP_MIX 28
//#define PARAM_FILTER_NOTCH_MIX 29
////AEG
//#define PARAM_AEG_ATTACK 73
//#define PARAM_AEG_DECAY 75
//#define PARAM_AEG_SUSTAIN 79
//#define PARAM_AEG_RELEASE 72
//#define PARAM_AEG_AMOUNT 7
////FEG
//#define PARAM_FEG_ATTACK 22
//#define PARAM_FEG_DECAY 23
//#define PARAM_FEG_SUSTAIN 24
//#define PARAM_FEG_RELEASE 25
////LFO
//#define PARAM_LFO_SHAPE 50
//#define PARAM_LFO_RATE 47
//#define PARAM_LFO_DEPTH 48
////Keys
//#define PARAM_KEYS_OCTAVE 114
//#define PARAM_KEYS_SCALE 102
//#define PARAM_KEYS_TRANSPOSE 106
////Voice
//#define PARAM_VOICE_MODE 103
////Modulation depths
//#define PARAM_MOD_VEL_AMP 63
//#define PARAM_MOD_VEL_FREQ 109
//#define PARAM_MOD_VEL_RESO 110
//#define PARAM_MOD_LFO_AMP 58
//#define PARAM_MOD_LFO_FREQ 112
//#define PARAM_MOD_LFO_RESO 56
////Not currently used
//#define PARAM_MOD_AT_FREQ 90
//#define PARAM_MOD_AT_LFO_DEPTH 89
////Effects
//#define PARAM_FX_DISTORTION_AMOUNT 13
////Global
//#define PARAM_GLOBAL_VINTAGE_AMOUNT 107
//#define PARAM_GLOBAL_VOLUME 9
////CC only used between the brain and voice
//#define PARAM_UPDATE_NOTE_PITCH 126
////special CC num used for sending commands rather than settings
//#define PARAM_CMD 127

//==========================================================================
// #define MUSIC_NUM_KEYS 18
// #define MUSIC_NUM_SCALES 8

// const int musicScales[MUSIC_NUM_SCALES][MUSIC_NUM_KEYS] =
// {
    // //chromatic
    // {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17},
    // //major
    // {0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 17, 19, 21, 23, 24, 26, 28, 29},
    // //major pentatonic
    // {0, 2, 4, 7, 9, 12, 14, 16, 19, 21, 24, 26, 28, 31, 33, 36, 38, 40},
    // //minor
    // {0, 2, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 20, 22, 24, 26, 27, 29},
    // //minor pentatonic
    // {0, 3, 5, 7, 10, 12, 15, 17, 19, 22, 24, 27, 29, 31, 34, 36, 39, 41},
    // //melodic minor
    // {0, 2, 3, 5, 7, 9, 11, 12, 14, 15, 17, 19, 21, 23, 24, 26, 27, 29},
    // //harmonic minor
    // {0, 2, 3, 5, 7, 8, 11, 12, 14, 15, 17, 19, 20, 23, 24, 26, 27, 29},
    // //blues
    // {0, 3, 5, 6, 7, 10, 12, 15, 17, 18, 19, 22, 24, 27, 29, 30, 31, 34}
// };


// // =================================================================
// struct ByteOrder
// // =================================================================
// {
   // constexpr inline static uint16_t
   // makeU16( uint8_t a, uint8_t b ) noexcept
   // {
       // return uint16_t( a ) | (uint16_t( b ) << 8);
   // }

   // constexpr inline static uint32_t
   // makeU32( uint8_t r, uint8_t g, uint8_t b, uint8_t a ) noexcept
   // {
      // return uint32_t( r ) |
           // ( uint32_t( g ) << 8  ) |
           // ( uint32_t( b ) << 16 ) |
           // ( uint32_t( a ) << 24 );
   // }

   // constexpr inline static uint64_t
   // makeU64( uint8_t r, uint8_t g, uint8_t b, uint8_t a,
            // uint8_t x, uint8_t y, uint8_t z, uint8_t w ) noexcept
   // {
      // return uint64_t( r ) |
           // ( uint64_t( g ) << 8  ) |
           // ( uint64_t( b ) << 16 ) |
           // ( uint64_t( a ) << 24 ) |
           // ( uint64_t( x ) << 32 ) |
           // ( uint64_t( y ) << 40 ) |
           // ( uint64_t( z ) << 48 ) |
           // ( uint64_t( w ) << 56 );
   // }

   // constexpr inline static uint16_t
   // bigEndianU16( void const* bytes ) noexcept
   // {
      // return makeU16(
               // static_cast< uint8_t const* >( bytes )[ 1 ],
               // static_cast< uint8_t const* >( bytes )[ 0 ] );
   // }

   // constexpr inline static uint32_t
   // bigEndianU32( void const* bytes ) noexcept
   // {
      // return makeU32(
               // static_cast< uint8_t const* >( bytes )[ 3 ],
               // static_cast< uint8_t const* >( bytes )[ 2 ],
               // static_cast< uint8_t const* >( bytes )[ 1 ],
               // static_cast< uint8_t const* >( bytes )[ 0 ] );
   // }

   // constexpr inline static uint64_t
   // bigEndianU64( void const* bytes ) noexcept
   // {
      // return makeU64(
               // static_cast< uint8_t const* >( bytes )[ 7 ],
               // static_cast< uint8_t const* >( bytes )[ 6 ],
               // static_cast< uint8_t const* >( bytes )[ 5 ],
               // static_cast< uint8_t const* >( bytes )[ 4 ],
               // static_cast< uint8_t const* >( bytes )[ 3 ],
               // static_cast< uint8_t const* >( bytes )[ 2 ],
               // static_cast< uint8_t const* >( bytes )[ 1 ],
               // static_cast< uint8_t const* >( bytes )[ 0 ] );
   // }

// };

// struct InputStream
// {
   // DE_CREATE_LOGGER("de.InputStream")
   // std::vector< uint8_t > m_data;
   // size_t m_readIndex;
   // std::string m_uri;

   // InputStream() noexcept : m_readIndex( 0 )
   // {
   // }

   // //   ~InputStream()
   // //   {
   // //   }

   // inline uint8_t const*
   // data() const noexcept { return m_data.data(); }

   // inline uint8_t*
   // data() noexcept { return m_data.data(); }

   // inline uint8_t const*
   // read() const noexcept { return m_data.data() + m_readIndex; }

   // inline size_t
   // size() const noexcept { return m_data.size(); }

   // inline size_t
   // tell() const noexcept { return m_readIndex; }

   // inline size_t
   // seek( size_t byteIndex ) noexcept
   // {
      // if ( byteIndex == tell() ) { return false; }
      // if ( byteIndex > m_data.size() - 1 ) { return false; }
      // m_readIndex = byteIndex;
      // //DE_DEBUG("seek(",byteIndex, ") from pos(",tell(), ")")
      // return tell();
   // }

   // inline size_t
   // skip( size_t bytesCount ) noexcept
   // {
      // if ( bytesCount < 1 ) return true;
      // return seek( tell() + bytesCount );
   // }

   // inline size_t
   // putback( size_t relativeBytes ) noexcept
   // {
      // if ( relativeBytes < 1 ) return true;
      // if ( relativeBytes > m_readIndex ) { m_readIndex = 0; return false; }
      // //DE_DEBUG("putback(",relativeBytes, ") from pos(",tell(), ")")
      // m_readIndex -= relativeBytes;
      // return true;
   // }

   // inline size_t
   // readU8( void* dst ) noexcept
   // {
      // if ( tell() > size() - 1 ) { return 0; }
      // auto pDst = static_cast< uint8_t* >( dst );
      // *pDst = m_data[ m_readIndex ];
      // m_readIndex += sizeof( uint8_t );
      // return 1; // 1 Byte consumed
   // }

   // inline size_t
   // readU16( void* dst ) noexcept
   // {
      // if ( tell() > size() - 2 ) { return 0; }
      // auto pDst = static_cast< uint16_t* >( dst );
      // auto pSrc = reinterpret_cast< uint16_t* >( &m_data[ m_readIndex ]  );
      // *pDst = *pSrc;
      // m_readIndex += sizeof( uint16_t );
      // return 2; // 2 Bytes consumed
   // }

   // inline size_t
   // readU32( void* dst ) noexcept
   // {
      // if ( tell() > size() - 4 ) { return 0; }
      // auto pDst = static_cast< uint32_t* >( dst );
      // auto pSrc = reinterpret_cast< uint32_t* >( &m_data[ m_readIndex ]  );
      // *pDst = *pSrc;
      // m_readIndex += sizeof( uint32_t );
      // return 4; // 4 Bytes consumed
   // }

   // inline size_t
   // readU8_be( void* dst ) noexcept { return readU8( dst ); }

   // inline size_t
   // readU16_be( void* dst ) noexcept
   // {
      // if ( tell() > size() - 2 ) { return 0; }
      // auto pDst = static_cast< uint16_t* >( dst );
      // auto pSrc = reinterpret_cast< uint16_t* >( &m_data[ m_readIndex ] );
      // *pDst = ByteOrder::bigEndianU16( pSrc );
      // m_readIndex += sizeof( uint16_t );
      // return 2; // 2 Bytes consumed
   // }

   // inline size_t
   // readU32_be( void* dst ) noexcept
   // {
      // if ( tell() > size() - 4 ) { return 0; }
      // auto pDst = static_cast< uint32_t* >( dst );
      // auto pSrc = reinterpret_cast< uint32_t* >( &m_data[ m_readIndex ] );
      // *pDst = ByteOrder::bigEndianU32( pSrc );
      // m_readIndex += sizeof( uint32_t );
      // return 4; // 4 Bytes consumed
   // }

   // // 8GB RAM limit
   // bool
   // loadFromFile( std::string const & uri, uint64_t byteLimit = (uint64_t(1) << 33) ) // noexcept
   // {
      // FILE* file = ::fopen64( uri.c_str(), "rb" );
      // if ( !file )
      // {
         // DE_ERROR("Cant open file ",uri)
         // return false;
      // }

      // ::fseeko64( file, 0, SEEK_END );    // Jump to end
      // auto siz = ::ftello64( file );      // Read file size
      // ::fseeko64( file, 0, SEEK_SET );    // Jump to begin

      // if ( siz < 1 ) // No data
      // {
         // DE_ERROR("Empty file ",uri)
         // return false;
      // }

      // if ( siz > byteLimit ) // Bad, reached RAM limit.
      // {
         // DE_ERROR("File reached RAM limit(",byteLimit,"), abort. ",uri)
         // return false;
      // }

      // // Copy entire file to RAM
      // m_data.resize( siz );
      // ::fread( m_data.data(), 1, m_data.size(), file );

      // // Close file and use m_data in RAM now ( aka very fast ) ...
      // ::fclose( file );

      // DE_DEBUG("File has uri = ",uri)
      // DE_DEBUG("File has byteCount = ",m_data.size())

      // m_readIndex = 0;
      // m_uri = uri;
      // return true;
   // }

   // bool
   // loadFromData( uint8_t const* src, uint64_t byteCount ) // noexcept
   // {
      // if ( !src )
      // {
         // DE_ERROR("No src")
         // return false;
      // }

      // m_data.resize( byteCount );
      // memcpy( m_data.data(), src, byteCount);

      // DE_DEBUG("Data has byteCount = ",m_data.size())
      // m_readIndex = 0;
      // m_uri = "Data";
      // return true;
   // }

// };

// // =======================================================================
// struct MidiTest
// // =======================================================================
// {
   // static std::vector< uint8_t >
   // testFile1()
   // {
      // return std::vector< uint8_t >{
         // 0x4D, 0x54, 0x68, 0x64, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01, 0x01, 0xE0, 0x4D, 0x54,
         // 0x72, 0x6B, 0x00, 0x00, 0x00, 0x86, 0x00, 0xFF, 0x03, 0x35, 0x47, 0x65, 0x6E, 0x65, 0x72, 0x61,
         // 0x74, 0x65, 0x64, 0x20, 0x6D, 0x6F, 0x72, 0x73, 0x65, 0x20, 0x63, 0x6F, 0x64, 0x65, 0x20, 0x77,
         // 0x77, 0x77, 0x2E, 0x6D, 0x6F, 0x62, 0x69, 0x6C, 0x65, 0x66, 0x69, 0x73, 0x68, 0x2E, 0x63, 0x6F,
         // 0x6D, 0x2F, 0x67, 0x6F, 0x2F, 0x6D, 0x6F, 0x72, 0x73, 0x65, 0x5F, 0x63, 0x6F, 0x64, 0x65, 0x00,
         // 0xFF, 0x02, 0x17, 0x28, 0x43, 0x29, 0x20, 0x32, 0x30, 0x31, 0x30, 0x20, 0x4D, 0x6F, 0x62, 0x69,
         // 0x6C, 0x65, 0x66, 0x69, 0x73, 0x68, 0x2E, 0x63, 0x6F, 0x6D, 0x00, 0xFF, 0x58, 0x04, 0x04, 0x02,
         // 0x18, 0x08, 0x00, 0xFF, 0x51, 0x03, 0x08, 0x7A, 0x23, 0x00, 0xC0, 0x4F, 0x81, 0x70, 0x90, 0x51,
         // 0x7F, 0x3A, 0x80, 0x51, 0x00, 0x3A, 0x90, 0x51, 0x7F, 0x81, 0x2E, 0x80, 0x51, 0x00, 0x82, 0x2C,
         // 0x90, 0x51, 0x00, 0x82, 0x2C, 0x80, 0x51, 0x00, 0x00, 0xFF, 0x2F, 0x00 };
   // }

   //4D 54 68 64 00 00 00 06 00 00 00 01 00 60
   //4D 54 72 6B 00 00 00 3B, 00 FF 58 04 04 02 18 08, 00 FF 51 03 07 A1 20, 00 C0 05
   //00 C1 2E, 00 C2 46, 00 92 30 60, 00 3C 60, 60 91 43 40, 60 90 4C 20, 81 40 82 30 40
   //00 3C 40, 00 81 43 40, 00 80 4C 40, 00 FF 2F 00

   //4D 54 68 64 = MThd
   //00 00 00 06 = header chunk length
   //00 00       = format 0
   //00 01       = one track
   //00 60       = 96 per quarter-note

   //Then the track chunk. Followed by the events (notice the running status is used in places):
   //4D 54 72 6B = MTrk
   //00 00 00 3B = chunk length (59)
   //Delta-Time [EventCodes]
   //00 [FF 58] 04 04 02 18 08= time signature 4bytes?, 4/4 time, (24 MIDI clocks = 1beat) / click?, 8 32nd notes / beat.
   //00 [FF 51] 03 07 A1 20   = tempo 3bytes? 500.000 usec / quarter note = 120bpm
   //00 [C0] 05               = Ch0 ProgramChange 5 = GM Patch 6 = Electric Piano 2
   //00 [C1] 2E               = Ch1 ProgramChange 46 = GM Patch 47 = Harp
   //00 [C2] 46               = Ch2 ProgramChange 70 = GM Patch 71 = Bassoon
   //00 [92] 30 60            = Ch2 NoteOn C3(48), forte(96)

   //00 [3C] 60               = running status
   //=00[3C] 60 60   > > > >  = Ch2 NoteOn C4(60), also forte(96)
   //60 [91] 43 64            = Ch1 NoteOn G4(67), mezzo-forte, delta-time(96)

   //96 [90] 76 32            = Ch0 NoteOn E5, piano

   //00 [92] 30 60            = Ch2 NoteOn C3, piano
   //00 [3C] 60               = running status
   //60 [91] 43 40            = Ch1 NoteOn C4, mezzo-forte
   //60 [90] 4C 20            = Ch2 NoteOn C3, piano
   //81 40 [82] 30 40 two-byte delta-time
   //00 [3C] 40 running status
   //00 [81] 43 40
   //00 [80] 4C 40
   //00 [FF 2F] 00 end of track


//Then the track chunk.
// Its header followed by the events (notice the running status is used in places):
//4D 54 72 6B MTrk
//00 00 00 3B chunk length (59)
//Delta-Time Event Comments
//00 FF 58 04 04 02 18 08 time signature
//00 FF 51 03 07 A1 20 tempo
//00 C0 05
//00 C1 2E
//00 C2 46
//00 92 30 60
//00 3C 60 running status
//60 91 43 40
//60 90 4C 20
//81 40 82 30 40 two-byte delta-time
//00 3C 40 running status
//00 81 43 40
//00 80 4C 40
//00 FF 2F 00 end of track

// };

// Opens a Midifile and display partitur like, 1st milestone
// MIDI section <-------------MIDI Header---------------> <-----Track Header----> <-Track data-> {Track out>
// Byte number  0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 [22 to x]      [x+1 to x+4]
// Byte data    4D 54 68 64 00 00 00 06 00 01 00 01 00 80 4D 54 72 6B 00 00 00 0A blah blah..... 00 FF 2F 00
// MIDI section A---------> B---------> C---> D---> E---> F---------> G---------> H------------> I--------->

// // =======================================================================
// struct MidiToken
// // =======================================================================
// {
   // uint32_t type; // 0 = Time, 1 = Event

   // uint32_t color;

   // std::vector< uint8_t > m_data;

   // std::string comment;
// };

// // =======================================================================
// struct MidiFile
// // =======================================================================
// {
   // DE_CREATE_LOGGER("de.MidiFile")
   // InputStream m_file;
   // //0-the file contains a single multi-channel track
   // //1-the file contains one or more simultaneous tracks (or MIDI outputs) of a sequence
   // //2-the file contains one or more sequentially independent single-track patterns
   // uint16_t m_fmt;
   // // <ntrks> the number of tracks. Always 1 for a format 0 file.
   // uint16_t m_trackCount;
   // //15| bits 14 thru 8            | bits 7 thru 0
   // //F | E | D | C | B | A | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
   // //0 | ticks per quarter-note bits 14 - 0 = 0...2^14 = 16 * 1024 |
   // //1 | negative SMPTE format     | ticks per frame               |
   // uint16_t m_resolution;

   // uint16_t m_bpm;
   // uint8_t m_time_signature_z;
   // uint8_t m_time_signature_n;

   // int32_t m_lastCommand;

   // MidiFile();
   // ~MidiFile();

   // bool parse();

   // static MidiFile
   // loadFromFile( std::string uri )
   // {
      // MidiFile m;
      // m.m_file.loadFromFile( uri );
      // m.parse();
      // return m;
   // }

   // static MidiFile
   // loadFromData( uint8_t const * src, uint64_t byteCount )
   // {
      // MidiFile m;
      // m.m_file.loadFromData( src, byteCount );
      // m.parse();
      // return m;
   // }



// protected:
   // //
   // // Convert a MIDI keynote to a string
   // //
   // std::string
   // midiNoteStr( int key );

   // //
   // // VLQ - Variable-Length Quantity
   // //
   // int
   // readVLQ( uint8_t const * src, int* vlq );

   // int
   // readDeltaTimeEvent( uint8_t const * src, int chunkSize );

   // bool
   // readTrk( uint8_t const * src, int chunkSize );

   // int readF0( uint8_t const * src, int chunkSize, int deltaTime );
   // int readF1( uint8_t const * src, int chunkSize, int deltaTime );
   // int readF2( uint8_t const * src, int chunkSize, int deltaTime );
   // int readF3( uint8_t const * src, int chunkSize, int deltaTime );
   // int readF4( uint8_t const * src, int chunkSize, int deltaTime );
   // int readF5( uint8_t const * src, int chunkSize, int deltaTime );
   // int readF6( uint8_t const * src, int chunkSize, int deltaTime );
   // int readF7( uint8_t const * src, int chunkSize, int deltaTime );
   // int readF8( uint8_t const * src, int chunkSize, int deltaTime );
   // int readF9( uint8_t const * src, int chunkSize, int deltaTime );
   // int readFA( uint8_t const * src, int chunkSize, int deltaTime );
   // int readFB( uint8_t const * src, int chunkSize, int deltaTime );
   // int readFC( uint8_t const * src, int chunkSize, int deltaTime );
   // int readFD( uint8_t const * src, int chunkSize, int deltaTime );
   // int readFE( uint8_t const * src, int chunkSize, int deltaTime );
   // int readFF_MetaEvent( uint8_t const * src, int chunkSize, int deltaTime );


   // std::vector< MidiToken > m_tokens;

//   static int
//   findActualEventLength( uint8_t const* data, int maxBytes ) noexcept;

   //
   // VLQ - Variable-Length Quantity
   //
   //   int
   //   readVLQ( std::istream& infile, int& trackbytes );

   //
   // readMidiEvent - Read a delta time and then a MIDI message (or meta message).
   //                 Returns 1 if not end-of-track meta message 0 otherwise.
   //
   //   int
   //   readMidiEvent( std::ostream& out, std::istream& in, int& trackbytes, int& command );

// };


/*
 *
//int
//MidiParser::readVLQ( int* chunkSize )
//{
//   auto pos = m_file.tell();

//   uint8_t c = 0;
//   if ( !m_file.readU8( &c ) ) { DE_ERROR("Cant read 1st byte of VLQ") return 0; }

//   int n = 1; // Number of bytes of the variable len time.

//   int retVal = 0x7f & c;
//   while ( c >= 0x80)
//   {
//      if ( !m_file.readU8( &c ) )
//      {
//         DE_ERROR("Cant read ",n,"-th byte of VLQ")
//         break;
//      }

//      retVal = (retVal << 7) | (0x7f & c);
//      n++;
//   }

//   //DE_DEBUG("[",pos,"] Parsed VLQ = ",retVal," after ",n," bytes")
//   if ( chunkSize ) *chunkSize -= n;
//   return retVal;
//}
int
MidiParser::readVLQ( int* vlq )
{
   auto pos = m_file.tell();

   uint8_t c = 0;
   if ( !m_file.readU8( &c ) ) { DE_ERROR("Cant read 1st byte of VLQ") return 0; }

   int n = 1; // Number of bytes of the variable len time.

   int retVal = 0x7f & c;
   while ( c >= 0x80)
   {
      if ( !m_file.readU8( &c ) )
      {
         DE_ERROR("Cant read ",n,"-th byte of VLQ")
         break;
      }

      retVal = (retVal << 7) | (0x7f & c);
      n++;
   }

   DE_DEBUG("[",pos,"] Parsed VLQ = ",retVal," after ",n," bytes")
   if ( vlq ) *vlq = retVal;
   return n;
}

bool
MidiParser::readTrk( int chunkSize )
{
   //DE_DEBUG("Got Byte c1 = ", hexStr( c1 ))

   while ( chunkSize > 0 )
   {
      //bool ok = m_file.seek( pos + chunkSize );
      int deltaTime = readVLQ( &chunkSize );

      int pos = m_file.tell();

      uint8_t c1 = 0;
      uint8_t c2 = 0;
      uint8_t c3 = 0;
      uint8_t c4 = 0;

      if ( !m_file.readU8( &c1 ) ) { DE_ERROR("No 1st byte of MIDI message") return false; }

      int command = 0;
      int channel = 0;

      // running status: command byte is previous one in data stream
      if ( c1 < 0x80 )
      {
         DE_DEBUG("[",pos,"] Running status = ", hexStr( c1 ), ", dt = ",deltaTime)
      }
      else
      {
         // midi command byte
         command = c1;
         channel = c1 & 0x0F;
         if ( !m_file.readU8( &c2 ) ) { DE_ERROR("No 2nd byte of MIDI message") return false; }
         chunkSize--;
         //DE_DEBUG("MIDI Command Byte c1 = ", hexStr( c1 ), " c2 = ", hexStr( c2 ) )
      }

      if ( command == 0xFF )
      {
         int metatype = c2;
         int length = readVLQ( &chunkSize );
         //DE_DEBUG("FF MetaMessage length ", length )

         int number = 0;

         if (metatype == 0x00) // sequence number, 2bytes big-endian decimal val.
         {
            m_file.readU8( &c3 );
            m_file.readU8( &c4 );
            number = (int(c3) << 8) | int(c4);
            chunkSize -= 2;
            DE_DEBUG("[",pos,"] FF 00 sequence number ", number, ", dt = ",deltaTime )
         }
         else if (metatype == 0x01) // text
         {
            DE_DEBUG("[",pos,"] FF 01 text, length = ", length, ", dt = ",deltaTime )
            m_file.skip( length );
            chunkSize -= length;
         }
         else if (metatype == 0x02)  // copyright
         {
            DE_DEBUG("[",pos,"] FF 02 copyright, length = ", length, ", dt = ",deltaTime )
            m_file.skip( length );
            chunkSize -= length;
         }
         else if (metatype == 0x03) // track name
         {
            DE_DEBUG("[",pos,"] FF 03 track name, length = ", length, ", dt = ",deltaTime )
            m_file.skip( length );
            chunkSize -= length;
         }
         else if (metatype == 0x04) // instrument name
         {
            DE_DEBUG("[",pos,"] FF 04 instrument name, length = ", length, ", dt = ",deltaTime )
            m_file.skip( length );
            chunkSize -= length;
         }
         else if (metatype == 0x05) // lyric
         {
            DE_DEBUG("[",pos,"] FF 05 lyric, length = ", length, ", dt = ",deltaTime )
            m_file.skip( length );
            chunkSize -= length;
         }
         else if (metatype == 0x06) // marker
         {
            DE_DEBUG("[",pos,"] FF 06 marker, length = ", length, ", dt = ",deltaTime )
            m_file.skip( length );
            chunkSize -= length;
         }
         else if (metatype == 0x07) // cue point
         {
            DE_DEBUG("[",pos,"] FF 07 cue point, length = ", length, ", dt = ",deltaTime )
            m_file.skip( length );
            chunkSize -= length;
         }
         else if (metatype == 0x08) // program name
         {
            DE_DEBUG("[",pos,"] FF 08 program name, length = ", length, ", dt = ",deltaTime )
            m_file.skip( length );
            chunkSize -= length;
         }
         else if (metatype == 0x09) // device name
         {
            DE_DEBUG("[",pos,"] FF 09 device name, length = ", length, ", dt = ",deltaTime )
            m_file.skip( length );
            chunkSize -= length;
         }
         else if (metatype == 0x20) // MIDI channel prefix
         {
            m_file.readU8( &c3 );
            chunkSize -= 1;
            DE_DEBUG("[",pos,"] FF 20 channel prefix ", hexStr(c3), ", dt = ",deltaTime )
         }
         else if (metatype == 0x21) // MIDI port display single-byte decimal number
         {
            m_file.readU8( &c3 );
            chunkSize -= 1;
            DE_DEBUG("[",pos,"] FF 21 port ", hexStr(c3), ", dt = ",deltaTime )
         }
         else if (metatype == 0x51) // Tempo, display tempo as "t" word.
         {
            m_file.readU8( &c3 );
            chunkSize -= 1;
            number = (number << 8) | c3;
            m_file.readU8( &c3 );
            chunkSize -= 1;
            number = (number << 8) | c3;
            m_file.readU8( &c3 );
            chunkSize -= 1;
            number = (number << 8) | c3;
            double tempo = 1000000.0 / number * 60.0;
            DE_DEBUG("[",pos,"] FF 51 tempo t = ", tempo, ", number = ", number, ", dt = ",deltaTime )
         }
         else if (metatype == 0x54) // SMPTE offset
         {
            m_file.readU8( &c3 );   // hour
            chunkSize -= 1;
            int hh = c3;
            m_file.readU8( &c3 );   // minutes
            chunkSize -= 1;
            int mm = c3;
            m_file.readU8( &c3 );   // seconds
            chunkSize -= 1;
            int ss = c3;
            m_file.readU8( &c3 );   // frames
            chunkSize -= 1;
            int frames = c3;
            m_file.readU8( &c3 );   // sub frames
            chunkSize -= 1;
            int subframes = c3;
            DE_DEBUG("[",pos,"] FF 54 SMPTE, dt = ",deltaTime)
         }
         else if (metatype == 0x58) // time signature
         {
            m_file.readU8( &c3 );   // numerator
            chunkSize -= 1;
            int numerator = c3;
            m_file.readU8( &c3 );   // denominator power
            chunkSize -= 1;
            int denominator = c3;
            m_file.readU8( &c3 );   // clocks per beat
            chunkSize -= 1;
            int clocksPerBeat = c3;
            m_file.readU8( &c3 );   // 32nd notes per beat
            chunkSize -= 1;
            int notesPerBeat32nd = c3;
            DE_DEBUG("[",pos,"] FF 58 time signature ",numerator,"/", denominator, ""
               " clocks_per_beat = ",clocksPerBeat,
               " 32nd_notes_per_beat = ",notesPerBeat32nd, ", dt = ",deltaTime)
         }
         else if (metatype == 0x59) // key signature
         {
            m_file.readU8( &c3 );   // key signature
            chunkSize -= 1;
            int keysignature = c3;
            m_file.readU8( &c3 );   // accidentals
            chunkSize -= 1;
            int accidentals = c3;
            m_file.readU8( &c3 );   // mode
            chunkSize -= 1;
            int mode = c3;
            DE_DEBUG("[",pos,"] FF 59 key-signature (",keysignature,"), "
                                    "accidentals(", accidentals,"), "
                                    "mode(", mode,"), dt = ",deltaTime )
         }
         else
         {
            DE_DEBUG("[",pos,"] FF XX DEFAULT CASE ERROR?, dt = ",deltaTime )
            m_file.skip( length );
            chunkSize -= length;

            for (int i=0; i<length; i++)
            {
               in.read((char*)&ch, 1);
               trackbytes++;
               s << " ";
               if (ch < 0x10)
               {
                  s << "0";
               }
               s << std::hex << (int)ch;
            }

         }

         //   switch (metatype)
         //   {
         //      case 0x00: comment += "sequence number";     break;
         //      case 0x01: comment += "text";                break;
         //      case 0x02: comment += "copyright notice";    break;
         //      case 0x03: comment += "track name";          break;
         //      case 0x04: comment += "instrument name";     break;
         //      case 0x05: comment += "lyric";               break;
         //      case 0x06: comment += "marker";              break;
         //      case 0x07: comment += "cue point";           break;
         //      case 0x08: comment += "program name";        break;
         //      case 0x09: comment += "device name";         break;
         //      case 0x20: comment += "MIDI channel prefix"; break;
         //      case 0x21: comment += "MIDI port";           break;
         //      case 0x51: comment += "tempo";               break;
         //      case 0x54: comment += "SMPTE offset";        break;
         //      case 0x58: comment += "time signature";      break;
         //      case 0x59: comment += "key signature";       break;
         //      case 0x7f: comment += "system exclusive";    break;
         //      case 0x2f:
         //         status = 0;
         //         comment += "end-of-track";
         //         break;
         //      default:
         //         comment += "meta-message";
         //   }

      }
      // [Begin SysEx] System Exclusive
      else if ( command == 0xF0 )
      {
         DE_DEBUG("[",pos,"] F0 SystemEx: channel(",channel,"), c2(", hexStr( c2 ),"), dt = ",deltaTime )
         // Read the first byte which is either 0xf0 or 0xf7.
         // Then a VLQ for the number of bytes that remain
         // in the message will follow.
         // Then read that number of bytes.
         // {
         //   in.putback(byte1);
         //   trackbytes--;
         //   int length = readVLQ( in, trackbytes );
         //   s << " v" << std::dec << length;
         //   for (int i=0; i<length; i++)
         //   {
         //      in.read((char*)&ch, 1);
         //      trackbytes++;
         //      if (ch < 0x10)
         //      {
         //         s << " 0" << std::hex << (int)ch;
         //      }
         //      else
         //      {
         //         s << " " << std::hex << (int)ch;
         //      }
         //   }
         //}

      }
      // [End SysEx] System Exclusive
      else if ( command == 0xF7 )
      {
         DE_DEBUG("[",pos,"] F7 SystemEx: channel(",channel,"), c2(", hexStr( c2 ),"), dt = ",deltaTime )
         // Read the first byte which is either 0xf0 or 0xf7.
         // Then a VLQ for the number of bytes that remain
         // in the message will follow.
         // Then read that number of bytes.
         // {
         //   in.putback(byte1);
         //   trackbytes--;
         //   int length = readVLQ( in, trackbytes );
         //   s << " v" << std::dec << length;
         //   for (int i=0; i<length; i++)
         //   {
         //      in.read((char*)&ch, 1);
         //      trackbytes++;
         //      if (ch < 0x10)
         //      {
         //         s << " 0" << std::hex << (int)ch;
         //      }
         //      else
         //      {
         //         s << " " << std::hex << (int)ch;
         //      }
         //   }
         //}
      }
      // MIDI Time Code Qtr. Frame
      else if ( command == 0xF1 )
      {
         DE_DEBUG("[",pos,"] F1 SystemEx: channel(",channel,"), c2(", hexStr( c2 ),"), dt = ",deltaTime )
      }
      // Song Position Pointer
      else if ( command == 0xF2 )
      {
         DE_DEBUG("[",pos,"] F2 SystemEx: channel(",channel,"), c2(", hexStr( c2 ),"), dt = ",deltaTime )
      }
      // Song Song Select (Song #) 1byte (0-127)
      else if ( command == 0xF3 )
      {
         DE_DEBUG("[",pos,"] F3 SystemEx: channel(",channel,"), c2(", hexStr( c2 ),"), dt = ",deltaTime )
      }
      // Undefined (Reserved)
      else if ( command == 0xF4 )
      {
         DE_DEBUG("[",pos,"] F4 SystemEx: channel(",channel,"), c2(", hexStr( c2 ),"), dt = ",deltaTime )
      }
      // Undefined (Reserved)
      else if ( command == 0xF5 )
      {
         DE_DEBUG("[",pos,"] F5 SystemEx: channel(",channel,"), c2(", hexStr( c2 ),"), dt = ",deltaTime )
      }
      // TuneRequest
      else if ( command == 0xF6 )
      {
         DE_DEBUG("[",pos,"] F6 SystemEx: channel(",channel,"), c2(", hexStr( c2 ),"), dt = ",deltaTime )
      }
      // Timing clock
      else if ( command == 0xF8 )
      {
         DE_DEBUG("[",pos,"] F8 SystemEx: channel(",channel,"), c2(", hexStr( c2 ),"), dt = ",deltaTime )
      }
      // Undefined (Reserved)
      else if ( command == 0xF9 )
      {
         DE_DEBUG("[",pos,"] F9 SystemEx: channel(",channel,"), c2(", hexStr( c2 ),"), dt = ",deltaTime )
      }
      // Start
      else if ( command == 0xFA )
      {
         DE_DEBUG("[",pos,"] FA SystemEx: channel(",channel,"), c2(", hexStr( c2 ),"), dt = ",deltaTime )
      }
      // Continue
      else if ( command == 0xFB )
      {
         DE_DEBUG("[",pos,"] FB SystemEx: channel(",channel,"), c2(", hexStr( c2 ),"), dt = ",deltaTime )
      }
      // Stop
      else if ( command == 0xFC )
      {
         DE_DEBUG("[",pos,"] FC SystemEx: channel(",channel,"), c2(", hexStr( c2 ),"), dt = ",deltaTime )
      }
      // Undefined (Reserved)
      else if ( command == 0xFD )
      {
         DE_DEBUG("[",pos,"] FD SystemEx: channel(",channel,"), c2(", hexStr( c2 ),"), dt = ",deltaTime )
      }
      // Active Sensing
      else if ( command == 0xFE )
      {
         DE_DEBUG("[",pos,"] ch(",channel,") FE ActiveSensing ", hexStr( c2 ),"), dt = ",deltaTime )
      }
      //
      else if ( command >= 80 )
      {
         DE_DEBUG("[",pos,"] ", hexStr( c1 )," ",hexStr( c2 )," ",hexStr( c3 ), " " )

         auto cmd2 = command & 0xF0;
         // [NoteOff] 2 bytes
         if ( cmd2 == 0x80 ) // note-off: 2 bytes
         {
            m_file.readU8( &c3 );
            chunkSize -= 1;

            int velocity = c3;
            DE_DEBUG("ch(",channel,") NOTE_OFF: ",midiNoteStr(c2),", velocity = ", velocity," dt = ",deltaTime )
         }
         // [NoteOn] 2 bytes
         else if ( cmd2 == 0x90 )
         {
            m_file.readU8( &c3 );
            chunkSize -= 1;

            int velocity = c3;
            if ( velocity > 0 )
            {
               DE_DEBUG("ch(",channel,") NOTE_ON: ",midiNoteStr(c2), ", velocity = ", velocity," dt = ",deltaTime )
            }
            else
            {
               DE_DEBUG("ch(",channel,") NOTE_OFF: ",midiNoteStr(c2),", velocity = ", velocity," dt = ",deltaTime )
            }
         }
         // [PAT] Polyphonic Aftertouch 2 bytes
         else if ( cmd2 == 0xA0 )
         {
            m_file.readU8( &c3 );
            chunkSize -= 1;
            DE_DEBUG("[",pos,"] ch(",channel,") AfterTouch: c2(", hexStr( c2 ),"), c3(", hexStr( c3 ),"), dt = ",deltaTime )
         }
         // [CC] Continuous controller, 2 bytes
         else if ( cmd2 == 0xB0 )
         {
            m_file.readU8( &c3 );
            chunkSize -= 1;
            DE_DEBUG("[",pos,"] ch(",channel,") CC-ContinousController: c2(", hexStr( c2 ),"), c3(", hexStr( c3 ),"), dt = ",deltaTime )
         }
         // [PC] PatchChange, 1 byte
         else if ( cmd2 == 0xC0 )
         {
            DE_DEBUG("[",pos,"] ch(",channel,") PatchChange: ",hexStr( c2 ),", dt = ",deltaTime )
         }
         // [CP] ChannelPressure, 1 byte
         else if ( cmd2 == 0xD0 )
         {
            DE_DEBUG("[",pos,"] ch(",channel,") ChannelPressure: ", hexStr( c2 ),"), dt = ",deltaTime )
         }
         // [PB] PitchBend, 2 bytes.
         else if ( cmd2 == 0xE0 )
         {
            m_file.readU8( &c3 );
            chunkSize -= 1;
            DE_DEBUG("[",pos,"] ch(",channel,") PitchBend: ", hexStr( c2 ),", c3(", hexStr( c3 ),"), dt = ",deltaTime )
         }
         else
         {
            DE_ERROR("[",pos,"] Unknown command ", hexStr( uint8_t(command) ),", dt = ",deltaTime )
         }
      }

   }

   return true;
}
*/
#endif
