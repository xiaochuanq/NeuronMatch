add_executable (loadneuron loadneuron.cpp ${SRC_DIR}/neuron.cpp)
add_executable (testmm testmm.cpp ${SRC_DIR}/dataio.cpp ${SRC_DIR}/desc.cpp)
add_executable (testmatrix testmatrix.cpp)
add_executable (createdesc createdesc.cpp ${SRC_DIR}/dataio.cpp ${SRC_DIR}/neuron.cpp ${SRC_DIR}/desc.cpp )

target_link_libraries( loadneuron ogremath)
target_link_libraries( testmatrix ogremath )
target_link_libraries( testmm ogremath mx mex mat )
target_link_libraries( createdesc ogremath mx mex mat )
