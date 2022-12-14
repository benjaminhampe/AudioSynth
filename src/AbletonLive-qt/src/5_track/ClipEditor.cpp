#include "ClipEditor.hpp"

using de::midi::MidiTools;

ClipEditor::ClipEditor( App & app, QWidget* parent )
   : QWidget( parent )
   , m_app( app )
   , m_playTimerId( 0 )
   , m_drawTimerId( 0 )
//   , m_freq( m_bpm / 60.0f )
//   , m_period( 1000.0 / m_freq ) // in [ms]
//   , m_periodStep( m_period / 16.0 ) // stepTime
//   , m_step( 0 )
//   , m_step_count( 16 )
{
   setObjectName( "ClipEditor" );
   setMouseTracking( true );
   setContentsMargins(0,0,0,0);
   setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

   reset();
}

ClipEditor::~ClipEditor()
{
   stopPlayTimer();
}

void
ClipEditor::reset()
{
   m_clip = nullptr;
   m_bpm = 60.0f;
//   m_beatBeg = 0;
//   m_beatEnd = 0;
//   m_timeBeg = 0; // 0.5 sec
//   m_timeEnd = int64_t(1000 * 1000 * 1000); // 1 sec
   m_isBeatSync = true;
   m_loops = 0;
   //m_beatCount = m_beatEnd - m_beatBeg;
   m_midiTicksPerBeat = 96; //?
   // m_barCount = 4;

   // 128 piano keys to draw on. Ascending keyIndex ...
   for ( size_t i = 0; i < m_keys.size(); ++i )
   {
      m_keys[ i ].reset( 127 - i ); // ... but descending frequency ( draw order ).
   }

   // X-Axis is time 't' ( complicated ) in [s], [bars], [ticks] and [beats]
   m_beatIndex = 0;
   m_beatCount = 16;  // show 4 bars eats

   m_topHeight = 20; // includes x-axis labels

   m_isPlaying = false;
   m_timeStart = dbNanoseconds();
   m_time = 0;
   m_loopTime = int64_t(500 * 1000 * 1000); // 0.5 sec
   m_loopTimeStart = 0;
   m_loopTimeEnd = int64_t(1000 * 1000 * 1000) * 2; // 2 sec
   m_loopTimeRange = m_loopTimeEnd - m_loopTimeStart;
   m_pixelsPerNanosecond = 250.0 / 1.0e9;
   m_nanosecondsPerPixel = 1.0 / m_pixelsPerNanosecond;
   //m_nanosecondsPerBeat = 1.0 / m_pixelsPerNanosecond;
   // [beat/px] = [beat/sec] *
   //m_beatsPerPixel = 1.0 / m_pixelsPerNanosecond;
   // [px/beat] = [beat/sec] *
   //m_pixelPerBeat = (m_bpm / 60.0);   //?

   // Single piano key size of the left shown PianoBar.
   m_keyWidth = 32;  // the width of the header column 0 ( const so far )
   m_keyHeight = 10; // line-height ( changed by mousedragY over col[0] )

   // Y-Axis is keyIndex low to high, but gives freqs from high to low
   m_keyStart = 0;   // keyIndex start ( changed by mousewheel y )
   m_keyCount = 0;   // visible keys in y-dir ( changed by widget resize )


   //m_loopStart = 0;  // for Looping

   m_mouseX = 0;
   m_mouseY = 0;
   m_detectedKeyIndex = -1;
   m_hoveredNote.reset();
   m_selectedNote.reset();
   m_isOverPianoBar = false;
   m_isOverBeatGrid = false;

   m_dragMode = 0;
   m_dragStartX = 0;
   m_dragStartY = 0;

   // 8 synths we send notes to.
   for ( size_t i = 0; i < m_synths.size(); ++i )
   {
      m_synths[ i ] = nullptr;
   }
}

void
ClipEditor::startPlayTimer()
{
   stopPlayTimer();
   m_playTimerId = startTimer( 1, Qt::CoarseTimer );
   m_drawTimerId = startTimer( 40, Qt::CoarseTimer );
}

void
ClipEditor::stopPlayTimer()
{
   if ( m_playTimerId )
   {
      killTimer( m_playTimerId );
      m_playTimerId = 0;
   }

   if ( m_drawTimerId )
   {
      killTimer( m_drawTimerId );
      m_drawTimerId = 0;
   }
}


void ClipEditor::resizeEvent( QResizeEvent* event )
{
   update();
   QWidget::resizeEvent( event );
}

