#ifndef DE_LIVE1_AUDIOENGINE_HPP
#define DE_LIVE1_AUDIOENGINE_HPP

#include <App.hpp>
#include <Button.hpp>

// ============================================================================
class Live1_Panel : public QWidget
// ============================================================================
{
   Q_OBJECT
public:
   Live1_Panel( App & app, QWidget* parent = 0 );
   ~Live1_Panel();
signals:
public slots:
   void play();
   void stop();
protected slots:
   void on_playButton( bool checked );
   void on_stopButton( bool checked );
protected:
   DE_CREATE_LOGGER("Live1_Panel")
   App & m_app;
   Button* m_tapButton;
   Button* m_bpmEdit;
   Button* m_nudgeSlow;
   Button* m_nudgeFast;
   Button* m_beatSig;
   Button* m_metronom;

   Button* m_displayFollow;
   Button* m_edtArrangementPos;
   Button* m_playButton;
   Button* m_stopButton;
   Button* m_recButton;
   Button* m_overdubButton;
   Button* m_backToArrangement;
   Button* m_barCombo;
   Button* m_penButton;

   Button* m_edtLoopStart;
   Button* m_togPunchIn;
   Button* m_togLooping;
   Button* m_togPunchOut;
   Button* m_edtLoopLength;

   Button* m_togKeyMidi;
   Button* m_ledKeyMidiIn;
   Button* m_ledKeyMidiOut;
   Button* m_togKeyMapping;
   Button* m_togMidiMapping;
   Button* m_barCpuUsage;
   Button* m_ledHddOverload; // HardDiskOverload indicator
   Button* m_ledLiveMidiIn;
   Button* m_ledLiveMidiOut;


   // GPianoKeyboard* m_Piano2D;
};

#endif // G_LOPASS1_HPP
