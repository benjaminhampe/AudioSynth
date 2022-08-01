/// (c) 2017 - 20180 Benjamin Hampe <benjaminhampe@gmx.de>

#include "Track.hpp"
#include "App.hpp"

Track::Track( App & app, QWidget* parent )
   : QWidget( parent )
   , m_app( app )
   , m_audioInput( nullptr )
   , m_audioEnd( nullptr )
   //, m_audioSynth( nullptr )
{
   setObjectName( "Track" );
   setContentsMargins( 0,0,0,0 );
   setMouseTracking( true );
   //setAcceptDrops( true );         // We can drop plugins ( Midi or Audio ) into this editor widget.

   m_midiMeter = new MidiMeter( m_app, this );

   m_dropTarget = new DropTarget( m_app, this );
   m_dropTarget->setAudioOnly( false );
}

Track::~Track()
{
   clearWidgets();
}

void
Track::sendNote( de::audio::Note const & note )
{
   if ( m_audioShells.empty() )
   {
      return;
   }
   auto plugin = m_audioShells.front();
   if ( !plugin )
   {
      return;
   }
   if ( !plugin->isSynth() )
   {
      return;
   }
   plugin->sendNote( note );
}

uint64_t
Track::readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
{
   //std::cout << "No audioEnd in track " << id() << std::endl;
   uint64_t dstSamples = dstFrames * dstChannels;
   de::audio::DSP_FILLZERO( dst, dstSamples );

//   if ( isBypassed() )
//   {
//      return dstSamples;
//   }

   if ( m_audioEnd )
   {
      uint64_t dstSamples = m_audioEnd->readSamples( pts, dst, dstFrames, dstChannels, dstRate );

      int iVolume = getVolume();
      float fVolume = 0.0001f * ( iVolume * iVolume );
      de::audio::DSP_MUL_LIMITED( dst, dstSamples, fVolume );
   }
   else
   {
      // DE_ERROR("No m_audioEnd")
   }

   return dstSamples;
}

void
Track::allNotesOff()
{
   if ( m_audioShells.empty() ) return;
   auto shell = m_audioShells.front();
   if ( shell ) shell->allNotesOff();
}

void
Track::setVolume( int volume )
{
   m_trackInfo.m_volume = volume;
}

