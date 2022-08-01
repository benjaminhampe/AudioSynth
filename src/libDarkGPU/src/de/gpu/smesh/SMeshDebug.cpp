#include <de/gpu/smesh/Debug.hpp>

namespace de {
namespace gpu {

VisualDebugData::VisualDebugData( uint32_t visualFlags )
   : flags( visualFlags )
   , normalLength( 1.0f )
   , normalColor( 0x2F0000FF )
   , bboxColor( 0x6F00DDEF )
   , bboxesColor( 0x7F30BBCC )
   , wireframeScale( 1.001f )
   , wireframeColor( 0x406AAADA )
{

}

namespace smesh {

Debug::Debug()
{}

void
Debug::clear()
{
   debugMesh.clear();
}

void
Debug::add( SMesh const & mesh, VisualDebugData const & dbgData )
{
   for ( SMeshBuffer const & src : mesh.Buffers )
   {
      add( src, dbgData );
   }
}

void
Debug::add( SMeshBuffer const & src, VisualDebugData const & dbgData )
{
   debugData = dbgData;

   if ( debugMesh.getMeshBufferCount() < 1 )
   {
      SMeshBuffer dst( PrimitiveType::Lines );
      dst.setLighting( 0 );
      dst.setBlend( Blend::alphaBlend() );
      debugMesh.addMeshBuffer( dst );
   }

   SMeshBuffer & mesh = debugMesh.Buffers.back();

   if ( debugData.flags & VisualDebugData::BBOXES )
   {
      SMeshBufferTool::addLineBox( mesh, src.BoundingBox.widen( 0.1f ), debugData.bboxesColor );
   }
   if ( debugData.flags & VisualDebugData::NORMALS )
   {
      SMeshBufferTool::addLineNormals( mesh, src, debugData.normalLength, debugData.normalColor );
   }
   if ( debugData.flags & VisualDebugData::WIREFRAME )
   {
      SMeshBufferTool::addWireframe( mesh, src, debugData.wireframeScale, debugData.wireframeColor );
   }
   if ( debugData.flags & VisualDebugData::BBOX )
   {
      SMeshBufferTool::addLineBox( mesh, src.BoundingBox.widen( 0.03f ), debugData.bboxColor );
   }
}

} // end namespace smesh.
} // end namespace gpu.
} // end namespace de.


