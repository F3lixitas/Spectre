#include "window/SWSWindow.hpp"
#include "window/SWSLabel.hpp"
#include "vulkan/SVRenderer.hpp"

int main(int argc, char* argv[]){

    SWSWidgetInfo winInfo;
    winInfo.sizeX = 400;

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
    rendererWidgetInfo.offsetY = 25;
    rendererWidgetInfo.sizeX = 200;
    rendererWidgetInfo.sizeY = 50;
    rendererWidgetInfo.parent = &window;
    rendererWidget.create(rendererWidgetInfo);

    SVRenderer renderer(&rendererWidget);
    renderer.init();


    window.addChild(&someWidget, 1);

    while(!window.shouldClose()){
        window.proc();
        someWidget.setText(L"something");
    }

    renderer.destroy();
    rendererWidget.destroy();
    someWidget.destroy();
    window.destroy();

    return 0;
}

