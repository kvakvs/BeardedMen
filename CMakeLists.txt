CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(BeardedMen)

# Qt library
find_package(Qt5Widgets)
find_package(Qt5OpenGL)
set(CMAKE_AUTOMOC ON)

#add_subdirectory("vl")

if(APPLE)
#    find_library(GLUT_LIBRARY GLUT)
#    MARK_AS_ADVANCED (GLUT_LIBRARY)
endif(APPLE)

set(CMAKE_CXX_FLAGS -std=c++14 ${CMAKE_CXX_FLAGS})

#Projects source files
SET(SRC_FILES
    src/main.cpp
    src/perlin.cpp
    src/model_id.h
    src/util_str.h
    src/vector.h
    src/world_pager.cpp		src/world_pager.h

    src/gfx/game_widget.cpp	src/gfx/game_widget.h
    src/gfx/gl_version.h
    src/gfx/loader.cpp		src/gfx/loader.h
    src/gfx/model.cpp		src/gfx/model.h
    src/gfx/ogl_widget.h	src/gfx/ogl_widget.inl
    src/gfx/qb_file.cpp		src/gfx/qb_file.h

    src/ai/action.cpp		src/ai/action.h
    src/ai/goal.cpp		src/ai/goal.h

    src/game/block_id.h
    src/game/component.cpp	src/game/component.h
    src/game/co_body.cpp	src/game/co_body.h
    src/game/co_brains.cpp	src/game/co_brains.h
    src/game/co_entity.cpp	src/game/co_entity.h
    src/game/co_worker.cpp	src/game/co_worker.h
    src/game/obj_bearded_man.cpp src/game/obj_bearded_man.h
    src/game/order.cpp          src/game/order.h
    src/game/settlement.cpp	src/game/settlement.h
    src/game/world.cpp		src/game/world.h
    
    src/ui/main_window.cpp	src/ui/main_window.h
    src/ui/dock.cpp		src/ui/dock.h

    polyvox/include/PolyVox/AStarPathfinder.h
    polyvox/include/PolyVox/AStarPathfinder.inl
    polyvox/include/PolyVox/AmbientOcclusionCalculator.h
    polyvox/include/PolyVox/AmbientOcclusionCalculator.inl
    polyvox/include/PolyVox/Array.h
    polyvox/include/PolyVox/BaseVolume.h
    polyvox/include/PolyVox/BaseVolume.inl
    polyvox/include/PolyVox/BaseVolumeSampler.inl
    polyvox/include/PolyVox/Config.h
    polyvox/include/PolyVox/CubicSurfaceExtractor.h
    polyvox/include/PolyVox/CubicSurfaceExtractor.inl
    polyvox/include/PolyVox/DefaultIsQuadNeeded.h
    polyvox/include/PolyVox/DefaultMarchingCubesController.h
    polyvox/include/PolyVox/Density.h
    polyvox/include/PolyVox/Exceptions.h
    polyvox/include/PolyVox/FilePager.h
    polyvox/include/PolyVox/Logging.h
    polyvox/include/PolyVox/LowPassFilter.h
    polyvox/include/PolyVox/LowPassFilter.inl
    polyvox/include/PolyVox/MarchingCubesSurfaceExtractor.h
    polyvox/include/PolyVox/MarchingCubesSurfaceExtractor.inl
    polyvox/include/PolyVox/Material.h
    polyvox/include/PolyVox/MaterialDensityPair.h
    polyvox/include/PolyVox/Mesh.h
    polyvox/include/PolyVox/Mesh.inl
    polyvox/include/PolyVox/PagedVolume.h
    polyvox/include/PolyVox/PagedVolume.inl
    polyvox/include/PolyVox/PagedVolumeChunk.inl
    polyvox/include/PolyVox/PagedVolumeSampler.inl
    polyvox/include/PolyVox/Picking.h
    polyvox/include/PolyVox/Picking.inl
    polyvox/include/PolyVox/RawVolume.h
    polyvox/include/PolyVox/RawVolume.inl
    polyvox/include/PolyVox/RawVolumeSampler.inl
    polyvox/include/PolyVox/Raycast.h
    polyvox/include/PolyVox/Raycast.inl
    polyvox/include/PolyVox/Region.h
    polyvox/include/PolyVox/Region.inl
    polyvox/include/PolyVox/Vector.h
    polyvox/include/PolyVox/Vector.inl
    polyvox/include/PolyVox/Vertex.h
    polyvox/include/PolyVox/VolumeResampler.h
    polyvox/include/PolyVox/VolumeResampler.inl

    polyvox/include/PolyVox/Impl/AStarPathfinderImpl.h
    polyvox/include/PolyVox/Impl/Assertions.h
    polyvox/include/PolyVox/Impl/ErrorHandling.h
    polyvox/include/PolyVox/Impl/ExceptionsImpl.h
    polyvox/include/PolyVox/Impl/Interpolation.h
    polyvox/include/PolyVox/Impl/IteratorController.h
    polyvox/include/PolyVox/Impl/IteratorController.inl
    polyvox/include/PolyVox/Impl/LoggingImpl.h
    polyvox/include/PolyVox/Impl/MarchingCubesTables.h
    polyvox/include/PolyVox/Impl/Morton.h
    polyvox/include/PolyVox/Impl/PlatformDefinitions.h
    polyvox/include/PolyVox/Impl/RandomUnitVectors.h
    polyvox/include/PolyVox/Impl/RandomVectors.h
    polyvox/include/PolyVox/Impl/Timer.h
    polyvox/include/PolyVox/Impl/Utility.h

    #lua-5.3.2/lua.c
    #lua-5.3.2/luac.c
    lua-5.3.2/lapi.c	lua-5.3.2/ldebug.c	lua-5.3.2/llex.h
    lua-5.3.2/lparser.h	lua-5.3.2/lapi.h
    lua-5.3.2/ldebug.h	lua-5.3.2/llimits.h	lua-5.3.2/lprefix.h
    lua-5.3.2/lua.h     lua-5.3.2/lauxlib.c	lua-5.3.2/ldo.c
    lua-5.3.2/lmathlib.c  lua-5.3.2/lstate.c
    lua-5.3.2/lauxlib.h	lua-5.3.2/ldo.h		lua-5.3.2/lmem.c
    lua-5.3.2/lstate.h	lua-5.3.2/luaconf.h     lua-5.3.2/lbaselib.c
    lua-5.3.2/ldump.c	lua-5.3.2/lmem.h        lua-5.3.2/lstring.c
    lua-5.3.2/lualib.h  lua-5.3.2/lbitlib.c	lua-5.3.2/lfunc.c
    lua-5.3.2/loadlib.c lua-5.3.2/lstring.h	lua-5.3.2/lundump.c
    lua-5.3.2/lcode.c	lua-5.3.2/lfunc.h	lua-5.3.2/lobject.c
    lua-5.3.2/lstrlib.c	lua-5.3.2/lundump.h     lua-5.3.2/lcode.h
    lua-5.3.2/lgc.c	lua-5.3.2/lobject.h     lua-5.3.2/ltable.c
    lua-5.3.2/lutf8lib.c lua-5.3.2/lcorolib.c	lua-5.3.2/lgc.h
    lua-5.3.2/lopcodes.c lua-5.3.2/ltable.h	lua-5.3.2/lvm.c
    lua-5.3.2/lctype.c	lua-5.3.2/linit.c	lua-5.3.2/lopcodes.h
    lua-5.3.2/ltablib.c	lua-5.3.2/lvm.h         lua-5.3.2/lctype.h
    lua-5.3.2/liolib.c	lua-5.3.2/loslib.c      lua-5.3.2/ltm.c
    lua-5.3.2/lzio.c    lua-5.3.2/ldblib.c	lua-5.3.2/llex.c
    lua-5.3.2/lparser.c lua-5.3.2/ltm.h		lua-5.3.2/lzio.h

    selene/include/selene/BaseFun.h     selene/include/selene/Class.h
    selene/include/selene/ClassFun.h    selene/include/selene/Ctor.h
    selene/include/selene/Dtor.h        selene/include/selene/ExceptionHandler.h
    selene/include/selene/ExceptionTypes.h
    selene/include/selene/Fun.h         selene/include/selene/function.h
    selene/include/selene/LuaName.h     selene/include/selene/LuaRef.h
    selene/include/selene/MetatableRegistry.h
    selene/include/selene/Obj.h         selene/include/selene/ObjFun.h
    selene/include/selene/primitives.h  selene/include/selene/references.h
    selene/include/selene/Registry.h    selene/include/selene/ResourceHandler.h
    selene/include/selene/Selector.h    selene/include/selene/State.h
    selene/include/selene/traits.h      selene/include/selene/Tuple.h
    selene/include/selene/util.h        selene/include/selene.h
)

