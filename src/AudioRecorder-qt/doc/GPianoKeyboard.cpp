#include "GPianoKeyboard.hpp"
#include <QPainter>
#include <QDebug>

GPianoKeyboard::GPianoKeyboard( QWidget* parent )
   : QWidget( parent ) // , Qt::NoDropShadowWindowHint
   , m_timerId( 0 )
   , m_keyStart( 0 )
   , m_timeLastNote( 0.0 )
   , m_timeNote( 0.0 )
{
   setContentsMargins( 0,0,0,0 );
   setMinimumSize( 120, 64 );
   setFocusPolicy( Qt::StrongFocus );
   setMouseTracking( true );
   for ( auto & synth : m_synths ) { synth = nullptr; }
   for ( auto & key : m_touched ) { key = nullptr; }
   setKeyRange( 24, 100 );

//   de::Image imgC = createImage( 0, 24, 64 );
//   dbSaveImage( imgC, "Audio_imgC.png");
//   QImage m_imgC = createKeyImage( 0, 24, 64 );
//   m_imgC.save("Audio_m_imgC.png");
}

GPianoKeyboard::~GPianoKeyboard()
{

}

int32_t
GPianoKeyboard::findKey( int midiNote ) const
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
GPianoKeyboard::setKeyRange( int midiNoteStart, int midiNoteCount )
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

void
GPianoKeyboard::setSynth( int i, de::audio::IDspChainElement* synth )
{
   if ( i < 0 || i >= m_synths.size() ) return;
   m_synths[ i ] = synth;
}



namespace {

void drawKey( QPainter & dc, QRect pos, QColor brushColor, QColor penColor )
{
   int x = pos.x()+1;
   int y = pos.y()+1;
   int w = pos.width()-1;
   int h = pos.height()-2;
   dc.setPen( Qt::NoPen );
   dc.setBrush( QBrush( brushColor ) );
   dc.drawRect( QRect(x+1,y+1,w-2,h-2) );

   dc.setPen( QPen( penColor ) );
   dc.drawLine( x+1,y, x+w-2, y );
   dc.drawLine( x+1,y+h-1, x+w-2, y+h-1 );
   dc.drawLine( x,y+1, x, y+h-2 );
   dc.drawLine( x+w-1,y+1, x+w-1, y+h-2 );
}

} // end namespace

void
GPianoKeyboard::paintEvent( QPaintEvent* event )
{
   QPainter dc(this);
   dc.setRenderHint( QPainter::NonCosmeticDefaultPen );
   dc.fillRect( rect(), QColor( 10,10,10 ) );

   int w = width();
   int h = height();

   QColor whiteBrushColor(255,255,255);
   QColor whitePenColor(205,205,205);

   QColor blackBrushColor(2,2,2);
   QColor blackPenColor(65,65,65);

   int gw = 20;
   int gh = 64;


   int x = 0;
   int y = 0;
   int k = w / gw;

   //m_keys[ keyIndex ].pressed = true;

   //int keyCount = std::min( int(m_keys.size()), k );

   //int whiteCount = std::min( int(m_keys.size()), k );

   for ( int i = 0; i < k+1; ++i )
   {
      drawKey( dc, QRect(x,y,gw,gh), whiteBrushColor, whitePenColor );
      x += gw;
   }

   x = 12;
   y = 0;

   for ( int i = 0; i < k; ++i )
   {
      int semi = i % 7;
      if ( semi != 2 && semi != 6 )
      {
         drawKey( dc, QRect(x,y,16,32), blackBrushColor, blackPenColor );
      }
      x += 20;
   }

//   m_font5x8.drawText( dc, w/2, 1, QString("L_min(%1), L_max(%2), R_min(%3), R_max(%4)")
//      .arg( L_min).arg( L_max ).arg( R_min).arg( R_max ), 0xFFFFFFFF, de::Align::CenterTop );

}


void
GPianoKeyboard::timerEvent( QTimerEvent* event )
{
   if ( event->timerId() == m_timerId )
   {
      update();
   }
}


void
GPianoKeyboard::resizeEvent( QResizeEvent* event )
{
   update();
}

void
GPianoKeyboard::mouseMoveEvent( QMouseEvent* event )
{
   // m_MousePos = glm::ivec2( event->x(), event->y() ); // current mouse pos
   // if ( m_MousePos != m_LastMousePos ) // Some button is clicked...
   // {
      // m_MouseMove = m_MousePos - m_LastMousePos; // current mouse pos
      // m_LastMousePos = m_MousePos;
   // }
   // else
   // {
      // m_MouseMove = { 0,0 };
   // }
}

