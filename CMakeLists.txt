cmake_minimum_required(VERSION 2.5 fatal_error)
project( NEURONMATCH )

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_BUILD_TYPE "DEBUG")
if(CMAKE_BUILD_TYPE strequal "DEBUG")
	set(CMAKE_CXX_FLAGS " -g -o0 -wall")
	set(CMAKE_VERBOSE_MAKEFILE ON)
else(cmake_build_type strequal "debug")
	set(CMAKE_CXX_FLAGS " -o3 ")
	set(CMAKE_VERBOSE_MAKEFILE OFF)
endif(CMAKE_BUILD_TYPE strequal "DEBUG")

add_subdirectory(Code)

set(DATA_DIR ${CMAKE_SOURCE_DIR}/Data)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/Code)
set(DOC_DIR ${CMAKE_SOURCE_DIR}/Doc)

enable_testing()

