#include "SVRenderer.hpp"
#include <iostream>
#include "../utils/SOpenFile.hpp"

void SVRenderer::createShaderModule(const std::vector<char> &code, VkShaderModule &shaderModule) {
    VkShaderModuleCreateInfo shaderInfo;
    shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderInfo.pNext = nullptr;
    shaderInfo.flags = 0;
    shaderInfo.codeSize = code.size();
    shaderInfo.pCode = (uint32_t*)code.data();

    vkCreateShaderModule(_device, &shaderInfo, nullptr, &shaderModule);
}

void SVRenderer::createInstance() {
    VkApplicationInfo appInfo;
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr;
    appInfo.apiVersion = VK_API_VERSION_1_2;
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
    appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 0);
    appInfo.pApplicationName = "Spectre";
    appInfo.pEngineName = "Spectre Engine";

    std::vector<const char*> layers = {"VK_LAYER_KHRONOS_validation"};

    const char* extensions[] = {"VK_KHR_xlib_surface", "VK_KHR_surface"};

    VkInstanceCreateInfo instCreateInfo;
    instCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instCreateInfo.pNext = nullptr;
    instCreateInfo.flags = 0;
    instCreateInfo.enabledExtensionCount = 2;
    instCreateInfo.enabledLayerCount = layers.size();
    instCreateInfo.pApplicationInfo = &appInfo;
    instCreateInfo.ppEnabledExtensionNames = extensions;
    instCreateInfo.ppEnabledLayerNames = layers.data();

    vkCreateInstance(&instCreateInfo, nullptr, &_instance);
}

void SVRenderer::initSurface() {
    _widget->loadSurface(_instance, _surface);
}

void SVRenderer::initPhysicalDevices() {
    vkEnumeratePhysicalDevices(_instance, &_amountOfPhysicalDevices, nullptr);
    _physicalDevices = new VkPhysicalDevice[_amountOfPhysicalDevices];
    vkEnumeratePhysicalDevices(_instance, &_amountOfPhysicalDevices, _physicalDevices);

    for(int i = 0; i < _amountOfPhysicalDevices; i++){
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(_physicalDevices[i], &props);
        std::cout << "api version : " << props.apiVersion << "\ndriver version : " << props.driverVersion
                  << "\nvendor ID : " << props.vendorID << "\ndevice ID : " << props.deviceID << "\ndevice type : "
                  << props.deviceType << "\ndevice name : " << props.deviceName << "\npipeline cache UUID : "
                  << props.pipelineCacheUUID << "\n\n";
    }

}

void SVRenderer::createLogicalDevice() {
    VkPhysicalDeviceFeatures physicalDevFeatures = {};
    physicalDevFeatures.samplerAnisotropy = VK_TRUE;
    vkGetPhysicalDeviceFeatures(_physicalDevices[0], &physicalDevFeatures);

    uint32_t amountOfQueues;
    vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevices[0], &amountOfQueues, nullptr);
    VkQueueFamilyProperties* queueProperties = new VkQueueFamilyProperties[amountOfQueues];
    vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevices[0], &amountOfQueues, queueProperties);

    uint32_t queueCount = 0;
    uint32_t queueIndex = 0;

    for(int i = 0; i < amountOfQueues; i++){
        if(queueProperties[i].queueCount > queueCount && queueProperties[i].queueFlags == 15){
            queueCount = queueProperties[i].queueCount;
            queueIndex = i;
        }

    }

    _queueFamilyIndex = queueIndex;

    float *queuePrio = new float[queueCount];
    for(int i = 0; i < queueCount; i++){
        queuePrio[i] = 1.0f;
    }

    VkDeviceQueueCreateInfo queueCreateInfo;
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.pNext = nullptr;
    queueCreateInfo.flags = 0;
    queueCreateInfo.pQueuePriorities = queuePrio;
    queueCreateInfo.queueCount = queueCount;
    queueCreateInfo.queueFamilyIndex = queueIndex;

    const std::vector<const char*> extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    VkDeviceCreateInfo devCreateInfo;
    devCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    devCreateInfo.pNext = nullptr;
    devCreateInfo.flags = 0;
    devCreateInfo.enabledExtensionCount = extensions.size();
    devCreateInfo.enabledLayerCount = 0;
    devCreateInfo.pEnabledFeatures = &physicalDevFeatures;
    devCreateInfo.ppEnabledExtensionNames = extensions.data();
    devCreateInfo.ppEnabledLayerNames = nullptr;
    devCreateInfo.queueCreateInfoCount = 1;
    devCreateInfo.pQueueCreateInfos = &queueCreateInfo;

    vkCreateDevice(_physicalDevices[0], &devCreateInfo, nullptr, &_device);

    vkGetDeviceQueue(_device, queueIndex, 0, &_queue);

    VkBool32 surfaceSupport;
    vkGetPhysicalDeviceSurfaceSupportKHR(_physicalDevices[0], queueIndex, _surface, &surfaceSupport);

    delete[] queueProperties;
    delete[] queuePrio;
}

void SVRenderer::createSwapchain() {
    VkSurfaceCapabilitiesKHR surfaceCapa;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_physicalDevices[0], _surface, &surfaceCapa);
    _displaySize = surfaceCapa.currentExtent;

    VkRect2D dimensions;
    dimensions.extent = _displaySize;
    dimensions.offset.x = 0;
    dimensions.offset.y = 0;
    _swapchain.init(_device, _physicalDevices, _surface, dimensions, _displayImageFormat, _displayImageColorSpace);
}

