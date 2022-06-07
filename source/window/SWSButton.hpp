#ifndef SPECTRE_WINDOWING_SYSTEM_BUTTON
#define SPECTRE_WINDOWING_SYSTEM_BUTTON

#include "SWSDrawable.hpp"

typedef struct SWSButtonInfo{
    int offsetX = 0;
    int offsetY = 0;
    int sizeX = 100;
    int sizeY = 100;
    uint32_t flags = 0;
    SWSWidget* parent = nullptr;
    void (*onCreate)(int,int) = nullptr;
    void (*onDestroy)(int, int) = nullptr;
    void (*onClick)() = nullptr;
}SWSButtonInfo;

class SWSButton : public SWSDrawable {
private:

    void (*_onClick)() = nullptr;
public:
    void create(SWSWidgetInfo& info);
    void create(SWSButtonInfo& info);
    void draw();
#if defined __linux__ || defined __APPLE__
    void proc(xcb_generic_event_t *event);
#endif
};

#endif