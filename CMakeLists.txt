CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(playable_alpha4)

# Qt library
find_package(Qt5Widgets)
find_package(Qt5OpenGL)
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_CXX_FLAGS}")

#Projects source files
SET(SRC_FILES
	src/main.cpp
	src/perlin.cpp
	src/base_app.cpp
)

#Projects headers files	
SET(INC_FILES	
	src/OpenGLWidget.h
	src/OpenGLWidget.inl
	src/perlin.h
	src/base_app.h
)

#"Sources" and "Headers" are the group names in Visual Studio.
#They may have other uses too...
SOURCE_GROUP("Sources" FILES ${SRC_FILES})
SOURCE_GROUP("Headers" FILES ${INC_FILES})

#Tell CMake the paths for OpenGL and for PolyVox (which is just relative to our current location)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR}
    ${PolyVoxHeaders_SOURCE_DIR}
    ../common
    polyvox/include)

#This will include the shader files inside the compiled binary
QT5_ADD_RESOURCES(
    COMMON_RESOURCES_RCC
    assets/pa.qrc)

# Put the resources in a seperate folder in Visual Studio
SOURCE_GROUP("Resources" FILES src/pa.qrc ${COMMON_RESOURCES_RCC})

#Build
ADD_EXECUTABLE(playable_alpha4 ${SRC_FILES} ${COMMON_RESOURCES_RCC})
IF(MSVC)
	SET_TARGET_PROPERTIES(
	    playable_alpha4 PROPERTIES
	    COMPILE_FLAGS "/W4 /wd4127")
ENDIF(MSVC)
TARGET_LINK_LIBRARIES(playable_alpha4 Qt5::OpenGL)
#SET_PROPERTY(TARGET playable_alpha4 PROPERTY FOLDER "Examples")

#Install - Only install the example in Windows
IF(WIN32)
	INSTALL(TARGETS playable_alpha4
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
