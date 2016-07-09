#include "main.h"
#include "gfx/qb_file.h"

namespace bm {

void Game::setup_visible_mouse(OIS::ParamList &pl) {
#if defined OIS_WIN32_PLATFORM
    pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
    pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
    pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
    pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM
    pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
    pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
    pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
    pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif
}

bool bm::Game::init()
{
    return init_ogre()
           && init_materials()
           && init_scene()
           && init_view()
           && init_input()
           && init_terrain_chunk()
           && init_models();
}

    bool bm::Game::init_ogre() {
    root_ = std::make_unique<Ogre::Root>();

    cf_.load("resources.cfg");
    res_group_ = &Ogre::ResourceGroupManager::getSingleton();

    // Go through all sections & settings in the file
    auto seci = cf_.getSectionIterator();

    Ogre::String section_n, type_n, path_name;
    while (seci.hasMoreElements()) {
        section_n = seci.peekNextKey();
        auto settings = seci.getNext();
        for (auto i: *settings) {
            type_n = i.first;
            path_name = i.second;
            res_group_->addResourceLocation(path_name, type_n, section_n);
        }
    }

    if (!root_->showConfigDialog()) {
        return false; // Exit the application on cancel
    }

    window_ = root_->initialise(true, "Bearded Men");

    res_group_->initialiseAllResourceGroups();

    return true;
}

void Game::run() {
    // blocks until a frame listener returns false. eg from pressing escape
    // in this example
    root_->startRendering();
}

Game::~Game() {
    input_mgr_->destroyInputObject(mouse_);
    mouse_ = nullptr;
    input_mgr_->destroyInputObject(keyboard_);
    keyboard_ = nullptr;
    OIS::InputManager::destroyInputSystem(input_mgr_);
    input_mgr_ = nullptr;
    root_.release();
}

bool Game::init_scene() {
    scene_mgr_ = root_->createSceneManager(Ogre::ST_EXTERIOR_CLOSE);
    scene_mgr_->setAmbientLight(Ogre::ColourValue(.5f, .5f, .5f));
    //auto r = scene_mgr_->getRootSceneNode();
    //sn_world_node_ = r->createChildSceneNode();

    Ogre::Light* l = scene_mgr_->createLight("sun");
    l->setType(Ogre::Light::LT_DIRECTIONAL);
    l->setDirection(-Ogre::Vector3::UNIT_Y);

    return true;
}

bool Game::init_view() {
    cam_ = scene_mgr_->createCamera("camera");
    viewport_ = window_->addViewport(cam_);

    cam_->setNearClipDistance(0.5f);
    cam_->setFarClipDistance(1000.f);
    cam_->setPosition(0.f, 0.f, 0.f);
    cam_->lookAt(20.f, 0.f, 0.f);

    return true;
}

bool Game::init_terrain_chunk() {
//    if ( world_ != NULL ) {
//        sn_world_node_->detachObject(world_);
//        scene_mgr_->destroyManualObject(world_);
//    }
    //world_ = scene_mgr_->createManualObject("Terrain");
    //sn_world_node_->attachObject(world_);

    return true;
}

bool Game::init_input() {
    //----------------------------------------------------
    // 7 add OIS input handling
    //----------------------------------------------------
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    //tell OIS about the Ogre window_
    window_->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
    setup_visible_mouse(pl);

    //setup the manager, keyboard_ and mouse_ to handle input
    input_mgr_ = OIS::InputManager::createInputSystem(pl);
    keyboard_ = static_cast<OIS::Keyboard*>(
            input_mgr_->createInputObject(OIS::OISKeyboard, true));
    mouse_ = static_cast<OIS::Mouse*>(
            input_mgr_->createInputObject(OIS::OISMouse, true));

    //tell OIS about the window_'s dimensions
    unsigned int width, height, depth;
    int top, left;
    window_->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mouse_->getMouseState();
    ms.width = width;
    ms.height = height;

    // everything is set up, now we listen for input and frames (replaces while loops)
    //key events
    keyboard_->setEventCallback(this);
    mouse_->setEventCallback(this);
    root_->addFrameListener(this);
    Ogre::WindowEventUtilities::addWindowEventListener(window_, this);

    return true;
}

bool Game::init_materials() {
//    auto resm = Ogre::ResourceGroupManager::getSingleton();
//    auto matm = Ogre::MaterialManager::getSingleton();
//
//    auto material_name = "assets/mat/Voxel";
//    auto ds = resm.openResource(material_name, "General", false);
//    matm.parseScript(ds, "General");
//    auto material = matm.load("Robot","General");

    return true;
}

bool Game::init_models() {
    auto m1 = create_model_from_qb(scene_mgr_, "bearded_man", false);
    auto m1o = scene_mgr_->getRootSceneNode()->createChildSceneNode("bmnode");
    m1o->attachObject(m1);
    m1o->setPosition(20.f, 0.f, 0.f);

//    "boulder";
//    "mark_pick";
    return true;
}


} // ns bm

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char **argv)
#endif
{
    auto game = std::make_unique<bm::Game>();
    if (not game->init()) {
        return 1;
    }
    game->run();
    return 0;
}
