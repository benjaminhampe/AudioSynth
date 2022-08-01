#ifndef DE_AUDIO_PLUGININFO_HPP
#define DE_AUDIO_PLUGININFO_HPP

#include <cstdint>
#include <sstream>
#include <vector>
#include <iostream>

#include <DarkImage.hpp>
#include <tinyxml2/tinyxml2.h>
#include "StringConv.hpp"

namespace de {
namespace audio {

// ============================================================================
struct PluginType
// ============================================================================
{
   enum eType
   {
      Midi = 0,
      Audio,
      Synth,  // audio 2 midi
      eTypeCount
   };

   static std::string
   getString( eType type )
   {
      switch( type )
      {
         case Midi: return "Midi";
         case Audio: return "Audio";
         case Synth: return "Synth";
         default: return "None";
      }
   }
};

// ============================================================================
struct PluginInfo
// ============================================================================
{
   bool m_isFile;
   bool m_isPlugin;
   bool m_isVST2x;
   bool m_isBypassed;
   bool m_isSynth;
   bool m_hasEditor;
   bool m_can32Bit;
   bool m_can64Bit;
   bool m_canProgramChunks;
   //int m_vendorVersion;
   //uint32_t m_rate;     // rate in Hz
   uint32_t m_numPrograms;
   uint32_t m_numParams;
   uint32_t m_numOutputs;
   uint32_t m_numInputs;
   //uint32_t m_flags;
   std::wstring m_uri;
   std::wstring m_name;
   std::string m_entry;
   std::string m_comment;
   //std::string m_vendor;
   //std::string m_product;

   PluginInfo();

   int numPrograms() const { return m_numPrograms; }
   int numParams() const { return m_numParams; }
   int numInputs() const { return m_numInputs; }
   int numOutputs() const { return m_numOutputs; }

   bool hasEditor() const { return m_hasEditor; }
   bool isSynth() const { return m_isSynth; }
   bool isBypassed() const { return m_isBypassed; }

   void reset();

   std::wstring
   toWString( bool bUri = false ) const;

   void
   writeXML( tinyxml2::XMLDocument & doc, tinyxml2::XMLElement* list ) const;

   bool
   readXML( int i, tinyxml2::XMLElement* pin );
};


} // end namespace audio
} // end namespace de

#endif // G_LOPASS1_HPP
