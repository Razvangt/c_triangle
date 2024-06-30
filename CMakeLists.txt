cmake_minimum_required(VERSION 3.23)
project(c_tringle C)

set(CMAKE_C_STANDARD 17)

add_executable(c_tringle src/main.c)
target_link_libraries(c_tringle glfw vulkan)
