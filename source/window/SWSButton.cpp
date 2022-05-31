#include "SWSButton.hpp"
#include <iostream>
void SWSButton::create(SWSWidgetInfo &info) {
    info.flags |= SPECTRE_MOUSE_BUTTON_PRESS;
    SWSDrawable::create(info);
    uint32_t        values[2]  = {_screen->black_pixel, 0};
    xcb_change_gc(_connection, _gc, XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES, values);
    xcb_flush(_connection);
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
    SWSDrawable::create(wInfo);
    uint32_t        values[2]  = {_screen->black_pixel, 0};
    xcb_change_gc(_connection, _gc, XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES, values);
    xcb_flush(_connection);
}

void SWSButton::draw() {
    xcb_arc_t            arcs[] = {
            {0, 0, 10, 10, 90 << 6, 90 << 6},
            {0, (int16_t)(_height - 11), 10, 10, 180 << 6, 90 << 6},
            {(int16_t)(_width - 11), 0, 10, 10, 0, 90 << 6},
            {(int16_t)(_width - 11), (int16_t)(_height - 11), 10, 10, 270 << 6, 90 << 6}};
    xcb_segment_t        segments[] = {
            {6, 0, (int16_t)(_width - 6), 0},
            {6, (int16_t)(_height - 1), (int16_t)(_width - 6), (int16_t)(_height - 1)},
            {0, 6, 0, (int16_t)(_height - 6)},
            {(int16_t)(_width - 1), 6, (int16_t)(_width - 1), (int16_t)(_height - 6)}};

    xcb_poly_arc(_connection, _window, _gc, 4, arcs);
    xcb_poly_segment(_connection, _window, _gc, 4, segments);
    xcb_flush (_connection);
}

#if defined __linux__ || defined __APPLE__
void SWSButton::proc(xcb_generic_event_t *event) {

    switch(event->response_type & ~0x80){
        case XCB_EXPOSE:
            draw();
            break;
        case XCB_BUTTON_PRESS:
            xcb_button_press_event_t *bpevent = (xcb_button_press_event_t *)event;
            if(bpevent->event == SWSContainer::_window){
                switch(bpevent->detail){
                    case 1:
                        if(_onClick) _onClick();
                        break;
                }
            }
            break;

    }
    SWSContainer::proc(event);
}
#endif