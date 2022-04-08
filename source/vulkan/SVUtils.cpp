#include "SVUtils.hpp"
#include <cstring>

void createBuffer(VkDevice* logicalDevice, VkPhysicalDevice* physicalDevice, VkDeviceSize bufferSize, VkBufferUsageFlags usage, VkBuffer& buffer, VkMemoryPropertyFlags memoryProperty, VkDeviceMemory& deviceMemory){
    VkBufferCreateInfo bufferInfo;
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.pNext = nullptr;
    bufferInfo.flags = 0;
    bufferInfo.size = bufferSize;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.queueFamilyIndexCount = 0;
    bufferInfo.pQueueFamilyIndices = nullptr;

    vkCreateBuffer(*logicalDevice, &bufferInfo, nullptr, &buffer);
    VkMemoryRequirements memoryRequirements;

    vkGetBufferMemoryRequirements(*logicalDevice, buffer, &memoryRequirements);

    uint32_t memIndex = getMemoryType(physicalDevice[0], memoryRequirements.memoryTypeBits, memoryProperty);

    VkMemoryAllocateInfo memoryAllocateInfo;
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.pNext = nullptr;
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = memIndex;

    vkAllocateMemory(*logicalDevice, &memoryAllocateInfo, nullptr, &deviceMemory);

    vkBindBufferMemory(*logicalDevice, buffer, deviceMemory, 0);
}

void createAndUploadBuffer(VkDevice& logicalDevice, VkPhysicalDevice* physicalDevice, VkDeviceSize bufferSize, VkBufferUsageFlags usage, VkBuffer& buffer, VkMemoryPropertyFlags memoryProperty, VkDeviceMemory& deviceMemory){
    createBuffer(&logicalDevice, physicalDevice, bufferSize, usage, buffer, memoryProperty, deviceMemory);

    void* data;
    vkMapMemory(logicalDevice, deviceMemory, 0, bufferSize, 0, &data);
    memcpy(data, buffer, static_cast<size_t>(bufferSize));
    vkUnmapMemory(logicalDevice, deviceMemory);
}



uint32_t getMemoryType(VkPhysicalDevice& physicalDevice, uint32_t memTypeBits, VkMemoryPropertyFlags memProp){
    VkPhysicalDeviceMemoryProperties physicalDeviceMemProp;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &physicalDeviceMemProp);

    uint32_t memIndex = 0;

    for(int i = 0; i < physicalDeviceMemProp.memoryTypeCount; i++){
        if(memTypeBits & (1 << i) && (physicalDeviceMemProp.memoryTypes[i].propertyFlags & memProp) == memProp)
            memIndex = i;
    }

    return memIndex;
}