#include "ArrangeDropTarget.hpp"
#include <QResizeEvent>

#include "App.hpp"

ArrangeDropTarget::ArrangeDropTarget( App & app, QWidget* parent )
   : QWidget(parent)
   , m_app( app )
{
   setObjectName( "ArrangeDropTarget" );
   setContentsMargins( 0,0,0,0 );
   setMouseTracking( true );
   setAcceptDrops( true );
}

// void
// ArrangeDropTarget::resizeEvent( QResizeEvent* event )
// {
   // QWidget::resizeEvent( event );
// }

void ArrangeDropTarget::paintEvent( QPaintEvent* event )
{
   int w = width();
   int h = height();

   if ( w > 0 && h > 0 )
   {
      QPainter dc( this );
      dc.setRenderHint( QPainter::NonCosmeticDefaultPen );

      auto const & skin = m_app.m_skin;
      dc.fillRect( rect(), skin.contentColor );
      dc.setPen( QPen( skin.symbolColor ) );
      dc.setBrush( QBrush( skin.symbolColor ) );

      //QString msg = "Ziehen Sie Audio-Effekte hierhin";

      QRect br = QRect( 6, 6, w-12, h-12 );
      dc.drawText( br, Qt::TextWordWrap | Qt::AlignCenter, "Drop synth plugin to create Midi Track\n"
                                                           "Drop audio plugin to create Audio-Only Track", &br );
   }
}


void
ArrangeDropTarget::dropEvent( QDropEvent* event )
{
   std::cout << "ArrangeDropTarget::dropEvent()" << std::endl;
   std::cout << event->mimeData()->text().toStdString() << std::endl;

   // FIND PLUGIN uri IN DATABASE

   std::wstring uri = event->mimeData()->text().toStdWString();

   auto p = m_app.getPluginInfo( uri );
   if ( p )
   {
      int id = p->isSynth() ? 3000 + m_app.m_userMidiTracks.size() : 2000 + m_app.m_userAudoTracks.size();
      std::string name = QString("%1 %2").arg(id).arg(QString::fromStdWString(p->m_name) ).toStdString();
      eTrackType tt = p->isSynth() ? eTrackMidi : eTrackAudio;
      QColor color = toQColor( de::randomRGB() );
      m_app.addTrack( id, name, tt, color );
      m_app.addPlugin( uri, true );
   }
   else
   {
      std::wcout << "ArrangeDropTarget::dropEvent() - No plugin info found in db " << uri << std::endl;
   }


   event->acceptProposedAction();
   QWidget::dropEvent( event );
}

void ArrangeDropTarget::dragEnterEvent( QDragEnterEvent* event )
{
   if ( event->mimeData()->hasFormat("text/plain") )
   {
      event->acceptProposedAction();
   }
   std::cout << __func__ << std::endl;
   QWidget::dragEnterEvent( event );
}

void ArrangeDropTarget::dragLeaveEvent( QDragLeaveEvent* event )
{
   std::cout << __func__ << std::endl;
   QWidget::dragLeaveEvent( event );
}

void ArrangeDropTarget::dragMoveEvent(QDragMoveEvent* event )
{
   std::cout << __func__ << std::endl;
   QWidget::dragMoveEvent( event );
}

void
ArrangeDropTarget::focusInEvent( QFocusEvent* event )
{
//   m_hasFocus = true;
   update();
   QWidget::focusInEvent( event );
}

void
ArrangeDropTarget::focusOutEvent( QFocusEvent* event )
{
//   m_hasFocus = true;
   update();
   QWidget::focusOutEvent( event );
}

void
ArrangeDropTarget::enterEvent( QEvent* event )
{
   QWidget::enterEvent( event );
}

void
ArrangeDropTarget::leaveEvent( QEvent* event )
{
   QWidget::leaveEvent( event );
}
