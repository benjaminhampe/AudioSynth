#include "Body.hpp"
#include <QResizeEvent>
#include "App.hpp"
#include "Draw.hpp"

Body::Body( App & app, QWidget* parent )
   : QWidget( parent )
   , m_app( app )
{
   setMinimumSize(383,77);
   setMouseTracking( true );

   m_hoverButton = -1;
   m_isDragging = false;
   m_dragMode = 0; // 0 = Idle, 1 = splitterV, 2 = splitterH
   m_dragStartX = 0;
   m_dragStartY = 0;

   //for ( auto & synth : m_synths ) { synth = nullptr; }
   //for ( auto & key : m_touched ) { key = nullptr; }
   //setKeyRange( 24, 100 );

   m_app.m_detailStack = new QStackedWidget( this );

   // Arrangement:
   m_app.m_timeLineOverview = new TimeLineOverview( m_app, this );
   m_app.m_arrangement = new Arrangement( m_app, this );

   m_app.m_btnShowArrangement = createShowArrangementButton();
   m_app.m_btnShowSession = createShowSessionButton();
   m_app.m_arrangeScrollBar = new ScrollBar( m_app, this );
   m_app.m_btnComposeIO = createComposeIOButton();
   m_app.m_btnComposeR = createComposeRButton();
   m_app.m_btnComposeM = createComposeMButton();
   m_app.m_btnComposeD = createComposeDButton();

//   auto v = createVBox();
//   v->setContentsMargins( 0,0,0,0);
//   v->setSpacing( 5 );
//   v->addWidget( m_btnShowArrangement );
//   v->addWidget( m_btnShowSessionButton );
//   v->addWidget( m_arrangeScrollBar,1 );
//   v->addWidget( m_btnComposeIO );
//   v->addWidget( m_btnComposeR );
//   v->addWidget( m_btnComposeM );
//   v->addWidget( m_btnComposeD );
//   setLayout( v );

   //m_app.m_pluginExplorer = new PluginExplorer2( m_app, this );
   //m_deviceBar = new DeviceBar( m_app, this );

   // :: ExplorerPanel ::
   m_app.m_btnShowExplorer = createShowExplorerButton();
   m_app.m_btnShowInternDevices = createShowInternDevicesButton();
   m_app.m_btnShowExternDevices = createShowExternDevicesButton();
   m_app.m_btnShowExplorer1 = createShowExplorer1Button();
   m_app.m_btnShowExplorer2 = createShowExplorer2Button();
   m_app.m_btnShowExplorer3 = createShowExplorer3Button();
   m_app.m_explorerScrollBar = new ScrollBar( m_app, this );
   m_app.m_btnShowGrooves = createShowGroovesButton();
//   auto v = createVBox();
//   v->setContentsMargins( 0,0,0,0);
//   v->setSpacing( 5 );
//   v->addWidget( m_btnShowExplorer );
//   v->addWidget( m_btnShowInternDevices );
//   v->addWidget( m_btnShowExternDevices );
//   v->addWidget( m_btnShowExplorer1 );
//   v->addWidget( m_btnShowExplorer2 );
//   v->addWidget( m_btnShowExplorer3 );
//   v->addWidget( m_scrollBar,1 );
//   v->addWidget( m_btnShowGrooves );
//   setLayout( v );

   //m_explorerPanel = new ExplorerPanel( m_app, this );
   m_app.m_pluginExplorer = new PluginExplorer( m_app, this );

//   auto h = createHBox();
//   h->setContentsMargins( 0,0,0,0);
//   h->setSpacing( 0 );
//   h->addWidget( m_deviceBar );
//   h->addWidget(m_explorerPanel,1 );
//   h->addWidget(m_pluginExplorer,1 );
//   setLayout( h );

   connect( m_app.m_btnShowExplorer, SIGNAL(toggled(bool)),
            this, SLOT(on_btnShowExplorerPanel(bool)) );

}

Body::~Body()
{}

void
Body::on_btnShowExplorerPanel( bool checked )
{
   //DE_DEBUG("Toggled")
   m_app.m_isExplorerVisible = checked;
   updateLayout();
}

void
Body::updateLayout()
{
   int w = width();
   int h = height();

   // std::cout << "Body.updateLayout(" << w << "," << h << ")" << std::endl;

   int p = m_app.m_skin.padding;

   // Min elements
   int headerH = 17 + 2*p;
   int vsplitH = p;
   int footerH = 12 + 16 + 2*p;

   // int minH =
//   int deviceBarWidth = 28;
//   int explorerWidth = m_hsplitterPos;
//   int arrangeWidth = 0;
//   int composeBarWidth = 0;

   m_rc1 = QRect();
   m_rcSplitterV = QRect();
   m_rc2 = QRect();

   // Main vertical layout compute variable heights
   int row1H = 0;
   int row2H = 0;
   if ( m_app.m_isDetailVisible )
   {
      #if 0
      row2H = m_app.m_vsplitterPos; // ClipEditor has variable height determined by splitter v.
      #else
      if ( m_app.m_isClipEdVisible )
      {
         row2H = m_app.m_vsplitterPos; // ClipEditor has variable height determined by splitter v.
      }
      else
      {
         row2H = m_app.m_spurEditorHeight; // Fx-Audio DSP chain has fixed height ( yet )
      }
      #endif
   }
   else
   {
      vsplitH = 0;
   }

   int availH = h - (headerH + vsplitH + footerH);
   if (availH > 0)
   {
      if ( row2H > availH )
      {
         row2H = availH;
      }
      else
      {
         row1H = availH - row2H;
      }
   }

   // Main vertical layout ( header+rc1+vsplit+rc2+footer )
   int y = 0;
   m_rcHeader = QRect( 0, y, w, headerH );   y += headerH;
   m_rc1 = QRect( 0, y, w, row1H );          y += row1H;
   m_rcSplitterV = QRect( 0, y, w, vsplitH );        y += vsplitH;
   m_rc2 = QRect( 0, y, w, row2H );          y += row2H;
   m_rcFooter = QRect( 0, y, w, footerH );   y += footerH;

   if ( h != y )
   {
      std::cout << "ERROR :: h("<<h<<") != y("<<y<<")" << std::endl;
   }

   // HeaderPanel
   if ( m_app.m_headerPanel )
   {
      setWidgetBounds( m_app.m_headerPanel, m_rcHeader );
      //m_headerPanel->updateLayout();
   }

   // FooterPanel
   if ( m_app.m_footerPanel )
   {
      setWidgetBounds( m_app.m_footerPanel, m_rcFooter );
   }

   updateTopPanelLayout();
   updateExplorerPanelLayout();
   updateArrangementLayout();
   updateDetailPanelLayout();
   update();
}


