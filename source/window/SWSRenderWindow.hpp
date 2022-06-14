#ifndef SPECTRE_WINDOWING_SYSTEM_RENDER_WINDOW
#define SPECTRE_WINDOWING_SYSTEM_RENDER_WINDOW

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include "../vulkan/SVRenderer.hpp"
#include "SWSWidget.hpp"

class SWSRenderWindow{
private:
    GLFWwindow  *_window;
    SVRenderer  *_renderer;

public:
    void create(SWSWidgetInfo &info);
    bool shouldClose();
    void loadSurface(VkInstance &instance, VkSurfaceKHR &surface);
};

#endif