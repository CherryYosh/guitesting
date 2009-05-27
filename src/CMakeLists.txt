FILE(GLOB SRC *.cpp *.h)
FILE(GLOB OSRC */*.cpp */*.h */*/*.cpp */*/*.h)

add_subdirectory(swig)

include_directories(${SDL_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIR}
${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${IL_INCLUDE_DIR} ${LUA_INCLUDE_DIR} "/usr/include/freetype2")

add_executable(test ${SRC} ${OSRC})

target_link_libraries(test ${SDL_LIBRARIE} ${OPENGL_LIBRARY} ${IL_LIBRARY}
	${ILU_LIBRARY} ${ILUT_LIBRARY} ${FREETYPE_LIBRARY} ${GLEW_LIBRARY}
	${LUA_LIBRARY} SDLmain )
