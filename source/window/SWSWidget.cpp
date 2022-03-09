#include "SWSWidget.hpp"

SWSWidget::SWSWidget() {

}

#if defined _WIN32
LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam){
    switch(msg){
        case WM_CREATE:
            break;
        case WM_DESTROY:
            break;
    }
    return 0;
}

#endif


void SWSWidget::create(const SWSWidgetInfo& info) {
#if defined __linux__ || defined __APPLE__
    _display = XOpenDisplay(nullptr);
    _screen = DefaultScreenOfDisplay(_display);
    _screenID = DefaultScreen(_display);

    _window = XCreateSimpleWindow(_display, RootWindowOfScreen(_screen), info.offsetX, info.offsetY, info.sizeX, info.sizeY,
                                  0, BlackPixel(_display, _screenID), WhitePixel(_display, _screenID));
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
    wc.lpfnWndProc = wndProc;
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