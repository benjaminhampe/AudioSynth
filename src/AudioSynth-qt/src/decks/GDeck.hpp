#ifndef DE_G_GROUP_MENU_H_HPP
#define DE_G_GROUP_MENU_H_HPP

#include "QImageWidget.hpp"
#include <QPushButton>
#include <QFont5x8.hpp>

#include <de/ColorGradient.hpp>
#include <de/audio/dsp/IDspChainElement.hpp>

// ============================================================================
class GLevelMeterH : public QWidget
// ============================================================================
{
   Q_OBJECT
   DE_CREATE_LOGGER("GLevelMeterH")
public:
   GLevelMeterH( QWidget* parent = 0 )
      : QWidget(parent)
      , m_value( 0.0f )
      , m_bgColor( 1,1,1 )
   {
      setObjectName( "GLevelMeterH" );
      setContentsMargins( 0,0,0,0 );
      setMinimumSize( 64, 5 );
   }
   ~GLevelMeterH()
   {}


signals:
public slots:
   void setValue( float t )
   {
      m_value = t;
   }

   void setImage( QImage const & img )
   {
      m_img = img;
      setMinimumSize(m_img.size());
      setMaximumSize(m_img.size());
   }

protected:
   void paintEvent( QPaintEvent* event ) override
   {
      int w = width();
      int h = height();

      QPainter dc(this);
      dc.setRenderHint( QPainter::NonCosmeticDefaultPen );

      // Draw Image
      dc.drawImage( 0,0, m_img );

      // Draw Cover Rect in Background color
      int px = int( m_value * w );
      int dx = w - px;

      //dc.setPen( QPen( QColor( 255,255,255,255 ) ) );
      dc.setPen( Qt::NoPen );
      dc.setBrush( QBrush( m_bgColor ) );
      dc.drawRect( QRect(px,0,dx,h) );
      //QWidget::paintEvent( event );
   }


private:
   float m_value;
   QColor m_bgColor;
   QImage m_img;
};

// ============================================================================
class GDeck : public QWidget, public de::audio::IDspChainElement
// ============================================================================
{
   Q_OBJECT
public:
   GDeck( QString name, QWidget* parent = 0 );
   ~GDeck();

   uint64_t readSamples( double pts, float* dst, uint32_t dstFrames,
               uint32_t dstChannels, uint32_t dstRate ) override;

   bool isMore() const { return m_more->isChecked(); }
   bool isEnabled() const { return m_btnEnabled->isChecked(); }
   bool isBypassed() const { return !m_btnEnabled->isChecked(); }
   bool isVisible() const { return m_btnVisible->isChecked(); }
   bool isHidden() const { return !m_btnVisible->isChecked(); }
signals:
   void toggledBypass(bool);
   void toggledMore(bool);
   void toggledHideAll(bool);
public slots:
   void startUpdateTimer();
   void stopUpdateTimer();
   void clearInputSignals();
   void setInputSignal( int i, de::audio::IDspChainElement* input );
   void setBackgroundColor( QColor bgColor ) { m_deckColor = bgColor; update(); }
   void setVisibleButton(bool visible) { m_btnVisible->setChecked( visible ); }
   void setBypassButton(bool bypassed) { m_btnEnabled->setChecked( !bypassed ); }
   void setTitleImageText( QString title );
protected slots:
   void on_buttonToggled_enabled(bool checked);
   void on_buttonToggled_more(bool checked);
   void on_buttonToggled_visible(bool checked);
protected:
   void timerEvent( QTimerEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;
public:
   DE_CREATE_LOGGER("GDeck")
   QColor m_deckColor;
   QPushButton* m_btnVisible;
   QPushButton* m_btnEnabled;
   QPushButton* m_more;
   QImageWidget* m_title;
   GLevelMeterH* m_levelL;
   GLevelMeterH* m_levelR;

   de::audio::IDspChainElement* m_inputSignal;
   //std::vector< float > m_inputBuffer;
   float m_Lmin;
   float m_Lmax;
   float m_Rmin;
   float m_Rmax;

   int m_updateTimerId;

//   std::vector< float > m_accumBuffer;
   QFont5x8 m_font5x8;
   de::LinearColorGradient m_ColorGradient;

};

#endif // DE_Q_IMAGEWIDGET_HPP
