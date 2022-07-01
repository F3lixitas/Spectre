#include "SWSRenderWindow.hpp"
#include <iostream>
#include <vector>

void SWSRenderWindow::create(SWSWidgetInfo &info){
    if(glfwInit()){
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        _window = glfwCreateWindow(info.sizeX, info.sizeY, "Spectre", nullptr, nullptr);
        glfwSetWindowPos(_window, info.offsetX, info.offsetY);
        if(!_window) glfwTerminate();

        if(!glfwVulkanSupported()) std::cout << "Error vulkan not supported !\n";
        _renderer = new SVRenderer(this);
        _renderer->init();
//        std::vector<SVVertex3D> vertices = {{{0.5, 0.5, 0},  {1, 0, 0}, {0, 0}},
//                                            {{-0.5, 0.5, 0}, {0, 1, 0}, {0, 0}},
//                                            {{0.5, -0.5, 0}, {0, 0, 1}, {0, 0}}};
//        std::vector<uint32_t> indices = {0, 1, 2};
//        _renderer->addMeshData(vertices, indices);
    }
}

void SWSRenderWindow::loadSurface(VkInstance &instance, VkSurfaceKHR *surface){
    VkResult res = glfwCreateWindowSurface(instance, _window, nullptr, surface);
    if(res != VK_SUCCESS) std::cout << "Error creating surface !\n";
}

bool SWSRenderWindow::shouldClose() {
    return glfwWindowShouldClose(_window);
}

void SWSRenderWindow::proc(){
    _renderer->render();
    glfwPollEvents();
}

void SWSRenderWindow::destroy(){
    _renderer->destroy();
    delete _renderer;
    glfwTerminate();
}

