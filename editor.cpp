#include "editor.hpp"

#include "toolbar.hpp"
#include "toolbox.hpp"
#include "track.hpp"

#include <iostream>

Editor* Editor::m_editor = 0;

// ----------------------------------------------------------------------------
bool Editor::init()
{
	m_device = createDevice(EDT_OPENGL,
            m_screen_size, 16,
		    false, false, true);

	if (!m_device) return false;
	m_device->setResizable(true);

	m_device->setWindowCaption(L"SuperTuxKart Track Editor");

	m_video_driver  = m_device->getVideoDriver();
	m_scene_manager = m_device->getSceneManager();
	m_gui_env       = m_device->getGUIEnvironment();

    m_track = Track::getTrack();

    ICameraSceneNode* cam = m_scene_manager->addCameraSceneNodeMaya();
    cam->setFarValue(20000.f);
    cam->setTarget(vector3df(0, 0, 0));
    cam->setInputReceiverEnabled(false);
    m_track->setFreeCamera(cam);

    cam = m_scene_manager->addCameraSceneNode(0, vector3df(0, 50, 0),
                                                 vector3df(0, 0, -10));
    m_scene_manager->setActiveCamera(cam);
    m_track->setNormalCamera(cam);

    m_toolbar = ToolBar::getToolBar();
    m_toolbox = ToolBox::getToolBox();


    m_device->setEventReceiver(this);

    return true;
} // init


// ----------------------------------------------------------------------------
Editor* Editor::getEditor(dimension2du screen_size)
{
	if (m_editor != 0) return m_editor;

	m_editor = new Editor();
    m_editor->m_screen_size = screen_size;
	if (!m_editor->init()) return 0;
	return m_editor;
} // getEditor

// ----------------------------------------------------------------------------
bool Editor::run()
{
	if (!m_device) return 0;


    long current_time = m_device->getTimer()->getTime();
    long last_time    = current_time;


	while (m_device->run())
    {

        current_time = m_device->getTimer()->getTime();
        m_track->animate(current_time - last_time);

		// drawing
		m_video_driver->beginScene(true, true, SColor(255, 80, 0, 170));

		m_scene_manager->drawAll();
		m_gui_env->drawAll();

		m_video_driver->endScene();

        if (m_video_driver->getScreenSize() != m_screen_size)
        {
            m_screen_size = m_video_driver->getScreenSize();
            m_toolbar->reallocate();
            m_toolbox->reallocate();
        }

        last_time = current_time;
    }
	return 1;
} // run

// ----------------------------------------------------------------------------
bool Editor::OnEvent(const SEvent& event)
{
    if (event.EventType == EET_GUI_EVENT)
    {
        s32 id = event.GUIEvent.Caller->getID();
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED)
        {
            m_gui_env->removeFocus(m_gui_env->getFocus()->getParent());
            switch (id)
            {
            case ToolBar::TBI_EXIT:
                m_device->closeDevice();
                break;
            case ToolBar::TBI_SELECT:
                m_track->setState(Track::SELECT);
                break;
            case ToolBar::TBI_MOVE:
                m_track->setState(Track::MOVE);
                break;
            case ToolBar::TBI_ROTATE:
                m_track->setState(Track::ROTATE);
                break;
            case ToolBar::TBI_DELETE:
                m_track->setState(Track::DELETE);
                break;
            case ToolBar::TBI_CAM:
                m_track->setState(Track::FREECAM);
                break;
            case ToolBar::TBI_GRID_ON_OFF:
                m_track->setGrid(!m_track->isGridOn());
                break;
            case ToolBar::TBI_GRID_INC:
                m_track->changeGridDensity(1);
                break;
            case ToolBar::TBI_GRID_DEC:
                m_track->changeGridDensity(-1);
                break;
            default:
                std::cerr << "Button click isn't handled!" << std::endl;
            }
            return true;
        } // EventType == EGET_BUTTON_CLICKED
    } // EventType == EET_GUI_EVENT

    if (m_gui_env->getFocus() != NULL) return false;

    if (event.EventType == EET_KEY_INPUT_EVENT)
    {
        m_track->keyEvent(event.KeyInput.Key, event.KeyInput.PressedDown);
    } // EventType == EET_KEY_INPUT_EVENT

    if (event.EventType == EET_MOUSE_INPUT_EVENT)
    {
        if (event.MouseInput.Y < 50) return false;
        m_track->mouseEvent(event);
    }

	return false;
} // OnEvent
