#ifndef DE_QWIDGET_FONTCHOOSEBAR_HPP
#define DE_QWIDGET_FONTCHOOSEBAR_HPP

#include <QWidget>
#include <QPushButton>
#include <QToolBar>
#include <QToolButton>
#include <QComboBox>
#include <QFontDatabase>
#include <QFontDialog>
#include <QDebug>

#include <QTextEdit>
#include <QFontDatabase>

#include "ColorButton.hpp"

#include <de_fontawesome.hpp>
#include <sstream>
#include <iostream>

inline QFont getFontAwesome( int size = 10 )
{
   QFont font("FontAwesome", size, QFont::Normal, false );
   font.setHintingPreference( QFont::PreferFullHinting );
   font.setKerning( true );
   font.setStyleStrategy( QFont::PreferAntialias );
   return font;
}

inline void addFontAwesome463()
{
   int fontAwesome = QFontDatabase::addApplicationFont( "fontawesome463.ttf" );
   //int fontAwesome = QFontDatabase::addApplicationFont( "la-regular-400.ttf" );
   auto names = QFontDatabase::applicationFontFamilies( fontAwesome );
   for ( int i = 0; i < names.size(); ++i )
   {
      std::wcout << "Font[" << i << "] = " << names.at( i ).toStdWString() << std::endl;
   }
}

class FontBar : public QToolBar
{
   Q_OBJECT
public:
   FontBar( QWidget * parent = nullptr );
   ~FontBar() override;

signals:
   void fontChanged( QFont const & font );
   void glyphIndexChanged( int );

public slots:
   void on_glyphIndexChanged(int);

   void onButtonClick_Font( bool );
   void onComboIndex_Family( int );
   void onComboIndex_Size( int );
   void onButtonClick_Bold( bool );
   void onButtonClick_Italic( bool );
   void onButtonClick_Underline( bool );
   void onButtonClick_StrikeThrough( bool );

public:
   void updateFont();

   QPushButton* m_btnAddFont;
   QComboBox* m_glyphIndex;
   // current state
   QString m_CurrFamily;
   QColor m_CurrLineColor;
   QColor m_CurrFillColor;
   int m_CurrPointSize;
   int m_CurrWeight;
   bool m_IsBold;
   bool m_IsItalic;
   bool m_IsStriked;
   bool m_IsUnderlined;
   int m_CurrTextAlign;
   QFont m_CurrFont;

   // ui
   QPushButton* m_FontInfo;
   QComboBox* m_FontFamily;
   QComboBox* m_FontSize;
   ColorButton* m_LineColor;
   ColorButton* m_FillColor;

   QToolButton* m_Bold;
   QToolButton* m_Italic;
   QToolButton* m_Underline;
   QToolButton* m_StrikeThrough;
};

#endif
