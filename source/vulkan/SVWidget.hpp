#ifndef SPECTRE_VULKAN_WIDGET
#define SPECTRE_VULKAN_WIDGET

#include "../window/SWSWidget.hpp"

#define VK_USE_PLATFORM_XCB_KHR
#include "vulkan/vulkan.hpp"

class SVWidget : public SWSWidget {

public:
    void loadSurface(VkInstance &instance, VkSurfaceKHR &surface);

};

#endif