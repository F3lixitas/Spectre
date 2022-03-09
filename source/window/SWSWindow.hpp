#ifndef SPECTRE_WINDOWING_SYSTEM_WINDOW
#define SPECTRE_WINDOWING_SYSTEM_WINDOW

#include "SWSContainer.hpp"

class SWSWindow : public SWSContainer {
private:
    XEvent          _event;
    bool            _shouldClose = false;
public:
    SWSWindow() : SWSContainer() {

    }
    bool shouldClose();
    void proc();
};

#endif