#include "Shell.hpp"
#include <QResizeEvent>
#include "App.hpp"

// ============================================================================
ShellTitle::ShellTitle( App & app, QWidget* parent )
// ============================================================================
   : QWidget(parent)
   , m_app( app )
   , m_title("Shell64")
{

}

void
ShellTitle::paintEvent( QPaintEvent* event )
{
   QPainter dc(this);
   dc.setRenderHint( QPainter::NonCosmeticDefaultPen );
   //dc.setBrush( Qt::NoBrush );
   //dc.fillRect( rect(), QColor( 10,10,10 ) );

   int w = width();
   int h = height();

   //dc.setCompositionMode( QPainter::CompositionMode_SourceOver );

   drawRectFill( dc, rect(), QColor(255,150,150) );
   drawRectBorder( dc, rect(), QColor(255,0,0) );

   drawText5x8( dc, 2,2,m_title, QColor(255,250,250) );
}

// ============================================================================
ShellAudioMeter::ShellAudioMeter( App & app, QWidget* parent )
// ============================================================================
   : QWidget(parent)
   , m_app( app )
   , m_isHovered( false )
   , m_hasFocus( false )
   , m_updateTimerId( 0 )
   //, m_inputSignal( nullptr )
{
//   setObjectName( "ShellAudioMeter" );
   m_ColorGradient.addStop( 0.0f, 0xFFFFFFFF );
   m_ColorGradient.addStop( 0.1f, 0xFF000000 );
   m_ColorGradient.addStop( 0.5f, 0xFF00FF00 );
   m_ColorGradient.addStop( 0.6f, 0xFF002000 );
   m_ColorGradient.addStop( 0.8f, 0xFF00FFFF );
   m_ColorGradient.addStop( 1.0f, 0xFF0000FF );
   m_ColorGradient.addStop( 1.1f, 0xFFFF00FF );
//       connect( this, SIGNAL(newSamples(float*,uint32_t,uint32_t)),
//                this, SLOT(pushSamples(float*,uint32_t,uint32_t)), Qt::QueuedConnection );
   startUpdateTimer();
}

uint64_t
ShellAudioMeter::pushSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
{
   // DE_DEBUG("pts")

   uint64_t dstSamples = dstFrames * dstChannels;
   if ( dst )
   {
      // Feed LevelMeter
      m_Lmin = std::numeric_limits< float >::max();
      m_Lmax = std::numeric_limits< float >::lowest();
      m_Rmin = std::numeric_limits< float >::max();
      m_Rmax = std::numeric_limits< float >::lowest();

      // Fast interleaved stereo path O(1) = one loop over samples
      if ( dstChannels == 2 )
      {
         float const* pSrc = dst;
         for ( uint64_t i = 0; i < dstFrames; ++i )
         {
            float L = *pSrc++;
            float R = *pSrc++;
            m_Lmin = std::min( m_Lmin, L );
            m_Lmax = std::max( m_Lmax, R );
            m_Rmin = std::min( m_Rmin, L );
            m_Rmax = std::max( m_Rmax, R );
         }
      }
      // Slower path O(N) = one loop for each channel of N channels.
      else
      {
         float const* pSrc = dst;
         for ( uint64_t i = 0; i < dstFrames; ++i )
         {
            float sample = *pSrc;
            m_Lmin = std::min( m_Lmin, sample );
            m_Lmax = std::max( m_Lmax, sample );
            pSrc += dstChannels;
         }

         if ( dstChannels > 1 )
         {
            pSrc = dst + 1;
            for ( uint64_t i = 0; i < dstFrames; ++i )
            {
               float sample = *pSrc;
               m_Rmin = std::min( m_Rmin, sample );
               m_Rmax = std::max( m_Rmax, sample );
               pSrc += dstChannels;
            }
         }
      }

      //update();
   }
   else
   {
      m_Lmin = m_Lmax = m_Rmin = m_Rmax = 0.0f;

   }

   return dstSamples;
}



