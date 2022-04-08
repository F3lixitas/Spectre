#ifndef SPECTRE_CONSOLE
#define SPECTRE_CONSOLE

#include "../conf.hpp"
#include "../window/SWSDrawable.hpp"
#include "../utils/SIndexedList.hpp"


class SWSConsole : public SWSDrawable{
private:
    void updateDisplay();
    void drawText(std::wstring text, uint32_t posX, uint32_t posY, uint32_t color = COLOR_FG1);
public:
    void create(SWSWidgetInfo& info);
    void printLogs();
};

#endif