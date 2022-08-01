#include "PluginInfoDb.hpp"
#include "ScanPluginFolder.hpp"

namespace de {
namespace audio {

void
PluginInfoDb::addDirNoLock( PluginInfoDbDir* dir )
{
   if ( !dir )
   {
      std::wcout << "PluginInfoDb.addDir() - Got nullptr\n";
      return;
   }

   std::wstring const & uri = dir->m_uri;

   if ( uri.empty() )
   {
      std::wcout << "PluginInfoDb.addDir() - Got empty uri\n";

      return;
   }

   PluginInfoDbDir* found = findDirNoLock( uri );
   if ( found )
   {
      std::wcout << "PluginInfoDb.addDir() - Dir already added " << uri << "\n";
      //return;
   }
   else
   {
      m_dirs.emplace_back( dir );
      rebuildAll();
   }

}

void
PluginInfoDb::removeDirNoLock( std::wstring const & uri )
{
   if ( uri.empty() )
   {
      std::wcout << "PluginInfoDb.removeDirNoLock() - Got empty uri\n";

      return;
   }

   PluginInfoDbDir* found = findDirNoLock( uri );
   if ( !found )
   {
      std::wcout << "PluginInfoDb.removeDirNoLock() - Dir not found " << uri << "\n";
      return;
   }

   // Find iterator to cached dir by pointer...
   auto it = std::find_if( m_dirs.begin(), m_dirs.end(), [&] ( PluginInfoDbDir* cached ) { return cached == found; } );

   // Delete iterator to cached dir, rebuild 'all' dir ( holding unique entries )
   if ( it != m_dirs.end() )
   {
      m_dirs.erase( it );
      rebuildAll();
   }

}

void
PluginInfoDb::rebuildAll()
{
   size_t numFiles = 0;
   size_t numDirs = 0;
   size_t numBytes = 0;

   m_all.clear( false ); // all-dir is not owner, any other dir is owner of its plugin-infos.

   for ( size_t i = 0; i < m_dirs.size(); ++i )
   {
      PluginInfoDbDir const * const dir = m_dirs[ i ];
      if ( !dir )
      {
         //DE_ERROR("Got nullptr in pluginDb !!!")
      }
      else
      {
         auto beg = dir->m_plugins.begin();
         auto end = dir->m_plugins.end();
         auto it = beg;
         while ( it != end )
         {
            m_all.addPlugin( *it );
            it++;
         }
         numFiles += dir->m_numFiles;
         numDirs += dir->m_numDirs;
         numBytes += dir->m_numBytes;

      }

   }

   m_all.m_numFiles = numFiles;
   m_all.m_numDirs = numDirs;
   m_all.m_numBytes = numBytes;

   // ASSERT( found )

   // Copy page entries to m_all for a big flat list.


   //m_dirs.emplace_back( dir );
}

bool
PluginInfoDb::save( std::string const & uri ) const
{
   dbRemoveFile( uri );

   tinyxml2::XMLDocument doc;
   tinyxml2::XMLElement* xmlDb = doc.NewElement( "plugin-info-db" );
   if ( !xmlDb )
   {
      std::cout <<"No <plugin-info-db>" << std::endl;
      return false;
   }

   {
      std::lock_guard< std::mutex > lg( m_mutex );

      xmlDb->SetAttribute( "dirs", int( m_dirs.size() ) );
//      db->SetAttribute( "plugins", int( m_all.m_plugins.size() ) );

      //doc.NewComment("Plugin info database search directories");
      for ( auto & p : m_dirs )
      {
         if ( p ) p->writeXML( doc, xmlDb );
      }

      //doc.NewComment("Plugin info database found items");
//      for ( auto & p : m_all.m_plugins )
//      {
//         if ( p ) p->writeXML( doc, db );
//      }

      doc.InsertEndChild( xmlDb );
   }

   auto e = doc.SaveFile( uri.c_str() );
   if ( e != tinyxml2::XML_SUCCESS )
   {
      std::cout << "Cant save xml retry " << uri << ", e = " << int(e) << std::endl;
      e = doc.SaveFile( uri.c_str() );
      if ( e != tinyxml2::XML_SUCCESS )
      {
         std::cout << "Cant save xml retry2 " << uri << ", e = " << int(e) << std::endl;
         e = doc.SaveFile( uri.c_str() );
      }
   }

   if ( e != tinyxml2::XML_SUCCESS )
   {
      std::cout << "Cant save xml " << uri << ", e = " << int(e) << std::endl;
      return false;
   }

   std::cout <<"Saved PluginInfoDb XML " << uri << std::endl;
   return true;
}

bool
PluginInfoDb::load( std::string const & uri )
{
   clearNoLock();

   //std::lock_guard< std::mutex > lg( m_mutex );

   tinyxml2::XMLDocument doc;
   auto e = doc.LoadFile( uri.c_str() );
   if ( e != tinyxml2::XML_SUCCESS )
   {
      std::cout << "Cant load PluginInfoDb xml " << uri << std::endl;
      return false;
   }

   tinyxml2::XMLElement* db = doc.FirstChildElement( "plugin-info-db" );
   if ( !db )
   {
      std::cout << "No <plugin-info-db> tag in xml " << uri << std::endl;
      return false;
   }

   int numDirs = db->IntAttribute( "dirs" );
   //int numPlugins = db->IntAttribute( "plugins" );

   // Read first directory, if any.
   tinyxml2::XMLElement* dirNode = db->FirstChildElement( "dir" );
   if ( !dirNode )
   {
      std::cout << "No <dir> in xml " << uri << std::endl;
   }
   else
   {
      // Read all next directories, if any.
      int k = 0;
      while ( dirNode )
      {
         auto dirInfo = new PluginInfoDbDir();
         if ( dirInfo->readXML( k, dirNode ) )
         {
            addDir( dirInfo );
            k++;
         }
         else
         {
            delete dirInfo;
         }

         dirNode = dirNode->NextSiblingElement( "dir" );
      }
   }

/*
   // Read first plugin
   tinyxml2::XMLElement* pluginNode = db->FirstChildElement( "plugin" );
   if ( !pluginNode )
   {
      std::cout << "No <plugin> in xml " << uri << std::endl;
   }
   else
   {
      // Read next plugins
      int k = 0;
      while ( pluginNode )
      {
         bool ok = false;

         auto pluginInfo = new PluginInfo();
         if ( pluginInfo->readXML( k, pluginNode ) )
         {
            ok = m_all.addPlugin( pluginInfo );
            k++;
         }

         if ( pluginInfo && !ok )
         {
            delete pluginInfo;
         }

         pluginNode = pluginNode->NextSiblingElement( "plugin" );
      }

   }
*/
   std::cout << "Loaded <plugin-info-db> xml " << uri << std::endl;
   std::cout << "<plugin-info-db> dirs = " << m_dirs.size() << ", expected dirs = " << numDirs << std::endl;
//   std::cout << "<plugin-info-db> plugins = " << m_all.m_plugins.size() << ", expected plugins = " << numPlugins << std::endl;

   rebuildAll();

   return true;
}

bool
PluginInfoDb::isSynth( std::wstring const & uri ) const
{
   auto p = getPlugin( uri );
   if ( !p ) { return false; }
   return p->isSynth();
}


/*
void
PluginInfoDb::addDir( PluginInfoDbDir dir )
{
   int found = findDir( uri );
   if ( found > -1 )
   {
      std::wcout << "Dir already added " << uri << "\n";
      return;
   }

   m_dirs.emplace_back( uri );
   if ( scanNow )
   {
      PluginInfos pi = scanPluginFolder( QString::fromStdWString( uri ), true );
      if ( pi.size() )
      {
         m_plugins.insert( m_plugins.end(), pi.begin(), pi.end() );
         std::cout << "Scanned " << pi.size() << " new plugins\n";
      }
   }
}

void
PluginInfoDb::rescanAll()
{
   m_plugins.clear();
   for ( size_t i = 0; i < m_dirs.size(); ++i )
   {
      std::wstring const & uri = m_dirs[ i ].m_uri;
      bool recursive = m_dirs[ i ].m_recursive;
      PluginInfos pi = scanPluginFolder( QString::fromStdWString( uri ), recursive );
      if ( pi.size() )
      {
         m_plugins.insert( m_plugins.end(), pi.begin(), pi.end() );
         std::cout << "Scanned " << pi.size() << " new plugins\n";
      }
   }
}
*/

} // end namespace audio
} // end namespace de