void
ShellAudioMeter::paintEvent( QPaintEvent* event )
{
   QPainter dc(this);
   dc.setRenderHint( QPainter::NonCosmeticDefaultPen );
   //dc.setBrush( Qt::NoBrush );
   dc.fillRect( rect(), QColor( 10,10,10 ) );

   int w = width();
   int h = height();

   //dc.setCompositionMode( QPainter::CompositionMode_SourceOver );

   //DE_DEBUG("m_Lmax = ", m_Lmax )
   int yL = int( (1.0f - m_Lmax) * h );
   int yR = int( (1.0f - m_Rmax) * h );
   QColor colorL = toQColor( m_ColorGradient.getColor32( m_Lmax ) );
   QColor colorR = toQColor( m_ColorGradient.getColor32( m_Rmax ) );
   QColor colorBg( 25,25,25 );
   //QColor color = toQColor( 0xFF10BF20 );

   dc.setPen( Qt::NoPen );
   dc.setBrush( QBrush( colorBg ) );
   dc.drawRect( QRect(0,0,w/2-1,yL) );

   dc.setBrush( QBrush( colorL ) );
   dc.drawRect( QRect(0,yL,w/2-1,h-1-yL) );


   dc.setPen( Qt::NoPen );
   dc.setBrush( QBrush( colorBg ) );
   dc.drawRect( QRect(w/2,0,w/2-1,yR) );

   dc.setBrush( QBrush( colorR ) );
   dc.drawRect( QRect(w/2,yR,w/2-1,h-1-yR) );

   if ( m_hasFocus )
   {
      drawRectBorder( dc, rect(), QColor(255,155,55) );
   }


   if ( m_isHovered )
   {
      drawRectBorder( dc, rect(), QColor(255,255,255) );
   }
}

/*
void
Shell::drawLevelMeter( QPainter & dc )
{
   int x = m_rcMeter.x();
   int y = m_rcMeter.y();
   int w = m_rcMeter.width();
   int h = m_rcMeter.height();

   dc.fillRect( m_rcMeter, QColor( 10,10,10 ) );

   int yL = int( (1.0f - m_Lmax) * h );
   int yR = int( (1.0f - m_Rmax) * h );
   QColor colorL = toQColor( m_colorGradient.getColor32( m_Lmax ) );
   QColor colorR = toQColor( m_colorGradient.getColor32( m_Rmax ) );
   QColor colorBg( 25,25,25 );
   //QColor color = toQColor( 0xFF10BF20 );

   dc.setPen( Qt::NoPen );
   dc.setBrush( QBrush( colorBg ) );
   dc.drawRect( QRect(x,y,w/2-1,yL) );

   dc.setBrush( QBrush( colorL ) );
   dc.drawRect( QRect(x,y+yL,w/2-1,h-1-yL) );

   dc.setPen( Qt::NoPen );
   dc.setBrush( QBrush( colorBg ) );
   dc.drawRect( QRect(x+w/2,y,w/2-1,yR) );

   dc.setBrush( QBrush( colorR ) );
   dc.drawRect( QRect(x+w/2,y+yR,w/2-1,h-1-yR) );

#if 0
   int w = width();
   int h = height();

   auto const & skin = m_app.m_skin;

   QPainter dc( this );
   dc.setRenderHint( QPainter::NonCosmeticDefaultPen );

   int x = (w - 5)/2+1;
   int y = (h - 40)/2;
   if ( x < 0 ) x = 0;
   if ( y < 0 ) y = 0;

   dc.setPen( Qt::NoPen );
   dc.setBrush( QBrush( skin.symbolColor ) );
   dc.drawRect( QRect(x,y,2,40) );
   dc.drawRect( QRect(x+3,y,2,40) );

   dc.setPen( QPen( skin.contentColor ) );
   dc.drawLine( x,y+6,x+2,y+6 );
   dc.drawLine( x+3,y+6,x+5,y+6 );
#endif
//   m_font5x8.drawText( dc, w/2, 1, QString("m_Lmin(%1), m_Lmax(%2), m_Rmin(%3), m_Rmax(%4)")
//      .arg( m_Lmin).arg( m_Lmax ).arg( m_Rmin).arg( m_Rmax ), 0xFFFFFFFF, de::Align::CenterTop );

}
*/

