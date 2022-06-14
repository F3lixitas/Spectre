#include "window/SWSRenderWindow.hpp"

#include <iostream>


int main(int argc, char* argv[]){

    SWSWidgetInfo info;
    info.sizeX = 1280;
    info.sizeY = 720;
    SWSRenderWindow window;
    window.create(info);

    while(!window.shouldClose()){

        //window.proc();
    }

    //window.destroy();


    return 0;
}