void ClipEditor::paintEvent( QPaintEvent* event )
{
   int w = width();
   int h = height();
   m_keyCount = std::clamp( (h - m_topHeight) / m_keyHeight, 0, 127 );
   m_keyStart = std::clamp( m_keyStart, 0, std::clamp( 127 - m_keyCount + 2, 0, 127 ) );
   //m_pixelPerBeat = w / m_beatCount;

   LiveSkin const & skin = m_app.m_skin;

   QPainter dc( this );
   dc.setRenderHint( QPainter::NonCosmeticDefaultPen );
   dc.fillRect( rect(), skin.panelColor );

   // Draw PianoBar + filled background of notes
   //int x = 0;
   int y = m_topHeight;

   for ( int i = 0; i < m_keyCount; ++i )
   {
      int keyIndex = m_keyStart + i;
      if ( keyIndex < 0 || keyIndex >= int( m_keys.size() ) )
      {
         DE_ERROR("keyIndex = ",keyIndex)
      }
      else
      {
         Key & key = m_keys[ keyIndex ];

         // Get key-rect
         QRect r( 0, y, m_keyWidth, m_keyHeight );

         // Draw key, determine colors ( black or white key )
         QColor fillColor = QColor(255,255,255);// white
         QColor penColor = QColor(205,205,205); // light white
         QColor textColor = QColor(2,2,2);      // xor
         if ( key.isBlack )
         {
            fillColor = QColor(2,2,2); // black
            penColor = QColor(65,65,65); // light black
            textColor = QColor(255,255,255); // xor
         }

         if (key.semi == 0) // C key more blue
         {
            fillColor = QColor(230,230,240);
         }

         drawKey( dc, r, fillColor, penColor );

         // Draw key text
         if ( m_keyHeight >= 9 )
         {
            auto s = MidiTools::noteStr( key.midiNote );
            auto t = QString::fromStdString( s );
            auto tx = r.x() + m_keyWidth - 6;
            auto ty = r.y() + m_keyHeight/2;
            auto textAlign = de::Align::MiddleRight;
            m_font5x8.drawText( dc, tx, ty, t, textColor, textAlign );
         }

         /// Draw note backgrounds
         fillColor = skin.whiteEditColor;    // white
         //strokeColor = QColor(220,220,220);  // light white
         if ( key.isBlack )
         {
            fillColor = skin.blackEditColor;    // white
            //strokeColor = QColor(150,150,150);
         }
         //dc.setPen( QPen( strokeColor ) );
         dc.fillRect( QRect(m_keyWidth, y, w-1-m_keyWidth, m_keyHeight), fillColor );

         if ( key.semi == 0 ) // every C draw blue line below
         {
            dc.setPen( QPen( QColor(100,100,255) ) );
            dc.setBrush( Qt::NoBrush );
            int y = r.y() + r.height() - 1;
            dc.drawLine( m_keyWidth, y, w-1-m_keyWidth, y );
         }

      }

      // End row
      y += m_keyHeight;
   }

   // Draw Time Grid ( 250ms )
   y = m_topHeight;
   dc.setPen( QPen( QColor( 155,155,155) ) );
   dc.setBrush( Qt::NoBrush );
   for ( int i = 0; i < 4*10; ++i )
   {
      int x = time2pixel( int64_t( 250*1000*1000 ) * i );
      int y1 = m_topHeight;
      int y2 = h - 1;
      dc.drawLine( x, y1, x, y2 );
   }

   // Draw Time Grid ( 1s )
   y = m_topHeight;
   dc.setPen( QPen( QColor( 200,200,200) ) );
   dc.setBrush( Qt::NoBrush );
   for ( int i = 0; i < 10; ++i )
   {
      int x = time2pixel( int64_t( 1000000000 ) * i );
      int y1 = m_topHeight;
      int y2 = h - 1;
      dc.drawLine( x, y1, x, y2 );
   }
   // Draw all visible notes
   y = m_topHeight;
   for ( int i = 0; i < m_keyCount; ++i )
   {
      int keyIndex = m_keyStart + i;
      if ( keyIndex < 0 || keyIndex >= int( m_keys.size() ) )
      {
         DE_ERROR("keyIndex = ",keyIndex)
      }
      else
      {
         dc.setPen( Qt::NoPen );
         Key & key = m_keys[ keyIndex ];
         for ( int j = 0; j < int( key.m_notes.size() ); ++j )
         {
            Note const & note = key.m_notes[ j ];
            int x1 = time2pixel( note.timeBeg );
            int x2 = time2pixel( note.timeEnd );
            dc.setBrush( QBrush( toQColor( note.color ) ) );
            dc.drawRect( x1, y, x2-x1+1, m_keyHeight );
         }
      }

      // End row
      y += m_keyHeight;
   }


   // [VisualDebug] Draw detected key index as full row rect.
   if ( m_isOverPianoBar )
   {
      //DE_DEBUG( "isOverPianoBar")
      dc.setPen( QPen( QColor(255,100,100) ) );
      dc.setBrush( Qt::NoBrush );
      QRect m_rectPianoBar( 0, m_topHeight, m_keyWidth, h-1-m_topHeight );
      dc.drawRect( m_rectPianoBar );
   }
   if ( m_isOverBeatGrid )
   {
      //DE_DEBUG( "isOverBeatGrid")
      dc.setPen( QPen( QColor(200,200,255) ) );
      dc.setBrush( Qt::NoBrush );
      QRect m_rectBeatGrid( m_keyWidth, m_topHeight, w-1-m_keyWidth, h-1-m_topHeight );
      dc.drawRect( m_rectBeatGrid );
   }

   if ( m_detectedKeyIndex > -1 )
   {
      //auto nsg = de::midi::MidiTools::noteStr( m_keys[ m_detectedKeyIndex ].midiNote );
      //DE_DEBUG( "m_detectedKeyIndex = ", m_detectedKeyIndex, ", ", nsg )
      dc.setPen( QPen( QColor(255,180,55) ) );
      dc.setBrush( Qt::NoBrush );
      QRect m_rectKeyIndex( 0, m_topHeight + (( m_detectedKeyIndex - m_keyStart ) * m_keyHeight), w-1, m_keyHeight-1 );
      dc.drawRect( m_rectKeyIndex );
   }

   // Draw Loop Start Time
   int xs = time2pixel( m_loopTimeStart );
   int xe = time2pixel( m_loopTimeEnd );
   int xt = time2pixel( m_loopTime );

   dc.setBrush( Qt::NoBrush );

   dc.setPen( QPen( QColor( 0,200,0 ) ) );
   dc.drawLine( xs, m_topHeight, xs, h );

   dc.setPen( QPen( QColor( 220,0,0 ) ) );
   dc.drawLine( xe, m_topHeight, xe, h );

   dc.setPen( QPen( QColor( 220,180,20 ) ) );
   dc.drawLine( xt, m_topHeight, xt, h );

   if ( m_dragMode == 1 ) // drawing
   {
      dc.setPen( Qt::NoPen );
      int x1 = m_dragStartX;
      int y1 = m_topHeight + (m_detectedKeyIndex - m_keyStart) * m_keyHeight;
      int x2 = m_mouseX;
      int y2 = y1 + m_keyHeight;
      dc.setBrush( QBrush( QColor(255,100,100) ) );
      dc.drawRect( x1, y1, x2-x1+1, y2-y1 );
   }
}




