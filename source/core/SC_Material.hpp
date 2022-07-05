#ifndef SPECTRE_CORE_MATERIAL
#define SPECTRE_CORE_MATERIAL

/*
 * For now an empty shell, will handle the loading of materials into the renderer
 * */

#include <vulkan/vulkan.hpp>

class SC_Material{
private:
    uint32_t*       _shaderBuffer;
    uint32_t        _shaderBufferSize;
    VkShaderModule  _shaderModule;
public:
    void loadFromFile(const char* path);
    void loadFromBuffer(const char* path);

    // internal
    void i_createShaderModule(VkDevice& device);
};

#endif