# Copyright (c) Michael Heilmann 2018

# Add sub-project 'nucleus' with appropriate options.
SET(With-Tests OFF CACHE BOOL
    "enable/disable compilation and execution of unit tests. \
     ON enables compilation and execution of unit tests, OFF disables compilation and execution of unit tests. \
     Initial value is ON.")
SET(With-Static-Runtime OFF CACHE BOOL
    "enable/disable compilation with static runtime. \
     ON enables compilation with static runtime, OFF disables compilation with static runtime. \
     Initial value is ON. \
     Only available under MSVC and MINGW")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/nucleus)

# Add sub-project 'libmp3lame'.
if (WIN32)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libmp3lame)
endif()

# Add sub-project 'libsndfile'.
if (WIN32)
  set(ENABLE_STATIC_RUNTIME OFF CACHE BOOL "Enable static runtime" FORCE)
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
  set(BUILD_TESTING OFF CACHE BOOL "Build tests" FORCE)
  set(BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
  set(BUILD_PROGRAMS OFF CACHE BOOL "Build programs" FORCE)
  set(DISABLE_EXTERNAL_LIBS ON CACHE BOOL "Disable use of FLAC, Ogg and Vorbis")
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libsndfile)
endif()