cmake_minimum_required(VERSION 2.6)
project(chora)

file(GLOB_RECURSE SRCS src/*.cpp game/*.cpp gui/*.cpp platform/*.cpp SDL_gfx/*.cpp sound/*.cpp)
file(GLOB_RECURSE HDRS Chora.hpp sdl.hpp include/*.hpp game/*.hpp gui/*.hpp platform/*.hpp SDL_gfx/*.hpp sound/*.hpp)

include_directories(. include game gui platform SDL_gfx sound)

add_library(chora ${HDRS} ${SRCS})
add_definitions("--std=c++11")
target_link_libraries(chora SDL2 SDL2_ttf SDL2_image SDL2_mixer)