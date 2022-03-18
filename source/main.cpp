#include "window/SWSWindow.hpp"
#include "window/SWSLabel.hpp"
#include "vulkan/SVRenderer.hpp"
#include "vulkan/SVMesh.hpp"

int main(int argc, char* argv[]){

    SWSWidgetInfo winInfo;
    winInfo.sizeX = 1280;
    winInfo.sizeY = 720;

    SWSWindow window;
    window.create(winInfo);

    SWSWidgetInfo widgetInfo;
    widgetInfo.offsetX = 25;
    widgetInfo.offsetY = 25;
    widgetInfo.sizeX = 50;
    widgetInfo.sizeY = 50;
    widgetInfo.parent = &window;

    SWSLabel someWidget;
    someWidget.create(widgetInfo);

    SVWidget rendererWidget;
    SWSWidgetInfo rendererWidgetInfo;
    rendererWidgetInfo.offsetX = 100;
    rendererWidgetInfo.offsetY = 20;
    rendererWidgetInfo.sizeX = 1000;
    rendererWidgetInfo.sizeY = 700;
    rendererWidgetInfo.parent = &window;
    rendererWidget.create(rendererWidgetInfo);

    SVRenderer renderer(&rendererWidget);
    renderer.init();

    std::vector<SVVertex> vertices(3);
    std::vector<uint32_t> indices(3);

    vertices[0] = {{0.5, 0.5}, {1, 0, 0}, {0, 0}};
    vertices[1] = {{-0.5, 0.5}, {0, 1, 0}, {0, 0}};
    vertices[2] = {{0.5, -0.5}, {0, 0, 1}, {0, 0}};

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;

    renderer.addMeshData(vertices, indices);

    window.addChild(&someWidget, 1);
    window.addChild(&rendererWidget, 2);

    while(!window.shouldClose()){
        window.proc();
        renderer.render();
        someWidget.setText(L"something");
    }

    renderer.destroy();
    rendererWidget.destroy();
    someWidget.destroy();
    window.destroy();

    return 0;
}