void
Body::updateTopPanelLayout()
{
   int p = m_app.m_skin.getInt( LiveSkin::Padding );
   int btnW = m_app.m_skin.getInt( LiveSkin::CircleButtonSize );
   int barW = btnW + 5;
   int explorerW = 0;
   int arrangeW = 0;

   int w = m_rc1.width();
   int h = m_rc1.height();
   // Row1 Content : DeviceBar + PluginExplorer + GMixer + ComposeBar
   //m_deviceBarWidth = 23; // fix width
   //m_explorerWidthMin = 20; // fix width
   //m_arrangementWidth = 0; // computed
   //m_composeBarWidth = 23; // fix width
   int trW = w - 2*p; // - 2*barW;
   int trH = h;

   int x = m_rc1.x() + p;
   int y = m_rc1.y();

   // H-Splitter
   if ( m_app.m_isExplorerVisible )
   {
      if ( trW < 2*barW + p )
      {

      }
      else
      {
         if ( trW < 2*barW + p + m_app.m_hsplitterPos )
         {
            explorerW = trW - 2*barW - p;
         }
         else
         {
            explorerW = m_app.m_hsplitterPos;
            arrangeW = trW - 2*barW - p - explorerW;
         }
      }

      if ( arrangeW < 100 )
      {
         explorerW -= 100 - arrangeW;
         arrangeW = 100;
         //m_hsplitterPos = explorerW;
      }

      m_rcTopLeft = QRect( x, y, explorerW + barW, trH );
      //m_app.m_rc1DeviceBar = QRect( x, y, btnW, trH );
      //m_app.m_rc1ExplorerPanel = QRect( x + barW, y, explorerW, trH );
      //m_app.m_rc1ExplorerContent = QRect( x + barW + 6, y + 6, explorerW - 12, trH - 12 );
      x += explorerW + barW;

      m_rcSplitterH = QRect( x, y, p, trH );
      x += p;

      m_rcTopRight = QRect( x, y, arrangeW + barW, trH );
      //m_app.m_rc1ArrangementPanel = QRect( x, y, arrangeW, trH );
      //m_app.m_rc1ArrangementContent = QRect( x + 6, y + 6, arrangeW - 12, trH - 12 );
      //m_app.m_rc1ComposeBar = QRect( x + arrangeW + 5, y, btnW, trH );
   }
   else
   {
      arrangeW = trW - barW;
      m_rcTopLeft = QRect( x, y, barW, trH ); x += barW;
      m_rcSplitterH = QRect();
      m_rcTopRight = QRect( x, y, arrangeW, trH ); x += arrangeW;
      //m_app.m_rc1ArrangementPanel = QRect( x, y, arrangeW, trH );
      //m_app.m_rc1ArrangementContent = QRect( x + 6, y + 6, arrangeW - 12, trH - 12 );
      //m_app.m_rc1ComposeBar = QRect( x + arrangeW + 5, y, btnW, trH );
   }
}

void
Body::updateExplorerPanelLayout()
{
   int w = m_rcTopLeft.width();
   int h = m_rcTopLeft.height();
   int x = m_rcTopLeft.x();
   int y = m_rcTopLeft.y();

   int b = m_app.m_skin.getInt( LiveSkin::CircleButtonSize );
   int s = m_app.m_skin.getInt( LiveSkin::Spacing );
   int bs = b + s;
   //int p = m_app.m_skin.getInt( LiveSkin::Padding );

   //m_app.m_rcTopLeft = QRect( x, y, explorerW + barW, trH );
   m_rcDeviceBar = QRect( x, y, b, h );

   m_rcExplorerPanel = QRect( x + bs, y, w - bs, h );

   m_rcExplorerContent = QRect( m_rcExplorerPanel.x() + 6,
                                m_rcExplorerPanel.y() + 6,
                                m_rcExplorerPanel.width() - 12,
                                m_rcExplorerPanel.height() - 12 );

   x = m_rcDeviceBar.x();
   y = m_rcDeviceBar.y();
   //w = m_rcDeviceBar.x();
   //h = m_rcDeviceBar.y();
   setWidgetBounds( m_app.m_btnShowExplorer, QRect( x, y, b, b ) ); y += bs;
   setWidgetBounds( m_app.m_btnShowInternDevices, QRect( x, y, b, b ) ); y += bs;
   setWidgetBounds( m_app.m_btnShowExternDevices, QRect( x, y, b, b ) ); y += bs;
   setWidgetBounds( m_app.m_btnShowExplorer1, QRect( x, y, b, b ) ); y += bs;
   setWidgetBounds( m_app.m_btnShowExplorer2, QRect( x, y, b, b ) ); y += bs;
   setWidgetBounds( m_app.m_btnShowExplorer3, QRect( x, y, b, b ) ); y += bs;

   int sy = h - 7*bs;
   if ( sy < 50 ) sy = 50;
   m_rcExplorerScrollBarPanel = QRect( x, y, b, sy );
   m_rcExplorerScrollBar = QRect( m_rcExplorerScrollBarPanel.x() + 7,
                                m_rcExplorerScrollBarPanel.y() + 7,
                                9,
                                m_rcExplorerScrollBarPanel.height() - 2*7 );

   setWidgetBounds( m_app.m_explorerScrollBar, m_rcExplorerScrollBar ); y += sy + s;
   setWidgetBounds( m_app.m_btnShowGrooves, QRect( x, y, b, b ) ); y += bs;

   setWidgetBounds( m_app.m_pluginExplorer, m_rcExplorerContent );
}

