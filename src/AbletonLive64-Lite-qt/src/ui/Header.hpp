﻿#ifndef DE_LIVE_HEADER_HPP
#define DE_LIVE_HEADER_HPP

#include <ImageButton.hpp>

struct App;

// Top button list: = EngineButtons, now called Header because of fix layout pos.
// Manages layout and draws Engine buttons
// Buttons are created and added by App constructor.
// ============================================================================
class Header : public QWidget
// ============================================================================
{
   Q_OBJECT
   DE_CREATE_LOGGER("Header")
   App & m_app;
   int m_spacing = 2;

   ImageButton* m_TapButton;
   ImageButton* m_BpmEdit;
   ImageButton* m_NudgeSlowButton;
   ImageButton* m_NudgeFastButton;
   ImageButton* m_BeatSignatureEdit;
   ImageButton* m_MetronomToggle;
   ImageButton* m_DisplayFollowToggle;
   ImageButton* m_ArrangmentPosEdit;
   ImageButton* m_PlayButton;
   ImageButton* m_StopButton;
   ImageButton* m_RecordButton;

   ImageButton* m_OverdubButton;
   ImageButton* m_BackToArrangementButton;
   ImageButton* m_BarComboBox;
   ImageButton* m_PenButton;

   ImageButton* m_LoopStartEdit;
   ImageButton* m_PunchInButton;
   ImageButton* m_LoopButton;
   ImageButton* m_PunchOutButton;
   ImageButton* m_LoopLengthEdit;

   ImageButton* m_KeyMidiButton;
   ImageButton* m_KeyMappingButton;
   ImageButton* m_KeyMidiInButton;
   ImageButton* m_KeyMidiOutButton;
   ImageButton* m_MidiMappingButton;
   ImageButton* m_CpuUsageEdit;
   ImageButton* m_DiskUsageEdit;
   ImageButton* m_LiveMidiInLED;
   ImageButton* m_LiveMidiOutLED;

   std::vector< ImageButton* > m_btnGroup1;
   std::vector< ImageButton* > m_btnGroup2;
   std::vector< ImageButton* > m_btnGroup3;
   std::vector< ImageButton* > m_btnGroup4;

public:
   Header( App & app, QWidget* parent = 0 );
   ~Header() override {}

signals:
public slots:
   void updateLayout();
protected slots:
   void on_playButton( bool checked );
   void on_stopButton( bool checked );
   void on_recordButton( bool checked );
protected:
   void updateLayout( QRect const & clipRect );
   void resizeEvent( QResizeEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;


   int getGroup1Width() const
   {
      int w = 0;
      for ( auto & p : m_btnGroup1 ) { w += p->rect().width() + m_spacing; }
      return w;
   }

   int getGroup2Width() const
   {
      int w = 0;
      for ( auto & p : m_btnGroup2 ) { w += p->rect().width() + m_spacing; }
      return w;
   }

   int getGroup3Width() const
   {
      int w = 0;
      for ( auto & p : m_btnGroup3 ) { w += p->rect().width() + m_spacing; }
      return w;
   }

   int getGroup4Width() const
   {
      int w = 0;
      for ( auto & p : m_btnGroup4 ) { w += p->rect().width() + m_spacing; }
      return w;
   }

   int getTotalWidth() const
   {
      return getGroup1Width() + getGroup2Width()
           + getGroup3Width() + getGroup4Width();
   }


private:
   ImageButton* createTapButton();
   ImageButton* createBpmEdit();
   ImageButton* createNudgeSlowButton();
   ImageButton* createNudgeFastButton();
   ImageButton* createBeatSignatureEdit();
   ImageButton* createMetronomToggle();
   ImageButton* createDisplayFollowToggle();
   ImageButton* createArrangmentPosEdit();
   ImageButton* createPlayButton();
   ImageButton* createStopButton();
   ImageButton* createRecordButton();

   ImageButton* createOverdubButton();
   ImageButton* createBackToArrangementButton();
   ImageButton* createBarComboBox();
   ImageButton* createPenButton();

   ImageButton* createLoopStartEdit();
   ImageButton* createPunchInButton();
   ImageButton* createLoopButton();
   ImageButton* createPunchOutButton();
   ImageButton* createLoopLengthEdit();

   ImageButton* createKeyMidiButton();
   ImageButton* createKeyMappingButton();
   ImageButton* createKeyMidiInButton();
   ImageButton* createKeyMidiOutButton();
   ImageButton* createMidiMappingButton();
   ImageButton* createCpuUsageEdit();
   ImageButton* createDiskUsageEdit();
   ImageButton* createLiveMidiInLED();
   ImageButton* createLiveMidiOutLED();
};

#endif // G_LOPASS1_HPP
