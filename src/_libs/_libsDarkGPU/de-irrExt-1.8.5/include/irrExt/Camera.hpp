#ifndef DE_ALPHASONIC_CREATE_CAMERA_HPP
#define DE_ALPHASONIC_CREATE_CAMERA_HPP

#include <irrExt/_common.hpp>

namespace irrExt {

struct Camera
{
    static irr::scene::ICameraSceneNode*
    createFPSCamera( irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* rootNode )
    {
        irr::SKeyMap keyMap[6];
        keyMap[0].Action = irr::EKA_MOVE_FORWARD;
        keyMap[1].Action = irr::EKA_MOVE_BACKWARD;
        keyMap[2].Action = irr::EKA_STRAFE_LEFT;
        keyMap[3].Action = irr::EKA_STRAFE_RIGHT;
        keyMap[4].Action = irr::EKA_JUMP_UP;
        keyMap[5].Action = irr::EKA_CROUCH;
#ifdef USE_IRRLICHT
		keyMap[0].KeyCode = irr::KEY_KEY_W;
        keyMap[1].KeyCode = irr::KEY_KEY_S;
        keyMap[2].KeyCode = irr::KEY_KEY_A;
        keyMap[3].KeyCode = irr::KEY_KEY_D;
        keyMap[4].KeyCode = irr::KEY_SPACE;		
        keyMap[5].KeyCode = irr::KEY_KEY_C;
#else // USE_IRRLICHT_GLES ( in work )
		keyMap[0].KeyCode = irr::IRR_KEY_W;
        keyMap[1].KeyCode = irr::IRR_KEY_S;
        keyMap[2].KeyCode = irr::IRR_KEY_A;
        keyMap[3].KeyCode = irr::IRR_KEY_D;
        keyMap[4].KeyCode = irr::IRR_KEY_SPACE;		
        keyMap[5].KeyCode = irr::IRR_KEY_C;
#endif
        irr::scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS( rootNode, 65.f,0.5f,-1,keyMap,6,false,1.0f, false, true );
        cam->setNearValue( 1.0f );
        cam->setFarValue( 10000.0f );
        cam->setPosition( irr::core::vector3df(0,200,-300) );
        cam->setTarget( irr::core::vector3df(0,0,0) );
        return cam;
    }

    irr::core::matrix4  m_AbsoluteTransform;
    irr::core::matrix4  m_View;
    irr::core::matrix4  m_Projection;

    irr::core::vector3df   m_Position;
    irr::core::vector3df   m_Target;
    irr::core::vector3df   m_UpVector;

    Camera()
    {
        m_Position = irr::core::vector3df( 0,0,100 );
        m_Target = irr::core::vector3df( 0,0,0 );
        m_UpVector = irr::core::vector3df( 0,1,0 );
        updateViewMatrix();
    }

    irr::core::matrix4 const & getAbsoluteTransformation() const
    {
        return m_AbsoluteTransform;
    }

    irr::core::matrix4 const & getViewMatrix() const
    {
        return m_View;
    }

    irr::core::matrix4 const & getProjectionMatrix() const
    {
        return m_Projection;
    }

    irr::core::vector3df const & getUpVector() const
    {
        return m_UpVector;
    }

    irr::core::vector3df const & getPosition() const
    {
        return m_Position;
    }

    irr::core::vector3df const & getTarget() const
    {
        return m_Target;
    }

    void setUpVector( irr::core::vector3df const & up )
    {
        m_UpVector = up;
        updateViewMatrix();
    }

    void setPosition( irr::core::vector3df const & pos )
    {
        m_Position = pos;
        updateViewMatrix();
    }

    void setTarget( irr::core::vector3df const & pos )
    {
        m_Position = pos;
        updateViewMatrix();
    }

    void updateViewMatrix()
    {
        m_View.makeIdentity();
        m_View.buildCameraLookAtMatrixLH( m_Position, m_Target, m_UpVector );
        updateAbsoluteTransformation();
    }

    void updateAbsoluteTransformation()
    {
        m_AbsoluteTransform = m_Projection * m_View;
    }

    void setPerspectiveFov( float fov, float aspect, float nearPlane = 0.01f, float farPlane = 1000.0f )
    {
        m_Projection.buildProjectionMatrixPerspectiveFovLH( fov, aspect, nearPlane, farPlane );
        updateAbsoluteTransformation();
    }

    void setOrtho( float w, float h, float nearPlane = 0.01f, float farPlane = 1000.0f )
    {
        m_Projection.buildProjectionMatrixOrthoLH( w, h, nearPlane, farPlane );
        updateAbsoluteTransformation();
    }

    void moveForward()
    {

    }

    void moveBackward()
    {

    }

    void strafeLeft()
    {

    }

    void strafeRight()
    {

    }

    void zoomIn()
    {

    }

    void zoomOut()
    {

    }

    bool OnEvent( const irr::SEvent& event )
    {
        if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
        {
            const irr::SEvent::SMouseInput& mouse = event.MouseInput;

            if (mouse.Wheel < 0.0f)
            {
                zoomIn();
                return true;
            }
            else if (mouse.Wheel > 0.0f)
            {
                zoomOut();
                return true;
            }
        }
        if (event.EventType == irr::EET_KEY_INPUT_EVENT)
        {
            const irr::SEvent::SKeyInput& key = event.KeyInput;
#ifdef USE_IRRLICHT
            if (key.Key == irr::KEY_KEY_W)
#else				
            if (key.Key == irr::IRR_KEY_W)
#endif				
            {
                moveForward();
                return true;
            }
#ifdef USE_IRRLICHT
            if (key.Key == irr::KEY_KEY_S)
#else
			if (key.Key == irr::IRR_KEY_S)
#endif				
            {
                moveBackward();
                return true;
            }
#ifdef USE_IRRLICHT
            if (key.Key == irr::KEY_KEY_A)
#else			
            if (key.Key == irr::IRR_KEY_A)
#endif				
            {
                strafeLeft();
                return true;
            }
#ifdef USE_IRRLICHT
            if (key.Key == irr::KEY_KEY_D)
#else			
            if (key.Key == irr::IRR_KEY_D)
#endif				
            {
                strafeRight();
                return true;
            }
        }
        return false;
    }

};

} // end namespace irrExt

#endif // DE_ALPHASONIC_SCENE_CAMERA_HPP
