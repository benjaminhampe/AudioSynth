#ifndef DE_LIVE_AUDIO_PLUGIN_SHELL_WITH_LEVELMETER_HPP
#define DE_LIVE_AUDIO_PLUGIN_SHELL_WITH_LEVELMETER_HPP

#include "PluginVST2.hpp"
#include "AudioMeter.hpp"

// ============================================================================
class ShellTitle : public QWidget
// ============================================================================
{
   Q_OBJECT
public:
   ShellTitle( App & app, QWidget* parent = 0 );
   ~ShellTitle() {}
signals:
public slots:
   void setShellTitle( QString title ) { m_title = title; update(); }

protected:
   void paintEvent( QPaintEvent* event ) override;
private:
   App & m_app;
   QString m_title;
   QFont5x8 m_font5x8;
   QImage m_imgTitleH;
   QImage m_imgTitleV;
};

// ============================================================================
class ShellAudioMeter : public QWidget //, public de::audio::IDspChainElement
// ============================================================================
{
   Q_OBJECT
public:
   ShellAudioMeter( App & app, QWidget* parent = 0 );
   ~ShellAudioMeter() { stopUpdateTimer(); }
   bool hasFocus() const { return m_hasFocus; }
   uint64_t pushSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate );
   //uint64_t readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate ) override;

signals:

public slots:
   //void setInputSignal( int i, de::audio::IDspChainElement* input ) override { m_inputSignal = input; }
   //void clearInputSignals() override { m_inputSignal = nullptr; }
   void stopUpdateTimer() { if ( m_updateTimerId ) { killTimer( m_updateTimerId ); m_updateTimerId = 0; } }
   void startUpdateTimer() { stopUpdateTimer(); m_updateTimerId = startTimer( 37, Qt::CoarseTimer ); }
protected:
   void paintEvent( QPaintEvent* event ) override;
   //void resizeEvent( QResizeEvent* event ) override;
   void timerEvent( QTimerEvent* event) override;
   void enterEvent( QEvent* event ) override;
   void leaveEvent( QEvent* event ) override;
   void focusInEvent( QFocusEvent* event ) override;
   void focusOutEvent( QFocusEvent* event ) override;
private:
   DE_CREATE_LOGGER("ShellAudioMeter")
   App & m_app;
   bool m_isHovered;
   bool m_hasFocus;
   int m_updateTimerId;
   float m_Lmin;
   float m_Lmax;
   float m_Rmin;
   float m_Rmax;
   // Input signal ptr ( what sends audio to us )
   // Stereo audio input signal mostly ( this VST host is limited to stereo, yet )
   // This is good so we connect to the levelMeter endpoint of some previous Shell dsp element
   // This gives us a const pointer to previous shell, but shell can internally point to some vst
   // plugin or not without effecting the outer user made network graph. See LevelMeter endpoint
   //de::audio::IDspChainElement* m_inputSignal;
//   std::vector< float > m_accumBuffer;
   QFont5x8 m_font5x8;
   de::LinearColorGradient m_ColorGradient;
};

// ============================================================================
class ShellHeader : public QWidget
// ============================================================================
{
   Q_OBJECT
public:
   ShellHeader( App & app, QWidget* parent = 0 );
   ~ShellHeader() {}
signals:
   toggledMinimize( bool );
//   toggledBypassButton( bool );
//   toggledMoreButton( bool );
//   toggledEditorButton( bool );

public slots:
   void setTitle( QString title ) { m_title->setShellTitle( title ); }

   void setDirection( bool directionH )
   {
      if ( directionH )
      {
         setLayout( m_layoutH );
      }
      else // if ( direction )
      {
         setLayout( m_layoutV );
      }
   }

//   void setActiveButtonChecked( bool checked )
//   {
//      m_btnEnabled->blockSignals( true );
//      m_btnEnabled->setChecked( checked );
//      m_btnEnabled->blockSignals( false );
//   }

//   void setMoreButtonChecked( bool checked )
//   {
//      m_btnMore->blockSignals( true );
//      m_btnMore->setChecked( checked );
//      m_btnMore->blockSignals( false );
//   }

//   void setEditorButtonChecked( bool checked )
//   {
//      m_btnEditor->blockSignals( true );
//      m_btnEditor->setChecked( checked );
//      m_btnEditor->blockSignals( false );
//   }
protected slots:
//   void on_toggled_button_bypass(bool checked) { emit toggledBypassButton(checked); }
//   void on_toggled_button_more(bool checked) { emit toggledMoreButton(checked); }
//   void on_toggled_button_editor(bool checked) { emit toggledEditorButton(checked); }
protected:
   void paintEvent( QPaintEvent* event ) override;
   void mouseDoubleClickEvent( QMouseEvent* event ) override;

   ImageButton* createEnableButton(); // Bypass button
   ImageButton* createMoreButton(); // More button
   ImageButton* createEditorButton();  // Show/hide (plugin) editor window button
   ImageButton* createUpdateButton();  // UpdateFrom button
   ImageButton* createSaveButton(); // Save button
private:
   App & m_app;
   bool m_isMinimized;
   QHBoxLayout* m_layoutH;
   QVBoxLayout* m_layoutV;
   // LiveApp :: Widgets
public:
   ImageButton* m_btnEnabled;
   ImageButton* m_btnMore;
   ImageButton* m_btnEditor;

   ShellTitle* m_title;


   //   ImageButton* m_btnLoadPreset;
   //   ImageButton* m_btnSavePreset;
};


// Shell is a plugin wrapper/container and acts/behaves like a plugin
// This makes it more difficult for the code developer, not the code user
// This way one can insert Shells like Plugins in an array of IPlugins*.

