project(main)

# add io/view
add_subdirectory(io)
add_subdirectory(zbuffer)

add_executable(render main.cpp)
target_link_libraries(render  io 
                              zbuffer
                              ${OPENGL_LIBRARIES} 
                              ${GLUT_LIBRARY})
