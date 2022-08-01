#ifndef DE_LIVE_MIDI_METER_HPP
#define DE_LIVE_MIDI_METER_HPP

#include <QWidget>
#include <QImage>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QThread>

#include "LiveSkin.hpp"
#include <de/audio/dsp/IDspChainElement.hpp>

//#include <BottomLeftPanel.hpp>
//#include <MidiMeter.hpp>
//#include <ClipEditor.hpp>

struct App;

// ============================================================================
class MidiMeter : public QWidget
// ============================================================================
{
   Q_OBJECT
   //DE_CREATE_LOGGER("MidiMeter")
   App & m_app;
   bool m_hasFocus;
   //   ClipEditor* m_clipEditor;
public:
   MidiMeter( App & app, QWidget* parent = 0 );
   ~MidiMeter() override {}
   bool hasFocus() const { return m_hasFocus; }
signals:
public slots:
protected:
   void resizeEvent( QResizeEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;
   void enterEvent( QEvent* event ) override;
   void leaveEvent( QEvent* event ) override;
   void focusInEvent( QFocusEvent* event ) override;
   void focusOutEvent( QFocusEvent* event ) override;
private:
};

#endif // DE_Q_IMAGEWIDGET_HPP
