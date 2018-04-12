# Copyright (c) Michael Heilmann 2018

# Minimum required CMake version.
cmake_minimum_required(VERSION 3.8)

# Project.
project(Nucleus.Audio)
message("building Nucleus.Audio")

include(CTest)
enable_testing()

set(NUCLEUS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/nucleus)
include(${NUCLEUS_PATH}/buildsystem/detect_compiler_and_platform.cmake)
include(${NUCLEUS_PATH}/buildsystem/set_default_project_properties.cmake)

# Add sub-project 'dependencies'.
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies)

# Add sub-project 'library'.
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/library)

# Add sub-project 'demo'.
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/demo)
