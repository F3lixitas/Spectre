#ifndef SPECTRE_WINDOWING_SYSTEM_RENDER_WINDOW
#define SPECTRE_WINDOWING_SYSTEM_RENDER_WINDOW

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include "../vulkan/SVRenderer.hpp"

typedef struct vec2i{
    int x;
    int y;
}vec2i;

typedef struct SWSWidgetInfo {
    int         offsetX = 0;
    int         offsetY = 0;
    int         sizeX = 100;
    int         sizeY = 100;
    uint32_t    flags = 0;
    void (*onCreate)(int,int) = nullptr;
    void (*onDestroy)(int, int) = nullptr;
} SWSWidgetInfo;

class SWSRenderWindow{
private:
    GLFWwindow  *_window;
    SVRenderer  *_renderer;
public:
    void create(SWSWidgetInfo &info);
    bool shouldClose();
    void loadSurface(VkInstance &instance, VkSurfaceKHR *surface);
    void proc();
    SVRenderer* getRenderer(){return _renderer;}
    vec2i getSize(){
        int x, y;
        glfwGetWindowSize(_window, &x, &y);
        return {x, y};
    }
    void destroy();
};

#endif