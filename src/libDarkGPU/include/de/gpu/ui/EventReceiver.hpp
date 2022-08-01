//////////////////////////////////////////////////////////////////////////////
/// @author Benjamin Hampe <benjamin.hampe@gmx.de>
//////////////////////////////////////////////////////////////////////////////

#ifndef DE_WINDOW_EVENTRECEIVER_HPP
#define DE_WINDOW_EVENTRECEIVER_HPP

#include <cstdint>
#include <sstream>
#include <vector>
#include <array>

#include <cwchar>
#include <codecvt>
#include <cstring> // strlen()
#include <cmath>
#include <codecvt> // convert wstring ( unicode ) <-> string ( multibyte utf8 )
#include <algorithm>
#include <iomanip> // string_converter
#include <memory>

namespace de {

   //inline bool
   //dbIsMouseOver( int mx, int my, int x1, int y1, int x2, int y2 )
   //{
   //   if ( x1 > x2 ) std::swap( x1, x2 );
   //   if ( y1 > y2 ) std::swap( y1, y2 );
   //   if ( mx < x1 ) return false;
   //   if ( my < y1 ) return false;
   //   if ( mx > x2 ) return false;
   //   if ( my > y2 ) return false;
   //   return true;
   //}

   // ========================================================================
   //! Enumeration for all event types there are.
   // ========================================================================
   enum EEventType
   {
      //! An event of the (underlying) native window.
      EET_WINDOW_RESIZE_EVENT = 0,

      //! An event of the graphical user interface.
      EET_GUI_EVENT,

      //! A mouse input event.
      EET_MOUSE_EVENT,

      //! A key input event.
      EET_KEY_EVENT,

      //! A joystick (joypad, gamepad) input event.
      EET_JOYSTICK_EVENT,

      //! A log event
      EET_LOG_EVENT,

      //! A user event with user data.
      EET_USER_EVENT,

      //! This enum is never used, it only forces the compiler to
      //! compile these enumeration values to 32 bit.
      EET_FORCE_32_BIT = 0x7fffffff
   };

   // Any kind of user event.
   struct SUserEvent
   {
      // Some user specified data as int
      int32_t UserData1;

      // Another user specified data as int
      int32_t UserData2;
   };

   namespace gpu {
   namespace ui {
      struct Widget;
   } // end namespace ui
   } // end namespace gpu

