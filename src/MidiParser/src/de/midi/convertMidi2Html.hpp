#ifndef DE_MIDI_2_HTML_CONVERTER_HPP
#define DE_MIDI_2_HTML_CONVERTER_HPP

#include <de/midi/MidiParser.hpp>
#include <de/midi/MidiHtmlWriter.hpp>
#include <de/midi/MidiImageWriter.hpp>

namespace de {
namespace midi {

struct convertMidi2Html
{
   DE_CREATE_LOGGER("convertMidi2Html")

   static bool
   convert( std::string loadUri, std::string saveUri )
   {
      if ( !dbExistFile( loadUri ) )
      {
         std::cout << "Error: no loadUri" << std::endl;
         return false;
      }

      if ( saveUri.empty() )
      {
         std::cout << "Error: no saveUri" << std::endl;
         saveUri = loadUri + ".html";
         // return false;
      }

      DE_DEBUG("MidiParser loadUri = ", loadUri )
      DE_DEBUG("HtmlWriter saveUri = ", saveUri )

      // (c) 2014 by <benjaminhampe@gmx.de>
      PerformanceTimer perf;

      // (c) 2022 by <benjaminhampe@gmx.de>
      // What receives midiTokens and produces a html file...
      MidiHtmlWriter htmlWriter;
      MidiImageWriter imageWriter;

      // (c) 2022 by <benjaminhampe@gmx.de>
      // What loads the midi HDD/SSD file completely into RAM memory.
      InputStream file;
      file.loadFromFile( loadUri );

      // (c) 2022 by <benjaminhampe@gmx.de>
      // What interprets the bytestream as midi protocol
      // separates Midi file into variable sized byte tokens
      // and emits them to all registered listeners of type IMidiParserListener.
      MidiParser parser;
      parser.addListener( &htmlWriter );
      parser.addListener( &imageWriter );

      // (c) 2022 by <benjaminhampe@gmx.de>
      // Parse now...
      uint8_t const* beg = file.beg();
      uint8_t const* end = file.end();
      perf.start();
      size_t e = parser.parse( beg, end, loadUri );
      perf.stop();
      DE_DEBUG("MidiParser needed ", perf.ms(), " ms, loadUri = ", loadUri )

      // (c) 2022 by <benjaminhampe@gmx.de>
      // Validate parser result.
      auto d = size_t(end - beg);
      if ( d != e )
      {
         DE_ERROR("Parser had error e(",e,") != d(",d,")")
         //return false;
      }

      // (c) 2022 by <benjaminhampe@gmx.de>
      // Write html ( most time intensive part )
      perf.start();
      htmlWriter.save( saveUri, loadUri, "" );
      perf.stop();
      DE_DEBUG("HtmlWriter needed ", perf.ms(), " ms, saveUri = ", saveUri )
      return true;
   }

};

} // end namespace midi
} // end namespace de

#endif
