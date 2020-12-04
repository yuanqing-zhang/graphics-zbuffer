set(ZBUFFER "zbuffer")
aux_source_directory("." ZBUFFER_SRC)
add_library(${ZBUFFER} SHARED ${ZBUFFER_SRC})