   // Enumeration for all events which are sendable by the gui system
   enum EGUI_EVENT_TYPE
   {
      // A gui element has lost its focus.
      /* GUIEvent.Caller is losing the focus to GUIEvent.Element.
      If the event is absorbed then the focus will not be changed. */
      EGET_ELEMENT_FOCUS_LOST = 0,
      // A gui element has got the focus.
      /* If the event is absorbed then the focus will not be changed. */
      EGET_ELEMENT_FOCUSED,
      // The mouse cursor hovered over a gui element.
      /* If an element has sub-elements you also get this message for the subelements */
      EGET_ELEMENT_HOVERED,
      // The mouse cursor left the hovered element.
      /* If an element has sub-elements you also get this message for the subelements */
      EGET_ELEMENT_LEFT,
      // An element would like to close.
      /* Windows and context menus use this event when they would like to close,
      this can be cancelled by absorbing the event. */
      EGET_ELEMENT_CLOSED,
      // A button was clicked.
      EGET_BUTTON_CLICKED,
      // A scrollbar has changed its position.
      EGET_SCROLL_BAR_CHANGED,
      // A checkbox has changed its check state.
      EGET_CHECKBOX_CHANGED,
      // A new item in a listbox was selected.
      /* NOTE: You also get this event currently when the same item was clicked again after more than 500 ms. */
      EGET_LISTBOX_CHANGED,
      // An item in the listbox was selected, which was already selected.
      /* NOTE: You get the event currently only if the item was clicked again within 500 ms or selected by "enter" or "space". */
      EGET_LISTBOX_SELECTED_AGAIN,
      // A file has been selected in the file dialog
      EGET_FILE_SELECTED,
      // A directory has been selected in the file dialog
      EGET_DIRECTORY_SELECTED,
      // A file open dialog has been closed without choosing a file
      EGET_FILE_CHOOSE_DIALOG_CANCELLED,
      // 'Yes' was clicked on a messagebox
      EGET_MESSAGEBOX_YES,
      // 'No' was clicked on a messagebox
      EGET_MESSAGEBOX_NO,
      // 'OK' was clicked on a messagebox
      EGET_MESSAGEBOX_OK,
      // 'Cancel' was clicked on a messagebox
      EGET_MESSAGEBOX_CANCEL,
      // In an editbox 'ENTER' was pressed
      EGET_EDITBOX_ENTER,
      // The text in an editbox was changed. This does not include automatic changes in text-breaking.
      EGET_EDITBOX_CHANGED,
      // The marked area in an editbox was changed.
      EGET_EDITBOX_MARKING_CHANGED,
      // The tab was changed in an tab control
      EGET_TAB_CHANGED,
      // A menu item was selected in a (context) menu
      EGET_MENU_ITEM_SELECTED,
      // The selection in a combo box has been changed
      EGET_COMBO_BOX_CHANGED,
      // The value of a spin box has changed
      EGET_SPINBOX_CHANGED,
      // A table has changed
      EGET_TABLE_CHANGED,
      EGET_TABLE_HEADER_CHANGED,
      EGET_TABLE_SELECTED_AGAIN,
      // A tree view node lost selection. See IGUITreeView::getLastEventNode().
      EGET_TREEVIEW_NODE_DESELECT,
      // A tree view node was selected. See IGUITreeView::getLastEventNode().
      EGET_TREEVIEW_NODE_SELECT,
      // A tree view node was expanded. See IGUITreeView::getLastEventNode().
      EGET_TREEVIEW_NODE_EXPAND,
      // A tree view node was collapsed. See IGUITreeView::getLastEventNode().
      EGET_TREEVIEW_NODE_COLLAPSE,
      // deprecated - use EGET_TREEVIEW_NODE_COLLAPSE instead. This
      // may be removed by Irrlicht 1.9
      EGET_TREEVIEW_NODE_COLLAPS = EGET_TREEVIEW_NODE_COLLAPSE,
      // No real event. Just for convenience to get number of events
      EGET_COUNT
   };

   // Any kind of GUI event.
   struct SGuiEvent
   {
      // Widget who created the event
      gpu::ui::Widget* Caller;

      // If the event has something to do with another element, it will be held here.
      gpu::ui::Widget* Element;

      // Type of GUI Event
      EGUI_EVENT_TYPE EventType;

   };

   //! Any kind of log event.
   struct SLogEvent
   {
      //! Pointer to text which has been logged
      //const char* Text;

      //! Log level in which the text has been logged
      //ELOG_LEVEL Level;
   };

   //! Any kind of user event.
   struct SWindowResizeEvent
   {
      //! Some user specified data as int
      int32_t w;

      //! Another user specified data as int
      int32_t h;

      uint64_t nativeHandle;

      SWindowResizeEvent() : w( 0 ), h( 0 ), nativeHandle( 0 )
      {}
   };

   // ========================================================================
   /// @brief EKEY
   // ========================================================================
   enum EKEY
   {
      KEY_UNKNOWN          = 0,

      KEY_TAB,
      //KEY_CLEAR,
      KEY_ENTER,
      KEY_SHIFT,
      KEY_CAPS_LOCK,
      KEY_CTRL,
      KEY_ALT,
      KEY_ALTGR,
      KEY_WIN,
      KEY_APPS,
      KEY_SPACE,
      KEY_BACKSPACE,
      KEY_BACKSLASH,

