#ifndef DE_AUDIO_ENUMERATE_PLUGIN_VST2X_HPP
#define DE_AUDIO_ENUMERATE_PLUGIN_VST2X_HPP

#include <de/audio/plugin/PluginInfoList.hpp>

namespace de {
namespace audio {

PluginInfo
getPluginInfo( std::wstring uri );

int
vstGetParamCount( void* vst, int32_t i );

float
vstGetParamValue( void* vst, int32_t i );

std::string
vstGetParamName( void* vst, int32_t i, std::vector< char > & buf );

std::string
vstGetParamLabel( void* vst, int32_t i, std::vector< char > & buf );

std::string
vstGetParamDisplay( void* vst, int32_t i, std::vector< char > & buf );

} // end namespace audio
} // end namespace de

#endif // G_LOPASS1_HPP
