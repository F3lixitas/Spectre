#include "SC_Material.hpp"
#include "../vulkan/SVMesh3D.hpp"
#include "../utils/SOpenFile.hpp"

void SC_Material::i_createShaderModules(VkDevice& device, VkRenderPass& renderPass, VkExtent2D displaySize, VkDescriptorSetLayout& descriptorSetLayout){
    VkShaderModuleCreateInfo vertexShaderInfo;
    vertexShaderInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    vertexShaderInfo.pNext    = nullptr;
    vertexShaderInfo.flags    = 0;
    vertexShaderInfo.codeSize = _vertexShaderBufferSize;
    vertexShaderInfo.pCode    = _vertexShaderBuffer;

    vkCreateShaderModule(device, &vertexShaderInfo, nullptr, &_vertexShaderModule);

    VkShaderModuleCreateInfo fragmentShaderInfo;
    fragmentShaderInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    fragmentShaderInfo.pNext    = nullptr;
    fragmentShaderInfo.flags    = 0;
    fragmentShaderInfo.codeSize = _fragmentShaderBufferSize;
    fragmentShaderInfo.pCode    = _fragmentShaderBuffer;

    vkCreateShaderModule(device, &fragmentShaderInfo, nullptr, &_fragmentShaderModule);

    std::vector<VkVertexInputBindingDescription> bindingDesc = SVVertex3D::getBindingDescriptions();
    std::vector<VkVertexInputAttributeDescription> attributeDesc = SVVertex3D::getAttributeDescriptions();

    SVPipelineConfig pipelineConf = {(uint32_t )attributeDesc.size(), (uint32_t )bindingDesc.size(), attributeDesc.data(), bindingDesc.data()};

    _pipeline.init(_vertexShaderModule, _fragmentShaderModule, displaySize, pipelineConf);
    _pipeline.enable(device, renderPass, descriptorSetLayout);
}

void SC_Material::i_bindMaterial(VkCommandBuffer* commandBuffer){
    vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline.getPipeline());
    vkCmdBindDescriptorSets(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline.getLayout(), 0, 1, &_descriptorSet, 0, nullptr);
}

void SC_Material::loadFromFiles(const char* pathV, const char* pathF) {
    // loads a shader, todo make it load a material file

    std::vector<char> vertexShader   = readFile(pathV);//copy shader from source/vulkan/shaders
    std::vector<char> fragmentShader = readFile(pathF);

    _vertexShaderBufferSize = vertexShader.size() / sizeof(uint32_t);
    _fragmentShaderBufferSize = fragmentShader.size() / sizeof(uint32_t);

    _vertexShaderBuffer = new uint32_t[_vertexShaderBufferSize];
    _fragmentShaderBuffer = new uint32_t[_fragmentShaderBufferSize];

    memcpy(_vertexShaderBuffer, vertexShader.data(), vertexShader.size());
    memcpy(_fragmentShaderBuffer, fragmentShader.data(), fragmentShader.size());
}