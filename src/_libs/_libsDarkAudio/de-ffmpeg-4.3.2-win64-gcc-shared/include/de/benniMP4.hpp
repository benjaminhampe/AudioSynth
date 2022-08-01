#ifndef DE_VIDEO_MP4_HPP
#define DE_VIDEO_MP4_HPP

#include <de_libavcodec.hpp>

namespace de {
namespace video {

// class Lib
// {
// public:
   // Lib()
   // {
      // avcodec_register_all();
      // av_register_all();
   // }

   // static Lib& 
   // getInstance()
   // {
      // static Lib singleton;
      // return singleton;
   // }

// };

} // end namespace video
} // end namespace de

/*
	
// Number of supported file-formats
const int AVCodec_FileTypeCount = 13;

// List supported file-formats
const char* const AVCodec_FileTypes[] = {
  "mp4",  // AAC format
  "m4a",  // AAC format // Apple Lossless Encoder (ALE)
  "m4b",  // AAC format
  "m4p",  // AAC format
  "m4v",  // AAC format
  "m4r",  // AAC format
  "aac",  // AAC format
  "ac3",  // AAC format
  "3gp",  // AAC format
  "wma",  // using ?
  "rm",  // using ?
  "ra",   // using ?
  "ram",  // using ?
};


// check that a given sample format is supported by the encoder
int check_sample_fmt( AVCodec* codec, enum AVSampleFormat sample_fmt );

// just pick the highest supported samplerate
int select_sample_rate( AVCodec* codec );

// select layout with the highest channel count
int select_channel_layout( AVCodec* codec );

*/

#endif // MMAUDIO_MP4_LIB_H
