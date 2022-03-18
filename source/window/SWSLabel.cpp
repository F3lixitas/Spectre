#include "SWSLabel.hpp"

void SWSLabel::setText(std::wstring text) {
    _text = text;
    clear();

    uint32_t values[3];
    xcb_font_t font;
    font = xcb_generate_id(_connection);
    xcb_open_font(_connection, font, 5, "fixed");

    values[0] = _screen->black_pixel;
    values[1] = _screen->white_pixel;
    values[2] = font;
    xcb_change_gc(_connection, _gc, XCB_GC_FOREGROUND | XCB_GC_BACKGROUND | XCB_GC_FONT, values);

    xcb_close_font(_connection, font);

    xcb_char2b_t* a = new xcb_char2b_t[_text.length()];
    for(int i = 0; i < _text.length(); i++){
        a[i].byte2 = _text[i] & 0x00ff;
        a[i].byte1 = (_text[i] & 0xff00) >> 8;
    }

    xcb_image_text_16(_connection, _text.length(), _window, _gc, 0, 10, a);

    delete[] a;
    xcb_flush(_connection);
}

