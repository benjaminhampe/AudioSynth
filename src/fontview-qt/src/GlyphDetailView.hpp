#ifndef DE_FontGlyphViewer_HPP
#define DE_FontGlyphViewer_HPP

#include <QWidget>
#include <QString>
#include <QPushButton>
#include <QLineEdit>
#include <QTreeWidget>
#include <QFont>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <vector>
// #include <FontAwesome.hpp>

class GlyphDetailView : public QWidget
{
   Q_OBJECT
public:
   explicit GlyphDetailView( QWidget* parent = nullptr );
   ~GlyphDetailView() override;

public slots:
   void setViewFont( QFont const & font );
   void setViewIndex( int glyphIndexStart );

protected:
   void updateGlyphs();

   uint32_t m_glyphIndexStart = 0;
   std::vector< QPushButton* > m_Buttons;

};

#endif // FontGlyphViewer_HPP
