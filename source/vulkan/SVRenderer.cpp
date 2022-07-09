#include "SVRenderer.hpp"
#include <iostream>
#include "../utils/SOpenFile.hpp"
#include "../window/SWSRenderWindow.hpp"

void SVRenderer::createInstance() {
    VkApplicationInfo appInfo;
    appInfo.sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext               = nullptr;
    appInfo.apiVersion          = VK_API_VERSION_1_3;
    appInfo.applicationVersion  = VK_MAKE_VERSION(0, 0, 0);
    appInfo.engineVersion       = VK_MAKE_VERSION(0, 0, 0);
    appInfo.pApplicationName    = "Spectre";
    appInfo.pEngineName         = "Spectre Engine";

    std::vector<const char*> layers = {"VK_LAYER_KHRONOS_validation"};

    uint32_t count;
    const char** extensions = glfwGetRequiredInstanceExtensions(&count);

    VkInstanceCreateInfo instCreateInfo;
    instCreateInfo.sType                    = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instCreateInfo.pNext                    = nullptr;
    instCreateInfo.flags                    = 0;
    instCreateInfo.enabledExtensionCount    = count;
    instCreateInfo.enabledLayerCount        = layers.size();
    instCreateInfo.pApplicationInfo         = &appInfo;
    instCreateInfo.ppEnabledExtensionNames  = extensions;
    instCreateInfo.ppEnabledLayerNames      = layers.data();

    vkCreateInstance(&instCreateInfo, nullptr, &_instance);
}

void SVRenderer::initSurface() {
    _window->loadSurface(_instance, &_surface);
}

void SVRenderer::initPhysicalDevices() {
    vkEnumeratePhysicalDevices(_instance, &_amountOfPhysicalDevices, nullptr);
    _physicalDevices = new VkPhysicalDevice[_amountOfPhysicalDevices];
    vkEnumeratePhysicalDevices(_instance, &_amountOfPhysicalDevices, _physicalDevices);
    for(int i = 0; i < _amountOfPhysicalDevices; i++){
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(_physicalDevices[i], &props);
        std::cout << "api version : "   << props.apiVersion     << "\ndriver version : "    << props.driverVersion
                  << "\nvendor ID : "   << props.vendorID       << "\ndevice ID : "         << props.deviceID
                  << "\ndevice type : " << props.deviceType     << "\ndevice name : "       << props.deviceName
                  << "\npipeline cache UUID : " << props.pipelineCacheUUID << "\n\n";
        if(props.deviceType == 2) _physicalDevice = &_physicalDevices[i];
    }

}

void SVRenderer::createLogicalDevice() {
    VkPhysicalDeviceFeatures physicalDevFeatures = {};
    physicalDevFeatures.samplerAnisotropy = VK_TRUE; // enable anisotropic filtering
    // todo make a selection as my laptop's graphic card is n2
    vkGetPhysicalDeviceFeatures(*_physicalDevice, &physicalDevFeatures);

    uint32_t amountOfQueues;
    vkGetPhysicalDeviceQueueFamilyProperties(*_physicalDevice, &amountOfQueues, nullptr);
    VkQueueFamilyProperties* queueProperties = new VkQueueFamilyProperties[amountOfQueues];
    vkGetPhysicalDeviceQueueFamilyProperties(*_physicalDevice, &amountOfQueues, queueProperties);

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
    queueCreateInfo.sType               = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.pNext               = nullptr;
    queueCreateInfo.flags               = 0;
    queueCreateInfo.pQueuePriorities    = queuePrio;
    queueCreateInfo.queueCount          = queueCount;
    queueCreateInfo.queueFamilyIndex    = queueIndex;

    const std::vector<const char*> extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    VkDeviceCreateInfo devCreateInfo;
    devCreateInfo.sType                     = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    devCreateInfo.pNext                     = nullptr;
    devCreateInfo.flags                     = 0;
    devCreateInfo.enabledExtensionCount     = extensions.size();
    devCreateInfo.enabledLayerCount         = 0;
    devCreateInfo.pEnabledFeatures          = &physicalDevFeatures;
    devCreateInfo.ppEnabledExtensionNames   = extensions.data();
    devCreateInfo.ppEnabledLayerNames       = nullptr;
    devCreateInfo.queueCreateInfoCount      = 1;
    devCreateInfo.pQueueCreateInfos         = &queueCreateInfo;

    vkCreateDevice(*_physicalDevice, &devCreateInfo, nullptr, &_device);

    vkGetDeviceQueue(_device, queueIndex, 0, &_queue);

    VkBool32 surfaceSupport;
    vkGetPhysicalDeviceSurfaceSupportKHR(*_physicalDevice, queueIndex, _surface, &surfaceSupport);

    delete[] queueProperties;
    delete[] queuePrio;
}

