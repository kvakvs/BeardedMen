# BM: Bearded Men (and a fortress)

A voxel-based simulation management game. Where bearded men build and dig
and defend and hunt, all that sort of thing.

<img src="http://i.imgur.com/jY1fi5b.png" width="100%" />
<img src="http://i.imgur.com/Tzjhfz1.png" width="100%" />

## Current features

* OpenGL 3.2 + Qt
* World grid rendering (current level and floor under it), can navigate deeper with +/- keys.
    That also rebuilds world mesh.
* QB (Qubicle format) RGB-colored voxel models. Keyboard controls the cursor
   (qb model), showing a bearded man (qb model)
* Tool dock showing cursor position and depth. Gives keyboard hints.
* Componented objects: object can be an entity, a worker, can have a body (that
   takes damage), a brain (to think). Game orders classes designed (positional,
   to an area, to certain entity).
* Game logic runs when you press Dot key. Objects with brain can
   think. Objects that are workers can take orders and try to path A-star to
   the order destination.
* Astar AI goal solver. Workers have list of actions they can take with preconditions.
    Player command sets a goal with postconditions. Astar solver navigates through
    this graph of possible steps to find a list of actions.
* Priority queuing of tasks (3 priorities). Failed tasks are lowered in priority
    (for example can't reach rock to dig).

TODO: everything else

## Compiling

This CMakeLists project builds under Qt5.6 (because QOpenGLWidget) and MacOSX.
But because it is Qt and I do not use any strange OS-specific functions - this
may also build under Windows and Linux. I will try one of them later and will
adjust the CMakeLists.

So...

1. Get Qt5.6 + QtCreator + hope you have OpenGL support at least version 3.1
    (shaders v150). On OSX this will also force you to have XCode, so get
    it too.
2. Get CMake (`brew install cmake` on OSX).
3. Open the CMakeLists file in QtCreator as a project (if you have error that CMAKE_MODULE_DIR
    something something not found - add `-DCMAKE_PREFIX_PATH=~/Qt/5.6/gcc_64/` to your
    command line field, adjust path as necessary)
4. Change in Project settings to run from project root directory (there are
    loadable assets).

NOTE: On Linux with software emulated Mesa GL the game runs VERY slow if your
machine runs on battery. Use hardware accelerated drivers with GL 3.0 support
or plug in your power cable!

## Translations

Run `make locatization-update` to update files in `lang/*.ts`, run `linguist` to change
translations, then run `make localization-release`. Game will load one `lang/*.qm` file
created by `lrelease`.