void
ClipEditor::play( int mode )
{
   m_isPlaying = true;
   m_timeStart = dbSeconds();
   //m_step = 0;
   startPlayTimer();
}

void
ClipEditor::stop()
{
   m_isPlaying = false;
   m_timeStart = dbSeconds();
   //m_step = 0;
   stopPlayTimer();
}

void
ClipEditor::hideEvent( QHideEvent* event )
{
   DE_DEBUG("")
   QWidget::hideEvent( event );
}

void
ClipEditor::showEvent( QShowEvent* event )
{
   DE_DEBUG("")
   QWidget::showEvent( event );
   //event->accept();
}

void
ClipEditor::setClip( Clip* clip )
{
   if ( m_clip != clip )
   {
      m_clip = clip;
      update();
   }
}

void
ClipEditor::setSynth( int i, de::audio::IDspChainElement* synth )
{
   if ( i < 0 || i >= int(m_synths.size()) ) return;
   m_synths[ i ] = synth;
}

void
ClipEditor::sendNote( de::audio::Note const & note )
{
   for ( auto synth : m_synths )
   {
      if ( !synth ) continue;
      synth->sendNote( note );
   }
}

void
ClipEditor::timerEvent( QTimerEvent* event )
{
   if ( event->timerId() == m_drawTimerId )
   {
      update();
   }

   if ( event->timerId() == m_playTimerId )
   {
      m_time = dbNanoseconds() - m_timeStart;
      m_loopTime = m_loopTimeStart
                 + m_time % m_loopTimeRange;

//      double timeStart = m_periodStep * m_step;
//      if ( t >= timeStart-0.5 )
//      {
//         auto & item = m_items[ m_step ];
//         if ( item.enabled->isChecked() && !item.isPlaying )
//         {
//            item.isPlaying = true;
//            de::audio::Note note;
//            note.m_midiNote = item.note->value();
//            note.m_velocity = 0; //item.velocity->value();
//            note.m_detune = 0; //item.detune->value();
//            sendNote( midiNote, detune, velocity );
//         }

//         m_step++;

//      }

//      if ( m_step >= m_step_count )
//      {
//         for ( auto & item : m_items)
//         {
//            item.isPlaying = false;
//         }
//         m_step = 0;
//      }
   }
   // if ( event->timerId() == m_timerId && m_isPlaying )
   // {
      //
   // }
}

