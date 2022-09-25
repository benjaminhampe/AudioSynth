#include <wx/wx.h>
#include <iostream>
#include <irrlicht.h>
#include "IrrlichtControl.h"

BEGIN_EVENT_TABLE(IrrlichtControl, wxControl)
    EVT_TIMER(ID_IRR_TIMER, IrrlichtControl::OnTimer)
    EVT_SIZE(IrrlichtControl::OnSize)
    EVT_PAINT(IrrlichtControl::OnPaint)
    EVT_ERASE_BACKGROUND(IrrlichtControl::OnEraseBackground)
    EVT_MOUSE_EVENTS(IrrlichtControl::OnMouse)
    EVT_KEY_DOWN(IrrlichtControl::OnKey)
    EVT_KEY_UP(IrrlichtControl::OnKey)
END_EVENT_TABLE()

IrrlichtControl::IrrlichtControl(wxWindow* parent, wxWindowID id, bool bs, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxControl(parent, id, pos, size, style, wxDefaultValidator, name),
	device(0),
    driver(0),
    smgr(0),
    guienv(0),
    camera(0),
    m_Timer(this, ID_IRR_TIMER),
    m_pFPSText(0),
	m_showFPS(true)
{
    std::cout<<"IrrlichtControl::IrrlichtControl()"<<std::endl;
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    init();
    startTimer();

}//ctor

IrrlichtControl::~IrrlichtControl()
{
    std::cout<<"IrrlichtControl::~IrrlichtControl()"<<std::endl;
	device->drop();
}//dtor

void IrrlichtControl::init()
{
    std::cout<<"IrrlichtControl::init()"<<std::endl;
    params.DriverType = irr::video::EDT_OPENGL;
    params.WindowSize = irr::core::dimension2d<irr::u32>((irr::u32)GetClientSize().GetX(), (irr::u32)GetClientSize().GetY());
    params.Bits = 32;
    params.LoggingLevel = irr::ELL_NONE;

        std::cout<<"Render() failed"<<std::endl;
#ifdef LINUX
//    wxStaticText* pnl = new wxStaticText(this, -1, wxEmptyString);
//    pnl->SetBackgroundStyle(wxBG_STYLE_CUSTOM);
//
//    GtkWidget* handle = pnl->GetHandle();
//    if (!handle)
//        return;
//    XID xHandle = GDK_WINDOW_XWINDOW(handle->window);
//    Display* mDisp = GDK_WINDOW_XDISPLAY(handle->window);
//    stringc dName = (long)mDisp;
//    dName += ':';
//    dName += (int)xHandle;
//    actual_params->WindowId = (long)dName.c_str();
#else
    params.WindowId = reinterpret_cast<void*>(this->GetHandle());
#endif

    device = irr::createDeviceEx(params);

    if (!device) {
        throw "Can't create Irrlicht device!";
	}//if
    device->setResizable(true);

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

    wxString o(wxT(""));
    o+=device->getOSOperator()->getOperationSystemVersion(); o+=wxT("\n");
    irr::u32 CPU_MHz, RAM_Total, RAM_Free;
    device->getOSOperator()->getProcessorSpeedMHz(&CPU_MHz);
    device->getOSOperator()->getSystemMemory(&RAM_Total, &RAM_Free);
    o+=wxT("CPU "); o+=CPU_MHz; o+=wxT(" MHz\n");
    o+=wxT("RAM "); o+=RAM_Free; o+=wxT(" / "); o+=RAM_Total; o+=wxT("\n");
    o+=wxT("GPU "); o+=wxString(driver->getVendorInfo().c_str(),wxConvUTF8); o+=wxT("\n");

    std::cout<<o.mb_str(wxConvUTF8)<<std::endl;

    driver->OnResize(params.WindowSize);

	if (m_showFPS) {
		m_pFPSText = guienv->addStaticText(L"FPS: 0", irr::core::rect<irr::s32>(50, 50, 150, 100), false);
		m_pFPSText->setOverrideColor(irr::video::SColor(255,255,255,255));
	}

    //test scene
    irr::scene::ISceneNode* root = smgr->getRootSceneNode();
	irr::scene::ICameraSceneNode* cam = smgr->addCameraSceneNode(root);
	if (cam)
    {
        cam->setPosition(irr::core::vector3df(0,50,0));
        cam->setTarget(irr::core::vector3df(0,0,0));
        irr::scene::ISceneNodeAnimator* anim = smgr->createFlyCircleAnimator(irr::core::vector3df(0,15,0), 30.0f);
        cam->addAnimator(anim);
        anim->drop();

    }

	irr::scene::ISceneNode* cube = smgr->addCubeSceneNode(20);
    if (cube)
    {
        cube->setMaterialTexture(0, driver->getTexture("../media/mauer.png"));
        cube->setMaterialTexture(1, driver->getTexture("../media/hyper01.jpg"));
        cube->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        cube->setMaterialType( irr::video::EMT_REFLECTION_2_LAYER );
    }

//			smgr->addSkyBoxSceneNode(
//			driver->getTexture("irrlicht2_up.jpg"),
//			driver->getTexture("irrlicht2_dn.jpg"),
//			driver->getTexture("irrlicht2_lf.jpg"),
//			driver->getTexture("irrlicht2_rt.jpg"),
//			driver->getTexture("irrlicht2_ft.jpg"),
//			driver->getTexture("irrlicht2_bk.jpg"));


}//InitIrr()


void IrrlichtControl::render()
{
    //std::cout<<"IrrlichtControl::Render()"<<std::endl;
    try
    {
        if (!device->run())
        {
            std::cout<<"Device not running"<<std::endl;
            return;
        }

        driver->beginScene(true, true, irr::video::SColor(255,255,128,40));
        smgr->drawAll();
        guienv->drawAll();
        driver->endScene();

        if (m_pFPSText && m_showFPS)
        {
            int fps = driver->getFPS();
            static int lastFPS = 0;
            if (lastFPS != fps) {
                irr::core::stringw str = L"FPS: ";
                str += fps;
                m_pFPSText->setText(str.c_str());
                lastFPS = fps;
            }//if
        }//if
    }
    catch(...)
    {
        std::cout<<"Render() failed"<<std::endl;
    }
}//Render()

void IrrlichtControl::startTimer(int milliseconds)
{
    std::cout<<"IrrlichtControl::StartRendering()"<<std::endl;
    m_Timer.Start(milliseconds);
}//StartRendering()

void IrrlichtControl::stopTimer()
{
    std::cout<<"IrrlichtControl::StopRendering()"<<std::endl;
    m_Timer.Stop();
}//StopRendering()

//void IrrlichtControl::OnRender()
//{
//    std::cout<<"IrrlichtControl::OnCreateScene()"<<std::endl;
//    try
//    {
//
//    driver->beginScene(true, true, irr::video::SColor(255,255,128,40));
//    smgr->drawAll();
//    guienv->drawAll();
//    driver->endScene();
//    }
//    catch (...)
//    {
//        std::cout<<"Error in Render."<<std::endl;
//    }
//}//OnRender()

void IrrlichtControl::OnPaint(wxPaintEvent& event)
{
    //std::cout<<"IrrlichtControl::OnPaint()"<<std::endl;
    wxPaintDC paint_dc(this);
    render();
}//OnPaint()

void IrrlichtControl::OnSize(wxSizeEvent& event)
{
    std::cout<<"IrrlichtControl::OnSize()"<<std::endl;
    if (!driver)
    {
        return;
	}//if

    irr::u32 w = event.GetSize().x;
    irr::u32 h = event.GetSize().y;

    //GetClientSize(&w, &h);
    //dimension2d<u32> size((u32)w, (u32)h);
    driver->OnResize(irr::core::dimension2du(w,h));
    driver->setViewPort(irr::core::recti(0,0,w,h));
    irr::scene::ICameraSceneNode* cam = smgr->getActiveCamera();
    if (cam) {
        cam->setAspectRatio((float)w / (float)h);
	}//if
	device->run();
}//OnSize

void IrrlichtControl::OnTimer(wxTimerEvent& event)
{
    //std::cout<<"IrrlichtControl::OnTimer()"<<std::endl;
    render();
}

void IrrlichtControl::OnMouse(wxMouseEvent& event)
{
    //std::cout<<"IrrlichtControl::OnMouse()"<<std::endl;
    irr::SEvent sevt;

    sevt.EventType = irr::EET_MOUSE_INPUT_EVENT;

    if (event.IsButton())
    {
        if (event.LeftDown())
            sevt.MouseInput.Event = irr::EMIE_LMOUSE_PRESSED_DOWN;
        else if (event.LeftUp())
            sevt.MouseInput.Event = irr::EMIE_LMOUSE_LEFT_UP;
        else if (event.MiddleDown())
            sevt.MouseInput.Event = irr::EMIE_MMOUSE_PRESSED_DOWN;
        else if (event.MiddleUp())
            sevt.MouseInput.Event = irr::EMIE_MMOUSE_LEFT_UP;
        else if (event.RightDown())
            sevt.MouseInput.Event = irr::EMIE_RMOUSE_PRESSED_DOWN;
        else if (event.RightUp())
            sevt.MouseInput.Event = irr::EMIE_RMOUSE_LEFT_UP;
    }//if

    else if (event.GetWheelRotation() != 0) {
        sevt.MouseInput.Wheel = (float)event.GetWheelRotation();
        sevt.MouseInput.Event = irr::EMIE_MOUSE_WHEEL;
    }//else if

    else if (event.Moving() || event.Dragging()) {
		sevt.MouseInput.Event = irr::EMIE_MOUSE_MOVED;
	}//else if

    sevt.MouseInput.X = event.GetX();
    sevt.MouseInput.Y = event.GetY();

    device->postEventFromUser(sevt);

    if (!m_Timer.IsRunning()) {
        render();
	}//if

    event.Skip();
}//OnMouse()

void IrrlichtControl::OnKey(wxKeyEvent& event)
{
    std::cout<<"IrrlichtControl::OnKey()"<<std::endl;

    irr::SEvent sevt;

    sevt.EventType = irr::EET_KEY_INPUT_EVENT;

    sevt.KeyInput.Key = (irr::EKEY_CODE)event.GetRawKeyCode();
    sevt.KeyInput.PressedDown = event.GetEventType() == wxEVT_KEY_DOWN;

    sevt.KeyInput.Shift = event.ShiftDown();
    sevt.KeyInput.Control = event.CmdDown();
#if wxUSE_UNICODE
    sevt.KeyInput.Char = event.GetUnicodeKey();
#else
    sevt.KeyInput.Char = event.GetKeyCode();
#endif

    device->postEventFromUser(sevt);

    if (!m_Timer.IsRunning()) {
        render();
	}//if

    event.Skip();
}//OnKey()
