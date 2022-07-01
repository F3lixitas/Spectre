#ifndef SPECTRE_WINDOWING_SYSTEM_RENDER_BUTTON
#define SPECTRE_WINDOWING_SYSTEM_RENDER_BUTTON

#include "SWSRenderWindow.hpp"

class SWSRenderButton{
    SWSRenderWindow* parent;
public:
    void create(SWSWidgetInfo &info, SWSRenderWindow *parent);


};

#endif