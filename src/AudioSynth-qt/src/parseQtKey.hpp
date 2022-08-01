#ifndef DE_PARSE_QTKEY_TO_SEVENT_KEY_HPP
#define DE_PARSE_QTKEY_TO_SEVENT_KEY_HPP

#include <QKeyEvent>
#include <DarkGPU.hpp>

inline void 
parseQtKey( QKeyEvent* event, de::SEvent & post )
{
   switch ( event->key() )
   {
      case Qt::Key_0: post.keyEvent.setKey( de::KEY_0 ); break;
      case Qt::Key_1: post.keyEvent.setKey( de::KEY_1 ); break;
      case Qt::Key_2: post.keyEvent.setKey( de::KEY_2 ); break;
      case Qt::Key_3: post.keyEvent.setKey( de::KEY_3 ); break;
      case Qt::Key_4: post.keyEvent.setKey( de::KEY_4 ); break;
      case Qt::Key_5: post.keyEvent.setKey( de::KEY_5 ); break;
      case Qt::Key_6: post.keyEvent.setKey( de::KEY_6 ); break;
      case Qt::Key_7: post.keyEvent.setKey( de::KEY_7 ); break;
      case Qt::Key_8: post.keyEvent.setKey( de::KEY_8 ); break;
      case Qt::Key_9: post.keyEvent.setKey( de::KEY_9 ); break;

      case Qt::Key_A: post.keyEvent.setKey( de::KEY_A ); break;
      case Qt::Key_B: post.keyEvent.setKey( de::KEY_B ); break;
      case Qt::Key_C: post.keyEvent.setKey( de::KEY_C ); break;
      case Qt::Key_D: post.keyEvent.setKey( de::KEY_D ); break;
      case Qt::Key_E: post.keyEvent.setKey( de::KEY_E ); break;
      case Qt::Key_F: post.keyEvent.setKey( de::KEY_F ); break;
      case Qt::Key_G: post.keyEvent.setKey( de::KEY_G ); break;
      case Qt::Key_H: post.keyEvent.setKey( de::KEY_H ); break;
      case Qt::Key_I: post.keyEvent.setKey( de::KEY_I ); break;
      case Qt::Key_J: post.keyEvent.setKey( de::KEY_J ); break;
      case Qt::Key_K: post.keyEvent.setKey( de::KEY_K ); break;
      case Qt::Key_L: post.keyEvent.setKey( de::KEY_L ); break;
      case Qt::Key_M: post.keyEvent.setKey( de::KEY_M ); break;
      case Qt::Key_N: post.keyEvent.setKey( de::KEY_N ); break;
      case Qt::Key_O: post.keyEvent.setKey( de::KEY_O ); break;
      case Qt::Key_P: post.keyEvent.setKey( de::KEY_P ); break;
      case Qt::Key_Q: post.keyEvent.setKey( de::KEY_Q ); break;
      case Qt::Key_R: post.keyEvent.setKey( de::KEY_R ); break;
      case Qt::Key_S: post.keyEvent.setKey( de::KEY_S ); break;
      case Qt::Key_T: post.keyEvent.setKey( de::KEY_T ); break;
      case Qt::Key_U: post.keyEvent.setKey( de::KEY_U ); break;
      case Qt::Key_V: post.keyEvent.setKey( de::KEY_V ); break;
      case Qt::Key_W: post.keyEvent.setKey( de::KEY_W ); break;
      case Qt::Key_X: post.keyEvent.setKey( de::KEY_X ); break;
      case Qt::Key_Y: post.keyEvent.setKey( de::KEY_Y ); break;
      case Qt::Key_Z: post.keyEvent.setKey( de::KEY_Z ); break;

      case Qt::Key_Escape: post.keyEvent.setKey( de::KEY_ESCAPE ); break;
      case Qt::Key_F1: post.keyEvent.setKey( de::KEY_F1 ); break;
      case Qt::Key_F2: post.keyEvent.setKey( de::KEY_F2 ); break;
      case Qt::Key_F3: post.keyEvent.setKey( de::KEY_F3 ); break;
      case Qt::Key_F4: post.keyEvent.setKey( de::KEY_F4 ); break;
      case Qt::Key_F5: post.keyEvent.setKey( de::KEY_F5 ); break;
      case Qt::Key_F6: post.keyEvent.setKey( de::KEY_F6 ); break;
      case Qt::Key_F7: post.keyEvent.setKey( de::KEY_F7 ); break;
      case Qt::Key_F8: post.keyEvent.setKey( de::KEY_F8 ); break;
      case Qt::Key_F9: post.keyEvent.setKey( de::KEY_F9 ); break;
      case Qt::Key_F10: post.keyEvent.setKey( de::KEY_F10 ); break;
      case Qt::Key_F11: post.keyEvent.setKey( de::KEY_F11 ); break;
      case Qt::Key_F12: post.keyEvent.setKey( de::KEY_F12 ); break;

      case Qt::Key_Enter: post.keyEvent.setKey( de::KEY_ENTER ); break;
      case Qt::Key_Backspace: post.keyEvent.setKey( de::KEY_BACKSPACE ); break;
      case Qt::Key_Backslash: post.keyEvent.setKey( de::KEY_BACKSLASH ); break;
      case Qt::Key_Shift: post.keyEvent.setKey( de::KEY_SHIFT ); break;
      case Qt::Key_Control: post.keyEvent.setKey( de::KEY_CTRL ); break;

      case Qt::Key_Up: post.keyEvent.setKey( de::KEY_UP ); break;
      case Qt::Key_Down: post.keyEvent.setKey( de::KEY_DOWN ); break;
      case Qt::Key_Left: post.keyEvent.setKey( de::KEY_LEFT ); break;
      case Qt::Key_Right: post.keyEvent.setKey( de::KEY_RIGHT ); break;
      default: break;
   }
}

#endif
