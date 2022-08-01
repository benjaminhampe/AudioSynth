#include "Driver.hpp"

/*
 *
namespace de {
namespace audio {


SpurInfo::SpurInfo( int id, std::string name, int type )
   : m_name( name )
   , m_isBypassed( false )
   , m_type( type )
   , m_id( id )
   , m_volume( 100 )
{}

SpurInfo::~SpurInfo()
{
   clear();
}

void
SpurInfo::clear()
{
   for ( size_t i = 0; i < m_plugins.size(); ++i )
   {
      auto p = m_plugins[ i ];
      if ( p ) delete p;
   }
   m_plugins.clear();
}

//int
//SpurInfo::findPlugin( int id ) const
//{
//   for ( size_t i = 0; i < m_plugins.size(); ++i )
//   {
//      auto p = m_plugins[ i ];
//      if ( p && p->m_id == id ) return i;
//   }
//   return -1;
//}

bool
SpurInfo::addPlugin( QString const & uri )
{
   int id = m_plugins.size();
   auto plugin = SpurPluginInfo( id, uri );
}

bool
SpurInfo::addPlugin( SpurPluginInfo* plugin )
{
   if ( !plugin )
   {
      return false;
   }
//   int found = findPlugin( id );
//   if ( found > -1 )
//   {
//      return false;
//   }

   int found = -1;
   for ( size_t i = 0; i < m_plugins.size(); ++i )
   {
      auto p = m_plugins[ i ];
      if ( p && p == plugin ) { found = i; break; }
   }

   if ( found < 0 )
   {
      m_plugins.emplace_back( plugin );
   }
   else
   {
      // Already added pointer
   }

   return true;
}

//bool
//SpurInfo::isAudioOnly() const { return m_type < 3; }

//std::string const &
//SpurInfo::name() const { return m_name; }

void
SpurInfo::writeXML( tinyxml2::XMLDocument & doc, tinyxml2::XMLElement* network ) const
{
   tinyxml2::XMLElement* chain = doc.NewElement( "chain" );
   chain->SetAttribute("id", m_id );
   chain->SetAttribute("type", m_type );
   chain->SetAttribute("name", m_name.c_str() );
   chain->SetAttribute("volume", m_volume );
   chain->SetAttribute("bypassed", int(m_isBypassed) );
   chain->SetAttribute("plugins", int(m_plugins.size()) );

   for ( size_t i = 0; i < m_plugins.size(); ++i )
   {
      auto p = m_plugins[ i ];
      if ( p ) p->writeXML( doc, chain );
   }

   network->InsertEndChild( chain );
}

bool
SpurInfo::readXML( int i, tinyxml2::XMLElement* chain )
{
   if ( !chain ) return false;
   if ( !chain->Name() || (std::string( chain->Name() ) != "chain") ) return false;

   if ( !chain->Attribute("name") )
   {
      std::cout << "SpurInfo[" << i << "] :: No name attribute" << std::endl;
   }

   m_id = chain->IntAttribute("id");
   m_type = chain->IntAttribute("type");
   m_name = chain->Attribute( "name" ); // Crash cand.
   m_isBypassed = chain->IntAttribute("bypassed") > 0;
   m_volume = chain->IntAttribute("volume");

   m_plugins.clear();
   int m_checkSum = chain->IntAttribute( "plugins" );

   // Read first child
   tinyxml2::XMLElement* plugin = chain->FirstChildElement( "plugin" );
   if ( !plugin )
   {
      std::cout << "No plugin[0]" << std::endl;
      return true;
   }

   // Read next children
   int k = 0;
   while ( plugin )
   {
      auto npi = new SpurPluginInfo();
      if ( npi->readXML( k, plugin ) )
      {
         m_plugins.emplace_back( npi );
         k++;
      }
      else
      {
         delete npi;
      }

      plugin = plugin->NextSiblingElement( "plugin" );
   }

//      std::cout <<"Loaded dsp chain from XML " << uri << std::endl;
//      //std::cout << "[loadXml] dir = " << m_vstDirMB << std::endl;
//      std::cout << "[loadXml] expectPlugins = " << m_checkSum << std::endl;
//      std::cout << "[loadXml] loadedPlugins = " << k << std::endl;

   return true;
}


// ============================================================================
// ============================================================================
// ============================================================================


NetworkInfo::NetworkInfo()
{

}

NetworkInfo::~NetworkInfo()
{
   clear();
}

void
NetworkInfo::clear()
{
   for ( size_t i = 0; i < m_spuren.size(); ++i )
   {
      auto p = m_spuren[ i ];
      if ( p ) delete p;
   }
   m_spuren.clear();

   m_master = nullptr;
   m_returns.clear();
   m_user.clear();
   m_userAudio.clear();
   m_userMidi.clear();
}

int
NetworkInfo::findSpur( int id ) const
{
   for ( size_t i = 0; i < m_spuren.size(); ++i )
   {
      auto p = m_spuren[ i ];
      if ( p && p->m_id == id ) return i;
   }
   return -1;
}

bool
NetworkInfo::addSpur( int id, std::string name, int type )
{
   int found = findSpur( id );
   if ( found > -1 )
   {
      return false;
   }

   m_spuren.emplace_back( id, name, type );
   auto back = &m_spuren.back();

   // 0 = master, 1 = return, 2 = audio-only, 3 = midi+audio
   if ( type == 0 )
   {
      m_master = back;
   }
   else if ( type == 1 )
   {
      m_returns.emplace_back( back );
   }
   else if ( type == 2 )
   {
      m_user.emplace_back( back );
      m_userAudio.emplace_back( back );
   }
   else if ( type == 3 )
   {
      m_user.emplace_back( back );
      m_userMidi.emplace_back( back );
   }
   return true;
}

void
NetworkInfo::createDefault()
{
   clear();
   addSpur( 0, "Master", 0 );
   addSpur( 1000, "A - Return", 1 );
   addSpur( 1001, "B - Return", 1 );
   addSpur( 2000, "1 - Audio", 2 );
   addSpur( 2001, "2 - Audio", 2 );
   addSpur( 3001, "3 - Midi", 3 );
   addSpur( 3002, "4 - Midi", 3 );
   addSpur( 3003, "5 - Midi", 3 );
}


bool
NetworkInfo::save( std::string const & uri ) const
{
   dbRemoveFile( uri );

   tinyxml2::XMLDocument doc;
   tinyxml2::XMLElement* network = doc.NewElement( "network" );
   network->SetAttribute( "count", int( m_spuren.size() ) );

   for ( size_t i = 0; i < m_spuren.size(); ++i )
   {
      auto p = m_spuren[ i ];
      if ( p ) p->writeXML( doc, network );
   }

   doc.InsertEndChild( network );

   auto e = doc.SaveFile( uri.c_str() );
   if ( e != tinyxml2::XML_SUCCESS )
   {
      std::cout << "Cant save <network> xml " << uri << ", e = " << int(e) << std::endl;
      return false;
   }

   std::cout <<"Saved <network> xml " << uri << std::endl;
   return true;
}

bool
NetworkInfo::load( std::string const & uri )
{
   tinyxml2::XMLDocument doc;
   auto e = doc.LoadFile( uri.c_str() );
   if ( e != tinyxml2::XML_SUCCESS )
   {
      std::cout << "Cant load <network> xml " << uri << std::endl;
      return false;
   }

   tinyxml2::XMLElement* root = doc.FirstChildElement( "network" );
   if ( !root )
   {
      std::cout << "No <network> tag in xml " << uri << std::endl;
      return false;
   }

   //m_vstDirMB = root->Attribute( "directory" );
   int m_checkSum = root->IntAttribute( "count" );
   //m_vstDir = QString::fromStdString( m_vstDirMB ).toStdWString();

   m_spuren.clear();

   // Read first child <PluginInfo> of parent <PluginInfoList>
   tinyxml2::XMLElement* spurNode = root->FirstChildElement( "chain" );
   if ( !spurNode )
   {
      std::cout << "No <chain> in <network> xml " << uri << std::endl;
      return false;
   }

   // Read all next children <PluginInfo> of parent <PluginInfoList>
   int k = 0;
   while ( spurNode )
   {
      auto spurInfo = new SpurInfo();
      if ( spurInfo->readXML( k, spurNode ) )
      {
         m_spuren.emplace_back( spurInfo );
         k++;
      }
      else
      {
         delete SpurInfo;
      }

      spurNode = spurNode->NextSiblingElement( "chain" );
   }

   std::cout <<"<network> OK loaded. " << uri << std::endl;
   //std::cout << "[loadXml] dir = " << m_vstDirMB << std::endl;
   std::cout << "<network> expect <chain> tags = " << m_checkSum << std::endl;
   std::cout << "<network> loaded <chain> tags = " << k << std::endl;

   return true;
}


} // end namespace live
} // end namespace de


*/