void ShellAudioMeter::timerEvent( QTimerEvent* event ) { if ( event->timerId() == m_updateTimerId ) { update(); } }
void ShellAudioMeter::focusInEvent( QFocusEvent* event ) { m_hasFocus = true; update(); QWidget::focusInEvent( event ); }
void ShellAudioMeter::focusOutEvent( QFocusEvent* event ) { m_hasFocus = false; update(); QWidget::focusOutEvent( event ); }
void ShellAudioMeter::enterEvent( QEvent* event ) { m_isHovered = true; update(); QWidget::enterEvent( event ); }
void ShellAudioMeter::leaveEvent( QEvent* event ) { m_isHovered = false; update(); QWidget::leaveEvent( event ); }

// ============================================================================
ShellHeader::ShellHeader( App & app, QWidget* parent )
// ============================================================================
   : QWidget(parent)
   , m_app( app )
   , m_isMinimized( false )
{
   m_btnEnabled = createEnableButton();
   m_btnMore = createMoreButton();
   m_btnEditor = createEditorButton();
//   m_btnLoadPreset = createUpdateButton();
//   m_btnSavePreset = createSaveButton();

   m_title = new ShellTitle( m_app, this );

   m_layoutH = new QHBoxLayout( this );
   m_layoutH -> setContentsMargins( 0,0,0,0 );
   m_layoutH -> setSpacing( 3 );
   m_layoutH -> addWidget( m_btnEnabled );
   m_layoutH -> addWidget( m_btnMore );
   m_layoutH -> addWidget( m_title );
   m_layoutH -> addWidget( m_btnEditor );

//   m_layoutV = new QVBoxLayout( this );
//   m_layoutV -> setContentsMargins( 0,0,0,0 );
//   m_layoutV -> setSpacing( 3 );
//   m_layoutV -> addWidget( m_btnEnabled );
//   m_layoutV -> addWidget( m_btnMore );
//   m_layoutV -> addWidget( m_title );
//   m_layoutV -> addWidget( m_btnEditor );

   setLayout( m_layoutH );

   connect( m_btnEnabled, SIGNAL(toggled(bool)), this, SLOT(on_toggled_button_bypass(bool)) );
   connect( m_btnMore, SIGNAL(toggled(bool)), this, SLOT(on_toggled_button_more(bool)) );
   connect( m_btnEditor, SIGNAL(toggled(bool)), this, SLOT(on_toggled_button_editor(bool)) );
   //connect( m_btnEditor, SIGNAL(toggled(bool)), this, SLOT(on_toggled_button_editor(bool)) );
}

void
ShellHeader::mouseDoubleClickEvent( QMouseEvent* event )
{
//   int mx = event->x();
//   int my = event->y();

//   if ( isMouseOverRect( mx, my, m_rcHeader ) )
//   {
//      m_isMinimized = !m_isMinimized;
//      updateLayout();

//      auto trackEditor = reinterpret_cast< Track* >( parentWidget() );
//      if ( trackEditor )
//      {
//         trackEditor->updateLayout();
//      }
//   }

   m_isMinimized = !m_isMinimized;
   emit toggledMinimize( m_isMinimized );
   QWidget::mouseDoubleClickEvent( event );
}

void
ShellHeader::paintEvent( QPaintEvent* event )
{
   QPainter dc(this);
   dc.setRenderHint( QPainter::NonCosmeticDefaultPen );

   LiveSkin const & skin = m_app.m_skin;
   int r = skin.getInt( LiveSkin::Radius );

   // Draw big content rr
   dc.setPen( Qt::NoPen );
   dc.setBrush( QBrush( skin.titleColor ) );
   dc.drawRoundedRect( rect(), r, r );

/*
   if ( m_isMinimized )
   {
   }
   else
   {
      m_font5x8.drawText( dc, 53,5, m_title, skin.symbolColor );
   }
*/
   //h->setDirection( Qt::Horizontal );

   QWidget::paintEvent( event );
}





// ============================================================================
Shell::Shell( App & app, QWidget* parent )
// ============================================================================
   : IPlugin( parent )
   , m_app( app )
   , m_isMinimized( false )
   , m_isHovered( false )
   , m_hasFocus( false )
   , m_updateTimerId( 0 )
   , m_inputSignal( nullptr )
