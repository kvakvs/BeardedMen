#pragma once

#define OIS_DYNAMIC_LIB

#include <Ogre.h>
#include <OgreFrameListener.h>
#include <OgreWindowEventUtilities.h>
#include <OIS/OIS.h>

namespace bm {

class Game: public Ogre::FrameListener,
            public Ogre::WindowEventListener,
            public OIS::KeyListener,
            public OIS::MouseListener
{
    std::unique_ptr<Ogre::Root> root_;
    Ogre::ConfigFile cf_;
    Ogre::RenderWindow *window_ = nullptr;
    Ogre::ResourceGroupManager *res_group_ = nullptr;
    Ogre::SceneManager* scene_mgr_ = nullptr;
    Ogre::Camera* cam_ = nullptr;
    Ogre::Viewport* viewport_ = nullptr;

    bool closing_ = false;

    //
    // IO
    //
    OIS::InputManager *input_mgr_ = nullptr;
    OIS::Keyboard *keyboard_ = nullptr;
    OIS::Mouse *mouse_ = nullptr;

    //
    // World and stuff
    //
    Ogre::SceneNode *sn_world_node_ = nullptr;
    Ogre::ManualObject *world_ = nullptr;

public:
    // Entry point
    Game() {}
    bool init();
    void run();
    virtual ~Game();

private:
    bool init_ogre();
    bool init_scene();
    bool init_materials();
    bool init_models();
    bool init_view();
    bool init_input();
    bool init_terrain_chunk();

public:
    // This gets called before the next frame is beeing rendered.
    virtual bool frameStarted(const Ogre::FrameEvent &evt) override {
        //update the input devices
        keyboard_->capture();
        mouse_->capture();

        //exit if key KC_ESCAPE pressed
        if (keyboard_->isKeyDown(OIS::KC_ESCAPE) || closing_) {
            return false;
        }

        return true;
    }

    // This gets called at the end of a frame.
    virtual bool frameEnded(const Ogre::FrameEvent &evt) override {
        return true;
    }

    // OIS::KeyListener
    virtual bool keyPressed(const OIS::KeyEvent &e) override { return true; }
    virtual bool keyReleased(const OIS::KeyEvent &e) override { return true; }

    // OIS::MouseListener
    virtual bool mouseMoved(const OIS::MouseEvent &e) override { return true; }

    virtual bool mousePressed(const OIS::MouseEvent &e,
                      OIS::MouseButtonID id) override { return true; }

    virtual bool mouseReleased(const OIS::MouseEvent &e,
                       OIS::MouseButtonID id) override {
        return true;
    }

    virtual void windowClosed(Ogre::RenderWindow *rw) override {}
    virtual bool windowClosing(Ogre::RenderWindow *rw) override {
        closing_ = true;
        return true;
    }
    //virtual void windowFocusChange(Ogre::RenderWindow *rw) override { return true; }
    //virtual void windowMoved(Ogre::RenderWindow *rw) override { return true; }
    //virtual void windowResized(Ogre::RenderWindow *rw) override { return true; }

protected:
    static void setup_visible_mouse(OIS::ParamList &pl);
};

} // ns bm
