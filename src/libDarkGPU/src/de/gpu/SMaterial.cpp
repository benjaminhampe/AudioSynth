#include <de/gpu/SMaterial.hpp>
#include <de/gpu/IVideoDriver.hpp>

namespace de {
namespace gpu {

bool
SMaterial::writeXML( tinyxml2::XMLDocument & doc, tinyxml2::XMLElement* matNode, IVideoDriver* driver ) const
{
   tinyxml2::XMLElement* cullNode = doc.NewElement( "culling" );
   cullNode->SetAttribute("on", state.culling.isEnabled() );
   cullNode->SetAttribute("front", state.culling.isFront() );
   cullNode->SetAttribute("back", state.culling.isFront() );
   cullNode->SetAttribute("cw", state.culling.isCW() );
   matNode->InsertEndChild( cullNode );

   tinyxml2::XMLElement* blendNode = doc.NewElement( "blend" );
   blendNode->SetAttribute("on", state.blend.enabled );
   blendNode->SetAttribute("eq", Blend::getEquationString(state.blend.equation).c_str() );
   blendNode->SetAttribute("src_rgb", Blend::getFunctionString(state.blend.src_rgb).c_str() );
   blendNode->SetAttribute("src_a", Blend::getFunctionString(state.blend.src_a).c_str() );
   blendNode->SetAttribute("dst_rgb", Blend::getFunctionString(state.blend.dst_rgb).c_str() );
   blendNode->SetAttribute("dst_a", Blend::getFunctionString(state.blend.dst_a).c_str() );
   matNode->InsertEndChild( blendNode );

   tinyxml2::XMLElement* depthNode = doc.NewElement( "depth" );
   depthNode->SetAttribute("on", state.depth.isEnabled() );
   depthNode->SetAttribute("zwrite", state.depth.isZWriteEnabled() );
   depthNode->SetAttribute("fn", Depth::getString( state.depth.getFunc() ).c_str() );
   matNode->InsertEndChild( depthNode );

   tinyxml2::XMLElement* stencilNode = doc.NewElement( "stencil" );
   stencilNode->SetAttribute("on", state.stencil.enabled );
   stencilNode->SetAttribute("sfail", Stencil::getActionString( state.stencil.sfail ).c_str() );
   stencilNode->SetAttribute("zfail", Stencil::getActionString( state.stencil.zfail ).c_str() );
   stencilNode->SetAttribute("zpass", Stencil::getActionString( state.stencil.zpass ).c_str() );
   stencilNode->SetAttribute("func", Stencil::getFuncString( state.stencil.testFunc ).c_str() );
   stencilNode->SetAttribute("refValue", state.stencil.testRefValue );
   stencilNode->SetAttribute("testMask", state.stencil.testMask );
   stencilNode->SetAttribute("stencilMask", state.stencil.stencilMask );
   matNode->InsertEndChild( stencilNode );

   tinyxml2::XMLElement* wireNode = doc.NewElement( "wireframe" );
   wireNode->SetAttribute("on", Wireframe );
   matNode->InsertEndChild( wireNode );

   tinyxml2::XMLElement* cloudtNode = doc.NewElement( "cloud" );
   cloudtNode->SetAttribute("on", CloudTransparent );
   matNode->InsertEndChild( cloudtNode );

   tinyxml2::XMLElement* illumNode = doc.NewElement( "illumination" );
   illumNode->SetAttribute("v", Lighting );
   matNode->InsertEndChild( illumNode );

   tinyxml2::XMLElement* fogNode = doc.NewElement( "fog" );
   fogNode->SetAttribute("v", FogEnable );
   matNode->InsertEndChild( fogNode );

   //	Ka 0.5882 0.5882 0.5882
   //	Kd 1.0000 1.0000 1.0000
   //	Ks 0.3600 0.3600 0.3600
   //	Ke 0.0000 0.0000 0.0000

   tinyxml2::XMLElement* ambientNode = doc.NewElement( "Ka" );
   ambientNode->SetAttribute("r", Ka.r );
   ambientNode->SetAttribute("g", Ka.g );
   ambientNode->SetAttribute("b", Ka.b );
   ambientNode->SetAttribute("a", Ka.a );
   matNode->InsertEndChild( ambientNode );

   tinyxml2::XMLElement* diffuseNode = doc.NewElement( "Kd" );
   diffuseNode->SetAttribute("r", Kd.r );
   diffuseNode->SetAttribute("g", Kd.g );
   diffuseNode->SetAttribute("b", Kd.b );
   diffuseNode->SetAttribute("a", Kd.a );
   matNode->InsertEndChild( diffuseNode );

   tinyxml2::XMLElement* specularNode = doc.NewElement( "Ks" );
   specularNode->SetAttribute("r", Ks.r );
   specularNode->SetAttribute("g", Ks.g );
   specularNode->SetAttribute("b", Ks.b );
   specularNode->SetAttribute("a", Ks.a );
   matNode->InsertEndChild( specularNode );

   tinyxml2::XMLElement* emissiveNode = doc.NewElement( "Ke" );
   emissiveNode->SetAttribute("r", Ke.r );
   emissiveNode->SetAttribute("g", Ke.g );
   emissiveNode->SetAttribute("b", Ke.b );
   emissiveNode->SetAttribute("a", Ke.a );
   matNode->InsertEndChild( emissiveNode );

   tinyxml2::XMLElement* reflectNode = doc.NewElement( "Kr" );
   reflectNode->SetAttribute("r", Kr.r );
   reflectNode->SetAttribute("g", Kr.g );
   reflectNode->SetAttribute("b", Kr.b );
   reflectNode->SetAttribute("a", Kr.a );
   matNode->InsertEndChild( reflectNode );

   tinyxml2::XMLElement* transparentNode = doc.NewElement( "Kt" );
   transparentNode->SetAttribute("r", Kt.r );
   transparentNode->SetAttribute("g", Kt.g );
   transparentNode->SetAttribute("b", Kt.b );
   transparentNode->SetAttribute("a", Kt.a );
   matNode->InsertEndChild( transparentNode );

   tinyxml2::XMLElement* shinyNode = doc.NewElement( "Ns" );
   shinyNode->SetAttribute("v", Shininess );
   matNode->InsertEndChild( shinyNode );

   tinyxml2::XMLElement* shinysNode = doc.NewElement( "Ni" );
   shinysNode->SetAttribute("v", Shininess_strength );
   matNode->InsertEndChild( shinysNode );

   tinyxml2::XMLElement* reflectivityNode = doc.NewElement( "Tr" );
   reflectivityNode->SetAttribute("v", Reflectivity );
   matNode->InsertEndChild( reflectivityNode );

   //	d 1.0000

//   if ( hasDiffuseMap() )
//   {
//      TexRef const & ref = getDiffuseMap();
//      tinyxml2::XMLElement* texNode = doc.NewElement( "Td" );
//      texNode->SetAttribute("name", ref.tex->getName().c_str() );
//      texNode->SetAttribute("src", ref.tex->getName().c_str() );
//      matNode->InsertEndChild( texNode );
//   }

//   if ( hasBumpMap() ){ s << ", Tb:" << getBumpMap().tex->getName(); }
//   if ( hasNormalMap() ){ s << ", Tn:" << getNormalMap().tex->getName(); }
//   if ( hasSpecularMap() ){ s << ", Ts:" << getSpecularMap().tex->getName(); }
//   if ( hasDetailMap() ){ s << ", Tdet:" << getDetailMap().tex->getName(); }
   return true;
}


bool
SMaterial::saveXML( std::string uri, IVideoDriver* driver ) const
{
   tinyxml2::XMLDocument doc;
   tinyxml2::XMLElement* matNode = doc.NewElement( "SMaterial" );
   matNode->SetAttribute( "name", Name.c_str() );
   matNode->SetAttribute( "tex-count", int( getTextureCount() ) );
   writeXML( doc, matNode, driver );

   doc.InsertEndChild( matNode );
   tinyxml2::XMLError err = doc.SaveFile( uri.c_str() );
   if ( err != tinyxml2::XML_SUCCESS )
   {
      DEM_ERROR("Cant save xml ", uri)
      return false;
   }
   return true;
}


bool
SMaterial::loadXML( std::string uri, IVideoDriver* driver )
{
   tinyxml2::XMLDocument doc;
   tinyxml2::XMLError err = doc.LoadFile( uri.c_str() );
   if ( err != tinyxml2::XML_SUCCESS )
   {
      DE_ERROR("Cant load xml ", uri)
      return false;
   }

/*
   std::string xmlDir = dbGetFileDir( uri );

   DE_DEBUG("=========================================" )
   DE_DEBUG("loadXml(",uri,")" )
   DE_DEBUG("=========================================" )
   //std::string baseName = dbGetFileBaseName( uri );
   //std::string dirName = dbGetFileDir( uri );
   //DE_DEBUG("URI baseName = ", baseName )
   //DE_DEBUG("URI dirName = ", dirName )

   tinyxml2::XMLElement* atlasNode = doc.FirstChildElement( "image-atlas" );
   if ( !atlasNode )
   {
      DE_ERROR("No image atlas node in xml ", uri)
      return false;
   }

   if ( atlasNode->Attribute( "name" ) )
   {
      Name = atlasNode->Attribute( "name" );
   }
   else
   {
      DE_ERROR("No atlas name found ", uri)
   }

   int pageCount = atlasNode->IntAttribute( "pages" );

   tinyxml2::XMLElement* pageNode = atlasNode->FirstChildElement( "page" );
   if ( !pageNode )
   {
      DE_ERROR("No image atlas pages found in xml ", uri)
      return false;
   }

   // Load Pages:
   while ( pageNode )
   {
      if ( !pageNode->Attribute("src") )
      {
         DE_ERROR("No page src" )
         continue;
      }

      int pageW = pageNode->IntAttribute("w");
      int pageH = pageNode->IntAttribute("h");
      pages.emplace_back();
      ImageAtlasPage & page = pages.back();
      page.pageId = int( pages.size() );
      page.name = pageNode->Attribute("src");

      std::string pageUri = xmlDir + "/" + page.name;
      if ( !dbLoadImage( page.img, pageUri ) )
      {
         DE_ERROR("No page ", page.pageId," src image uri ", pageUri)
         page.img.clear();
      }

      if ( pageW != page.img.getWidth() )
      {
         DE_ERROR("Differing src image width ", page.name)
      }
      if ( pageH != page.img.getHeight() )
      {
         DE_ERROR("Differing src image height ", page.name)
      }

      // Load Refs:
      int refCount = pageNode->IntAttribute("refs");

      // Load Refs:
      tinyxml2::XMLElement* refNode = pageNode->FirstChildElement( "img" );
      while ( refNode )
      {
         page.refs.emplace_back();
         ImageAtlasRef & ref = page.refs.back();

         int refId = refNode->IntAttribute("id");
         int refX = refNode->IntAttribute("x");
         int refY = refNode->IntAttribute("y");
         int refW = refNode->IntAttribute("w");
         int refH = refNode->IntAttribute("h");
         bool wantMipmaps = refNode->IntAttribute("mipmaps") != 0 ? true:false;
         std::string name = refNode->Attribute("name" );
         ref.id = refId;
         ref.name = name;
         ref.img = &page.img;
         ref.page = page.pageId;
         ref.mipmaps = wantMipmaps;
         ref.rect = Recti( refX, refY, refW, refH );

         //DE_DEBUG("Add ref ", ref.toString() )

         refNode = refNode->NextSiblingElement( "img" );
      }

      pageNode = pageNode->NextSiblingElement( "page" );
   }

   DE_DEBUG("Loaded atlas xml ", uri)

//   DE_DEBUG("PageCount = ", pages.size())
//   for ( int i = 0; i < pages.size(); ++i )
//   {
//      ImageAtlasPage & page = pages[ i ];
//      DE_DEBUG("Page[",i,"].RefCount = ", page.refs.size())
//      for ( int r = 0; r < page.refs.size(); ++r )
//      {
//         ImageAtlasRef & ref = page.refs[ r ];
//         DE_DEBUG("Page[",i,"].Ref[",r,"].Rect = ", ref.rect.toString() )
//      }
//   }
*/
   return true;
}

SMaterial::SMaterial()
   : state()
   , Lighting( 0 )
   , Wireframe( false ) // ? Not used, wireframes are overlays of debug geom
   , CloudTransparent( false ) // Benni special. uses only tex.r channel as color(r,r,r,r)
   , FogEnable( false )
   , FogColor( 0xFFFFFFFF )
   , FogMode( 0 )
   , FogLinearStart( 0.0f )
   , FogLinearEnd( 1.0f )
   , FogExpDensity( 0.001f )
   , Kd( 1,1,1,1 )
   , Ka( 0,0,0,0 )
   , Ks( 1,1,1,1 )
   , Ke( 0,0,0,0 )
   , Kt( 1,1,1,1 ) // ?
   , Kr( 1,1,1,1 ) // ?
   , Opacity( 1.0f ) // ?
   , Reflectivity( 0.1f ) // ?
   , Shininess( 30.0f ) // ?
   , Shininess_strength( 1.5f ) // ?
   , MaterialType( 0 ) // ?
   , GouraudShading( false ) // ?
   , DiffuseMap()
   , BumpMap()
   , BumpScale( 24.0f / 256.0f )
   , NormalMap()
   , NormalScale( 1.0f )
   , SpecularMap()
   , DetailMap()
   , Name("")
{
   // Default ctr constructed is also always the default material.
}

std::string
SMaterial::toString() const
{
   std::stringstream s;
   if ( state.culling.isEnabled() ) { s << "C:" << state.culling.toString(); }
   if ( Wireframe ){ s << ", W"; }
   if ( FogEnable ){ s << ", F"; }
   //	illum 2
   if ( Lighting ) { s << ", illum:"<<Lighting; }
   if ( hasDiffuseMap() ){ s << ", Td:" << getDiffuseMap().tex->getName(); }
   if ( hasBumpMap() ){ s << ", Tb:" << getBumpMap().tex->getName(); }
   if ( hasNormalMap() ){ s << ", Tn:" << getNormalMap().tex->getName(); }
   if ( hasSpecularMap() ){ s << ", Ts:" << getSpecularMap().tex->getName(); }
   if ( hasDetailMap() ){ s << ", Tdet:" << getDetailMap().tex->getName(); }
   //	Ka 0.5882 0.5882 0.5882
   //	Kd 1.0000 1.0000 1.0000
   //	Ks 0.3600 0.3600 0.3600
   //	Ke 0.0000 0.0000 0.0000
   //	Tf 1.0000 1.0000 1.0000
   //	Ns 30.0000
   //	Ni 1.5000
   //	Tr 0.0000
   { s << ", Ka:" << Ka; }
   { s << ", Kd:" << Kd; }
   { s << ", Ks:" << Ks; }
   { s << ", Ke:" << Ke; }
   { s << ", Kt:" << Kt; }
   { s << ", Ns:" << Shininess; }
   { s << ", Ni:" << Shininess_strength; }
   { s << ", Tr:" << Reflectivity; }
   //	d 1.0000
   return s.str();
}

/*
bool
SMaterial::loadMTL( std::string uri, IVideoDriver* driver )
{
   std::string content = dbLoadText( uri );
   dbStrReplace( content, "\r\n", "\n" );
   dbStrReplace( content, "\n\r", "\n" );
   std::vector< std::string > lines = dbStrSplit( content, '\n' );

   std::string baseDir = dbGetFileDir( uri );

   size_t y = 0;
   while ( y < lines.size() )
   {
      auto const & line = lines[ y ];
      size_t x = 0;
      bool foundComment = false;
      while ( x < line.size() )
      {
         if ( line[ x ] == ' ' )
            x++;
         else if ( line[ x ] == '#' )
         {
            foundComment = true;
            break;
         }
         else {
            break;
         }
      }

      if ( foundComment )
      {
         DE_WARN("Line ", y, " is a comment ", line )
      }
      else
      {
         DE_DEBUG("Line ", y, " is ", line )
         auto ls = dbStrSplit( line, ' ' );
         if ( ls.empty() )
         {
            DE_DEBUG("Line ", y, " is empty" )
         }
         else
         {
            if ( ls[0] == "Ka" )
            {
               DE_DEBUG("Found Ka" )
               float r = ls.size() > 1 ? atof( ls[ 1 ].c_str() ) : 0.0f;
               float g = ls.size() > 2 ? atof( ls[ 2 ].c_str() ) : 0.0f;
               float b = ls.size() > 3 ? atof( ls[ 3 ].c_str() ) : 0.0f;
               Ka = RGBA( glm::vec4( r,g,b,1.0f ) );
            }
            else if ( ls[0] == "Kd" )
            {
               DE_DEBUG("Found Kd" )
               float r = ls.size() > 1 ? atof( ls[ 1 ].c_str() ) : 0.0f;
               float g = ls.size() > 2 ? atof( ls[ 2 ].c_str() ) : 0.0f;
               float b = ls.size() > 3 ? atof( ls[ 3 ].c_str() ) : 0.0f;
               Kd = RGBA( glm::vec4( r,g,b,1.0f ) );
            }
            else if ( ls[0] == "Ks" )
            {
               DE_DEBUG("Found Ks" )
               float r = ls.size() > 1 ? atof( ls[ 1 ].c_str() ) : 0.0f;
               float g = ls.size() > 2 ? atof( ls[ 2 ].c_str() ) : 0.0f;
               float b = ls.size() > 3 ? atof( ls[ 3 ].c_str() ) : 0.0f;
               Ks = RGBA( glm::vec4( r,g,b,1.0f ) );
            }
            else if ( ls[0] == "Ke" )
            {
               DE_DEBUG("Found Ke" )
               float r = ls.size() > 1 ? atof( ls[ 1 ].c_str() ) : 0.0f;
               float g = ls.size() > 2 ? atof( ls[ 2 ].c_str() ) : 0.0f;
               float b = ls.size() > 3 ? atof( ls[ 3 ].c_str() ) : 0.0f;
               Ke = RGBA( glm::vec4( r,g,b,1.0f ) );
            }
            else if ( ls[0] == "Ns" )
            {
               DE_DEBUG("Found Ns" )
               float r = ls.size() > 1 ? atof( ls[ 1 ].c_str() ) : 0.0f;
               Shininess = r;
            }
            else if ( ls[0] == "map_Kd" )
            {
               TexRef ref;
               if ( ls.size() > 1 )
               {
                  DE_DEBUG("Found map_Kd" )
                  ref.tex = driver->addTexture2D( ls[ 1 ] );
               }
               DiffuseMap = ref;
            }
            else if ( ls[0] == "map_Ks" )
            {
               TexRef ref;
               if ( ls.size() > 1 )
               {
                  DE_DEBUG("Found map_Ks" )
                  ref.tex = driver->addTexture2D( ls[ 1 ] );
               }
               SpecularMap = ref;
            }
            else if ( ls[0] == "map_bump" )
            {
               TexRef ref;
               if ( ls.size() > 1 )
               {
                  DE_DEBUG("Found map_bump" )
                  ref.tex = driver->addTexture2D( ls[ 1 ] );
               }
               BumpMap = ref;
            }
            else if ( ls[0] == "bump" )
            {
               TexRef ref;
               if ( ls.size() > 1 )
               {
                  DE_DEBUG("Found bump" )
                  ref.tex = driver->addTexture2D( ls[ 1 ] );
               }
               BumpMap = ref;
            }
            else
            {
               DE_DEBUG("Found unknown line ", line )
            }
         }
      }
      y++;
   }

   DE_DEBUG("Loaded MTL ", uri, " with lines = ", lines.size() )
   return false;
}

bool
SMaterial::saveMTL( std::string uri ) const
{
   DE_DEBUG("Writing material as MTL ", uri )
   std::ostringstream s; s <<
   "# exported by libDarkGPU (c) Benjamin Hampe 2021\n"
   "newmtl " << Name << "\n";

   auto write_float = [&] ( std::string name, float f )
   {
      s << name << " " << f << "\n";
   };

   auto write_rgb = [&] ( std::string name, uint32_t color )
   {
      s << name
        << " " << RGBA_Rf( color )
        << " " << RGBA_Gf( color )
        << " " << RGBA_Bf( color ) << "\n";
   };

   auto write_tex = [&] ( std::string name, std::string tex )
   {
      if ( tex.empty() ) return;
      s << name << " " << tex << "\n";
   };

   write_rgb( "Ka", Ka );
   write_rgb( "Kd", Kd );
   write_rgb( "Ks", Ks );
   write_rgb( "Ke", Ke );
   write_float( "Ns", Shininess ); // shininess / 0.128f;
   if ( getDiffuseMap().tex )
   {
      write_tex( "map_Kd", getDiffuseMap().tex->getName() );
   }
   if ( getBumpMap().tex )
   {
      write_tex( "map_bump", getBumpMap().tex->getName() );
   }
   if ( getNormalMap().tex )
   {
      write_tex( "map_Kn", getNormalMap().tex->getName() );
   }
   if ( getSpecularMap().tex )
   {
      write_tex( "map_Ks", getSpecularMap().tex->getName() );
   }

  //write_tex( "bump", getTexture( 3 ) );

   return dbSaveText( uri, s.str() );
}
*/

} // end namespace gpu.
} // end namespace de.
