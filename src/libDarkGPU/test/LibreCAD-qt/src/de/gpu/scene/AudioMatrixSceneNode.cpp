#include "AudioMatrixSceneNode.hpp"
//#include <de/gpu/AudioMatrixSceneNode.hpp>
#include <de/gpu/IVideoDriver.hpp>

namespace de {
namespace gpu {
namespace scene {

AudioMatrixSceneNode::AudioMatrixSceneNode( ISceneManager* smgr, ISceneNode* parent, int id )
   : ISceneNode( smgr,parent,id )
   , m_BoundingBox( 0,0,0, 10000,10000,10000 )
{
   m_PrimitiveRenderer.setDriver( m_SceneManager->getVideoDriver() );

   auto bbSiz = getBoundingBox().getSize();
   auto bbMin = getBoundingBox().getMin();
   auto bbMax = getBoundingBox().getMax();

   int n = 100;

   float dx = bbSiz.x / float(n);
   float dy = bbSiz.y / float(n);
   float dz = bbSiz.z / float(n);

   for ( int i = 0; i <= n; ++i )
   {
      auto a = glm::vec3( dx*i, 0, bbMin.z );
      auto b = glm::vec3( dx*i, 0, bbMax.z );
      auto colorA = RGBA( 200,124,24,4*8 );
      auto colorB = RGBA( 255,0,0,4*8 );
      m_PrimitiveRenderer.add3DLine( a,b, colorA, colorB );
   }

   for ( int i = 0; i <= n; ++i )
   {
      auto a = glm::vec3( bbMin.x, 0, dz*i );
      auto b = glm::vec3( bbMax.x, 0, dz*i );
      auto colorA = RGBA( 255,224,90,4*8 );
      auto colorB = RGBA( 200,120,20,4*8 );
      m_PrimitiveRenderer.add3DLine( a,b, colorA, colorB );
   }

}

AudioMatrixSceneNode::~AudioMatrixSceneNode()
{

}

void
AudioMatrixSceneNode::render()
{
   if ( !m_IsVisible ) return;
   m_PrimitiveRenderer.getMesh().setBlend( Blend::alphaBlend() );
   m_PrimitiveRenderer.render();
}

} // end namespace scene.
} // end namespace gpu.
} // end namespace de.



