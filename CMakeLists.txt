SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

cmake_minimum_required(VERSION 2.6)

project(Testing CXX C)

find_package(Freetype)
find_package(SDL)
find_package(OpenGL)
find_package(GLEW)
find_package(DevIL)
find_package(Lua51)
find_package(SWIG)

include(${SWIG_USE_FILE})

if( NOT SDL_FOUND )
	message( FATAL_ERROR "SDL not Found" )
endif(NOT SDL_FOUND)

add_definitions( -DGL_GLEXT_PROTOTYPES -D_DEBUG_ -DILUT_USE_OPENGL )

add_subdirectory(src)
