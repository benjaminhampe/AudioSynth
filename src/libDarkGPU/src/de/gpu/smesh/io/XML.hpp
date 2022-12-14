// Copyright (C) 2008-2021 Benjamin Hampe

#ifndef DE_GPU_SMESH_XML_BENNI_HPP
#define DE_GPU_SMESH_XML_BENNI_HPP

#include <de/gpu/smesh/SMesh.hpp>

namespace de {
namespace gpu {

struct SMeshXML
{
   DE_CREATE_LOGGER("de.gpu.SMeshXML")

   static bool
   writeXML( tinyxml2::XMLDocument & doc, tinyxml2::XMLElement* bufNode,
      SMeshBuffer const & buf, IVideoDriver* driver )
   {
      SMaterial const & mat = buf.getMaterial();
      tinyxml2::XMLElement* matNode = doc.NewElement( "SMaterial" );
      matNode->SetAttribute( "name", mat.Name.c_str() );
      matNode->SetAttribute( "tex-count", int( mat.getTextureCount() ) );
      mat.writeXML( doc, matNode, driver );
      bufNode->InsertEndChild( matNode );

      for ( size_t i = 0; i < buf.getVertexCount(); ++i )
      {
         S3DVertex const & V = buf.getVertex( i );
         tinyxml2::XMLElement* vNode = doc.NewElement( "S3DVertex" );
         vNode->SetAttribute( "x", V.pos.x );
         vNode->SetAttribute( "y", V.pos.y );
         vNode->SetAttribute( "z", V.pos.z );
         vNode->SetAttribute( "nx", V.normal.x );
         vNode->SetAttribute( "ny", V.normal.y );
         vNode->SetAttribute( "nz", V.normal.z );
         vNode->SetAttribute( "r", RGBA_Rf(V.color) );
         vNode->SetAttribute( "g", RGBA_Gf(V.color) );
         vNode->SetAttribute( "b", RGBA_Bf(V.color) );
         vNode->SetAttribute( "a", RGBA_Af(V.color) );
         vNode->SetAttribute( "u", V.tex.x );
         vNode->SetAttribute( "v", V.tex.y );
         bufNode->InsertEndChild( vNode );
      }

      tinyxml2::XMLElement* iNode = doc.NewElement( "Indices" );
      iNode->SetAttribute( "count", buf.getIndexCount()  );

      std::ostringstream s;
      s << "\n";
      int k = 0;

      for ( size_t i = 0; i < buf.getIndexCount(); ++i )
      {
         if ( i > 0 ) s << " ";
         auto iA = buf.getIndex( i );
         s << iA;
         if ( k >= 12 )
         {
            s << "\n";
            k = 0;
         }
         else
         {
            ++k;
         }
      }

      iNode->SetText( s.str().c_str() );
      bufNode->InsertEndChild( iNode );
      return true;
   }

   static bool
   save( SMesh const & mesh, std::string uri, IVideoDriver* driver )
   {
      DE_DEBUG("Writing XML mesh = ", uri )

      tinyxml2::XMLDocument doc;
      tinyxml2::XMLElement* meshNode = doc.NewElement( "SMesh" );
      meshNode->SetAttribute( "name", mesh.Name.c_str() );
      meshNode->SetAttribute( "buf-count", mesh.getMeshBufferCount() );

      for ( size_t b = 0; b < mesh.getMeshBufferCount(); ++b )
      {
         SMeshBuffer const & buf = mesh.getMeshBuffer( b );
         tinyxml2::XMLElement* bufNode = doc.NewElement( "SMeshBuffer" );
         bufNode->SetAttribute( "name", buf.Name.c_str() );
         bufNode->SetAttribute( "prim-type", PrimitiveType::getString( buf.PrimType ).c_str() );
         writeXML( doc, bufNode, buf, driver );
         meshNode->InsertEndChild( bufNode );
      }

      doc.InsertEndChild( meshNode );
      tinyxml2::XMLError err = doc.SaveFile( uri.c_str() );
      if ( err != tinyxml2::XML_SUCCESS )
      {
         DEM_ERROR("Cant save xml ", uri)
      }

      return true;
   }

   static bool
   save( SMeshBuffer const & buf, std::string uri, IVideoDriver* driver )
   {
      DE_DEBUG("Writing XML mesh = ", uri )

      tinyxml2::XMLDocument doc;
      tinyxml2::XMLElement* bufNode = doc.NewElement( "SMeshBuffer" );
      bufNode->SetAttribute( "name", buf.Name.c_str() );
      bufNode->SetAttribute( "prim-type", PrimitiveType::getString( buf.PrimType ).c_str() );
      writeXML( doc, bufNode, buf, driver );
      doc.InsertEndChild( bufNode );

      tinyxml2::XMLError err = doc.SaveFile( uri.c_str() );
      if ( err != tinyxml2::XML_SUCCESS )
      {
         DEM_ERROR("Cant save xml ", uri)
      }

      return true;
   }



   static bool
   load( SMesh & mesh, std::string uri, IVideoDriver* driver )
   {
      std::string matUri = dbGetFileBaseName( uri ) + ".mtl";
      DE_DEBUG("Writing OFF mesh = ", uri )
      DE_DEBUG("Writing OFF material = ", matUri )

      // write header

      std::ostringstream s;

      return true;
   }


};


} // end namespace
} // end namespace

#endif

