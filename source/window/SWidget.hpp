#if defined __linux__ || defined __APPLE__
    #include <X11/Xlib.h>
#elif _WIN32
    #include <Windows.h>
#endif

class SWidget;

typedef struct SWidgetInfo{
    int offsetX = 0;
    int offsetY = 0;
    int sizeX = 100;
    int sizeY = 100;
    SWidget* parent = nullptr;
    void (*onCreate)(int,int) = nullptr;
    void (*onDestroy)(int, int) = nullptr;
} SWidgetInfo;

class SWidget {
private:

#if defined __linux__ || defined __APPLE__
    Display*    _display;
    Window      _window;
    GC          _gc;
    Screen*     _screen;
    int         _screenID;
#elif _WIN32
    HWND        _widgetHandle;
#endif

    void (*_onCreate)(int, int);
    void (*_onDestroy)(int, int);

public:

    SWidget();
    void create(const SWidgetInfo&);

    void onCreate(int, int);
    void onDestroy(int, int);

};