void SVRenderer::createSwapchain() {
    VkSurfaceCapabilitiesKHR surfaceCapa;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*_physicalDevice, _surface, &surfaceCapa);
    _displaySize = surfaceCapa.currentExtent;

    VkRect2D dimensions;
    dimensions.extent   = _displaySize;
    dimensions.offset.x = 0;
    dimensions.offset.y = 0;
    _swapchain.init(_physicalDevice, _surface, dimensions, _displayImageFormat, _displayImageColorSpace);
}

void SVRenderer::createImageViews() {
    vkGetSwapchainImagesKHR(_device, _swapchain.getSwapchain(), &_amountOfSwapchainImages, nullptr);
    VkImage *images = new VkImage[_amountOfSwapchainImages];
    vkGetSwapchainImagesKHR(_device, _swapchain.getSwapchain(), &_amountOfSwapchainImages, images);

    _imageViews = new VkImageView[_amountOfSwapchainImages];
    for (int i = 0; i < _amountOfSwapchainImages; i++) {
        VkImageViewCreateInfo imageViewCreateInfo;
        imageViewCreateInfo.sType       = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.pNext       = nullptr;
        imageViewCreateInfo.flags       = 0;
        imageViewCreateInfo.image       = images[i];
        imageViewCreateInfo.viewType    = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format      = _displayImageFormat;

        imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
        imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
        imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
        imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;

        imageViewCreateInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.baseMipLevel   = 0;
        imageViewCreateInfo.subresourceRange.layerCount     = 1;
        imageViewCreateInfo.subresourceRange.levelCount     = 1;

        vkCreateImageView(_device, &imageViewCreateInfo, nullptr, &_imageViews[i]);
    }
}

void SVRenderer::createDescriptorSetLayout() {
    VkDescriptorSetLayoutBinding layoutBinding;
    layoutBinding.binding               = 0;
    layoutBinding.descriptorType        = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    layoutBinding.descriptorCount       = 1;
    layoutBinding.stageFlags            = VK_SHADER_STAGE_FRAGMENT_BIT;
    layoutBinding.pImmutableSamplers    = nullptr;

    VkDescriptorSetLayoutCreateInfo layoutInfo;
    layoutInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.pNext        = nullptr;
    layoutInfo.flags        = 0;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings    = &layoutBinding;

    vkCreateDescriptorSetLayout(_device, &layoutInfo, nullptr, &_descriptorSetLayout);
}

void SVRenderer::initPipeline() {

    std::vector<VkVertexInputBindingDescription> bindingDesc = SVVertex3D::getBindingDescriptions();
    std::vector<VkVertexInputAttributeDescription> attributeDesc = SVVertex3D::getAttributeDescriptions();
//
    SVPipelineConfig emptyPipelineConf = {0, 0, nullptr, nullptr, "shaders/emptyShader.vert.spv", "shaders/emptyShader.frag.spv"};
    //
    SVPipelineConfig defaultPipelineConf = {(uint32_t )attributeDesc.size(), (uint32_t )bindingDesc.size(),
                                            attributeDesc.data(), bindingDesc.data(),
                                            "shaders/defaultVertexShader2D.vert.spv", "shaders/simpleShader.frag.spv"};
//
    _pipelines.push_back(SVPipeline(&_device));
    _pipelines[0].init(_displaySize, emptyPipelineConf);
    _pipelines[0].enable(_device, _swapchain.getRenderPass(), _descriptorSetLayout);
//
    _pipelines.push_back(SVPipeline(&_device));
    _pipelines[1].init(_displaySize, defaultPipelineConf);
    _pipelines[1].enable(_device, _swapchain.getRenderPass(), _descriptorSetLayout);
}

