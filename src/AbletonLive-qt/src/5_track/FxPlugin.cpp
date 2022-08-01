#include "FxPlugin.hpp"

// For Window64 mostly,
// should also work on Linux64 using wine or gcc.
#ifdef USE_BENNI_VST2x_HOST

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
   #include <QFileDialog>
   //#define ASSERT_THROW(c,e) if(!(c)) { throw std::runtime_error(e); }
   //#define CLOSE_HANDLE(x)   if((x)) { CloseHandle(x); x = nullptr; }
   //#define RELEASE(x)        if((x)) { (x)->Release(); x = nullptr; }

#endif

FxPlugin::FxPlugin( App & app, QWidget* parent )
   : QWidget( parent )
   , m_app( app )
{
   setObjectName( "FxPlugin" );
   setContentsMargins( 0,0,0,0 );

   m_hasFocus = false;
   m_btnW = 16;
   m_btnEnabled = new QPushButton( "I", this );
   m_btnEnabled->setToolTip("This DSP element is now (e)nabled = not bypassed");
   m_btnEnabled->setCheckable( true );
   m_btnEnabled->setChecked( true );
   m_btnEnabled->setMinimumSize( m_btnW,m_btnW );
   m_btnEnabled->setMaximumSize( 20,20 );

   m_btnMore = new QPushButton( "-", this );
   m_btnMore->setToolTip("All DSP options are visible now");
   m_btnMore->setCheckable( true );
   m_btnMore->setChecked( false );
   m_btnMore->setMinimumSize( 20,20 );
   m_btnMore->setMaximumSize( 20,20 );

   m_btnEditor = new QPushButton( "S", this );
   m_btnEditor->setToolTip( "This DSP element is fully visible now and (s)hown." );
   m_btnEditor->setCheckable( true );
   m_btnEditor->setChecked( false );
   m_btnEditor->setMinimumSize( 20,20 );
   m_btnEditor->setMaximumSize( 20,20 );

   m_title = new QImageWidget( this );
   m_title->setImage( QFont5x8().createImage(10,4,"FxPlugin"), true );

   QVBoxLayout* v = new QVBoxLayout();
   v->setContentsMargins( 1,1,1,1 );
   v->setSpacing( 1 );
   v->addWidget( m_btnEnabled );
   v->addStretch( 1 );
   v->addWidget( m_title );
   v->addStretch( 1 );
   v->addWidget( m_btnMore );
   v->addWidget( m_btnEditor );

   v->setAlignment( m_btnEnabled, Qt::AlignHCenter );
   v->setAlignment( m_title, Qt::AlignHCenter );
   v->setAlignment( m_btnMore, Qt::AlignHCenter );
   v->setAlignment( m_btnEditor, Qt::AlignHCenter );
   setLayout( v );

   connect( m_btnEnabled, SIGNAL(toggled(bool)), this, SLOT(on_bypassed(bool)) );
   connect( m_btnMore, SIGNAL(toggled(bool)), this, SLOT(on_visibleMore(bool)) );
   connect( m_btnEditor, SIGNAL(toggled(bool)), this, SLOT(on_visibleEditor(bool)) );

#ifdef USE_BENNI_VST2x_HOST
   m_isLoaded = false;
   m_isDirty = true;
   m_volume = 100;
   m_sampleRate = 0;
   m_bufferFrames = 0;
   m_inputSignal = nullptr;
   m_editorWindow = nullptr;
   m_dllHandle = 0; // HMODULE on Windows
   m_vst = nullptr;  // A real pointer to a C++ class
   m_framePos = 0;   // ?
#endif
   aboutToStart( 64, 2, 48000 );
}

FxPlugin::~FxPlugin()
{
   closePlugin();
}

void
FxPlugin::on_focusChanged( bool focused )
{
   m_hasFocus = focused;
   update();
}

