#include "PluginInfo.hpp"

namespace de {
namespace audio {

PluginInfo::PluginInfo()
   : m_isBypassed( false )
{
   reset();
}

void
PluginInfo::reset()
{
   m_isFile = false;
   m_isPlugin = false;
   m_isVST2x = false;
   m_isBypassed = false;
   m_isSynth = false;
   m_hasEditor = false;
   m_can32Bit = false;
   m_can64Bit = false;
   m_canProgramChunks = false;
   //m_dirMultiByte = "";
   //m_pluginName = "";
   //m_rate = 0;     // rate in Hz
   m_numPrograms = 0;
   m_numParams = 0;
   m_numOutputs = 0;
   m_numInputs = 0;
   //m_flags = 0;

   m_uri = L"";
   m_name = L"";
   m_entry = "";
   m_comment = "";

   //m_vendorVersion = 0;
   //m_vendor;
   //m_product;
}

std::wstring
PluginInfo::toWString( bool bUri ) const
{
   std::wstringstream s;
   s <<
   "name(" << m_name << "), "
   "isBypassed(" << m_isBypassed << "), "
   "isSynth(" << m_isSynth << "), "
   "hasEditor(" << m_hasEditor << "), "
   "VST2x(" << m_isVST2x << "), "
   "nProgs(" << m_numPrograms << "), "
   "nParams(" << m_numParams << "), "
   "nOut(" << m_numOutputs << "), "
   "nIn(" << m_numInputs << "), "
   "32rp(" << m_can32Bit << "), "
   "64rp(" << m_can64Bit << "), "
   "entry(" << StringConv::toWStr( m_entry ) << ")";
   if ( bUri ) s  << ", uri(" << m_uri << ")";

   return s.str();
}

void
PluginInfo::writeXML( tinyxml2::XMLDocument & doc, tinyxml2::XMLElement* list ) const
{
   tinyxml2::XMLElement* pin = doc.NewElement( "plugin" );

   if ( m_name.empty() )
   {
      pin->SetAttribute("name", "none" );
   }
   else
   {
      pin->SetAttribute("name", StringConv::toStr(m_name).c_str() );
   }

   pin->SetAttribute("bypassed", int(m_isBypassed) );
   pin->SetAttribute("editor", int(m_hasEditor) );
   //pin->SetAttribute("plugin", int(m_isPlugin) );
   pin->SetAttribute("synth", int(m_isSynth) );
   pin->SetAttribute("programs", m_numPrograms );
   pin->SetAttribute("params", m_numParams );
   pin->SetAttribute("outputs", m_numOutputs );
   pin->SetAttribute("inputs", m_numInputs );
   pin->SetAttribute("can32Bit", int(m_can32Bit) );
   pin->SetAttribute("can64Bit", int(m_can64Bit) );

   if ( m_entry.empty() )
   {
      pin->SetAttribute("entry", "none" );
   }
   else
   {
      pin->SetAttribute("entry", m_entry.c_str() );
   }

   if ( m_uri.empty() )
   {
      pin->SetAttribute("uri", "none" );
   }
   else
   {
      pin->SetAttribute("uri", StringConv::toStr(m_uri).c_str() );
   }

   if ( m_comment.size() > 0 )
   {
      pin->SetText( m_comment.c_str() );
   }

   list->InsertEndChild( pin );
}

bool
PluginInfo::readXML( int i, tinyxml2::XMLElement* pin )
{
   if ( !pin->FindAttribute("uri") )
   {
      std::cout << "Plugin[" << i << "] :: No uri attrib in xml" << std::endl;
      return false;
   }

   if ( !pin || !pin->Name() ) return false;
   if ( std::string( pin->Name() ) != "plugin" ) return false;
   //pin->SetAttribute("type", "vst24" );

   reset();

   if ( pin->FindAttribute( "name" ) )
   {
      std::string name = pin->Attribute( "name" );
      m_name = StringConv::toWStr( name );
   }

   if ( pin->FindAttribute( "entry" ) )
   {
      m_entry = pin->Attribute( "entry" );
   }

   if ( pin->FindAttribute("uri" ) )
   {
      std::string uri = pin->Attribute("uri" );
      m_uri = StringConv::toWStr( uri );
   }

   m_isFile = true;
   m_isPlugin = true;
   m_isBypassed = false;

   if ( pin->FindAttribute("bypassed" ) )
   {
      m_isBypassed = pin->IntAttribute("bypassed") != 0;
   }

   if ( pin->FindAttribute("editor" ) )
   {
      m_hasEditor = pin->IntAttribute("editor") != 0;
   }

   if ( pin->FindAttribute("synth" ) )
   {
      m_isSynth = pin->IntAttribute("synth") != 0;
   }

   if ( pin->FindAttribute("programs" ) )
   {
      m_numPrograms = pin->UnsignedAttribute("programs");
   }

   if ( pin->FindAttribute("params" ) )
   {
      m_numParams = pin->UnsignedAttribute("params");
   }

   if ( pin->FindAttribute("outputs" ) )
   {
      m_numOutputs = pin->UnsignedAttribute("outputs");
   }

   if ( pin->FindAttribute("inputs" ) )
   {
      m_numInputs = pin->UnsignedAttribute("inputs");
   }

   if ( pin->FindAttribute("can32Bit" ) )
   {
      m_can32Bit = pin->IntAttribute("can32Bit") != 0;
   }

   if ( pin->FindAttribute("can64Bit" ) )
   {
      m_can64Bit = pin->IntAttribute("can64Bit") != 0;
   }

   return true;
}

} // end namespace audio
} // end namespace de

