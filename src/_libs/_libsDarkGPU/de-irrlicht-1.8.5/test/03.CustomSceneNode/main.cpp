/** Example 003 Custom SceneNode */
#include <irrlicht.h>
#include <driverChoice.h>

using namespace irr;

class CSampleSceneNode : public scene::ISceneNode
{
	core::aabbox3d<f32> Box;
	video::S3DVertex Vertices[4];
	video::SMaterial Material;
public:
	CSampleSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id)
		: scene::ISceneNode(parent, mgr, id)
	{
		Material.Wireframe = false;
		Material.Lighting = false;
		Vertices[0] = video::S3DVertex(0,0,10, 1,1,0, video::SColor(255,0,255,255), 0, 1);
		Vertices[1] = video::S3DVertex(10,0,-10, 1,0,0, video::SColor(255,255,0,255), 1, 1);
		Vertices[2] = video::S3DVertex(0,20,0, 0,1,1, video::SColor(255,255,255,0), 1, 0);
		Vertices[3] = video::S3DVertex(-10,0,-10, 0,0,1, video::SColor(255,0,255,0), 0, 0);
		Box.reset(Vertices[0].Pos);
		for (s32 i=1; i<4; ++i) Box.addInternalPoint( Vertices[i].Pos );
	}

   virtual void
   OnRegisterSceneNode()
	{
		if (IsVisible) SceneManager->registerNodeForRendering(this);
		ISceneNode::OnRegisterSceneNode();
	}

   virtual void
   render()
	{
		u16 indices[] = {	0,2,3, 2,1,3, 1,0,3, 2,0,1	};
		video::IVideoDriver* driver = SceneManager->getVideoDriver();
		driver->setMaterial(Material);
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		driver->drawVertexPrimitiveList(&Vertices[0], 4, &indices[0], 4, video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);
	}

   virtual const core::aabbox3d<f32>& getBoundingBox() const{return Box;}

   virtual u32 getMaterialCount() const{return 1;}

   virtual video::SMaterial& getMaterial(u32 i){return Material;}
};

int main()
{
	// ask user for driver
	video::E_DRIVER_TYPE driverType=driverChoiceConsole();
	if (driverType==video::EDT_COUNT) return 1;

	// create device
	IrrlichtDevice* device = createDevice( driverType, core::dimension2d<u32>(1024, 768), 32, false);
   if (!device) return 1; // could not create selected driver.

	// create engine and camera
   device->setResizable( true );
	device->setWindowCaption(L"Custom Scene Node - Irrlicht Engine Demo");
	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	smgr->addCameraSceneNode(0, core::vector3df(0,-40,0), core::vector3df(0,0,0));
	CSampleSceneNode* myNode =	new CSampleSceneNode(smgr->getRootSceneNode(), smgr, 666);
	scene::ISceneNodeAnimator* anim = smgr->createRotationAnimator(core::vector3df(0.8f, 0, 0.8f));
	if (anim)
	{
		myNode->addAnimator(anim);
		anim->drop();
		anim = 0;
	}
	myNode->drop();
	myNode = 0; // As I shouldn't refer to it again, ensure that I can't

   // Now draw everything and finish.
	u32 frames=0;
	while (device->run())
	{
		driver->beginScene(true, true, video::SColor(0,100,100,100));
		smgr->drawAll();
		driver->endScene();
		if (++frames==100)
		{
			core::stringw str = L"Irrlicht Engine [";
			str += driver->getName();
			str += L"] FPS: ";
			str += (s32)driver->getFPS();
			device->setWindowCaption(str.c_str());
			frames=0;
		}
	}
	device->drop();
	return 0;
}