void
FxPlugin::paintEvent( QPaintEvent* event )
{
   int w = width();
   int h = height();

   if ( w > 1 && h > 1 )
   {
      LiveSkin const & skin = m_app.m_skin;

      QPainter dc( this );
      dc.setRenderHint( QPainter::NonCosmeticDefaultPen );

      // Draw effect background
      dc.fillRect( rect(), m_hasFocus ? skin.focusColor : skin.panelColor );

      int px = 0;
      int py = 0;

      dc.setPen( Qt::NoPen );

      // Draw big content rr
      QRectF pos( px, py, w-2*px, h-2*py);
      dc.setBrush( QBrush( skin.contentColor ) );
      dc.drawRoundedRect( pos, skin.radius, skin.radius );

      // Draw title rr with second border
      pos = QRectF( px, py, w-2*px, skin.titleH + skin.radius );
      dc.setBrush( QBrush( skin.titleColor ) );
      dc.drawRoundedRect( pos, skin.radius, skin.radius );

      // Draw straight rect over title for correction
      pos = QRectF( px, py + skin.titleH, w-2*px, 2*skin.radius );
      dc.setBrush( QBrush( skin.contentColor ) );
      dc.drawRect( pos );

      m_font5x8.drawText( dc,
                  42,20, "Hypersonic",
                  skin.titleTextColor );

      dc.end();
   }

   QWidget::paintEvent( event );
}


void
FxPlugin::on_bypassed( bool bypassed )
{
   //m_isBypassed = checked;
}

void
FxPlugin::on_visibleMore( bool checked )
{
   //m_qpeak->setVisible( checked );
   //m_volume->setVisible( checked );
}

void
FxPlugin::on_visibleEditor( bool checked )
{
   if ( m_editorWindow )
   {
      if ( checked )
      {
         m_editorWindow->show();
      }
      else
      {
         m_editorWindow->hide();
      }
   }

}

void
FxPlugin::on_editorClosed()
{
   DE_ERROR("Editor closed")
   if ( m_btnEditor )
   {
      m_btnEditor->blockSignals( true );
      m_btnEditor->setChecked( false );
      m_btnEditor->blockSignals( false );
   }
}

