#include "SWSLabel.hpp"

void SWSLabel::setText(std::wstring text) {
    _text = text;
    clear();
#if defined __linux__ || defined __APPLE__

    uint32_t values[3];
    xcb_font_t font;
    font = xcb_generate_id(_connection);
    xcb_open_font(_connection, font, 5, "fixed");

    values[0] = 0x00FF0000;
    values[1] = COLOR_BG_TRANSPARENT;
    values[2] = font;
    xcb_change_gc(_connection, _gc, XCB_GC_FOREGROUND | XCB_GC_BACKGROUND | XCB_GC_FONT, values);

    xcb_close_font(_connection, font);

    uint8_t * a = new uint8_t [_text.length() * 2];
    for(int i = 0; i < _text.length(); i++){
        a[i * 2]= _text[i] & 0x00ff;
        a[i * 2 + 1] = (_text[i] & 0xff00) >> 8;
    }

    xcb_poly_text_8(_connection, _window, _gc, 0, 10, 4, (uint8_t*)"text");
    //xcb_image_text_16(_connection, _text.length(), _window, _gc, 0, 10, a);

    delete[] a;
    xcb_flush(_connection);
#endif
}

