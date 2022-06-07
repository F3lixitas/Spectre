#ifndef SPECTRE_WINDOWING_SYSTEM_LABEL
#define SPECTRE_WINDOWING_SYSTEM_LABEL

#include "SWSDrawable.hpp"
#include <string>

class SWSLabel : public SWSDrawable {
private:
    std::wstring _text;
public:
    void setText(std::wstring text, uint32_t textColor = COLOR_FG1, uint32_t backgroundColor = COLOR_BG1);
};

#endif