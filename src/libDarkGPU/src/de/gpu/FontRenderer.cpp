#include <de/gpu/FontRenderer.hpp>

#ifdef USE_LIB_DARKFONT

namespace de {
namespace gpu {

#pragma pack( push )
#pragma pack( 1 )

// ===========================================================================
struct FontVertex
// ===========================================================================
{
   glm::vec3 pos;
   glm::vec2 tex;    // 12 + 12 + 4 + 8 = 36 Bytes if packed
   //uint32_t color;

   static FVF const & getFVF() {
      static FVF const vertexFormat( sizeof( FontVertex ),
         {  VertexAttrib( VertexAttribType::F32, 3, VertexAttribSemantic::Pos ),
            VertexAttrib( VertexAttribType::F32, 2, VertexAttribSemantic::Tex )
         // VertexAttrib( VertexAttribType::U8, 4, VertexAttribSemantic::Color )
         }
      );
      return vertexFormat;
   }

   FontVertex() : pos(), tex() //, color( 0xFFFFFFFF )
   {}

   FontVertex( float x, float y, float z, float u, float v ) : pos( x,y,z ), tex( u, v ) //, color( color )
   {}

   glm::vec3 const & getPos() const       { return pos; }
   glm::vec3 &       getPos()             { return pos; }
   glm::vec2 const & getTexCoord() const  { return tex; }
   //uint32_t const & getColor() const    { return color; }

   void setPos( glm::vec3 const & position ) { pos = position; }
   void setTexCoord( glm::vec2 const & texCoord ) { tex = texCoord; }
   //void setColor( uint32_t const & color ) { color = color; }

};

#pragma pack( pop )

// ===========================================================================
FontRenderer::FontRenderer( IVideoDriver* driver )
// ===========================================================================
   : m_Driver( driver )
   , m_Shader( nullptr )
{
//   addFontFamily( "../../media/fonts/arial.ttf", Font("arial", 32 ) );
//   addFontFamily( "../../media/fonts/fontawesome463.ttf", Font("awesome", 32 ) );
//   addFontFamily( "../../media/fonts/la-brands-400.ttf", Font("brands", 64 ) );
//   addFontFamily( "../../media/fonts/la-regular-400.ttf", Font("regular", 16 ) );
//   //addFontFamily( "../../media/fonts/la-brands-400.ttf", Font("brands", 64 ) );
}

FontRenderer::~FontRenderer()
{
   clear();
}

void
FontRenderer::clear()
{
}

TextSize
FontRenderer::getTextSize( std::wstring const & msg, Font const & font )
{
   return dbGetTextSize( msg, font );
}

void
FontRenderer::draw2DText( int x, int y, std::wstring const & msg, uint32_t color,
                            Align::EAlign align, Font const & font )
{
   if (msg.empty()) return;
   if (!m_Driver) return;
   int w = m_Driver->getScreenWidth();
   int h = m_Driver->getScreenHeight();
   if ( w < 2 || h < 2 ) return;

   std::shared_ptr< IFontAtlas > fontImpl = getFontAtlas( font );
   if ( !fontImpl ) { DE_ERROR("No font impl") return; }

   //Image dbg = fontImpl->createTextImage( msg, 0xFFFFFFFF, 0x00000000 );
   //dbSaveImage( dbg, dbStr(msg) + ".png" );

   std::vector< Glyph > glyphs;
   bool needUpload = fontImpl->cacheString( msg, glyphs );

   TextSize ts = fontImpl->getTextSize( msg );

   de::Image const * atlasImage = fontImpl->getAtlasImage();
   if ( !atlasImage ) { DE_ERROR("No atlas image") return; }

   std::string uri = atlasImage->getUri();
   //DE_DEBUG("Font: ", font.toString(), " has atlasURI(", uri, "), msg(",dbStr(msg),")")

   SO const so(
         SO::MagLinear,
         SO::MinLinear,
         SO::ClampToEdge,
         SO::ClampToEdge );

   TexRef ref = m_Driver->getTexture( uri );
   if ( !ref.tex )
   {
      ref = m_Driver->addTexture2D( uri, *atlasImage, false, so );
      fontImpl->setDirty( false );
      DE_DEBUG("Created font texture ", uri)
   }
   else
   {
      if ( fontImpl->isDirty() )
      {
         m_Driver->uploadTexture( ref.tex, *atlasImage );
         fontImpl->setDirty( false );
         DE_DEBUG("Uploaded font texture ", uri)
      }
   }

   if ( !ref.tex )
   {
      DE_ERROR("No atlas tex(", uri, ")")
      return;
   }

   int tex_w = ref.tex->getWidth();
   int tex_h = ref.tex->getHeight();

   // Align the text_size(w,h) to the text_pos(x,y) only.
   int32_t tw = ts.width;
   int32_t th = ts.height;
//   if ( font.lcdFit )
//   {
//      tw /= 3;
//   }

   // horizontal align to entire screen
   glm::ivec2 aligned_pos;
        if ( align & Align::Left )  { aligned_pos.x = x;       }
   else if ( align & Align::Center ){ aligned_pos.x = x - tw/2; }
   else if ( align & Align::Right ) { aligned_pos.x = x - tw;   }
   else { DE_ERROR("Invalid horizontal text align") return; }
   // vertical align to entire screen
        if ( align & Align::Top )   { aligned_pos.y = y;       }
   else if ( align & Align::Middle ){ aligned_pos.y = y - th/2; }
   else if ( align & Align::Bottom ){ aligned_pos.y = y - th;   }
   else { DE_ERROR("Invalid vertical text align") return; }

   int px = aligned_pos.x;
   int py = aligned_pos.y;

   SMeshBuffer quads( PrimitiveType::Triangles );

   // Loop over glyphs we got from cacheString() and
   // render them now...
   for ( size_t i = 0; i < glyphs.size(); ++i )
   {
      // current glyph to render
      Glyph const & glyph = glyphs[ i ];

      uint32_t unicode = glyph.unicode;

      if ( unicode == '\n' || unicode == '\r' )
      {
         px = aligned_pos.x;
         py += ts.lineHeight;
      }
      else
      {
         // Kerning
         glm::ivec2 kerning(0,0);
         if ( i > 0 )
         {
            Glyph const & prev = glyphs[ i-1 ];
            kerning = fontImpl->getKerning( prev.unicode, glyph.unicode );
         }

         // Draw
         if ( glyph.ref.getWidth() > 0 && glyph.ref.getHeight() > 0 )
         {
            int x1 = glyph.bmp.x + px + kerning.x; // + glyph.bmp.getX();
            int y1 = py + ts.lineHeight - ts.baseline - glyph.bmp.y; // + ts.baseline; // + glyph.ref.getY();

            float gw = float( glyph.ref.getWidth() );
            float gh = float( glyph.ref.getHeight() );
            if ( font.lcdFit ) gw /= 2;
            int x2 = x1 + gw-1;
            int y2 = y1 + gh-1;

            auto const & rect = glyph.ref.rect;
            float u1 = (float(rect.getX1())) / float( tex_w );
            float v1 = (float(rect.getY1())) / float( tex_h );
            float u2 = u1+(float(rect.getWidth())) / float( tex_w );
            float v2 = v1+(float(rect.getHeight())) / float( tex_h );

            S3DVertex A( x1, y2, -1.f, 0,0,-1, color, u1, v2 ); // color is set as uniform now
            S3DVertex B( x1, y1, -1.f, 0,0,-1, color, u1, v1 ); // not as vertex attrib anymore
            S3DVertex C( x2, y1, -1.f, 0,0,-1, color, u2, v1 );
            S3DVertex D( x2, y2, -1.f, 0,0,-1, color, u2, v2 );

            quads.addVertex( A );
            quads.addVertex( B );
            quads.addVertex( C );
            quads.addVertex( A );
            quads.addVertex( C );
            quads.addVertex( D );

            //DE_DEBUG( "Glyph[",i,"] bmp_rect(", glyph.bmp.toString(), ")" )
            //DE_DEBUG( "Glyph[",i,"] atlas_rect(", glyph.atlas.rect.toString(), ")" )
            //DE_DEBUG("Draw Glyph[",i,"] u1(",u1,"), v1(",v1,"), u2(",u2,"), v2(",v2,")" )
         }
      }
      if ( font.lcdFit )
      {
         px += glyph.advance / 2;
      }
      else
      {
         px += glyph.advance;
      }

      //if ( font.lcdFit )
      //{
         //px += glyph.advance / 3;
      //}

   }

   //m_Driver->setDebug( false );
#if 0
   SMeshBuffer lines = SMeshBufferTool::createWireframe( quads, 1.0f, 0xFFFFFFFF );
   m_Driver->draw2D( lines );
#endif

#if 0
   SMeshBuffer colorQuads = quads;
   SMeshBufferTool::colorVertices( colorQuads, 0x19888888);
   //colorQuads.setDepth( Depth::disabled() );
   colorQuads.setBlend( Blend::alphaBlend() );
   colorQuads.setTexture( 0, nullptr );
   m_Driver->draw2D( colorQuads );
#endif

   //quads.setDepth( Depth::disabled() );
   quads.setBlend( Blend::alphaBlend() );
   quads.setTexture( 0, ref );
   quads.Material.CloudTransparent = true;
   m_Driver->draw2D( quads );


   //draw2DTextDebug( x,y,msg,color,align,font);
}

void
FontRenderer::draw2DTextDebug( int x, int y, std::wstring const & msg, uint32_t color,
                            Align::EAlign align, Font const & font )
{
   if (msg.empty()) return;
   if (!m_Driver) return;
   int w = m_Driver->getScreenWidth();
   int h = m_Driver->getScreenHeight();
   if ( w < 2 || h < 2 ) return;

   //DE_DEBUG("FONT ", w," x ",h)

   //m_Driver->setDebug( true );

   std::shared_ptr< IFontAtlas > fontImpl = getFontAtlas( font );
   if ( !fontImpl ) { DE_ERROR("No font impl") return; }
   de::Image const * atlasImage = fontImpl->getAtlasImage();
   if ( !atlasImage ) { DE_ERROR("No atlas image") return; }

   std::vector< Glyph > glyphs;
   bool needUpload = fontImpl->cacheString( msg, glyphs );

   // Align the text_size(w,h) to the text_pos(x,y) only.
   TextSize ts = fontImpl->getTextSize( msg );
   int32_t tw = ts.width;
   int32_t th = ts.height;
   if ( font.lcdFit )
   {
      tw /= 3;
   }

   // horizontal align to entire screen
   glm::ivec2 aligned_pos;
        if ( align & Align::Left )  { aligned_pos.x = x;       }
   else if ( align & Align::Center ){ aligned_pos.x = x - tw/2; }
   else if ( align & Align::Right ) { aligned_pos.x = x - tw;   }
   else { DE_ERROR("Invalid horizontal text align") return; }
   // vertical align to entire screen
        if ( align & Align::Top )   { aligned_pos.y = y;       }
   else if ( align & Align::Middle ){ aligned_pos.y = y - th/2; }
   else if ( align & Align::Bottom ){ aligned_pos.y = y - th;   }
   else { DE_ERROR("Invalid vertical text align") return; }

   float px = aligned_pos.x;
   float py = aligned_pos.y + ts.baseline;
   float pz = -1.0f; // At -1.0 it disappears, not inside frustum.

   SMeshBuffer lines( PrimitiveType::Lines );

   // Loop over glyphs we got from cacheString() and
   // render them now...
   for ( size_t i = 0; i < glyphs.size(); ++i )
   {
      // current glyph to render
      Glyph const & glyph = glyphs[ i ];

      uint32_t unicode = glyph.unicode;

      if ( unicode == '\n' || unicode == '\r' )
      {
         px = aligned_pos.x;
         py += ts.lineHeight;
      }
      else
      {
         // Kerning
         glm::ivec2 kerning(0,0);
         if ( i > 0 )
         {
            Glyph const & prev = glyphs[ i-1 ];
            kerning = fontImpl->getKerning( prev.unicode, glyph.unicode );
         }

         // Draw
         if ( glyph.ref.getWidth() > 0 && glyph.ref.getHeight() > 0 )
         {
            float x1 = px + kerning.x; // + glyph.bmp.getX();
            float y1 = py - glyph.ref.getY();

            float gw = float( glyph.ref.getWidth() );
            float gh = float( glyph.ref.getHeight() );
            if ( font.lcdFit ) gh /= 3;
            float x2 = x1 + gw;
            float y2 = y1 + gh;
            float z1 = pz;

            Rectf const & texCoords = glyph.getRef().getTexCoords();
            float u1 = texCoords.u1();
            float v1 = texCoords.v1();
            float u2 = texCoords.u2();
            float v2 = texCoords.v2();

            S3DVertex A( x1, y2, z1, 0,0,0, 0x4F0000FF, 0, 1 );
            S3DVertex B( x1, y1, z1, 0,0,0, 0x4F00FF00, 0, 0 );
            S3DVertex C( x2, y1, z1, 0,0,0, 0x4FFF0000, 1, 0 );
            S3DVertex D( x2, y2, z1, 0,0,0, 0x4F00FFFF, 1, 1 );

            lines.addVertex( A ); lines.addVertex( B );
            lines.addVertex( B ); lines.addVertex( C );
            lines.addVertex( C ); lines.addVertex( D );
            lines.addVertex( D ); lines.addVertex( A );
         }
      }
      if ( font.lcdFit )
      {
         px += glyph.advance / 3;
      }
      else
      {
         px += glyph.advance;
      }

   }

   lines.Material.setDepth( Depth::disabled() );
   lines.Material.setCulling( false );
   lines.Material.setBlend( Blend::alphaBlend() );
   m_Driver->draw2D( lines );

}


} // end namespace gpu.
} // end namespace de.


#if 0

void
FontRenderer::test()
{
   int w = 800;
   int h = 600;
   if ( m_Driver )
   {
      w = m_Driver->getScreenWidth();
      h = m_Driver->getScreenHeight();
   }

   Text text1;
   text1.x = 10;
   text1.y = 10;
   text1.align = TextAlign::TopLeft;
   text1.font = Font( "../Arial.ttf", 52, false, false );
   text1.brush = Brush( RGBA( 255,255,255,255 ), nullptr );
   text1.pen = Pen( RGBA( 255,0,0,255 ), 2.5f );
   text1.msg = L"Hello World";
   addText( text1 );

   Text text2;
   text2.x = w / 2;
   text2.y = 10;
   text2.align = TextAlign::TopCenter;
   text2.font = Font( "../Arial.ttf", 52, false, false );
   text2.brush = Brush( RGBA( 255,255,255,255 ), nullptr );
   text2.pen = Pen( RGBA( 255,0,0,255 ), 2.5f );
   text2.msg = L"Hello TopCenter World";
   addText( text2 );
}

#endif

#endif // #ifdef USE_LIB_DARKFONT