bool
FxPlugin::openPlugin()
{
   if ( m_isLoaded )
   {
      std::wcout << "FxPlugin already loaded, " << m_uri << std::endl;
      closePlugin();
   }

   if ( m_uri.empty() )
   {
      std::wcout << "FxPlugin empty filename" << std::endl;
      return false;
   }

   // Plugin needs path/directory of itself
   {
      wchar_t buf[ MAX_PATH + 1 ] {};
      wchar_t* namePtr = nullptr;
      auto const r = GetFullPathName( m_uri.c_str(), _countof(buf), buf, &namePtr );
      if ( r && namePtr )
      {
         *namePtr = 0;
         char mbBuf[ _countof(buf) * 4 ] {};
         if ( auto s = WideCharToMultiByte(CP_OEMCP, 0, buf, -1, mbBuf, sizeof(mbBuf), 0, 0) )
         {
            m_directoryMultiByte = mbBuf;
         }
      }
   }

   HMODULE dll = reinterpret_cast< HMODULE >( m_dllHandle );
   dll = LoadLibrary( m_uri.c_str() );
   if ( !dll )
   {
      std::wcout << "Can't open VST DLL " << m_uri << std::endl;
      return false;
   }

   typedef AEffect* (VstEntryProc)(audioMasterCallback);
   auto* entryProc = reinterpret_cast< VstEntryProc* >( GetProcAddress(dll, "VSTPluginMain") );
   if ( !entryProc )
   {
      entryProc = reinterpret_cast< VstEntryProc* >( GetProcAddress(dll, "main") );
   }
   if ( !entryProc )
   {
      std::wcout << "No VST entry point found, " << m_uri << std::endl;
      return false;
   }

   m_dllHandle = uint64_t( dll );
   m_vst = entryProc( hostCallback_static );
   if ( !m_vst )
   {
      std::wcout << "Not a VST plugin I, " << m_uri << std::endl;
      return false;
   }

   if ( m_vst->magic != kEffectMagic )
   {
      std::wcout << "Not a VST plugin with kEffectMagic, " << m_uri << std::endl;
      return false;
   }

   m_vst->user = this;

   std::wcout << "VST open plugin '" << m_uri << "'" << std::endl;
   std::cout << "VST directoryMB '" << m_directoryMultiByte << "'" << std::endl;
   std::cout << "VST plugin programCount = " << numPrograms() << std::endl;
   std::cout << "VST plugin parameterCount = " << numParams() << std::endl;
   std::cout << "VST plugin inputCount = " << numInputs() << std::endl;
   std::cout << "VST plugin outputCount = " << numOutputs() << std::endl;
   std::cout << "VST plugin can float replacing = " << getFlags( effFlagsCanReplacing ) << std::endl;
   std::cout << "VST plugin can double replacing = " << getFlags( effFlagsCanDoubleReplacing ) << std::endl;
   std::cout << "VST plugin has editor = " << getFlags( effFlagsHasEditor ) << std::endl;
   std::cout << "VST plugin has program chunks = " << getFlags( effFlagsProgramChunks ) << std::endl;

//   m_inBuffer.resize( m_vst->numInputs * getBlockSize() );
//   for( int i = 0; i < m_vst->numInputs; ++i )
//   {
//      m_inBufferHeads.push_back( &m_inBuffer[ i * getBlockSize() ] );
//   }

//   m_outBuffer.resize( m_vst->numOutputs * getBlockSize() );
//   for( int i = 0; i < m_vst->numOutputs; ++i )
//   {
//      m_outBufferHeads.push_back( &m_outBuffer[ i * getBlockSize() ] );
//   }

   std::cout << "VST plugin uses " << m_bufferFrames << " frames per channel." << std::endl;

   dispatcher(effOpen);
   m_isDirty = true;
   aboutToStart( 64, 2, 48000 );

   if( hasEditor() )
   {
      m_editorWindow = new FxPluginEditorWindow( nullptr );
      m_editorWindow->hide();
      connect( m_editorWindow, SIGNAL(closed()), this, SLOT(on_editorClosed()), Qt::QueuedConnection );
      dispatcher(effEditOpen, 0, 0, (void*)m_editorWindow->winId() );
      ERect* erc = nullptr;
      dispatcher(effEditGetRect, 0, 0, &erc);
      int x = erc->left;
      int y = erc->top;
      int w = erc->right - x;
      int h = erc->bottom - y;
      resizeEditor( QRect( x,y,w,h ) );
      m_editorWindow->show();
      m_editorWindow->raise();
      m_editorImage->setImage( m_editorWindow->grab().toImage().scaledToHeight( 128 ), true );
      m_editorImage->setImagePreserveAspectWoH( true );
      m_editorImage->show();
      //m_editorPixmap = m_editorWindow->grab().scaledToHeight( 64 );
//      m_loadButton->setIcon( QIcon( m_editorPixmap ) );
//      m_loadButton->setIconSize( m_editorPixmap.size() );
//      m_loadButton->setText("");
      //ShowWindow(m_editorWinHandle, SW_SHOW);
   }

   if ( isSynth() )
   {
      emit addedSynth( this );
   }

   m_isLoaded = true;
   return m_isLoaded;
}


void
FxPlugin::closePlugin()
{
   if ( !m_isLoaded ) return; // Already closed

   m_isLoaded = false;  // Set this first, so the audio callback does bypass this dsp element.

   if ( isSynth() )
   {
      emit removedSynth( this ); // Unregister synth from MIDI keyboards
   }

   dispatcher(effMainsChanged, 0, 0);  // Stop plugin
   dispatcher(effStopProcess);         // Stop plugin

   if ( m_editorWindow )               // Stop plugin
   {                                   // Stop plugin
      m_editorWindow->enableClosing(); // Stop plugin
      dispatcher(effEditClose);        // Stop plugin
      m_editorWindow->close();         // Stop plugin
      delete m_editorWindow;           // Stop plugin
      m_editorWindow = nullptr;        // Stop plugin
   }                                   // Stop plugin

   dispatcher(effClose);               // Stop plugin

   if ( m_dllHandle )                  // Close plugin
   {
      HMODULE hModule = reinterpret_cast< HMODULE >( m_dllHandle );
      FreeLibrary(hModule);
      m_dllHandle = 0;
   }

   m_inBuffer.clear();
   m_inBufferHeads.clear();
   m_outBuffer.clear();
   m_outBufferHeads.clear();
   m_framePos = 0;

   //m_loadButton->setIcon( QIcon() );
   m_editorImage->hide();
}


