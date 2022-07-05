#include "SWSRenderButton.hpp"
#include <vector>
#include <iostream>

#define SPECTRE_NEW_WAY

void SWSRenderButton::create(SWSWidgetInfo &info, SWSRenderWindow *parent){
#ifdef SPECTRE_OLD_WAY
    /// old way, one single square
    float relPosX = ((float)info.offsetX / parent->getSize().x) * 2 - 1;
    float relPosY = ((float)info.offsetY / parent->getSize().y) * 2 - 1;
    float relSizeX = ((float)info.sizeX / parent->getSize().x) * 2;
    float relSizeY = ((float)info.sizeY / parent->getSize().y) * 2;

    std::cout << "x: " << relPosX << ", y: " << relPosY << ", sizeX: " << relSizeX << ", sizeY: " << relSizeY << std::endl;

    std::vector<SVVertex3D> vertices = {
            {{relPosX, relPosY, 0}, {1, 1, 1}, {0, 0}},
            {{relPosX, relPosY + relSizeY, 0}, {1, 1, 1}, {0, 1}},
            {{relPosX + relSizeX, relPosY, 0}, {1, 1, 1}, {1, 0}},
            {{relPosX + relSizeX, relPosY + relSizeY, 0}, {1, 1, 1}, {1, 1}}
    };

    std::vector<uint32_t> indices = {0, 1, 2, 1, 2, 3};
    parent->getRenderer()->addMeshData(vertices, indices);
#else
    /// separate by border sections
    /// 5 pixels for border

    // center section
    {
        float relPosX = ((float) (info.offsetX + 5) / parent->getSize().x) * 2 - 1;
        float relPosY = ((float) (info.offsetY + 5) / parent->getSize().y) * 2 - 1;
        float relSizeX = ((float) (info.sizeX - 10) / parent->getSize().x) * 2;
        float relSizeY = ((float) (info.sizeY - 10) / parent->getSize().y) * 2;

        std::vector<SVVertex3D> vertices = {
                {{relPosX,            relPosY,            0}, {1, 1, 1}, {0, 0}},
                {{relPosX,            relPosY + relSizeY, 0}, {1, 1, 1}, {0, 1}},
                {{relPosX + relSizeX, relPosY,            0}, {1, 1, 1}, {1, 0}},
                {{relPosX + relSizeX, relPosY + relSizeY, 0}, {1, 1, 1}, {1, 1}}
        };
        std::vector<uint32_t> indices = {0, 1, 2, 1, 2, 3};
        parent->getRenderer()->addMeshData(vertices, indices);
    }

    // left
    {
        float relPosX = ((float) info.offsetX / parent->getSize().x) * 2 - 1;
        float relPosY = ((float) (info.offsetY + 5) / parent->getSize().y) * 2 - 1;
        float relSizeX = (5.0f / parent->getSize().x) * 2;
        float relSizeY = ((float) (info.sizeY - 10) / parent->getSize().y) * 2;

        std::vector<SVVertex3D> vertices = {
                {{relPosX,            relPosY,            0}, {1, 1, 1}, {0, 0}},
                {{relPosX,            relPosY + relSizeY, 0}, {1, 1, 1}, {0, 1}},
                {{relPosX + relSizeX, relPosY,            0}, {1, 1, 1}, {1, 0}},
                {{relPosX + relSizeX, relPosY + relSizeY, 0}, {1, 1, 1}, {1, 1}}
        };
        std::vector<uint32_t> indices = {0, 1, 2, 1, 2, 3};
        parent->getRenderer()->addMeshData(vertices, indices);
    }
    // right
    {
        float relPosX = ((float) (info.offsetX + info.sizeX - 5) / parent->getSize().x) * 2 - 1;
        float relPosY = ((float) (info.offsetY + 5) / parent->getSize().y) * 2 - 1;
        float relSizeX = (5.0f / parent->getSize().x) * 2;
        float relSizeY = ((float) (info.sizeY - 10) / parent->getSize().y) * 2;

        std::vector<SVVertex3D> vertices = {
                {{relPosX,            relPosY,            0}, {1, 1, 1}, {0, 0}},
                {{relPosX,            relPosY + relSizeY, 0}, {1, 1, 1}, {0, 1}},
                {{relPosX + relSizeX, relPosY,            0}, {1, 1, 1}, {1, 0}},
                {{relPosX + relSizeX, relPosY + relSizeY, 0}, {1, 1, 1}, {1, 1}}
        };
        std::vector<uint32_t> indices = {0, 1, 2, 1, 2, 3};
        parent->getRenderer()->addMeshData(vertices, indices);
    }
    // top
    {
        float relPosX = ((float) (info.offsetX + 5) / parent->getSize().x) * 2 - 1;
        float relPosY = ((float) (info.offsetY) / parent->getSize().y) * 2 - 1;
        float relSizeX = ((float) (info.sizeX - 10) / parent->getSize().x) * 2;
        float relSizeY = (5.0f / parent->getSize().y) * 2;

        std::vector<SVVertex3D> vertices = {
                {{relPosX,            relPosY,            0}, {1, 1, 1}, {0, 0}},
                {{relPosX,            relPosY + relSizeY, 0}, {1, 1, 1}, {0, 1}},
                {{relPosX + relSizeX, relPosY,            0}, {1, 1, 1}, {1, 0}},
                {{relPosX + relSizeX, relPosY + relSizeY, 0}, {1, 1, 1}, {1, 1}}
        };
        std::vector<uint32_t> indices = {0, 1, 2, 1, 2, 3};
        parent->getRenderer()->addMeshData(vertices, indices);
    }
    // bottom
    {
        float relPosX = ((float) (info.offsetX + 5) / parent->getSize().x) * 2 - 1;
        float relPosY = ((float) (info.offsetY + info.sizeY - 5) / parent->getSize().y) * 2 - 1;
        float relSizeX = ((float) (info.sizeX - 10) / parent->getSize().x) * 2;
        float relSizeY = (5.0f / parent->getSize().y) * 2;

        std::vector<SVVertex3D> vertices = {
                {{relPosX,            relPosY,            0}, {1, 1, 1}, {0, 0}},
                {{relPosX,            relPosY + relSizeY, 0}, {1, 1, 1}, {0, 1}},
                {{relPosX + relSizeX, relPosY,            0}, {1, 1, 1}, {1, 0}},
                {{relPosX + relSizeX, relPosY + relSizeY, 0}, {1, 1, 1}, {1, 1}}
        };
        std::vector<uint32_t> indices = {0, 1, 2, 1, 2, 3};
        parent->getRenderer()->addMeshData(vertices, indices);
    }

    // top-left corner
    {
        float relPosX = ((float) (info.offsetX) / parent->getSize().x) * 2 - 1;
        float relPosY = ((float) (info.offsetY) / parent->getSize().y) * 2 - 1;
        float relSizeX = (5.0f / parent->getSize().x) * 2;
        float relSizeY = (5.0f / parent->getSize().y) * 2;

        std::vector<SVVertex3D> vertices = {
                {{relPosX,            relPosY,            0}, {1, 1, 1}, {0, 0}},
                {{relPosX,            relPosY + relSizeY, 0}, {1, 1, 1}, {0, 1}},
                {{relPosX + relSizeX, relPosY,            0}, {1, 1, 1}, {1, 0}},
                {{relPosX + relSizeX, relPosY + relSizeY, 0}, {1, 1, 1}, {1, 1}}
        };
        std::vector<uint32_t> indices = {0, 1, 2, 1, 2, 3};
        parent->getRenderer()->addMeshData(vertices, indices);
    }
    // top-right corner
    {
        float relPosX = ((float) (info.offsetX + info.sizeX - 5) / parent->getSize().x) * 2 - 1;
        float relPosY = ((float) (info.offsetY) / parent->getSize().y) * 2 - 1;
        float relSizeX = (5.0f / parent->getSize().x) * 2;
        float relSizeY = (5.0f / parent->getSize().y) * 2;

        std::vector<SVVertex3D> vertices = {
                {{relPosX,            relPosY,            0}, {1, 1, 1}, {0, 0}},
                {{relPosX,            relPosY + relSizeY, 0}, {1, 1, 1}, {0, 1}},
                {{relPosX + relSizeX, relPosY,            0}, {1, 1, 1}, {1, 0}},
                {{relPosX + relSizeX, relPosY + relSizeY, 0}, {1, 1, 1}, {1, 1}}
        };
        std::vector<uint32_t> indices = {0, 1, 2, 1, 2, 3};
        parent->getRenderer()->addMeshData(vertices, indices);
    }
    // bottom-left corner
    {
        float relPosX = ((float) (info.offsetX) / parent->getSize().x) * 2 - 1;
        float relPosY = ((float) (info.offsetY + info.sizeY - 5) / parent->getSize().y) * 2 - 1;
        float relSizeX = (5.0f / parent->getSize().x) * 2;
        float relSizeY = (5.0f / parent->getSize().y) * 2;

        std::vector<SVVertex3D> vertices = {
                {{relPosX,            relPosY,            0}, {1, 1, 1}, {0, 0}},
                {{relPosX,            relPosY + relSizeY, 0}, {1, 1, 1}, {0, 1}},
                {{relPosX + relSizeX, relPosY,            0}, {1, 1, 1}, {1, 0}},
                {{relPosX + relSizeX, relPosY + relSizeY, 0}, {1, 1, 1}, {1, 1}}
        };
        std::vector<uint32_t> indices = {0, 1, 2, 1, 2, 3};
        parent->getRenderer()->addMeshData(vertices, indices);
    }
    // top-left corner
    {
        float relPosX = ((float) (info.offsetX + info.sizeX - 5) / parent->getSize().x) * 2 - 1;
        float relPosY = ((float) (info.offsetY + info.sizeY - 5) / parent->getSize().y) * 2 - 1;
        float relSizeX = (5.0f / parent->getSize().x) * 2;
        float relSizeY = (5.0f / parent->getSize().y) * 2;

        std::vector<SVVertex3D> vertices = {
                {{relPosX,            relPosY,            0}, {1, 1, 1}, {0, 0}},
                {{relPosX,            relPosY + relSizeY, 0}, {1, 1, 1}, {0, 1}},
                {{relPosX + relSizeX, relPosY,            0}, {1, 1, 1}, {1, 0}},
                {{relPosX + relSizeX, relPosY + relSizeY, 0}, {1, 1, 1}, {1, 1}}
        };
        std::vector<uint32_t> indices = {0, 1, 2, 1, 2, 3};
        parent->getRenderer()->addMeshData(vertices, indices);
    }

#endif



}