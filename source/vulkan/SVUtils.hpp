#ifndef SPECTRE_VULKAN_UTILS
#define SPECTRE_VULKAN_UTILS

#include <vulkan/vulkan.hpp>
#include <vector>

#ifdef __linux__
#define RESTRICT __restrict__
#elif #defined WIN32
#define RESTRICT __restrict
#else
    #define RESTRICT
#endif

void createBuffer(VkDevice& logicalDevice, VkPhysicalDevice* physicalDevice, VkDeviceSize bufferSize, VkBufferUsageFlags usage, VkBuffer& buffer, VkMemoryPropertyFlags memoryProperty, VkDeviceMemory& deviceMemory);

void createAndUploadBuffer(VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkDeviceSize bufferSize, VkBufferUsageFlags usage, VkBuffer& buffer, VkMemoryPropertyFlags memoryProperty, VkDeviceMemory& deviceMemory);

uint32_t getMemoryType(VkPhysicalDevice& physicalDevice, uint32_t memTypeBits, VkMemoryPropertyFlags memProp);


#endif