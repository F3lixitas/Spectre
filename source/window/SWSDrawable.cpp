#include "SWSDrawable.hpp"

void SWSDrawable::clear() {
#if defined __linux__ || __APPLE__
    xcb_clear_area(_connection, false, _window, 0, 0, 100, 100); //todo : put adapted values
#endif
}

void SWSDrawable::create(const SWSWidgetInfo &info) {
    SWSWidget::create(info);
    _gc = xcb_generate_id (_connection);
    uint32_t values[2];
    values[0] = _screen->black_pixel;
    values[1] = 0;
    xcb_create_gc (_connection, _gc, _window, XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES, values);

    xcb_flush(_connection);
}