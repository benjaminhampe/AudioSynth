#ifndef DE_GPU_SCENE_AUDIO_MATRIX_HPP
#define DE_GPU_SCENE_AUDIO_MATRIX_HPP

#include <de/gpu/IVideoDriver.hpp>
#include <de/gpu/Primitive3DRenderer.hpp>

namespace de {
namespace gpu {
namespace scene {

// =======================================================================
struct AudioMatrixSceneNode : public ISceneNode
// =======================================================================
{
   AudioMatrixSceneNode( ISceneManager* smgr, ISceneNode* parent = nullptr, int id = -1 );
   ~AudioMatrixSceneNode() override;
   void render() override;
   Box3f getBoundingBox() const override { return m_BoundingBox; }
   SceneNodeType::EType getNodeType() const override { return SceneNodeType::User; }

protected:
   DE_CREATE_LOGGER("de.gpu.AudioMatrixSceneNode")
   Box3f m_BoundingBox;
   Primitive3DRenderer m_PrimitiveRenderer;
};

} // end namespace scene.
} // end namespace gpu.
} // end namespace de.

#endif

