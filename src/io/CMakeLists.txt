set(IO "io")
aux_source_directory("." IO_SRC)
add_library(${IO} SHARED ${IO_SRC})