#ifndef DE_AUDIO_PLUGIN_INFO_DB_DIR_HPP
#define DE_AUDIO_PLUGIN_INFO_DB_DIR_HPP

#include "ScanPluginFolder.hpp"
#include <mutex>

namespace de {
namespace audio {

// ============================================================================
struct PluginInfoDbDir
// ============================================================================
{
   PluginInfoDbDir();

   PluginInfoDbDir( std::wstring uri );

   bool
   hasPlugin( std::wstring const & uri ) const;

   PluginInfo const*
   getPlugin( std::wstring const & uri ) const;

   PluginInfo*
   getPlugin( std::wstring const & uri );

/*
   int
   findPluginInfo( std::wstring const & uri ) const;
*/

   void
   clear( bool bDeleteAsOwner = true );

   bool
   addPlugin( PluginInfo* info );

   bool
   readXML( int i, tinyxml2::XMLElement* dirNode );

   void
   writeXML( tinyxml2::XMLDocument & doc, tinyxml2::XMLElement* parent ) const;

public:
   std::wstring m_uri;  // directory uri of the plugins, not the xml storing data.
   bool m_recursive;
   size_t m_numSynths;
   size_t m_numEffects;
   size_t m_numDirs;
   size_t m_numFiles;
   size_t m_numBytes;

   PluginInfos m_plugins;

   std::unordered_map< std::wstring, PluginInfo* > m_lutByUri;
   //std::unordered_map< int, PluginInfo* > m_lutById;


};

} // end namespace audio
} // end namespace de

#endif
