#ifndef DE_ID3V2_COVERART_HPP
#define DE_ID3V2_COVERART_HPP

#include <de/audio/de_taglib.hpp>
#include <DarkImage.hpp>

//#include <audio.FileInfo.h>
//#include <audio.TagList.h>

namespace de {
namespace audio {

   struct CoverArt
   {
      DE_CREATE_LOGGER("de.CoverArt")

      enum eCoverArtType
      {
         Other              = 0x00,
         FileIcon           = 0x01,
         OtherFileIcon      = 0x02,
         FrontCover         = 0x03,
         BackCover          = 0x04,
         LeafletPage        = 0x05,
         Media              = 0x06,
         LeadArtist         = 0x07,
         Artist             = 0x08,
         Conductor          = 0x09,
         Band               = 0x0A,
         Composer           = 0x0B,
         Lyricist           = 0x0C,
         RecordingLocation  = 0x0D,
         DuringRecording    = 0x0E,
         DuringPerformance  = 0x0F,
         MovieScreenCapture = 0x10,
         ColouredFish       = 0x11,
         Illustration       = 0x12,
         BandLogo           = 0x13,
         PublisherLogo      = 0x14,
         CoverArtTypeCount  = 0x15
      };

//      char const * const s_CoverArtType[] = {
//         "Other",                                  // 0
//         "32x32 pixels 'file icon' (PNG only)",    // 1
//         "Other file icon",                        // 2
//         "Cover (front)",                          // 3
//         "Cover (back)",                           // 4
//         "Leaflet page",                           // 5
//         "Media (e.g. label side of CD)",
//         "Lead artist/lead performer/soloist",
//         "Artist/performer",
//         "Conductor",
//         "Band/Orchestra",
//         "Composer",
//         "Lyricist/text writer",
//         "Recording Location",
//         "During recording",
//         "During performance",
//         "Movie/video screen capture",
//         "A bright coloured fish",
//         "Illustration",
//         "Band/artist logotype",
//         "Publisher/Studio logotype",
//         "ECoverArtTypeCount"
//      };

      //eFileMagic fileMagic;
      //std::string uri;       // --> mostly a temporary file to create

      //std::string TextEncoding;
      //std::string MimeType;      // Dont trust the mime type, trust the filemagic of data.
      //std::string PicType;
      //std::string Description;
      //char* PicData;

   #ifdef USE_QT
      QImage img;
   #endif

      std::string uri;  // Temporary created fileName, is not unicode.

      de::Image img;

      static std::string
      fileSuffixFromImageMagic( TagLib::ByteVector const & in )
      {
         if ( in.size() < 8 ) return "";
         uint32_t const MAGIC_JPG 		= 0xFFD8FF00;
         //uint32_t const MAGIC_JPG_JFIF = 0x4A464946;
         //uint32_t const MAGIC_JPG_Exif = 0x45786966;
         //uint32_t const MAGIC_JPG_EXIF = 0x45584946;
         uint32_t const MAGIC_PNG_1 	= 0x89504E47;
         uint32_t const MAGIC_PNG_2 	= 0x0D0A1A0A;
         uint16_t const MAGIC_BMP 		= 0x424D;
         uint16_t const MAGIC_BMP2 		= 0x24D4;
         uint32_t const MAGIC_GIF_1 	= 0x47494638;
         uint16_t const MAGIC_GIF_2 	= 0x3761;
         uint16_t const MAGIC_GIF_3 	= 0x3961;
         uint32_t const MAGIC_TIFF 		= 0x49204900;
         uint32_t const MAGIC_TIFF_LE 	= 0x49492A00;
         uint32_t const MAGIC_TIFF_BE 	= 0x4D4D002A;
         uint32_t mA = in.mid(0,4).toUInt();
         uint32_t mB = in.mid(5,4).toUInt();
         if ( (mA & 0xFFFFFF00) == MAGIC_JPG )
         {
            return "jpg";
         }
         else if ( (mA == MAGIC_PNG_1) && (mB == MAGIC_PNG_2) )
         {
            return "png";
         }
         else if ( ((mA & 0xFFFF) == MAGIC_BMP) || ((mA & 0xFFFF) == MAGIC_BMP2) )
         {
            return "bmp";
         }
         else if ( (mA == MAGIC_GIF_1) && (((mB & 0xFFFF) == MAGIC_GIF_2) || ((mB & 0xFFFF) == MAGIC_GIF_3)) )
         {
            return "gif";
         }
         else if ( (mA == MAGIC_TIFF_LE) || (mA == MAGIC_TIFF_BE) || ((mA & 0xFFFFFF00) == MAGIC_TIFF) )
         {
            return "tif";
         }

         DE_ERROR("No magic found")
         return "";
      }


