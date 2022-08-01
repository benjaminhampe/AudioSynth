#include "GetPluginInfo.hpp"
#include <array>

// For Window64 mostly, should also work on Linux64 emulating windows using wine or such.
#ifndef UNICODE
#define UNICODE
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0600 // CreateEventEx() needs atleast this API version = WinXP or so.

#include <windows.h>
#include <synchapi.h>
#include <process.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
//#include <commdlg.h>
//#include <QFileDialog>
//#define ASSERT_THROW(c,e) if(!(c)) { throw std::runtime_error(e); }
//#define CLOSE_HANDLE(x)   if((x)) { CloseHandle(x); x = nullptr; }
//#define RELEASE(x)        if((x)) { (x)->Release(); x = nullptr; }

//#pragma warning(push)
//#pragma warning(disable : 4996)
#include <pluginterfaces/vst2.x/aeffectx.h>  // de_vst2sdk
#include <pluginterfaces/vst2.x/aeffect.h>
//#pragma warning(pop)

// Plugin needs path/directory of itself
inline std::string
toMultiByte( std::wstring const & msg )
{
   if ( msg.empty() ) return "";

   std::array< char, 4096 > mbuf{ 0 };

   auto n = WideCharToMultiByte( CP_OEMCP, 0, msg.data(), -1, mbuf.data(), mbuf.size(), 0, 0 );

   return mbuf.data();
}

// // Plugin needs path/directory of itself
// inline std::wstring
// getFullPathNameW( std::wstring const & msg )
// {
   // wchar_t buf[ MAX_PATH + 1 ] {};
   // wchar_t* namePtr = nullptr;
   // auto const r = GetFullPathName( msg.c_str(), _countof(buf), buf, &namePtr );
   // if ( r && namePtr )
   // {
      // return std::wstring( namePtr );
   // }
   // return std::wstring();
// }
#include <iostream>

