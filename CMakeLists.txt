cmake_minimum_required(VERSION 3.15)
project(arclib VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE LIB_HEADERS "include/arclib/*.h")
file(GLOB_RECURSE LIB_SOURCES "source/*.cpp")
add_library(arclib ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(arclib PUBLIC include/)

enable_testing()
add_subdirectory(tests)
