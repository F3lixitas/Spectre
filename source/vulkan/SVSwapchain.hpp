#ifndef SPECTRE_VULKAN_SWAPCHAIN
#define SPECTRE_VULKAN_SWAPCHAIN

#include <vulkan/vulkan.hpp>

class SVSwapchain {
private:
    VkSwapchainKHR  _swapchain;
    VkRenderPass    _renderPass;

    VkDevice*       _device;

    void createRenderPass();

public:
    SVSwapchain(VkDevice* device);

    void init(VkPhysicalDevice* physicalDevice, VkSurfaceKHR& surface, VkRect2D displayDimensions,
              VkFormat displayFormat, VkColorSpaceKHR colorSpace);

    void destroy(VkDevice& device){
        vkDestroyRenderPass(device, _renderPass, nullptr);
        vkDestroySwapchainKHR(device, _swapchain, nullptr);
    }

    void beginRenderPass(VkCommandBuffer& commandBuffer, VkFramebuffer& framebuffer, VkExtent2D size);

    VkSwapchainKHR& getSwapchain(){
        return _swapchain;
    }

    VkRenderPass& getRenderPass(){
        return _renderPass;
    }
};

#endif