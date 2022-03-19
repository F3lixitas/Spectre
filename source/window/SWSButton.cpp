#include "SWSButton.hpp"
#include <iostream>
void SWSButton::create(SWSWidgetInfo &info) {
    info.flags |= SPECTRE_MOUSE_BUTTON_PRESS;
    SWSContainer::create(info);
}

void SWSButton::create(SWSButtonInfo &info) {
    _onClick = info.onClick;
    SWSWidgetInfo wInfo;
    wInfo.offsetX = info.offsetX;
    wInfo.offsetY = info.offsetY;
    wInfo.sizeX = info.sizeX;
    wInfo.sizeY = info.sizeY;
    wInfo.parent = info.parent;
    wInfo.onCreate = info.onCreate;
    wInfo.onDestroy = info.onDestroy;
    wInfo.flags = SPECTRE_MOUSE_BUTTON_PRESS;
    SWSContainer::create(wInfo);
}

void SWSButton::proc(xcb_generic_event_t *event) {
    std::cout << "button proc\n";


    switch(event->response_type){
        case XCB_BUTTON_PRESS:
            xcb_button_press_event_t *bpevent = (xcb_button_press_event_t *)event;
            if(bpevent->root == SWSContainer::_window) std::cout << "button clicked!" <<std::endl;
            std::cout << (uint32_t)bpevent->detail << std::endl;
            switch(bpevent->detail){
                case 1:
                    if(_onClick) _onClick();
                    std::cout << "button clicked\n";
                    break;
            }
            break;
    }

    SWSContainer::proc(event);
}