void
Body::updateArrangementLayout()
{
   int p = m_app.m_skin.getInt( LiveSkin::Padding );
   int b = m_app.m_skin.getInt( LiveSkin::CircleButtonSize );
   int s = m_app.m_skin.getInt( LiveSkin::Spacing );
   int bs = b + s;
   int c = m_app.m_skin.getInt( LiveSkin::SmallCircleButtonSize );

   int w = m_rcTopRight.width();
   int h = m_rcTopRight.height();
   int x = m_rcTopRight.x();
   int y = m_rcTopRight.y();

   m_rcArrangePanel = QRect( x, y, w - bs, h );
   m_rcArrangePanelContent = QRect( x+6, y+6, w - bs - 12, h - 12 );
   m_rcComposeBar = QRect( x+w-1-b, y, b, h );

   int m_timeLineH = m_app.m_timeLineOverview->computeBestHeight();

   m_rcArrangeOverview = QRect( m_rcArrangePanelContent.x(),
                         m_rcArrangePanelContent.y(),
                         m_rcArrangePanelContent.width(),
                         m_timeLineH );

   setWidgetBounds( m_app.m_timeLineOverview, m_rcArrangeOverview );

   m_rcArrangeContent = QRect( m_rcArrangePanelContent.x(),
                            m_rcArrangePanelContent.y() + m_timeLineH + p,
                            m_rcArrangePanelContent.width(),
                            m_rcArrangePanelContent.height() - m_timeLineH - p );

   setWidgetBounds( m_app.m_arrangement, m_rcArrangeContent );

   x = m_rcComposeBar.x();
   y = m_rcComposeBar.y();
   //w = m_rcDeviceBar.x();
   //h = m_rcDeviceBar.y();
   setWidgetBounds( m_app.m_btnShowArrangement, QRect( x, y, b, b ) ); y += bs;
   setWidgetBounds( m_app.m_btnShowSession, QRect( x, y, b, b ) ); y += bs;

   int sy = h - 2*bs - s - 4*(c+s)-s;
   if ( sy < 50 ) sy = 50;

   x += 7;
   m_rcArrangeScrollBarPanel = QRect( x, y, bs, sy );

   m_rcArrangeScrollBar = QRect( m_rcArrangeScrollBarPanel.x(),
                                m_rcArrangeScrollBarPanel.y() + 7,
                                9,
                                m_rcArrangeScrollBarPanel.height() - 2*7 );

   setWidgetBounds( m_app.m_arrangeScrollBar, m_rcArrangeScrollBar ); y += sy + s;

   x -= 1;
   setWidgetBounds( m_app.m_btnComposeIO, QRect( x, y, c, c ) ); y += c+s;
   setWidgetBounds( m_app.m_btnComposeR, QRect( x, y, c, c ) ); y += c+s;
   setWidgetBounds( m_app.m_btnComposeM, QRect( x, y, c, c ) ); y += c+s;
   setWidgetBounds( m_app.m_btnComposeD, QRect( x, y, c, c ) ); y += c+s;

   //if ( w < 4 ) return;
   //if ( h < 4 ) return;
}


void
Body::updateDetailPanelLayout()
{
   int p = m_app.m_skin.padding;

   int w = m_rc2.width();
   int h = m_rc2.height();
   int x = m_rc2.x() + p;
   int y = m_rc2.y();

   // Row1 Content : DeviceBar + PluginExplorer + GMixer + ComposeBar
   //m_deviceBarWidth = 23; // fix width
   //m_explorerWidthMin = 20; // fix width
   //m_arrangementWidth = 0; // computed
   //m_composeBarWidth = 23; // fix width

   // Bottom Rc2 sub rects
   m_rcQuickHelpPanel = QRect();
   m_rcQuickHelpContent = QRect();
   m_rcDetailPanel = QRect();
   m_rcDetailContent = QRect();

   //DE_DEBUG( "m_app.m_isQuickHelpVisible = ", m_app.m_isQuickHelpVisible)

   if ( m_app.m_isDetailVisible )
   {
      int drW = w - 2*p;
      int drH = h;
      int helpW = 0;

      if ( m_app.m_isQuickHelpVisible )
      {
         helpW = m_app.m_quickHelpWidth;

         m_rcQuickHelpPanel = QRect( x, y, helpW, drH );

         x += helpW + p;

         int clipW = drW - helpW - p;
         m_rcDetailPanel = QRect( x, y, clipW, drH );
      }
      else
      {
         int clipW = drW;
         m_rcDetailPanel = QRect( x, y, clipW, drH );
      }
   }

   m_rcQuickHelpContent = QRect( m_rcQuickHelpPanel.x() + 6,
                                 m_rcQuickHelpPanel.y() + 6,
                                 m_rcQuickHelpPanel.width() - 12,
                                 m_rcQuickHelpPanel.height() - 12 );

   m_rcDetailContent = QRect( m_rcDetailPanel.x() + 6,
                                 m_rcDetailPanel.y() + 6,
                                 m_rcDetailPanel.width() - 12,
                                 m_rcDetailPanel.height() - 12 );

   // Toggle <=> {ClipEditor|SpurEditor}
   if ( m_app.m_isDetailVisible )
   {
      if ( m_app.m_isClipEdVisible )
      {
         setWidgetBounds( m_app.m_clipContent, m_rcDetailContent );  // Show ClipEditor
         setWidgetBounds( m_app.m_detailStack, QRect() );  // Hide TrackEditor
      }
      else // if ( !m_app.m_isClipEdVisible )
      {
         setWidgetBounds( m_app.m_clipContent, QRect() );  // Hide ClipEditor
         setWidgetBounds( m_app.m_detailStack, m_rcDetailContent );  // Show TrackEditor
      }

   }
   else
   {
      setWidgetBounds( m_app.m_clipContent, QRect() );  // Hide ClipEditor
      setWidgetBounds( m_app.m_detailStack, QRect() );  // Hide TrackEditor
   }
}

/*
int32_t
Body::findKey( int midiNote ) const
{
   auto it = std::find_if( m_keys.begin(), m_keys.end(), [&] (Key const & key) { return key.midiNote == midiNote; });
   if ( it != m_keys.end() )
   {
      return int32_t( std::distance( m_keys.begin(), it ) );
   }
   else
   {
      return -1;
   }
}

void
Body::setKeyRange( int midiNoteStart, int midiNoteCount )
{
   m_keyStart = midiNoteStart;
   m_keys.clear();

   for ( int i = 0; i < midiNoteCount; i++ )
   {
      int midiNote = m_keyStart + i;
      int oktave = midiNote / 12;
      int semitone = midiNote - 12 * oktave;
      m_keys.emplace_back( midiNote, oktave, semitone );
   }
}
*/

void
Body::resizeEvent( QResizeEvent* event )
{
   int w = event->size().width();
   int h = event->size().height();
   topLevelWidget()->setWindowTitle( m_app.appName
                        + QString(" | WindowSize(%1,%2)").arg(w).arg(h) );
   updateLayout();

   QWidget::resizeEvent( event );
}

