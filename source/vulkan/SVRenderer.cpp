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

    const char* extensions[] = {"VK_KHR_xcb_surface", "VK_KHR_surface"};

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
    auto emptyVertexShader   = readFile("../source/vulkan/shaders/emptyShader.vert.spv");
    auto emptyFragmentShader = readFile("../source/vulkan/shaders/emptyShader.frag.spv");

    auto defaultVertexShader   = readFile("../source/vulkan/shaders/simpleShader.vert.spv");
    auto defaultFragmentShader = readFile("../source/vulkan/shaders/simpleShader.frag.spv");

    VkShaderModule emptyVertModule;
    VkShaderModule emptyFragModule;
    createShaderModule(emptyVertexShader, emptyVertModule);
    createShaderModule(emptyFragmentShader, emptyFragModule);

    VkShaderModule defaultVertModule;
    VkShaderModule defaultFragModule;
    createShaderModule(defaultVertexShader, defaultVertModule);
    createShaderModule(defaultFragmentShader, defaultFragModule);

    std::vector<VkVertexInputBindingDescription> bindingDesc = SVVertex::getBindingDescriptions();
    std::vector<VkVertexInputAttributeDescription> attributeDesc = SVVertex::getAttributeDescriptions();

    SVPipelineConfig emptyPipelineConf = {0, 0, nullptr, nullptr};
    SVPipelineConfig defaultPipelineConf = {(uint32_t )attributeDesc.size(), (uint32_t )bindingDesc.size(), attributeDesc.data(), bindingDesc.data()};

    _pipelines.push_back(SVPipeline());
    _pipelines[0].init(emptyVertModule, emptyFragModule, _displaySize, emptyPipelineConf);
    _pipelines[0].enable(_device, _renderPass, _descriptorSetLayout);

    _pipelines.push_back(SVPipeline());
    _pipelines[1].init(defaultVertModule, defaultFragModule, _displaySize, defaultPipelineConf);
    _pipelines[1].enable(_device, _renderPass, _descriptorSetLayout);

    vkDestroyShaderModule(_device, emptyVertModule, nullptr);
    vkDestroyShaderModule(_device, emptyFragModule, nullptr);

    vkDestroyShaderModule(_device, defaultVertModule, nullptr);
    vkDestroyShaderModule(_device, defaultFragModule, nullptr);
}

void SVRenderer::initFramebuffers(){
    _framebuffers = new VkFramebuffer[_amountOfSwapchainImages];

    for(int i = 0; i < _amountOfSwapchainImages; i++){
        VkFramebufferCreateInfo framebufferCreateInfo;
        framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.pNext = nullptr;
        framebufferCreateInfo.flags = 0;
        framebufferCreateInfo.renderPass = _renderPass;
        framebufferCreateInfo.attachmentCount = 1;
        framebufferCreateInfo.pAttachments = &_imageViews[i];
        framebufferCreateInfo.width = _displaySize.width;
        framebufferCreateInfo.height = _displaySize.height;
        framebufferCreateInfo.layers = 1;

        vkCreateFramebuffer(_device, &framebufferCreateInfo, nullptr, &_framebuffers[i]);
    }
}

void SVRenderer::initCommandPool(){
    VkCommandPoolCreateInfo commandPoolInfo;
    commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolInfo.pNext = nullptr;
    commandPoolInfo.flags = 0;
    commandPoolInfo.queueFamilyIndex = _queueFamilyIndex;

    vkCreateCommandPool(_device, &commandPoolInfo, nullptr, &_commandPool);
}


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

    vkAllocateDescriptorSets(_device, &descSetAllocate, &_descriptorSet);

    //VkDescriptorImageInfo imageInfo;
    //imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    //imageInfo.imageView = *_textures[0].getImageView();
    //imageInfo.sampler = *_textures[0].getSampler();

    VkWriteDescriptorSet writeDescSet{};
    writeDescSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescSet.dstSet = _descriptorSet;
    writeDescSet.dstBinding = 0;
    writeDescSet.dstArrayElement = 0;
    writeDescSet.descriptorCount = 1;
    writeDescSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
//    writeDescSet.pImageInfo = &imageInfo;
    writeDescSet.pImageInfo = nullptr;
    writeDescSet.pBufferInfo = nullptr;
    writeDescSet.pTexelBufferView = nullptr;


    //vkUpdateDescriptorSets(_device, 1, &writeDescSet, 0, nullptr);
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

        VkClearValue clearValue = {0.0f, 0.0f, 0.0f, 1.0f};

        VkRenderPassBeginInfo renderPassBeginInfo;
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.pNext = nullptr;
        renderPassBeginInfo.renderPass = _renderPass;
        renderPassBeginInfo.renderArea.offset = {0, 0};
        renderPassBeginInfo.renderArea.extent = _displaySize;
        renderPassBeginInfo.framebuffer = _framebuffers[i];
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues = &clearValue;

        vkCmdBeginRenderPass(_commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

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

void SVRenderer::init() {
    createInstance();
    initSurface();
    initPhysicalDevices();
    createLogicalDevice();
    createSwapchain();
    createImageViews();
    createRenderPass();
    createDescriptorSetLayout();
    initPipeline();
    initFramebuffers();
    initCommandPool();
    createDescriptorPool();
    createCommand();
    initSemaphore();
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

        VkClearValue clearValue = {0.0f, 0.0f, 0.0f, 1.0f};

        VkRenderPassBeginInfo renderPassBeginInfo;
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.pNext = nullptr;
        renderPassBeginInfo.renderPass = _renderPass;
        renderPassBeginInfo.renderArea.offset = {0, 0};
        renderPassBeginInfo.renderArea.extent = _displaySize;
        renderPassBeginInfo.framebuffer = _framebuffers[i];
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues = &clearValue;

        vkCmdBeginRenderPass(_commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

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
            _mesh[j].bind(&_commandBuffers[i]);
            _mesh[j].draw(&_commandBuffers[i]);
        }

        vkCmdEndRenderPass(_commandBuffers[i]);
        vkEndCommandBuffer(_commandBuffers[i]);
    }

}

void SVRenderer::addMeshData(std::vector<SVVertex>& vertices, std::vector<uint32_t>& indices){
    _mesh.push_back(SVMesh(&_device));
    _mesh[_mesh.size() - 1].loadVertices(&vertices, &indices, &_physicalDevices[0]);
    vkDeviceWaitIdle(_device);
    updateRenderingCommands();
}

void SVRenderer::removeMeshData(uint32_t index){
    if(index >= _mesh.size()) return;
    std::vector<SVMesh>::iterator it = _mesh.begin();
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
