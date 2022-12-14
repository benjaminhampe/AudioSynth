#ifndef DE_LIVE_IMAGEBUTTON_HPP
#define DE_LIVE_IMAGEBUTTON_HPP

#include "LiveSkin.hpp"

// ============================================================================
class ImageButton : public QPushButton
// ============================================================================
{
   Q_OBJECT
   bool m_isHovered;
   QImage m_imgIdle;
   QImage m_imgIdleHover;
   QImage m_imgActive;
   QImage m_imgActiveHover;

public:
   ImageButton( QWidget* parent = 0 )
      : QPushButton( "", parent )
      , m_isHovered( false )
   {}

   ~ImageButton() override
   {}
   bool isHovered() const { return m_isHovered; }
   int x() const { return rect().x(); }
   int y() const { return rect().y(); }
   int w() const { return rect().width(); }
   int h() const { return rect().height(); }

signals:
public slots:
   void setRect( QRect const & rect )
   {
      move( rect.topLeft() );
      setMinimumSize( rect.size() );
      setMaximumSize( rect.size() );
   }

   void setImage( int i, QImage const & img )
   {
      setMinimumSize( img.size() );
      setMaximumSize( img.size() );

      switch( i )
      {
         case 0: m_imgIdle = img; break;
         case 1: m_imgIdleHover = img; break;
         case 2: m_imgActive = img; break;
         case 3: m_imgActiveHover = img; break;
         default: break;
      }
   }
protected:
   void resizeEvent( QResizeEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;
   void enterEvent( QEvent* event ) override;
   void leaveEvent( QEvent* event ) override;

};

#endif // G_LOPASS1_HPP
