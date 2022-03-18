#ifndef SPECTRE_WINDOWING_SYSTEM_DRAWABLE
#define SPECTRE_WINDOWING_SYSTEM_DRAWABLE

#include "SWSWidget.hpp"

class SWSDrawable : public SWSWidget {
protected:
    //GC _gc;
    xcb_gcontext_t _gc;
public:
    void create(const SWSWidgetInfo&);
    void clear();
};

#endif