#ifndef DE_FILEMAGIC_HPP
#define DE_FILEMAGIC_HPP

#include <cstdint>

namespace de {

enum EFileType
{
   EFT_PNG = 0,
   EFT_BMP,
   EFT_GIF,
   EFT_TGA,
   EFT_DDS,
   EFT_JPG,
   EFT_TIF,
   EFT_PCX,

   EFT_WAV,
   EFT_AAC,
   EFT_MP3,
   EFT_MP4,
   EFT_M4A,
   EFT_FLAC,
   EFT_OGG,
   EFT_WMA,
   EFT_FLV,

   EFT_UNKNOWN,
};

EFileType
getFileMagic( void* ptr );

} // end namespace de

#endif // DE_FILEMAGIC_HPP
