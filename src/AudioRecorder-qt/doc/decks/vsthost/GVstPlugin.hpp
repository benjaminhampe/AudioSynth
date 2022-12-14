#ifndef G_VST_PLUGIN_HPP
#define G_VST_PLUGIN_HPP

#include <QWidget>
#include <QImage>
//#include <QTimer>
//#include <QPainter>
//#include <QPaintEvent>
//#include <QResizeEvent>
//#include <QMouseEvent>
//#include <QKeyEvent>
#include <QDebug>
//#include <QThread>
#include <QSlider>
#include <QDial>
#include <QLabel>
#include <QCheckBox>

#include "GSliderV.hpp"
#include "GGroupV.hpp"


#include <cstdint>
#include <sstream>
#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include <memory>
#include <mutex>
#include <atomic>
#include <thread>

#include <de/audio/dsp/IDspChainElement.hpp>

#include <QRect>
#include <QFont5x8.hpp>

//#pragma warning(push)
//#pragma warning(disable : 4996)
#include <pluginterfaces/vst2.x/aeffectx.h>  // de_vst2sdk
//#pragma warning(pop)

#include <iostream>

struct ComInit
{
   ComInit();
   ~ComInit();
};

// ============================================================================
class GVstEditorWindow : public QWidget
// ============================================================================
{
   Q_OBJECT
   bool m_enableClosing;
public:
   GVstEditorWindow( QWidget* parent = 0 ) : QWidget( parent ), m_enableClosing(false) {}
   ~GVstEditorWindow() override {}
signals:
   void closed();
public slots:
   void enableClosing() { m_enableClosing = true; }
   void disableClosing() { m_enableClosing = false; }
protected:
   void closeEvent( QCloseEvent* event ) override
   {
      if ( !m_enableClosing )
      {
         event->ignore();
      }

      hide();
      //event->accept();
      emit closed();
   }
};

// ============================================================================
class GVstPlugin : public QWidget, public de::audio::IDspChainElement
// ============================================================================
{
   Q_OBJECT
   DE_CREATE_LOGGER("GVstPlugin")
   GGroupV* m_menu;
   QPushButton* m_loadButton;
   GSliderV* m_volumeSlider;

   QString m_uri; // Plugin file name
   bool m_isLoaded;
   bool m_isBypassed;
//   bool m_isSynth;
//   bool m_isEffect;
   int m_volume;
   uint32_t m_sampleRate;     // rate in Hz
   uint32_t m_bufferFrames;   // frames per channel
   uint32_t m_outputChannels; // >0 expected for all plugins, >2 will be limited to 2, =1 will be doubled to 2 (stereo).
   uint32_t m_inputChannels;  // = 0 for synths, >0 for all else effects
   de::audio::IDspChainElement* m_inputSignal;
   GVstEditorWindow* m_editorWindow; // HWND
   //QPixmap m_editorPixmap;
   QImageWidget* m_editorImage;
   //uint64_t m_editorWinHandle; // HWND
   uint64_t m_dllHandle; // HMODULE
   AEffect* m_aEffect;
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

public:
   GVstPlugin( QWidget* parent = 0 );
   ~GVstPlugin();

   bool isSynth() const { return m_inputChannels < 2 && m_outputChannels >= 2; }
   bool isEffect() const { return m_inputChannels >= 2 && m_outputChannels >= 2; }

   uint64_t
   readSamples( double pts,
                float* dst,
                uint32_t dstFrames,
                uint32_t dstChannels,
                uint32_t dstRate ) override;
signals:
//   void addedSynth( de::audio::synth::ISynth* );
//   void removedSynth( de::audio::synth::ISynth* );
public slots:
   void sendNote( de::audio::Note const & note ) override;
   void setupDspElement( uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate ) override;
   // VST effect plugins need audio input signals. VSTi synth plugins dont.
   void clearInputSignals();
   void setInputSignal( int i, de::audio::IDspChainElement* input );
   void setBypassed( bool enabled );
   void setVisibleButton(bool visible) { m_menu->setVisibleButton( visible ); }
   void setBypassButton(bool bypassed) { m_menu->setBypassButton( bypassed ); }
   //void setVisibleButton(bool visible) { m_menu->setVisibleButton( visible ); }

   bool open( std::wstring const & uri );
   void close();

private slots:
   void on_menuButton_bypass(bool checked);
   void on_menuButton_more(bool checked);
   void on_menuButton_hideAll(bool checked);
   void on_volumeSlider( int v );
   void on_loadButton( bool checked );
   void on_editorClosed();
protected:

//   uint32_t getSampleRate() const override;
//   uint64_t getSamplePos() const;
//   uint64_t getBlockSize() const;
//   uint64_t getChannelCount() const;
   std::string getVendorString();
   std::string getProductString();
   int         getVendorVersion();
//      effFlagsHasEditor     = 1 << 0,			///< set if the plug-in provides a custom editor
//      effFlagsCanReplacing  = 1 << 4,			///< supports replacing process mode (which should the default mode in VST 2.4)
//      effFlagsProgramChunks = 1 << 5,			///< program data is handled in formatless chunks
//      effFlagsIsSynth       = 1 << 8,			///< plug-in is a synth (VSTi), Host may assign mixer channels for its outputs
//      effFlagsNoSoundInStop = 1 << 9,			///< plug-in does not produce sound when input is all silence
//      effFlagsCanDoubleReplacing = 1 << 12,	///< plug-in supports double precision processing
//      DECLARE_VST_DEPRECATED (effFlagsHasClip) = 1 << 1,			///< deprecated in VST 2.4
//      DECLARE_VST_DEPRECATED (effFlagsHasVu)   = 1 << 2,			///< deprecated in VST 2.4
//      DECLARE_VST_DEPRECATED (effFlagsCanMono) = 1 << 3,			///< deprecated in VST 2.4
//      DECLARE_VST_DEPRECATED (effFlagsExtIsAsync)   = 1 << 10,	///< deprecated in VST 2.4
//      DECLARE_VST_DEPRECATED (effFlagsExtHasBuffer) = 1 << 11		///< deprecated in VST 2.4
   bool getFlags( int32_t m ) const;
   bool hasEditor() const;

   void
   sendMidiNote( int midiChannel, int noteNumber, int velocity );

private:

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
};

#endif // G_LOPASS1_HPP
