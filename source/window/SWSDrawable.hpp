#ifndef SPECTRE_WINDOWING_SYSTEM_DRAWABLE
#define SPECTRE_WINDOWING_SYSTEM_DRAWABLE

#include "SWSWidget.hpp"

class SWSDrawable : public SWSWidget {
protected:
#if defined __linux__ || defined __APPLE__
    xcb_gcontext_t _gc;
#endif
public:
    void create(const SWSWidgetInfo&);
    void clear();
};

#endif