void SVRenderer::initFramebuffers(){
    _framebuffers = new VkFramebuffer[_amountOfSwapchainImages];

    for(int i = 0; i < _amountOfSwapchainImages; i++){
        VkFramebufferCreateInfo framebufferCreateInfo;
        framebufferCreateInfo.sType             = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.pNext             = nullptr;
        framebufferCreateInfo.flags             = 0;
        framebufferCreateInfo.renderPass        = _swapchain.getRenderPass();
        framebufferCreateInfo.attachmentCount   = 1;
        framebufferCreateInfo.pAttachments      = &_imageViews[i];
        framebufferCreateInfo.width             = _displaySize.width;
        framebufferCreateInfo.height            = _displaySize.height;
        framebufferCreateInfo.layers            = 1;

        vkCreateFramebuffer(_device, &framebufferCreateInfo, nullptr, &_framebuffers[i]);
    }
}

void SVRenderer::initCommandPool(){
    VkCommandPoolCreateInfo commandPoolInfo;
    commandPoolInfo.sType               = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolInfo.pNext               = nullptr;
    commandPoolInfo.flags               = 0;
    commandPoolInfo.queueFamilyIndex    = _queueFamilyIndex;

    vkCreateCommandPool(_device, &commandPoolInfo, nullptr, &_commandPool);
}


void SVRenderer::createDescriptorPool() {
    VkDescriptorPoolSize poolSize;
    poolSize.type               = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSize.descriptorCount    = 1;

    VkDescriptorPoolCreateInfo poolInfo;
    poolInfo.sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.pNext          = nullptr;
    poolInfo.flags          = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.maxSets        = 1;
    poolInfo.poolSizeCount  = 1;
    poolInfo.pPoolSizes     = &poolSize;

    vkCreateDescriptorPool(_device, &poolInfo, nullptr, &_descriptorPool);

    VkDescriptorSetAllocateInfo descSetAllocate;
    descSetAllocate.sType               = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descSetAllocate.pNext               = nullptr;
    descSetAllocate.descriptorPool      = _descriptorPool;
    descSetAllocate.descriptorSetCount  = 1;
    descSetAllocate.pSetLayouts         = &_descriptorSetLayout;

    vkAllocateDescriptorSets(_device, &descSetAllocate, &_descriptorSet);

    VkDescriptorImageInfo imageInfo;
    imageInfo.imageLayout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView     = _texture.getImageView();
    imageInfo.sampler       = _texture.getSampler();

    VkWriteDescriptorSet writeDescSet{};
    writeDescSet.sType              = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescSet.dstSet             = _descriptorSet;
    writeDescSet.dstBinding         = 0;
    writeDescSet.dstArrayElement    = 0;
    writeDescSet.descriptorCount    = 1;
    writeDescSet.descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDescSet.pImageInfo         = &imageInfo;


    vkUpdateDescriptorSets(_device, 1, &writeDescSet, 0, nullptr);
}

