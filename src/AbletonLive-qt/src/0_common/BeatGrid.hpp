#ifndef DE_LIVE3_BEAT_GRID_HPP
#define DE_LIVE3_BEAT_GRID_HPP

#include <LiveSkin.hpp>

//inline double
//sec2nano( double seconds )
//{
//   return seconds * 1000000000.0;
//}

//inline double
//nano2sec( double nanos )
//{
//   return nanos * 1.0e-9;
//}

// ============================================================================
struct BeatGrid
// ============================================================================
{
   DE_CREATE_LOGGER("BeatGrid")

   double m_pixelsPerSecond;
   double m_secondsPerPixel;

   double m_pixelsPerBeat;
   double m_beatsPerPixel;


   int barNum;
   int barDen;


   int pxPerBeat;
   int pxPerBar;
   int64_t timeStart;
   int beatStart;

   BeatGrid()
   {
      reset();
      std::cout << "BeatGrid(" << toString() << ")" << std::endl;
   }

   double
   time2pixel( double second ) const
   {
      double pixel = m_pixelsPerSecond * second;
      std::cout << "sec2pixel(" << second << ") = " << pixel << ", "
        "m_pixelsPerSecond = "  << m_pixelsPerSecond << std::endl;
      return pixel;
   }

   double
   pixel2time( double pixel ) const
   {
      double seconds = m_secondsPerPixel * pixel;
      std::cout << "pixel2sec(" << pixel << ") = " << seconds << ", "
        "m_secondsPerPixel = "  << m_secondsPerPixel << std::endl;
      return seconds;
   }

   void reset()
   {
      barNum = 4;
      barDen = 4;

      m_pixelsPerSecond = 20.0;
      m_secondsPerPixel = 1.0 / m_pixelsPerSecond;

      m_pixelsPerBeat = 50.0;
      m_beatsPerPixel = 1.0 / m_pixelsPerBeat;


      pxPerBeat = 20;
      pxPerBar = pxPerBeat * barNum;

      timeStart = 0;
      beatStart = 0;
   }

   std::string
   toString() const
   {
      std::ostringstream s;

      s << barNum << "/" << barDen;
      s << ", m_pixelsPerSecond = " << m_pixelsPerSecond
        << ", m_secondsPerPixel = " << m_secondsPerPixel;

      return s.str();
   }

   /*

   static BeatGrid &
   get()
   {
      static BeatGrid s_instance;
      return s_instance;
   }*/

};

#endif // G_LOPASS1_HPP