      // Function Keys
      KEY_ESCAPE,
      KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6,
      KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,
      // KEY_F13, KEY_F14, KEY_F15, KEY_F16, KEY_F17, KEY_F18,
      // KEY_F19, KEY_F20, KEY_F21, KEY_F22, KEY_F23, KEY_F24,


      // ControlBlock 3x1
      KEY_PRINT,
      KEY_SCROLL,  // SCROLL LOCK key
      KEY_PAUSE,
      KEY_SLEEP,

      // ControlBlock 3x2
      KEY_INSERT,
      KEY_HOME,  // POS1
      KEY_PAGE_UP,
      KEY_DELETE,
      KEY_END,
      KEY_PAGE_DOWN,

      // Numpad
      KEY_NP_LOCK, KEY_NP_DIV, KEY_NP_MUL, KEY_NP_SUB,
      KEY_NP_7, KEY_NP_8, KEY_NP_9, KEY_NP_ADD,
      KEY_NP_4, KEY_NP_5, KEY_NP_6,
      KEY_NP_1, KEY_NP_2, KEY_NP_3,
      KEY_NP_0, KEY_NP_COMMA,KEY_NP_ENTER,


      KEY_PLUS,
      KEY_COMMA,
      KEY_MINUS,
      KEY_PERIOD,
      KEY_COLON,


//      KEY_KANA             = 0x15,  // IME Kana mode
//      KEY_HANGUEL          = 0x15,  // IME Hanguel mode (maintained for compatibility use HANGUL)
//      KEY_HANGUL           = 0x15,  // IME Hangul mode
//      KEY_JUNJA            = 0x17,  // IME Junja mode
//      KEY_FINAL            = 0x18,  // IME final mode
//      KEY_HANJA            = 0x19,  // IME Hanja mode
//      KEY_KANJI            = 0x19,  // IME Kanji mode

//      KEY_CONVERT          = 0x1C,  // IME convert
//      KEY_NONCONVERT       = 0x1D,  // IME nonconvert
//      KEY_ACCEPT           = 0x1E,  // IME accept
//      KEY_MODECHANGE       = 0x1F,  // IME mode change request

      // Arrow keys
      KEY_LEFT, KEY_UP, KEY_RIGHT, KEY_DOWN,

      KEY_0, KEY_1, KEY_2, KEY_3, KEY_4,
      KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,

      KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H,
      KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P,
      KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X,
      KEY_Y, KEY_Z,

      KEY_SELECT,
      KEY_EXECUT,
      KEY_SNAPSHOT,
      KEY_HELP,

      KEY_OEM_1,  // for US    ";:"
      KEY_OEM_2,  // for US    "/?"
      KEY_OEM_3,  // for US    "`~"
      KEY_OEM_4,  // for US    "[{"
      KEY_OEM_5,  // for US    "\|"
      KEY_OEM_6,  // for US    "]}"
      KEY_OEM_7,  // for US    "'""
      KEY_OEM_8,  // None
      KEY_OEM_AX,  // for Japan "AX"
      KEY_OEM_102,  // "<>" or "\|"
      EKEY_COUNT // this is not a key, but the amount of keycodes there are.
   };

   // ========================================================================
   /// @brief StringConverter
   // ========================================================================
   struct StringConverter
   {
      // GET string FROM wstring
      static std::string
      toString( std::wstring const & txt )
      {
         std::wstring_convert< std::codecvt_utf8< wchar_t > > converter;
         return converter.to_bytes( txt );
      }

      // GET string FROM wchar.
      static std::string
      toString( wchar_t const wc )
      {
         std::wstringstream w; w << wc;
         std::wstring_convert< std::codecvt_utf8< wchar_t > > converter;
         return converter.to_bytes( w.str() );
      }
   };

   // ========================================================================
   /// @brief KeyEvent
   // ========================================================================
   struct SKeyEvent
   {
      // GLFW: int key, int scancode, int action, int mods
      enum EModifier
      {
         None = 0,
         Pressed = 0x01,
         Shift = 0x02,
         Ctrl = 0x04,
         Alt = 0x08,
         AltGr = 0x10,
         Super = 0x20,
         Fn = 0x40,
         EModifierCount = 7
      };