namespace de {
namespace audio {

//ComInit::ComInit()  { CoInitializeEx(nullptr, COINIT_MULTITHREADED); }
//ComInit::~ComInit() { CoUninitialize(); }

// static
VstIntPtr
DummyHostCallback_static( AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void *ptr, float opt )
{
   switch( opcode )
   {
      case audioMasterVersion:    return kVstVersion;
      default:                    return 0;
   }
}

intptr_t
vstDispatch( AEffect* vst, int32_t opcode, int32_t index = 0, intptr_t value = 0, void* ptr = nullptr,
                float opt = 0.0f )
{
   if ( !vst ) { return 0; }
   return vst->dispatcher( vst, opcode, index, value, ptr, opt );
}

int
vstGetParamCount( void* plugin, int32_t i )
{
   AEffect* vst = reinterpret_cast< AEffect* >( plugin );
   if ( !vst ) return 0;
   return vst->numParams;
}

float
vstGetParamValue( void* plugin, int32_t i )
{
   AEffect* vst = reinterpret_cast< AEffect* >( plugin );
   if ( !vst ) return 0.0f;

   if ( i < 0 || i >= vst->numParams )
   {
      return 0.0f;
   }

   if ( !vst->getParameter )
   {
      return 0.0f;
   }

   return vst->getParameter( vst, i );
}

std::string
vstGetParamName( void* plugin, int32_t i, std::vector< char > & buf )
{
   AEffect* vst = reinterpret_cast< AEffect* >( plugin );
   if ( !vst ) return "";
   if ( i < 0 || i >= vst->numParams ) { return ""; }

   ::memset( buf.data(), 0, buf.size() );
   vstDispatch( vst, effGetParamName, i, 0, buf.data() );
   std::string msg;
   if ( buf.data() && buf.size() > 1 && buf[ 0 ] != 0 )
   {
      msg = buf.data();
   }

   return msg;
}

std::string
vstGetParamLabel( void* plugin, int32_t i, std::vector< char > & buf )
{
   AEffect* vst = reinterpret_cast< AEffect* >( plugin );
   if ( !vst ) return "";
   if ( i < 0 || i >= vst->numParams ) { return ""; }

   ::memset( buf.data(), 0, buf.size() );
   vstDispatch( vst, effGetParamLabel, i, 0, buf.data() );
   std::string msg;
   if ( buf.data() && buf.size() > 1 && buf[ 0 ] != 0 )
   {
      msg = buf.data();
   }

   return msg;
}

std::string
vstGetParamDisplay( void* plugin, int32_t i, std::vector< char > & buf )
{
   AEffect* vst = reinterpret_cast< AEffect* >( plugin );
   if ( !vst ) return "";
   if ( i < 0 || i >= vst->numParams ) { return ""; }

   ::memset( buf.data(), 0, buf.size() );
   vstDispatch( vst, effGetParamDisplay, i, 0, buf.data() );
   std::string msg;
   if ( buf.data() && buf.size() > 1 && buf[ 0 ] != 0 )
   {
      msg = buf.data();
   }

   return msg;
}

PluginInfo
getPluginInfo( std::wstring uri )
{
   std::wcout << "Scan DLL " << uri << std::endl;

   PluginInfo pin;
   pin.m_uri = uri;
   pin.m_uriMB = toMultiByte( uri );
   pin.m_nameMB = de::FileSystem::fileBase( pin.m_uriMB );
   dbStrReplace( pin.m_nameMB, "&apos;", "'" );
   pin.m_name = QString::fromStdString( pin.m_uriMB ).toStdWString();
   pin.m_isFile = false;
   pin.m_isPlugin = false;

//   if ( !dbExistFile( uri ) )
//   {
//      std::wcout << "No uri " << uri << std::endl;
//      return pin;
//   }

   HMODULE dll = LoadLibrary( uri.c_str() );
   if ( !dll )
   {
      std::wcout << "Can't open dll, probably not in 64 bit." << uri << std::endl;
      return pin;
   }

   typedef AEffect* (VstEntryProc)(audioMasterCallback);
   auto* fn_entryProc = reinterpret_cast< VstEntryProc* >( GetProcAddress(dll, "VSTPluginMain") );
   pin.m_entry="VSTPluginMain";
   if ( !fn_entryProc )
   {
      std::wcout << "No entry VSTPluginMain(), fallback to main... " << uri << std::endl;
      fn_entryProc = reinterpret_cast< VstEntryProc* >( GetProcAddress(dll, "main") );
      pin.m_entry="main";
   }
   if ( !fn_entryProc )
   {
      std::wcout << "No entry VSTPluginMain() or main() found, " << uri << std::endl;
      return pin;
   }

   AEffect* m_vst = fn_entryProc( DummyHostCallback_static );
   if ( !m_vst )
   {
      std::wcout << "Not a VST plugin dll, " << uri << std::endl;
      return pin;
   }

   if ( m_vst->magic != kEffectMagic )
   {
      std::wcout << "Not a VST plugin with kEffectMagic, " << uri << std::endl;
      return pin;
   }

   pin.m_isPlugin = true; // Wow a DLL plugin:
   pin.m_isVST2x = true; // Wow a VST plugin:
//   // Plugin needs path/directory of itself
//   {
//      wchar_t buf[ MAX_PATH + 1 ] {};
//      wchar_t* namePtr = nullptr;
//      auto const r = GetFullPathName( uri.c_str(), _countof(buf), buf, &namePtr );
//      if ( r && namePtr )
//      {
//         *namePtr = 0;
//         char mbBuf[ _countof(buf) * 4 ] {};
//         if ( auto s = WideCharToMultiByte(CP_OEMCP, 0, buf, -1, mbBuf, sizeof(mbBuf), 0, 0) )
//         {
//            pin.m_dirMultiByte = mbBuf;
//         }
//      }
//   }

   pin.m_numPrograms = m_vst->numPrograms;
   pin.m_numParams = m_vst->numParams;
   pin.m_numInputs = m_vst->numInputs;
   pin.m_numOutputs = m_vst->numOutputs;

   pin.m_isSynth = m_vst->flags & effFlagsIsSynth;
   pin.m_hasEditor = m_vst->flags & effFlagsHasEditor;
   pin.m_can32Bit = m_vst->flags & effFlagsCanReplacing;
   pin.m_can64Bit = m_vst->flags & effFlagsCanDoubleReplacing;
   pin.m_canProgramChunks = m_vst->flags & effFlagsProgramChunks;

   vstDispatch( m_vst, effOpen );
   //vstDispatch( m_vst, effSetProgram, 0, 0 );
//	effSetProgram,		///< [value]: new program number  @see AudioEffect::setProgram
//	effGetProgram,		///< [return value]: current program number  @see AudioEffect::getProgram
//	effSetProgramName,	///< [ptr]: char* with new program name, limited to #kVstMaxProgNameLen  @see AudioEffect::setProgramName
//	effGetProgramName,	///< [ptr]: char buffer for current program name, limited to #kVstMaxProgNameLen  @see AudioEffect::getProgramName

//	effGetParamLabel,	///< [ptr]: char buffer for parameter label, limited to #kVstMaxParamStrLen  @see AudioEffect::getParameterLabel
//	effGetParamDisplay,	///< [ptr]: char buffer for parameter display, limited to #kVstMaxParamStrLen  @see AudioEffect::getParameterDisplay
//	effGetParamName,	///< [ptr]: char buffer for parameter name, limited to #kVstMaxParamStrLen  @see AudioEffect::getParameterName
   std::ostringstream s;

//   if ( pin.m_numParams > 0 )
//   {

//      std::vector< char > buf( 4096, 0x00 );

//      for ( size_t i = 0; i < pin.m_numParams; ++i )
//      {
//         float paramValue = vstGetParamValue( m_vst, i );
//         std::string paramName = vstGetParamName( m_vst, i, buf );
//         std::string paramLabel = vstGetParamLabel( m_vst, i, buf );
//         std::string paramDisplay = vstGetParamDisplay( m_vst, i, buf );
//         s << "name(" << paramName << "), ";
//         s << "label(" << paramLabel << "), ";
//         s << "display(" << paramDisplay << "), ";
//         s << "value(" << paramValue << ")\n";

//         //   if ( i < pin.m_numParams - 1 )
//         //   {
//         //      s << ", ";
//         //   }
//      }
//   }

//   if ( pin.m_numPrograms > 0 )
//   {
//      std::vector< char > buf( 4096, 0x00 );

//      for ( size_t i = 0; i < pin.m_numPrograms; ++i )
//      {
//         ::memset( buf.data(), 0, buf.size() );
//         //vstDispatch( m_vst, effSetProgram, 0, i );
//         vstDispatch( m_vst, effGetProgramName, i, 0, buf.data() );
//         std::string programName = "";
//         if ( buf.data() && buf.size() > 1 && buf[ 0 ] != 0 )
//         {
//            programName = buf.data();
//         }
//         s << "Program[" << i << "].Name = " << programName << "\n";
//      }
//   }

   pin.m_comment = s.str();
   //vstDispatch(effSetSampleRate, 0, 0, 0, static_cast<float>(getSampleRate()));
   //vstDispatch(effSetBlockSize, 0, getBlockSize());
   //vstDispatch(effSetProcessPrecision, 0, kVstProcessPrecision32);
   //vstDispatch(effMainsChanged, 0, 1);
   //vstDispatch(effStartProcess);

   // if( hasEditor() )
   // {
      // m_editorWindow = new GVstEditorWindow( nullptr );
      // m_editorWindow->hide();
      // connect( m_editorWindow, SIGNAL(closed()), this, SLOT(on_editorClosed()), Qt::QueuedConnection );
      // vstDispatch(effEditOpen, 0, 0, (void*)m_editorWindow->winId() );
      // ERect* erc = nullptr;
      // vstDispatch(effEditGetRect, 0, 0, &erc);
      // int x = erc->left;
      // int y = erc->top;
      // int w = erc->right - x;
      // int h = erc->bottom - y;
      // resizeEditor( QRect( x,y,w,h ) );
      // m_editorWindow->show();
      // m_editorWindow->raise();
      // m_editorImage->setImage( m_editorWindow->grab().toImage().scaledToHeight( 128 ), true );
      // m_editorImage->setImagePreserveAspectWoH( true );
      // m_editorImage->show();
      // //m_editorPixmap = m_editorWindow->grab().scaledToHeight( 64 );
// //      m_loadButton->setIcon( QIcon( m_editorPixmap ) );
// //      m_loadButton->setIconSize( m_editorPixmap.size() );
// //      m_loadButton->setText("");
      // //ShowWindow(m_editorWinHandle, SW_SHOW);
   // }

//   vstDispatch( m_vst, effMainsChanged, 0, 0 );  // Stop plugin
//   vstDispatch( m_vst, effStopProcess );         // Stop plugin
//   vstDispatch( m_vst, effEditClose );
   vstDispatch( m_vst, effClose );               // Stop plugin

   if ( dll )                  // Close plugin
   {
      //HMODULE hModule = reinterpret_cast< HMODULE >( m_dllHandle );
      FreeLibrary( dll );
      //m_dllHandle = 0;
   }

   return pin;
}




} // end namespace audio
} // end namespace de
