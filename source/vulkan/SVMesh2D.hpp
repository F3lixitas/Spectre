#ifndef SPECTRE_VULKAN_VERTEX
#define SPECTRE_VULKAN_VERTEX

#include "../conf.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include "SVUtils.hpp"

struct SVVertex2D{
    glm::vec2 position;
    glm::vec3 color;
    glm::vec2 UV;

    static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
};

class SVMesh2D{
private:
    VkBuffer               _vertexBuffer;
    VkBuffer               _indexBuffer;

    VkDeviceMemory         _vertexBufferMemory;
    VkDeviceMemory         _indexBufferMemory;

    VkDevice*              _logicalDevice;
    uint32_t               _amountOfVertices;
    uint32_t               _amountOfIndices;
public:
    SVMesh2D(VkDevice* logicalDevice){
        _logicalDevice = logicalDevice;
    }

    void destroy(){
        vkFreeMemory(*_logicalDevice, _indexBufferMemory, nullptr);
        vkDestroyBuffer(*_logicalDevice, _indexBuffer, nullptr);

        vkFreeMemory(*_logicalDevice, _vertexBufferMemory, nullptr);
        vkDestroyBuffer(*_logicalDevice, _vertexBuffer, nullptr);
    }

    SLog loadVertices(std::vector<SVVertex2D>* RESTRICT vertices, std::vector<uint32_t>* RESTRICT indices, VkPhysicalDevice* RESTRICT physicalDevice);
    void draw(VkCommandBuffer* commandBuffer);
    void bind(VkCommandBuffer* commandBuffer);

};

#endif