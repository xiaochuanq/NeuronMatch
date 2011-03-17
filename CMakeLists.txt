cmake_minimum_required (VERSION 2.6)
project( NEURONMATCH )

set(DATA_DIR ${CMAKE_SOURCE_DIR}/Data)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/Code)
set(DOC_DIR ${CMAKE_SOURCE_DIR}/Doc)
set(TEST_DIR ${SRC_DIR}/Test)
set(HEADER_DIR ${SRC_DIR}/Include)
set(MATLAB_ROOT_DIR /Applications/MATLAB_R2010b.app)

include_directories( ${HEADER_DIR} ) # build-in
include_directories( ${MATLAB_ROOT_DIR}/extern/include ) # extern

link_directories(	${MATLAB_ROOT_DIR}/extern/lib/maci64 
					${MATLAB_ROOT_DIR}/bin/maci64 
					${MATLAB_ROOT_DIR}/sys/os/maci64 
					${PROJECT_BINARY_DIR}/bin/lib )

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_CXX_COMPILER g++ )
set(CMAKE_CXX_FLAGS " -g -O0 -wall")
set(CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory(Code bin)

enable_testing()
add_test(test_neuron loadneuron)

