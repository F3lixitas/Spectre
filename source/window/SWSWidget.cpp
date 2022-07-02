#include "SWSWidget.hpp"
#include <iostream>

SWSWidget::SWSWidget() {

}

SWSWindowHandle SWSWidget::getHandle() const {
#if defined __linux__ || defined __APPLE__
    return {_window, _connection, _screen};
#elif defined _WIN32
    return {_widgetHandle};
#endif
}
#if defined _WIN32
LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam){
    switch(msg){
        case WM_CREATE:
            break;
        case WM_DESTROY:
            ::PostQuitMessage(0);
            break;
        default:
            return ::DefWindowProc(hwnd, msg, wparam, lparam);
    }
    return 0;
}
#endif

void SWSWidget::create(const SWSWidgetInfo& info) {
#if defined __linux__ || defined __APPLE__

    SWSWindowHandle parentHandle;
    if(info.parent){
        parentHandle = info.parent->getHandle();
        _connection = parentHandle.connection;
        _screen = parentHandle.screen;
    } else {
        _connection = xcb_connect(nullptr, &_screenID);
        xcb_screen_iterator_t iterator;
        iterator = xcb_setup_roots_iterator(xcb_get_setup(_connection));
        for(;iterator.rem; --_screenID, xcb_screen_next(&iterator)){
            if(_screenID == 0) {
                _screen = iterator.data;
                break;
            }
        }
    }

    _window = xcb_generate_id(_connection);

    uint32_t value[2];

    value[0] = COLOR_BG1;
    value[1] = XCB_EVENT_MASK_EXPOSURE | info.flags;

    xcb_create_window(_connection, XCB_COPY_FROM_PARENT, _window, info.parent ? parentHandle.window : _screen->root,
                      info.offsetX, info.offsetY, info.sizeX, info.sizeY, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT,
                      _screen->root_visual, XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK, value);

    if(!info.parent){
        xcb_change_property (_connection, XCB_PROP_MODE_REPLACE, _window,
                             XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
                             7, "Spectre");

        xcb_change_property (_connection, XCB_PROP_MODE_REPLACE, _window,
                             XCB_ATOM_WM_ICON_NAME, XCB_ATOM_STRING, 8,
                             7, "Spectre");
    }

    xcb_map_window(_connection, _window);

    xcb_flush(_connection);

#elif defined _WIN32

    /*wProc = (LRESULT CALLBACK(*)(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam))[](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT CALLBACK {
        switch(msg){
            case WM_CREATE:
                break;
            case WM_DESTROY:
                ::PostQuitMessage(0);
                break;
            default:
                return ::DefWindowProc(hwnd, msg, wparam, lparam);
        }
        return 0;
    };*/

    std::cout << "window created !\n";
    if(info.parent != nullptr){
        _widgetHandle = ::CreateWindowExW(0, LPCWSTR(L"Spectre"), nullptr, WS_CHILD | WS_BORDER,
                                          info.offsetX, info.offsetY, info.sizeX, info.sizeY, info.parent->_widgetHandle, nullptr, nullptr, nullptr);
    } else {
        WNDCLASSEX wc;
        wc.cbClsExtra = 0;
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.cbWndExtra = 0;
        wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
        wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
        wc.lpszClassName = LPCSTR ("Spectre");
        wc.lpszMenuName = LPCSTR ("");
        wc.lpfnWndProc = nullptr;

        if(!::RegisterClassEx(&wc)) abort();
        _widgetHandle = ::CreateWindowExW(WS_EX_OVERLAPPEDWINDOW, LPCWSTR(L"Spectre"), LPCWSTR(L"Spectre"), WS_OVERLAPPEDWINDOW,
                                          info.offsetX, info.offsetY, info.sizeX, info.sizeY, nullptr, nullptr, nullptr, nullptr);


    }
    if(!_widgetHandle) std::cout << "Bad creation \n";
    ::ShowWindow(_widgetHandle, SW_SHOW);
    ::UpdateWindow(_widgetHandle);
#endif
}

#if defined __linux__ || defined __APPLE__
void SWSWidget::proc(xcb_generic_event_t* event){
    switch(event->response_type){
        case XCB_EXPOSE:
            xcb_expose_event_t *eevent = (xcb_expose_event_t*)event;
            if(eevent->window == _window){
                onCreate(5, 5);
            }
    }
}
#endif

void SWSWidget::onCreate(int a, int b) {
    if(_onCreate)
        _onCreate(a, b);
}

void SWSWidget::onDestroy(int a, int b) {
    if(_onDestroy)
        _onDestroy(a, b);
}