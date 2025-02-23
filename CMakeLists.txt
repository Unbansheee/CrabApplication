cmake_minimum_required(VERSION 3.0...3.25)
project(
    CrabEditor # name of the project, which will also be the name of the visual studio solution if you use it
    VERSION 0.1.0 # any version number
    LANGUAGES CXX C # programming languages used by the project
)

file(GLOB SOURCES
        src/*.h
        src/*.c
        src/*.hpp
        src/*.cpp
)

add_executable(CrabApplication
        ${SOURCES}
)

set_target_properties(CrabApplication PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    COMPILE_WARNING_AS_ERROR OFF
)

if (MSVC)
    target_compile_options(CrabApplication PRIVATE /W4)
    # Disable warning C4244: conversion from 'int' to 'short', possible loss of data
    target_compile_options(CrabApplication PUBLIC /wd4244)
    target_compile_options(CrabApplication PUBLIC /MP)
else()
    target_compile_options(CrabApplication PRIVATE -Wall -Wextra -pedantic)
endif()

add_subdirectory(./vendor/CrabEngine)
target_link_libraries(CrabApplication PUBLIC CrabEngine)

# We add an option to enable different settings when developing the CrabApplication than
# when distributing it.
option(DEV_MODE "Set up development helper settings" ON)

if(DEV_MODE)
    # In dev mode, we load resources from the source tree, so that when we
    # dynamically edit resources (like shaders), these are correctly
    # versionned.
    target_compile_definitions(CrabApplication PRIVATE
            RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resources"
    )
else()
    # In release mode, we just load resources relatively to wherever the
    # executable is launched from, so that the binary is portable
    target_compile_definitions(CrabApplication PRIVATE
            RESOURCE_DIR="./resources"
    )
endif()


target_include_directories(CrabApplication PRIVATE .)
target_include_directories(CrabApplication PRIVATE ./vendor)
target_include_directories(CrabApplication PRIVATE ./src)

