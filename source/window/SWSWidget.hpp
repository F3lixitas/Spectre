#ifndef SPECTRE_WINDOWING_SYSTEM_WIDGET
#define SPECTRE_WINDOWING_SYSTEM_WIDGET

#if defined __linux__ || defined __APPLE__
    #include <xcb/xcb.h>
    #include <xcb/xcb_atom.h>
#elif _WIN32
    #include <Windows.h>
    #include <inttypes.h>
#endif

#include "SWSFlags.hpp"

class SWSWidget;

typedef struct SWSWidgetInfo {
    int         offsetX = 0;
    int         offsetY = 0;
    int         sizeX = 100;
    int         sizeY = 100;
    uint32_t    flags = 0;
    SWSWidget*  parent = nullptr;
    void (*onCreate)(int,int) = nullptr;
    void (*onDestroy)(int, int) = nullptr;
} SWSWidgetInfo;

typedef struct SWSWindowHandle {
#if defined __linux__ || defined __APPLE__
    xcb_window_t        window;
    xcb_connection_t*   connection;
    xcb_screen_t*       screen;
#elif _WIN32
    HWND        windowHandle;
#endif
} SWSWindowHandle;

class SWSWidget {
protected:

#if defined __linux__ || defined __APPLE__
    xcb_connection_t*       _connection;
    xcb_screen_t*           _screen;
    xcb_window_t            _window;
    int                     _screenID;
#elif _WIN32
    HWND                    _widgetHandle;
    LRESULT CALLBACK (*wProc)(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#endif

    void (*_onCreate)(int, int) = nullptr;
    void (*_onDestroy)(int, int) = nullptr;

public:
    SWSWidget();
    virtual void create(const SWSWidgetInfo&);

#if defined __linux__ || defined __APPLE__
    virtual void proc(xcb_generic_event_t* event);
#endif
    SWSWindowHandle getHandle() const;
    void onCreate(int, int);
    void onDestroy(int, int);

};

#endif