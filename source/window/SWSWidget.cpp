#include "SWSWidget.hpp"

SWSWidget::SWSWidget() {

}

#if defined _WIN32
LRESULT CALLBACK SWSWidget::wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam){
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

SWSWindowHandle SWSWidget::getHandle() const {
    return {_window, _connection, _screen};
}

void SWSWidget::create(const SWSWidgetInfo& info) {
#if defined __linux__ || defined __APPLE__
    //SWSWindowHandle parentHandle;
    //if(info.parent) {
    //    parentHandle = info.parent->getHandle();
    //    _display = parentHandle.display;
    //} else {
    //    _display = XOpenDisplay(nullptr);
    //}
//
    //_screen = DefaultScreenOfDisplay(_display);
    //_screenID = DefaultScreen(_display);
//
    //_window = XCreateSimpleWindow(_display, info.parent ? parentHandle.window : RootWindowOfScreen(_screen), info.offsetX, info.offsetY, info.sizeX, info.sizeY,
    //                              1, BlackPixel(_display, _screenID), WhitePixel(_display, _screenID));
    //XSelectInput(_display, _window, KeyPressMask|KeyReleaseMask|StructureNotifyMask|ExposureMask);
    //XMapRaised(_display, _window);

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

    value[0] = _screen->white_pixel;
    value[1] = XCB_EVENT_MASK_EXPOSURE;

    xcb_create_window(_connection, XCB_COPY_FROM_PARENT, _window, info.parent ? parentHandle.window : _screen->root,
                      info.offsetX, info.offsetY, info.sizeX, info.sizeY, 1, XCB_WINDOW_CLASS_INPUT_OUTPUT,
                      _screen->root_visual, XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK, value);

    xcb_map_window(_connection, _window);

    xcb_flush(_connection);

#elif defined _WIN32
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
    wc.lpfnWndProc = &wndProc;

    ::RegisterClassEx(&wc);

    _widgetHandle = ::CreateWindowExW(WS_EX_OVERLAPPEDWINDOW, LPCWSTR(L"Spectre"), LPCWSTR(L"Spectre"), WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, CW_USEDEFAULT, 720, 480, nullptr, nullptr, nullptr, nullptr);

    ::ShowWindow(_widgetHandle, SW_SHOW);
    ::UpdateWindow(_widgetHandle);
#endif
}

void SWSWidget::destroy() {
    xcb_disconnect(_connection);
}

void SWSWidget::proc(xcb_generic_event_t* event){
    switch(event->response_type){
        case XCB_EXPOSE:
            xcb_expose_event_t *eevent = (xcb_expose_event_t*)event;
            if(eevent->window == _window){
                onCreate(5, 5);
            }
    }
}

void SWSWidget::onCreate(int a, int b) {
    if(_onCreate)
        _onCreate(a, b);
}

void SWSWidget::onDestroy(int a, int b) {
    if(_onDestroy)
        _onDestroy(a, b);
}