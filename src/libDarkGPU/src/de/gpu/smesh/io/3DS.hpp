#ifndef DE_GPU_SMESH_LOAD_3DS_HPP
#define DE_GPU_SMESH_LOAD_3DS_HPP

#include <de/gpu/smesh/SMesh.hpp>

namespace de {
namespace gpu {

struct IVideoDriver;

struct SMeshLoad3DS
{
   DE_CREATE_LOGGER("de.gpu.SMeshLoad3DS")

   static bool
   isSupportedFileExtension( std::string const & ext )
   {
      return ext == "3ds";
   }

   static bool
   load( SMesh & mesh, Binary & file, IVideoDriver* driver );

   static bool
   load( SMesh & mesh, std::string uri, IVideoDriver* driver )
   {
      Binary file( uri );
      if ( !file.is_open() )
      {
         DE_ERROR("Cant open ",uri)
         return false;
      }
      return load( mesh, file, driver );
   }

};

} // end namespace gpu
} // end namespace de

#endif

