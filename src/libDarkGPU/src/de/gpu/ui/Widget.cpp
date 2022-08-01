#include <de/gpu/ui/Widget.hpp>
#include <de/gpu/ui/UI.hpp>
#include <de/gpu/IVideoDriver.hpp>

namespace de {
namespace gpu {

// =====================================================================
Widget::Widget( UI* manager, Widget* parent, int id )
// =====================================================================
   : m_UI( manager )
   , m_Parent( parent )
   , m_Id( id )
   , m_IsVisible( true )
   , m_IsModal( false )
   , m_Size(24,24)
   , m_Pos(0,0)
   , m_Radius(8,8)
{
   //if ( m_UI ) m_UI->registerWidget( this );
}

Widget::~Widget()
{
   DE_DEBUG("Delete Children ", m_Children.size())
   for ( size_t i = 0; i < m_Children.size(); ++i )
   {
      SAFE_DELETE( m_Children[ i ] );
   }
   m_Children.clear();
}

glm::ivec2
Widget::getAbsolutePos() const
{
   if ( m_Parent )
   {
      return m_Parent->getAbsolutePos();
   }
   else
   {
      return m_Pos;
   }
}

Recti
Widget::getAbsoluteRect() const
{
   auto pos = getAbsolutePos();
   return Recti(pos.x, pos.y, m_Size.x, m_Size.y );
}

bool
Widget::isReallyVisible() const
{
   if ( m_Parent )
   {
      return m_Parent->isReallyVisible() && m_IsVisible;
   }
   else
   {
      return m_IsVisible;
   }
}

void
Widget::render()
{
   for ( size_t i = 0; i < m_Children.size(); ++i )
   {
      if ( !m_Children[ i ] )
      {
         // throw std::runtime_error( "Widget has nullptr as child " + std::to_string( i ) );
         DE_ERROR( "Widget has nullptr as child ",i )
         continue;
      }
      m_Children[ i ]->render();
   }

//   de::gpu::IVideoDriver* driver = m_UI->getDriver();
//   if ( !driver ) { DE_ERROR("No driver") return; }

//   glm::ivec2 origin(0,0);
//   if ( m_Parent ) { origin = m_Parent->getAbsolutePos(); }

//   // Layout
//   //int w = driver->getScreenWidth();
//   //int h = driver->getScreenHeight();
//   //elem.pos = de::Align::apply( elem.start_pos, elem.align, w, h );
//   Recti pos( origin.x + m_Pos.x, origin.y + m_Pos.y, m_Size.x, m_Size.y );

//   //DE_DEBUG("Render UI type(",int(m_Type),") Rect(",pos,")")
//   m_UI->drawShape( m_Shape, pos, m_Radius, m_FillColor, m_TexRef, m_BorderWidth, m_BorderColor );

//   int cx = pos.getCenterX();
//   int cy = pos.getCenterY();
//   driver->draw2DText5x8( cx,cy, m_Text, m_TextColor, de::Align::Centered );

//   for ( size_t i = 0; i < m_Children.size(); ++i )
//   {
//      if ( !m_Children[ i ] )
//      {
//         // throw std::runtime_error( "Widget has nullptr as child " + std::to_string( i ) );
//         DE_ERROR( "Widget has nullptr as child ",i )
//         continue;
//      }
//      m_Children[ i ]->render();
//   }
}


// =====================================================================
Panel::Panel( UI* manager, Widget* parent, int id )
// =====================================================================
   : Widget( manager, parent, id )
{

}

void
Panel::render()
{
   Widget::render();
}

// =====================================================================
Label::Label( std::string txt, UI* manager, Widget* parent, int id )
// =====================================================================
   : Widget( manager, parent, id )
   , m_TextColor( 0xFFFFFFFF )
   , m_TextShadowColor( 0xFF000000 )
   , m_TextAlign( Align::TopLeft )
   , m_Text( txt )
{}

//Label::~Label()
//{}

void
Label::render()
{
   if ( !m_UI ) { DE_ERROR("No ui") return; }
   de::gpu::IVideoDriver* driver = m_UI->getDriver();
   if ( !driver ) { DE_ERROR("No driver") return; }
   glm::ivec2 pos(0,0);
   if ( m_Parent ) { pos = m_Parent->getAbsolutePos(); }
   Recti rect( pos.x + m_Pos.x, pos.y + m_Pos.y, m_Size.x, m_Size.y );
   int cx = rect.getCenterX();
   int cy = rect.getCenterY();
   driver->draw2DText5x8( cx,cy, m_Text, m_TextColor, m_TextAlign );
}

// =====================================================================
Button::Button( std::string txt, UI* manager, Widget* parent, int id )
// =====================================================================
   : Widget( manager, parent, id )
   , m_IsEnabled( true )
   , m_IsChecked( false )
   , m_IsCheckable( false )
   , m_TextAlign( Align::Centered )
   , m_TextColor( 0xFFFFFFFF )
   , m_TextShadowColor( 0xFF000000 )
   , m_Text( txt )
   , m_FillColor( 0xC0CCCCCC )
   , m_FillShape( WS_Rect )
   , m_FillTex()
   , m_BorderWidth(1)
   , m_BorderColor( 0xFFDDDDDD )
   , m_BorderColorLight( lightenColor(m_BorderColor,50) )
   , m_BorderColorDark( darkenColor(m_BorderColor,50) )
{}

//Button::~Button()
//{}

void
Button::render()
{
   if ( !m_UI ) { DE_ERROR("No ui") return; }
   de::gpu::IVideoDriver* driver = m_UI->getDriver();
   if ( !driver ) { DE_ERROR("No driver") return; }

   glm::ivec2 pos(0,0);
   if ( m_Parent ) { pos = m_Parent->getAbsolutePos(); }
   Recti rect( pos.x + m_Pos.x, pos.y + m_Pos.y, m_Size.x, m_Size.y );
   m_UI->drawShape( WS_Rect, rect, m_Radius, m_FillColor, m_FillTex, m_BorderWidth, m_BorderColor );

   int cx = rect.getCenterX();
   int cy = rect.getCenterY();
   driver->draw2DText5x8( cx,cy, m_Text, m_TextColor, m_TextAlign, m_TextFont );
}

// =====================================================================
CheckBox::CheckBox( std::string txt, UI* manager, Widget* parent, int id )
// =====================================================================
   : Widget( manager, parent, id )
   , m_IsEnabled( true )
   , m_IsChecked( false )
   , m_IsCheckable( false )
   , m_TextAlign( Align::TopLeft )
   , m_TextColor( 0xFFFFFFFF )
   , m_TextShadowColor( 0xFF000000 )
   , m_Text( txt )
   , m_FillColor( 0xC0CCCCCC )
   , m_FillShape( WS_Rect )
   , m_FillTex()
   , m_BorderWidth(1)
   , m_BorderColor( 0xFFDDDDDD )
   , m_BorderColorLight( lightenColor(m_BorderColor,50) )
   , m_BorderColorDark( darkenColor(m_BorderColor,50) )
{}

//CheckBox::~CheckBox()
//{}

void
CheckBox::render()
{
   if ( !m_UI ) { DE_ERROR("No ui") return; }
   de::gpu::IVideoDriver* driver = m_UI->getDriver();
   if ( !driver ) { DE_ERROR("No driver") return; }

   glm::ivec2 pos(0,0);
   if ( m_Parent ) { pos = m_Parent->getAbsolutePos(); }
   Recti r_cbx( pos.x + m_Pos.x, pos.y + m_Pos.y, m_Size.y, m_Size.y );
   m_UI->drawShape( WS_Rect, r_cbx, {0,0}, 0xFFFFFFFF, TexRef(), 1, 0x80444444 );
   driver->draw2DText5x8( r_cbx.x, r_cbx.y, m_Text, m_TextColor, de::Align::TopLeft, m_TextFont );
}

// =====================================================================
EditBox::EditBox( std::string txt, UI* manager, Widget* parent, int id )
// =====================================================================
   : Widget( manager, parent, id )
   , m_IsEnabled( true )
   , m_TextAlign( Align::TopCenter )
   , m_TextColor( 0xFFFFFFFF )
   , m_TextShadowColor( 0xFF000000 )
   , m_Text( txt )
   , m_FillColor( 0xC0CCCCCC )
   , m_FillShape( WS_Rect )
   , m_FillTex()
   , m_BorderWidth(1)
   , m_BorderColor( 0xFFDDDDDD )
   , m_BorderColorLight( lightenColor(m_BorderColor,50) )
   , m_BorderColorDark( darkenColor(m_BorderColor,50) )
{
   m_Size.x = 100;
   m_Size.y = 24;
}

//EditBox::~EditBox()
//{}

void
EditBox::render()
{
   if ( !m_UI ) { DE_ERROR("No ui") return; }
   de::gpu::IVideoDriver* driver = m_UI->getDriver();
   if ( !driver ) { DE_ERROR("No driver") return; }

   glm::ivec2 pos(0,0);
   if ( m_Parent ) { pos = m_Parent->getAbsolutePos(); }
   Recti rect( pos.x + m_Pos.x, pos.y + m_Pos.y, m_Size.x, m_Size.y );
   m_UI->drawShape( WS_Rect, rect, m_Radius, 0x60FFFFFF, TexRef(), 1, 0x80CCCCCC );

   int cx = rect.getCenterX();
   int cy = rect.getCenterY();
   driver->draw2DText5x8( cx,cy, m_Text, m_TextColor, de::Align::Centered );

}

// =====================================================================
ComboBox::ComboBox( UI* manager, Widget* parent, int id )
// =====================================================================
   : Widget( manager, parent, id )
   , m_IsEnabled( true )
   , m_TextAlign( Align::TopLeft )
   , m_TextColor( 0xFFFFFFFF )
   , m_TextShadowColor( 0xFF000000 )
   , m_Text("Current Item Text")
   , m_FillColor( 0xC0CCCCCC )
   , m_FillShape( WS_Rect )
   , m_FillTex()
   , m_BorderWidth(1)
   , m_BorderColor( 0xFFDDDDDD )
   , m_BorderColorLight( lightenColor(m_BorderColor,50) )
   , m_BorderColorDark( darkenColor(m_BorderColor,50) )
{
}

//ComboBox::~ComboBox()
//{}

void ComboBox::render()
{


}

// =====================================================================
Slider::Slider( UI* manager, Widget* parent, int id )
// =====================================================================
   : Widget( manager, parent, id )
   //, m_Text("Slider")
{
   //m_Text = "Slider";
}

//Slider::~Slider()
//{

//}

void
Slider::render()
{
//   de::gpu::IVideoDriver* driver = m_UI->getDriver();
//   if ( !driver ) {
//      DE_ERROR("No driver")
//      return;
//   }

//   glm::ivec2 origin(0,0);
//   if ( m_Parent )
//   {
//      origin = m_Parent->getAbsolutePos();
//   }
//   // Layout
//   //int w = driver->getScreenWidth();
//   //int h = driver->getScreenHeight();
//   //elem.pos = de::Align::apply( elem.start_pos, elem.align, w, h );

//   Recti pos( origin.x + m_Pos.x, origin.y + m_Pos.y, m_Size.x, m_Size.y );

//   //DE_DEBUG("Render UI type(",int(m_Type),") Rect(",pos,")")

//   m_UI->drawShape( m_Shape, pos, m_Radius, m_FillColor, m_TexRef, m_BorderWidth, m_BorderColor );

//   int cx = pos.getCenterX();
//   int cy = pos.getCenterY();
//   driver->draw2DText5x8( cx,cy, m_Text, m_TextColor, de::Align::Centered );
}

// =====================================================================
LogBox::LogBox( UI* manager, Widget* parent, int id )
// =====================================================================
   : Widget( manager, parent, id )
   //, m_Text("LogBox")
{
   //m_Text = "LogBox";
}

//LogBox::~LogBox()
//{
//}

void
LogBox::render()
{
//   de::gpu::IVideoDriver* driver = m_UI->getDriver();
//   if ( !driver ) {
//      DE_ERROR("No driver")
//      return;
//   }

//   glm::ivec2 origin(0,0);
//   if ( m_Parent )
//   {
//      origin = m_Parent->getAbsolutePos();
//   }
//   // Layout
//   //int w = driver->getScreenWidth();
//   //int h = driver->getScreenHeight();
//   //elem.pos = de::Align::apply( elem.start_pos, elem.align, w, h );

//   Recti pos( origin.x + m_Pos.x, origin.y + m_Pos.y, m_Size.x, m_Size.y );

//   //DE_DEBUG("Render UI type(",int(m_Type),") Rect(",pos,")")

//   m_UI->drawShape( m_Shape, pos, m_Radius, m_FillColor, m_TexRef, m_BorderWidth, m_BorderColor );

//   int cx = pos.getCenterX();
//   int cy = pos.getCenterY();
//   driver->draw2DText5x8( cx,cy, m_Text, m_TextColor, de::Align::Centered );

}


} // end namespace gpu.
} // end namespace de.

