FILE(GLOB INTERFACES *.i)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../)

set(CMAKE_SWIG_FLAGS "-c++")
SET(CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set_source_files_properties(${INTERFACES} PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(${INTERFACES} PROPERTIES CMAKE_SWIG_FLAGS "-includeall")

swig_add_module( theme lua theme.i )
swig_link_libraries( theme ${LUA_LIBRARY} )

swig_add_module( events lua events.i )
swig_link_libraries( events ${LUA_LIBRARY} )

swig_add_module( color lua color.i )
swig_link_libraries( color ${LUA_LIBRARY} )

swig_add_module( widgets lua widgets.i )
swig_link_libraries( widgets ${LUA_LIBRARY} )
