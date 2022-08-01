#include "PluginVST2.hpp"
#include "App.hpp"

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

// LevelMeter:
//   m_updateTimerId = 0;
//   m_Lmin = m_Lmax = m_Rmin = m_Rmax = 0.0f;
//   m_colorGradient.addStop( 0.0f, 0xFFFFFFFF );
//   m_colorGradient.addStop( 0.1f, 0xFF000000 );
//   m_colorGradient.addStop( 0.5f, 0xFF00FF00 );
//   m_colorGradient.addStop( 0.6f, 0xFF002000 );
//   m_colorGradient.addStop( 0.8f, 0xFF00FFFF );
//   m_colorGradient.addStop( 1.0f, 0xFF0000FF );
//   m_colorGradient.addStop( 1.1f, 0xFFFF00FF );


// ============================================================================
PluginEditorWindow::PluginEditorWindow( QWidget* parent )
// ============================================================================
   : QWidget( parent )
   , m_enableClosing(false)
{}

PluginEditorWindow::~PluginEditorWindow()
{}

void
PluginEditorWindow::enableClosing()
{
   m_enableClosing = true;
}

void
PluginEditorWindow::disableClosing()
{
   m_enableClosing = false;
}

void
PluginEditorWindow::closeEvent( QCloseEvent* event )
{
   if ( !m_enableClosing ) { event->ignore(); }
   hide();
   emit closed();
}

// ============================================================================
PluginVST2::PluginVST2( App & app, QWidget* parent )
// ============================================================================
   : IPlugin( parent )
   , m_app( app )
   , m_hasFocus( false )
   , m_isLoaded( false )
   , m_isDirty( true )
   , m_isMoreVisible( true )
   //, m_isMore( false )
// #ifdef USE_BENNI_VST2x_HOST
   , m_id( -1 )
   , m_sampleRate( 0 )
   , m_bufferFrames( 0 )
   , m_inputSignal( nullptr )
   , m_dllHandle( 0 )
   , m_vst( nullptr )
   , m_editorWindow( nullptr )
   , m_framePos( 0 )
// #endif
{
   setObjectName( "PluginVST2" );
   setContentsMargins( 0,0,0,0 );
   setMinimumSize( 158, 190 );
   setMaximumSize( 158, 190 );

   m_editorWindow = new PluginEditorWindow( nullptr );
   m_editorWindow->hide();
   connect( m_editorWindow, SIGNAL(closed()), this, SLOT(on_editorClosed()), Qt::QueuedConnection );

//   m_btnEnabled = createEnableButton();
//   m_btnMore = createMoreButton();
//   m_btnEditor = createEditorButton();
   m_btnLoadPreset = createUpdateButton();
   m_btnSavePreset = createSaveButton();

//   connect( m_btnEnabled, SIGNAL(toggled(bool)), this, SLOT(setBypassed(bool)) );
//   connect( m_btnEditor, SIGNAL(toggled(bool)), this, SLOT(setEditorVisible(bool)) );
//   connect( m_btnMore, SIGNAL(toggled(bool)), this, SLOT(setMoreVisible(bool)) );

   aboutToStart( 64, 2, 48000 );

   updateLayout();
//   startUpdateTimer();
}

PluginVST2::~PluginVST2()
{
//   stopUpdateTimer();
   closePlugin();
}

void
PluginVST2::on_editorClosed()
{
   DE_ERROR("Editor closed")
//   if ( m_btnEditor )
//   {
//      m_btnEditor->blockSignals( true );
//      m_btnEditor->setChecked( false );
//      m_btnEditor->blockSignals( false );
//   }
}


