#include <irrExt/BaseEventReceiver.hpp>
#include <iostream>

namespace irrExt {

BaseEventReceiver::BaseEventReceiver( irr::IrrlichtDevice* device )
    : m_Device( device )
{
    std::cout << __FUNCTION__ << "("<< (void*)m_Device << ")\n";

    assert( m_Device );

    m_Device->setEventReceiver( this );
}

BaseEventReceiver::~BaseEventReceiver()
{
    std::cout << __FUNCTION__ << "()\n";
}

bool
BaseEventReceiver::OnEvent( const irr::SEvent& event )
{
    if ( event.EventType == irr::EET_KEY_INPUT_EVENT )
    {
        irr::SEvent::SKeyInput const & keyEvent = event.KeyInput;
#ifdef USE_IRRLICHT
        if ( keyEvent.Key == irr::KEY_ESCAPE && !keyEvent.PressedDown )
#else // USE_IRRLICHT_GLES
        if ( keyEvent.Key == irr::IRR_KEY_ESCAPE && !keyEvent.PressedDown )
#endif
		{
            if ( m_Device )
            {
                m_Device->closeDevice();
            }
            return true;
        }
#ifdef USE_IRRLICHT
        if ( keyEvent.Key == irr::KEY_SPACE && !keyEvent.PressedDown )
#else // USE_IRRLICHT_GLES
        if ( keyEvent.Key == irr::IRR_KEY_SPACE && !keyEvent.PressedDown )
#endif
		{
            if ( m_Device )
            {
                irr::scene::ICameraSceneNode* camera = m_Device->getSceneManager()->getActiveCamera();
                if ( camera )
                {
                    camera->setInputReceiverEnabled( !camera->isInputReceiverEnabled() );
                }
            }
            return true;
        }
    }

    return false;
}

void
BaseEventReceiver::OnResize( irr::core::dimension2du const & winSize )
{
    if ( m_Device )
    {
        m_Device->getVideoDriver()->OnResize( winSize );
    }
}

void
BaseEventReceiver::setDevice( irr::IrrlichtDevice* device )
{
    if ( device == nullptr )
    {
        m_Device = device;
    }


}

} // end namespace irrExt