bool
ClipEditor::isMouseOverPianoBar() const
{
   int w = width();
   int h = height();
   if ( w < 1 || h < 1 ) return false;
   if ( m_mouseY < m_topHeight || m_mouseY > h - 1 ) return false;
   if ( m_mouseX < 0 || m_mouseX > w - 1 || m_mouseX > m_keyWidth ) return false;
   return true;
}

bool
ClipEditor::isMouseOverBeatGrid() const
{
   int w = width();
   int h = height();
   if ( w < 1 || h < 1 ) return false;
   if ( m_mouseX < m_keyWidth || m_mouseX > w - 1 ) return false;
   if ( m_mouseY < m_topHeight || m_mouseY > h - 1 ) return false;
   return true;
}

int
ClipEditor::getKeyIndexFromMousePos() const
{
   int w = width();
   int h = height();
   // Check canvas
   if ( w < 1 || h < 1 ) return -1;
   // Check coords
   if ( m_mouseX < 0 || m_mouseX > w - 1 ) return -1;
   if ( m_mouseY < 0 || m_mouseY > h - 1 ) return -1;
   // compute key index, the higher mouse_y,
   // then the higher key index, but the lower the freq.
   int keyIndex = m_keyStart + (m_mouseY-m_topHeight) / m_keyHeight;
   if ( keyIndex < 0 || keyIndex > 127 )
   {
      DE_ERROR("No valid keyIndex = ",keyIndex) return -1;
   }
   return keyIndex;
}

ClipEditor::SelNote
ClipEditor::computeHoveredNote() const
{
//   if ( !m_clip )
//   {
//      return {};
//   }

   int keyIndex = m_detectedKeyIndex;
   if ( keyIndex < 0 || keyIndex > 127 )
   {
      DE_ERROR("No m_detectedKeyIndex = ",keyIndex)
      return {};
   }

   auto const & key = m_keys[ m_detectedKeyIndex ];
   auto const & notes = key.m_notes;
   for ( size_t i = 0; i < notes.size(); ++i )
   {
      auto const & note = notes[ i ];
      int a = time2pixel( note.timeBeg );
      int b = time2pixel( note.timeEnd );
      if ( m_mouseX >= a && m_mouseX < b )
      {
         SelNote sel;
         sel.keyIndex = m_detectedKeyIndex;
         sel.noteIndex = i;
         return sel;
      }
   }

   return {};
}

void
ClipEditor::mouseMoveEvent( QMouseEvent* event )
{
   m_mouseX = event->x();
   m_mouseY = event->y();

   m_isOverPianoBar = isMouseOverPianoBar();
   m_isOverBeatGrid = isMouseOverBeatGrid();
   m_detectedKeyIndex = getKeyIndexFromMousePos();
   m_hoveredNote = computeHoveredNote();

   if ( m_dragMode == 1 ) // drawing
   {
//      int sy = m_mouseY - m_dragStartY;
//      DE_DEBUG("StretchY(",sy,")")
   }
   if ( m_dragMode == 2 ) // stretch y
   {
      int sy = m_mouseY - m_dragStartY;
      DE_DEBUG("StretchY(",sy,")")
   }

   update();
   QWidget::mouseMoveEvent( event );
}

void
ClipEditor::wheelEvent( QWheelEvent* event )
{
   int wheel = event->angleDelta().y();
   DE_DEBUG("MouseWheel = ",wheel )

   if ( wheel >= 1 )
   {
      m_keyStart = std::clamp( m_keyStart - 3, 0, 125 );
      update();
   }
   else if ( wheel <= 1 )
   {
      m_keyStart = std::clamp( m_keyStart + 3, 0, 125 );
      update();
   }

   QWidget::wheelEvent( event );

//   if ( m_driver )
//   {
//      de::SEvent post;
//      post.type = de::EET_MOUSE_EVENT;
//      post.mouseEvent.m_Flags = de::SMouseEvent::Wheel;
//      post.mouseEvent.m_X = event->x();
//      post.mouseEvent.m_Y = event->y();
//      post.mouseEvent.m_Wheel = event->angleDelta().y(); //    //QPoint delta = event->pixelDelta();
//      // DE_DEBUG("post.mouseEvent.m_Wheel = ",post.mouseEvent.m_Wheel)
//      m_driver->postEvent( post );
//   }

   //event->accept();
}


