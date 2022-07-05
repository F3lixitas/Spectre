#ifndef SPECTRE_VULKAN_RENDERER_2D
#define SPECTRE_VULKAN_RENDERER_2D

#include "SVRenderer.hpp"
#include "SVMesh2D.hpp"
#include "../core/SC_Material.hpp"

class SVRenderer2D : SVRenderer{
private:
    std::vector<SVMesh2D> _meshes;
public:
    void addMeshData(std::vector<SVVertex2D>& vertices, std::vector<uint32_t>& indices, SC_Material* material);
};

#endif