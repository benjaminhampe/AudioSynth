#ifndef DE_LIVE6_FXPLUGIN_VST2X_HPP
#define DE_LIVE6_FXPLUGIN_VST2X_HPP

#include <App.hpp>
#include <de/audio/dsp/IDspChainElement.hpp>
#include <de/audio/plugin/ComInit.hpp>

#ifndef USE_BENNI_VST2x_HOST
#define USE_BENNI_VST2x_HOST 1
#endif

#ifdef USE_BENNI_VST2x_HOST

//#pragma warning(push)
//#pragma warning(disable : 4996)
#include <pluginterfaces/vst2.x/aeffectx.h>  // de_vst2sdk
//#pragma warning(pop)

#endif

// ============================================================================
class FxPluginEditorWindow : public QWidget
// ============================================================================
{
   Q_OBJECT
   bool m_enableClosing;
public:
   FxPluginEditorWindow( QWidget* parent = 0 ) : QWidget( parent ), m_enableClosing(false) {}
   ~FxPluginEditorWindow() override {}
signals:
   void closed();
public slots:
   void enableClosing() { m_enableClosing = true; }
   void disableClosing() { m_enableClosing = false; }
protected:
   void closeEvent( QCloseEvent* event ) override
   {
      if ( !m_enableClosing ) { event->ignore(); }
      hide();
      emit closed();
      //event->
   }
};

// ============================================================================
class FxPlugin : public QWidget, public de::audio::IDspChainElement
// ============================================================================
{
   DE_CREATE_LOGGER("FxPlugin")
   Q_OBJECT
public:
   FxPlugin( App & app, QWidget* parent = 0 );
   ~FxPlugin();
   std::wstring const & getUri() const { return m_uri; }
   bool isBypassed() const override { return !m_btnEnabled->isChecked(); }
   bool isVisibleMore() const { return m_btnMore->isChecked(); }
   bool isVisibleEditor() const { return m_btnEditor->isChecked(); }
   int numPrograms() const { return m_vst->numPrograms; }
   int numParams() const { return m_vst->numParams; }
   int numInputs() const { return m_vst->numInputs; }
   int numOutputs() const { return m_vst->numOutputs; }
   bool isSynth() const override { return getFlags(effFlagsIsSynth); }
   bool isEffect() const { return !isSynth(); }
   bool getFlags( int32_t m ) const { return (m_vst->flags & m) == m; }
   bool hasEditor() const { return getFlags(effFlagsHasEditor); }
   int getVendorVersion();
   std::string getVendorString();
   std::string getProductString();
   //uint32_t getSampleRate() const override;
   //uint64_t getSamplePos() const;
   //uint64_t getBlockSize() const;
   //uint64_t getChannelCount() const;

   uint64_t
   readSamples( double pts,
                float* dst,
                uint32_t dstFrames,
                uint32_t dstChannels,
                uint32_t dstRate ) override;
signals:
   void addedSynth( de::audio::IDspChainElement* );
   void removedSynth( de::audio::IDspChainElement* );
public slots:
   void on_focusChanged( bool focused );

   void setUri( std::wstring const & uri ) { m_uri = uri; }
   bool openPlugin();
   void closePlugin();
   void setBypassed( bool bypassed ) override { m_btnEnabled->setChecked( !bypassed ); }
   void setVisibleMore( bool visible ) { m_btnMore->setChecked( visible ); }
   void setVisibleEditor( bool visible ) { m_btnEditor->setChecked( visible ); }
   void sendNote( de::audio::Note const & note ) override;
   void aboutToStart( uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate ) override;
   void clearInputSignals() override { m_inputSignal = nullptr; }
   void setInputSignal( int i, de::audio::IDspChainElement* input ) override { m_inputSignal = input; }

protected slots:
   void on_bypassed( bool bypassed );
   void on_visibleMore( bool visible );
   void on_visibleEditor( bool visible );
   void on_editorClosed();
   //void on_loadButton( bool checked );

protected:
   //void resizeEvent( QResizeEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;
protected:
   App & m_app;
   QPushButton* m_btnEnabled;
   QPushButton* m_btnMore;
   QPushButton* m_btnEditor;
   QPushButton* m_btnLoadPreset;
   QPushButton* m_btnSavePreset;
   QImageWidget* m_title;
   QFont5x8 m_font5x8;
   int m_btnW;
   bool m_hasFocus;

#ifdef USE_BENNI_VST2x_HOST
   std::wstring m_uri; // Plugin file name

   bool m_isLoaded;
   bool m_isDirty;
   int m_volume;
   uint32_t m_sampleRate;     // rate in Hz
   uint32_t m_bufferFrames;   // frames per channel
   de::audio::IDspChainElement* m_inputSignal;
   FxPluginEditorWindow* m_editorWindow; // HWND
   //QPixmap m_editorPixmap;
   QImageWidget* m_editorImage;
   //uint64_t m_editorWinHandle; // HWND
   uint64_t m_dllHandle; // HMODULE
   AEffect* m_vst;
   std::atomic< uint64_t > m_framePos;
   VstTimeInfo m_timeInfo;
   std::string m_directoryMultiByte;

   // VST seems to work channelwise / planar, not interleaved audio.
   std::vector< float > m_outBuffer;
   std::vector< float* > m_outBufferHeads;
   std::vector< float > m_inBuffer;
   std::vector< float* > m_inBufferHeads;
   // VST midi event handling
   std::vector< VstMidiEvent > m_vstMidiEvents;
   std::vector< char > m_vstEventBuffer;

   struct
   {
      std::unique_lock< std::mutex >
      lock() const
      {
         return std::unique_lock<std::mutex>(m_mutex);
      }

      std::vector< VstMidiEvent > events;
   private:
      std::mutex mutable m_mutex;
   } m_vstMidi;




protected:

   static VstIntPtr
   hostCallback_static( AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void *ptr, float opt );

   VstIntPtr
   hostCallback(VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float);

   intptr_t
   dispatcher( int32_t opcode, int32_t index = 0, intptr_t value = 0, void *ptr = nullptr, float opt = 0.0f) const;

   // This function is called from refillCallback() which is running in audio thread.
   void
   processVstMidiEvents();

   // This function is called from refillCallback() which is running in audio thread.
//   float**
//   processAudio( uint64_t frameCount, uint64_t & outputFrameCount );

   void
   resizeEditor( QRect const & clientRc );

   const char**
   getCapabilities() const;

//   void setParameterProc( VstInt32 index, float parameter )
//   {

//   }

//   static void SetParameterProc_static( AEffect* effect, VstInt32 index, float value )
//   {
//      if ( !effect ) return;
//      DE_DEBUG("Set VST Parameter[",index," / ",effect->numParams,"] = ", value )
//   }

//   static float GetParameterProc_static( AEffect* effect, VstInt32 index )
//   {
//      if ( !effect ) return;
//      DE_DEBUG("Get VST Parameter[",index," / ",effect->numParams,"]")

//   }
#endif

};

#endif
