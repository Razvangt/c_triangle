cmake_minimum_required(VERSION 3.23)

set(CMAKE_C_COMPILER   "/usr/bin/clang")
project(c_triangle C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_executable(c_triangle main.c)

add_subdirectory(src)


target_link_libraries(c_triangle app glfw vulkan)