      uint32_t Unicode; // For unknown enum value, corresponds to the key (0, if not a character)
      uint32_t Key;     // Platform independent enum key code
      uint32_t Flags;
      //wchar_t Char; // For unknown enum value, corresponds to the key (0, if not a character)

      SKeyEvent()
         : Unicode( 0 ), Key( EKEY_COUNT ), Flags( None )
      {}

      void setUnicode( uint32_t unicode )
      {
         Unicode = unicode;
      }
      void setKey( EKEY key )
      {
         Key = key;
      }
      void setModifer( uint32_t modifier )
      {
         Flags = modifier & 0xFF;
      }

//      uint32_t unicode() const { return Unicode; }
//      EKEY key() const { return Key; }
//      uint32_t modifers() const { return Flags; }
      bool pressed() const { return Flags & Pressed; }
      bool released() const { return !pressed(); }
      bool shift() const { return Flags & Shift; }
      bool ctrl() const { return Flags & Ctrl; }
      bool alt() const { return Flags & Alt; }
      bool altgr() const { return Flags & AltGr; }
      bool win() const { return Flags & Super; }
      bool fn() const { return Flags & Fn; }

      std::string
      toString() const
      {
         std::ostringstream s;
         s << "Key(" << int(Key) << "), Flags(" << Flags << "), Unicode(" << Unicode << ")";
         return s.str();
      }
   };

   // ========================================================================
   /// @brief MouseEvent
   // ========================================================================
   struct SMouseEvent
   {
      //! Masks for mouse button states
      enum EFlags : uint32_t
      {
         None       = 0,
         BtnLeft    = 1,
         BtnRight   = 1 << 1,
         BtnMiddle  = 1 << 2,
         BtnExtra1  = 1 << 3,   // A mouse with 4 buttons the developer never had.
         BtnExtra2  = 1 << 4,   // A mouse with 4 buttons the developer never had.
         BtnExtra3  = 1 << 5,   // A mouse with 4 buttons the developer never had.
         BtnExtra4  = 1 << 6,   // A mouse with 4 buttons the developer never had.
         BtnExtra5  = 1 << 7,   // A mouse with 4 buttons the developer never had.
         BtnExtra6  = 1 << 8,   // A mouse with 4 buttons the developer never had.
         BtnExtra7  = 1 << 9,   // A mouse with 4 buttons the developer never had.
         BtnExtra8  = 1 << 10,   // A mouse with 4 buttons the developer never had.
         BtnExtra9  = 1 << 11,   // A mouse with 5+ buttons the developer never had.
         BtnExtra10 = 1 << 12,   // A mouse with 5+ buttons the developer never had.
         BtnExtra11 = 1 << 13,   // A mouse with 5+ buttons the developer never had.
         BtnExtra12 = 1 << 14,   // A mouse with 5+ buttons the developer never had.
         BtnExtra13 = 1 << 15,   // A mouse with 5+ buttons the developer never had.
         Pressed = 1 << 16,
         Released= 1 << 17,
         Moved   = 1 << 18,
         Wheel   = 1 << 19,
         DoubleClick= 1 << 20,
         TripleClick= 1 << 21,
         WithShift  = 1 << 22, // True if SHIFT was also pressed
         WithCtrl   = 1 << 23, // True if CTRL was also pressed
         WithAlt    = 1 << 24,  // True if ALT was also pressed
         WithAltGr  = 1 << 25,// True if Alt-Gr was also pressed
         WithSuper  = 1 << 26,// True if WinKey/Super1Key was also pressed
         FORCE_32_BIT = 0x7fffffff
      };

