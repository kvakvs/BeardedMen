#pragma once

#define OIS_DYNAMIC_LIB

#include <Ogre.h>
#include <OgreFrameListener.h>
#include <OIS/OIS.h>

namespace bm {

class SimpleFrameListener : public Ogre::FrameListener {
public:
    SimpleFrameListener(OIS::Keyboard *keyboard, OIS::Mouse *mouse) {
        mKeyboard = keyboard;
        mMouse = mouse;
    }

    // This gets called before the next frame is beeing rendered.
    bool frameStarted(const Ogre::FrameEvent &evt) {
        //update the input devices
        mKeyboard->capture();
        mMouse->capture();

        //exit if key KC_ESCAPE pressed
        if (mKeyboard->isKeyDown(OIS::KC_ESCAPE)) {
            return false;
        }

        return true;
    }

    // This gets called at the end of a frame.
    bool frameEnded(const Ogre::FrameEvent &evt) {
        return true;
    }

private:
    OIS::Keyboard *mKeyboard;
    OIS::Mouse *mMouse;
};

    class SimpleKeyListener : public OIS::KeyListener {
public:
    bool keyPressed(const OIS::KeyEvent &e) { return true; }

    bool keyReleased(const OIS::KeyEvent &e) { return true; }
};

class SimpleMouseListener : public OIS::MouseListener {
public:
    bool mouseMoved(const OIS::MouseEvent &e) { return true; }

    bool mousePressed(const OIS::MouseEvent &e,
                      OIS::MouseButtonID id) { return true; }

    bool mouseReleased(const OIS::MouseEvent &e,
                       OIS::MouseButtonID id) { return true; }
};

} // ns bm
