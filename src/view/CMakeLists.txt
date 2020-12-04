set(VIEW "view")
aux_source_directory("." VIEW_SRC)
add_library(${VIEW} SHARED ${VIEW_SRC})