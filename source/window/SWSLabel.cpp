#include "SWSLabel.hpp"

void SWSLabel::setText(std::wstring text) {
    _gc = XCreateGC(_display, _window, 0, nullptr);
    _text = text;
    clear();
    XFontStruct* font = XLoadQueryFont(_display, "fixed");
    XSetFont(_display, _gc, font->fid);

    XChar2b* a = new XChar2b[_text.length()];
    for(int i = 0; i < _text.length(); i++){
        a[i].byte2 = _text[i] & 0x00ff;
        a[i].byte1 = (_text[i] & 0xff00) >> 8;
    }

    XDrawString16(_display, _window, _gc, 0, 10, a, (int)_text.length());
    delete[] a;
}

