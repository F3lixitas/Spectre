#ifndef SPECTRE_WINDOWING_SYSTEM_WINDOW
#define SPECTRE_WINDOWING_SYSTEM_WINDOW

#include "SWSContainer.hpp"

class SWSWindow : public SWSContainer {
private:
#if defined __linux__ || defined __APPLE__

#endif
    xcb_generic_event_t*    _event;
    bool            _shouldClose = false;
public:
    SWSWindow() : SWSContainer() {

    }
    bool shouldClose();
    void proc();

    void destroy();
};

#endif