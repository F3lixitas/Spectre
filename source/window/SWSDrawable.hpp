#ifndef SPECTRE_WINDOWING_SYSTEM_DRAWABLE
#define SPECTRE_WINDOWING_SYSTEM_DRAWABLE

#include "SWSWidget.hpp"

class SWSDrawable : public SWSWidget {
protected:
    GC _gc;
public:
    void clear();
};

#endif