bool
PluginVST2::openPlugin( de::audio::PluginInfo const & pluginInfo )
{
   closePlugin();

   m_pluginInfo = pluginInfo;

   auto const & uri = m_pluginInfo.m_uri;

   setBypassed( true );

   if ( uri.empty() )
   {
      std::wcout << "PluginVST2 empty filename" << std::endl;
      return false;
   }

   // PluginVST2 needs path/directory of itself
   {
      wchar_t buf[ MAX_PATH + 1 ] {};
      wchar_t* namePtr = nullptr;
      auto const r = GetFullPathName( uri.c_str(), _countof(buf), buf, &namePtr );
      if ( r && namePtr )
      {
         *namePtr = 0;
         char mbBuf[ _countof(buf) * 4 ] {};
         int ok = WideCharToMultiByte(CP_OEMCP, 0, buf, -1, mbBuf, sizeof(mbBuf), 0, 0);
         if (ok)
         {
            m_directoryMultiByte = mbBuf;
         }
      }
   }

   HMODULE dll = reinterpret_cast< HMODULE >( m_dllHandle );
   dll = LoadLibrary( uri.c_str() );
   if ( !dll )
   {
      std::wcout << "Can't open VST DLL " << uri << std::endl;
      return false;
   }

   typedef AEffect* (VstEntryProc)(audioMasterCallback);
   auto* entryProc = reinterpret_cast< VstEntryProc* >( GetProcAddress(dll, "VSTPluginMain") );
   m_pluginInfo.m_entry = "VSTPluginMain";
   if ( !entryProc )
   {
      entryProc = reinterpret_cast< VstEntryProc* >( GetProcAddress(dll, "main") );
      m_pluginInfo.m_entry = "main";
   }
   if ( !entryProc )
   {
      std::wcout << "No VST entry point found, " << uri << std::endl;
      return false;
   }

   m_dllHandle = uint64_t( dll );
   m_vst = entryProc( hostCallback_static );
   if ( !m_vst )
   {
      std::wcout << "Not a VST plugin I, " << uri << std::endl;
      return false;
   }

   if ( m_vst->magic != kEffectMagic )
   {
      std::wcout << "Not a VST plugin with kEffectMagic, " << uri << std::endl;
      return false;
   }

   m_vst->user = this;

   m_pluginInfo.m_name = de::FileSystem::fileBase( uri );

   m_pluginInfo.m_numPrograms = m_vst->numPrograms;
   m_pluginInfo.m_numParams = m_vst->numParams;
   m_pluginInfo.m_numInputs = m_vst->numInputs;
   m_pluginInfo.m_numOutputs = m_vst->numOutputs;
   m_pluginInfo.m_isSynth = getFlags( effFlagsIsSynth );
   m_pluginInfo.m_hasEditor = getFlags( effFlagsHasEditor );

   DE_DEBUG("New ", dbStr(m_pluginInfo.toWString()) )
   //DE_DEBUG("VST directoryMB '", m_directoryMultiByte, "'")
   //DE_DEBUG("VST plugin-info '", dbStr(m_pluginInfo.toWString()), "'")

//   std::cout << "VST plugin is synth = " << m_isSynth << std::endl;
//   std::cout << "VST plugin has editor = " << m_hasEditor << std::endl;
//   std::cout << "VST plugin programCount = " << numPrograms() << std::endl;
//   std::cout << "VST plugin parameterCount = " << numParams() << std::endl;
//   std::cout << "VST plugin inputCount = " << numInputs() << std::endl;
//   std::cout << "VST plugin outputCount = " << numOutputs() << std::endl;
//   std::cout << "VST plugin can float replacing = " << getFlags( effFlagsCanReplacing ) << std::endl;
//   std::cout << "VST plugin can double replacing = " << getFlags( effFlagsCanDoubleReplacing ) << std::endl;
//   std::cout << "VST plugin has program chunks = " << getFlags( effFlagsProgramChunks ) << std::endl;

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

   //std::cout << "VST plugin uses " << m_bufferFrames << " frames per channel." << std::endl;

   dispatcher(effOpen);

   m_isDirty = true;
   aboutToStart(64, 2, 48000);

   if( hasEditor() )
   {
//      if ( m_editorWindow )
//      {
//         delete m_editorWindow;
//      }
//      m_editorWindow = new PluginEditorWindow( nullptr );
//      m_editorWindow->hide();

      connect( m_editorWindow, SIGNAL(closed()), this, SLOT(on_editorClosed()), Qt::QueuedConnection );

      dispatcher( effEditOpen, 0, 0, (void*)m_editorWindow->winId() );
      ERect* rc = nullptr;
      dispatcher( effEditGetRect, 0, 0, &rc );
      int x = rc->left;
      int y = rc->top;
      int w = rc->right - x;
      int h = rc->bottom - y;

      m_editorWindow->setMinimumSize( w, h );
      m_editorWindow->setMaximumSize( w, h );
      m_editorWindow->move( x, y );
//      m_editorWindow->show();
//      m_editorWindow->raise();
//      m_editorImage->setImage( m_editorWindow->grab().toImage().scaledToHeight( 128 ), true );
//      m_editorImage->setImagePreserveAspectWoH( true );
//      m_editorImage->show();
      //m_editorPixmap = m_editorWindow->grab().scaledToHeight( 64 );
//      m_loadButton->setIcon( QIcon( m_editorPixmap ) );
//      m_loadButton->setIconSize( m_editorPixmap.size() );
//      m_loadButton->setText("");
      //ShowWindow(m_editorWinHandle, SW_SHOW);

      hideEditor();
   }

//   if ( isSynth() )
//   {
//      emit addedSynth( this );
//   }

   setBypassed( m_pluginInfo.m_isBypassed );

   m_isLoaded = true;

   update();

   return m_isLoaded;
}