//   , m_btnEnabled( nullptr )
//   , m_btnMore( nullptr )
//   , m_btnEditor( nullptr )
//   , m_btnLoadPreset( nullptr )
//   , m_btnSavePreset( nullptr )
{
   setObjectName( "Shell" );
   setContentsMargins( 0,0,0,0 );
   //setMinimumSize( 158 + 9, 190 );
   //setMaximumSize( 158 + 9, 190 );

   m_header = new ShellHeader( m_app, this );
   m_meter = new ShellAudioMeter( m_app, this );
   m_plugin = nullptr;
   //m_plugin = new PluginVST2( m_app, this );

//   connect( m_editorWindow, SIGNAL(closed()), this, SLOT(on_editorClosed()), Qt::QueuedConnection );

   connect( m_header->m_btnEnabled, SIGNAL(toggled(bool)), this, SLOT(on_btn_active(bool)) );
   connect( m_header->m_btnMore, SIGNAL(toggled(bool)), this, SLOT(on_btn_more(bool)) );
   connect( m_header->m_btnEditor, SIGNAL(toggled(bool)), this, SLOT(on_btn_editor(bool)) );
   //connect( m_header, SIGNAL(toggledMinimize(bool)), this, SLOT(setEditorVisible(bool)) );

   aboutToStart( 64, 2, 48000 );

//   auto v = new QVBoxLayout();
//   v->setContentsMargins(0,0,0,0);
//   v->setSpacing(0);
//   v->addWidget( m_header );
//   v->addWidget( m_plugin );

//   auto h = new QHBoxLayout();
//   h->setContentsMargins(0,0,0,0);
//   h->setSpacing(0);
//   h->addLayout( v );
//   h->addWidget( m_meter );

   updateLayout();
//   startUpdateTimer();
   //setLayout( h );
}

Shell::~Shell()
{
   //stopUpdateTimer();

}

void Shell::on_btn_active( bool checked )
{
   if ( !m_plugin )
   {
      return;
   }

   DE_DEBUG("on_btn_active", checked )
   m_plugin->setBypassed( !checked );
   DE_DEBUG("setBypassed", isBypassed() )

}

void Shell::on_btn_more( bool visible )
{
}

void Shell::on_btn_editor( bool visible )
{
   if ( m_plugin )
   {
      m_plugin->setEditorVisible( visible );
      DE_DEBUG("setEditorVisible", m_plugin->isEditorVisible() )
   }
}

void Shell::on_editorClosed()
{
   DE_ERROR("Editor closed")
   m_header->m_btnEditor->blockSignals( true );
   m_header->m_btnEditor->setChecked( false );
   m_header->m_btnEditor->blockSignals( false );
}

void Shell::setBypassed( bool bypassed )
{
   if ( !m_plugin )
   {
      DE_ERROR("No plugin, bypassed = ", bypassed )
      return;
   }

   DE_DEBUG("setBypassed", bypassed )
   m_plugin->setBypassed( bypassed );

   m_header->m_btnEnabled->blockSignals( true );
   m_header->m_btnEnabled->setChecked( !bypassed );
   m_header->m_btnEnabled->blockSignals( false );

   DE_DEBUG("isBypassed = ", isBypassed() )
   DE_DEBUG("isChecked = ", m_header->m_btnEnabled->isChecked() )

}

void Shell::setMoreVisible( bool visible )
{
   if ( !m_plugin )
   {
      DE_ERROR("No plugin, visible = ", visible )
      return;
   }

   m_header->m_btnMore->blockSignals( true );
   m_header->m_btnMore->setChecked( visible );
   m_header->m_btnMore->blockSignals( false );

}

void Shell::setEditorVisible( bool visible )
{
   if ( !m_plugin )
   {
      DE_ERROR("No plugin, visible = ", visible )
      return;
   }

   m_plugin->setEditorVisible( visible );
   DE_DEBUG("setEditorVisible", m_plugin->isEditorVisible() )

   m_header->m_btnEditor->blockSignals( true );
   m_header->m_btnEditor->setChecked( visible );
   m_header->m_btnEditor->blockSignals( false );
}

