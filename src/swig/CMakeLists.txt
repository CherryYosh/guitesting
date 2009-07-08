FILE(GLOB INTERFACES *.i)

set(CMAKE_SWIG_FLAGS "-c++")

set_source_files_properties(${INTERFACES} PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(${INTERFACES} PROPERTIES iCMAKE_SWIG_FLAGS "-includeall")

swig_add_module( ../../theme lua theme.i )
swig_add_module( ../../events lua events.i )
swig_add_module( ../../color lua color.i )
swig_add_module( ../../widgets lua widgets.i )