void
PluginVST2::closePlugin()
{
   if ( !m_vst )
   {
      // Already closed
      DE_WARN("No vst loaded")
      return;
   }
   else
   {

   }

   m_isLoaded = false;  // Set this first, so the audio callback does bypass this dsp element.


//   if ( isSynth() )
//   {
//      emit removedSynth( this ); // Unregister synth from MIDI keyboards
//   }

   DE_WARN("Stop vst plugin")

   dispatcher(effMainsChanged, 0, 0);  // Stop plugin
   dispatcher(effStopProcess);         // Stop plugin

   if ( m_editorWindow )               // Stop plugin
   {
      m_editorWindow->hide();
      DE_DEBUG("Delete m_editorWindow")  // Stop plugin
      m_editorWindow->enableClosing(); // Stop plugin
      dispatcher(effEditClose);        // Stop plugin
      if ( !m_editorWindow->close() )
      {
         DE_ERROR("Editor window not closed!")
      }
      else
      {
         DE_DEBUG("Editor window closed OK")
      }

      delete m_editorWindow;           // Stop plugin
      m_editorWindow = nullptr;        // Stop plugin
   }
   else
   {
      DE_ERROR("No m_editorWindow in plugin !?!?")
   }

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
   //m_editorImage->hide();
}


void
PluginVST2::updateLayout()
{
   int w = 158;
   int h = 190;

//   setMinimumSize( QSize( w,h ) );
//   setMaximumSize( QSize( w,h ) );
//   m_rcMeter = QRect( w-1-9,0,9,h );
//   setWidgetBounds( m_audioMeter, m_rcMeter );
//   //setWidgetBounds( m_levelMeter, m_rcMeter );
//   m_rcPanel = QRect( 0,0,w - 9,h );
//   m_rcHeader = QRect( 0,0,w - 9,17 );

   int x = 2;
   int y = 2;
//   setWidgetBounds( m_btnEnabled, QRect( x,y,13,13 ) ); x += 16;
//   setWidgetBounds( m_btnMore, QRect( x,y,13,13 ) ); x += 16;
//   setWidgetBounds( m_btnEditor, QRect( x,y,13,13 ) ); x += 16;

   x = 8;
   y = 22;
   setWidgetBounds( m_btnLoadPreset, QRect( x,y,13,13 ) ); x += 16;
   setWidgetBounds( m_btnSavePreset, QRect( x,y,13,13 ) ); x += 16;

   update();
}


void
PluginVST2::resizeEvent( QResizeEvent* event )
{
   updateLayout();
   QWidget::resizeEvent( event );
}

