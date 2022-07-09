#ifndef SPECTRE_CORE_MATERIAL
#define SPECTRE_CORE_MATERIAL

/*
 * For now an empty shell, will handle the loading of materials into the renderer
 * */

#include <vector>
#include <vulkan/vulkan.hpp>
#include "../vulkan/SVPipeline.hpp"

class SVMesh3D;

class SC_Material{
private:
    uint32_t*       _vertexShaderBuffer;
    uint32_t        _vertexShaderBufferSize;
    uint32_t*       _fragmentShaderBuffer;
    uint32_t        _fragmentShaderBufferSize;
    VkShaderModule  _vertexShaderModule;
    VkShaderModule  _fragmentShaderModule;

    VkDescriptorSet _descriptorSet;

    SVPipeline*     _pipeline;

    std::vector<SVMesh3D*> _meshes;
public:
    void loadFromFiles(const char* pathV, const char* pathF);
    void loadFromBuffer(const char* buffer, uint32_t size);

    void addMesh(SVMesh3D* mesh) {
        _meshes.push_back(mesh);
    }

    void destroy(){
        _pipeline->destroy();
        delete _pipeline;
    }

    // internal
    void i_addToRenderer(VkDevice& device, VkExtent2D size);
    void i_bindMaterial(VkCommandBuffer* commandBuffer);
    void i_draw(VkCommandBuffer& cmdBuffer);

};

#endif