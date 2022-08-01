#ifndef DE_AUDIO_PLUGININFOLIST_HPP
#define DE_AUDIO_PLUGININFOLIST_HPP

#include <cstdint>
#include <sstream>
#include <vector>
#include <iostream>
#include <tinyxml2/tinyxml2.h>

#include <DarkImage.hpp>

#include <QString>

namespace de {
namespace audio {

// ============================================================================
struct PluginInfo
// ============================================================================
{
   bool m_isFile;
   bool m_isPlugin;
   bool m_isVST2x;
   bool m_isSynth;
   bool m_hasEditor;
   bool m_can32Bit;
   bool m_can64Bit;
   bool m_canProgramChunks;

   std::wstring m_name;
   std::string m_nameMB;

   std::wstring m_uri;
   std::string m_uriMB;
   //std::string m_dirMultiByte;

   std::string m_entry;
   //std::wstring m_pluginName;

   //uint32_t m_rate;     // rate in Hz
   uint32_t m_numPrograms;
   uint32_t m_numParams;
   uint32_t m_numOutputs;
   uint32_t m_numInputs;
   //uint32_t m_flags;

   std::string m_comment;

   //int m_vendorVersion;
   //std::string m_vendor;
   //std::string m_product;

   PluginInfo() { reset(); }

   void reset()
   {
      m_name = L"";
      m_nameMB = "";

      m_isFile = false;
      m_isPlugin = false;
      m_isVST2x = false;
      m_isSynth = false;
      m_hasEditor = false;
      m_can32Bit = false;
      m_can64Bit = false;
      m_canProgramChunks = false;

      m_uri = L"";
      m_uriMB = "";
      m_entry = "";
      //m_dirMultiByte = "";
      //m_pluginName = "";
      //m_rate = 0;     // rate in Hz
      m_numPrograms = 0;
      m_numParams = 0;
      m_numOutputs = 0;
      m_numInputs = 0;
      //m_flags = 0;

      m_comment = "";
      //m_vendorVersion = 0;
      //m_vendor;
      //m_product;
   }

   bool hasEditor() const { return m_hasEditor; }

   std::wstring
   toWString() const
   {
      std::wstringstream s;
      s <<
      "name(" << m_name << "), "
      "VST2x(" << m_isVST2x << "), "
      "hasEditor(" << m_hasEditor << "), "
      "isSynth(" << m_isSynth << "), "
      "programs(" << m_numPrograms << "), "
      "params(" << m_numParams << "), "
      "outputs(" << m_numOutputs << "), "
      "inputs(" << m_numInputs << "), "
      "can32Bit(" << m_can32Bit << "), "
      "can64Bit(" << m_can64Bit << "), "
      "entry(" << m_entry.c_str() << "), "
      "uri(" << m_uri << ")"
      ;

      return s.str();
   }

   void
   writeXML( tinyxml2::XMLDocument & doc, tinyxml2::XMLElement* list ) const
   {
      tinyxml2::XMLElement* pin = doc.NewElement( "PluginInfo" );
      //pin->SetAttribute("type", "vst24" );
      pin->SetAttribute("name", m_nameMB.c_str() );
      pin->SetAttribute("editor", int(m_hasEditor) );
      pin->SetAttribute("synth", int(m_isSynth) );
      pin->SetAttribute("programs", m_numPrograms );
      pin->SetAttribute("params", m_numParams );
      pin->SetAttribute("outputs", m_numOutputs );
      pin->SetAttribute("inputs", m_numInputs );
      pin->SetAttribute("can32Bit", int(m_can32Bit) );
      pin->SetAttribute("can64Bit", int(m_can64Bit) );
      pin->SetAttribute("entry", m_entry.c_str() );
      pin->SetAttribute("uri", m_uriMB.c_str() );
      if ( m_comment.size() > 0 )
      {
         pin->SetText( m_comment.c_str() );
      }
      list->InsertEndChild( pin );
   }

   bool
   readXML( int i, tinyxml2::XMLElement* pin )
   {
      if ( !pin->Attribute("uri") )
      {
         std::cout << "PluginInfo[" << i << "] :: No uri attrib in xml" << std::endl;
         return false;
      }

      if ( !pin || !pin->Name() ) return false;
      if ( std::string( pin->Name() ) != "PluginInfo" ) return false;
      //pin->SetAttribute("type", "vst24" );

      reset();

      if ( pin->Attribute( "name" ) )
      {
         m_nameMB = pin->Attribute( "name" );
         m_name = QString::fromStdString( m_nameMB ).toStdWString();
      }
      m_hasEditor = pin->IntAttribute("editor") > 0;
      m_isSynth = pin->IntAttribute("synth") > 0;
      m_numPrograms = pin->IntAttribute("programs");
      m_numParams = pin->IntAttribute("params");
      m_numOutputs = pin->IntAttribute("outputs");
      m_numInputs = pin->IntAttribute("inputs");
      m_can32Bit = pin->IntAttribute("can32Bit") > 0;
      m_can64Bit = pin->IntAttribute("can64Bit") > 0;
      if ( pin->Attribute("entry" ) )
      {
         m_entry = pin->Attribute("entry" );
      }
      if ( pin->Attribute("uri" ) )
      {
         m_uriMB = pin->Attribute("uri" );
         m_uri = QString::fromStdString( m_uriMB ).toStdWString();
      }
      return true;
   }
};

// ============================================================================
struct PluginInfoList
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
      s << "PluginInfoList.Directory = " << m_vstDir << "\n";
      s << "PluginInfoList.Count = " << m_list.size() << "\n";

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
      tinyxml2::XMLElement* root = doc.NewElement( "PluginInfoList" );

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

      std::cout <<"Saved PluginInfoList XML " << uri << std::endl;

      return true;
   }

   bool
   loadXml( std::string const & uri )
   {
      tinyxml2::XMLDocument doc;
      auto e = doc.LoadFile( uri.c_str() );
      if ( e != tinyxml2::XML_SUCCESS )
      {
         std::cout << "Cant load PluginInfoList xml " << uri << std::endl;
         return false;
      }

      tinyxml2::XMLElement* root = doc.FirstChildElement( "PluginInfoList" );
      if ( !root )
      {
         std::cout << "No PluginInfoList tag in xml " << uri << std::endl;
         return false;
      }

      m_vstDirMB = root->Attribute( "directory" );
      m_checkSum = root->IntAttribute( "count" );
      m_vstDir = QString::fromStdString( m_vstDirMB ).toStdWString();

      m_list.clear();

      // Read all children <PluginInfo> of parent <PluginInfoList>

      // Read first child <PluginInfo> of parent <PluginInfoList>
      tinyxml2::XMLElement* pin = root->FirstChildElement( "PluginInfo" );
      if ( !pin )
      {
         std::cout << "No PluginInfo in xml " << uri << std::endl;
         return true;
      }

      // Read all next children <PluginInfo> of parent <PluginInfoList>
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

      std::cout <<"Loaded PluginInfoList XML " << uri << std::endl;
      std::cout << "[loadXml] dir = " << m_vstDirMB << std::endl;
      std::cout << "[loadXml] expectPlugins = " << m_checkSum << std::endl;
      std::cout << "[loadXml] loadedPlugins = " << k << std::endl;

      return true;
   }

};

} // end namespace audio
} // end namespace de

#endif // G_LOPASS1_HPP