void SVRenderer::createImageViews() {
    vkGetSwapchainImagesKHR(_device, _swapchain.getSwapchain(), &_amountOfSwapchainImages, nullptr);
    VkImage *images = new VkImage[_amountOfSwapchainImages];
    vkGetSwapchainImagesKHR(_device, _swapchain.getSwapchain(), &_amountOfSwapchainImages, images);

    _imageViews = new VkImageView[_amountOfSwapchainImages];
    for (int i = 0; i < _amountOfSwapchainImages; i++) {
        VkImageViewCreateInfo imageViewCreateInfo;
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.pNext = nullptr;
        imageViewCreateInfo.flags = 0;
        imageViewCreateInfo.image = images[i];
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = _displayImageFormat;
        imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
        imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
        imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
        imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;
        imageViewCreateInfo.subresourceRange.levelCount = 1;

        vkCreateImageView(_device, &imageViewCreateInfo, nullptr, &_imageViews[i]);
    }
}

void SVRenderer::createRenderPass() {
    VkAttachmentDescription attachmentDescription{};
    attachmentDescription.flags = 0;
    attachmentDescription.format = VK_FORMAT_B8G8R8A8_SRGB;
    attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
    attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference attachmentReference{};
    attachmentReference.attachment = 0;
    attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpassDescription{};
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &attachmentReference;

    VkSubpassDependency subpassDependency;
    subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpassDependency.dstSubpass = 0;
    subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.srcAccessMask = 0;
    subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    subpassDependency.dependencyFlags = 0;

    VkRenderPassCreateInfo renderPassCreateInfo;
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.pNext = nullptr;
    renderPassCreateInfo.flags = 0;
    renderPassCreateInfo.attachmentCount = 1;
    renderPassCreateInfo.pAttachments = &attachmentDescription;
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = &subpassDescription;
    renderPassCreateInfo.dependencyCount = 1;
    renderPassCreateInfo.pDependencies = &subpassDependency;

    vkCreateRenderPass(_device, &renderPassCreateInfo, nullptr, &_renderPass);
}

void SVRenderer::createDescriptorSetLayout() {
    VkDescriptorSetLayoutBinding layoutBinding;
    layoutBinding.binding = 0;
    layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    layoutBinding.descriptorCount = 1;
    layoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    layoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo layoutInfo;
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.pNext = nullptr;
    layoutInfo.flags = 0;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &layoutBinding;

    vkCreateDescriptorSetLayout(_device, &layoutInfo, nullptr, &_descriptorSetLayout);
}

void SVRenderer::initPipeline() {
    auto emptyVertexShader   = readFile("shaders/emptyShader.vert.spv");
    auto emptyFragmentShader = readFile("shaders/emptyShader.frag.spv");

    auto defaultVertexShader   = readFile("shaders/simpleShader.vert.spv");
    auto defaultFragmentShader = readFile("shaders/simpleShader.frag.spv");

    VkShaderModule emptyVertModule;
    VkShaderModule emptyFragModule;
    createShaderModule(emptyVertexShader, emptyVertModule);
    createShaderModule(emptyFragmentShader, emptyFragModule);

    VkShaderModule defaultVertModule;
    VkShaderModule defaultFragModule;
    createShaderModule(defaultVertexShader, defaultVertModule);
    createShaderModule(defaultFragmentShader, defaultFragModule);
}

/*
void SVRenderer::createDescriptorPool() {
    VkDescriptorPoolSize poolSize;
    poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSize.descriptorCount = 1;

    VkDescriptorPoolCreateInfo poolInfo;
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.pNext = nullptr;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.maxSets = 1;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;

    vkCreateDescriptorPool(_device, &poolInfo, nullptr, &_descriptorPool);

    VkDescriptorSetAllocateInfo descSetAllocate;
    descSetAllocate.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descSetAllocate.pNext = nullptr;
    descSetAllocate.descriptorPool = _descriptorPool;
    descSetAllocate.descriptorSetCount = 1;
    descSetAllocate.pSetLayouts = &_descriptorSetLayout;

    vkAllocateDescriptorSets(*_logicalDevice, &descSetAllocate, _descriptorSet);

    VkDescriptorImageInfo imageInfo;
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = *_textures[0].getImageView();
    imageInfo.sampler = *_textures[0].getSampler();

    VkWriteDescriptorSet writeDescSet{};
    writeDescSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescSet.dstSet = *_descriptorSet;
    writeDescSet.dstBinding = 0;
    writeDescSet.dstArrayElement = 0;
    writeDescSet.descriptorCount = 1;
    writeDescSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDescSet.pImageInfo = &imageInfo;
    writeDescSet.pBufferInfo = nullptr;
    writeDescSet.pTexelBufferView = nullptr;


    vkUpdateDescriptorSets(*_logicalDevice, 1, &writeDescSet, 0, nullptr);
}
*/
void SVRenderer::init() {
    createInstance();
    initSurface();
    initPhysicalDevices();
    createLogicalDevice();
    createSwapchain();
    createImageViews();
    createRenderPass();
    createDescriptorSetLayout();
}

void SVRenderer::destroy() {
    for(int i = 0; i < _amountOfSwapchainImages; i++){
        vkDestroyImageView(_device, _imageViews[i], nullptr);
    }
    _swapchain.destroy(_device);
    vkDestroyDevice(_device, nullptr);
    vkDestroySurfaceKHR(_instance, _surface, nullptr);
    vkDestroyInstance(_instance, nullptr);
}

SVRenderer::SVRenderer(SVWidget *widget) {
    if(widget == nullptr) {
        _widget = new SVWidget;
        SWSWidgetInfo widgetInfo;
        widgetInfo.sizeX = 100;
        widgetInfo.sizeY = 100;
    }
    else
        _widget = widget;
}

SVRenderer::~SVRenderer() {

}
