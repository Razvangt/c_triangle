cmake_minimum_required(VERSION 3.23)
## add on environment variable
project(c_triangle)
set(CMAKE_C_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#################################################################################################
###      FIND LIBRARYS
#################################################################################################
find_package(Vulkan REQUIRED) # throws error if could not find Vulkan
message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")

find_package(glib-2.0 REQUIRED HINTS ${GLIB_DIR})
#################################################################################################
###      LINKING PART
#################################################################################################

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.c)

## Execution
add_executable(c_triangle main.c ${SOURCES})

## normal library
target_include_directories(c_triangle PUBLIC ${Vulkan_INCLUDE_DIRS} 
	${GLFW_INCLUDE_DIRS} 
	${CGLM_INCLUDE_DIR}
	${GLIB_DIR}
	${PROJECT_SOURCE_DIR}/src
)

target_link_directories(c_triangle PUBLIC  
	${VULKAN_LIBRARIES} 
	${GLFW_LIBRARIES} 
	${CGLM_LIB}
	${GLIB_LIB}
) 
target_link_libraries(c_triangle PUBLIC glib-2.0.lib cglm glfw3 ${Vulkan_LIBRARIES})