void
PluginVST2::paintEvent( QPaintEvent* event )
{
      int w = width();
      int h = height();

      auto const & skin = m_app.m_skin;
      int r = skin.getInt( LiveSkin::Radius );

      if ( w > 1 && h > 1 )
      {
         QPainter dc(this);
         dc.setRenderHint( QPainter::NonCosmeticDefaultPen );
         //dc.setBrush( Qt::NoBrush );

         drawRoundRectFill( dc, QRect(0,0,w,h), QColor( 100,100,250 ), r, r );

      }

   //   int r = skin.getInt( LiveSkin::Radius );
   //   int th = m_rcHeader.height();

   //   dc.setPen( Qt::NoPen );

   //   // Draw big content rr
   //   dc.setBrush( QBrush( skin.contentColor ) );
   //   dc.drawRoundedRect( m_rcPlugin, r, r );

   //   // Draw title rr with second border
   //   dc.setBrush( QBrush( skin.titleColor ) );

   //   dc.drawRoundedRect( QRect( m_rcPlugin.x(),
   //                           m_rcPlugin.y(),
   //                           m_rcPlugin.width(),
   //                           th + r ), r, r );

   //   // Draw straight rect over title for correction
   //   dc.setBrush( QBrush( skin.contentColor ) );
   //   dc.drawRect( QRect( m_rcPlugin.x(),
   //                    m_rcPlugin.y() + th,
   //                    m_rcPlugin.width(),
   //                    2*r ) );

   //   dc.setBrush( QBrush( skin.symbolColor ) );
   //   dc.drawRoundedRect( QRect( m_rcPlugin.x() + 9,
   //                           m_rcPlugin.y() + th + 25,
   //                           140, 120 ), r,r );

   //   m_font5x8.drawText( dc, 53,5, m_title, skin.symbolColor );


   QWidget::paintEvent( event );
}

void
PluginVST2::focusInEvent( QFocusEvent* event )
{
   m_hasFocus = true;
   update();
   QWidget::focusInEvent( event );
}

void
PluginVST2::focusOutEvent( QFocusEvent* event )
{
   m_hasFocus = true;
   update();
   QWidget::focusOutEvent( event );
}

void
PluginVST2::enterEvent( QEvent* event )
{
   QWidget::enterEvent( event );
}

void
PluginVST2::leaveEvent( QEvent* event )
{
   QWidget::leaveEvent( event );
}

void
PluginVST2::mouseDoubleClickEvent( QMouseEvent* event )
{
//   int mx = event->x();
//   int my = event->y();
   QWidget::mouseDoubleClickEvent( event );
}


