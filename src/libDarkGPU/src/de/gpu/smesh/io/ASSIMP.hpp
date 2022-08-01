#ifndef DE_GPU_SMESHLOADER_ASSIMP_HPP
#define DE_GPU_SMESHLOADER_ASSIMP_HPP

#include <de/gpu/smesh/SMesh.hpp>

#ifdef USE_LIB_ASSIMP

namespace de {
namespace gpu {

struct IVideoDriver;

// ===========================================================================
struct SMeshLoadASSIMP
// ===========================================================================
{
   DE_CREATE_LOGGER("de.gpu.SMeshLoadASSIMP")

   static bool
   load( SMesh & out, std::string const & uri, IVideoDriver* driver, SMeshOptions const & options = SMeshOptions() );
};


} // end namespace gpu.
} // end namespace de.

#endif // #ifdef USE_LIB_ASSIMP

#endif // DE_GPU_SMESHLOADER_HPP
