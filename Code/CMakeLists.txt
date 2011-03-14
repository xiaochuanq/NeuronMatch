#add_subdirectory(Math)

add_executable (loadneuron loadneuron.cpp neuron.cpp)
#add_executable (nclassify nclassify.cpp)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)