#ifndef DE_G_GROUP_MENU_V_HPP
#define DE_G_GROUP_MENU_V_HPP

#include "QImageWidget.hpp"
#include <QPushButton>
#include <QFont5x8.hpp>
// ============================================================================
class GGroupV : public QWidget
// ============================================================================
{
   Q_OBJECT
public:
   GGroupV( QString name, QWidget* parent = 0 );
   ~GGroupV();
   bool isMore() const { return m_btnMore->isChecked(); }
   bool isEnabled() const { return m_btnEnabled->isChecked(); }
   bool isBypassed() const { return !m_btnEnabled->isChecked(); }
   bool isVisible() const { return m_btnVisible->isChecked(); }
   bool isHidden() const { return !m_btnVisible->isChecked(); }
signals:
   void toggledBypass(bool);
   void toggledMore(bool);
   void toggledHideAll(bool);
public slots:
   void setVisibleButton(bool visible) { m_btnVisible->setChecked( visible ); }
   void setMoreButtonVisible(bool visible) { m_btnMore->setVisible( visible ); }
   void setBypassButton(bool bypassed) { m_btnEnabled->setChecked( !bypassed ); }
protected slots:
   void on_buttonToggled_enabled(bool checked);
   void on_buttonToggled_more(bool checked);
   void on_buttonToggled_visible(bool checked);
public:
   QPushButton* m_btnEnabled;
   QImageWidget* m_title;
   QPushButton* m_btnMore;
   QPushButton* m_btnVisible;
   QFont5x8 m_font;
};

#endif // DE_Q_IMAGEWIDGET_HPP
