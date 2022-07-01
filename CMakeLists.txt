cmake_minimum_required(VERSION 3.21)
project(Spectre)

set(CMAKE_CXX_STANDARD 23)

add_executable(Spectre source/window/SWSWidget.cpp source/main.cpp source/window/SWSWindow.cpp source/window/SWSContainer.cpp source/window/SWSDrawable.cpp source/window/SWSLabel.cpp source/window/SWSButton.cpp source/vulkan/SVWidget.cpp source/vulkan/SVRenderer.cpp source/vulkan/SVSwapchain.cpp source/vulkan/SVPipeline.cpp source/vulkan/SVUtils.cpp source/vulkan/SVMesh3D.cpp source/vulkan/SVTexture.cpp source/logging/Console.cpp source/conf.cpp source/window/SWSRenderWindow.cpp source/window/SWSRenderButton.cpp)

set(LOCAL_GLFW_PATH C:/Users/Travail/Documents/glfw-3.3.7)

if(UNIX)
    find_package(X11 REQUIRED)
    target_include_directories(${PROJECT_NAME} PUBLIC ${X11_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES})
    target_include_directories(${PROJECT_NAME} PUBLIC ${X11_xcb_INCLUDE_PATH})
    target_link_libraries(${PROJECT_NAME} ${X11_xcb_LIB})
else()
#    find_package(glfw3 3.3 REQUIRED HINT $ENV{GLFW_SDK}/lib-mingw-w64)
    target_include_directories(${PROJECT_NAME} PUBLIC ${LOCAL_GLFW_PATH}/include)
    target_link_directories(${PROJECT_NAME} PUBLIC C:/Users/Travail/Documents/glfw-3.3.7/lib-mingw-w64)
    target_link_libraries(${PROJECT_NAME} glfw3)
endif()

find_package(Vulkan REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)
