#include "SVTexture.hpp"

void SVTexture::changeImageLayout(VkDevice& device, VkCommandPool& cmdPool, VkQueue& queue, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
    VkCommandBuffer cmdBuffer = beginSingleTimeCommands(cmdPool, device);

    VkImageMemoryBarrier memBarrier{};
    memBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    memBarrier.oldLayout = oldLayout;
    memBarrier.newLayout = newLayout;
    memBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    memBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    memBarrier.image = _image;
    memBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    memBarrier.subresourceRange.baseMipLevel = 0;
    memBarrier.subresourceRange.levelCount = 1;
    memBarrier.subresourceRange.baseArrayLayer = 0;
    memBarrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        memBarrier.srcAccessMask = 0;
        memBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        memBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        memBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else {
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(cmdBuffer, sourceStage, destinationStage, 0, 0,
                         nullptr, 0, nullptr, 1, &memBarrier);

    endSingleTimeCommands(cmdBuffer, cmdPool, queue, device);
}

void SVTexture::load(VkDevice& device, VkPhysicalDevice& physicalDevice, VkCommandPool& cmdPool, VkQueue& queue, const char *path) {
    stbi_uc* pixels = stbi_load(path, &_sizeX, &_sizeY, &_channels, STBI_rgb_alpha);
    VkDeviceSize imageSize = _sizeX * _sizeY * 4;

    VkBuffer        stagingBuffer;
    VkDeviceMemory  stagingBufferMemory;

    createBuffer(&device, &physicalDevice, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, stagingBuffer,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBufferMemory);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(device, stagingBufferMemory);

    stbi_image_free(pixels);

    VkImageCreateInfo imageCreateInfo;
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.pNext = nullptr;
    imageCreateInfo.flags = 0;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.extent.width = static_cast<uint32_t>(_sizeX);
    imageCreateInfo.extent.height = static_cast<uint32_t>(_sizeY);
    imageCreateInfo.extent.depth = 1;
    imageCreateInfo.mipLevels = 1;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;

    vkCreateImage(device, &imageCreateInfo, nullptr, &_image);

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, _image, &memRequirements);

    uint32_t memIndex = getMemoryType(physicalDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    VkMemoryAllocateInfo allocationInfo;
    allocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocationInfo.pNext = nullptr;
    allocationInfo.allocationSize = memRequirements.size;
    allocationInfo.memoryTypeIndex = memIndex;

    vkAllocateMemory(device, &allocationInfo, nullptr, &_imageMemory);

    vkBindImageMemory(device, _image, _imageMemory, 0);

    changeImageLayout(device, cmdPool, queue, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    VkCommandBuffer cmdBuffer = beginSingleTimeCommands(cmdPool, device);

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = {
            static_cast<uint32_t>(_sizeX),
            static_cast<uint32_t>(_sizeY),
            1
    };
    vkCmdCopyBufferToImage(cmdBuffer, stagingBuffer, _image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    endSingleTimeCommands(cmdBuffer, cmdPool, queue, device);

    changeImageLayout(device, cmdPool, queue, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void SVTexture::create(const VkDevice& device) {
    VkImageViewCreateInfo texViewCreate;
    texViewCreate.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    texViewCreate.pNext = nullptr;
    texViewCreate.flags = 0;
    texViewCreate.image = _image;
    texViewCreate.viewType = VK_IMAGE_VIEW_TYPE_2D;
    texViewCreate.format = VK_FORMAT_R8G8B8A8_SRGB;
    texViewCreate.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    texViewCreate.subresourceRange.baseArrayLayer = 0;
    texViewCreate.subresourceRange.baseMipLevel = 0;
    texViewCreate.subresourceRange.layerCount = 1;
    texViewCreate.subresourceRange.levelCount = 1;

    vkCreateImageView(device, &texViewCreate, nullptr, &_imageView);
}

void SVTexture::destroy(VkDevice &device) {
    vkDestroyImageView(device, _imageView, nullptr);
    vkFreeMemory(device, _imageMemory, nullptr);
    vkDestroyImage(device, _image, nullptr);
}