bool
Track::addPlugin( de::audio::PluginInfo const & pluginInfo, bool stopAudio )
{
   std::string const uri = dbStr(pluginInfo.m_uri);

   DE_DEBUG("Add plugin, uri = ", uri )

   bool const isMidiPlugin = pluginInfo.isSynth(); // || (pluginInfo.m_numInputs < 2);
   if ( isMidiPlugin && isAudioOnly() )
   {
      DE_ERROR("Cant add syntheziser to an Audio-Only track (yet), uri = ", uri )
      return false;
   }

   if ( isAudioOnly() && pluginInfo.numInputs() < 2 )
   {
      DE_ERROR("Cant add input missing plugin to audio-only track (ever), uri = ", uri )
      return false;
   }

   // CREATE PLUGIN ( audio still playing )
   auto plugin = new PluginVST2( m_app, nullptr );
   if ( !plugin->openPlugin( pluginInfo ) )
   {
      DE_ERROR("Cant open VST audio plugin, uri = ", uri )
      delete plugin;
      return false;
   }

   // STOP AUDIO GENTLY ( no blocking )
   bool wasPlaying = m_app.isAudioMasterPlaying();
   m_app.stopAudioMaster();

   // DO something ASYNC while waiting for AUDIO to stop
   auto shell = new Shell( m_app, this );

   // STOP AUDIO NOT GENTLY ( blocking )
   m_app.stopAudioMasterBlocking();

   shell->setPlugin( plugin );

   // --- burocrazy ---

   // + m_audioPlugins alls list -> used in hideEditorWindows()

   if ( m_audioShells.empty() )
   {
      // REPLACE name
      if ( m_trackInfo.m_type != eTrackMaster )
      {
         m_trackInfo.m_name = QString::fromStdWString( plugin->m_pluginInfo.m_name ).toStdString();
      }

      m_audioShells.emplace_back( shell );
   }
   else
   {
      // Either ADD SYNTH PLUGIN ...
      if ( plugin->isSynth() )
      {
         // Get front
         auto f = m_audioShells.front();
         assert( f );

         // REPLACE old (front) with new SYNTH
         if ( f->isSynth() )
         {
            delete m_audioShells.front();
            m_audioShells.front() = shell;

            // REPLACE name
            if ( m_trackInfo.m_type != eTrackMaster )
            {
               m_trackInfo.m_name = QString::fromStdWString( plugin->m_pluginInfo.m_name ).toStdString();
            }
         }
         // or ADD SYNTH to front and shift all effects one to right
         else
         {
            std::vector< Shell* > shells;
            shells.reserve( m_audioShells.size() + 1 );
            shells.emplace_back( shell );
            shells.insert( shells.end(), m_audioShells.begin(), m_audioShells.end() );
            m_audioShells.swap( shells );

            // REPLACE name
            m_trackInfo.m_name = QString::fromStdWString( plugin->m_pluginInfo.m_name ).toStdString();
         }
      }
      // ... or ADD EFFECT PLUGIN
      else
      {
         m_audioShells.emplace_back( shell );
      }
   }

   updateDspChain();
   m_app.updateDspChain();
/*
   // Simply push back when empty or AudioOnly + Effect or Synth with input/relay channels
   if ( isAudioOnly() )
   {
      if ( m_audioShells.empty() )
      {
         // REPLACE name
         m_trackInfo.m_name = QString::fromStdWString( plugin->m_pluginInfo.m_name ).toStdString();
      }

      m_audioShells.emplace_back( shell );
   }
   else
   {
      if ( m_audioShells.empty() )
      {
         // REPLACE name
         m_trackInfo.m_name = QString::fromStdWString( plugin->m_pluginInfo.m_name ).toStdString();

         m_audioShells.emplace_back( shell );
      }
      else
      {
         auto f = m_audioShells.front();
         assert( f );

         // Always REPLACE SYNTH
         if ( f->isSynth() )
         {
            delete m_audioShells.front();
            m_audioShells.front() = shell;

            // REPLACE name
            m_trackInfo.m_name = QString::fromStdWString( plugin->m_pluginInfo.m_name ).toStdString();
         }
         // or ADD NEW EFFECT
         else
         {
            m_audioShells.emplace_back( shell );
         }

      }

   }
*/
   updateLayout();

   // Add PLUGIN-INFO to TRACK-INFO
   // REBUILD list TRACKINFO::m_ALL -> used in hideEditorWindows()

   auto & ti = m_trackInfo;

   ti.m_plugins.clear();

   for ( auto & p : m_audioShells )
   {
      if ( !p ) continue;
      ti.m_plugins.emplace_back( *p->pluginInfo() );
   }

   // RECONNECT DSP CHAIN


//   // RESTART AUDIO, if any
//   if ( stopAudio && wasPlaying )
//   {
//      playAudioMaster();
//   }

   return true;
}


void
Track::updateDspChain()
{
   // Determine END link of AUDIO DSP CHAIN, END starts with being first chain element.

   m_audioEnd = m_audioInput; // Start Endpoint is audio input

   if ( !m_audioInput )
   {
      if ( isAudioOnly() )
      {
         DE_ERROR("ERROR Nothing to hear, because no input signal, ", m_trackInfo.m_name )
      }
      else
      {
         //DE_ERROR("ERROR Nothing to hear, because no input signal, ", m_trackInfo.m_name )
      }

      // return; // false;
   }

//   if ( !isAudioOnly() )
//   {
//      if ( m_audioSynth )
//      {
//         m_audioEnd = m_audioSynth;
//      }
//      else
//      {
//         DE_ERROR("No synthesizer to connect")
//      }
//   }

//   if ( !m_audioEnd )
//   {
//      DE_ERROR("ERROR Nothing to hear")
//      return; // false;
//   }

   // WE HAVE ENDPOINT NOW, LETS CONNECT AUDIO FX CHAIN TO SYNTH,
   // MAKE LAST CHAIN ELEM THE NEW ENDPOINT OF ENTIRE CHAIN
   for ( auto shell : m_audioShells )
   {
      if ( shell )
      {
         shell->setInputSignal( 0, m_audioEnd ); // Conn to current dsp endpoint
         m_audioEnd = shell; // Replace current dsp endpoint with validated us.
      }
      else
      {
         DE_ERROR("Got nullptr, ", m_trackInfo.m_name)
      }
   }

   if ( !m_audioEnd )
   {
      DE_ERROR("ERROR Nothing to hear II, ", m_trackInfo.m_name)
      //return; // false;
   }

}