void
FxPlugin::aboutToStart( uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
{
   if ( m_bufferFrames != dstFrames )
   {
      m_bufferFrames = dstFrames;
      m_isDirty = true;
   }

   if ( m_sampleRate != dstRate )
   {
      m_sampleRate = dstRate;
      m_isDirty = true;
   }

   if ( m_vst && m_isDirty )
   {
      m_isDirty = false;

      dispatcher(effStopProcess);
      dispatcher(effMainsChanged, 0, 0);

      // Prepare input buffer + input channel heads ( planar = non-interleaved )
      size_t chIn = numInputs();
      if ( chIn < 1 )
      {
         m_inBuffer.clear();
         m_inBufferHeads.clear();
      }
      else
      {
         auto nSamples = chIn * m_bufferFrames;
         if ( nSamples != m_inBuffer.size() )
         {
            m_inBuffer.resize( nSamples );
            for( size_t i = 0; i < chIn; ++i )
            {
               m_inBufferHeads.push_back( &m_inBuffer[ i * m_bufferFrames ] );
            }
         }
      }

      // Prepare output buffer + output channel heads ( planar = non-interleaved )
      size_t chOut = numOutputs();
      if ( chOut < 1 )
      {
         m_outBuffer.clear();
         m_outBufferHeads.clear();
      }
      else
      {
         auto nSamples = chOut * m_bufferFrames;
         if ( nSamples != m_outBuffer.size() )
         {
            m_outBuffer.resize( nSamples );
            for( size_t i = 0; i < chOut; ++i )
            {
               m_outBufferHeads.push_back( &m_outBuffer[ i * m_bufferFrames ] );
            }
         }
      }

      // Setup VST plugin
      dispatcher(effSetSampleRate, 0, 0, 0, float( m_sampleRate ) );
      dispatcher(effSetBlockSize, 0, m_bufferFrames);
      dispatcher(effSetProcessPrecision, 0, kVstProcessPrecision32);
      dispatcher(effMainsChanged, 0, 1);
      dispatcher(effStartProcess);
      dispatcher(effSetProgram, 0, 0, 0);
   }

}

uint64_t
FxPlugin::readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
{
   using namespace de::audio; // ...DSP_functions...

   // The block of samples to process now...
   uint64_t dstSamples = dstFrames * dstChannels;

   // Handle bypass
   if ( !m_isLoaded || isBypassed() )
   {
      if ( m_inputSignal )
      {
         m_inputSignal->readSamples( pts, dst, dstFrames, dstChannels, dstRate );
      }
      else
      {
         DSP_FILLZERO( dst, dstSamples );
      }
      return dstSamples;
   }

   aboutToStart( dstFrames, dstChannels, dstRate );

   // Process AUDIO input ( for effects, relays, mixer, delays, filters, etc... )
   size_t chIn = numInputs();
   if ( chIn > 0 )
   {
      if ( m_inputSignal )
      {
         m_inputSignal->readSamples( pts, dst, dstFrames, dstChannels, dstRate );
      }
      else
      {
         DSP_FILLZERO( dst, dstSamples );
      }
   }

   // Process AUDIO input ( write dst to buffer heads, pointing to inBuffer )
   if ( chIn > 0 )
   {
      DSP_GET_CHANNEL( m_inBufferHeads[0], dstFrames, dst, dstFrames, 0, dstChannels );
   }
   if ( chIn > 1 )
   {
      DSP_GET_CHANNEL( m_inBufferHeads[1], dstFrames, dst, dstFrames, 1, dstChannels );
   }

   // Process MIDI input ( for synthesizer )
   processVstMidiEvents();

   // Process AUDIO output ( for all plugins )
   m_vst->processReplacing( m_vst, m_inBufferHeads.data(), m_outBufferHeads.data(), dstFrames );
   m_framePos += dstFrames; // atomic.

   // Write AUDIO output to my interleaved stereo float32 DSP chain.
   auto pDst = dst;
   for ( size_t i = 0; i < dstFrames; ++i )
   {
      for ( size_t c = 0; c < dstChannels; ++c )
      {
         auto & channelData = m_outBufferHeads[ c ];
         *pDst++ = channelData[ i ];
      }
   }

   return dstSamples;
}

std::string
FxPlugin::getVendorString() { return "AbletonLive64-Lite"; }
std::string
FxPlugin::getProductString() { return "AbletonLive64-Lite"; }
int
FxPlugin::getVendorVersion() { return 1; }
const char**
FxPlugin::getCapabilities() const
{
   static const char* hostCapabilities[] =
   {
      "sendVstEvents",
      "sendVstMidiEvents",
      "sizeWindow",
      "startStopProcess",
      "sendVstMidiEventFlagIsRealtime",
      nullptr
   };
   return hostCapabilities;
}

//uint64_t
//FxPlugin::getSamplePos() const { return m_framePos; }
//uint32_t
//FxPlugin::getSampleRate() const { return m_sampleRate; }
//uint64_t
//FxPlugin::getBlockSize() const { return m_bufferFrames; }
//uint64_t
//FxPlugin::getChannelCount() const { return m_channelCount; }

//bool
//FxPlugin::isSynth() const { return getFlags(effFlagsIsSynth); }
intptr_t
FxPlugin::dispatcher( int32_t opcode, int32_t index, intptr_t value, void *ptr, float opt ) const
{
//   if ( !m_isLoaded )
//   {
//      DE_ERROR("No plugin loaded")
//      return 0;
//   }

   if ( !m_vst )
   {
      DE_ERROR("No plugin, bad")
      return 0;
   }
   return m_vst->dispatcher( m_vst, opcode, index, value, ptr, opt );
}
void
FxPlugin::sendNote( de::audio::Note const & note )
{
   int channel = note.m_channel & 0x0F;
   int midiNote = note.m_midiNote & 0x7F;
   int velocity = note.m_velocity & 0x7F;

   VstMidiEvent e;
   e.type        = kVstMidiType;
   e.byteSize    = sizeof( VstMidiEvent );
   e.flags       = kVstMidiEventIsRealtime;
   e.midiData[0] = static_cast<char>( channel + 0x90);
   e.midiData[1] = static_cast<char>( midiNote );
   e.midiData[2] = static_cast<char>( velocity );

   size_t n = 0;
   if ( auto l = m_vstMidi.lock() )
   {
      m_vstMidi.events.push_back( e );
      n = m_vstMidi.events.size();
   }
   DE_DEBUG("events(",n,"), channel(",channel,"), midiNote(", midiNote,"), velocity(", velocity,")")
}

void
FxPlugin::resizeEditor( QRect const & pos )
{
   if ( m_editorWindow )
   {
      m_editorWindow->setMinimumSize( pos.width(), pos.height() );
      m_editorWindow->setMaximumSize( pos.width(), pos.height() );
      m_editorWindow->move( pos.x(), pos.y() );
   }
}

// This function is called from refillCallback() which is running in audio thread.
void
FxPlugin::processVstMidiEvents()
{
   m_vstMidiEvents.clear();
   if ( auto l = m_vstMidi.lock() )
   {
      std::swap( m_vstMidiEvents, m_vstMidi.events );
      //m_vstMidi.events.clear();
   }

   if ( !m_vstMidiEvents.empty() )
   {
      auto const n = m_vstMidiEvents.size();
      auto const byteCount = sizeof( VstEvents ) + sizeof( VstEvent* ) * n;
      m_vstEventBuffer.resize( byteCount );
      auto vstEvents = reinterpret_cast< VstEvents* >( m_vstEventBuffer.data() );
      vstEvents->numEvents = n;
      vstEvents->reserved = 0;
      for ( size_t i = 0; i < n; ++i )
      {
         vstEvents->events[ i ] = reinterpret_cast< VstEvent* >( &m_vstMidiEvents[ i ] );
      }
      DE_ERROR("Dispatch MIDI n = ",n)
      dispatcher( effProcessEvents, 0, 0, vstEvents );
   }
}

// This function is called from refillCallback() which is running in audio thread.
//float**
//FxPlugin::processAudio( uint64_t frameCount, uint64_t & outputFrameCount )
//{
//   //frameCount = std::min( uint64_t(frameCount), uint64_t(m_outBuffer.size()) / m_outputChannels );
//   m_vst->processReplacing( m_vst, m_inBufferHeads.data(), m_outBufferHeads.data(), frameCount );
//   m_framePos += frameCount;
//   outputFrameCount = frameCount;
//   return m_outBufferHeads.data();
//}

// static
VstIntPtr
FxPlugin::hostCallback_static( AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void *ptr, float opt )
{
   if ( effect && effect->user )
   {
      auto me = static_cast< FxPlugin* >( effect->user );
      return me->hostCallback( opcode, index, value, ptr, opt );
   }

   switch( opcode )
   {
      case audioMasterVersion:    return kVstVersion;
      default:                    return 0;
   }
}

VstIntPtr
FxPlugin::hostCallback(VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float)
{
   switch(opcode)
   {
      default:                                break;
      case audioMasterVersion:                return kVstVersion;
      case audioMasterCurrentId:              return m_vst->uniqueID;
      case audioMasterGetSampleRate:          return m_sampleRate;
      case audioMasterGetBlockSize:           return m_bufferFrames;
      case audioMasterGetCurrentProcessLevel: return kVstProcessLevelUnknown;
      case audioMasterGetAutomationState:     return kVstAutomationOff;
      case audioMasterGetLanguage:            return kVstLangEnglish;
      case audioMasterGetVendorVersion:       return getVendorVersion();
      case audioMasterGetVendorString:
         strcpy_s(static_cast<char*>(ptr), kVstMaxVendorStrLen, getVendorString().c_str());
         return 1;
      case audioMasterGetProductString:
         strcpy_s(static_cast<char*>(ptr), kVstMaxProductStrLen, getProductString().c_str());
         return 1;
      case audioMasterGetTime:
         m_timeInfo.flags      = 0;
         m_timeInfo.samplePos  = m_framePos;
         m_timeInfo.sampleRate = m_sampleRate;
         //DE_DEBUG("audioMasterGetTime(",m_timeInfo.samplePos,")")
         return reinterpret_cast< VstIntPtr >( &m_timeInfo );
      case audioMasterGetDirectory:
         return reinterpret_cast< VstIntPtr >( m_directoryMultiByte.c_str() );
      case audioMasterIdle:
         if ( m_editorWindow ) { dispatcher(effEditIdle); } break;
      case audioMasterSizeWindow:
         if ( m_editorWindow )
         {
            //RECT rc {};
            //GetWindowRect(m_editorWinHandle, &rc);
            //rc.right = rc.left + static_cast<int>(index);
            //rc.bottom = rc.top + static_cast<int>(value);
            //resizeEditor(rc);
            int w = int( index );
            int h = int( value );
            int x = m_editorWindow->x();
            int y = m_editorWindow->y();
            DE_DEBUG("audioMasterSizeWindow(",w,",",h,"), pos(",x,",",y,")")
            resizeEditor( QRect(x,y,w,h) );
         }
         break;
      case audioMasterCanDo:
         for ( const char** pp = getCapabilities(); *pp; ++pp )
         {
            if ( strcmp(*pp, static_cast<const char*>(ptr)) == 0 )
            {
               return 1;
            }
         }
         return 0;
   }
   return 0;
}


/*
 *

    unsigned threadFunc() {
        ComInit comInit {};
        const HANDLE events[2] = { hClose, hRefillEvent };
        for(bool run = true; run; ) {
            const auto r = WaitForMultipleObjects(_countof(events), events, FALSE, INFINITE);
            if(WAIT_OBJECT_0 == r) {    // hClose
                run = false;
            } else if(WAIT_OBJECT_0+1 == r) {   // hRefillEvent
                UINT32 c = 0;
                audioClient->GetCurrentPadding(&c);

                const auto a = bufferFrameCount - c;
                float* data = nullptr;
                audioRenderClient->GetBuffer(a, reinterpret_cast<BYTE**>(&data));

                const auto r = refillFunc(data, a, mixFormat);
                audioRenderClient->ReleaseBuffer(a, r ? 0 : AUDCLNT_BUFFERFLAGS_SILENT);
            }
        }
        return 0;
    }

    HANDLE                  hThread { nullptr };
    IMMDeviceEnumerator*    mmDeviceEnumerator { nullptr };
    IMMDevice*              mmDevice { nullptr };
    IAudioClient*           audioClient { nullptr };
    IAudioRenderClient*     audioRenderClient { nullptr };
    WAVEFORMATEX*           mixFormat { nullptr };
    HANDLE                  hRefillEvent { nullptr };
    HANDLE                  hClose { nullptr };
    UINT32                  bufferFrameCount { 0 };
    RefillFunc              refillFunc {};
};


// This function is called from Wasapi::threadFunc() which is running in audio thread.
bool
refillCallback(
      VstPlugin& vstPlugin,
      float* const data,
      uint32_t availableFrameCount,
      const WAVEFORMATEX* const mixFormat)
{
    vstPlugin.processEvents();

    const auto nDstChannels = mixFormat->nChannels;
    const auto nSrcChannels = vstPlugin.getChannelCount();
    const auto vstSamplesPerBlock = vstPlugin.getBlockSize();

    int ofs = 0;
    while(availableFrameCount > 0) {
        size_t outputFrameCount = 0;
        float** vstOutput = vstPlugin.processAudio(availableFrameCount, outputFrameCount);

        // VST vstOutput[][] format :
        //  vstOutput[a][b]
        //      channel = a % vstPlugin.getChannelCount()
        //      frame   = b + floor(a/2) * vstPlugin.getBlockSize()

        // wasapi data[] format :
        //  data[x]
        //      channel = x % mixFormat->nChannels
        //      frame   = floor(x / mixFormat->nChannels);

        const auto nFrame = outputFrameCount;
        for(size_t iFrame = 0; iFrame < nFrame; ++iFrame) {
            for(size_t iChannel = 0; iChannel < nDstChannels; ++iChannel) {
                const int sChannel = iChannel % nSrcChannels;
                const int vstOutputPage = (iFrame / vstSamplesPerBlock) * sChannel + sChannel;
                const int vstOutputIndex = (iFrame % vstSamplesPerBlock);
                const int wasapiWriteIndex = iFrame * nDstChannels + iChannel;
                *(data + ofs + wasapiWriteIndex) = vstOutput[vstOutputPage][vstOutputIndex];
            }
        }

        availableFrameCount -= nFrame;
        ofs += nFrame * nDstChannels;
    }
    return true;
}


void mainLoop(const std::wstring& dllFilename)
{
    VstPlugin vstPlugin { dllFilename.c_str(), GetConsoleWindow() };

    Wasapi wasapi { [&vstPlugin](float* const data, uint32_t availableFrameCount, const WAVEFORMATEX* const mixFormat) {
        return refillCallback(vstPlugin, data, availableFrameCount, mixFormat);
    }};

    struct Key {
        Key(int midiNote) : midiNote { midiNote } {}
        int     midiNote {};
        bool    status { false };
    };

    std::map<int, Key> keyMap {
               {'2', {61}}, {'3', {63}},              {'5', {66}}, {'6', {68}}, {'7', {70}},
        {'Q', {60}}, {'W', {62}}, {'E', {64}}, {'R', {65}}, {'T', {67}}, {'Y', {69}}, {'U', {71}}, {'I', {72}},

               {'S', {49}}, {'D', {51}},              {'G', {54}}, {'H', {56}}, {'J', {58}},
        {'Z', {48}}, {'X', {50}}, {'C', {52}}, {'V', {53}}, {'B', {55}}, {'N', {57}}, {'M', {59}}, {VK_OEM_COMMA, {60}},
    };

    for(bool run = true; run; WaitMessage()) {
        MSG msg {};
        while(BOOL b = PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            if(b == -1) {
                run = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        for(auto& e : keyMap) {
            auto& key = e.second;
            const auto on = (GetKeyState(e.first) & 0x8000) != 0;
            if(key.status != on) {
                key.status = on;
                vstPlugin.sendMidiNote(0, key.midiNote, on, 100);
            }
        }
    }
}


int main() {
    volatile ComInit comInit;

    const auto dllFilename = []() -> std::wstring {
        wchar_t fn[MAX_PATH+1] {};
        OPENFILENAME ofn { sizeof(ofn) };
        ofn.lpstrFilter = L"VSTi DLL(*.dll)\0*.dll\0All Files(*.*)\0*.*\0\0";
        ofn.lpstrFile   = fn;
        ofn.nMaxFile    = _countof(fn);
        ofn.lpstrTitle  = L"Select VST DLL";
        ofn.Flags       = OFN_FILEMUSTEXIST | OFN_ENABLESIZING;
        GetOpenFileName(&ofn);
        return fn;
    } ();

    try {
        mainLoop(dllFilename);
    } catch(std::exception &e) {
        std::cout << "Exception : " << e.what() << std::endl;
    }
}

*/