void Shell::updateLayout()
{
   if ( m_isMinimized )
   {
      setMinimumSize( QSize( 17 + 9, 190 ) );
      setMaximumSize( QSize( 17 + 9, 190 ) );

      m_rcHeader = QRect(0,0,17,190);  // visible, vertical
      m_rcPlugin = QRect(); // hidden
      m_rcMeter = QRect(17, 60, 9, 66 );  // visible, vertical

      m_header->setDirection( false );
   }
   else
   {
      setMinimumSize( QSize( 158+9, 190 ) );
      setMaximumSize( QSize( 158+9, 190 ) );

      m_rcHeader = QRect( 0,0,158,17 );  // visible, horizontal
      m_rcPlugin = QRect( 0,18,158, 158-18 );  // visible, horizontal
      m_rcMeter = QRect( 158, 60, 9, 66 );  // visible, vertical

      m_header->setDirection( true );
   }

   setWidgetBounds( m_header, m_rcHeader );
   setWidgetBounds( m_meter, m_rcMeter );
   setWidgetBounds( m_plugin, m_rcPlugin );
   update();
}


void
Shell::setPlugin( IPlugin* plugin )
{
   if ( m_plugin )
   {
      DE_DEBUG("Deleting current plugin")
      //m_plugin->setInputSignal( 0, nullptr );

      delete m_plugin;

      m_plugin = nullptr;
   }

   m_plugin = plugin;

   if ( m_plugin )
   {
      m_plugin->setParent( this );

      m_plugin->setInputSignal( 0, m_inputSignal );

      m_header->setTitle( QString::fromStdWString( m_plugin->pluginInfo()->m_name ) );

      if ( m_plugin->editorWindow() )
      {
         connect( m_plugin->editorWindow(),
                  SIGNAL(closed()),
                  this,
                  SLOT(on_editorClosed()),
                  Qt::QueuedConnection );
      }

      updateLayout();

      //   m_editorWindow = nullptr;
      //   if( hasEditor() && editorWindow )
      //   {
      //      m_editorWindow = editorWindow;
      //      QObject::connect( m_editorWindow, SIGNAL(closed()), this, SLOT(on_editorClosed()), Qt::QueuedConnection );
      //      dispatcher( effEditOpen, 0, 0, (void*)m_editorWindow->winId() );
      //      ERect* rc = nullptr;
      //      dispatcher( effEditGetRect, 0, 0, &rc );
      //      int x = rc->left;
      //      int y = rc->top;
      //      int w = rc->right - x;
      //      int h = rc->bottom - y;

      //      m_editorWindow->setMinimumSize( w, h );
      //      m_editorWindow->setMaximumSize( w, h );
      //      m_editorWindow->move( x, y );
      ////      m_editorWindow->show();
      ////      m_editorWindow->raise();
      ////      m_editorImage->setImage( m_editorWindow->grab().toImage().scaledToHeight( 128 ), true );
      ////      m_editorImage->setImagePreserveAspectWoH( true );
      ////      m_editorImage->show();
      //      //m_editorPixmap = m_editorWindow->grab().scaledToHeight( 64 );
      ////      m_loadButton->setIcon( QIcon( m_editorPixmap ) );
      ////      m_loadButton->setIconSize( m_editorPixmap.size() );
      ////      m_loadButton->setText("");
      //      //ShowWindow(m_editorWinHandle, SW_SHOW);

      //      hideEditor();
      //   }

//      if ( isSynth() )
//      {
//         emit addedSynth( this );
//      }

      setBypassed( m_plugin->isBypassed() );
   }
   else
   {
      DE_ERROR("No plugin")
      setBypassed( true );
   }
}

void
Shell::resizeEvent( QResizeEvent* event )
{
   QWidget::resizeEvent( event );
   updateLayout();
}

void
Shell::paintEvent( QPaintEvent* event )
{
//   int w = width();
//   int h = height();

//   if ( w > 1 && h > 1 )
//   {
//      QPainter dc(this);
//      dc.setRenderHint( QPainter::NonCosmeticDefaultPen );
//      //dc.setBrush( Qt::NoBrush );
//      //dc.fillRect( rect(), QColor( 10,10,10 ) );

//      int w = width();
//      int h = height();

//      if ( m_hasFocus )
//      {
//         drawRectBorder( dc, rect(), QColor(255,155,55) );
//      }

//      if ( m_isHovered )
//      {
//         drawRectBorder( dc, rect(), QColor(255,255,255) );
//      }

//   }

   QWidget::paintEvent( event );
}


