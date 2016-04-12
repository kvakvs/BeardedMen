# BM: Bearded Men (and a fortress)

A voxel-based simulation management game. Where bearded men build and dig
and defend and hunt, all that sort of thing.

<img src="http://i.imgur.com/oOWkTRT.png" width="100%" />

## Current features

* OpenGL 3.2 + Qt
* World grid rendering (current level and floor under it), can navigate deeper with +/- keys.
    That also rebuilds world mesh.
* QB (Qubicle format) RGB-colored voxel models
* Keyboard controls the cursor (qb model), showing a bearded man (qb model)
* Tool dock showing cursor position and depth.
* XYZ axes in screen corner (very proud of them!)
* Game logic (simple now) runs when you press Dot key.

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
3. Open the CMakeLists file in QtCreator as a project
4. Change in Project settings to run from project root directory (there are
    loadable assets).
