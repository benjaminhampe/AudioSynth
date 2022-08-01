#include <de/gpu/ui/UI.hpp>
#include <de/gpu/IVideoDriver.hpp>

namespace de {
namespace gpu {

// =====================================================================
UI::UI()
// =====================================================================
   : m_Driver( nullptr )
   , m_MouseX( 0 )
   , m_MouseY( 0 )
{
}

UI::~UI()
{
   clearWidgets();
}

void
UI::clearWidgets()
{
   for ( size_t i = 0; i < m_Widgets.size(); ++i )
   {
      SAFE_DELETE( m_Widgets[ i ] );
   }
   m_Widgets.clear();

   for ( size_t i = 0; i < m_TopLevel.size(); ++i )
   {
      SAFE_DELETE( m_TopLevel[ i ] );
   }
   m_TopLevel.clear();
}

Widget*
UI::getWidget( int id )
{
   if ( id < 1 ) { return nullptr; }
   for ( size_t i = 0; i < m_Widgets.size(); ++i )
   {
      if ( !m_Widgets[ i ] )
      {
         throw std::runtime_error( "Got nullptr as widget");
      }
      if ( m_Widgets[ i ]->getId() == id )
      {
         return m_Widgets[ i ];
      }
   }
   return nullptr;
}

int32_t
UI::findWidget( Widget* w ) const
{
   if ( !w ) { return -1; }
   for ( size_t i = 0; i < m_Widgets.size(); ++i )
   {
      if ( !m_Widgets[ i ] )
      {
         throw std::runtime_error( "Got nullptr as widget");
      }
      if ( m_Widgets[ i ] == w )
      {
         return int32_t( i );
      }
   }
   return -1;
}

void
UI::dump()
{
   DE_DEBUG("m_Mouse(",m_MouseX,", ", m_MouseY,"), m_TopLevel.size() = ",m_TopLevel.size())

   for ( size_t i = 0; i < m_TopLevel.size(); ++i )
   {
      Widget* topLevel = m_TopLevel[ i ];
      DE_DEBUG("m_TopLevel[",i,"]", topLevel)
   }
}

void
UI::drawShape( Widget::EShape shape, Recti const & pos, glm::ivec2 const & radius,
      uint32_t fillColor, TexRef const & tex,
      int borderWidth, uint32_t borderColor )
{
   if ( !m_Driver ) { DE_ERROR("No driver") return; }

   int tess = 33;

   //DE_DEBUG("Shape(",int(shape),") Pos(",pos,"), Radius(",radius,")")

   if ( shape == Widget::WS_Rect )
   {
      m_Driver->draw2DRect( pos, fillColor, tex );
      m_Driver->draw2DRectOutline( pos, borderColor, tex, borderWidth );
   }
   else if ( shape == Widget::WS_RoundRect )
   {
      m_Driver->draw2DRoundRect( pos, radius.x, radius.y, fillColor, tex, tess );
      m_Driver->draw2DRoundRectOutline( pos, radius.x, radius.y, borderColor, tex, borderWidth, tess );
   }
   else if ( shape == Widget::WS_Circle )
   {
      m_Driver->draw2DCircle( pos, fillColor, tex, tess );
      m_Driver->draw2DCircleOutline( pos, borderColor, tex, borderWidth, tess );
   }

//   int cx = pos.getCenterX();
//   int cy = pos.getCenterY();
//   m_Driver->draw2DText5x8( cx,cy, m_Text, m_TextColor, de::Align::Centered );
}


void
UI::update()
{
   while ( !m_EventFifo.empty() )
   {
      handleEvent( m_EventFifo.front() );
      m_EventFifo.pop_front();
   }
}

void
UI::handleEvent( SEvent event )
{
   if ( event.type == EET_MOUSE_EVENT )
   {
      m_MouseX = event.mouseEvent.x();
      m_MouseY = event.mouseEvent.y();

      if ( event.mouseEvent.m_Flags & SMouseEvent::Pressed )
      {
         if ( event.mouseEvent.m_Flags & SMouseEvent::BtnLeft )
         {
            DE_DEBUG("LEFT PRESSED x(",m_MouseX,"), y(",m_MouseY,")")
         }
         else if ( event.mouseEvent.m_Flags & SMouseEvent::BtnRight )
         {
            DE_DEBUG("RIGHT PRESSED x(",m_MouseX,"), y(",m_MouseY,")")
         }
         else if ( event.mouseEvent.m_Flags == SMouseEvent::BtnMiddle )
         {
            DE_DEBUG("MIDDLE PRESSED x(",m_MouseX,"), y(",m_MouseY,")")
         }
      }
      else // if ( event.mouseEvent.m_Flags & SMouseEvent::Released )
      {
         if ( event.mouseEvent.m_Flags & SMouseEvent::BtnLeft )
         {
            DE_DEBUG("LEFT Released x(",m_MouseX,"), y(",m_MouseY,")")
         }
         else if ( event.mouseEvent.m_Flags & SMouseEvent::BtnRight )
         {
            DE_DEBUG("RIGHT Released x(",m_MouseX,"), y(",m_MouseY,")")
         }
         else if ( event.mouseEvent.m_Flags == SMouseEvent::BtnMiddle )
         {
            DE_DEBUG("MIDDLE Released x(",m_MouseX,"), y(",m_MouseY,")")
         }
      }
   }
}

void
UI::postEvent( SEvent event )
{
   m_EventFifo.emplace_back( event );
}

void
UI::render()
{
   if ( !m_Driver )
   {
      DE_ERROR("No driver")
      return;
   }

   update();

   // Pick - 2D Collision-Detection by CPU ColorPicking
   int w = m_Driver->getScreenWidth();
   int h = m_Driver->getScreenHeight();

   m_ColorPicking.resize( w,h );
   m_ColorPicking.clear();
   for ( uint32_t i = 0; i < m_Widgets.size(); ++i )
   {
      Widget * p = m_Widgets[ i ];
      if ( !p ) { continue; }
      m_ColorPicking.draw( p->getAbsoluteRect(), i | 0xFF000000 );
   }

   uint32_t pick = m_ColorPicking.pick( m_MouseX, m_MouseY );

   // Pick - 2D Collision-Detection
   m_HoverIndex = -1;
   if ( pick & 0xFF000000 ) // Alpha means something found
   {
      m_HoverIndex = (pick & 0x00FFFFFF); // dont use alpha for id/index.
   }

   // Pick - 2D Collision-Detection
//   m_HoverIndex = -1;

//   for ( int i = 0; i < m_Widgets.size(); ++i )
//   {
//      Widget * p = m_Widgets[ i ];
//      if ( !p ) { continue; }
//      Widget & w = *p;
//      if ( !w.isReallyVisible() ) { continue; }
//      if ( w.getAbsoluteRect().contains( mx,my ) )
//      {
//         //DEM_DEBUG("Hit element ", i, ", name:", dbStr(elem.text) )
//         m_HoverIndex = int(i);
//         break;
//      }
//   }

   if ( m_HoverIndex > -1 )
   {
      m_HoverWidget = m_Widgets[ m_HoverIndex ];
   }

   // Layout
   //int w = m_Driver->getScreenWidth();
   //int h = m_Driver->getScreenHeight();
   //m_Pos = de::Align::apply( elem.start_pos, elem.align, w, h );

   //DE_DEBUG("Draw m_TopLevel.size() = ",m_TopLevel.size())

   for ( size_t i = 0; i < m_TopLevel.size(); ++i )
   {
      Widget* elem = m_TopLevel[ i ];
      if ( !elem ) { DE_ERROR("!w ",i) continue; }
      if ( !elem->m_IsVisible ) { DE_ERROR("!elem->m_IsVisible ",i) continue; }
      elem->render();
   }

   if ( m_HoverWidget )
   {
      m_Driver->draw2DRectOutline( m_HoverWidget->getAbsoluteRect(), m_Skin.HoverBorderColor, TexRef(), 1 );
   }

   if ( m_FocusWidget )
   {
      m_Driver->draw2DRectOutline( m_FocusWidget->getAbsoluteRect(), m_Skin.FocusBorderColor, TexRef(), 1 );
   }
}

void
UI::setDriver( IVideoDriver* driver )
{
   //DE_DEBUG("Set driver ", driver)
   m_Driver = driver;
}

IVideoDriver*
UI::getDriver()
{
   return m_Driver;
}

} // end namespace gpu.
} // end namespace de.

