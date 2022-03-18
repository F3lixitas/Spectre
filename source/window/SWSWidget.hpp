#ifndef SPECTRE_WINDOWING_SYSTEM_WIDGET
#define SPECTRE_WINDOWING_SYSTEM_WIDGET

#if defined __linux__ || defined __APPLE__
    #include <X11/Xlib.h>
    #include <xcb/xcb.h>
#elif _WIN32
    #include <Windows.h>
#endif

class SWSWidget;

typedef struct SWSWidgetInfo {
    int offsetX = 0;
    int offsetY = 0;
    int sizeX = 100;
    int sizeY = 100;
    SWSWidget* parent = nullptr;
    void (*onCreate)(int,int) = nullptr;
    void (*onDestroy)(int, int) = nullptr;
} SWSWidgetInfo;

typedef struct SWSWindowHandle {
#if defined __linux__ || defined __APPLE__
    Display*    display;
    Window      window;
#elif _WIN32
    HWND        windowHandle;
#endif
} SWSWindowHandle;

class SWSWidget {
protected:

#if defined __linux__ || defined __APPLE__
//    Display*    _display;
//    Window      _window;
//    Screen*     _screen;
//    int         _screenID;

    xcb_connection_t*       _connection;
    xcb_screen_t*           _screen;
    xcb_window_t            _window;
    xcb_generic_event_t*    _event;
    int                     _screenID;
#elif _WIN32
    HWND        _widgetHandle;
    LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#endif

    void (*_onCreate)(int, int);
    void (*_onDestroy)(int, int);

public:
    SWSWidget();
    void create(const SWSWidgetInfo&);
    void destroy();

#if defined __linux__ || defined __APPLE__
    SWSWindowHandle getHandle() const;
#endif

    void onCreate(int, int);
    void onDestroy(int, int);
    virtual void proc();
};

#endif