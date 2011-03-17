include_directories( ${HEADER_DIR}/OgreMath )
aux_source_directory(. LIBOGREMATH_SRC_LIST)
add_library(ogremath STATIC ${LIBOGREMATH_SRC_LIST}) 