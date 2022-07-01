#include "SWSRenderButton.hpp"
#include <vector>
#include <iostream>

void SWSRenderButton::create(SWSWidgetInfo &info, SWSRenderWindow *parent){
    float relPosX = ((float)info.offsetX / parent->getSize().x) * 2 - 1;
    float relPosY = ((float)info.offsetY / parent->getSize().y) * 2 - 1;
    float relSizeX = ((float)info.sizeX / parent->getSize().x) * 2;
    float relSizeY = ((float)info.sizeY / parent->getSize().y) * 2;

    std::cout << "x: " << relPosX << ", y: " << relPosY << ", sizeX: " << relSizeX << ", sizeY: " << relSizeY << std::endl;

    std::vector<SVVertex3D> vertices = {
            {{relPosX, relPosY, 0}, {1, 1, 1}, {0, 0}},
            {{relPosX, relPosY + relSizeY, 0}, {1, 1, 1}, {0, 0}},
            {{relPosX + relSizeX, relPosY, 0}, {1, 1, 1}, {0, 0}},
            {{relPosX + relSizeX, relPosY + relSizeY, 0}, {1, 1, 1}, {0, 0}}
    };

    std::vector<uint32_t> indices = {0, 1, 2, 1, 2, 3};
    parent->getRenderer()->addMeshData(vertices, indices);
}