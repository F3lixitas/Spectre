#include "SVRenderer2D.hpp"

void SVRenderer2D::addMeshData(std::vector<SVVertex2D>& vertices, std::vector<uint32_t>& indices, SC_Material* material){
    _meshes.emplace_back(&_device);
    SLog log = _meshes.back().loadVertices(&vertices, &indices, _physicalDevice);
    _meshes.back().setMaterial(material);
    ASSERT(log);
    vkDeviceWaitIdle(_device);
    updateRenderingCommands();
}

void SVRenderer2D::updateRenderingCommands() {
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

        for(int j = 0; j < _mesh.size(); j++){
            _meshes[j].bind(&_commandBuffers[i]);
            _meshes[j].draw(&_commandBuffers[i]);
        }

        vkCmdEndRenderPass(_commandBuffers[i]);
        vkEndCommandBuffer(_commandBuffers[i]);
    }
}