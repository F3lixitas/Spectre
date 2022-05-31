#ifndef SPECTRE_WINDOWING_SYSTEM_DRAWABLE
#define SPECTRE_WINDOWING_SYSTEM_DRAWABLE

#include "SWSContainer.hpp"

class SWSDrawable : public SWSContainer {
protected:
#if defined __linux__ || defined __APPLE__
    uint32_t _width, _height;
    xcb_gcontext_t _gc;
#endif
public:
    void create(const SWSWidgetInfo&);
    virtual void draw(){}
    void clear();
};

#endif