cmake_minimum_required(VERSION 3.16)
project(sentinel_swarm LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

add_executable(sentinel-swarm
    src/main.cpp
    src/swarm_render_gl3.cpp
    external/glad/src/glad.c
)

target_include_directories(sentinel-swarm PRIVATE
    include
    external/glad/include
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(sentinel-swarm
    ${SDL2_LIBRARIES}
    GL
)
