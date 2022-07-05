#ifndef SPECTRE_VULKAN_RENDERER_3D
#define SPECTRE_VULKAN_RENDERER_3D

#include "SVRenderer.hpp"

class SVRenderer3D : SVRenderer{
private:

public:

    void addMeshData(std::vector<SVVertex3D>& vertices, std::vector<uint32_t>& indices);

};

#endif