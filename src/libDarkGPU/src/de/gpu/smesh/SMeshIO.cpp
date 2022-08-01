#include <de/gpu/smesh/SMeshIO.hpp>

#include <de/gpu/IVideoDriver.hpp>
#include <de/gpu/smesh/io/3DS.hpp>
#include <de/gpu/smesh/io/XML.hpp>
#include <de/gpu/smesh/io/OFF.hpp>
#include <de/gpu/smesh/io/OBJ.hpp>
#include <de/gpu/smesh/io/STL.hpp>

#ifdef USE_LIB_ASSIMP
#include <de/gpu/smesh/io/ASSIMP.hpp>
#endif 

namespace de {
namespace gpu {

// ===========================================================================

bool
SMeshIO::load( SMeshBuffer & mesh, std::string uri, IVideoDriver* driver, SMeshOptions const & options )
{
   //DE_DEBUG("Loading meshbuffer ", uri )

   std::string ext = dbGetFileSuffix( uri );
   bool ok = false;
   if ( ext == "off" )
   {
      ok = SMeshBufferOFF::load( mesh, uri );
   }
   else if ( ext == "stl" )
   {
      ok = SMeshBufferSTL::load( mesh, uri );
   }
//   else if ( ext == "obj" )
//   {
//      return SMeshBufferLoadOBJ::load( mesh, uri );
//   }
//   else if ( ext == "off" )
//   {
//      return SMeshBufferLoadOFF::load( mesh, uri );
//   }
   if ( !ok )
   {
      DE_ERROR("Unsupported extension ", ext, ", uri = ", uri)
   }
   else
   {
      mesh.recalculateBoundingBox();
   }
   return ok;
}

// ===========================================================================

bool
SMeshIO::load( SMesh & mesh, std::string uri, IVideoDriver* driver, SMeshOptions const & options )
{
   // DE_DEBUG("Loading mesh ", uri )

   PerformanceTimer timer;
   timer.start();
   std::string ext = dbGetFileSuffix( uri );

   bool ok = false;
   if ( ext == "xml" )
   {
      ok = SMeshXML::load( mesh, uri, driver );
   }
   else if ( ext == "3ds" )
   {
      ok = SMeshLoad3DS::load( mesh, uri, driver );
   }
   else if ( ext == "obj" )
   {
      //ok = SMeshOBJ::load( mesh, uri, driver );
   }
   else if ( ext == "off" )
   {
      //ok = SMeshOBJ::load( mesh, uri, driver );
   }
#ifdef USE_LIB_ASSIMP
   if ( !ok )
   {
      ok = SMeshLoadASSIMP::load( mesh, uri, driver, options );
   }
#endif

   if ( !ok )
   {
      DE_ERROR("Unsupported 3d model file! ext(", ext, "), uri(", uri,")")
      return false;
   }

   if ( options.flipYZ )
   {
      SMeshTool::flipYZ( mesh );
   }

   if ( options.centerVertices )
   {
      SMeshTool::centerVertices( mesh );
   }

   if ( options.scaleToMaxSize )
   {
      auto size = mesh.getBoundingBox().getSize();
      auto max_size = std::max( std::max( size.x, size.y ), size.z );
      auto scale = options.maxSize / max_size;
      SMeshTool::scaleVertices( mesh, scale, scale, scale );
   }

   if ( options.computeNormals )
   {
      SMeshTool::recalculateNormals( mesh );
   }

//   if ( options.debug )
//   {
//      mesh.recalculateBoundingBox();
//      DE_DEBUG("Needed ms(",timer.ms(),"), ", mesh.toString() )
//   }

   timer.stop();
   DE_DEBUG("Needed ms(",timer.ms(),"), mesh(", mesh.toString(),")" )
   return ok;
}

// ===========================================================================

bool
SMeshIO::save( SMeshBuffer const & mesh, std::string uri, IVideoDriver* driver, SMeshOptions const & options )
{
   //DE_DEBUG("Saving meshbuffer ", uri )

   std::string ext = dbGetFileSuffix( uri );

   if ( ext == "off" )
   {
      return SMeshBufferOFF::save( mesh, uri );
   }
   else if ( ext == "stl" )
   {
      return SMeshBufferSTL::save( mesh, uri );
   }
   else if ( ext == "xml" )
   {
      return SMeshXML::save( mesh, uri, driver );
   }
   else if ( ext == "obj" )
   {
      return SMeshOBJ::save( mesh, uri, driver );
   }
   else
   {
      DE_ERROR("Unsupported extension ", ext, ", uri = ", uri)
      return false;
   }
}

// ===========================================================================

bool
SMeshIO::save( SMesh const & mesh, std::string uri, IVideoDriver* driver, SMeshOptions const & options )
{
   // DE_DEBUG("Save SMesh ", uri )

   PerformanceTimer timer;
   timer.start();

   std::string ext = dbGetFileSuffix( uri );
   bool ok = false;
   if ( ext == "obj" )
   {
      ok = SMeshOBJ::save( mesh, uri, driver );
   }
   else if ( ext == "xml" )
   {
      ok = SMeshXML::save( mesh, uri, driver );
   }

//   else if ( ext == "off" )
//   {
//      return SMeshOFF::save( mesh, uri );
//   }
//   else if ( ext == "stl" )
//   {
//      return SMeshSTL::save( mesh, uri );
//   }

   if ( !ok )
   {
      DE_ERROR("Unsupported 3d model file save ext(", ext, "), uri(", uri,")")
      return false;
   }

   timer.stop();
   DE_DEBUG("Needed ms(",timer.ms(),"), mesh(", mesh.toString(),")" )
   return true;
}



} // end namespace
} // end namespace

