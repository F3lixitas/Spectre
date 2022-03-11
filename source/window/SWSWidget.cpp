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
    return {_display, _window, _gc};
}

void SWSWidget::create(const SWSWidgetInfo& info) {
#if defined __linux__ || defined __APPLE__
    SWSWindowHandle parentHandle;
    if(info.parent) {
        parentHandle = info.parent->getHandle();
        _display = parentHandle.display;
    } else {
        _display = XOpenDisplay(nullptr);
    }

    _screen = DefaultScreenOfDisplay(_display);
    _screenID = DefaultScreen(_display);

    _window = XCreateSimpleWindow(_display, info.parent ? parentHandle.window : RootWindowOfScreen(_screen), info.offsetX, info.offsetY, info.sizeX, info.sizeY,
                                  1, BlackPixel(_display, _screenID), WhitePixel(_display, _screenID));
    XSelectInput(_display, _window, KeyPressMask|KeyReleaseMask|StructureNotifyMask|ExposureMask);
    XMapRaised(_display, _window);
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

void SWSWidget::onCreate(int a, int b) {
    if(_onCreate)
        _onCreate(a, b);
}

void SWSWidget::onDestroy(int a, int b) {
    if(_onDestroy)
        _onDestroy(a, b);
}