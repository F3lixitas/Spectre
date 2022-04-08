#include "window/SWSWindow.hpp"
#include "window/SWSLabel.hpp"
#include "vulkan/SVRenderer.hpp"
#include "vulkan/SVMesh2D.hpp"
#include "window/SWSButton.hpp"

SVRenderer * Renderer;

void buttonClick(){
    std::vector<SVVertex2D> vertices(3);
    std::vector<uint32_t> indices(3);
    vertices[0] = {{-0.5, -0.5}, {1, 0, 0}, {0, 0}};
    vertices[1] = {{-0.5, 0.5}, {0, 1, 0}, {0, 0}};
    vertices[2] = {{0.5, -0.5}, {0, 0, 1}, {0, 0}};

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;

    Renderer->addMeshData(vertices, indices);
}

int main(int argc, char* argv[]){
    SWSWidgetInfo winInfo{};
    winInfo.sizeX = 1280;
    winInfo.sizeY = 720;

    SWSWindow window;
    window.create(winInfo);

    SVWidget rendererWidget;
    SWSWidgetInfo rendererWidgetInfo{};
    rendererWidgetInfo.offsetX = 100;
    rendererWidgetInfo.offsetY = 20;
    rendererWidgetInfo.sizeX = 1000;
    rendererWidgetInfo.sizeY = 700;
    rendererWidgetInfo.parent = &window;
    rendererWidget.create(rendererWidgetInfo);

    SVRenderer renderer(&rendererWidget);
    renderer.init();
    Renderer = &renderer;

    std::vector<SVVertex2D> vertices(3);
    std::vector<uint32_t> indices(3);

    vertices[0] = {{0.5, 0.5}, {1, 0, 0}, {0, 0}};
    vertices[1] = {{-0.5, 0.5}, {0, 1, 0}, {0, 0}};
    vertices[2] = {{0.5, -0.5}, {0, 0, 1}, {0, 0}};

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;

    renderer.addMeshData(vertices, indices);

    SWSButtonInfo buttonInfo{};
    buttonInfo.offsetX = 25;
    buttonInfo.offsetY = 100;
    buttonInfo.sizeX = 50;
    buttonInfo.sizeY = 50;
    buttonInfo.parent = &window;
    buttonInfo.onClick = &buttonClick;

    SWSButton button;
    button.create(buttonInfo);

    SWSWidgetInfo widgetInfo{};
    widgetInfo.sizeX = 50;
    widgetInfo.sizeY = 50;
    widgetInfo.parent = &button;

    SWSLabel someWidget;
    someWidget.create(widgetInfo);

    button.addChild(&someWidget, 1);
    window.addChild(&button, 3);
    window.addChild(&rendererWidget, 2);

    while(!window.shouldClose()){
        window.proc();
        renderer.render();
        someWidget.setText(L"something");
    }

    renderer.destroy();
    window.destroy();


    return 0;
}

