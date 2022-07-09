#include "SC_Material.hpp"
#include "../vulkan/SVMesh3D.hpp"
#include "../utils/SOpenFile.hpp"


void SC_Material::i_addToRenderer(VkDevice& device, VkExtent2D size){
    _pipeline = new SVPipeline(&device);
    std::vector<VkVertexInputBindingDescription> bindingDesc = SVVertex3D::getBindingDescriptions();
    std::vector<VkVertexInputAttributeDescription> attributeDesc = SVVertex3D::getAttributeDescriptions();
    SVPipelineConfig defaultPipelineConf = {(uint32_t )attributeDesc.size(), (uint32_t )bindingDesc.size(),
                                            attributeDesc.data(), bindingDesc.data(),
                                            "shaders/defaultVertexShader2D.vert.spv", "shaders/simpleShader.frag.spv"};
    _pipeline->init(size, defaultPipelineConf);
}

void SC_Material::i_bindMaterial(VkCommandBuffer* commandBuffer){
    vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline->getPipeline());
    vkCmdBindDescriptorSets(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline->getLayout(), 0, 1, &_descriptorSet, 0, nullptr);
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

void SC_Material::i_draw(VkCommandBuffer& cmdBuffer){
    for (uint32_t i = 0; i < _meshes.size(); i++){
        _meshes[i]->draw(cmdBuffer);
    }
}