uint64_t
Shell::readSamples( double pts, float* dst, uint32_t dstFrames, uint32_t dstChannels, uint32_t dstRate )
{
   uint64_t dstSamples = dstFrames * dstChannels;

   if ( !m_plugin )
   {
      de::audio::DSP_FILLZERO( dst, dstSamples );
      DE_ERROR("No plugin")
      return dstSamples;
   }


   if ( isBypassed() )
   {
      //DE_WARN("isBypassed, uri = ", dbStr(pluginInfo()->m_name) )
      de::audio::DSP_FILLZERO( dst, dstSamples );

   }
   else
   {
      auto ret = m_plugin->readSamples( pts, dst, dstFrames, dstChannels, dstRate );
      if ( ret != dstSamples )
      {
         DE_ERROR("retSamples != dstSamples, need zero filling")
      }
   }

   if ( m_meter )
   {
      m_meter->pushSamples( pts, dst, dstFrames, dstChannels, dstRate );
   }

   return dstSamples;
}


void
Shell::focusInEvent( QFocusEvent* event )
{
   m_hasFocus = true;
   update();
   QWidget::focusInEvent( event );
}

void
Shell::focusOutEvent( QFocusEvent* event )
{
   m_hasFocus = true;
   update();
   QWidget::focusOutEvent( event );
}

void
Shell::enterEvent( QEvent* event )
{
   QWidget::enterEvent( event );
}

void
Shell::leaveEvent( QEvent* event )
{
   QWidget::leaveEvent( event );
}

void
Shell::mouseDoubleClickEvent( QMouseEvent* event )
{
   int mx = event->x();
   int my = event->y();

   if ( isMouseOverRect( mx, my, m_rcHeader ) )
   {
      m_isMinimized = !m_isMinimized;
      updateLayout();

      auto trackEditor = reinterpret_cast< Track* >( parentWidget() );
      if ( trackEditor )
      {
         trackEditor->updateLayout();
      }
   }

   QWidget::mouseDoubleClickEvent( event );
}



/*
void
Track::updateLayout()
{
   int w = width();
   int h = height();
   int x = 0;
   int y = 0;

   int mmw = 4;   // Midi Meter Width
   int amw = 9;   // Audio Meter Width
   int editorW = 158;

   // Layout Midi Meter
   setWidgetBounds( m_midiMeter, QRect(x,y,mmw,h) ); x += mmw + 2;

   // Layout Audio Plugin
   if ( m_audioSynth )
   {
      //editorW = m_audioSynth->maximumWidth();
      setWidgetBounds( m_audioSynth, QRect(x,y,editorW,h) ); x += editorW;
   }
   // Layout Audio Plugin Meter
   if ( m_audioSynthMeter )
   {
      setWidgetBounds( m_audioSynthMeter, QRect(x,y,amw,h) ); x += amw;
   }

   for ( size_t i = 0; i < m_audioEffects.size(); ++i )
   {
      auto fx = m_audioEffects[ i ];
      if ( fx )
      {
         // Layout Audio Plugin
         //editorW = fx->maximumWidth();
         setWidgetBounds( fx, QRect(x,y,editorW,h) ); x += editorW;

         // Layout Audio Plugin Meter
         if ( i < m_audioEffectsMeter.size() )
         {
            auto fx_meter = m_audioEffectsMeter[ i ];
            if ( fx_meter )
            {
               setWidgetBounds( fx_meter, QRect(x,y,amw,h) ); x += amw;
            }
         }
      }
   }

   // Layout Drop dummy
   int dummyW = w - x;
   if ( dummyW < 50 ) dummyW = 50;
   setWidgetBounds( m_dropTarget, QRect(x,y,dummyW,h) );

   update();
}
*/

//void
//Shell::resizeEditor( QRect const & pos )
//{
//   if ( m_editorWindow )
//   {
//      m_editorWindow->setMinimumSize( pos.width(), pos.height() );
//      m_editorWindow->setMaximumSize( pos.width(), pos.height() );
//      m_editorWindow->move( pos.x(), pos.y() );
//   }
//}


