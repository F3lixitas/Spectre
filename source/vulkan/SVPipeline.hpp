#ifndef SPECTRE_VULKAN_PIPELINE
#define SPECTRE_VULKAN_PIPELINE

#include <vulkan/vulkan.hpp>
#include <string>

typedef struct SVPipelineConfig{
    uint32_t                            amountOfVertexAttributes;
    uint32_t                            amountOfVertexBindings;
    VkVertexInputAttributeDescription*  vertexAttributes;
    VkVertexInputBindingDescription*    vertexBindings;
    std::string                         vertexShaderPath;
    std::string                         fragmentShaderPath;
}SVPipelineConfig;

class SVPipeline {
private:
    VkPipeline          _pipeline;
    VkPipelineLayout    _pipelineLayout;

    VkShaderModule      _vertexShaderModule;
    VkShaderModule      _fragmentShaderModule;
    std::vector<char>   _vertexShaderB;
    std::vector<char>   _fragmentShaderB;

    VkDescriptorSet _descriptorSet;

    VkPipelineShaderStageCreateInfo _shaderStageInfoVertex;
    VkPipelineShaderStageCreateInfo _shaderStageInfoFragment;

    VkPipelineVertexInputStateCreateInfo    _vertexInputInfo;
    VkPipelineInputAssemblyStateCreateInfo  _inputAssemblyInfo;

    VkViewport _viewport;
    VkRect2D   _scissor;

    VkPipelineViewportStateCreateInfo       _viewportInfo;
    VkPipelineRasterizationStateCreateInfo  _rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo    _multisampleInfo;
    VkPipelineColorBlendStateCreateInfo     _colorInfo;
    VkPipelineDynamicStateCreateInfo        _dynamicInfo;
    VkPipelineColorBlendAttachmentState     _colorAttachment;

    VkDevice*   _device;

    VkDynamicState _dynamicStates[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
public:
    SVPipeline(VkDevice* device) : _device(device) {}
    void enable(VkDevice& device, VkRenderPass& renderPass, VkDescriptorSetLayout& descriptorSetLayout);

    void init(VkExtent2D size, SVPipelineConfig config);
    void destroy(){
        vkDestroyShaderModule(*_device, _vertexShaderModule, nullptr);
        vkDestroyShaderModule(*_device, _fragmentShaderModule, nullptr);
        vkDestroyPipeline(*_device, _pipeline, nullptr);
        vkDestroyPipelineLayout(*_device, _pipelineLayout, nullptr);
    }
    VkPipeline& getPipeline(){
        return _pipeline;
    }

    VkPipelineLayout& getLayout(){
        return _pipelineLayout;
    }
};

#endif