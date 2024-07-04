cmake_minimum_required(VERSION 3.23)
## add on environment variable
project(c_triangle)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#################################################################################################
###      FIND LIBRARYS
#################################################################################################
find_package(Vulkan REQUIRED) # throws error if could not find Vulkan
message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")
#################################################################################################
###      LINKING PART
#################################################################################################

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

## Execution
add_executable(c_triangle main.cpp ${SOURCES})
## normal library
target_include_directories(c_triangle PUBLIC ${Vulkan_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${GLM_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/src)
target_link_directories(c_triangle PUBLIC  ${VULKAN_LIBRARIES} ${GLFW_LIBRARIES} ${GLM_LIB}) 
target_link_libraries(c_triangle PUBLIC glm glfw3 ${Vulkan_LIBRARIES})

