#ifndef SPECTRE_VULKAN_RENDERER
#define SPECTRE_VULKAN_RENDERER

#include "../conf.hpp"

#include "SVWidget.hpp"
#include "SVSwapchain.hpp"
#include "SVPipeline.hpp"
#include "SVMesh3D.hpp"
#include "SVTexture.hpp"
#include <vector>
#include "../core/SC_Material.hpp"

#include <GLFW/glfw3.h>

class SWSRenderWindow;

class SVRenderer {
private:
    SVWidget*               _widget;
    SWSRenderWindow*        _window;
    SVSwapchain             _swapchain;
    std::vector<SVPipeline> _pipelines;

    //////// VULKAN FRAMEWORK ATTRIBUTES ////////
    VkInstance              _instance;
    VkSurfaceKHR            _surface;
    VkPhysicalDevice*       _physicalDevices; //
    VkPhysicalDevice*       _physicalDevice;
    uint32_t                _amountOfPhysicalDevices;
    VkDevice                _device;
    VkQueue                 _queue;
    VkImageView*            _imageViews; //
    VkRenderPass            _renderPass;
    VkDescriptorSetLayout   _descriptorSetLayout;
    VkFramebuffer*          _framebuffers; //
    VkCommandPool           _commandPool;
    VkDescriptorSet         _descriptorSet;
    VkDescriptorPool        _descriptorPool;
    VkCommandBuffer*        _commandBuffers; //
    VkSemaphore             _semaphoreBegin;
    VkSemaphore             _semaphoreEnd;

    uint32_t                _queueFamilyIndex;
    uint32_t                _amountOfSwapchainImages;
    VkColorSpaceKHR         _displayImageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    VkFormat                _displayImageFormat     = VK_FORMAT_B8G8R8A8_SRGB;
    VkExtent2D              _displaySize;

    std::vector<SVMesh3D>       _mesh;
    SVTexture                   _texture;
    std::vector<SC_Material>    _materials;

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
    void initFramebuffers();
    void initCommandPool();
    void createDescriptorPool();
    void createCommand();
    void initSemaphore();

    void createTextureView();

    void updateRenderingCommands();
    void createShaderModule(const std::vector<char>& code, VkShaderModule& shaderModule);
public:
    SVRenderer(SVWidget *widget = nullptr);
    SVRenderer(SWSRenderWindow *window);
    ~SVRenderer();
    void init();
    void destroy();

    void addMeshData(std::vector<SVVertex3D>& vertices, std::vector<uint32_t>& indices);
    void removeMeshData(uint32_t index);
    void render();

};

#endif