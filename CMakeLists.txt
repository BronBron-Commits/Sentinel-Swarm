cmake_minimum_required(VERSION 3.16)
project(sentinel_swarm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

# =========================
# Sentinel Sim Core
# =========================
add_library(simcore STATIC
    external/sentinel-sim-core/src/sim_hash.cpp
    external/sentinel-sim-core/src/sim_input_io.cpp
    external/sentinel-sim-core/src/sim_runner.cpp
    external/sentinel-sim-core/src/sim_update.cpp
)

target_include_directories(simcore PUBLIC
    external/sentinel-sim-core/include
)

# =========================
# Sentinel Orchestra
# =========================
add_subdirectory(external/sentinel-orchestra)

# =========================
# Swarm Executable
# =========================
add_executable(sentinel-swarm
    src/main.cpp
    src/swarm_update.cpp
    src/swarm_neighbors.cpp
    src/swarm_render.cpp
)

target_include_directories(sentinel-swarm PRIVATE
    include
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(sentinel-swarm
    simcore
    sentinel-orchestra-core
    ${SDL2_LIBRARIES}
    GL
)
