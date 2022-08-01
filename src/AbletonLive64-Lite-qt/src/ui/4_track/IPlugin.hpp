#ifndef DE_LIVE_IPLUGIN_HPP
#define DE_LIVE_IPLUGIN_HPP

#include "LiveSkin.hpp"
#include "ImageButton.hpp"
#include <de/audio/dsp/IDspChainElement.hpp>

struct App;

// ============================================================================
class PluginEditorWindow : public QWidget
// ============================================================================
{
   Q_OBJECT
   bool m_enableClosing;
public:
   PluginEditorWindow( QWidget* parent = 0 );
   ~PluginEditorWindow() override;

signals:
   void closed();
public slots:
   void enableClosing();
   void disableClosing();
protected:
   void closeEvent( QCloseEvent* event ) override;
};

// ============================================================================
struct IPlugin : public QWidget, public de::audio::IDspChainElement
// ============================================================================
{
   Q_OBJECT
public:
   IPlugin( QWidget* parent = 0 ) : QWidget( parent ) {}
   ~IPlugin() = default;
signals:
   //void editorClosed();
public:
   // --- new mandatory interface ---
   virtual de::audio::PluginInfo const* pluginInfo() const = 0;
   virtual de::audio::PluginInfo* pluginInfo() = 0;

   // --- old interface IDspChainElement overridden ---
   bool isBypassed() const override { return pluginInfo()->isBypassed(); }
   bool isSynth() const override { return pluginInfo()->isSynth(); }

   // --- new mandatory interface ---
   virtual int id() const = 0;
public slots:
   void setBypassed( bool bypassed ) override { pluginInfo()->m_isBypassed = bypassed; }
   virtual void setId( int id ) = 0;

   // --- new mandatory interface ---
   virtual bool openPlugin( de::audio::PluginInfo const & pluginInfo ) = 0;
   virtual void closePlugin() = 0;
   // --- new mandatory interface ---
   virtual void moveEditor( int x, int y ) = 0;
   virtual void setEditorVisible( bool visible ) = 0;
public:
   virtual PluginEditorWindow* editorWindow() = 0;
   virtual QRect editorRect() const = 0;
   virtual bool isEditorVisible() const = 0;
   virtual bool isMoreVisible() const { return false; }
   //inline QSize editorSize() const { return QSize( editorW(), editorH() ); }
   //inline QPoint editorPos() const { return QPoint( editorX(), editorY() ); }

public slots:
   // --- new <optional> interface ---
   virtual void showEditor() { setEditorVisible( true ); }
   virtual void hideEditor() { setEditorVisible( false ); }
public:
   virtual std::wstring const & uri() const { return pluginInfo()->m_uri; }
   virtual bool hasEditor() const { return pluginInfo()->hasEditor(); }
   virtual int numPrograms() const { return pluginInfo()->numPrograms(); }
   virtual int numParams() const { return pluginInfo()->numParams(); }
   virtual int numInputs() const { return pluginInfo()->numInputs(); }
   virtual int numOutputs() const { return pluginInfo()->numOutputs(); }
   // --- new <optional> interface ---
   virtual int getVendorVersion() { return 1; }
   virtual std::string getVendorString() { return "IPluginVendor"; }
   virtual std::string getProductString() { return "IPluginProduct"; }

   //uint32_t getSampleRate() const override;
   //uint64_t getSamplePos() const;
   //uint64_t getBlockSize() const;
   //uint64_t getChannelCount() const;

};

#endif