void
PluginVST2::aboutToStart( uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
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
PluginVST2::readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
{
   using namespace de::audio; // ...DSP_functions...

   if ( !dst )
   {
      throw std::runtime_error("No dst audio dsp buffer in PluginVST2::readSamples()!");
   }

   // The block of samples to process now...
   uint64_t dstSamples = dstFrames * dstChannels;

   if ( m_inputSignal )
   {
      m_inputSignal->readSamples( pts, dst, dstFrames, dstChannels, dstRate );
   }
   else
   {
      DSP_FILLZERO( dst, dstSamples );
      if ( !isSynth() )
      {
         DE_ERROR("No input signal, uri = ", dbStr(m_pluginInfo.m_name) )
      }
   }

   // Handle bypass and unloaded state
   if ( !m_isLoaded )
   {
      DE_WARN("Not loaded, uri = ", dbStr(m_pluginInfo.m_name) )
      return dstSamples;
   }

   // Handle bypass and unloaded state
   if ( isBypassed() )
   {
      DE_WARN("isBypassed, uri = ", dbStr(m_pluginInfo.m_name) )
      return dstSamples;
   }

   DE_WARN("Process, uri = ", dbStr(m_pluginInfo.m_name) )

   // Handle non bypass case
   aboutToStart( dstFrames, dstChannels, dstRate );

   // Process AUDIO input ( for effects, relays, mixer, delays, filters, etc... )
   size_t chIn = numInputs();
   if ( chIn == 2 )
   {
      if ( m_inputSignal )
      {
         m_inputSignal->readSamples( pts, dst, dstFrames, dstChannels, dstRate );
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

   // assert( dst );

   // L
   if ( m_outBufferHeads.size() > 0 && dstChannels > 0 )
   {
      //DE_ERROR("L m_outBufferHeads.size() = ",m_outBufferHeads.size(),", dstChannels = ",dstChannels)
      auto pDst = dst;
      for ( size_t i = 0; i < dstFrames; ++i )
      {
         *pDst = m_outBufferHeads[ 0 ][ i ];
         pDst += dstChannels;
      }
   }

   // R
   if ( m_outBufferHeads.size() > 1 && dstChannels > 1 )
   {
      //DE_ERROR("R m_outBufferHeads.size() = ",m_outBufferHeads.size(),", dstChannels = ",dstChannels)
      auto pDst = dst + 1;
      for ( size_t i = 0; i < dstFrames; ++i )
      {
         *pDst = m_outBufferHeads[ 1 ][ i ];
         pDst += dstChannels;
      }
   }

//   Shell* shell = reinterpret_cast<Shell*>( parentWidget() );
//   if ( shell )
//      shell->update();
   return dstSamples;
}

const char**
PluginVST2::getCapabilities() const
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
//PluginVST2::getSamplePos() const { return m_framePos; }
//uint32_t
//PluginVST2::getSampleRate() const { return m_sampleRate; }
//uint64_t
//PluginVST2::getBlockSize() const { return m_bufferFrames; }
//uint64_t
//PluginVST2::getChannelCount() const { return m_channelCount; }

//bool
//PluginVST2::isSynth() const { return getFlags(effFlagsIsSynth); }
intptr_t
PluginVST2::dispatcher( int32_t opcode, int32_t index, intptr_t value, void *ptr, float opt ) const
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
PluginVST2::sendMidi( uint8_t byte1, uint8_t data1, uint8_t data2  )
{
   if ( !isSynth() )
   {
      return;
   }

   VstMidiEvent e;
   e.type        = kVstMidiType;
   e.byteSize    = sizeof( VstMidiEvent );
   e.flags       = kVstMidiEventIsRealtime;
   e.midiData[0] = static_cast<char>( byte1 );
   e.midiData[1] = static_cast<char>( data1 );
   e.midiData[2] = static_cast<char>( data2 );

   // If all note off
   if ( ((byte1 & 0xF0) == 0xB0) && (data1 == 0x7B) && (data2 == 0x00) )
   {
      if ( auto l = m_vstMidi.lock() )
      {
         m_vstMidi.events.clear();
      }
   }

   size_t n = 0;
   if ( auto l = m_vstMidi.lock() )
   {
      m_vstMidi.events.push_back( e );
      n = m_vstMidi.events.size();
   }
   DE_DEBUG("events(",n,"), byte1(",dbStrByte(byte1),"), data1(", dbStrByte(data1),"), data2(",  dbStrByte(data2),")")
}


// This function is called from refillCallback() which is running in audio thread.
void
PluginVST2::processVstMidiEvents()
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
      VstEvents* vstEvents = reinterpret_cast< VstEvents* >( m_vstEventBuffer.data() );
      memset( vstEvents, 0, sizeof( VstEvents ) );

      vstEvents->numEvents = n;
      vstEvents->reserved = 0;
      for ( size_t i = 0; i < n; ++i )
      {
         vstEvents->events[ i ] = reinterpret_cast< VstEvent* >( &m_vstMidiEvents[ i ] );
      }
      //DE_ERROR("Dispatch MIDI n = ",n)
      dispatcher( effProcessEvents, 0, 0, vstEvents );
   }
}

// This function is called from refillCallback() which is running in audio thread.
//float**
//PluginVST2::processAudio( uint64_t frameCount, uint64_t & outputFrameCount )
//{
//   //frameCount = std::min( uint64_t(frameCount), uint64_t(m_outBuffer.size()) / m_outputChannels );
//   m_vst->processReplacing( m_vst, m_inBufferHeads.data(), m_outBufferHeads.data(), frameCount );
//   m_framePos += frameCount;
//   outputFrameCount = frameCount;
//   return m_outBufferHeads.data();
//}



// static
VstIntPtr
PluginVST2::hostCallback_static( AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void *ptr, float opt )
{
   if ( effect && effect->user )
   {
      auto me = static_cast< PluginVST2* >( effect->user );
      return me->hostCallback( opcode, index, value, ptr, opt );
   }

   switch( opcode )
   {
      case audioMasterVersion:    return kVstVersion;
      default:                    return 0;
   }
}

VstIntPtr
PluginVST2::hostCallback(VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float)
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
            //DE_DEBUG("audioMasterSizeWindow(",w,",",h,"), pos(",x,",",y,")")
            setMinimumSize( w, h );
            //setMaximumSize( w, h );
            moveEditor( x, y );
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









ImageButton*
PluginVST2::createEnableButton()
{
   auto btn = new ImageButton( this );
   btn->setToolTip("This DSP element is now (e)nabled = not bypassed");

   LiveSkin const & skin = m_app.m_skin;

   int b = 13;

   btn->setCheckable( true );
   btn->setChecked( false );

   auto symColor = skin.symbolColor;
   auto bgColor = skin.windowColor;
   auto fgColor = skin.panelColor; // or panelColor

   //QFont font = getFontAwesome( 14 );
   std::string
   msg = "  #####\n"
         " #     #\n"
         "#       #\n"
         "#   #   #\n"
         "#   #   #\n"
         "#   #   #\n"
         "#       #\n"
         " #     #\n"
         "  #####\n";

   // [idle] has active color, means not bypassed
   QImage ico = createAsciiArt( skin.activeColor, fgColor, msg );
   QImage img = createCircleImage( b,b, bgColor, fgColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( symColor, fgColor, msg );
   img = createCircleImage( b,b, bgColor, fgColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );

   return btn;
}

ImageButton*
PluginVST2::createMoreButton()
{
   auto btn = new ImageButton( this );
   btn->setToolTip("All DSP options are visible now");

   LiveSkin const & skin = m_app.m_skin;
   int b = 13;

   btn->setCheckable( true );
   btn->setChecked( false );

   auto symColor = skin.symbolColor;
   auto bgColor = skin.windowColor;
   auto fgColor = skin.panelColor; // or panelColor

   //QFont font = getFontAwesome( 14 );
   std::string
   msg = "#######\n"
         " #####\n"
         " #####\n"
         "  ###\n"
         "  ###\n"
         "   #\n"
         "   #\n";
   // [idle]
   QImage ico = createAsciiArt( symColor, fgColor, msg );
   QImage img = createCircleImage( b,b, bgColor, fgColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   msg = "#\n"
         "###\n"
         "#####\n"
         "#######\n"
         "#####\n"
         "###\n"
         "#\n";
   ico = createAsciiArt( skin.activeColor, fgColor, msg );
   img = createCircleImage( b,b, bgColor, fgColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );

   return btn;
}


ImageButton*
PluginVST2::createEditorButton()
{
   auto btn = new ImageButton( this );
   LiveSkin const & skin = m_app.m_skin;
   int b = 13;

   btn->setCheckable( true );
   btn->setChecked( false );

   auto symColor = skin.symbolColor;
   auto bgColor = skin.windowColor;
   auto fgColor = skin.panelColor; // or panelColor

   //QFont font = getFontAwesome( 14 );
   std::string
   msg = "# #####\n"
         " \n"
         "# #####\n"
         " \n"
         "# #####\n";

   // [idle]
   QImage ico = createAsciiArt( symColor, fgColor, msg );
   QImage img = createCircleImage( b,b, bgColor, fgColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( skin.activeColor, fgColor, msg );
   img = createCircleImage( b,b, bgColor, fgColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );

   return btn;
}

ImageButton*
PluginVST2::createUpdateButton()
{
   auto btn = new ImageButton( this );
   LiveSkin const & skin = m_app.m_skin;
   int b = 13;

   btn->setCheckable( true );
   btn->setChecked( false );

   auto symColor = skin.symbolColor;
   auto bgColor = skin.windowColor;
   auto fgColor = skin.panelColor; // or panelColor

   //QFont font = getFontAwesome( 14 );
   std::string
   msg = "   ##\n"
         "  #\n"
         " #     #\n"
         "###   ###\n"
         " #     #\n"
         "      #\n"
         "    ##\n";

   // [idle]
   QImage ico = createAsciiArt( symColor, fgColor, msg );
   QImage img = createCircleImage( b,b, bgColor, fgColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( skin.activeColor, fgColor, msg );
   img = createCircleImage( b,b, bgColor, fgColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );

   return btn;
}


ImageButton*
PluginVST2::createSaveButton()
{
   auto btn = new ImageButton( this );
   LiveSkin const & skin = m_app.m_skin;
   int b = 13;

   btn->setCheckable( true );
   btn->setChecked( false );

   auto symColor = skin.symbolColor;
   auto bgColor = skin.windowColor;
   auto fgColor = skin.panelColor; // or panelColor

   //QFont font = getFontAwesome( 14 );
   std::string
   msg = "######\n"
         "##   ##\n"
         "##   ##\n"
         "#######\n"
         "#######\n"
         "#######\n"
         "#######\n";

   // [idle]
   QImage ico = createAsciiArt( symColor, fgColor, msg );
   QImage img = createCircleImage( b,b, bgColor, fgColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( skin.activeColor, fgColor, msg );
   img = createCircleImage( b,b, bgColor, fgColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );

   return btn;
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
