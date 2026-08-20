cmake_minimum_required(VERSION 3.16)

project(OchengineBrickBreaker LANGUAGES CXX)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(brick_breaker brick_breaker.cpp)

target_compile_features(brick_breaker PRIVATE cxx_std_11)
target_include_directories(brick_breaker PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
target_link_libraries(brick_breaker PRIVATE sfml-graphics sfml-window sfml-system)
