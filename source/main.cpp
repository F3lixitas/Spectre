#include "window/SWSWindow.hpp"

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

    SWSWidget someWidget;
    someWidget.create(widgetInfo);

    window.addChild(&someWidget, 1);

    while(!window.shouldClose()){
        window.proc();
    }

    return 0;
}

