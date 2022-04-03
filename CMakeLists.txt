cmake_minimum_required(VERSION 3.21)
project(vulkan_tutorial)

set(CMAKE_CXX_STANDARD 17)

# CUSTOM
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") ## add -v for verbose output
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D _DEBUG ")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D _DEBUG ")

#set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /Z7 /EHsc /Wall /analyze")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Z7 /EHsc /Wall /analyze")
# CUSTOM
set(GLFW_SDK "E:/dev/tools/glfw-3.3.6.bin.WIN64")
set(VULKAN_SDK "E:/dev/tools/Vulkan")
set(GLM_SDK "E:/dev/tools/glm")

find_package(Vulkan REQUIRED FATAL_ERROR)

if (exists ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif ()

include_directories(${VULKAN_SDK}/Include)
include_directories(${GLFW_SDK}/include)
include_directories(${GLM_SDK})

link_libraries(${VULKAN_SDK}/Lib/vulkan-1.lib)
link_libraries(${GLFW_SDK}/lib-vc2022/glfw3.lib)

file(GLOB MAIN ./src/main.cpp)
file(GLOB_RECURSE SRCFILES ./src/*.cpp)
file(GLOB_RECURSE HEADERFILES ./src/*.h)

add_executable(vulkan_tutorial ${MAIN} ${SRCFILES} ${HEADERFILES})
