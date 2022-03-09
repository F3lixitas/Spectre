#include "window/SWSWindow.hpp"

int main(int argc, char* argv[]){

    SWidgetInfo winInfo;
    winInfo.sizeX = 400;

    SWSWindow window;
    window.create(winInfo);

    while(!window.shouldClose()){
        window.proc();
    }

    return 0;
}

