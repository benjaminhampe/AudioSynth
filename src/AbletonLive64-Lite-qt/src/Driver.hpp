/// (c) 2017 - 20180 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef DE_LIVE_DRIVER_HPP
#define DE_LIVE_DRIVER_HPP

#include <de/audio/dsp/IDspChainElement.hpp>
#include <de/audio/stream/Stream.hpp>
#include <QString>

namespace de {
namespace audio {

// ============================================================================
struct DriverInfo
// ============================================================================
{
   std::string m_deviceUuid;
   std::string m_deviceName;  // expected name we are looking for
   int m_api; // 0 = WASAPI
   int m_deviceIndex;         // expected device index, -1 = default system device
   int m_sampleRate;
   int m_frameCount;
   int m_channelCount;
   int m_firstChannel;

   de::audio::Stream* m_stream;

   DriverInfo()
      : m_api( 0 )
      , m_deviceIndex( 0 )
      , m_sampleRate( 0 )
      , m_frameCount( 0 )
      , m_channelCount( 2 )
      , m_firstChannel( 0 )
   {}
};


} // end namespace live
} // end namespace de


#endif
