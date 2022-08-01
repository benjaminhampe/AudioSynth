#ifndef DE_GPIANOKEYBOARD_QWIDGET_HPP
#define DE_GPIANOKEYBOARD_QWIDGET_HPP

#include <QWidget>
#include <QImage>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QKeyEvent>

#include <de/Logger.hpp>
#include <de/audio/dsp/IDspChainElement.hpp>

#include "ImageButton.hpp"

struct App;

// =======================================================================
struct PitchWheel : public QWidget
// =======================================================================
{
   Q_OBJECT
   DE_CREATE_LOGGER("PitchWheel")
   App & m_app;
   int m_sliderValue;
   float m_controlValue;
public:
   PitchWheel( App & app, QWidget* parent );
   ~PitchWheel() override;
signals:
public slots:
protected:
   void paintEvent( QPaintEvent* event ) override;
   void resizeEvent( QResizeEvent* event ) override;

   void mousePressEvent( QMouseEvent* event ) override;
   void mouseReleaseEvent( QMouseEvent* event ) override;
   void mouseMoveEvent( QMouseEvent* event ) override;
   void wheelEvent( QWheelEvent* event ) override;

//   void keyPressEvent( QKeyEvent* event ) override;
//   void keyReleaseEvent( QKeyEvent* event ) override;
};


// =======================================================================
struct KeyboardArea : public QWidget
// =======================================================================
{
   Q_OBJECT
   DE_CREATE_LOGGER("KeyboardArea")
   App & m_app;

   bool m_isRepeatingPressEvents;

   struct Key
   {
      bool pressed;
      int midiNote;
      int oktave;
      int semitone;
      Key()
         : pressed( false ), midiNote( 0 ), oktave( 0 ), semitone( 0 ) {}
      Key( int iNote, int iOktave, int iSemi )
         : pressed( false ), midiNote( iNote ), oktave( iOktave ), semitone( iSemi )    {}
   };

   int m_timerId;
   int m_keyStart;
   int m_keyWidth;
   int m_whiteKeyCount;
   int m_blackKeyCount;
   double m_timeLastNote;
   double m_timeNote;

   std::vector< Key > m_keys;
   std::array< Key*, 10 > m_touched;
   std::array< de::audio::IDspChainElement*, 8 > m_synths;
   std::array< QImage, 12 > m_images;

public:
   KeyboardArea( App & app, QWidget* parent );
   ~KeyboardArea() override;
   int32_t findKey( int midiNote ) const;

   int keyIndexFromMouse( int mx, int my ) const;

signals:
   //void noteEvent( de::audio::Note const & note );
public slots:
   void setKeyRange( int midiNoteStart, int midiNoteCount );

   void noteOn( int midiNote, int velocity = 90 );
   void noteOff( int midiNote );

   //void sendNote( de::audio::Note const & note );
   //void emit_noteEvent( int channel, int midiNote, int velocity );
   //void setSynth( int i, de::audio::IDspChainElement* synth );
protected:
   void timerEvent( QTimerEvent* event) override;
   void paintEvent( QPaintEvent* event ) override;
   void resizeEvent( QResizeEvent* event ) override;
   void mousePressEvent( QMouseEvent* event ) override;
   void mouseReleaseEvent( QMouseEvent* event ) override;
   void mouseMoveEvent( QMouseEvent* event ) override;
   void wheelEvent( QWheelEvent* event ) override;

   void enterEvent( QEvent* event ) override;
   void leaveEvent( QEvent* event ) override;
//   void focusInEvent( QFocusEvent* event ) override;
//   void focusOutEvent( QFocusEvent* event ) override;
};

// =======================================================================
struct MidiMasterKeyboard : public QWidget
// =======================================================================
{
   Q_OBJECT
   DE_CREATE_LOGGER("MidiMasterKeyboard")
   App & m_app;
   PitchWheel* m_pitchWheel;
   PitchWheel* m_modWheel;
   KeyboardArea* m_keyboardArea;

   ImageButton* m_btnLeave;
   ImageButton* m_btnRepeat;

public:
   MidiMasterKeyboard( App & app, QWidget* parent );
   ~MidiMasterKeyboard() override;
signals:
public slots:
protected:
//   void timerEvent( QTimerEvent* event) override;
//   void paintEvent( QPaintEvent* event ) override;
//   void resizeEvent( QResizeEvent* event ) override;

   void keyPressEvent( QKeyEvent* event ) override;
   void keyReleaseEvent( QKeyEvent* event ) override;

   ImageButton* createLeaveButton();
   ImageButton* createRepeatButton();
};

#endif
