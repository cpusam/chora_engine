SET(COMPILE_TO_WINDOWS FALSE)
SET(RELEASE FALSE)
SET(DEBUG TRUE)
IF (CMAKE_VERSION VERSION_LESS "3.1")
	IF (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
	ENDIF (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
ELSE (CMAKE_VERSION VERSION_LESS "3.1")
	SET (CMAKE_CXX_STANDARD 11)
	SET(CMAKE_CXX_STANDARD_REQUIRED ON)
	#add_compile_options("-std=c++11")
ENDIF (CMAKE_VERSION VERSION_LESS "3.1")


cmake_minimum_required(VERSION 2.8)
PROJECT(chora CXX)

file(GLOB_RECURSE SRCS src/*.cpp game/*.cpp gui/*.cpp platform/*.cpp SDL_gfx/*.cpp sound/*.cpp)
file(GLOB_RECURSE HDRS Chora.hpp sdl.hpp include/*.hpp game/*.hpp gui/*.hpp platform/*.hpp SDL_gfx/*.hpp sound/*.hpp)

include_directories(. include)
add_library(chora ${HDRS} ${SRCS})


SET(COMPILE_TO_WINDOWS FALSE)

IF (WIN32 OR COMPILE_TO_WINDOWS)
	set(CMAKE_SYSTEM_NAME Windows)
	set(TOOLCHAIN_PREFIX /usr/bin/x86_64-w64-mingw32)

	# cross compilers to use for C, C++ and Fortran
	set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
	set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
	set(CMAKE_Fortran_COMPILER ${TOOLCHAIN_PREFIX}-gfortran)
	set(CMAKE_RC_COMPILER ${TOOLCHAIN_PREFIX}-windres)

	# target environment on the build host system
	set(CMAKE_FIND_ROOT_PATH /usr/${TOOLCHAIN_PREFIX})

	# modify default behavior of FIND_XXX() commands
	set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
	set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
	set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

	include_directories(SDL2/include)
	add_definitions("-L./SDL2/lib/")
	target_link_libraries(chora SDL2main SDL2 SDL2_ttf SDL2_image SDL2_mixer)
ENDIF(WIN32 OR COMPILE_TO_WINDOWS)

IF (UNIX AND NOT COMPILE_TO_WINDOWS)
	target_link_libraries(chora SDL2 SDL2_ttf SDL2_image SDL2_mixer)
ENDIF(UNIX AND NOT COMPILE_TO_WINDOWS)


