#include "ImageReaderGIF.hpp"

#ifdef DE_IMAGE_READER_GIF_ENABLED

#include <gif5/gif_lib.h>

namespace de {
namespace image {
namespace gif {

// ===========================================================================
// de.image.gif.Decoder
// ===========================================================================
struct RAII_Decoder
{
   GifFileType* m_File;
   std::string m_Uri;
   DE_CREATE_LOGGER("de.image.gif.Decoder")

   RAII_Decoder()
      : m_File( nullptr )
   {}

   ~RAII_Decoder()
   {
      close(); // most important call of this class -> is now RAII = auto cleanup on leaving scope.
   }

   bool is_open() const
   {
      return( m_File != nullptr );
   }

   bool open( std::string const & uri )
   {
      if ( m_Uri == uri ) return is_open();

      close();

      int error = 0;
      m_Uri = uri;
      m_File = DGifOpenFileName( m_Uri.c_str(), &error );
      if ( !m_File )
      {
         DE_ERROR("Cant open gif uri(", uri, ")")
      }

      return is_open();
   }

   void close()
   {
      if ( m_File )
      {
         DE_ERROR("Closing gif uri(", m_Uri, ")" )
         // DumpScreen2RGB(OutFileName, OneFileFlag, colorMap, rows, file->SWidth, file->SHeight);
         //(void)free(rows);
         int error;
         if ( !DGifCloseFile( m_File, &error ) )
         {
            DE_ERROR("")
         }

         m_File = nullptr;
      }
   }
};

} // end namespace gif


// ===========================================================================
// de.image.ImageReaderGIF
// ===========================================================================
bool
ImageReaderGIF::load( Image & img, std::string const & uri )
{
   int iOffsets[] = { 0, 4, 2, 1 };  /* The way Interlaced image should. */
   int iJumps[] = { 8, 8, 4, 2 };   /* be read - offsets and jumps... */

   gif::RAII_Decoder decoder; // wrapper auto closes the file on func exit/return.
   if ( !decoder.open( uri ) )
   {
      DE_ERROR("")
      //PrintGifError( error );
      //exit(EXIT_FAILURE);
      return false;
   }

   GifFileType* file = decoder.m_File;
   if ( file->SHeight == 0 || file->SWidth == 0 )
   {
      DE_ERROR("")
      //printf( "Image of width or height 0\n" );
      //exit(EXIT_FAILURE);
      return false;
   }

   uint32_t w = file->SWidth;
   uint32_t h = file->SHeight;
   size_t bytesPerRow = w * sizeof( GifPixelType ); // Size in bytes one row.
   //size_t byteCount = bytesPerRow * h;

   GifRowType scanline = (GifRowType)malloc( bytesPerRow );
   if ( !scanline )
   {
      DE_ERROR("")
      //GIF_EXIT("Failed to allocate memory required, aborted.");
      return false;
   }

#if STRANGER_THINGS
   for ( uint32_t x = 0; x < w; ++x )
   {
      // Set its color to BackGround.
      scanline[ x ] = file->SBackGroundColor;
   }
#endif

   /* Lets dump it - set the global variables required and do it: */
   ColorMapObject* colorMap = file->Image.ColorMap ? file->Image.ColorMap : file->SColorMap;
   if ( !colorMap )
   {
      DE_ERROR("No color map")
      //printf( "Gif Image does not have a colormap\n" );
      //exit(EXIT_FAILURE);
      return false;
   }

   /* check that the background color isn't garbage (SF bug #87) */
   if ( file->SBackGroundColor < 0 ||
        file->SBackGroundColor >= colorMap->ColorCount )
   {
      DE_ERROR("")
      //printf( "Background color out of range for colormap\n");
      //exit(EXIT_FAILURE);
      return false;
   }

   img = de::Image( w, h );
   img.setUri( uri );
   img.fill( 0xFFFFFFFF );
   /* Scan the content of the GIF file and load the image(s) in: */
   GifRecordType chunk;
   do
   {
      if ( !DGifGetRecordType( file, &chunk ) )
      {
         DE_ERROR("")
         //PrintGifError(file->Error);
         //exit(EXIT_FAILURE);
         continue;
      }

      switch ( chunk )
      {
         case IMAGE_DESC_RECORD_TYPE:
         {
            if ( !DGifGetImageDesc( file ) )
            {
               DE_ERROR("")
               // PrintGifError(file->Error);
               // exit(EXIT_FAILURE);
               return false;
            }

            de::Recti rect( file->Image.Left, file->Image.Top,
                            file->Image.Width, file->Image.Height );

            //printf( "Image %d at (%d, %d) [%dx%d]: ",++ImageNum, Col, Row, Width, Height );

            if ( rect.getX2() >= int32_t( w ) || rect.getY2() >= int32_t( h ) )
            {
               DE_ERROR("rect.getX2() >= w || rect.getY2() >= h")
               // printf( "Image %d is not confined to screen dimension, aborted.\n",ImageNum);
               // exit(EXIT_FAILURE);
               return false;
            }

            if ( !file->Image.Interlace )
            {
               DE_WARN("GIF non-interlaced, direct decode.")
               for ( int32_t y = 0; y < rect.getHeight(); ++y )
               {
                  //GifQprintf("\b\b\b\b%-4d", i);
                  if ( !DGifGetLine( file, scanline, rect.getWidth() ) )
                  {
                     DE_ERROR("ScanLine Error ")
                     //PrintGifError(file->Error);
                     //exit(EXIT_FAILURE);
                     return false;
                  }

                  for ( int32_t x = 0; x < rect.getWidth(); ++x )
                  {
                     int index = scanline[ x ];
                     GifColorType gifColor = colorMap->Colors[ index ];
                     uint32_t color = RGBA( gifColor.Red, gifColor.Green, gifColor.Blue );
                     if ( index == file->SBackGroundColor )
                     {
                        color = 0x00000000;
                     }

                     img.setPixel( x,y,color );
                  }
               }
            }
            else
            {
               DE_WARN("GIF interlaced, need 4 passes to decode")

               // Need to perform 4 passes on the images:
               for ( int pass = 0; pass < 4; ++pass )
               {
                  for ( int j = rect.getY1() + iOffsets[ pass ];
                           j <= rect.getY2();
                           j += iJumps[ pass ] )
                  {
                     // GifQprintf("\b\b\b\b%-4d", Count++);
                     if ( !DGifGetLine( file, scanline, rect.getWidth() ) )
                     {
                        DE_ERROR("")
                        // PrintGifError(file->Error);
                        // exit(EXIT_FAILURE);
                        return false;
                     }
                  }
               }
            }
         }
         break;
      case EXTENSION_RECORD_TYPE:
         {
            GifByteType* ext;
            int extCode;

            /* Skip any extension blocks in file: */
            if ( !DGifGetExtension( file, &extCode, &ext ) )
            {
               DE_ERROR("")
               // PrintGifError(file->Error);
               // exit(EXIT_FAILURE);
               return false;
            }
            while ( !ext )
            {
               if ( !DGifGetExtensionNext( file, &ext ) )
               {
                  DE_ERROR("")
                  // PrintGifError(file->Error);
                  // exit(EXIT_FAILURE);
                  return false;
               }
            }
         }
         break;
      case TERMINATE_RECORD_TYPE:
         break;
      default:
         break;// Should be trapped by DGifGetRecordType.
      }
   } while ( chunk != TERMINATE_RECORD_TYPE );

   // DumpScreen2RGB(OutFileName, OneFileFlag, colorMap, rows, file->SWidth, file->SHeight);
   return true;
}

} // end namespace image.
} // end namespace de.


