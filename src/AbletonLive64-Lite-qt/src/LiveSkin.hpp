/// (c) 2017 - 20180 Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef DE_LIVE_SKIN_HPP
#define DE_LIVE_SKIN_HPP

#include "LiveCommon.hpp"

struct LiveSkin
{
   enum eInt
   {
      Padding = 0,
      Spacing,
      Radius,
      EngineButtonH,
      CircleButtonSize,
      ScrollButtonSize,
      SmallCircleButtonSize,
      Margin,
      TitleH,
      SearchBarH,
      TdH,
      eIntCount
   };

   enum eColor
   {
      WindowColor,
      PanelColor,
      SymbolColor,
      FocusColor,
      ActiveColor,
      TitleColor,
      TitleTextColor,
      ContentColor,
      EditBoxColor,
      TdColor,
      // BlackEditColor,
      // WhiteEditColor,
      // PenColor,
      // FillColor,
      // SemiBeatColor,
      // TextColor,
      HeaderBtnPenColor,
      HeaderBtnFillColor,
      HeaderBtnTextColor,
      eColorCount
   };

   int padding = 8;
   int spacing = 5;
   int radius = 8;
   int engineButtonH = 17;
   int circleButtonSize = 23;
   int scrollButtonSize = 9;
   int smallCircleButtonSize = 11;
   int margin = 4;
   int titleH = 12;
   int searchBarH = 24;
   int tdH = 12;
   float zoomUI = 1.0f;

   QColor windowColor = QColor(0,0,0,0);  // transparent
   QColor symbolColor = QColor(24,30,35);  // dark blue
   QColor panelColor = QColor(103,116,140);  // semi dark blue
   QColor focusColor = QColor(127,137,147); // darker gray
   QColor activeColor = QColor( 238, 246, 13 ); // yellow highlight
   QColor titleColor = QColor(217,221,226);   // light white blue
   QColor titleTextColor = windowColor;  // same as Window
   QColor contentColor = QColor(170,178,183); // greyish
   QColor editBoxColor = QColor(232,232,232);  // almost white
   QColor tdColor = QColor(140,153,175);  // table header

   // HeaderButtons
   QColor headerBtnPenColor = QColor( 166, 181, 186 ); // Engine Button
   QColor headerBtnFillColor = QColor( 146, 157, 168 );
   QColor headerBtnTextColor = QColor( 29, 31, 33 );

   // ClipEditor
   QColor blackEditColor = QColor(217,221,226);
   QColor whiteEditColor = QColor(196,200,204);

   // QColor semiBeatColor = QColor(146,156,167);  // light grey between main beat grid

   // QColor playColor = QColor(79,254,29);
   // QColor recColor = QColor(254,49,34);
   QColor panelBlendColor = blendRGB( windowColor, panelColor, 0.25f );

   void update();

   QColor getColor( eColor color );
   int getInt( eInt param ) const;



};

#endif
