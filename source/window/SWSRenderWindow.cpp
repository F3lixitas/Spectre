#include "SWSRenderWindow.hpp"

void SWSRenderWindow::create(SWSWidgetInfo &info){
    if(glfwInit()){
        _window = glfwCreateWindow(info.sizeX, info.sizeY, "Spectre", nullptr, nullptr);
        if(!_window) glfwTerminate();

        glfwMakeContextCurrent(_window);

        _renderer = new SVRenderer(this);
        _renderer->init();
    }
}

void SWSRenderWindow::loadSurface(VkInstance &instance, VkSurfaceKHR &surface){
    glfwCreateWindowSurface(instance, _window, nullptr, &surface);
}

bool SWSRenderWindow::shouldClose() {
    return glfwWindowShouldClose(_window);
}

