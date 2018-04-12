# Copyright (c) Michael Heilmann 2018

# Minimum required CMake version.
cmake_minimum_required(VERSION 3.8)

# Project.
project (Nucleus.Audio.Library C)
message("building Nucleus.Audio.Library")

if (NUCLEUS_PATH)
  include(${NUCLEUS_PATH}/buildsystem/detect_compiler_and_platform.cmake)
  include(${NUCLEUS_PATH}/buildsystem/set_default_project_properties.cmake)
else()
  include(${CMAKE_CURRENT_SOURCE_DIR}/../buildsystem/detect_compiler_and_platform.cmake)
  include(${CMAKE_CURRENT_SOURCE_DIR}/../buildsystem/set_default_project_properties.cmake)
endif()

# Find and configure source files.
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}" "src/*.c")
# Find and configure header files.
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}" "src/*.h")
set_source_files_properties(${HEADER_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)
# Find and configure inlay files.
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}" "src/*.i")
set_source_files_properties(${INLAY_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)
# Define and configure the library.
add_library(Nucleus.Audio.Library STATIC ${SOURCE_FILES} ${HEADER_FILES} ${INLAY_FILES})
target_link_libraries(Nucleus.Audio.Library Nucleus.Library)
target_include_directories(Nucleus.Audio.Library PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(Nucleus.Audio.Library INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/src")
# Set its properties.
set_project_default_properties(Nucleus.Audio.Library)

# Integrate libsndfile.
# Build libsndfile using the dependencies for MSVC.
# Under Linux, things just work.
if (WIN32)
    target_link_libraries(Nucleus.Audio.Library sndfile-static)
else()
    find_path(SNDFILE_INCLUDE_DIRS
    NAMES
      sndfile.h
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
    )
    find_library(SNDFILE_LIBRARIES
    NAMES
      sndfile
    PATHS
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib
    )
target_include_directories(Nucleus.Audio.Library PRIVATE ${SNDFILE_INCLUDE_DIRS})
target_link_libraries(Nucleus.Audio.Library ${SNDFILE_LIBRARIES})
endif()

# Integrate lame.
if (WIN32)
    target_link_libraries(Nucleus.Audio.Library mp3lame)
else()
    find_path(LAME_INCLUDE_DIRS
    NAMES
      lame.h
    PATHS
      /usr/include/
      /usr/include/lame
      /usr/local/include
      /usr/local/include/lame
      /opt/local/include
      /opt/local/include/lame
      /sw/include
    )
    find_library(LAME_LIBRARIES
    NAMES
      mp3lame
    PATHS
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib
    )
target_include_directories(Nucleus.Audio.Library PRIVATE ${LAME_INCLUDE_DIRS})
target_link_libraries(Nucleus.Audio.Library ${LAME_LIBRARIES})
endif()

IF(DOXYGEN_FOUND)
    ADD_CUSTOM_TARGET(Nucleus.Audio.Library.Documentation ${DOXYGEN_EXECUTABLE} COMMENT "Building documentation")
ELSE(DOXYGEN_FOUND)
    MESSAGE(STATUS "Doxygen not found. You won't be able to build documentation.")
ENDIF(DOXYGEN_FOUND)

