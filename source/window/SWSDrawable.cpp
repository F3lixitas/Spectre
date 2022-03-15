#include "SWSDrawable.hpp"

void SWSDrawable::clear() {
#if defined __linux__ || __APPLE__
    XClearWindow(_display, _window);
#endif
}

