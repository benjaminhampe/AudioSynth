#ifndef DE_CAD_HPP
#define DE_CAD_HPP

#include <DarkGPU.hpp>
#include <de/gpu/TrajectoryRenderer.hpp>

namespace de {
namespace gpu {

// =====================================================================
struct CADObj
// =====================================================================
{
   int id;
   bool visible;
   VisualDebugData debugData;
   SMesh mesh;
   TRSd trs;
   //glm::dmat4 trs = glm::dmat4( 1.0 );
};


//// =====================================================================
//struct LibreCadLine
//// =====================================================================
//{
//   glm::dvec3 A;
//   glm::dvec3 B;
//   uint32_t ColorA = 0xFFFFFFFF;
//   uint32_t ColorB = 0xFFFFFFFF;
//   bool Visible;

//};

// =====================================================================
struct LiCAD_Face
// =====================================================================
{
   std::vector< glm::dvec3 > Points;
   std::vector< uint32_t > Colors;
   bool visible = true;
};

// =====================================================================
struct LiCAD_IPathElement
// =====================================================================
{
   virtual ~LiCAD_IPathElement() = default;
};

// =====================================================================
struct LiCAD_PathLineElement : public LiCAD_IPathElement
// =====================================================================
{
   glm::dvec3 A;
   glm::dvec3 B;
   uint32_t ColorA = 0xFFFFFFFF;
   uint32_t ColorB = 0xFFFFFFFF;
   bool visible = true;
};

// =====================================================================
struct LiCAD_CircleArcPathElement : public LiCAD_IPathElement
// =====================================================================
{
   glm::dvec3 a;
   glm::dvec3 b;
   glm::dvec3 sattel; // Control point is local min/max of curve
   glm::dvec3 sattelTangent; // Control point tangent

   uint32_t colorA = 0xFFFFFFFF;
   uint32_t colorB = 0xFFFFFFFF;
   uint32_t colorSattel = 0xFFFFFFFF;
   bool visible = true;
};

// =====================================================================
struct LiCAD_CatmullRomPathElement : public LiCAD_IPathElement
// =====================================================================
{
   glm::dvec3 a;
   glm::dvec3 b;
   glm::dvec3 sattel; // Control point is local min/max of curve
   glm::dvec3 sattelTangent; // Control point tangent

   uint32_t colorA = 0xFFFFFFFF;
   uint32_t colorB = 0xFFFFFFFF;
   uint32_t colorSattel = 0xFFFFFFFF;
   bool visible = true;
};

// =====================================================================
struct LibreCAD_IShape
// =====================================================================
{
   int shapeType;
   int shapeId;
};

// =====================================================================
struct LibreCAD_Path : public LibreCAD_IShape
// =====================================================================
{
   std::vector< LiCAD_IPathElement* > elements;
   std::vector< uint32_t > colors;
   bool visible;
};


// =====================================================================
struct AppContext
// =====================================================================
{
   DE_CREATE_LOGGER("AppContext")

   IVideoDriver* driver;
   ICamera camera;

   TrajectoryRenderer m_TrajectoryRenderer;

   std::array< bool, 256*4 > keys;

   int mouseX = 0;
   int mouseY = 0;
   int mouseXLast = 0;
   int mouseYLast = 0;
   int mouseMoveX = 0;
   int mouseMoveY = 0;
   bool isFullscreen = true;
   bool isLeftPressed = false;
   bool isRightPressed = false;
   bool isMiddlePressed = false;
   bool isDragging = false;

   double time;
   double timeLastTitleUpdate;

   std::vector< CADObj > objs;

   // Picking ( Collision Control )
   int pickObjIndex = -1;
   int pickObjBufferIndex = -1;
   int pickObjVertexIndex = -1;
   int pickObjTriangleIndex = -1;
   CADObj* pickObj = nullptr;

   double pickDist = std::numeric_limits< double >::max();
   glm::dvec3 pickPos;

   AppContext()
      : driver( nullptr )
   {
      DE_DEBUG("")
   }
   ~AppContext()
   {
      DE_DEBUG("")
   }
};

}
}

// ========================================================================
int
CAD_findObj( de::gpu::AppContext & app, int objId );

de::gpu::CADObj &
CAD_getObj( de::gpu::AppContext & app, int objId );

#endif
