CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(BeardedMen)

#SET (CMAKE_C_COMPILER   "clang-3.8")
#SET (CMAKE_CXX_COMPILER "clang++-3.8")

#add_subdirectory("vl")
set(OGRE_DIR "/usr/local/include/OGRE")
set(OIS_DIR "/usr/include/OIS")
include_directories(
        ${OGRE_DIR}/
        ${OGRE_DIR}/Overlay/
        ${OIS_DIR}
        )

add_definitions(-DBM_DEBUG=1)

if(APPLE)
#    find_library(GLUT_LIBRARY GLUT)
#    MARK_AS_ADVANCED (GLUT_LIBRARY)
endif(APPLE)

set(CMAKE_CXX_FLAGS -std=c++14 ${CMAKE_CXX_FLAGS})
set(PV_DIR deps/polyvox/include/PolyVox )
set(LUA_DIR deps/lua-5.3.2 )
set(ASTAR_DIR deps/astar/cpp )
set(SELENE_DIR deps/selene/include )
set(SPATIAL deps/spatial/src )

#Projects source files
SET(SRC_FILES
    src/main.cpp
    src/perlin.cpp
    src/model_id.h

    src/util/error.h
    src/util/optional.h
    src/util/str.h
    src/util/vec.cpp		src/util/vec.h

#    src/gfx/game_widget.cpp	src/gfx/game_widget.h
#    src/gfx/gl_version.h
#    src/gfx/mesh_maker.cpp	src/gfx/mesh_maker.h
#    src/gfx/model.cpp		src/gfx/model.h
#    src/gfx/ogl_widget.h	src/gfx/ogl_widget.inl
#    src/gfx/qb_file.cpp		src/gfx/qb_file.h
#    src/gfx/ramp.cpp		src/gfx/ramp.h

    src/ai/action.cpp		src/ai/action.h
    src/ai/goal.cpp		src/ai/goal.h
    src/ai/plan.cpp		src/ai/plan.h

    src/tab/t_action.cpp	src/tab/t_action.h
    src/tab/t_metric.cpp	src/tab/t_metric.h
    src/tab/t_block.h

    src/game/component.cpp	src/game/component.h
    src/game/co_body.cpp	src/game/co_body.h
    src/game/co_brains.cpp	src/game/co_brains.h
    src/game/co_entity.cpp	src/game/co_entity.h
    src/game/co_worker.cpp	src/game/co_worker.h
    src/game/inanimate.cpp	src/game/inanimate.h
    src/game/obj_bearded_man.cpp src/game/obj_bearded_man.h
    src/game/order.cpp          src/game/order.h
    src/game/settlement.cpp	src/game/settlement.h
    src/game/world.cpp		src/game/world.h
    src/game/world_volume.cpp	src/game/world_volume.h

#    src/ui/main_window.cpp	src/ui/main_window.h
#    src/ui/cnc_dock.cpp		src/ui/cnc_dock.h
#    src/ui/log_dock.cpp		src/ui/log_dock.h

    ${ASTAR_DIR}/stlastar.h		${ASTAR_DIR}/fsa.h

    ${PV_DIR}/AStarPathfinder.h			${PV_DIR}/AStarPathfinder.inl
    ${PV_DIR}/AmbientOcclusionCalculator.h	${PV_DIR}/AmbientOcclusionCalculator.inl
    ${PV_DIR}/Array.h
    ${PV_DIR}/BaseVolume.h			${PV_DIR}/BaseVolume.inl
    ${PV_DIR}/BaseVolumeSampler.inl		${PV_DIR}/Config.h
    ${PV_DIR}/CubicSurfaceExtractor.h		${PV_DIR}/CubicSurfaceExtractor.inl
    ${PV_DIR}/DefaultIsQuadNeeded.h		${PV_DIR}/DefaultMarchingCubesController.h
    ${PV_DIR}/Density.h				${PV_DIR}/Exceptions.h
    ${PV_DIR}/FilePager.h			${PV_DIR}/Logging.h
    ${PV_DIR}/LowPassFilter.h			${PV_DIR}/LowPassFilter.inl
    ${PV_DIR}/MarchingCubesSurfaceExtractor.h	${PV_DIR}/MarchingCubesSurfaceExtractor.inl
    ${PV_DIR}/Material.h			${PV_DIR}/MaterialDensityPair.h
    ${PV_DIR}/Mesh.h				${PV_DIR}/Mesh.inl
    ${PV_DIR}/PagedVolume.h			${PV_DIR}/PagedVolume.inl
    ${PV_DIR}/PagedVolumeChunk.inl		${PV_DIR}/PagedVolumeSampler.inl
    ${PV_DIR}/Picking.h				${PV_DIR}/Picking.inl
    ${PV_DIR}/RawVolume.h			${PV_DIR}/RawVolume.inl
    ${PV_DIR}/RawVolumeSampler.inl
    ${PV_DIR}/Raycast.h				${PV_DIR}/Raycast.inl
    ${PV_DIR}/Region.h				${PV_DIR}/Region.inl
    ${PV_DIR}/Vector.h				${PV_DIR}/Vector.inl
    ${PV_DIR}/Vertex.h
    ${PV_DIR}/VolumeResampler.h			${PV_DIR}/VolumeResampler.inl

    ${PV_DIR}/Impl/AStarPathfinderImpl.h	${PV_DIR}/Impl/Assertions.h
    ${PV_DIR}/Impl/ErrorHandling.h		${PV_DIR}/Impl/ExceptionsImpl.h
    ${PV_DIR}/Impl/Interpolation.h
    ${PV_DIR}/Impl/IteratorController.h		${PV_DIR}/Impl/IteratorController.inl
    ${PV_DIR}/Impl/LoggingImpl.h		${PV_DIR}/Impl/MarchingCubesTables.h
    ${PV_DIR}/Impl/Morton.h			${PV_DIR}/Impl/PlatformDefinitions.h
    ${PV_DIR}/Impl/RandomUnitVectors.h		${PV_DIR}/Impl/RandomVectors.h
    ${PV_DIR}/Impl/Timer.h			${PV_DIR}/Impl/Utility.h

#    ${LUA_DIR}/lapi.c	${LUA_DIR}/ldebug.c	${LUA_DIR}/llex.h
#    ${LUA_DIR}/lparser.h	${LUA_DIR}/lapi.h
#    ${LUA_DIR}/ldebug.h	${LUA_DIR}/llimits.h	${LUA_DIR}/lprefix.h
#    ${LUA_DIR}/lua.h     ${LUA_DIR}/lauxlib.c	${LUA_DIR}/ldo.c
#    ${LUA_DIR}/lmathlib.c  ${LUA_DIR}/lstate.c
#    ${LUA_DIR}/lauxlib.h	${LUA_DIR}/ldo.h		${LUA_DIR}/lmem.c
#    ${LUA_DIR}/lstate.h	${LUA_DIR}/luaconf.h     ${LUA_DIR}/lbaselib.c
#    ${LUA_DIR}/ldump.c	${LUA_DIR}/lmem.h        ${LUA_DIR}/lstring.c
#    ${LUA_DIR}/lualib.h  ${LUA_DIR}/lbitlib.c	${LUA_DIR}/lfunc.c
#    ${LUA_DIR}/loadlib.c ${LUA_DIR}/lstring.h	${LUA_DIR}/lundump.c
#    ${LUA_DIR}/lcode.c	${LUA_DIR}/lfunc.h	${LUA_DIR}/lobject.c
#    ${LUA_DIR}/lstrlib.c	${LUA_DIR}/lundump.h     ${LUA_DIR}/lcode.h
#    ${LUA_DIR}/lgc.c	${LUA_DIR}/lobject.h     ${LUA_DIR}/ltable.c
#    ${LUA_DIR}/lutf8lib.c ${LUA_DIR}/lcorolib.c	${LUA_DIR}/lgc.h
#    ${LUA_DIR}/lopcodes.c ${LUA_DIR}/ltable.h	${LUA_DIR}/lvm.c
#    ${LUA_DIR}/lctype.c	${LUA_DIR}/linit.c	${LUA_DIR}/lopcodes.h
#    ${LUA_DIR}/ltablib.c	${LUA_DIR}/lvm.h         ${LUA_DIR}/lctype.h
#    ${LUA_DIR}/liolib.c	${LUA_DIR}/loslib.c      ${LUA_DIR}/ltm.c
#    ${LUA_DIR}/lzio.c    ${LUA_DIR}/ldblib.c	${LUA_DIR}/llex.c
#    ${LUA_DIR}/lparser.c ${LUA_DIR}/ltm.h		${LUA_DIR}/lzio.h

    ${SELENE_DIR}/selene/BaseFun.h		${SELENE_DIR}/selene/Class.h
    ${SELENE_DIR}/selene/ClassFun.h		${SELENE_DIR}/selene/Ctor.h
    ${SELENE_DIR}/selene/Dtor.h			${SELENE_DIR}/selene/ExceptionHandler.h
    ${SELENE_DIR}/selene/ExceptionTypes.h
    ${SELENE_DIR}/selene/Fun.h			${SELENE_DIR}/selene/function.h
    ${SELENE_DIR}/selene/LuaName.h		${SELENE_DIR}/selene/LuaRef.h
    ${SELENE_DIR}/selene/MetatableRegistry.h
    ${SELENE_DIR}/selene/Obj.h			${SELENE_DIR}/selene/ObjFun.h
    ${SELENE_DIR}/selene/primitives.h		${SELENE_DIR}/selene/references.h
    ${SELENE_DIR}/selene/Registry.h		${SELENE_DIR}/selene/ResourceHandler.h
    ${SELENE_DIR}/selene/Selector.h		${SELENE_DIR}/selene/State.h
    ${SELENE_DIR}/selene/traits.h		${SELENE_DIR}/selene/Tuple.h
    ${SELENE_DIR}/selene/util.h			${SELENE_DIR}/selene.h

    ${SPATIAL}/spatial.hpp                  ${SPATIAL}/point_multiset.hpp
    ${SPATIAL}/box_multiset.hpp             ${SPATIAL}/metric.hpp
    ${SPATIAL}/idle_box_multiset.hpp        ${SPATIAL}/region_iterator.hpp
    ${SPATIAL}/equal_iterator.hpp           ${SPATIAL}/idle_box_multimap.hpp
    ${SPATIAL}/exception.hpp                ${SPATIAL}/neighbor_iterator.hpp
    ${SPATIAL}/point_multimap.hpp           ${SPATIAL}/box_multimap.hpp
    ${SPATIAL}/mapping_iterator.hpp         ${SPATIAL}/idle_point_multimap.hpp
    ${SPATIAL}/ordered_iterator.hpp         ${SPATIAL}/function.hpp
    ${SPATIAL}/idle_point_multiset.hpp      ${SPATIAL}/algorithm.hpp
    ${SPATIAL}/bits/spatial_equal.hpp       ${SPATIAL}/bits/spatial_assert.hpp
    ${SPATIAL}/bits/spatial_except.hpp      ${SPATIAL}/bits/spatial_region.hpp
    ${SPATIAL}/bits/spatial_bidirectional.hpp
    ${SPATIAL}/bits/spatial_mutate.hpp      ${SPATIAL}/bits/spatial_rank.hpp
    ${SPATIAL}/bits/spatial_mapping.hpp     ${SPATIAL}/bits/spatial_import_type_traits.hpp
    ${SPATIAL}/bits/spatial_closed_region.hpp
    ${SPATIAL}/bits/spatial_kdtree.hpp      ${SPATIAL}/bits/spatial_math.hpp
    ${SPATIAL}/bits/spatial_overlap_region.hpp ${SPATIAL}/bits/spatial_manhattan_neighbor.hpp
    ${SPATIAL}/bits/spatial_quadrance_neighbor.hpp ${SPATIAL}/bits/spatial_check_concept.hpp
    ${SPATIAL}/bits/spatial_neighbor.hpp    ${SPATIAL}/bits/spatial_relaxed_kdtree.hpp
    ${SPATIAL}/bits/spatial_enclosed_region.hpp ${SPATIAL}/bits/spatial_open_region.hpp
    ${SPATIAL}/bits/spatial_builtin.hpp     ${SPATIAL}/bits/spatial_node.hpp
    ${SPATIAL}/bits/spatial_value_compare.hpp ${SPATIAL}/bits/spatial_ordered.hpp
    ${SPATIAL}/bits/spatial_compress.hpp    ${SPATIAL}/bits/spatial_import_tuple.hpp
    ${SPATIAL}/bits/spatial_condition.hpp   ${SPATIAL}/bits/spatial_preorder.hpp
    ${SPATIAL}/bits/spatial_euclidian_neighbor.hpp  ${SPATIAL}/bits/spatial_template_member_swap.hpp

)

#SET(INC_FILES .......)

#"Sources" and "Headers" are the group names in Visual Studio.
#They may have other uses too...
#SOURCE_GROUP("Sources" FILES ${SRC_FILES})
#SOURCE_GROUP("Headers" FILES ${INC_FILES})

#Tell CMake the paths for OpenGL and for PolyVox (which is just relative to our current location)
INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/deps/polyvox/include
    ${CMAKE_SOURCE_DIR}/${LUA_DIR}
    ${CMAKE_SOURCE_DIR}/deps/selene/include
    ${CMAKE_SOURCE_DIR}/deps/astar/cpp
    ${CMAKE_SOURCE_DIR}/${SPATIAL}
#    ${OPENGL_INCLUDE_DIR}
#    ${PolyVoxHeaders_SOURCE_DIR}
)

#Build
ADD_EXECUTABLE(BeardedMen ${SRC_FILES} ${COMMON_RESOURCES_RCC})
IF(MSVC)
	SET_TARGET_PROPERTIES(
            BeardedMen PROPERTIES
	    COMPILE_FLAGS "/W4 /wd4127")
ENDIF(MSVC)
TARGET_LINK_LIBRARIES(BeardedMen
        OgreMain OgreOverlay boost_system OIS)
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