void
Body::paintEvent( QPaintEvent* event )
{
   int w = width();
   int h = height();
   if ( w > 1 && h > 1 )
   {
      LiveSkin const & skin = m_app.m_skin;

      int r = skin.radius;
      int s = skin.getInt( LiveSkin::Spacing );
      int b = skin.getInt( LiveSkin::CircleButtonSize );
      int p = skin.getInt( LiveSkin::Padding );

      QPainter dc( this );
      dc.setRenderHint( QPainter::NonCosmeticDefaultPen );
      //dc.fillRect( rect(), skin.windowColor );

      QColor c = skin.panelColor; //  hasFocus() ? skin.focusColor :
      QColor f = skin.focusColor; //  hasFocus() ? skin.focusColor :


      // ||| ExplorerPanel |||
      if ( m_app.m_focusPanel == eLivePanelExplorer )
      {
         drawRoundRectFill( dc, m_rcExplorerPanel, skin.focusColor, r, r );
      }
      else
      {
         drawRoundRectFill( dc, m_rcExplorerPanel, skin.panelColor, r, r );
      }

      // ||| ArrangePanel |||
      if ( m_app.m_focusPanel == eLivePanelArrange )
      {
         drawRoundRectFill( dc, m_rcArrangePanel, skin.focusColor, r, r );
      }
      else
      {
         drawRoundRectFill( dc, m_rcArrangePanel, skin.panelColor, r, r );
      }

      // ||| QuickHelpPanel |||
      if ( m_app.m_focusPanel == eLivePanelQuickHelp
            && m_app.m_isQuickHelpVisible )
      {
         drawRoundRectFill( dc, m_rcQuickHelpPanel, skin.focusColor, r, r );
      }
      else
      {
         drawRoundRectFill( dc, m_rcQuickHelpPanel, skin.panelColor, r, r );
      }

      // ||| DetailPanel |||
      if ( m_app.m_focusPanel == eLivePanelDetails )
      {
         drawRoundRectFill( dc, m_rcDetailPanel, skin.focusColor, r, r );
      }
      else
      {
         drawRoundRectFill( dc, m_rcDetailPanel, skin.panelColor, r, r );
      }

      // ||| DeviceBar :: ScrollBar
      int x = m_rcExplorerScrollBarPanel.x();
      int y = m_rcExplorerScrollBarPanel.y();
      w = m_rcExplorerScrollBarPanel.width() + 3*s;
      h = m_rcExplorerScrollBarPanel.height();
      drawRoundRectFill( dc, QRect(x,y,w,h), skin.panelColor, r, r );

      // ||| ComposeBar |||
      int bs = b + s;
      x = m_rcComposeBar.x() - 2* p;
      y = m_rcComposeBar.y() + 2 * bs;
      int m = m_rcComposeBar.width() + 2*p;
      int n = h - y;
      drawRoundRectFill( dc, QRect(x,y,m,n), skin.panelColor, r, r );

      // ||| SplitterH |||
      drawRectBorder( dc, m_rcSplitterH, QColor(255,129,30) );
      if ( m_app.m_isOverSplitH )
      {
         drawRectBorder( dc, m_rcSplitterH, 0xFFFFFFFF );
      }

      // ||| SplitterV |||
      drawRectBorder( dc, m_rcSplitterV, QColor(255,129,30) );
      if ( m_app.m_isOverSplitV )
      {
         drawRectBorder( dc, m_rcSplitterV, 0xFFFFFFFF );
      }

   }

   QWidget::paintEvent( event );

}

void
Body::enterEvent( QEvent* event )
{
   QWidget::enterEvent( event );
}

void
Body::leaveEvent( QEvent* event )
{
//   if ( m_app.m_isOverSplitV )
//   {
//      m_isOverSplitV = false;
//      updateLayout();
//   }
   QWidget::leaveEvent( event );
}

void
Body::mouseMoveEvent( QMouseEvent* event )
{
   int mx = event->x();
   int my = event->y();

   // === Find hover splitter ===
   m_app.m_isOverSplitV = isMouseOverRect( mx, my, m_rcSplitterV );
   m_app.m_isOverSplitH = isMouseOverRect( mx, my, m_rcSplitterH );
   //m_app.m_isOverSplitH = isMouseOverRect( mx, my, m_app.m_rc1Splitter );

   if ( isMouseOverRect( mx,my, m_rcExplorerPanel ) )
   {
      m_app.m_hoverPanel = eLivePanelExplorer;
   }
   else if ( isMouseOverRect( mx,my, m_rcArrangePanel ) )
   {
      m_app.m_hoverPanel = eLivePanelArrange;
   }
   else if ( isMouseOverRect( mx,my, m_rcQuickHelpPanel ) )
   {
      m_app.m_hoverPanel = eLivePanelQuickHelp;
   }
   else if ( isMouseOverRect( mx,my, m_rcDetailPanel ) )
   {
      m_app.m_hoverPanel = eLivePanelDetails;
   }
   else
   {
      m_app.m_hoverPanel = eLivePanelCount;
   }





   if ( m_app.m_isOverSplitV )
   {
      if ( cursor().shape() != Qt::SizeVerCursor )
      {
         setCursor( QCursor( Qt::SizeVerCursor ) );
      }
   }
   else if ( m_app.m_isOverSplitH )
   {
      if ( cursor().shape() != Qt::SizeHorCursor )
      {
         setCursor( QCursor( Qt::SizeHorCursor ) );
      }
   }
   else
   {
      if ( cursor().shape() != Qt::ArrowCursor )
      {
         setCursor( QCursor( Qt::ArrowCursor ) );
      }
   }

   // === Update layout when splitting h ===
   if ( m_dragMode == 2 ) // hsplit
   {
      m_app.m_hsplitterPos = m_dragData + (mx - m_dragStartX);
      int k = m_app.m_skin.padding + m_app.m_skin.circleButtonSize + 5;
      int x1 = k;
      int x2 = width() - 1 - k;
      if ( m_app.m_hsplitterPos < x1 )
      {
         m_app.m_hsplitterPos = x1;
      }
      else if ( m_app.m_hsplitterPos >= x2 )
      {
         m_app.m_hsplitterPos = x2;
      }

      updateLayout();
   }



   if ( m_app.m_isDetailVisible && m_app.m_isClipEdVisible )
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
Body::mousePressEvent( QMouseEvent* event )
{
   int mx = event->x();
   int my = event->y();

   if ( m_dragMode > 0 )
   {

   }
   else
   {
      if ( m_app.m_isOverSplitH )
      {
         m_dragMode = 2;
         //m_app.m_dragData = m_app.m_hsplitterPos; // Store original pos.
         m_dragData = m_rcSplitterH.x(); // Store original pos.
         m_dragStartX = mx;
         m_dragStartY = my;
         updateLayout();
      }

      if ( m_app.m_isDetailVisible && m_app.m_isClipEdVisible )
      {
         if ( m_app.m_isOverSplitV )
         {
            m_dragMode = 1;
            m_dragData = m_app.m_vsplitterPos; // Store original pos.
            //m_app.m_dragData = m_app.m_rcSplitterV.y(); // Store original pos.
            m_dragStartX = mx;
            m_dragStartY = my;
            updateLayout();
         }
      }

   }

   QWidget::mousePressEvent( event );
}

void
Body::mouseReleaseEvent( QMouseEvent* event )
{
   if ( m_dragMode > 0 )
   {
      if ( m_dragMode == 1 )
      {
      }
      else if ( m_dragMode == 2 )
      {
      }

      updateLayout();
      m_dragMode = 0;
   }

   int mx = event->x();
   int my = event->y();

   if ( isMouseOverRect( mx,my, m_rcExplorerPanel ) )
   {
      m_app.m_focusPanel = eLivePanelExplorer;
      update();
   }
   else if ( isMouseOverRect( mx,my, m_rcArrangePanel ) )
   {
      m_app.m_focusPanel = eLivePanelArrange;
      update();
   }
   else if ( isMouseOverRect( mx,my, m_rcQuickHelpPanel ) )
   {
      m_app.m_focusPanel = eLivePanelQuickHelp;
      update();
   }
   else if ( isMouseOverRect( mx,my, m_rcDetailPanel ) )
   {
      m_app.m_focusPanel = eLivePanelDetails;
      update();
   }
//   else
//   {
//      m_app.m_focusPanel = eLivePanelCount;
//      update();
//   }
   QWidget::mouseReleaseEvent( event );
}


void
Body::wheelEvent( QWheelEvent* event )
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

//   m_app.m_clipEditor.wheelEvent( me );

   QWidget::wheelEvent( event );
}

