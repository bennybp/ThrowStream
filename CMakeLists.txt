cmake_minimum_required(VERSION 2.6)

include_directories(${CMAKE_SOURCE_DIR})

add_definitions("-Wall")

option(EXCEPTIONSOURCE "Include line, file, and function in the ThrowStream output" ON)

if (EXCEPTIONSOURCE)
  add_definitions(-DTHROWSTREAM_EXCEPTIONSOURCE)
endif (EXCEPTIONSOURCE)

add_executable(ThrowStream_example examples/ThrowStream_example)
