#ifndef SPECTRE_VULKAN_PIPELINE
#define SPECTRE_VULKAN_PIPELINE

#include <vulkan/vulkan.hpp>

typedef struct SVPipelineConfig{
    uint32_t                            amountOfVertexAttributes;
    uint32_t                            amountOfVertexBindings;
    VkVertexInputAttributeDescription*  vertexAttributes;
    VkVertexInputBindingDescription*    vertexBindings;
}SVPipelineConfig;

class SVPipeline {
private:
    VkPipeline          _pipeline;
    VkPipelineLayout    _pipelineLayout;

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

    VkDynamicState _dynamicStates[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
public:
    void enable(VkDevice& device, VkRenderPass& renderPass, VkDescriptorSetLayout& descriptorSetLayout);

    void init(VkShaderModule vertexShader, VkShaderModule fragmentShader, VkExtent2D size, SVPipelineConfig config);
    void destroy(VkDevice &device){
        vkDestroyPipeline(device, _pipeline, nullptr);
        vkDestroyPipelineLayout(device, _pipelineLayout, nullptr);
    }
    VkPipeline& getPipeline(){
        return _pipeline;
    }

    VkPipelineLayout& getLayout(){
        return _pipelineLayout;
    }
};

#endif