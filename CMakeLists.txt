cmake_minimum_required(VERSION 3.16)
project(sentinel_swarm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/sentinel-sim-core)
add_subdirectory(external/sentinel-orchestra)

add_executable(sentinel-swarm
    src/main.cpp
    src/swarm_update.cpp
)

target_include_directories(sentinel-swarm PRIVATE
    include
)

target_link_libraries(sentinel-swarm
    sentinel-sim-core
    sentinel-orchestra
)