// An audio shell has 1 audio plugin and 1 level meter.
// The level meter is deactive when plugin is bypassed -> gives user info.

// Shell can be collapsed/minimized to save screen space
// Shell has a text title, 1-5 buttons ( bypass, more, editor, ... )

// ============================================================================
class Shell : public IPlugin
// ============================================================================
{
   Q_OBJECT
public:
   Shell( App & app, QWidget* parent = 0 );
   ~Shell() override;
   // --- new mandatory interface ---
   de::audio::PluginInfo const* pluginInfo() const { return m_plugin ? m_plugin->pluginInfo() : nullptr; }
   de::audio::PluginInfo* pluginInfo() { return m_plugin ? m_plugin->pluginInfo() : nullptr; }
   // --- old interface IDspChainElement overridden ---
   //bool isBypassed() const override { return m_pluginInfo.m_isBypassed; }
   //bool isSynth() const override { return m_pluginInfo.m_isSynth; }
   // --- new mandatory interface ---
   int id() const override { return m_plugin ? m_plugin->id() : -1; }
public slots:
   void setId( int id ) override { if (m_plugin) m_plugin->setId( id ); }

   // --- new mandatory interface ---
   bool openPlugin( de::audio::PluginInfo const & pluginInfo ) { return m_plugin ? m_plugin->openPlugin( pluginInfo ) : false; }
   void closePlugin() override { if (m_plugin) m_plugin->closePlugin(); }

   // --- new mandatory interface ---
   void moveEditor( int x, int y ) override { if ( m_plugin ){ m_plugin->moveEditor( x, y ); } }
   void setEditorVisible( bool visible ) override;
public:
   PluginEditorWindow* editorWindow() override { return m_plugin ? m_plugin->editorWindow() : nullptr; }
   QRect editorRect() const override { return m_plugin ? m_plugin->editorRect() : QRect(); }
   bool isEditorVisible() const override { return m_plugin ? m_plugin->isEditorVisible() : false; }

   bool isMoreVisible() const override { return m_plugin ? m_plugin->isMoreVisible() : false; }

signals:
   //void ShellAudioMeterData( float l_low, float l_high, float r_low, float r_high );
   //void addedSynth( de::audio::IDspChainElement* );
   //void removedSynth( de::audio::IDspChainElement* );

public slots:
   void setPlugin( IPlugin* plugin );
   void setInputSignal( int i, de::audio::IDspChainElement* input ) override
   {
      m_inputSignal = input;
      if ( m_plugin ) m_plugin->setInputSignal( 0, m_inputSignal );
      else { DE_ERROR("No plugin") }
   }
   void clearInputSignals() override { m_inputSignal = nullptr; }
   uint64_t readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate ) override;

   void setBypassed( bool bypassed ) override;
   void setMoreVisible( bool visible );

protected slots:
   void on_editorClosed();
   void on_btn_active( bool checked );
   void on_btn_more( bool visible );
   void on_btn_editor( bool visible );
protected:
   //void timerEvent( QTimerEvent* event) override;
   void resizeEvent( QResizeEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;
   void enterEvent( QEvent* event ) override;
   void leaveEvent( QEvent* event ) override;
   void focusInEvent( QFocusEvent* event ) override;
   void focusOutEvent( QFocusEvent* event ) override;
   void mouseDoubleClickEvent( QMouseEvent* event ) override;
/*
   QSize sizeHint() const override;
   void mousePressEvent( QMouseEvent* event ) override;
   void mouseReleaseEvent( QMouseEvent* event ) override;
   void mouseMoveEvent( QMouseEvent* event ) override;
   void wheelEvent( QWheelEvent* event ) override;
   void keyPressEvent( QKeyEvent* event ) override;
   void keyReleaseEvent( QKeyEvent* event ) override;
*/
   void updateLayout();

private:

   DE_CREATE_LOGGER("Shell")
   App & m_app;
   bool m_isMinimized;
   bool m_isHovered;
   bool m_hasFocus;
   int m_updateTimerId;

   // To connect Shells, shells need input and are called by outputs using 'readSamples'
   // Called backward benni DSP chain. AudioEngine calls 'readSamples' first on last DSP chain element ( mixer )
   // and works back the chain to begin elements like synthesizers and players.
   //std::atomic< de::audio::IDspChainElement* > m_inputSignal;
   de::audio::IDspChainElement* m_inputSignal;

   // Input signal ptr ( what sends audio to us )
   // Stereo audio input signal mostly ( this VST host is limited to stereo, yet )
   // This is good so we connect to the levelMeter endpoint of some previous Shell dsp element
   // This gives us a const pointer to previous shell, but shell can internally point to some vst
   // plugin or not without effecting the outer user made network graph. See LevelMeter endpoint
   // Output signal ptr ( what gives manipulated sound to next chain element(s) )
   // Computes stereo only, yet
   // Next dsp element connects to shell and shell to m_levelMeter, and m_levelMeter is bypassed
   // for empty plugin, or is not bypassed when (vst) plugin is ready to play
   ShellHeader* m_header;
   ShellAudioMeter* m_meter;
   IPlugin* m_plugin;

//   // LiveApp :: Widgets
//   ImageButton* m_btnEnabled;
//   ImageButton* m_btnMore;
//   ImageButton* m_btnEditor;
   //   ImageButton* m_btnLoadPreset;
   //   ImageButton* m_btnSavePreset;

   // Editor window
   //PluginEditorWindow* m_editorWindow; // HWND
   // Layout
   QImage m_imgEditorContent;
   QRect m_rcPlugin;
   QRect m_rcHeader;
   QRect m_rcMeter;
};

#endif // DE_Q_IMAGEWIDGET_HPP