      uint32_t m_Flags;
      int32_t m_X;
      int32_t m_Y;
      float m_Wheel;        //! mouse wheel delta, often 1.0 or -1.0, but can have other values < 0.f or > 0.f;

      bool isLeftPressed() const { return m_Flags & ( BtnLeft | Pressed ); }
      bool isRightPressed() const { return m_Flags & ( BtnRight | Pressed ); }
      bool isMiddlePressed() const { return m_Flags & ( BtnMiddle | Pressed ); }

      bool isPressed() const { return m_Flags & Pressed; }
      bool isLeft() const { return m_Flags & BtnLeft; }
      bool isRight() const { return m_Flags & BtnRight; }
      bool isMiddle() const { return m_Flags & BtnMiddle; }

//      bool isLeftPressed() const { return m_Flags & BtnLeft; }
//      bool isRightPressed() const { return m_Flags & BtnRight; }
//      bool isMiddlePressed() const { return m_Flags & BtnMiddle; }

      int x() const { return m_X; }
      int y() const { return m_Y; }
      float z() const { return m_Wheel; }
      float wheel() const { return m_Wheel; }

      int getButton() const
      {
         int btn = 0;
         btn += ( isLeft() ) ? 1 : 0;
         btn += ( isRight() ) ? 2 : 0;
         btn += ( isMiddle() ) ? 3 : 0;
         return btn;
      }

      SMouseEvent()
         : m_Flags( 0 )
         , m_X(0)
         , m_Y(0)
         , m_Wheel( 0.0f )
      {}

      std::string toString() const
      {
         std::stringstream s;
         s << "flags(" << m_Flags << "), "
            "button(" << getButton() << "), "
            "pos(" << m_X << "," << m_Y << "), "
            "wheel(" << m_Wheel << "), "
            "shift(" << (m_Flags & WithShift) << "), "
            "ctrl(" << (m_Flags & WithCtrl) << ")";
         return s.str();
      }
   };

   // ========================================================================
   /// @brief MouseMultiClicks
   // ========================================================================
   struct SMouseMultiClicks
   {
      SMouseMultiClicks()
         : DoubleClickTime(500)
         , CountSuccessiveClicks(0)
         , LastClickTime(0)
         , LastClickX(0)
         , LastClickY(0)
         , LastFlags(0)
      {}

      uint32_t DoubleClickTime;
      uint32_t CountSuccessiveClicks;
      uint32_t LastClickTime;
      int32_t LastClickX;
      int32_t LastClickY;
      uint32_t LastFlags;
   };

   // ========================================================================
   /// @brief A joystick event.
   // ========================================================================
   /** Unlike other events, joystick events represent the result of polling
   * each connected joystick once per run() of the device. Joystick events will
   * not be generated by default.  If joystick support is available for the
   * active device, _IRR_COMPILE_WITH_JOYSTICK_EVENTS_ is defined, and
   * @ref irr::IrrlichtDevice::activateJoysticks() has been called, an event of
   * this type will be generated once per joystick per @ref IrrlichtDevice::run()
   * regardless of whether the state of the joystick has actually changed. */
   struct SJoystickEvent
   {
      enum
      {
         NUMBER_OF_BUTTONS = 32,
         AXIS_X = 0, // e.g. analog stick 1 left to right
         AXIS_Y,		// e.g. analog stick 1 top to bottom
         AXIS_Z,		// e.g. throttle, or analog 2 stick 2 left to right
         AXIS_R,		// e.g. rudder, or analog 2 stick 2 top to bottom
         AXIS_U,
         AXIS_V,
         NUMBER_OF_AXES
      };

      /** A bitmap of button states.  You can use IsButtonPressed() to
      ( check the state of each button from 0 to (NUMBER_OF_BUTTONS - 1) */
      uint32_t ButtonStates;

