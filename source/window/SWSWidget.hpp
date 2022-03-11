#ifndef SPECTRE_WINDOWING_SYSTEM_WIDGET
#define SPECTRE_WINDOWING_SYSTEM_WIDGET

#if defined __linux__ || defined __APPLE__
    #include <X11/Xlib.h>
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
    GC          gc;
#elif _WIN32
    HWND        windowHandle;
#endif
} SWSWindowHandle;

class SWSWidget {
protected:

#if defined __linux__ || defined __APPLE__
    Display*    _display;
    Window      _window;
    GC          _gc;
    Screen*     _screen;
    int         _screenID;
#elif _WIN32
    HWND        _widgetHandle;
    LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#endif

    void (*_onCreate)(int, int);
    void (*_onDestroy)(int, int);

public:


    SWSWidget();
    void create(const SWSWidgetInfo&);

#if defined __linux__ || defined __APPLE__
    SWSWindowHandle getHandle() const;
#endif

    void onCreate(int, int);
    void onDestroy(int, int);

};

#endif