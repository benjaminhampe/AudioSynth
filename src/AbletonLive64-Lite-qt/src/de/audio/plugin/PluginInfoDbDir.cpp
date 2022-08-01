#include "PluginInfoDbDir.hpp"
#include "ScanPluginFolder.hpp"

namespace de {
namespace audio {


PluginInfoDbDir::PluginInfoDbDir()
   : m_uri()
   , m_recursive( true )
   , m_numSynths( 0 )
   , m_numEffects( 0 )
   , m_numDirs(0)
   , m_numFiles(0)
   , m_numBytes(0)
{}

PluginInfoDbDir::PluginInfoDbDir( std::wstring uri )
   : m_uri( uri )
   , m_recursive( true )
   , m_numSynths( 0 )
   , m_numEffects( 0 )
   , m_numDirs(0)
   , m_numFiles(0)
   , m_numBytes(0)
{
   clear();
}

bool
PluginInfoDbDir::hasPlugin( std::wstring const & uri ) const
{
   return getPlugin( uri ) != nullptr;
}

PluginInfo const*
PluginInfoDbDir::getPlugin( std::wstring const & uri ) const
{
   auto it = m_lutByUri.find( uri );
   if ( it == m_lutByUri.end() ) return nullptr;
   else return it->second;
}

PluginInfo*
PluginInfoDbDir::getPlugin( std::wstring const & uri )
{
   auto it = m_lutByUri.find( uri );
   if ( it == m_lutByUri.end() ) return nullptr;
   else return it->second;
}

/*
int
PluginInfoDbDir::findPluginInfo( std::wstring const & uri ) const
{
   auto beg = m_plugins.begin();
   auto end = m_plugins.end();
   auto it = std::find_if( beg, end, [&] ( PluginInfo const & cached_plugin ) { return cached_plugin.m_uri == uri; } );
   if ( it == end ) { return -1; }
   return std::distance( beg, it );
}
*/

void
PluginInfoDbDir::clear( bool bDeleteAsOwner )
{
   m_lutByUri.clear();
   if ( bDeleteAsOwner )
   {
      for ( size_t i = 0; i < m_plugins.size(); ++i )
      {
         auto p = m_plugins[ i ];
         if ( p ) delete p;
      }
   }
   m_plugins.clear();
   m_numSynths = 0;
   m_numEffects = 0;
}

bool
PluginInfoDbDir::addPlugin( PluginInfo* info )
{
   if ( !info )
   {
      std::wcout << __func__ << "() - Got nullptr" << std::endl;
      return false;
   }

   std::wstring const & uri = info->m_uri;
   if ( uri.empty() )
   {
      std::wcout << __func__ << "() - Got empty uri" << std::endl;
      return false;
   }

//      if ( !info->m_isPlugin )
//      {
//         std::wcout << __func__ << "() - Not a plugin " << uri << std::endl;
//         return false;
//      }

   PluginInfo* found = getPlugin( uri );
   if ( found )
   {
      std::wcout << __func__ << "() - Plugin already added " << uri << std::endl;
      return false;
   }

   if ( info -> isSynth() )
   {
      m_numSynths++;
   }
   else
   {
      m_numEffects++;
   }

   m_plugins.emplace_back( info );
   m_lutByUri[ uri ] = info;

   //std::wcout << __func__ << "() + Added plugin " << uri << "\n";
   return true;
}

bool
PluginInfoDbDir::readXML( int i, tinyxml2::XMLElement* dirNode )
{
   if ( !dirNode || !dirNode->Name() ) return false;
   if ( std::string( dirNode->Name() ) != "dir" ) return false;

   if ( !dirNode->FindAttribute("uri") )
   {
      std::cout << "Dir[" << i << "] :: No uri attrib in xml" << std::endl;
      return false;
   }

   std::string uri = dirNode->Attribute("uri" );
   m_uri = StringConv::toWStr( uri );

   if ( dirNode->FindAttribute("recursive") )
   {
      m_recursive = dirNode->IntAttribute("recursive") > 0;
   }

   if ( dirNode->FindAttribute("plugins") )
   {
      int checkSum = dirNode->IntAttribute("plugins");
   }

   if ( dirNode->FindAttribute("synths") )
   {
      m_numSynths = dirNode->IntAttribute("synths");
   }

   if ( dirNode->FindAttribute("effects") )
   {
      m_numEffects = dirNode->IntAttribute("effects");
   }

   if ( dirNode->FindAttribute("dirs") )
   {
      m_numDirs = dirNode->Unsigned64Attribute("dirs");
   }

   if ( dirNode->FindAttribute("files") )
   {
      m_numFiles = dirNode->Unsigned64Attribute("files");
   }

   if ( dirNode->FindAttribute("bytes") )
   {
      m_numBytes = dirNode->Unsigned64Attribute("bytes");
   }

   // Read first plugin
   tinyxml2::XMLElement* pluginNode = dirNode->FirstChildElement( "plugin" );
   if ( !pluginNode )
   {
      std::cout << "No <plugin> in dir " << i << std::endl;
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
            ok = addPlugin( pluginInfo );
            k++;
         }

         if ( pluginInfo && !ok )
         {
            delete pluginInfo;
         }

         pluginNode = pluginNode->NextSiblingElement( "plugin" );
      }

   }

   return true;
}

void
PluginInfoDbDir::writeXML( tinyxml2::XMLDocument & doc, tinyxml2::XMLElement* parent ) const
{
   if ( !parent )
   {
      return;
   }

   tinyxml2::XMLElement* dirNode = doc.NewElement( "dir" );
   if ( !dirNode )
   {
      return;
   }

   if ( m_uri.empty() )
   {
      dirNode->SetAttribute( "uri", "none" );
   }
   else
   {
      dirNode->SetAttribute( "uri", StringConv::toStr( m_uri ).c_str() );
   }

   dirNode->SetAttribute( "recursive", int( m_recursive ) );
   dirNode->SetAttribute( "plugins", m_plugins.size() );
   dirNode->SetAttribute( "synths", int( m_numSynths ) );
   dirNode->SetAttribute( "effects", int( m_numEffects ) );
   dirNode->SetAttribute( "dirs", m_numDirs );
   dirNode->SetAttribute( "files", m_numFiles );
   dirNode->SetAttribute( "bytes", m_numBytes );

   for ( auto & p : m_plugins )
   {
      if ( p ) p->writeXML( doc, dirNode );
   }

   parent->InsertEndChild( dirNode );
}

} // end namespace audio
} // end namespace de
