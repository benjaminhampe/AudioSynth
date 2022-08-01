#ifndef DE_AUDIO_PLUGININFOLIST_HPP
#define DE_AUDIO_PLUGININFOLIST_HPP

#include "PluginInfo.hpp"

namespace de {
namespace audio {

// ============================================================================
struct PluginDir
// ============================================================================
{
   std::wstring m_vstDir;
   std::string m_vstDirMB;
   int m_checkSum = 0;
   std::vector< PluginInfo > m_list;

   PluginInfo const & operator[] ( size_t i ) const { return m_list[ i ]; }
   PluginInfo & operator[] ( size_t i ) { return m_list[ i ]; }

   size_t size() const { return m_list.size(); }

   std::wstring
   toWString() const
   {
      std::wstringstream s;
      s << "PluginDir.Directory = " << m_vstDir << "\n";
      s << "PluginDir.Count = " << m_list.size() << "\n";

      for ( size_t i = 0; i < m_list.size(); ++i )
      {
         s << "Plugin[" << i << "] " << m_list[ i ].toWString() << "\n";
      }
      return s.str();
   }

   bool
   saveXml( std::string const & uri ) const
   {
      std::string dir = dbGetFileDir( uri );

      dbMakeDirectory( dir );

      tinyxml2::XMLDocument doc;
      tinyxml2::XMLElement* root = doc.NewElement( "PluginDir" );

      root->SetAttribute( "directory", m_vstDirMB.c_str() );
      root->SetAttribute( "count", int( m_list.size() ) );

      for ( int i = 0; i < m_list.size(); ++i )
      {
         m_list[ i ].writeXML( doc, root );
      }
      doc.InsertEndChild( root );

      auto e = doc.SaveFile( uri.c_str() );
      if ( e != tinyxml2::XML_SUCCESS )
      {
         std::cout << "Cant save xml " << uri << std::endl;
         return false;
      }

      std::cout <<"Saved PluginDir XML " << uri << std::endl;

      return true;
   }

   bool
   loadXml( std::string const & uri )
   {
      tinyxml2::XMLDocument doc;
      auto e = doc.LoadFile( uri.c_str() );
      if ( e != tinyxml2::XML_SUCCESS )
      {
         std::cout << "Cant load PluginDir xml " << uri << std::endl;
         return false;
      }

      tinyxml2::XMLElement* root = doc.FirstChildElement( "PluginDir" );
      if ( !root )
      {
         std::cout << "No PluginDir tag in xml " << uri << std::endl;
         return false;
      }

      m_vstDirMB = root->Attribute( "directory" );
      m_checkSum = root->IntAttribute( "count" );
      m_vstDir = QString::fromStdString( m_vstDirMB ).toStdWString();

      m_list.clear();

      // Read all children <PluginInfo> of parent <PluginDir>

      // Read first child <PluginInfo> of parent <PluginDir>
      tinyxml2::XMLElement* pin = root->FirstChildElement( "PluginInfo" );
      if ( !pin )
      {
         std::cout << "No PluginInfo in xml " << uri << std::endl;
         return true;
      }

      // Read all next children <PluginInfo> of parent <PluginDir>
      int k = 0;
      while ( pin )
      {
         PluginInfo pluginInfo;
         if ( pluginInfo.readXML( k, pin ) )
         {
            m_list.emplace_back( std::move( pluginInfo ) );
            k++;
         }

         pin = pin->NextSiblingElement( "PluginInfo" );
      }

      std::cout <<"Loaded PluginDir XML " << uri << std::endl;
      std::cout << "[loadXml] dir = " << m_vstDirMB << std::endl;
      std::cout << "[loadXml] expectPlugins = " << m_checkSum << std::endl;
      std::cout << "[loadXml] loadedPlugins = " << k << std::endl;

      return true;
   }

};

} // end namespace audio
} // end namespace de

#endif // G_LOPASS1_HPP
