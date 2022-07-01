#include "window/SWSRenderWindow.hpp"
#include "window/SWSRenderButton.hpp"

/*
 *
 * positions in the UI are set in pixels
 *
 * */


int main(int argc, char* argv[]){

    SWSWidgetInfo info;
    info.sizeX = 1280;
    info.sizeY = 720;
    info.offsetX = 10;
    info.offsetY = 10;
    SWSRenderWindow window;
    window.create(info);

    SWSRenderButton button;
    SWSWidgetInfo binfo;
    binfo.sizeX = 100;
    binfo.sizeY = 50;
    binfo.offsetX = 50;
    binfo.offsetY = 50;
    button.create(binfo, &window);

    while(!window.shouldClose()){
        window.proc();
    }
    window.destroy();

    return 0;
}