void
GPianoKeyboard::mousePressEvent( QMouseEvent* event )
{
   // if ( m_Driver )
   // {
      // de::SEvent post;
      // post.type = de::EET_MOUSE_EVENT;
      // post.mouseEvent.m_Flags = de::SMouseEvent::Pressed;
      // post.mouseEvent.m_X = event->x();
      // post.mouseEvent.m_Y = event->y();
      // post.mouseEvent.m_Wheel = 0.0f;

      // if ( event->button() == Qt::LeftButton )
      // {
         // post.mouseEvent.m_Flags |= de::SMouseEvent::BtnLeft;
      // }
      // else if ( event->button() == Qt::RightButton )
      // {
         // post.mouseEvent.m_Flags |= de::SMouseEvent::BtnRight;
      // }
      // else if ( event->button() == Qt::MiddleButton )
      // {
         // post.mouseEvent.m_Flags |= de::SMouseEvent::BtnMiddle;
      // }
      // m_Driver->postEvent( post );

      // auto camera = m_Driver->getActiveCamera();
      // if ( camera )
      // {
         // de::gpu::Ray3d ray = camera->computeRay( event->x(), event->y() );
         // auto a = ray.getPos() + ray.getDir() * 10.0;
         // auto b = ray.getPos() + ray.getDir() * 1000.0;
         // m_LineRenderer.add3DLine( a,b, de::randomColor() );
      // }

   // }

   //m_MousePos = glm::ivec2( event->x(), event->y() ); // current mouse pos
}

void
GPianoKeyboard::mouseReleaseEvent( QMouseEvent* event )
{
   // if ( m_Driver )
   // {
      // de::SEvent post;
      // post.type = de::EET_MOUSE_EVENT;
      // post.mouseEvent.m_Flags = de::SMouseEvent::Released;
      // post.mouseEvent.m_X = event->x();
      // post.mouseEvent.m_Y = event->y();
      // post.mouseEvent.m_Wheel = 0.0f;
      // if ( event->button() == Qt::LeftButton )
      // {
         // post.mouseEvent.m_Flags |= de::SMouseEvent::BtnLeft;
      // }
      // else if ( event->button() == Qt::RightButton )
      // {
         // post.mouseEvent.m_Flags |= de::SMouseEvent::BtnRight;
      // }
      // else if ( event->button() == Qt::MiddleButton )
      // {
         // post.mouseEvent.m_Flags |= de::SMouseEvent::BtnMiddle;
      // }
      // m_Driver->postEvent( post );
   // }

   //m_MousePos = glm::ivec2( event->x(), event->y() ); // current mouse pos
   //m_MouseMove = { 0,0 };
}


void
GPianoKeyboard::wheelEvent( QWheelEvent* event )
{
   // if ( m_Driver )
   // {
      // de::SEvent post;
      // post.type = de::EET_MOUSE_EVENT;
      // post.mouseEvent.m_Flags = de::SMouseEvent::Wheel;
      // post.mouseEvent.m_X = event->x();
      // post.mouseEvent.m_Y = event->y();
      // post.mouseEvent.m_Wheel = event->angleDelta().y(); //    //QPoint delta = event->pixelDelta();
      // // DE_DEBUG("post.mouseEvent.m_Wheel = ",post.mouseEvent.m_Wheel)
      // m_Driver->postEvent( post );
   // }

   //event->accept();
}

void
GPianoKeyboard::keyPressEvent( QKeyEvent* event )
{
   auto noteOn = [&] ( int midiNote, int velocity = 90 )
   {
      for ( auto synth : m_synths )
      {
         if ( synth )
         {
            synth->sendNote( de::audio::Note( 0, midiNote, 0, velocity ) );
         }
      }

      int keyIndex = findKey( midiNote );
      if ( keyIndex > -1 )
      {
         m_keys[ keyIndex ].pressed = true;
      }
   };

   auto key = event->key();

   DE_DEBUG("KeyPress(",key,")")

   int k = 59; // 59;
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

   k = 48;
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

   event->accept();
}

void
GPianoKeyboard::keyReleaseEvent( QKeyEvent* event )
{
   auto noteOff = [&] ( int midiNote, int velocity = 90 )
   {
      int keyIndex = findKey( midiNote );
      if ( keyIndex > -1 )
      {
         m_keys[ keyIndex ].pressed = false;
      }

      for ( auto synth : m_synths )
      {
         if ( synth )
         {
            de::audio::Note note( 0, midiNote, 0, 0 );
            //note.m_noteOff = true;
            synth->sendNote( note );
         }
      }
   };

   auto key = event->key();

   DE_DEBUG("keyRelease(",key,")")

   int k = 59; // 59;
   if ( key == Qt::Key_1 ) { noteOff( k ); } k++;
   if ( key == Qt::Key_Q ) { noteOff( k ); } k++;
   if ( key == Qt::Key_2 ) { noteOff( k ); } k++;
   if ( key == Qt::Key_W ) { noteOff( k ); } k++;
   if ( key == Qt::Key_3 ) { noteOff( k ); } k++;
   if ( key == Qt::Key_E ) { noteOff( k ); } k++;
   if ( key == Qt::Key_R ) { noteOff( k ); } k++;
   if ( key == Qt::Key_5 ) { noteOff( k ); } k++;
   if ( key == Qt::Key_T ) { noteOff( k ); } k++;
   if ( key == Qt::Key_6 ) { noteOff( k ); } k++;
   if ( key == Qt::Key_Z ) { noteOff( k ); } k++;
   if ( key == Qt::Key_7 ) { noteOff( k ); } k++;
   if ( key == Qt::Key_U ) { noteOff( k ); } k++;
   if ( key == Qt::Key_I ) { noteOff( k ); } k++;
   if ( key == Qt::Key_9 ) { noteOff( k ); } k++;
   if ( key == Qt::Key_O ) { noteOff( k ); } k++;
   if ( key == Qt::Key_0 ) { noteOff( k ); } k++;
   if ( key == Qt::Key_P ) { noteOff( k ); } k++;

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

   event->accept();
}


