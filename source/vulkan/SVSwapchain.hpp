#ifndef SPECTRE_VULKAN_SWAPCHAIN
#define SPECTRE_VULKAN_SWAPCHAIN

#include <vulkan/vulkan.hpp>

class SVSwapchain {
private:
VkSwapchainKHR _swapchain;

public:
    void init(VkDevice& logicalDevice, VkPhysicalDevice* physicalDevice, VkSurfaceKHR& surface, VkRect2D displayDimensions,
              VkFormat displayFormat, VkColorSpaceKHR colorSpace);

    void destroy(VkDevice& device){
        vkDestroySwapchainKHR(device, _swapchain, nullptr);
    }

    VkSwapchainKHR& getSwapchain(){
        return _swapchain;
    }
};

#endif