# Copyright (c) Michael Heilmann 2017, 2018

# Minimum required CMake version.
cmake_minimum_required (VERSION 3.8)

# Project.
project(Nucleus.Audio.Demo.WavToMp3BatchConverter CXX)
message("building Nucleus.Audio.Demo.WavToMp3BatchConverter")

if (NUCLEUS_PATH)
  include(${NUCLEUS_PATH}/buildsystem/detect_compiler_and_platform.cmake)
  include(${NUCLEUS_PATH}/buildsystem/set_default_project_properties.cmake)
else()
  include(${CMAKE_CURRENT_SOURCE_DIR}/../buildsystem/detect_compiler_and_platform.cmake)
  include(${CMAKE_CURRENT_SOURCE_DIR}/../buildsystem/set_default_project_properties.cmake)
endif()

# Find and configure source files.
#file(GLOB_RECURSE C_SOURCE_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "src/*.c")
file(GLOB_RECURSE CPP_SOURCE_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "src/*.cpp")
# Find and configure header files.
#file(GLOB_RECURSE H_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "src/*.h")
file(GLOB_RECURSE HPP_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "src/*.hpp")
set_source_files_properties(${HEADER_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)
# Add and configure executable.
add_executable(Nucleus.Audio.Demo.WavToMp3BatchConverter ${C_SOURCE_FILES} ${H_FILES} ${CPP_SOURCE_FILES} ${HPP_FILES})
# TODO: CMake 10 or above provides a better solution.
if(${NUCLEUS_CPP_COMPILER_ID} EQUAL ${NUCLEUS_CPP_COMPILER_ID_MSVC})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif()
if (${NUCLEUS_CPP_COMPILER_ID} EQUAL ${NUCLEUS_CPP_COMPILER_ID_GCC})
  add_compile_options(-std=gnu++1z -D_GNU_SOURCE) # Applies to this directory and its subdirectories.
endif()
target_include_directories(Nucleus.Audio.Demo.WavToMp3BatchConverter PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(Nucleus.Audio.Demo.WavToMp3BatchConverter Nucleus.Audio.Library)
# Set its properties.
set_project_default_properties(Nucleus.Audio.Demo.WavToMp3BatchConverter)
