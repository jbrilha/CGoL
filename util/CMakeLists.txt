file(GLOB UTIL_HEADERS "*.hpp")

add_library(util INTERFACE)

target_include_directories(util INTERFACE ${CMAKE_SOURCE_DIR})

# file(GLOB UTIL_SOURCES "*.cpp")
# add_library(util STATIC ${UTIL_SOURCES})