void
Body::keyPressEvent( QKeyEvent* event )
{
/*
   auto noteOn = [&] ( int midiNote, int velocity = 90 )
   {
      m_app.sendNote( de::audio::Note( 0, midiNote, 0, velocity ) );

      int keyIndex = findKey( midiNote );
      if ( keyIndex > -1 )
      {
         m_keys[ keyIndex ].pressed = true;
      }
   };

   auto key = event->key();

   DE_DEBUG("KeyPress(",key,")")

   int k = 59-12; // 59;
//   if ( key == Qt::Key_1 ) { noteOn( 59 ); }
//   if ( key == Qt::Key_Q ) { noteOn( 60 ); }
//   if ( key == Qt::Key_2 ) { noteOn( 61 ); }
//   if ( key == Qt::Key_W ) { noteOn( 62 ); }
//   if ( key == Qt::Key_3 ) { noteOn( 63 ); }
//   if ( key == Qt::Key_E ) { noteOn( 64 ); }
//   if ( key == Qt::Key_R ) { noteOn( 65 ); }
//   if ( key == Qt::Key_5 ) { noteOn( 66 ); }
//   if ( key == Qt::Key_T ) { noteOn( 67 ); }
//   if ( key == Qt::Key_6 ) { noteOn( 68 ); }
//   if ( key == Qt::Key_Z ) { noteOn( 69 ); }
//   if ( key == Qt::Key_7 ) { noteOn( 70 ); }
//   if ( key == Qt::Key_U ) { noteOn( 71 ); }
//   if ( key == Qt::Key_I ) { noteOn( 72 ); }
//   if ( key == Qt::Key_9 ) { noteOn( 73 ); }
//   if ( key == Qt::Key_O ) { noteOn( 74 ); }
//   if ( key == Qt::Key_0 ) { noteOn( 75 ); }
//   if ( key == Qt::Key_P ) { noteOn( 76 ); }
//   if ( key == Qt::Key_Udiaeresis ) { noteOn( 77 ); } // Ü
//   if ( key == Qt::Key_ssharp ) { noteOn( 78 ); } // ß
//   if ( key == Qt::Key_acute ) { noteOn( 78 ); } // ´
//   if ( key == Qt::Key_Plus ) { noteOn( 79 ); } // +
   if ( key == Qt::Key_1 ) { noteOn( k ); } k++;
   if ( key == Qt::Key_Q ) { noteOn( k ); } k++;
   if ( key == Qt::Key_2 ) { noteOn( k ); } k++;
   if ( key == Qt::Key_W ) { noteOn( k ); } k++;
   if ( key == Qt::Key_3 ) { noteOn( k ); } k++;
   if ( key == Qt::Key_E ) { noteOn( k ); } k++;
   if ( key == Qt::Key_R ) { noteOn( k ); } k++;
   if ( key == Qt::Key_5 ) { noteOn( k ); } k++;
   if ( key == Qt::Key_T ) { noteOn( k ); } k++;
   if ( key == Qt::Key_6 ) { noteOn( k ); } k++;
   if ( key == Qt::Key_Z ) { noteOn( k ); } k++;
   if ( key == Qt::Key_7 ) { noteOn( k ); } k++;
   if ( key == Qt::Key_U ) { noteOn( k ); } k++;
   if ( key == Qt::Key_I ) { noteOn( k ); } k++;
   if ( key == Qt::Key_9 ) { noteOn( k ); } k++;
   if ( key == Qt::Key_O ) { noteOn( k ); } k++;
   if ( key == Qt::Key_0 ) { noteOn( k ); } k++;
   if ( key == Qt::Key_P ) { noteOn( k ); } k++;
   if ( key == Qt::Key_Udiaeresis ) { noteOn( k ); } k++; // Ü
   if ( key == Qt::Key_ssharp ) { noteOn( k ); } k++; // ß
   if ( key == Qt::Key_acute ) { noteOn( k ); } // ´
   if ( key == Qt::Key_Plus ) { noteOn( k ); } k++; // +

   k = 48-12;
   if ( key == Qt::Key_Greater || key == Qt::Key_Less ) { noteOn( k ); } k++;
   if ( key == Qt::Key_A ) { noteOn( k ); } k++;
   if ( key == Qt::Key_Y ) { noteOn( k ); } k++;
   if ( key == Qt::Key_S ) { noteOn( k ); } k++;
   if ( key == Qt::Key_X ) { noteOn( k ); } k++;
   //if ( key == Qt::Key_D ) { noteOn( k ); } k++;
   if ( key == Qt::Key_C ) { noteOn( k ); } k++;
   if ( key == Qt::Key_F ) { noteOn( k ); } k++;
   if ( key == Qt::Key_V ) { noteOn( k ); } k++;
   if ( key == Qt::Key_G ) { noteOn( k ); } k++;
   if ( key == Qt::Key_B ) { noteOn( k ); } k++;
   if ( key == Qt::Key_H ) { noteOn( k ); } k++;
   if ( key == Qt::Key_N ) { noteOn( k ); } k++;
   if ( key == Qt::Key_M ) { noteOn( k ); } k++;
   if ( key == Qt::Key_K ) { noteOn( k ); } k++;
   if ( key == Qt::Key_Comma ) { noteOn( k ); } k++;
   if ( key == Qt::Key_L ) { noteOn( k ); } k++;
   if ( key == Qt::Key_Period ) { noteOn( k ); } k++;
   if ( key == Qt::Key_Odiaeresis ) { noteOn( k ); } k++; // Ö
   if ( key == Qt::Key_Minus ) { noteOn( k ); } k++;
   if ( key == Qt::Key_Adiaeresis ) { noteOn( k ); } k++; // Ä
   if ( key == Qt::Key_NumberSign ) { noteOn( k ); } k++; // #


   // event->accept();
*/
   //DE_DEBUG("KeyPress(",event->key(),")")
   QWidget::keyPressEvent( event );
}

