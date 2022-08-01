#ifndef DE_GUI_ENVIRONMENT_HPP
#define DE_GUI_ENVIRONMENT_HPP

#include <de/gpu/ui/Widget.hpp>
#include <de/cpu/ColorPicking.hpp>
#include <deque>
#include <memory>
#include <thread>

namespace de {
namespace gpu {

// =====================================================================
struct UI
// =====================================================================
{
   DE_CREATE_LOGGER("de.gpu.ui.UI")

   UI();
   ~UI();
   void setDriver( IVideoDriver* driver );
   IVideoDriver* getDriver();

   void render();
   void update();
   void handleEvent( SEvent event );
   void postEvent( SEvent event );
   void dump();

   void clearWidgets();
   //void registerWidget( Widget* w );
   //void unregisterWidget( Widget* w );
   //void addTopLevelWidget( Widget* w );

   void
   addTopLevelWidget( Widget* w )
   {
      if ( !w ) { return; } // Nothing todo

      registerWidget( w );

      auto it = std::find_if( m_TopLevel.begin(), m_TopLevel.end(),
         [&]( Widget const * const cached ) { return cached == w; } );

      if ( it == m_TopLevel.end() )
      {
         //DE_DEBUG("AddTopLevelWidget ",w)
         m_TopLevel.push_back( w );
         //DE_DEBUG("m_TopLevel.size() = ", m_TopLevel.size())
      }
   }

   void
   registerWidget( Widget* w )
   {
      if ( !w ) { return; }

//      auto it = std::find_if( m_Widgets.begin(), m_Widgets.end(),
//         [&]( Widget::SharedPtr const & cached ) { return cached.get() == w.get(); } );

//      if ( it == m_Widgets.end() )
//      {
//         m_Widgets.push_back( w );
//         DE_DEBUG("RegisterWidget(",w,"), widgetCount = ",m_Widgets.size())
//      }
   }

   void
   unregisterWidget( Widget* w )
   {
//      if ( !w ) { return; }

//      auto it = std::find_if( m_Widgets.begin(), m_Widgets.end(),
//            [&]( Widget const * const cached ) { return cached == w; } );

//      if ( it != m_Widgets.end() )
//      {
//         int64_t index = std::distance( m_Widgets.begin(), it );
//         if ( index < 0 || index >= m_Widgets.size() )
//         {
//            DE_ERROR("invalid index(",index,") of size(",m_Widgets.size(),")")
//         }
//         else
//         {
//            SAFE_DELETE( m_Widgets[ index ] );
//         }
//         m_Widgets.erase( it );
//      }
   }

   //void grabWidget( Widget* w );
   //void dropWidget( Widget* w );

   Widget* getWidget( int id );
   int32_t findWidget( Widget* w ) const;

   void drawShape( Widget::EShape shape, Recti const & pos, glm::ivec2 const & radius,
      uint32_t fillColor = 0xA0808080, TexRef const & tex = TexRef(),
      int borderWidth = 1, uint32_t borderColor = 0xDF00AFFF );

protected:
   gpu::IVideoDriver* m_Driver;
   Skin m_Skin;

   std::deque< SEvent > m_EventFifo;

   // Manages a list of unique widget pointers to ensure single deletion.
   std::vector< Widget* > m_Widgets;

   // TopLevelRoot panels on 'desktop' ( inside window screen )
   std::vector< Widget* > m_TopLevel; // Non owning simple pointers refs to m_Widgets.

   int m_MouseX = 0;
   int m_MouseY = 0;

   cpu::ColorPicking m_ColorPicking; // used to determine widget on top graphically.

// Register changes in UI, changed by mousemove, mouseclick or Tab
// Current:
   int m_TabIndex = -1; // Index into m_Widgets or -1
   int m_HoverIndex = -1; // Index into m_Widgets or -1
   int m_FocusIndex = -1; // Index into m_Widgets or -1
   Widget* m_HoverWidget = nullptr; // Where the mouse is at
   Widget* m_FocusWidget = nullptr; // Where the mouse last clicked on that can have focus ( e.g. EditBox )
// Last: Now we can act on a delta...
   int m_LastTabIndex = -1; // Index into m_Widgets or -1
   int m_LastHoverIndex = -1; // Index into m_Widgets or -1
   int m_LastFocusIndex = -1; // Index into m_Widgets or -1
   Widget* m_LastHoverWidget = nullptr; // Where the mouse is at
   Widget* m_LastFocusWidget = nullptr; // Where the mouse last clicked on that can have focus ( e.g. EditBox )

};

} // end namespace gpu.
} // end namespace de.

#endif

