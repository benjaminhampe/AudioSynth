#ifndef DE_GUI_WIDGET_HPP
#define DE_GUI_WIDGET_HPP

#include <de/gpu/smesh/SMesh.hpp>
#include <de/gpu/Font5x8Renderer.hpp>
#include <de/gpu/ui/EventReceiver.hpp>

namespace de {
namespace gpu {

struct Widget;
struct UI;

//   , m_Type( WT_Panel )
//   , m_Shape( WS_Rect )
//   , m_TextColor( 0xFFFFFFFF )
//   , m_FillColor( 0x40000000 )
//   , m_BorderWidth( 3 )
//   , m_BorderColor( 0xFFAAAAAA )
//   , m_BorderColorLight( 0xFFCCCCCC )
//   , m_BorderColorDark( 0xFF686868 )

// =====================================================================
struct Skin
// =====================================================================
{
   int MinSizeX = 24;
   int MinSizeY = 24;
   Font5x8 TextFont;
   uint32_t WindowColor = 0x80CCCCCC;
   uint32_t FillColor = 0x40CCCCCC;

   uint32_t IdleBorderColor = 0x70FFFFFF;
   uint32_t IdleBorderColorLight = 0x70FFFFFF;
   uint32_t IdleBorderColorDark = 0x70FFFFFF;

   uint32_t HoverBorderColor = 0x70008FFF;
   uint32_t HoverBorderColorLight = lightenColor( HoverBorderColor, 50 );
   uint32_t HoverBorderColorDark = darkenColor( HoverBorderColor, 50 );

   uint32_t FocusBorderColor = 0x70008FFF;
   uint32_t FocusBorderColorLight = lightenColor( FocusBorderColor, 50 );
   uint32_t FocusBorderColorDark = darkenColor( FocusBorderColor, 50 );

   uint32_t LabelTextColor = 0x70FFFFFF;
   uint32_t LabelTextBgColor = 0x70FFFFFF;

//   uint32_t BorderColor = 0x70CCCCCC;
//   uint32_t BorderColorLight = lightenColor( BorderColor, 50 );
//   uint32_t BorderColorDark = darkenColor( BorderColor, 50 );

//   uint32_t EditColor = 0x70FFFFFF;
//   uint32_t EditTextColor = 0x70FFFFFF;
////   EShape m_Shape;
////   Align::EAlign m_TextAlign;

//   int m_BorderWidth;
//   gpu::TexRef m_TexRef;
//   int m_PreserveAspect = 0;
   //int m_Tess = 13;
   //int m_Border = 3;
   //de::Align::EAlign m_Align;
//   std::string m_InfoText;
};

// =====================================================================
struct Widget
// =====================================================================
{
   typedef std::shared_ptr< Widget > SharedPtr;
   typedef std::weak_ptr< Widget > WeakPtr;

   DE_CREATE_LOGGER("Widget")

   enum EType
   {
      WT_Panel = 0,
      WT_Window,
      WT_Dialog,
      WT_Label,
      WT_Image,
      WT_Button,
      WT_EditBox,
      WT_CheckBox,
      WT_CheckBool,
      WT_ComboBox,
      WT_ComboSel,
      WT_ContextMenu,
      WT_Slider,
      WT_ScrollBar,
      WT_PrevNextBox,
      ETypeCount
   };

   enum EShape
   {
      WS_Rect = 0,
      WS_RoundRect,
      WS_Circle,
      EShapeCount
   };

   Widget( UI* manager, Widget* parent = nullptr, int id = -1 );
   virtual ~Widget();
   virtual EType getType() const { return WT_Panel; }
   virtual void render();

   glm::ivec2 getAbsolutePos() const;
   Recti getAbsoluteRect() const;
   bool isReallyVisible() const;

   glm::ivec2 const & getSize() const { return m_Size; }
   void setSize( int x, int y ) { m_Size = glm::ivec2(x,y); }

   glm::ivec2 const & getPos() const { return m_Pos; }
   void setPos( int x, int y ) { m_Pos = glm::ivec2(x,y); }

   glm::ivec2 const & getRadius() const { return m_Radius; }
   void setRadius( glm::ivec2 const & radius ) { m_Radius = radius; }

   bool isVisible() const { return m_IsVisible; }
   void setVisible( bool visible ) { m_IsVisible = visible; }

   int getId() const { return m_Id; }
   void setId( int id ) { m_Id = id; }

//   EShape getShape() const { return m_Shape; }
//   void setShape( EShape shape ) { m_Shape = shape; }
//   std::string getShapeStr() const
//   {
//      switch ( getShape() )
//      {
//         case WS_Rect: return "WS_Rect";
//         case WS_RoundRect: return "WS_RoundRect";
//         case WS_Circle: return "WS_Circle";
//         default: return "";
//      }
//   }
   void
   addChild( Widget* w )
   {
      if ( !w ) return;
      m_Children.emplace_back( w );
      m_Children.back()->m_Parent = this;
      //m_UI->registerWidget( w );
   }


// protected:
   UI* m_UI;
   Widget* m_Parent;

   int m_Id;
   int m_TabIndex;

   //int m_ZIndex;
   bool m_IsVisible;
   bool m_IsModal;

   //std::vector< Widget* > m_Children; // list of children

   glm::ivec2 m_Size;
   glm::ivec2 m_Pos;
   glm::ivec2 m_Radius;     // radius, only for RoundRect

   glm::ivec2 m_AbsolutePos;

   std::vector< Widget* > m_Children; // list of children
};


// =====================================================================
struct Panel : public Widget
// =====================================================================
{
   Panel( UI* manager, Widget* parent = nullptr, int id = -1 );
   //~Panel() override;
   EType getType() const override { return WT_Panel; }
   void render() override;
};

// =====================================================================
struct Label : public Widget
// =====================================================================
{
   Label( std::string txt, UI* manager, Widget* parent = nullptr, int id = -1 );
   //~Label();
   EType getType() const override { return WT_Label; }
   void render() override;

   void setText( std::string txt ) { m_Text = txt; }
   void setTextAlign( Align::EAlign align ) { m_TextAlign = align; }
   void setTextColor( uint32_t color ) { m_TextColor = color; }
   void setTextShadowColor( uint32_t color ) { m_TextShadowColor = color; }

   Font5x8 m_TextFont;
   uint32_t m_TextColor = 0xFFFFFFFF;
   uint32_t m_TextShadowColor = 0xFF000000;
   Align::EAlign m_TextAlign = Align::TopLeft;
   std::string m_Text;
//   EShape m_Shape;
//   int m_BorderWidth;
//   uint32_t m_BorderColor;
//   uint32_t m_BorderColorLight;
//   uint32_t m_BorderColorDark;
//   gpu::TexRef m_TexRef;
//   int m_PreserveAspect = 0;
   //int m_Tess = 13;
   //int m_Border = 3;
   //de::Align::EAlign m_Align;
//   std::string m_InfoText;

   //std::string m_ButtonText;
};

// =====================================================================
struct Button : public Widget
// =====================================================================
{
   Button( std::string txt, UI* manager, Widget* parent = nullptr, int id = -1 );
   //~Button() override;
   EType getType() const override { return WT_Button; }
   void render() override;

   void setText( std::string txt ) { m_Text = txt; }
   void setTextAlign( Align::EAlign align ) { m_TextAlign = align; }
   void setTextColor( uint32_t color ) { m_TextColor = color; }
   void setTextShadowColor( uint32_t color ) { m_TextShadowColor = color; }
   // void setFillColor( uint32_t color, int state = 0 ) { m_FillColor = color; }
   // void setBorderColor( uint32_t color, int state = 0 ) { m_BorderColorLight = m_BorderColorDark = m_BorderColor = color; }

   bool m_IsEnabled;
   bool m_IsChecked;
   bool m_IsCheckable;
   Align::EAlign m_TextAlign;
   uint32_t m_TextColor;
   uint32_t m_TextShadowColor;
   std::string m_Text;
   Font5x8 m_TextFont;

   uint32_t m_FillColor;
   EShape m_FillShape;
   TexRef m_FillTex;

   int m_BorderWidth;
   uint32_t m_BorderColor;
   uint32_t m_BorderColorLight;
   uint32_t m_BorderColorDark;
};

// =====================================================================
struct EditBox : public Widget
// =====================================================================
{
   EditBox( std::string txt, UI* manager, Widget* parent = nullptr, int id = -1 );
   //~EditBox() override;
   EType getType() const override { return WT_EditBox; }
   void render() override;

   void setText( std::string txt ) { m_Text = txt; }
   void setTextAlign( Align::EAlign align ) { m_TextAlign = align; }
   void setTextColor( uint32_t color ) { m_TextColor = color; }
   void setTextShadowColor( uint32_t color ) { m_TextShadowColor = color; }

   bool m_IsEnabled;
   Align::EAlign m_TextAlign;
   uint32_t m_TextColor;
   uint32_t m_TextShadowColor;
   std::string m_Text;
   Font5x8 m_TextFont;

   uint32_t m_FillColor;
   EShape m_FillShape;
   TexRef m_FillTex;

   int m_BorderWidth;
   uint32_t m_BorderColor;
   uint32_t m_BorderColorLight;
   uint32_t m_BorderColorDark;
};

// =====================================================================
struct CheckBox : public Widget
// =====================================================================
{
   CheckBox( std::string txt, UI* manager, Widget* parent = nullptr, int id = -1 );
   //~CheckBox() override;
   EType getType() const override { return WT_CheckBox; }
   void render() override;

   void setText( std::string txt ) { m_Text = txt; }
   void setEnabled( bool enabled ) { m_IsEnabled = enabled; }
   void setChecked( bool checked ) { m_IsChecked = checked; }
   void setCheckable( bool checkable ) { m_IsCheckable = checkable; }

   bool isEnabled() const { return m_IsEnabled; }
   bool isChecked() const { return m_IsChecked; }
   bool isCheckable() const { return m_IsCheckable; }

   bool m_IsEnabled;
   bool m_IsChecked;
   bool m_IsCheckable;
   Align::EAlign m_TextAlign;
   uint32_t m_TextColor;
   uint32_t m_TextShadowColor;
   std::string m_Text;
   Font5x8 m_TextFont;

   uint32_t m_FillColor;
   EShape m_FillShape;
   TexRef m_FillTex;

   int m_BorderWidth;
   uint32_t m_BorderColor;
   uint32_t m_BorderColorLight;
   uint32_t m_BorderColorDark;

};

// =====================================================================
struct ComboBox : public Widget
// =====================================================================
{
   ComboBox( UI* manager, Widget* parent = nullptr, int id = -1 );
   //~ComboBox() override;
   EType getType() const override { return WT_ComboBox; }
   void render() override;

   int m_SelectedItem;

   struct Item
   {
      std::string text;
      std::string userData;
      TexRef icon;
      int icon_w;
      int icon_h;
   };

   std::vector< Item > m_Items;

   void addItem( std::string txt, TexRef ref )
   {

   }


   void setText( std::string txt ) { m_Text = txt; }
   void setEnabled( bool enabled ) { m_IsEnabled = enabled; }
   bool isEnabled() const { return m_IsEnabled; }

   bool m_IsEnabled;

   Align::EAlign m_TextAlign;
   uint32_t m_TextColor;
   uint32_t m_TextShadowColor;
   std::string m_Text;
   Font5x8 m_TextFont;

   uint32_t m_FillColor;
   EShape m_FillShape;
   TexRef m_FillTex;

   int m_BorderWidth;
   uint32_t m_BorderColor;
   uint32_t m_BorderColorLight;
   uint32_t m_BorderColorDark;
};

// =====================================================================
struct ScrollBar : public Widget
// =====================================================================
{
   ScrollBar( UI* manager, Widget* parent = nullptr, int id = -1 );
   // ~ScrollBar() override;
   EType getType() const override { return WT_ScrollBar; }
   void render() override;
};

// =====================================================================
struct Slider : public Widget
// =====================================================================
{
   Slider( UI* manager, Widget* parent = nullptr, int id = -1 );
   // ~Slider() override;
   EType getType() const override { return WT_Slider; }
   void render() override;
};

// =====================================================================
struct LogBox : public Widget
// =====================================================================
{
   LogBox( UI* manager, Widget* parent = nullptr, int id = -1 );
   // ~LogBox() override;
   EType getType() const override { return WT_ContextMenu; }
   void render() override;
};

} // end namespace gpu.
} // end namespace de.

#endif

