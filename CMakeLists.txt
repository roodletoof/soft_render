cmake_minimum_required(VERSION 3.27)
project(softRender C)

set(CMAKE_C_STANDARD 17)

link_libraries(X11)

add_executable(softRender main.c)