void
Track::clearWidgets()
{
   for ( auto & shell : m_audioShells )
   {
      if ( shell ) delete shell;
   }
   m_audioShells.clear();

#if 0
   if ( m_audioSynth )
   {
      delete m_audioSynth;
      m_audioSynth = nullptr;
   }

   for ( auto effect : m_audioEffects )
   {
      if ( effect ) delete effect;
   }
   m_audioEffects.clear();
#endif

   //m_audioInput = nullptr;
   m_audioEnd = m_audioInput;
}

void
Track::updateLayout()
{

   int h = height();
   int x = 0;
   int y = 0;

   int mmw = 4;   // Midi Meter Width
   int editorW = 158 + 9;

   // Layout Midi Meter
   setWidgetBounds( m_midiMeter, QRect(x,y,mmw,h) ); x += mmw + 2;

   // Layout Audio Plugin
   for ( auto & fx : m_audioShells )
   {
      if ( fx )
      {
         // Layout Audio Plugin
         //editorW = fx->maximumWidth();
         setWidgetBounds( fx, QRect(x,y,editorW,h) );
         x += editorW;
      }
   }

   // Layout Drop dummy
   int w = width();
   int dummyW = w - x;
   if ( dummyW < 50 ) dummyW = 50;
   setWidgetBounds( m_dropTarget, QRect(x,y,dummyW,h) );

#if 0
   if ( m_audioSynth )
   {
      //editorW = m_audioSynth->maximumWidth();
      setWidgetBounds( m_audioSynth, QRect(x,y,editorW,h) );
      x += editorW;
   }

   for ( size_t i = 0; i < m_audioEffects.size(); ++i )
   {
      auto fx = m_audioEffects[ i ];
      if ( fx )
      {
         // Layout Audio Plugin
         //editorW = fx->maximumWidth();
         setWidgetBounds( fx, QRect(x,y,editorW,h) );
         x += editorW;
      }
   }

   // Layout Drop dummy
   int dummyW = w - x;
   if ( dummyW < 50 ) dummyW = 50;
   setWidgetBounds( m_dropTarget, QRect(x,y,dummyW,h) );
#endif
   update();
}

/*
void
Plugin::updateLayout()
{
   int w = 17 + 9;
   int h = 190;

   if ( m_isMinimized )
   {
      setMinimumSize( QSize( w, h ) );
      setMaximumSize( QSize( w, h ) );
      m_rcPanel = QRect( 0,0,w - 9,h );
      m_rcMeter = QRect( w-1-9,0,9,h );
      m_rcHeader = m_rcPanel;
      setWidgetBounds( m_audioMeter, m_rcMeter );
      //setWidgetBounds( m_levelMeter, m_rcMeter );

      int x = 2;
      int y = 2;
      setWidgetBounds( m_btnEnabled, QRect( x,y,13,13 ) ); y += 16;
      setWidgetBounds( m_btnEditor, QRect( x,y,13,13 ) ); y += 16;
      setWidgetBounds( m_btnMore, QRect() );
      setWidgetBounds( m_btnLoadPreset, QRect() );
      setWidgetBounds( m_btnSavePreset, QRect() );
   }
   else
   {
      w = 158 + 9;
      setMinimumSize( QSize( w, h ) );
      setMaximumSize( QSize( w, h ) );
      m_rcMeter = QRect( w-1-9,0,9,h );
      setWidgetBounds( m_audioMeter, m_rcMeter );
      //setWidgetBounds( m_levelMeter, m_rcMeter );
      m_rcPanel = QRect( 0,0,w - 9,h );
      m_rcHeader = QRect( 0,0,w - 9,17 );

      int x = 2;
      int y = 2;
      setWidgetBounds( m_btnEnabled, QRect( x,y,13,13 ) ); x += 16;
      setWidgetBounds( m_btnMore, QRect( x,y,13,13 ) ); x += 16;
      setWidgetBounds( m_btnEditor, QRect( x,y,13,13 ) ); x += 16;

      x = 8;
      y = 22;
      setWidgetBounds( m_btnLoadPreset, QRect( x,y,13,13 ) ); x += 16;
      setWidgetBounds( m_btnSavePreset, QRect( x,y,13,13 ) ); x += 16;
   }

   update();
}
*/

