#ifndef SPECTRE_WINDOWING_SYSTEM_LABEL
#define SPECTRE_WINDOWING_SYSTEM_LABEL

#include "SWSDrawable.hpp"
#include <string>

class SWSLabel : public SWSDrawable {
private:
    std::wstring _text;
public:
    void setText(std::wstring text);
};

#endif