cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER "g++")

add_compile_options(-Wall -Wextra -Wpedantic)

project(cgol LANGUAGES C CXX VERSION 1.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/build/debug)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

find_package(Freetype REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/includes
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/automata
    ${CMAKE_SOURCE_DIR}/src/text
    ${CMAKE_SOURCE_DIR}/src/ui
)

add_subdirectory(includes)
add_subdirectory(utils)

file(GLOB SOURCES_SRC "src/*.cpp")
file(GLOB SOURCES_AUTO "src/automata/*.cpp")
file(GLOB SOURCES_MENU "src/ui/*.cpp")
file(GLOB SOURCES_TEXT "src/text/*.cpp")
list(APPEND SOURCES ${SOURCES_SRC})
list(APPEND SOURCES ${SOURCES_AUTO})
list(APPEND SOURCES ${SOURCES_MENU})
list(APPEND SOURCES ${SOURCES_TEXT})
# message(STATUS "SOURCES: ${SOURCES}")

add_executable(cgol ${SOURCES})

target_link_libraries(cgol PRIVATE ${OpenGL_gl_LIBRARY} ${FREETYPE_LIBRARIES} glfw glad utils)
target_include_directories(cgol PRIVATE ${FREETYPE_INCLUDE_DIRS})
