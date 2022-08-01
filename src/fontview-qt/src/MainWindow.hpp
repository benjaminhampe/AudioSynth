#ifndef DE_QMAINWINDOW_FONTVIEWER_HPP
#define DE_QMAINWINDOW_FONTVIEWER_HPP

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "FontBar.hpp"
#include "GlyphDetailView.hpp"

class MainWindow : public QMainWindow
{
   Q_OBJECT
public:
   explicit MainWindow( QWidget* parent = nullptr );
   ~MainWindow() override;
protected:
   FontBar* m_FontToolBar;
   GlyphDetailView* m_glyphView;

};

#endif
