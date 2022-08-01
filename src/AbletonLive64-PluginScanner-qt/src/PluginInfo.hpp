#ifndef DE_AUDIO_PLUGININFO_HPP
#define DE_AUDIO_PLUGININFO_HPP

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

} // end namespace audio
} // end namespace de

#endif // G_LOPASS1_HPP