void SVRenderer::createCommand(){

    vkDeviceWaitIdle(_device);

    VkCommandBufferAllocateInfo cmdBufferAllocateInfo;
    cmdBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdBufferAllocateInfo.pNext = nullptr;
    cmdBufferAllocateInfo.commandPool = _commandPool;
    cmdBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdBufferAllocateInfo.commandBufferCount = _amountOfSwapchainImages;

    _commandBuffers = new VkCommandBuffer[_amountOfSwapchainImages];

    vkAllocateCommandBuffers(_device, &cmdBufferAllocateInfo, _commandBuffers);

    VkCommandBufferBeginInfo cmdBufferBeginInfo;
    cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBufferBeginInfo.pNext = nullptr;
    cmdBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    cmdBufferBeginInfo.pInheritanceInfo = nullptr;

    for(int i = 0; i < _amountOfSwapchainImages; i++){
        vkBeginCommandBuffer(_commandBuffers[i], &cmdBufferBeginInfo);

        _swapchain.beginRenderPass(_commandBuffers[i], _framebuffers[i], _displaySize);

        vkCmdBindPipeline(_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelines[0].getPipeline());

        VkViewport viewport;
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = _displaySize.width;
        viewport.height = _displaySize.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        vkCmdSetViewport(_commandBuffers[i], 0, 1, &viewport);

        VkRect2D scissor;
        scissor.offset = {0, 0};
        scissor.extent = _displaySize;

        vkCmdSetScissor(_commandBuffers[i], 0, 1, &scissor);

        vkCmdBindDescriptorSets(_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelines[0].getLayout(), 0, 1, &_descriptorSet, 0, nullptr);

        vkCmdDraw(_commandBuffers[i], 0, 1, 0, 0);

        vkCmdEndRenderPass(_commandBuffers[i]);
        vkEndCommandBuffer(_commandBuffers[i]);
    }


}

void SVRenderer::initSemaphore(){
    VkSemaphoreCreateInfo semaphoreInfo;
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreInfo.pNext = nullptr;
    semaphoreInfo.flags = 0;

    vkCreateSemaphore(_device, &semaphoreInfo, nullptr, &_semaphoreBegin);

    vkCreateSemaphore(_device, &semaphoreInfo, nullptr, &_semaphoreEnd);
}

void SVRenderer::createTextureView() {

}

void SVRenderer::init() {
    createInstance();
    initSurface();
    initPhysicalDevices();
    createLogicalDevice();
    createSwapchain();
    createImageViews();
    createDescriptorSetLayout();
    initPipeline();
    initFramebuffers();
    initCommandPool();
    _texture.load(_device, *_physicalDevice, _commandPool, _queue, "data/spectre.png");
    _texture.create(_device);
    createDescriptorPool();
    createCommand();
    initSemaphore();

    sAddLog({L"Renderer initialized.", S_LOG_INFO});
}

void SVRenderer::updateRenderingCommands(){

    vkFreeCommandBuffers(_device, _commandPool, _amountOfSwapchainImages, _commandBuffers);

    VkCommandBufferAllocateInfo cmdBufferAllocateInfo;
    cmdBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdBufferAllocateInfo.pNext = nullptr;
    cmdBufferAllocateInfo.commandPool = _commandPool;
    cmdBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdBufferAllocateInfo.commandBufferCount = _amountOfSwapchainImages;

    _commandBuffers = new VkCommandBuffer[_amountOfSwapchainImages];

    vkAllocateCommandBuffers(_device, &cmdBufferAllocateInfo, _commandBuffers);

    VkCommandBufferBeginInfo cmdBufferBeginInfo;
    cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBufferBeginInfo.pNext = nullptr;
    cmdBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    cmdBufferBeginInfo.pInheritanceInfo = nullptr;

    for(int i = 0; i < _amountOfSwapchainImages; i++){
        vkBeginCommandBuffer(_commandBuffers[i], &cmdBufferBeginInfo);

        _swapchain.beginRenderPass(_commandBuffers[i], _framebuffers[i], _displaySize);
        vkCmdBindPipeline(_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelines[1].getPipeline());

        VkViewport viewport;
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = _displaySize.width;
        viewport.height = _displaySize.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        vkCmdSetViewport(_commandBuffers[i], 0, 1, &viewport);

        VkRect2D scissor;
        scissor.offset = {0, 0};
        scissor.extent = _displaySize;

        vkCmdSetScissor(_commandBuffers[i], 0, 1, &scissor);

        VkDeviceSize offsets[] = {0};
        vkCmdBindDescriptorSets(_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelines[1].getLayout(), 0, 1, &_descriptorSet, 0, nullptr);
        for(int j = 0; j < _mesh.size(); j++){
            _mesh[j].draw(_commandBuffers[i]);
        }

        vkCmdEndRenderPass(_commandBuffers[i]);
        vkEndCommandBuffer(_commandBuffers[i]);
    }

}

