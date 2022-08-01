#ifndef DE_AUDIO_GET_PLUGIN_INFO_LIST_HPP
#define DE_AUDIO_GET_PLUGIN_INFO_LIST_HPP

#include <sstream>
#include <GetPluginInfo.hpp>

namespace de {
namespace audio {

PluginInfoList
getPluginInfoList( std::wstring vstDir, bool searchSubFolders );

} // end namespace audio
} // end namespace de

#endif // G_LOPASS1_HPP
