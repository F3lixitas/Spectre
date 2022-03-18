#include "SWSWindow.hpp"
#include <iostream>

void SWSWindow::proc() {
#if defined __linux__ || defined __APPLE__
    if((_event = xcb_wait_for_event(_connection)))
        SWSContainer::proc(_event);
    else
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

void SWSWindow::destroy() {
    xcb_flush(_connection);
    free(_event);
    xcb_disconnect(_connection);
}