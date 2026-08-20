cmake_minimum_required(VERSION 3.16)

project(OchengineBrickBreaker3D LANGUAGES CXX)

include("${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/OchengineClean.cmake")
ochengine_enable_clean("${CMAKE_CURRENT_SOURCE_DIR}")

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(brick_breaker_3d brick_breaker_3d.cpp)

target_compile_features(brick_breaker_3d PRIVATE cxx_std_11)
target_include_directories(brick_breaker_3d PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
target_link_libraries(brick_breaker_3d PRIVATE sfml-graphics sfml-window sfml-system)
