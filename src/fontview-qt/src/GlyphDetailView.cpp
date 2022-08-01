#include "GlyphDetailView.hpp"

namespace {

void
computeMinSize( QPushButton* btn )
{
   if (!btn) return;
   QSize s = btn->fontMetrics().tightBoundingRect( btn->text() ).size() + QSize(6,6);
   btn->setMinimumSize( QSize( std::min( s.width(), 128 ), std::min( s.height(), 48 ) ) );
}

}

GlyphDetailView::GlyphDetailView( QWidget* parent )
   : QWidget( parent )
{
   setAcceptDrops( true );
   setContentsMargins( 5, 5, 5, 5 );

   QGridLayout* grid = new QGridLayout;
   grid->setContentsMargins( 0,0,0,0 );
   grid->setSpacing( 4 );
   for ( int y = 0; y < 8; ++y )
   {
      for ( int x = 0; x < 16; ++x )
      {
         uint16_t unicode = y * 16 + x;

         auto btn = new QPushButton( QChar( unicode ), parent );
         btn->setToolTip( QString("Unicode char: ") + QString::number( unicode, 16 ) );
         btn->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
         computeMinSize( btn );
         grid->addWidget( btn, y, x, 1, 1 );
         m_Buttons.emplace_back( btn );
      }
   }

   setLayout( grid );
}

GlyphDetailView::~GlyphDetailView()
{

}

void
GlyphDetailView::updateGlyphs()
{
   for ( size_t i = 0; i < m_Buttons.size(); ++i )
   {
      uint16_t unicode = m_glyphIndexStart + i;
      QString toolhlp = QString("GlyphStart(%1), i(%2), unicode(%3), char(%4)")
                     .arg( m_glyphIndexStart )
                     .arg( i )
                     .arg( QString::number( unicode, 16 ) )
                     .arg( char( unicode ) );

      QPushButton* btn = m_Buttons[ i ];
      btn->setText( QChar( unicode ) );
      btn->setToolTip( toolhlp );
      computeMinSize( btn );
   }
}

void
GlyphDetailView::setViewFont( QFont const & font )
{
   //m_glyphIndexStart = 0;
   setFont( font );
   updateGlyphs();
}

void
GlyphDetailView::setViewIndex( int glyphIndexStart )
{
   m_glyphIndexStart = glyphIndexStart;
   updateGlyphs();
}
