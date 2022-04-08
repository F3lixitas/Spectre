#include "Console.hpp"

void SWSConsole::create(SWSWidgetInfo &info) {
    SWSDrawable::create(info);
//    SWSWidgetInfo textInfo;
//    textInfo.sizeX = info.sizeX - 4;
//    textInfo.sizeY = info.sizeY - 4;
//    textInfo.offsetX = 2;
//    textInfo.offsetY = 2;
//    textInfo.parent = this;
//    _text.create(textInfo);
}

void SWSConsole::printLogs() {
    clear();
    updateDisplay();
}

void SWSConsole::updateDisplay() {
    uint16_t lines = (_height - 4) / 15;
    if(logs->size() > 0 && lines > 0){
        SLog& log = *logs->next(true);
        switch(log.type){
            case S_LOG_SUCCESS:
                drawText(L"[SUCCESS]", 2, (lines) * 15, 0xFF00FF00);
                drawText(log.text, 60, (lines) * 15, COLOR_FG1);
                break;
            case S_LOG_INFO:
                drawText(L"[INFO]", 2, (lines) * 15, COLOR_FG1);
                drawText(log.text, 40, (lines) * 15, COLOR_FG1);
                break;
            case S_LOG_WARNING:
                drawText(L"[WARNING]", 2, (lines) * 15, 0xFFA06000);
                drawText(log.text, 60, (lines) * 15, COLOR_FG1);
                break;
            case S_LOG_ERROR:
                drawText(L"[ERROR]", 2, (lines) * 15, 0xFFFF0000);
                drawText(log.text, 45, (lines) * 15, COLOR_FG1);
                break;
            case S_LOG_CRITICAL:
                drawText(L"[CRITICAL]", 2, (lines) * 15, 0xFFFF0000);
                drawText(log.text, 65, (lines) * 15, 0xFFFF0000);
                break;
        }
        for(uint16_t i = 1; i < lines && i < logs->size(); i++){
            SLog& l = *logs->next();
            switch(l.type){
                case S_LOG_SUCCESS:
                    drawText(L"[SUCCESS]", 2, (lines - i) * 15, 0xFF00FF00);
                    drawText(l.text, 60, (lines - i) * 15, COLOR_FG1);
                    break;
                case S_LOG_INFO:
                    drawText(L"[INFO]", 2, (lines - i) * 15, COLOR_FG1);
                    drawText(l.text, 40, (lines - i) * 15, COLOR_FG1);
                    break;
                case S_LOG_WARNING:
                    drawText(L"[WARNING]", 2, (lines - i) * 15, 0xFFA06000);
                    drawText(l.text, 60, (lines - i) * 15, COLOR_FG1);
                    break;
                case S_LOG_ERROR:
                    drawText(L"[ERROR]", 2, (lines - i) * 15, 0xFFFF0000);
                    drawText(l.text, 45, (lines - i) * 15, COLOR_FG1);
                    break;
                case S_LOG_CRITICAL:
                    drawText(L"[CRITICAL]", 2, (lines - i) * 15, 0xFFFF0000);
                    drawText(l.text, 65, (lines - i) * 15, 0xFFFF0000);
                    break;
            }
        }
    }

}

void SWSConsole::drawText(std::wstring text, uint32_t posX, uint32_t posY, uint32_t color) {
#if defined __linux__ || defined __APPLE__

    uint32_t values[3];
    xcb_font_t font;
    font = xcb_generate_id(_connection);
    xcb_open_font(_connection, font, 5, "fixed");

    values[0] = color;
    values[1] = COLOR_BG1;
    values[2] = font;
    xcb_change_gc(_connection, _gc, XCB_GC_FOREGROUND | XCB_GC_BACKGROUND | XCB_GC_FONT, values);

    xcb_close_font(_connection, font);

    xcb_char2b_t * a = new xcb_char2b_t [text.length()];

    for(int i = 0; i < text.length(); i++){
        a[i].byte2 = text[i] & 0x00ff;
        a[i].byte1 = (text[i] & 0xff00) >> 8;
    }

    xcb_image_text_16(_connection, text.length(), _window, _gc, posX, posY, a);

    delete[] a;
    xcb_flush(_connection);
#endif
}