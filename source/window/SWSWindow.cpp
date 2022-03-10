#include "SWSWindow.hpp"

void SWSWindow::proc() {
#if defined __linux__ || defined __APPLE__
    XNextEvent(_display, &_event);
    if (_event.type == Expose) {
        XFillRectangle(_display, _window, DefaultGC(_display, _screenID), 20, 20, 10, 10);
        XDrawString(_display, _window, DefaultGC(_display, _screenID), 10, 50, "hello world", 11);
    }
    if (_event.type == KeyPress)
        _shouldClose = true;
#elif defined _WIN32
    MSG message;
    while (::PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
#endif
}

bool SWSWindow::shouldClose() {
    return _shouldClose;
}