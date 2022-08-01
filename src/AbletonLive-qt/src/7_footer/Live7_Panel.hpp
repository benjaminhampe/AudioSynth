/// (c) 2017 - 20180 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef DE_LIVE7_PANEL_HPP
#define DE_LIVE7_PANEL_HPP

#include <App.hpp>
#include <Live0_Panel.hpp>
#include <Button.hpp>

class LongTextDisplay : public QWidget
{
   Q_OBJECT
   App & m_app;
   QString m_longText;
public:
   LongTextDisplay( App & app, QWidget* parent = nullptr );
   ~LongTextDisplay() override{}

public slots:
   void setLongText( QString longText ) { m_longText = longText; update(); }
protected:
   //void updateImage();
   //void resizeEvent( QResizeEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;

};


class MidiDisplay : public QWidget
{
   Q_OBJECT
   App & m_app;
public:
   MidiDisplay( App & app, QWidget* parent = nullptr );
   ~MidiDisplay() override{}

public slots:
protected:
   //void updateImage();
   //void resizeEvent( QResizeEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;

};

class AudioDisplay : public QWidget
{
   Q_OBJECT
   App & m_app;
public:
   AudioDisplay( App & app, QWidget* parent = nullptr );
   ~AudioDisplay() override{}

public slots:
protected:
   //void updateImage();
   //void resizeEvent( QResizeEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;

};

class Live7_Panel : public QWidget
{
   Q_OBJECT
   App & m_app;
public: // Connected by Body
   Button* m_showHelpButton;
   LongTextDisplay* m_longText;
   MidiDisplay* m_midi;
   AudioDisplay* m_audio;
   Live0_Panel* m_longTextPanel;
   Live0_Panel* m_midiPanel;
   Live0_Panel* m_audioPanel;
   Button* m_showPanelButton;
public:
   Live7_Panel( App & app, QWidget* parent = nullptr );
   ~Live7_Panel() override{}
signals:
   void showHelpPanel( bool checked );
   void showFooterPanel( bool checked );

protected:
   //void updateImage();
   //void resizeEvent( QResizeEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;

};

#endif
