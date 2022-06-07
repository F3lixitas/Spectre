#ifndef SPECTRE_VULKAN_TEXTURE
#define SPECTRE_VULKAN_TEXTURE

#include "../conf.hpp"
#include "SVUtils.hpp"

#include <vulkan/vulkan.hpp>
#ifdef SPECTRE_ENGINE
    #define STB_IMAGE_IMPLEMENTATION
    #include "../external/stb/stb_image.h"
#elif defined SPECTRE_CORE
///todo custom implementation
#endif

class SVTexture{
private:
    VkImageView     _imageView;
    VkImage         _image;
    VkDeviceMemory  _imageMemory;

    int             _sizeX, _sizeY, _channels;

    void changeImageLayout(VkDevice& device, VkCommandPool& cmdPool, VkQueue& queue, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
public:
    void load(VkDevice& device, VkPhysicalDevice& physicalDevice, VkCommandPool& cmdPool, VkQueue& queue, const char* path);
    void create(const VkDevice& device);

    void destroy(VkDevice& device);
};

#endif