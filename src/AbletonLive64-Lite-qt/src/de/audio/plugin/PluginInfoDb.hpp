#ifndef DE_AUDIO_PLUGIN_DATABASE_HPP
#define DE_AUDIO_PLUGIN_DATABASE_HPP

#include "PluginInfoDbDir.hpp"
#include <mutex>

namespace de {
namespace audio {

typedef std::vector< PluginInfoDbDir* > PluginInfoDbDirs;

// ============================================================================
struct PluginInfoDb
// ============================================================================
{
   mutable std::mutex m_mutex;
   //std::string m_uri;
   PluginInfoDbDir m_all;   // All VST dirs ( flat list )
   PluginInfoDbDirs m_dirs;  // All VST plugins sorted by directory uris

   PluginInfoDb()
   {
   }

   ~PluginInfoDb()
   {
      clearNoLock();
   }

   //void setUri( std::string const & uri ) { m_uri = uri; }
   bool save( std::string const & uri ) const;
   bool load( std::string const & uri );

   bool isSynth( std::wstring const & uri ) const;

   int
   numEffects() const
   {
      //std::lock_guard< std::mutex > lg( m_mutex );
      return m_all.m_numEffects;
   }

   int
   numSynths() const
   {
      //std::lock_guard< std::mutex > lg( m_mutex );
      return m_all.m_numSynths;
   }
   PluginInfo const*
   getPlugin( std::wstring const & uri ) const
   {
      auto it = m_all.m_lutByUri.find( uri );
      if ( it == m_all.m_lutByUri.end() ) return nullptr;
      else return it->second;
   }

   PluginInfo*
   getPlugin( std::wstring const & uri )
   {
      auto it = m_all.m_lutByUri.find( uri );
      if ( it == m_all.m_lutByUri.end() ) return nullptr;
      else return it->second;
   }

   PluginInfoDbDir const*
   findDirNoLock( std::wstring const & uri ) const
   {
      for ( size_t i = 0; i < m_dirs.size(); ++i )
      {
         auto p = m_dirs[ i ];
         if ( p && p->m_uri == uri ) return p;
      }
      return nullptr;
   }

   PluginInfoDbDir*
   findDirNoLock( std::wstring const & uri )
   {
      for ( size_t i = 0; i < m_dirs.size(); ++i )
      {
         auto p = m_dirs[ i ];
         if ( p && p->m_uri == uri ) return p;
      }
      return nullptr;
   }

   void
   clearNoLock()
   {
      //std::lock_guard< std::mutex > lg( m_mutex );
      for ( size_t i = 0; i < m_dirs.size(); ++i )
      {
         auto p = m_dirs[ i ];
         if ( p ) delete p;
      }
      m_dirs.clear();
      m_all.clear();
   }

   std::mutex & getMutex() { return m_mutex; }
   std::mutex const & getMutex() const { return m_mutex; }

   void addDirNoLock( PluginInfoDbDir* dir );
   void addDir( PluginInfoDbDir* dir )
   {
      std::lock_guard< std::mutex > lg( m_mutex );
      addDirNoLock( dir );
   }

   void rebuildAll();

   void removeDirNoLock( std::wstring const & uri );
   void removeDir( std::wstring const & uri )
   {
      std::lock_guard< std::mutex > lg( m_mutex );
      removeDirNoLock( uri );
   }
   // PluginInfo const & operator[] ( size_t i ) const { return m_plugins[ i ]; }
   // PluginInfo & operator[] ( size_t i ) { return m_plugins[ i ]; }
   // size_t size() const { return m_plugins.size(); }

   std::wstring
   toWString() const
   {
      std::lock_guard< std::mutex > lg( m_mutex );

      std::wstringstream s;
      //s << "PluginInfoDb.XmlUri = " << StringConv::toWStr( m_uri ) << "\n";
      s << "PluginInfoDb.DirectoryCount = " << m_dirs.size() << "\n";
      s << "PluginInfoDb.PluginCount = " << m_all.m_lutByUri.size() << "\n";

//      for ( size_t i = 0; i < m_dirs.size(); ++i )
//      {
//         s << "Directory[" << i << "] " << m_dirs[ i ].m_uri << ", recursive(" << m_dirs[ i ].m_recursive << ")\n";
//      }

//      for ( size_t i = 0; i < m_plugins.size(); ++i )
//      {
//         s << "Plugin[" << i << "] " << m_plugins[ i ].toWString() << "\n";
//      }
      return s.str();
   }

/*



   int
   findDir( std::wstring const & uri ) const
   {
      std::lock_guard< std::mutex > lg( m_mutex );
      auto beg = m_dirs.begin();
      auto end = m_dirs.end();
      auto it = std::find_if( beg, end, [&] ( PluginInfoDbDir const & cached_dir ) { return cached_dir.m_uri == uri; } );
      if ( it == end ) { return -1; }
      return std::distance( beg, it );
   }

   int
   findPluginNoLock( std::wstring const & uri ) const
   {
      //std::lock_guard< std::mutex > lg( m_mutex );
      auto beg = m_plugins.begin();
      auto end = m_plugins.end();
      auto it = std::find_if( beg, end, [&] ( PluginInfo const & cached_plugin ) { return cached_plugin.m_uri == uri; } );
      if ( it == end ) { return -1; }
      return std::distance( beg, it );
   }

   PluginInfo const*
   getPlugin( std::wstring const & uri ) const
   {
      std::lock_guard< std::mutex > lg( m_mutex );
      auto beg = m_plugins.begin();
      auto end = m_plugins.end();
      auto it = std::find_if( beg, end, [&] ( PluginInfo const & cached_plugin ) { return cached_plugin.m_uri == uri; } );
      if ( it == end )
      {
         return nullptr;
      }
      else
      {
         return &(*it);
      }
   }

   PluginInfo*
   getPlugin( std::wstring const & uri )
   {
      std::lock_guard< std::mutex > lg( m_mutex );
      auto beg = m_plugins.begin();
      auto end = m_plugins.end();
      auto it = std::find_if( beg, end, [&] ( PluginInfo & cached_plugin ) { return cached_plugin.m_uri == uri; } );
      if ( it == end )
      {
         return nullptr;
      }
      else
      {
         return &(*it);
      }
   }
*/
};

} // end namespace audio
} // end namespace de

#endif
