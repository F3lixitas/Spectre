#ifndef SPECTRE_CORE_MATERIAL
#define SPECTRE_CORE_MATERIAL

/*
 * For now an empty shell, will handle the loading of materials into the renderer
 * */

#include <vulkan/vulkan.hpp>
#include "../vulkan/SVPipeline.hpp"


class SC_Material{
private:
    uint32_t*       _vertexShaderBuffer;
    uint32_t        _vertexShaderBufferSize;
    uint32_t*       _fragmentShaderBuffer;
    uint32_t        _fragmentShaderBufferSize;
    VkShaderModule  _vertexShaderModule;
    VkShaderModule  _fragmentShaderModule;

    VkDescriptorSet _descriptorSet;

    SVPipeline      _pipeline;
public:
    void loadFromFiles(const char* pathV, const char* pathF);
    void loadFromBuffer(const char* buffer, uint32_t size);

    // internal
    void i_createShaderModules(VkDevice& device, VkRenderPass& renderPass, VkExtent2D displaySize, VkDescriptorSetLayout& descriptorSetLayout);
    void i_bindMaterial(VkCommandBuffer* commandBuffer);
};

#endif