      static bool
      getCoverArt( CoverArt & coverArt, std::wstring const & uri )
      {
//       ID3v2::Tag* tag = f.ID3v2Tag();
//       if ( tag )
//       {
//          cout << "ID3v2."
//          << tag->header()->majorVersion()
//          << "."
//          << tag->header()->revisionNumber()
//          << ", "
//          << tag->header()->tagSize()
//          << " bytes in tag"
//          << endl;

//       ID3v2::FrameList::ConstIterator it = tag->frameList().begin();
//       for ( ; it != tag->frameList().end(); it++ )
//       {
//          cout << (*it)->frameID() << " - \"" << (*it)->toString() << "\"" << endl;
//       }


         TagLib::MPEG::File file( uri.c_str() );
         if ( !file.isOpen() || !file.isValid() )
         {
            return false;
         }

         auto id3v2 = file.ID3v2Tag(false);
         if (!id3v2) return false;

         auto const & frameList = id3v2->frameList();
         auto itBegin = frameList.begin();
         auto itEnd = frameList.end();
         //auto it = itBegin;
         for ( TagLib::ID3v2::FrameList::ConstIterator it = itBegin; it != itEnd; it++ )
         {
            TagLib::ID3v2::Frame* frame = *it;
            if ( frame )
            {
               if ( TagLib::String( frame->frameID() ) == "APIC" )
               {
                  readAPIC( coverArt, dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*>(frame) );
               }
            }
         }

         return true;
      }

      static bool
      readAPIC( CoverArt & coverArt, TagLib::ID3v2::AttachedPictureFrame* apic )
      {
         if ( !apic ) return false;
         TagLib::ByteVector const & bytes = apic->picture();
         //if ( !bytes.data() ) return;
         if ( bytes.size() < 1 ) return false;
         std::string ext = fileSuffixFromImageMagic( bytes );     // --> check later
         std::string uri = "CoverArt." + ext;

//         FILE* file = fopen( uri.c_str(), "w+b"); // Write extracted image to temporary file
//         if ( file )
//         {
//            fwrite( bytes.data(), 1, bytes.size(), file );
//            fclose( file );
//         }

         //out->Description = toString( in->description() );
         //out->MimeType = toString( in->mimeType() );
         //out->PicType = toString( in->type() );

         auto pSrc = reinterpret_cast< uint8_t const* >( bytes.data() );
         if ( dbLoadImageFromMemory( coverArt.img, pSrc, bytes.size(), uri ) )
         {
            DE_DEBUG("coverArt(",coverArt.img,")")
            return true;
         }
         else
         {
            return false;
         }
      }
   };

   // // ============================================================================
   // struct CoverArtList
   // // ============================================================================
   // {
   //    std::vector< CoverArt > covers;
   // };

} // end namespace audio
} // end namespace de

#if 0
TagList scanMP3_TagList( const std::string& fileName );

/// specialized for MP3 files
//ID3v1*          scanMP3_ID3v1( ID3v1* out, TagLib::ID3v1::Tag* tag );
//ID3v2*          scanMP3_ID3v2( ID3v2* out, TagLib::ID3v1::Tag* tag );

void scanMP3_ID3v1( FileInfo* out, TagLib::ID3v1::Tag* tag );
void scanMP3_ID3v2( FileInfo* out, TagLib::ID3v2::Tag* tag );
void scanMP3_TBPM( FileInfo* out, TagLib::ID3v2::TextIdentificationFrame* in );
void scanMP3_TIPL( FileInfo* out, TagLib::ID3v2::PopularimeterFrame* in );
void scanMP3_APIC( bpmCoverArt* out, TagLib::ID3v2::AttachedPictureFrame* in);
void scanMP3_CoverArt( bpmCoverArt* out, TagLib::ID3v2::Tag* tag );
void scanMP3_FileInfo( FileInfo* out, const std::string& fileName );

/// scans all types of audio-files
void scanWaveFormat( WaveFormat* out, TagLib::AudioProperties* in);

void scanFileInfo( FileInfo* out, const std::string& fileName );

FileInfo* createAndScanFileInfo( const std::string& fileName );
//IFile*      scanFile( IFile* file, const std::string& fileName );



void test_TagScanner();

#endif

#endif // MMAUDIO_TAG_SCANNER_H
