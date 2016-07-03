#include "main.h"

namespace bm {

void BeardedOgre::setup_visible_mouse(OIS::ParamList &pl) {
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

}

void setup_visible_mouse(OIS::ParamList &multimap);
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char **argv)
#endif
{
    auto root = std::make_unique<Ogre::Root>();

    // configure resource paths
    //-----------------------------------------------------
    // Load resource paths from config file
    // File format is:
    //  [ResourceGroupName]
    //  ArchiveType=Path
    //  .. repeat
    // For example:
    //  [General]
    //  FileSystem=media/
    //  Zip=packages/level1.zip

    Ogre::ConfigFile cf;
    cf.load("resources.cfg");

    // Go through all sections & settings in the file
    auto seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements()) {
        secName = seci.peekNextKey();
        auto settings = seci.getNext();
        for (auto i: *settings) {
            typeName = i.first;
            archName = i.second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                    archName, typeName, secName);
        }
    }

    //-----------------------------------------------------
    // 3 Configures the application and creates the window
    //-----------------------------------------------------
    if (!root->showConfigDialog()) {
        return false; // Exit the application on cancel
    }

    auto window = root->initialise(true, "Bearded Men");

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    //-----------------------------------------------------
    // 4 Create the SceneManager
    //
    //        ST_GENERIC = octree
    //        ST_EXTERIOR_CLOSE = simple terrain
    //        ST_EXTERIOR_FAR = nature terrain (depreciated)
    //        ST_EXTERIOR_REAL_FAR = paging landscape
    //        ST_INTERIOR = Quake3 BSP
    //----------------------------------------------------- 
    Ogre::SceneManager* sceneMgr = root->createSceneManager(Ogre::ST_GENERIC);

    //----------------------------------------------------- 
    // 5 Create the camera 
    //----------------------------------------------------- 
    Ogre::Camera* camera = sceneMgr->createCamera("SimpleCamera");

    //----------------------------------------------------- 
    // 6 Create one viewport, entire window 
    //----------------------------------------------------- 
    Ogre::Viewport* viewPort = window->addViewport(camera);

    //---------------------------------------------------- 
    // 7 add OIS input handling 
    //----------------------------------------------------
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    //tell OIS about the Ogre window
    window->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
    bm::BeardedOgre::setup_visible_mouse(pl);

    //setup the manager, keyboard and mouse to handle input
    auto inputManager = OIS::InputManager::createInputSystem(pl);
    auto keyboard = static_cast<OIS::Keyboard*>(
            inputManager->createInputObject(OIS::OISKeyboard, true));
    auto mouse = static_cast<OIS::Mouse*>(
            inputManager->createInputObject(OIS::OISMouse, true));

    //tell OIS about the window's dimensions
    unsigned int width, height, depth;
    int top, left;
    window->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mouse->getMouseState();
    ms.width = width;
    ms.height = height;

    // everything is set up, now we listen for input and frames (replaces while loops)
    //key events
    auto game_listener = std::make_unique<bm::BeardedOgre>(keyboard, mouse);
    keyboard->setEventCallback(game_listener.get());
    mouse->setEventCallback(game_listener.get());
    root->addFrameListener(game_listener.get());
    Ogre::WindowEventUtilities::addWindowEventListener(
            window, game_listener.get());

    //----------------------------------------------------
    // 8 start rendering 
    //----------------------------------------------------
    // blocks until a frame listener returns false. eg from pressing escape
    // in this example
    root->startRendering();

    //----------------------------------------------------
    // 9 clean 
    //----------------------------------------------------
    //OIS
    inputManager->destroyInputObject(mouse); mouse = 0;
    inputManager->destroyInputObject(keyboard); keyboard = 0;
    OIS::InputManager::destroyInputSystem(inputManager); inputManager = 0;
    //listeners
//    delete frameListener;
//    delete mouseListener;
//    delete keyListener;
//    //Ogre
//    delete root;

    return 0;
}
