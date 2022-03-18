#ifndef SPECTRE_WINDOWING_SYSTEM_CONTAINER
#define SPECTRE_WINDOWING_SYSTEM_CONTAINER

#include "SWSWidget.hpp"
#include "../utils/STypes.hpp"

class SWSContainer : public SWSWidget {
protected:
    SIndexedList<SWSWidget> _children;
    unsigned int    _amountOfChildren;

public:
    SWSContainer() : SWSWidget(){
        _amountOfChildren = 0;
    }

    void addChild(SWSWidget* child, unsigned long index);
    void proc(xcb_generic_event_t* event);
};

#endif