void SVRenderer::addMeshData(std::vector<SVVertex3D>& vertices, std::vector<uint32_t>& indices){
    _mesh.push_back(SVMesh3D(&_device));
    SLog log = _mesh[_mesh.size() - 1].loadVertices(&vertices, &indices, _physicalDevice);
    ASSERT(log);
    vkDeviceWaitIdle(_device);
    updateRenderingCommands();
}

void SVRenderer::removeMeshData(uint32_t index){
    if(index >= _mesh.size()) return;
    std::vector<SVMesh3D>::iterator it = _mesh.begin();
    std::advance(it, index);
    vkDeviceWaitIdle(_device);
    _mesh[index].destroy();
    _mesh.erase(it);
    updateRenderingCommands();
}

void SVRenderer::render(){
    vkDeviceWaitIdle(_device);
    uint32_t imageIndex;
    vkAcquireNextImageKHR(_device, _swapchain.getSwapchain(), std::numeric_limits<uint64_t>::max(), _semaphoreBegin, VK_NULL_HANDLE, &imageIndex);

    VkPipelineStageFlags waitStageMask[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

    VkSubmitInfo submitInfo;
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = nullptr;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &_semaphoreBegin;
    submitInfo.pWaitDstStageMask = waitStageMask;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &_commandBuffers[imageIndex];
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &_semaphoreEnd;

    vkQueueSubmit(_queue, 1, &submitInfo, VK_NULL_HANDLE);

    VkPresentInfoKHR presentInfo;
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext = nullptr;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &_semaphoreEnd;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &_swapchain.getSwapchain();
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    vkQueuePresentKHR(_queue, &presentInfo);
}

void SVRenderer::renderToImage(SVRenderer* renderer){

}

void SVRenderer::destroy() {
    vkDeviceWaitIdle(_device);

    for(int i = 0; i < _mesh.size(); i++){
        _mesh[i].destroy();
    }

    vkDestroySemaphore(_device, _semaphoreBegin, nullptr);
    vkDestroySemaphore(_device, _semaphoreEnd, nullptr);

    vkFreeCommandBuffers(_device, _commandPool, _amountOfSwapchainImages, _commandBuffers);

    vkFreeDescriptorSets(_device, _descriptorPool, 1, &_descriptorSet);

    vkDestroyDescriptorPool(_device, _descriptorPool, nullptr);
    for(int i = 0; i < _amountOfSwapchainImages; i++){
        vkDestroyFramebuffer(_device, _framebuffers[i], nullptr);
    }

    _texture.destroy(_device);

    vkDestroyCommandPool(_device, _commandPool, nullptr);

    for(int i = 0; i < _pipelines.size(); i++){
        _pipelines[i].destroy();
    }

    vkDestroyDescriptorSetLayout(_device, _descriptorSetLayout, nullptr);

    for(int i = 0; i < _amountOfSwapchainImages; i++){
        vkDestroyImageView(_device, _imageViews[i], nullptr);
    }
    _swapchain.destroy(_device);
    vkDestroyDevice(_device, nullptr);
    vkDestroySurfaceKHR(_instance, _surface, nullptr);
    vkDestroyInstance(_instance, nullptr);
}

SVRenderer::SVRenderer(SWSRenderWindow *window) {
    _window = window;
}

SVRenderer::~SVRenderer() {
    delete[] _physicalDevices;
    delete[] _imageViews;
    delete[] _framebuffers;
    delete[] _commandBuffers;
}
