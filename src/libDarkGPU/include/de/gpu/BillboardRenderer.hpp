#ifndef DE_GPU_BILLBOARD_RENDERER_HPP
#define DE_GPU_BILLBOARD_RENDERER_HPP

#include <de/gpu/smesh/SMesh.hpp>

namespace de {
namespace gpu {

struct IVideoDriver;

// ===========================================================================
struct Billboard
// ===========================================================================
{
   enum EType
   {
      Rect = 0,
      RoundRect,
      Circle,
      ETypeCount
   };

   int typ = Rect;
   int tess = 33;
   glm::vec2 size;
   glm::vec2 radius; // relevant for RoundRect, not Circle
   glm::vec3 pos;
   float zoffset; // for multiple billboards at same pos, order of overlap zoffset.
   uint32_t color;
   TexRef tex;
   SMeshBuffer mesh;
};

// ===========================================================================
struct BillboardRenderer
// ===========================================================================
{
   DE_CREATE_LOGGER("de.gpu.BillboardRenderer")

   SMeshBuffer meshRect;
   SMeshBuffer meshRoundRect;
   SMeshBuffer meshCircle;

   std::vector< Billboard > billboards;
   BillboardRenderer();
   ~BillboardRenderer();

   void
   clear() { billboards.clear(); }

   void
   render( IVideoDriver* driver );

   void
   draw3D( IMeshBuffer & vao, IVideoDriver* driver );

   void
   add( int typ,
        glm::vec2 size,
        glm::vec3 pos,
        glm::vec2 radius,
        uint32_t color = 0xFFFFFFFF,
        TexRef ref = TexRef() );

   void
   addRect( glm::vec2 size, glm::vec3 pos,
            uint32_t color = 0xFFFFFFFF, TexRef ref = TexRef() )
   {
      add( Billboard::Rect, size, pos, glm::vec2(0,0), color, ref );
   }

   void
   addRoundRect( glm::vec2 size, glm::vec3 pos, glm::vec2 radius,
            uint32_t color = 0xFFFFFFFF, TexRef ref = TexRef() )
   {
      add( Billboard::RoundRect, size, pos, radius, color, ref );
   }

   void
   addCircle( glm::vec2 size, glm::vec3 pos,
            uint32_t color = 0xFFFFFFFF, TexRef ref = TexRef() )
   {
      add( Billboard::Circle, size, pos, glm::vec2(0,0), color, ref );
   }

};

} // end namespace gpu.
} // end namespace de.

#endif

