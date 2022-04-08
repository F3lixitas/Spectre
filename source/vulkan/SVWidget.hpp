#ifndef SPECTRE_VULKAN_WIDGET
#define SPECTRE_VULKAN_WIDGET

#include "../window/SWSWidget.hpp"

#if defined __linux__ || __APPLE__
    #define VK_USE_PLATFORM_XCB_KHR
#elif defined _WIN32
    #define VK_USE_PLATFORM_WIN32_KHR
#endif
#include "vulkan/vulkan.hpp"

class SVWidget : public SWSWidget {

public:
    void loadSurface(VkInstance &instance, VkSurfaceKHR &surface);
};

#endif