#if 0


/******************************************************************************
 The real screen dumping routine.
******************************************************************************/
static void DumpScreen2RGB(char *FileName, int OneFileFlag,
            ColorMapObject *ColorMap,
            GifRowType *ScreenBuffer,
            int ScreenWidth, int ScreenHeight)
{
    int i, j;
    GifRowType GifRow;
    GifColorType *ColorMapEntry;
    FILE *rgbfp[3];

    if (FileName != NULL)
    {
        if (OneFileFlag)
        {
            if ((rgbfp[0] = fopen(FileName, "wb")) == NULL)
            GIF_EXIT("Can't open input file name.");
        }
        else
        {
            static char *Postfixes[] = { ".R", ".G", ".B" };
            char OneFileName[80];

            for (i = 0; i < 3; i++)
            {
                strncpy(OneFileName, FileName, sizeof(OneFileName)-1);
                strncat(OneFileName, Postfixes[i], sizeof(OneFileName) - 1 - strlen(OneFileName) );

                if ((rgbfp[i] = fopen(OneFileName, "wb")) == NULL)
                {
                    GIF_EXIT("Can't open input file name.");
                }
            }
        }
    }
    else
    {
        OneFileFlag = true;

#ifdef _WIN32
   _setmode(1, O_BINARY);
#endif /* _WIN32 */

        rgbfp[0] = stdout;
    }

   // ================================

   if (ColorMap == NULL)
   {
      fprintf(stderr, "Color map pointer is NULL.\n");
      exit(EXIT_FAILURE);
   }

    if (OneFileFlag)
    {
        unsigned char *BufferP;
        unsigned char *Buffer = (unsigned char *) malloc(ScreenWidth * 3)
        if ( !Buffer )
        {
            GIF_EXIT("Failed to allocate memory required, aborted.");
         }
        for (i = 0; i < ScreenHeight; i++)
        {
            GifRow = ScreenBuffer[i];
            GifQprintf("\b\b\b\b%-4d", ScreenHeight - i);
            for (j = 0, BufferP = Buffer; j < ScreenWidth; j++)
            {
                ColorMapEntry = &ColorMap->Colors[GifRow[j]];
                *BufferP++ = ColorMapEntry->Red;
                *BufferP++ = ColorMapEntry->Green;
                *BufferP++ = ColorMapEntry->Blue;
            }
            if (fwrite(Buffer, ScreenWidth * 3, 1, rgbfp[0]) != 1)
                GIF_EXIT("Write to file(s) failed.");
        }

        free((char *) Buffer);
        fclose(rgbfp[0]);
    }
    else
    {
        unsigned char *Buffers[3];

        if ((Buffers[0] = (unsigned char *) malloc(ScreenWidth)) == NULL ||
            (Buffers[1] = (unsigned char *) malloc(ScreenWidth)) == NULL ||
            (Buffers[2] = (unsigned char *) malloc(ScreenWidth)) == NULL)
            GIF_EXIT("Failed to allocate memory required, aborted.");

        for (i = 0; i < ScreenHeight; i++) {
            GifRow = ScreenBuffer[i];
            GifQprintf("\b\b\b\b%-4d", ScreenHeight - i);
            for (j = 0; j < ScreenWidth; j++) {
                ColorMapEntry = &ColorMap->Colors[GifRow[j]];
                Buffers[0][j] = ColorMapEntry->Red;
                Buffers[1][j] = ColorMapEntry->Green;
                Buffers[2][j] = ColorMapEntry->Blue;
            }
            if (fwrite(Buffers[0], ScreenWidth, 1, rgbfp[0]) != 1 ||
                fwrite(Buffers[1], ScreenWidth, 1, rgbfp[1]) != 1 ||
                fwrite(Buffers[2], ScreenWidth, 1, rgbfp[2]) != 1)
                GIF_EXIT("Write to file(s) failed.");
        }

        free((char *) Buffers[0]);
        free((char *) Buffers[1]);
        free((char *) Buffers[2]);
        fclose(rgbfp[0]);
        fclose(rgbfp[1]);
        fclose(rgbfp[2]);
    }
}

#endif


#endif // DE_IMAGE_READER_GIF_ENABLED
