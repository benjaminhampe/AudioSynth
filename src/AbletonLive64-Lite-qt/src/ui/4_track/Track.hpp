/// (c) 2017 - 20180 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef DE_LIVE_TRACK_EDITOR_HPP
#define DE_LIVE_TRACK_EDITOR_HPP

#include "TrackInfo.hpp"
#include "MidiMeter.hpp"
#include "Shell.hpp"
#include "DropTarget.hpp"

#include <iterator>
#include <de/audio/dsp/IDspChainElement.hpp>
//#include <de/audio/plugin/PluginInfo.hpp>
//#include <de/audio/plugin/IPlugin.hpp>
//#include <de/audio/dsp/Mixer.hpp>

struct App;

// ============================================================================
struct Track : public QWidget, public de::audio::IDspChainElement
// ============================================================================
{
   Q_OBJECT
public:
   Track( App & app, QWidget* parent = nullptr );
   ~Track() override;

   int id() const { return trackInfo().id(); }
   bool isAudioOnly() const { return trackInfo().isAudioOnly(); }
   std::string const & name() const { return trackInfo().name(); }
   bool isBypassed() const override { return trackInfo().isBypassed(); }
   void setBypassed( bool bypassed ) override { trackInfo().setBypassed( bypassed ); }
   int32_t getVolume() const override { return trackInfo().m_volume; }

   void clearInputSignals() { m_audioInput = nullptr; }
   void setInputSignal( int i, IDspChainElement* input ) { m_audioInput = input; }

   uint64_t
   readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate ) override;

   void
   sendNote( de::audio::Note const & note ) override;

   void
   allNotesOff();

//   void writeXML( tinyxml2::XMLDocument & doc, tinyxml2::XMLElement* parent ) const;
//   bool readXML( int i, tinyxml2::XMLElement* spur );

//   Clips const & clips() const;
//   Clips & clips();
//   void addClip( Clip clip );
//   void addClip( double timeBeg, double timeEnd, std::string name = "Clip" );
   //bool addPlugin( std::wstring const & uri );
   //void updateDspChain();
   //void setTrackInfo( int trackId );
   //void updateFromTrackInfo();

signals:
   void audioMeterData( float l_low, float l_high, float r_low, float r_high );
   void addedSynth( de::audio::IDspChainElement* );
   void removedSynth( de::audio::IDspChainElement* );
public slots:
   bool addPlugin( de::audio::PluginInfo const & pluginInfo, bool stopAudio );
   void updateDspChain();
   void updateLayout();
   void clearWidgets();
   void setVolume( int volume );
protected slots:
protected:
   void resizeEvent( QResizeEvent* event ) override;
   //void paintEvent( QPaintEvent* event ) override;
   void enterEvent( QEvent* event ) override;
   void leaveEvent( QEvent* event ) override;
   void focusInEvent( QFocusEvent* event ) override;
   void focusOutEvent( QFocusEvent* event ) override;

   void dropEvent( QDropEvent* event ) override;
   void dragEnterEvent( QDragEnterEvent* event ) override;
   void dragLeaveEvent( QDragLeaveEvent* event ) override;
   void dragMoveEvent(QDragMoveEvent* event ) override;


public:
   DE_CREATE_LOGGER("Track")
   App & m_app;

   // +=========+
   // |TrackInfo|
   // +=========+
   // Interchangeable per user+session trackinfo.xml
   TrackInfo m_trackInfo;
   TrackInfo const & trackInfo() const { return m_trackInfo; }
   TrackInfo & trackInfo() { return m_trackInfo; }

   // +===========+
   // |TrackEditor|
   // +===========+

   //   | |
   //   |=|
   //   |=|
   //   |=|
   //   |=|
   //
   MidiMeter* m_midiMeter; // Not in Audio-Only
   //std::vector< IMidiSpurElement* > m_midiFx;// the rest of the audio Track is a series of effects.
   // 1st AudioPlugin after <last> MidiMeter
   //   // 2st AudioPlugin 1st Effectafter <last> MidiMeter

   // All (audio) plugin shells
   //IPlugin* m_audioPluginStart;            // Link between MIDI and audio chain, either a Player or Synthesizer
   //std::vector< IPlugin* > m_audioEffects; // Series of audio effects. // Not really used?

   std::vector< Shell* > m_audioShells;

   //
   DropTarget* m_dropTarget;

   IDspChainElement* m_audioInput; // Link to audio DSP input ( synths can have audio input )
   IDspChainElement* m_audioEnd; // Link to audio DSP end, for convenience to connect to a mixer.

};

#endif
