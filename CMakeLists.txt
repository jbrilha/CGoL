cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER "g++")
# set(CMAKE_CXX_EXTENSIONS OFF)

# add_compile_options(-Wall -Wextra -Wpedantic)
add_compile_options(-Wall)

project(cgol LANGUAGES C CXX VERSION 1.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/build/debug)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})

add_subdirectory(includes)
add_executable(cgol src/main.cpp src/simulation.cpp src/automaton.cpp src/life.cpp src/seeds.cpp src/brain.cpp src/disease.cpp)

target_link_libraries(cgol ${OpenGL_gl_LIBRARY} glfw glad)
include_directories(${CMAKE_SOURCE_DIR}/includes)
