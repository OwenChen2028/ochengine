cmake_minimum_required(VERSION 3.16)

project(
    Ochengine
    VERSION 0.1.0
    DESCRIPTION "A small 2D physics engine with SFML rendering"
    LANGUAGES CXX
)

option(OCHENGINE_BUILD_EXAMPLES "Build the Ochengine examples" ON)

set_property(
    DIRECTORY APPEND
    PROPERTY ADDITIONAL_CLEAN_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/examples/brick_breaker/build"
        "${CMAKE_CURRENT_SOURCE_DIR}/examples/brick_breaker_3d/build"
)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_library(ochengine INTERFACE)
add_library(ochengine::ochengine ALIAS ochengine)

target_compile_features(ochengine INTERFACE cxx_std_11)
target_include_directories(
    ochengine
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(
    ochengine
    INTERFACE
        sfml-graphics
        sfml-window
        sfml-system
)

if(OCHENGINE_BUILD_EXAMPLES)
    add_executable(simulation_demo examples/simulation_demo.cpp)
    target_link_libraries(simulation_demo PRIVATE ochengine::ochengine)

    add_executable(pong_demo examples/pong_demo.cpp)
    target_link_libraries(pong_demo PRIVATE ochengine::ochengine)
endif()