void
Body::keyReleaseEvent( QKeyEvent* event )
{
/*
   auto noteOff = [&] ( int midiNote )
   {
      m_app.sendNote( de::audio::Note( 0, midiNote, 0, 0 ) );
   };

   auto key = event->key();

   DE_DEBUG("keyRelease(",key,")")

   int k = 59; // 59;
//   if ( key == Qt::Key_1 ) { noteOff( k ); } k++;
//   if ( key == Qt::Key_Q ) { noteOff( k ); } k++;
//   if ( key == Qt::Key_2 ) { noteOff( k ); } k++;
//   if ( key == Qt::Key_W ) { noteOff( k ); } k++;
//   if ( key == Qt::Key_3 ) { noteOff( k ); } k++;
//   if ( key == Qt::Key_E ) { noteOff( k ); } k++;
//   if ( key == Qt::Key_R ) { noteOff( k ); } k++;
//   if ( key == Qt::Key_5 ) { noteOff( k ); } k++;
//   if ( key == Qt::Key_T ) { noteOff( k ); } k++;
//   if ( key == Qt::Key_6 ) { noteOff( k ); } k++;
//   if ( key == Qt::Key_Z ) { noteOff( k ); } k++;
//   if ( key == Qt::Key_7 ) { noteOff( k ); } k++;
//   if ( key == Qt::Key_U ) { noteOff( k ); } k++;
//   if ( key == Qt::Key_I ) { noteOff( k ); } k++;
//   if ( key == Qt::Key_9 ) { noteOff( k ); } k++;
//   if ( key == Qt::Key_O ) { noteOff( k ); } k++;
//   if ( key == Qt::Key_0 ) { noteOff( k ); } k++;
//   if ( key == Qt::Key_P ) { noteOff( k ); } k++;
//   if ( key == Qt::Key_Udiaeresis ) { noteOff( k ); } k++; // Ü
//   if ( key == Qt::Key_ssharp ) { noteOff( k ); } k++; // ß
//   if ( key == Qt::Key_acute ) { noteOff( k ); } // ´
//   if ( key == Qt::Key_Plus ) { noteOff( k ); } k++; // +
   if ( key == Qt::Key_1 ) { noteOff( 59 ); }
   if ( key == Qt::Key_Q ) { noteOff( 60 ); }
   if ( key == Qt::Key_2 ) { noteOff( 61 ); }
   if ( key == Qt::Key_W ) { noteOff( 62 ); }
   if ( key == Qt::Key_3 ) { noteOff( 63 ); }
   if ( key == Qt::Key_E ) { noteOff( 64 ); }
   if ( key == Qt::Key_R ) { noteOff( 65 ); }
   if ( key == Qt::Key_5 ) { noteOff( 66 ); }
   if ( key == Qt::Key_T ) { noteOff( 67 ); }
   if ( key == Qt::Key_6 ) { noteOff( 68 ); }
   if ( key == Qt::Key_Z ) { noteOff( 69 ); }
   if ( key == Qt::Key_7 ) { noteOff( 70 ); }
   if ( key == Qt::Key_U ) { noteOff( 71 ); }
   if ( key == Qt::Key_I ) { noteOff( 72 ); }
   if ( key == Qt::Key_9 ) { noteOff( 73 ); }
   if ( key == Qt::Key_O ) { noteOff( 74 ); }
   if ( key == Qt::Key_0 ) { noteOff( 75 ); }
   if ( key == Qt::Key_P ) { noteOff( 76 ); }
   if ( key == Qt::Key_Udiaeresis ) { noteOff( 77 ); } // Ü
   if ( key == Qt::Key_ssharp ) { noteOff( 78 ); } // ß
   if ( key == Qt::Key_acute ) { noteOff( 78 ); } // ´
   if ( key == Qt::Key_Plus ) { noteOff( 79 ); } // +

   k = 48;
   if ( key == Qt::Key_Greater || key == Qt::Key_Less ) { noteOff( k ); } k++;
   if ( key == Qt::Key_A ) { noteOff( k ); } k++;
   if ( key == Qt::Key_Y ) { noteOff( k ); } k++;
   if ( key == Qt::Key_S ) { noteOff( k ); } k++;
   if ( key == Qt::Key_X ) { noteOff( k ); } k++;
   //if ( key == Qt::Key_D ) { noteOff( k ); } k++;
   if ( key == Qt::Key_C ) { noteOff( k ); } k++;
   if ( key == Qt::Key_F ) { noteOff( k ); } k++;
   if ( key == Qt::Key_V ) { noteOff( k ); } k++;
   if ( key == Qt::Key_G ) { noteOff( k ); } k++;
   if ( key == Qt::Key_B ) { noteOff( k ); } k++;
   if ( key == Qt::Key_H ) { noteOff( k ); } k++;
   if ( key == Qt::Key_N ) { noteOff( k ); } k++;
   if ( key == Qt::Key_M ) { noteOff( k ); } k++;
   if ( key == Qt::Key_K ) { noteOff( k ); } k++;
   if ( key == Qt::Key_Comma ) { noteOff( k ); } k++;
   if ( key == Qt::Key_L ) { noteOff( k ); } k++;
   if ( key == Qt::Key_Period ) { noteOff( k ); } k++;
   if ( key == Qt::Key_Odiaeresis ) { noteOff( k ); } k++; // Ö
   if ( key == Qt::Key_Minus ) { noteOff( k ); } k++;
   if ( key == Qt::Key_Adiaeresis ) { noteOff( k ); } k++; // Ä
   if ( key == Qt::Key_NumberSign ) { noteOff( k ); } k++; // #

   // event->accept();
*/
   //DE_DEBUG("KeyRelease(",event->key(),")")
   QWidget::keyReleaseEvent( event );
}







// ExplorerPanel


