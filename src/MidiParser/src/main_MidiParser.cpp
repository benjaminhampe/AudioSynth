//////////////////////////////////////////////////////////////////////////////
/// @file main_MidiParser.cpp
/// @author Benjamin Hampe <benjamin.hampe@gmx.de>
//////////////////////////////////////////////////////////////////////////////

#include <de/midi/convertMidi2Html.hpp>

//========================================================================
int main( int argc, char** argv )
//========================================================================
{
   std::cout << argv[ 0 ] << " by (c) 2022 by <benjaminhampe@gmx.de>";

   de::midi::MidiImageWriter::test();

//      DE_DEBUG( "Usage: ", argv[ 0 ], " | (c) 2022 by <benjaminhampe@gmx.de>" )
//      DE_DEBUG( "Case1: <noargs> - Opens interactive filedialog for loadUri, no extra saveUri." )
//      DE_DEBUG( "Case2: path/to/loadUri.mid - No extra saveUri." )
//      DE_DEBUG( "Case3: path/to/loadUri.mid path/to/saveUri.html - Manually controlled saveUri." )

   std::string loadUri;
   if ( argc > 1 )
   {
      loadUri = argv[1];
   }

   // (c) 2022 by <benjaminhampe@gmx.de>
   // Choose midi file...
   if ( !dbExistFile( loadUri ) )
   {
      loadUri = dbOpenFileDlg("MidiParser expects a midi file (.mid, .midi, .smf ) | (c) 2022 by <benjaminhampe@gmx.de>");
   }

   // (c) 2022 by <benjaminhampe@gmx.de>
   // If no command 3rd command line arg for saveUri was found it
   // chooses to save mid.html along the .mid file in same directory.
   std::string saveUri;
   if ( argc > 2 )
   {
      saveUri = argv[ 2 ];
   }
   saveUri = loadUri + ".html";

   // (c) 2022 by <benjaminhampe@gmx.de>
   // Validate dialog result.
   if ( !de::midi::convertMidi2Html::convert( loadUri, saveUri ) )
   {
      std::cout << "Cant convert midi("<<loadUri<<") to html("<<saveUri<<"), program exits now."<<std::endl;
      return 0;
   }

   // Open your local html browser
   std::stringstream ss;
   ss << "\"" << saveUri << "\"";  // Add Quotation marks for uris with spaces.
   std::string command = ss.str();
   std::cout << "Run command " << command << std::endl;
   system( command.c_str() );

   return 0;
}
