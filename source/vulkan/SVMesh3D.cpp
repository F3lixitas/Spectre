#include "SVMesh3D.hpp"

SLog SVMesh3D::loadVertices(std::vector<SVVertex3D> *vertices, std::vector<uint32_t> *indices,
                            VkPhysicalDevice *physicalDevice) {
    _amountOfVertices = static_cast<uint32_t>(vertices->size());
    VkDeviceSize bufferSize = sizeof(SVVertex3D) * _amountOfVertices;
    SLog log = createBuffer(_logicalDevice, physicalDevice, bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, _vertexBuffer,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _vertexBufferMemory);
    if(log.type != S_LOG_SUCCESS) return {L"Failed to load vertices" + log.text, S_LOG_ERROR};

    void* data;
    vkMapMemory(*_logicalDevice, _vertexBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices->data(), static_cast<size_t>(bufferSize));
    vkUnmapMemory(*_logicalDevice, _vertexBufferMemory);

    _amountOfIndices = static_cast<uint32_t>(indices->size());
    VkDeviceSize indexBufferSize = sizeof(uint32_t) * _amountOfIndices;
    log = createBuffer(_logicalDevice, physicalDevice, indexBufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, _indexBuffer,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _indexBufferMemory);
    if(log.type != S_LOG_SUCCESS) return {L"Failed to load vertices" + log.text, S_LOG_ERROR};

    void* indexData;
    vkMapMemory(*_logicalDevice, _indexBufferMemory, 0, indexBufferSize, 0, &indexData);
    memcpy(indexData, indices->data(), static_cast<size_t>(indexBufferSize));
    vkUnmapMemory(*_logicalDevice, _indexBufferMemory);

    return {L"", S_LOG_SUCCESS};
}

void SVMesh3D::draw(VkCommandBuffer *commandBuffer) {
    if(_material != nullptr)
        _material->i_bindMaterial(commandBuffer);

    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(*commandBuffer, 0, 1, &_vertexBuffer, offsets);
    vkCmdBindIndexBuffer(*commandBuffer, _indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    vkCmdDrawIndexed(*commandBuffer, _amountOfIndices, 1, 0, 0, 0);
}

void SVMesh3D::bind(VkCommandBuffer *commandBuffer) {

}

std::vector<VkVertexInputAttributeDescription> SVVertex3D::getAttributeDescriptions() {
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions(3);
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(SVVertex3D, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(SVVertex3D, color);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(SVVertex3D, UV);
    return attributeDescriptions;
}

std::vector<VkVertexInputBindingDescription> SVVertex3D::getBindingDescriptions() {
    std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(SVVertex3D);
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescriptions;
}