ImageButton*
Body::createShowExplorerButton()
{
   auto btn = new ImageButton( this );
   btn->setCheckable( true );
   btn->setChecked( true );

   LiveSkin const & skin = m_app.m_skin;
   int bw = skin.circleButtonSize;
   int bh = skin.circleButtonSize;
   auto symColor = skin.symbolColor;
   auto bgColor = skin.windowColor;
   auto fgColor = skin.panelColor; // or panelColor

   //QFont font = getFontAwesome( 14 );
   std::string
   msg = "#########\n"
         "#########\n"
         " #######\n"
         " #######\n"
         "  #####\n"
         "  #####\n"
         "   ###\n"
         "   ###\n"
         "    #\n";
   // [idle]
   QImage ico = createAsciiArt( symColor, fgColor, msg );
   QImage img = createCircleImage( bw,bh, bgColor, fgColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   msg = "##\n"
         "####\n"
         "######\n"
         "########\n"
         "#########\n"
         "########\n"
         "######\n"
         "####\n"
         "##\n";

   ico = createAsciiArt( symColor, fgColor, msg );
   img = createCircleImage( bw,bh, bgColor, fgColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );

   return btn;
}

ImageButton*
Body::createShowInternDevicesButton()
{
   auto btn = new ImageButton( this );
   btn->setCheckable( true );
   btn->setChecked( false );
   btn->setEnabled( false );

   LiveSkin const & skin = m_app.m_skin;
   int bw = skin.circleButtonSize;
   int bh = skin.circleButtonSize;
   auto symColor = skin.symbolColor;
   auto bgColor = skin.windowColor;
   auto fgColor = skin.panelColor;

   //QFont font = getFontAwesome( 14 );
   std::string
   msg = " ########### \n"
         "##         ##\n"
         "##         ##\n"
         "#############\n"
         "#############\n"
         "#############\n"
         "#############\n"
         "#############\n"
         "#############\n"
         " ########### \n";
   // [idle]
   QImage ico = createAsciiArt( symColor, fgColor, msg );
   QImage img = createCircleImage( bw,bh, bgColor, fgColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( skin.activeColor, fgColor, msg );
   img = createCircleImage( bw,bh, bgColor, fgColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

ImageButton*
Body::createShowExternDevicesButton()
{
   auto btn = new ImageButton( this );
   btn->setCheckable( true );
   btn->setChecked( false );
   //btn->setEnabled( false );

   LiveSkin const & skin = m_app.m_skin;
   int bw = skin.circleButtonSize;
   int bh = skin.circleButtonSize;

   auto symColor = skin.symbolColor;
   auto bgColor = skin.windowColor;
   auto fgColor = skin.panelColor;

   //QFont font = getFontAwesome( 14 );
   std::string
   msg = "      ####\n"
         "   #  #######\n"
         "   #######\n"
         "##########\n"
         "   #######\n"
         "   #  #######\n"
         "      ####\n";
   // [idle]
   QImage ico = createAsciiArt( symColor, fgColor, msg );
   QImage img = createCircleImage( bw,bh, bgColor, fgColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( skin.activeColor, fgColor, msg );
   img = createCircleImage( bw,bh, bgColor, fgColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

ImageButton*
Body::createShowExplorer1Button()
{
   auto btn = new ImageButton( this );
   btn->setCheckable( true );
   btn->setChecked( false );
   btn->setEnabled( false );

   LiveSkin const & skin = m_app.m_skin;
   int bw = skin.circleButtonSize;
   int bh = skin.circleButtonSize;
   auto symColor = skin.symbolColor;
   auto bgColor = skin.windowColor;
   auto fgColor = skin.panelColor;

   //QFont font = getFontAwesome( 14 );
   std::string
   msg = "#####\n"
         "#####\n"
         "###########\n"
         "###     ###\n"
         "### ##  ###\n"
         "###  #  ###\n"
         "###  #  ###\n"
         "###     ###\n"
         "###########\n";
   // [idle]
   QImage ico = createAsciiArt( symColor, fgColor, msg );
   QImage img = createCircleImage( bw,bh, bgColor, fgColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( skin.activeColor, fgColor, msg );
   img = createCircleImage( bw,bh, bgColor, fgColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

ImageButton*
Body::createShowExplorer2Button()
{
   auto btn = new ImageButton( this );
   btn->setCheckable( true );
   btn->setChecked( false );
   btn->setEnabled( false );

   LiveSkin const & skin = m_app.m_skin;
   int bw = skin.circleButtonSize;
   int bh = skin.circleButtonSize;
   auto symColor = skin.symbolColor;
   auto bgColor = skin.windowColor;
   auto fgColor = skin.panelColor;

   //QFont font = getFontAwesome( 14 );
   std::string
   msg = "#####\n"
         "#####\n"
         "###########\n"
         "####   ####\n"
         "###  #  ###\n"
         "##  # #  ##\n"
         "##    #  ##\n"
         "##   #   ##\n"
         "### ### ###\n"
         "####   ####\n"
         "###########\n";

   // [idle]
   QImage ico = createAsciiArt( symColor, fgColor, msg );
   QImage img = createCircleImage( bw,bh, bgColor, fgColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( skin.activeColor, fgColor, msg );
   img = createCircleImage( bw,bh, bgColor, fgColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

ImageButton*
Body::createShowExplorer3Button()
{
   auto btn = new ImageButton( this );
   btn->setCheckable( true );
   btn->setChecked( false );
   btn->setEnabled( false );

   LiveSkin const & skin = m_app.m_skin;
   int bw = skin.circleButtonSize;
   int bh = skin.circleButtonSize;
   auto symColor = skin.symbolColor;
   auto bgColor = skin.windowColor;
   auto fgColor = skin.panelColor;

   //QFont font = getFontAwesome( 14 );
   std::string
   msg = "#####\n"
         "#####\n"
         "###########\n"
         "###     ###\n"
         "### ##  ###\n"
         "###  #  ###\n"
         "###  #  ###\n"
         "###     ###\n"
         "###########\n";
   // [idle]
   QImage ico = createAsciiArt( symColor, fgColor, msg );
   QImage img = createCircleImage( bw,bh, bgColor, fgColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]

   ico = createAsciiArt( skin.activeColor, fgColor, msg );
   img = createCircleImage( bw,bh, bgColor, fgColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

ImageButton*
Body::createShowGroovesButton()
{
   auto btn = new ImageButton( this );
   btn->setCheckable( true );
   btn->setChecked( false );
   //btn->setEnabled( false );

   LiveSkin const & skin = m_app.m_skin;
   int bw = skin.circleButtonSize;
   int bh = skin.circleButtonSize;
   auto symColor = skin.symbolColor;
   auto bgColor = skin.windowColor;
   auto fgColor = skin.panelColor;

   //QFont font = getFontAwesome( 14 );
   std::string
   msg = "  ##\n"
         " #  #\n"
         "#    #    #\n"
         "      #  #\n"
         "       ##\n"
         "  ##\n"
         " #  #\n"
         "#    #    #\n"
         "      #  #\n"
         "       ##\n";

   // [idle]
   QImage ico = createAsciiArt( symColor, fgColor, msg );
   QImage img = createCircleImage( bw,bh, bgColor, fgColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( skin.activeColor, fgColor, msg );
   img = createCircleImage( bw,bh, bgColor, fgColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}













// ArrangementPanel


ImageButton*
Body::createShowArrangementButton()
{
   auto btn = new ImageButton( this );
   LiveSkin const & skin = m_app.m_skin;

   int cbs = skin.getInt( LiveSkin::CircleButtonSize );

   btn->setCheckable( true );
   btn->setChecked( true );

   auto symColor = skin.symbolColor;
   auto bgColor = skin.windowColor;
   auto fgColor = skin.focusColor; // or panelColor

   //QFont font = getFontAwesome( 14 );
   std::string
   msg = "###########\n"
         "###########\n"
         "###########\n"
         " \n"
         " \n"
         "###########\n"
         "###########\n"
         "###########\n"
         " \n"
         " \n"
         "###########\n"
         "###########\n"
         "###########\n"
         ;

   // [idle]
   QImage ico = createAsciiArt( symColor, fgColor, msg );
   QImage img = createCircleImage( cbs,cbs, bgColor, fgColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( skin.activeColor, fgColor, msg );
   img = createCircleImage( cbs,cbs, bgColor, fgColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

ImageButton*
Body::createShowSessionButton()
{
   auto btn = new ImageButton( this );
   LiveSkin const & skin = m_app.m_skin;

   int cbs = skin.getInt( LiveSkin::CircleButtonSize );

   btn->setCheckable( true );
   btn->setChecked( false );

   auto symColor = skin.symbolColor;
   auto bgColor = skin.windowColor;
   auto fgColor = skin.focusColor; // or panelColor

   std::string
   msg = "###  ###  ###\n"
         "###  ###  ###\n"
         "###  ###  ###\n"
         "###  ###  ###\n"
         "###  ###  ###\n"
         "###  ###  ###\n"
         "###  ###  ###\n"
         "###  ###  ###\n"
         "###  ###  ###\n"
         ;

   // [idle]
   QImage ico = createAsciiArt( symColor, fgColor, msg );
   QImage img = createCircleImage( cbs,cbs, bgColor, fgColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( skin.activeColor, fgColor, msg );
   img = createCircleImage( cbs,cbs, bgColor, fgColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}


ImageButton*
Body::createComposeIOButton()
{
   auto btn = new ImageButton( this );
   LiveSkin const & skin = m_app.m_skin;

   int bs = skin.getInt( LiveSkin::SmallCircleButtonSize );

   btn->setCheckable( true );
   btn->setChecked( true );

   auto symColor = skin.symbolColor;
   auto bgColor = skin.panelColor;
   auto fgColor = skin.windowColor;

   std::string
   msg = "#  ###\n"
         "# #   #\n"
         "# #   #\n"
         "# #   #\n"
         "#  ###\n";

   // [idle]
   QImage ico = createAsciiArt( skin.titleColor, symColor, msg );
   QImage img = createCircleImage( bs,bs, QColor(0,0,0,0), symColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( symColor, skin.activeColor, msg );
   img = createCircleImage( bs,bs, QColor(0,0,0,0), skin.activeColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

ImageButton*
Body::createComposeRButton()
{
   auto btn = new ImageButton( this );
   LiveSkin const & skin = m_app.m_skin;
   int bs = skin.getInt( LiveSkin::SmallCircleButtonSize );

   btn->setCheckable( true );
   btn->setChecked( true );

   auto symColor = skin.symbolColor;
   auto bgColor = skin.panelColor;
   auto fgColor = skin.windowColor;

   std::string
   msg = "####\n"
         "#   #\n"
         "####\n"
         "# #\n"
         "#  ##\n";

   // [idle]
   QImage ico = createAsciiArt( skin.titleColor, symColor, msg );
   QImage img = createCircleImage( bs,bs, QColor(0,0,0,0), symColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( symColor, skin.activeColor, msg );
   img = createCircleImage( bs,bs, QColor(0,0,0,0), skin.activeColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

ImageButton*
Body::createComposeMButton()
{
   auto btn = new ImageButton( this );
   LiveSkin const & skin = m_app.m_skin;
   int bs = skin.getInt( LiveSkin::SmallCircleButtonSize );

   btn->setCheckable( true );
   btn->setChecked( true );

   auto symColor = skin.symbolColor;
   auto bgColor = skin.panelColor;
   auto fgColor = skin.windowColor;

   std::string
   msg = "## ##\n"
         "# # #\n"
         "# # #\n"
         "#   #\n"
         "#   #\n";

   // [idle]
   QImage ico = createAsciiArt( skin.titleColor, symColor, msg );
   QImage img = createCircleImage( bs,bs, QColor(0,0,0,0), symColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( symColor, skin.activeColor, msg );
   img = createCircleImage( bs,bs, QColor(0,0,0,0), skin.activeColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}

ImageButton*
Body::createComposeDButton()
{
   auto btn = new ImageButton( this );
   LiveSkin const & skin = m_app.m_skin;
   int bs = skin.getInt( LiveSkin::SmallCircleButtonSize );

   btn->setCheckable( true );
   btn->setChecked( false );

   auto symColor = skin.symbolColor;
   auto bgColor = skin.panelColor;
   auto fgColor = skin.windowColor;

   std::string
   msg = "####\n"
         "#   #\n"
         "#   #\n"
         "#   #\n"
         "####\n";

   // [idle]
   QImage ico = createAsciiArt( skin.titleColor, symColor, msg );
   QImage img = createCircleImage( bs,bs, QColor(0,0,0,0), symColor, ico );
   btn->setImage( 0, img );
   // [idle_hover]
   btn->setImage( 1, img );

   // [active]
   ico = createAsciiArt( symColor, skin.activeColor, msg );
   img = createCircleImage( bs,bs, QColor(0,0,0,0), skin.activeColor, ico );
   btn->setImage( 2, img );
   // [active_hover]
   btn->setImage( 3, img );
   return btn;
}