void
ClipEditor::mousePressEvent( QMouseEvent* event )
{
//   m_X = event->x();
//   m_Y = event->y();
//   m_Wheel = 0.0f;
   m_mouseX = event->x();
   m_mouseY = event->y();


   int x = event->x();
   int y = event->y();

   if ( m_dragMode > 0 )
   {
      DE_ERROR("Already dragging ", m_dragMode)
   }
   else
   {
      m_dragStartX = x;
      m_dragStartY = y;
      //m_selectedNote = m_hoveredNote;

      if ( m_isOverBeatGrid )
      {
         m_dragMode = 1;
         DE_DEBUG("DrawMode ", m_dragMode)
      }

      if ( m_isOverPianoBar )
      {
         m_dragMode = 2;
         DE_DEBUG("StretchMode ", m_dragMode)
      }
      update();
   }

   QWidget::mousePressEvent( event );
}

int64_t
ClipEditor::pixel2time( int64_t px ) const
{
   return (px - m_keyWidth) * m_nanosecondsPerPixel;
}

int64_t
ClipEditor::time2pixel( int64_t timeInNanoSec ) const
{
   return m_keyWidth + (timeInNanoSec * m_pixelsPerNanosecond );
}

void
ClipEditor::mouseReleaseEvent( QMouseEvent* event )
{
   m_mouseX = event->x();
   m_mouseY = event->y();

//   m_isOverPianoBar = isMouseOverPianoBar();
//   m_isOverBeatGrid = isMouseOverBeatGrid();

   // Finish Drawing
   if ( m_dragMode == 1 && m_detectedKeyIndex > -1 )
   {
      auto a = pixel2time( m_dragStartX );
      auto b = pixel2time( m_mouseX );
      if ( a > b )
      {
         std::swap( a, b );
      }
      if ( a < b )
      {
         Note note;
         note.timeBeg = a;
         note.timeEnd = b;
         note.color = de::randomColor();
         Key & key = m_keys[ m_detectedKeyIndex ];
         key.m_notes.emplace_back( std::move( note ) );
         DE_DEBUG("Added Note ")
      }
   }

   // Finish StretchY
   if ( m_dragMode == 2 && m_isOverPianoBar )
   {
      int sy = m_mouseY - m_dragStartY;
      DE_DEBUG("StretchY Final(",sy,")")
      m_keyHeight = std::clamp( m_keyHeight + sy/4, 2, 48 );
   }

   // Reset all
   m_dragMode = 0;

   // Enqueue ( async ) request for redraw.
   update();

   QWidget::mouseReleaseEvent( event );
}


void
ClipEditor::keyPressEvent( QKeyEvent* event )
{
   //DE_DEBUG("KeyPress(",event->key(),")")

//   if ( m_driver )
//   {
//      de::SEvent post;
//      post.type = de::EET_KEY_EVENT;
//      post.keyEvent.Key = de::KEY_UNKNOWN;
//      post.keyEvent.Flags = de::SKeyEvent::Pressed;
//      if ( event->modifiers() & Qt::ShiftModifier ) { post.keyEvent.Flags |= de::SKeyEvent::Shift; }
//      if ( event->modifiers() & Qt::ControlModifier ) { post.keyEvent.Flags |= de::SKeyEvent::Ctrl; }
//      if ( event->modifiers() & Qt::AltModifier ) { post.keyEvent.Flags |= de::SKeyEvent::Alt; }
//      parseQtKey( event, post );
//      m_driver->postEvent( post );
//   }

   QWidget::keyPressEvent( event );
}

void
ClipEditor::keyReleaseEvent( QKeyEvent* event )
{
   //DE_DEBUG("KeyRelease(",event->key(),")")

//   if ( m_driver )
//   {
//      de::SEvent post;
//      post.type = de::EET_KEY_EVENT;
//      post.keyEvent.Key = de::KEY_UNKNOWN;
//      post.keyEvent.Flags = de::SKeyEvent::None;
//      if ( event->modifiers() & Qt::ShiftModifier ) { post.keyEvent.Flags |= de::SKeyEvent::Shift; }
//      if ( event->modifiers() & Qt::ControlModifier ) { post.keyEvent.Flags |= de::SKeyEvent::Ctrl; }
//      if ( event->modifiers() & Qt::AltModifier ) { post.keyEvent.Flags |= de::SKeyEvent::Alt; }
//      parseQtKey( event, post );
//      m_driver->postEvent( post );
//   }

//   event->accept();
   QWidget::keyReleaseEvent( event );
}

