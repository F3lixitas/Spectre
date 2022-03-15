#include "SVPipeline.hpp"

void
SVPipeline::init(VkShaderModule vertexShader, VkShaderModule fragmentShader, VkExtent2D size, SVPipelineConfig config) {
    _shaderStageInfoVertex.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    _shaderStageInfoVertex.pNext = nullptr;
    _shaderStageInfoVertex.flags = 0;
    _shaderStageInfoVertex.stage = VK_SHADER_STAGE_VERTEX_BIT;
    _shaderStageInfoVertex.module = vertexShader;
    _shaderStageInfoVertex.pName = "main";
    _shaderStageInfoVertex.pSpecializationInfo = nullptr;

    _shaderStageInfoFragment.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    _shaderStageInfoFragment.pNext = nullptr;
    _shaderStageInfoFragment.flags = 0;
    _shaderStageInfoFragment.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    _shaderStageInfoFragment.module = fragmentShader;
    _shaderStageInfoFragment.pName = "main";
    _shaderStageInfoFragment.pSpecializationInfo = nullptr;


    _vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    _vertexInputInfo.pNext = nullptr;
    _vertexInputInfo.flags = 0;
    _vertexInputInfo.vertexBindingDescriptionCount = config.amountOfVertexBindings;
    _vertexInputInfo.pVertexBindingDescriptions = config.vertexBindings;
    _vertexInputInfo.vertexAttributeDescriptionCount = config.amountOfVertexAttributes;
    _vertexInputInfo.pVertexAttributeDescriptions = config.vertexAttributes;


    _inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    _inputAssemblyInfo.pNext = nullptr;
    _inputAssemblyInfo.flags = 0;
    _inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    _inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    _viewport.x = 0.0f;
    _viewport.y = 0.0f;
    _viewport.width = size.width;
    _viewport.height = size.height;
    _viewport.minDepth = 0.0f;
    _viewport.maxDepth = 1.0f;

    _scissor.offset = {0, 0};
    _scissor.extent = size;

    _viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    _viewportInfo.pNext = nullptr;
    _viewportInfo.flags = 0;
    _viewportInfo.viewportCount = 1;
    _viewportInfo.pViewports = &_viewport;
    _viewportInfo.scissorCount = 1;
    _viewportInfo.pScissors = &_scissor;

    _rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    _rasterizationInfo.pNext = nullptr;
    _rasterizationInfo.flags = 0;
    _rasterizationInfo.depthClampEnable = VK_FALSE;
    _rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    _rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
    _rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
    _rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    _rasterizationInfo.depthBiasEnable = VK_FALSE;
    _rasterizationInfo.depthBiasConstantFactor = 0.0f;
    _rasterizationInfo.depthBiasClamp = 0.0f;
    _rasterizationInfo.depthBiasSlopeFactor = 0.0f;
    _rasterizationInfo.lineWidth = 1.0f;

    _multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    _multisampleInfo.pNext = nullptr;
    _multisampleInfo.flags = 0;
    _multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    _multisampleInfo.sampleShadingEnable = VK_FALSE;
    _multisampleInfo.minSampleShading = 1.0f;
    _multisampleInfo.pSampleMask = nullptr;
    _multisampleInfo.alphaToCoverageEnable = VK_FALSE;
    _multisampleInfo.alphaToOneEnable = VK_FALSE;

    _colorAttachment.blendEnable = VK_FALSE;
    _colorAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    _colorAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    _colorAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    _colorAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    _colorAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    _colorAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
    _colorAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    _colorInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    _colorInfo.pNext = nullptr;
    _colorInfo.flags = 0;
    _colorInfo.logicOpEnable = VK_FALSE;
    _colorInfo.logicOp = VK_LOGIC_OP_NO_OP;
    _colorInfo.attachmentCount = 1;
    _colorInfo.pAttachments = &_colorAttachment;
    _colorInfo.blendConstants[0] = 0.0f;
    _colorInfo.blendConstants[1] = 0.0f;
    _colorInfo.blendConstants[2] = 0.0f;
    _colorInfo.blendConstants[3] = 0.0f;

    _dynamicInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    _dynamicInfo.pNext = nullptr;
    _dynamicInfo.flags = 0;
    _dynamicInfo.dynamicStateCount = 2;
    _dynamicInfo.pDynamicStates = _dynamicStates;
}

void SVPipeline::enable(VkDevice &device, VkRenderPass &renderPass, VkDescriptorSetLayout &descriptorSetLayout) {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo;
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.pNext = nullptr;
    pipelineLayoutInfo.flags = 0;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &_pipelineLayout);

    VkPipelineShaderStageCreateInfo shaders[] = {_shaderStageInfoVertex, _shaderStageInfoFragment};

    VkGraphicsPipelineCreateInfo graphicsInfo;
    graphicsInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphicsInfo.pNext = nullptr;
    graphicsInfo.flags = 0;
    graphicsInfo.stageCount = 2;
    graphicsInfo.pStages = shaders;
    graphicsInfo.pVertexInputState = &_vertexInputInfo;
    graphicsInfo.pInputAssemblyState = &_inputAssemblyInfo;
    graphicsInfo.pTessellationState = nullptr;
    graphicsInfo.pViewportState = &_viewportInfo;
    graphicsInfo.pRasterizationState = &_rasterizationInfo;
    graphicsInfo.pMultisampleState = &_multisampleInfo;
    graphicsInfo.pDepthStencilState = nullptr;
    graphicsInfo.pColorBlendState = &_colorInfo;
    graphicsInfo.pDynamicState = &_dynamicInfo;
    graphicsInfo.layout = _pipelineLayout;
    graphicsInfo.renderPass = renderPass;
    graphicsInfo.subpass = 0;
    graphicsInfo.basePipelineHandle = VK_NULL_HANDLE;
    graphicsInfo.basePipelineIndex = -1;

    vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &graphicsInfo, nullptr, &_pipeline);
}