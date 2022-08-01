#ifndef BASEEVENTRECEIVER_HPP
#define BASEEVENTRECEIVER_HPP

#include <irrExt/_common.hpp>

namespace irrExt {

class BaseEventReceiver : public irr::IEventReceiver
{
public:
    explicit BaseEventReceiver( irr::IrrlichtDevice* device = nullptr );
    virtual ~BaseEventReceiver();

    bool OnEvent( const irr::SEvent& event ) override;
    void OnResize( irr::core::dimension2du const & winSize );
    void setDevice( irr::IrrlichtDevice* device );

public:
    irr::IrrlichtDevice* m_Device = nullptr;
    irr::core::dimension2du m_WindowSize;
};


} // end namespace irrExt

#endif // BASEEVENTRECEIVER_HPP