/*

void
Shell::mouseReleaseEvent( QMouseEvent* event )
{
   //   int mx = event->x();
   //   int my = event->y();

   if ( m_dragMode > 0 )
   {
      if ( m_dragMode == 1 )
      {
      }

      updateLayout();
      m_dragMode = 0;
   }

   QWidget::mouseReleaseEvent( event );
}

void
Shell::mousePressEvent( QMouseEvent* event )
{
   int mx = event->x();
   int my = event->y();

   if ( m_dragMode > 0 )
   {

   }
   else
   {
      if ( m_app.m_isDetailVisible && m_app.m_isClipEditorVisible )
      {
         if ( m_isOverSplitV )
         {
            m_dragMode = 1;
            m_dragData = m_app.m_vsplitterPos; // Store original pos.
            //m_app.m_dragData = m_app.m_rcV.y(); // Store original pos.
            m_dragStartX = mx;
            m_dragStartY = my;
            updateLayout();
         }
      }
   }

   QWidget::mousePressEvent( event );
}

void
Shell::mouseMoveEvent( QMouseEvent* event )
{
   int mx = event->x();
   int my = event->y();

   // === Find hover splitter ===
   m_isOverSplitV = isMouseOverRect( mx, my, m_rcV );
   //m_app.m_isOverSplitH = isMouseOverRect( mx, my, m_app.m_rc1Splitter );

   if ( m_app.m_isDetailVisible && m_app.m_isClipEditorVisible )
   {
      // === Update layout when splitting v ===
      if ( m_dragMode == 1 ) // vsplit
      {
         m_app.m_vsplitterPos = m_dragData - (my - m_dragStartY);
         int y1 = m_rcHeader.y() + m_rcHeader.height();
         int y2 = m_rcFooter.y();
         if ( m_app.m_vsplitterPos < y1 )
         {
            m_app.m_vsplitterPos = y1;
         }
         else if ( m_app.m_vsplitterPos > y2 )
         {
            m_app.m_vsplitterPos = y2;
         }

         updateLayout();
      }
   }


   QWidget::mouseMoveEvent( event );
}

void
Shell::wheelEvent( QWheelEvent* event )
{
   //   int mx = event->x();
   //   int my = event->y();
   //   me.m_wheelY = event->angleDelta().y();
   //   if ( me.m_wheelX != 0.0f )
   //   {
   //      me.m_flags |= de::MouseEvent::WheelX;
   //   }
   //   if ( me.m_wheelY != 0.0f )
   //   {
   //      me.m_flags |= de::MouseEvent::WheelY;
   //   }

   QWidget::wheelEvent( event );
}

void
Shell::keyPressEvent( QKeyEvent* event )
{
   //DE_DEBUG("KeyPress(",event->key(),")")
   QWidget::keyPressEvent( event );
}

void
Shell::keyReleaseEvent( QKeyEvent* event )
{
   //DE_DEBUG("KeyRelease(",event->key(),")")
   QWidget::keyReleaseEvent( event );
}

void
Shell::stopUpdateTimer()
{
   if ( m_updateTimerId )
   {
      killTimer( m_updateTimerId );
      m_updateTimerId = 0;
   }
}

void
Shell::startUpdateTimer()
{
   stopUpdateTimer();
   m_updateTimerId = startTimer( 37, Qt::CoarseTimer );
}

QSize
Shell::sizeHint() const
{
   if ( m_isMinimized )
   {
      return QSize( 23, 190 );
   }
   else
   {
      return QSize( 159 + 8, 190 );
   }
}

void
Shell::stopUpdateTimer()
{
   if ( m_updateTimerId )
   {
      killTimer( m_updateTimerId );
      m_updateTimerId = 0;
   }
}

void
Shell::startUpdateTimer( int ms )
{
   stopUpdateTimer();
   if ( ms < 1 ) { ms = 1; }
   m_updateTimerId = startTimer( ms, Qt::CoarseTimer );
}
void
Shell::timerEvent( QTimerEvent* event )
{
   if ( event->timerId() == m_updateTimerId )
   {
      update();
   }
}
*/



ImageButton*
ShellHeader::createEnableButton()
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
ShellHeader::createMoreButton()
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
ShellHeader::createEditorButton()
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
ShellHeader::createUpdateButton()
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
ShellHeader::createSaveButton()
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
