#ifndef SPECTRE_VULKAN_TEXTURE
#define SPECTRE_VULKAN_TEXTURE

#include "../conf.hpp"
#include "SVUtils.hpp"

#include <vulkan/vulkan.hpp>


class SVTexture{
private:
    VkImageView     _imageView;
    VkImage         _image;
    VkDeviceMemory  _imageMemory;
    VkSampler       _sampler;

    VkDescriptorImageInfo _descriptor;

    int             _sizeX, _sizeY, _channels;
    void changeImageLayout(VkDevice& device, VkCommandPool& cmdPool, VkQueue& queue, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
public:
    void load(VkDevice& device, VkPhysicalDevice& physicalDevice, VkCommandPool& cmdPool, VkQueue& queue, const char* path);
    void create(const VkDevice& device);

    VkSampler getSampler(){return _sampler;}
    VkImageView getImageView(){return _imageView;}

    void destroy(VkDevice& device);
};

#endif