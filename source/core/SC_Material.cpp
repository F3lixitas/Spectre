#include "SC_Material.hpp"

void SC_Material::i_createShaderModule(VkDevice& device){
    VkShaderModuleCreateInfo shaderInfo;
    shaderInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderInfo.pNext    = nullptr;
    shaderInfo.flags    = 0;
    shaderInfo.codeSize = _shaderBufferSize;
    shaderInfo.pCode    = _shaderBuffer;

    vkCreateShaderModule(device, &shaderInfo, nullptr, &_shaderModule);
}