cmake_minimum_required(VERSION 3.21)
project(Spectre)

set(CMAKE_CXX_STANDARD 23)


add_executable(Spectre main.cpp)

find_package(Vulkan REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)
