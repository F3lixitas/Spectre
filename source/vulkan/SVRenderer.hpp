#ifndef SPECTRE_VULKAN_RENDERER
#define SPECTRE_VULKAN_RENDERER

#include "SVWidget.hpp"
#include "SVSwapchain.hpp"
#include <vector>

class SVRenderer {
private:
    SVWidget*       _widget;
    SVSwapchain     _swapchain;

    //////// VULKAN FRAMEWORK ATTRIBUTES ////////
    VkInstance              _instance;
    VkSurfaceKHR            _surface;
    VkPhysicalDevice*       _physicalDevices;
    uint32_t                _amountOfPhysicalDevices;
    VkDevice                _device;
    VkQueue                 _queue;
    VkImageView*            _imageViews;
    VkRenderPass            _renderPass;
    VkDescriptorSetLayout   _descriptorSetLayout;
    VkDescriptorPool        _descriptorPool;

    uint32_t            _queueFamilyIndex;
    uint32_t            _amountOfSwapchainImages;
    VkColorSpaceKHR     _displayImageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    VkFormat            _displayImageFormat     = VK_FORMAT_B8G8R8A8_SRGB;
    VkExtent2D          _displaySize;

    /////////// VULKAN SETUP METHODS ///////////
    void createInstance();
    void initSurface();
    void initPhysicalDevices();
    void createLogicalDevice();
    void createSwapchain();
    void createImageViews();
    void createRenderPass();
    void createDescriptorSetLayout();
    void initPipeline();
    void createDescriptorPool();

    void createShaderModule(const std::vector<char>& code, VkShaderModule& shaderModule);
public:
    SVRenderer(SVWidget *widget = nullptr);
    ~SVRenderer();
    void init();
    void destroy();
};

#endif