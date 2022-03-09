#include "SWidget.hpp"

SWidget::SWidget() {

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


void SWidget::create(const SWidgetInfo& info) {
#if defined __linux__ || defined __APPLE__

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

void SWidget::onCreate(int a, int b) {
    if(_onCreate)
        _onCreate(a, b);
}

void SWidget::onDestroy(int a, int b) {
    if(_onDestroy)
        _onDestroy(a, b);
}