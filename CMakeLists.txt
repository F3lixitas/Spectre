cmake_minimum_required(VERSION 3.21)
project(Spectre)

set(CMAKE_CXX_STANDARD 23)

add_executable(Spectre source/window/SWSWidget.cpp source/main.cpp source/window/SWSWindow.cpp source/window/SWSContainer.cpp source/window/SWSDrawable.cpp source/window/SWSLabel.cpp source/window/SWSButton.cpp source/vulkan/SVWidget.cpp source/vulkan/SVRenderer.cpp source/vulkan/SVSwapchain.cpp source/vulkan/SVPipeline.cpp source/vulkan/SVUtils.cpp source/vulkan/SVMesh2D.cpp source/vulkan/SVTexture.cpp source/logging/Console.cpp source/conf.cpp)

if(UNIX)
    find_package(X11 REQUIRED)
    target_include_directories(${PROJECT_NAME} PUBLIC ${X11_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES})
    target_include_directories(${PROJECT_NAME} PUBLIC ${X11_xcb_INCLUDE_PATH})
    target_link_libraries(${PROJECT_NAME} ${X11_xcb_LIB})
endif()

find_package(Vulkan REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)
