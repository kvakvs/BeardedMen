#pragma once

#include <Ogre.h>
#include <memory>

namespace bm {
    namespace uniq {
        using ManualObject = std::unique_ptr<Ogre::ManualObject>;
    }
    namespace shared {
        using Mesh = Ogre::MeshPtr;
    }
} // ns bm