void
Track::resizeEvent( QResizeEvent* event )
{
   updateLayout();
   QWidget::resizeEvent( event );
}

//void
//Track::paintEvent( QPaintEvent* event )
//{
//   QWidget::paintEvent( event );
//}


void
Track::dropEvent( QDropEvent* event )
{
   std::cout << "Track::" << __func__ << std::endl;
   std::cout << event->mimeData()->text().toStdString() << std::endl;

   std::wstring uri = event->mimeData()->text().toStdWString();
   m_app.addPlugin( uri, true );

   event->acceptProposedAction();
   QWidget::dropEvent( event );
}

void
Track::dragEnterEvent( QDragEnterEvent* event )
{
   if ( event->mimeData()->hasFormat("text/plain") )
   {
      event->acceptProposedAction();
   }
   std::cout << __func__ << std::endl;
   QWidget::dragEnterEvent( event );
}

void
Track::dragLeaveEvent( QDragLeaveEvent* event )
{
   std::cout << __func__ << std::endl;
   QWidget::dragLeaveEvent( event );
}

void
Track::dragMoveEvent(QDragMoveEvent* event )
{
   std::cout << __func__ << std::endl;
   QWidget::dragMoveEvent( event );
}

void
Track::focusInEvent( QFocusEvent* event )
{
   m_trackInfo.m_hasFocus = true;
   update();
   QWidget::focusInEvent( event );
}

void
Track::focusOutEvent( QFocusEvent* event )
{
   m_trackInfo.m_hasFocus = true;
   update();
   QWidget::focusOutEvent( event );
}

void
Track::enterEvent( QEvent* event )
{
   QWidget::enterEvent( event );
}

void
Track::leaveEvent( QEvent* event )
{
   QWidget::leaveEvent( event );
}



/*
   // Calc LevelMeter data

   // Now 'dst' contains valid audio data processed by VST plugin. Lets compute min/max of data...

   // |---|   |---|
   // |   |   |   |
   // |---|   |---|
   // |   |   |   |
   // |   |   |   |
   // +---+   +---+
   // | L |   | R |

   // Fast interleaved stereo path O(1) = one loop over samples
   if ( dstChannels == 0 )
   {
      //emit audioMeterData( m_Lmin, m_Lmax, m_Rmin, m_Rmax );
   }
   else if ( dstChannels == 1 )
   {
      float m_Lmin = std::numeric_limits< float >::max();
      float m_Lmax = std::numeric_limits< float >::lowest();

      float const* p1 = dst;
      for ( uint64_t i = 0; i < dstFrames; ++i )
      {
         float sampleL = *p1;
         m_Lmin = std::min( m_Lmin, sampleL );
         m_Lmax = std::max( m_Lmax, sampleL );
         p1 += dstChannels;
      }

      emit audioMeterData( m_Lmin, m_Lmax, 0.f, 0.f );
   }
   else // if ( dstChannels >= 2 )
   {
      float m_Lmin = std::numeric_limits< float >::max();
      float m_Lmax = std::numeric_limits< float >::lowest();
      float m_Rmin = std::numeric_limits< float >::max();
      float m_Rmax = std::numeric_limits< float >::lowest();

      float const* p1 = dst;
      float const* p2 = p1;
      for ( uint64_t i = 0; i < dstFrames; ++i )
      {
         float sampleL = *p2;
         m_Lmin = std::min( m_Lmin, sampleL );
         m_Lmax = std::max( m_Lmax, sampleL );
         p2++;
         float sampleR = *p2;
         m_Rmin = std::min( m_Rmin, sampleR );
         m_Rmax = std::max( m_Rmax, sampleR );
         p1 += dstChannels;
         p2 = p1;
      }

      emit audioMeterData( m_Lmin, m_Lmax, m_Rmin, m_Rmax );
   }
*/
