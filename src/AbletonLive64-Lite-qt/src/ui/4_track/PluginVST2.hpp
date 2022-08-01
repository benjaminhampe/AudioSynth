#ifndef DE_LIVE_PLUGIN_VST2X_HPP
#define DE_LIVE_PLUGIN_VST2X_HPP

#include "LiveSkin.hpp"
#include "ImageButton.hpp"
#include "IPlugin.hpp"
//#pragma warning(push)
//#pragma warning(disable : 4996)
#include <pluginterfaces/vst2.x/aeffectx.h>  // de_vst2sdk
//#pragma warning(pop)

struct App;

// ============================================================================
struct PluginVST2 : public IPlugin
// ============================================================================
{
   Q_OBJECT
public:
   PluginVST2( App & app, QWidget* parent = 0 );
   ~PluginVST2();
   // --- new mandatory interface ---
   de::audio::PluginInfo const* pluginInfo() const override { return &m_pluginInfo; }
   de::audio::PluginInfo* pluginInfo() override { return &m_pluginInfo; }
   // --- old interface IDspChainElement overridden ---
   //bool isBypassed() const override { return m_pluginInfo.m_isBypassed; }
   //bool isSynth() const override { return m_pluginInfo.m_isSynth; }
   // --- new mandatory interface ---
   int id() const override { return m_id; }

//   int getVendorVersion() { return 1; }
//   std::string getVendorString() { return "AbletonLive64-Lite"; }
//   std::string getProductString() { return "AbletonLive64-Lite"; }

public slots:
   void setId( int id ) override { m_id = id; }

   // --- new mandatory interface ---
   bool openPlugin( de::audio::PluginInfo const & pluginInfo ) override;
   void closePlugin() override;
   // --- new mandatory interface ---
   void moveEditor( int x, int y ) override { if ( m_editorWindow ) { m_editorWindow->move( x,y ); } }
   void setEditorVisible( bool visible ) override { if ( m_editorWindow ) { m_editorWindow->setVisible( visible ); } }

   void sendMidi( uint8_t byte1, uint8_t data1, uint8_t data2 ) override;
   void aboutToStart( uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate ) override;
   void clearInputSignals() override { m_inputSignal = nullptr; }
   void setInputSignal( int i, de::audio::IDspChainElement* input ) override { m_inputSignal = input; }
   uint64_t readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate ) override;

   //void setBypassed( bool bypassed ) override;

   void updateLayout();
public:
   PluginEditorWindow* editorWindow() override { return m_editorWindow; }
   QRect editorRect() const override { return m_editorWindow->rect(); }
   bool isEditorVisible() const override { return m_editorWindow->isVisible(); }
   bool isMoreVisible() const override { return m_isMoreVisible; }

   //uint32_t getSampleRate() const override;
   //uint64_t getSamplePos() const;
   //uint64_t getBlockSize() const;
   //uint64_t getChannelCount() const;

protected slots:
   void on_editorClosed();
protected:
   bool getFlags( int32_t m ) const
   {
      return m_vst ? ((m_vst->flags & m) == m) : 0;
   }

   static VstIntPtr
   hostCallback_static(
      AEffect* effect,
      VstInt32 opcode,
      VstInt32 index,
      VstIntPtr value,
      void *ptr,
      float opt );

   VstIntPtr
   hostCallback(
      VstInt32 opcode,
      VstInt32 index,
      VstIntPtr value,
      void* ptr,
      float opt );

   intptr_t
   dispatcher(
      int32_t opcode,
      int32_t index = 0,
      intptr_t value = 0,
      void *ptr = nullptr,
      float opt = 0.0f ) const;

   void
   processVstMidiEvents();

   const char**
   getCapabilities() const;

protected:
   //void timerEvent( QTimerEvent* event) override;
   void resizeEvent( QResizeEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;
   void enterEvent( QEvent* event ) override;
   void leaveEvent( QEvent* event ) override;
   void focusInEvent( QFocusEvent* event ) override;
   void focusOutEvent( QFocusEvent* event ) override;
   void mouseDoubleClickEvent( QMouseEvent* event ) override;

protected:
   DE_CREATE_LOGGER("PluginVST2")
   App & m_app;
public:
   de::audio::PluginInfo m_pluginInfo;
protected:
// #ifdef USE_BENNI_VST2x_HOST
   bool m_hasFocus;
   bool m_isLoaded;
   bool m_isDirty;
   bool m_isMoreVisible;

// LiveApp :: Widgets
//   ImageButton* m_btnEnabled;
//   ImageButton* m_btnMore;
//   ImageButton* m_btnEditor;
   ImageButton* m_btnLoadPreset;
   ImageButton* m_btnSavePreset;

   //bool m_isMore;
   int m_id;
   uint32_t m_sampleRate;     // rate in Hz
   uint32_t m_bufferFrames;   // frames per channel
   de::audio::IDspChainElement* m_inputSignal;
   uint64_t m_dllHandle; // HMODULE
   AEffect* m_vst;
   PluginEditorWindow* m_editorWindow; // HWND
   //QRect m_editorRect;

   std::atomic< uint64_t > m_framePos;
   std::wstring m_uri;                 // PluginVST2 file name
   std::string m_directoryMultiByte;
   VstTimeInfo m_timeInfo;
   // VST seems to work channelwise / planar, not interleaved audio.
   std::vector< float > m_outBuffer;
   std::vector< float*> m_outBufferHeads;
   std::vector< float > m_inBuffer;
   std::vector< float*> m_inBufferHeads;
   // VST midi event handling
   std::vector< VstMidiEvent > m_vstMidiEvents;
   std::vector< char > m_vstEventBuffer;
   struct
   {
      std::unique_lock< std::mutex >
      lock() const { return std::unique_lock<std::mutex>(m_mutex); }

      std::vector< VstMidiEvent > events;
   private:
      std::mutex mutable m_mutex;
   } m_vstMidi;
//#endif


   ImageButton* createEnableButton(); // Bypass button
   ImageButton* createMoreButton(); // More button
   ImageButton* createEditorButton();  // Show/hide (plugin) editor window button
   ImageButton* createUpdateButton();  // UpdateFrom button
   ImageButton* createSaveButton(); // Save button
};

#endif