#SET(INC_FILES .......)

#"Sources" and "Headers" are the group names in Visual Studio.
#They may have other uses too...
#SOURCE_GROUP("Sources" FILES ${SRC_FILES})
#SOURCE_GROUP("Headers" FILES ${INC_FILES})

#Tell CMake the paths for OpenGL and for PolyVox (which is just relative to our current location)
INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/polyvox/include
    ${CMAKE_SOURCE_DIR}/lua-5.3.2
    ${CMAKE_SOURCE_DIR}/selene/include
    ${OPENGL_INCLUDE_DIR}
#    ${PolyVoxHeaders_SOURCE_DIR}
)

#This will include the shader files inside the compiled binary
QT5_ADD_RESOURCES(
    COMMON_RESOURCES_RCC
    assets/pa.qrc)

# Put the resources in a seperate folder in Visual Studio
SOURCE_GROUP("Resources" FILES src/pa.qrc ${COMMON_RESOURCES_RCC})

#Build
ADD_EXECUTABLE(BeardedMen ${SRC_FILES} ${COMMON_RESOURCES_RCC})
IF(MSVC)
	SET_TARGET_PROPERTIES(
            BeardedMen PROPERTIES
	    COMPILE_FLAGS "/W4 /wd4127")
ENDIF(MSVC)
TARGET_LINK_LIBRARIES(BeardedMen
    Qt5::OpenGL
    # ${GLUT_LIBRARY}
)
#SET_PROPERTY(TARGET BeardedMen PROPERTY FOLDER "Examples")

#Install - Only install the example in Windows
IF(WIN32)
        INSTALL(TARGETS BeardedMen
		RUNTIME DESTINATION Examples/OpenGL/bin
		LIBRARY DESTINATION Examples/OpenGL/lib
		ARCHIVE DESTINATION Examples/OpenGL/lib
		COMPONENT example
	)

	#.dlls should be installed in shared builds.
	#INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/../../release/PolyVoxCore.dll DESTINATION Examples/OpenGL/bin CONFIGURATIONS Release)
	#INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/../../release/PolyVoxUtil.dll DESTINATION Examples/OpenGL/bin CONFIGURATIONS Release)
	
	#INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/../../debug/PolyVoxCore.dll DESTINATION Examples/OpenGL/bin CONFIGURATIONS Debug)
	#INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/../../debug/PolyVoxUtil.dll DESTINATION Examples/OpenGL/bin CONFIGURATIONS Debug)
ENDIF(WIN32)