      /** For AXIS_X, AXIS_Y, AXIS_Z, AXIS_R, AXIS_U and AXIS_V
      * Values are in the range -32768 to 32767, with 0 representing
      * the center position.  You will receive the raw value from the
      * joystick, and so will usually want to implement a dead zone around
      * the center of the range. Axes not supported by this joystick will
      * always have a value of 0. On Linux, POV hats are represented as axes,
      * usually the last two active axis.
      */
      int16_t Axis[NUMBER_OF_AXES];

      /** The POV represents the angle of the POV hat in degrees * 100,
      * from 0 to 35,900.  A value of 65535 indicates that the POV hat
      * is centered (or not present).
      * This value is only supported on Windows.  On Linux, the POV hat
      * will be sent as 2 axes instead. */
      uint16_t POV;

      //! The ID of the joystick which generated this event.
      /** This is an internal Irrlicht index; it does not map directly
      * to any particular hardware joystick. */
      uint8_t Id;

      //! A helper function to check if a button is pressed.
      bool IsButtonPressed(uint32_t button) const
      {
         if(button >= (uint32_t)NUMBER_OF_BUTTONS)
         return false;

         return (ButtonStates & (1 << button)) ? true : false;
      }
   };

   // ========================================================================
   /// @brief JoystickInfo
   // ========================================================================
   //! Information on a joystick, returned from @ref irr::IrrlichtDevice::activateJoysticks()
   struct SJoystickInfo
   {
      //! The ID of the joystick
      /** This is an internal Irrlicht index; it does not map directly
      * to any particular hardware joystick. It corresponds to the
      * irr::SJoystickEvent Joystick ID. */
      uint8_t				Id;

      //! The name that the joystick uses to identify itself.
      std::string	Name;

      //! The number of buttons that the joystick has.
      uint32_t				Buttons;

      //! The number of axes that the joystick has, i.e. X, Y, Z, R, U, V.
      /** Note: with a Linux device, the POV hat (if any) will use two axes. These
      *  will be included in this count. */
      uint32_t				Axes;

      //! An indication of whether the joystick has a POV hat.
      /** A Windows device will identify the presence or absence or the POV hat.  A
      *  Linux device cannot, and will always return POV_HAT_UNKNOWN. */
      enum
      {
      //! A hat is definitely present.
      POV_HAT_PRESENT,

      //! A hat is definitely not present.
      POV_HAT_ABSENT,

      //! The presence or absence of a hat cannot be determined.
      POV_HAT_UNKNOWN
      } PovHat;
   }; // struct SJoystickInfo

   // ========================================================================
   /// @brief SEvent
   // ========================================================================
   struct SEvent
   {
      EEventType type;
      union
      {
         struct SWindowResizeEvent resizeEvent;
         struct SGuiEvent guiEvent;
         struct SMouseEvent mouseEvent;
         struct SKeyEvent keyEvent;
         struct SJoystickEvent joystickEvent;
         struct SLogEvent logEvent;
         struct SUserEvent userEvent;
      };

      SEvent() : type( EET_USER_EVENT ), userEvent() {}
      SEvent( SKeyEvent event ) : type( EET_KEY_EVENT ), keyEvent( event ) {}
      SEvent( SMouseEvent event ) : type( EET_MOUSE_EVENT ), mouseEvent( event ) {}
      SEvent( SUserEvent event ) : type( EET_USER_EVENT ), userEvent( event ) {}
      SEvent( SWindowResizeEvent event ) : type( EET_WINDOW_RESIZE_EVENT ), resizeEvent( event ) {}
      SEvent( SJoystickEvent event ) : type( EET_JOYSTICK_EVENT ), joystickEvent( event ) {}
   };

   // ========================================================================
   /// @brief IEventReceiver
   // ========================================================================
   class IEventReceiver
   {
   public:
      virtual ~IEventReceiver() {}
      virtual void onEvent( SEvent e ) = 0;
      virtual bool isEnabled() const { return true; }
      virtual void setEnabled( bool enable ) {}
   };

} // end namespace de.


#endif // DARKENGINE_IWINDOW_HPP

