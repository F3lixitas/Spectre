#ifndef SPECTRE_WINDOWING_SYSTEM_RENDER_BUTTON
#define SPECTRE_WINDOWING_SYSTEM_RENDER_BUTTON

#include "SWSRenderWindow.hpp"
#include "../core/SC_Material.hpp"

class SWSRenderButton{
    SWSRenderWindow*    _parent;
    SC_Material         _material;
public:
    void create(SWSWidgetInfo &info, SWSRenderWindow *parent);

};

#endif