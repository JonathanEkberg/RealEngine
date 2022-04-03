cmake_minimum_required(VERSION 3.21)
project(vulkan_tutorial)

set(CMAKE_CXX_STANDARD 17)

# CUSTOM
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") ## add -v for verbose output
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D _DEBUG ")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D _DEBUG ")

find_package(Vulkan REQUIRED FATAL_ERROR)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

include_directories($ENV{VULKAN_SDK}/Include)
include_directories($ENV{GLFW_SDK}/include)
include_directories($ENV{GLM_SDK})

link_libraries($ENV{VULKAN_SDK}/Lib/vulkan-1.lib)
link_libraries($ENV{GLFW_LIB_DIR}/glfw3.lib)

file(GLOB MAIN ./src/main.cpp)
file(GLOB_RECURSE SRCFILES ./src/*.cpp)
file(GLOB_RECURSE HEADERFILES ./src/*.h)

add_executable(vulkan_tutorial ${MAIN} ${SRCFILES} ${HEADERFILES})
