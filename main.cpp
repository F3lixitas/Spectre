#include <iostream>
#ifdef __linux
#include <X11/Xlib.h>
#define VK_USE_PLATFORM_XLIB_KHR
#elif _WIN32
#include <Windows.h>
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include <vulkan/vulkan.hpp>

int main() {
#ifdef __linux
    Display* display = XOpenDisplay(nullptr);
    Screen* screen = DefaultScreenOfDisplay(display);
    int screenID = DefaultScreen(display);
    Window window = XCreateSimpleWindow(display, RootWindowOfScreen(screen), 0, 0, 720, 480, 0, BlackPixel(display, screenID),
                                        WhitePixel(display, screenID));
    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);
#endif


    VkApplicationInfo appInfo;
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr;
    appInfo.apiVersion = VK_API_VERSION_1_2;
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
    appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 0);
    appInfo.pApplicationName = "Vulkan Renderer";
    appInfo.pEngineName = "Vulkan Engine";

    std::vector<const char*> layers = {"VK_LAYER_KHRONOS_validation"};

#ifdef __linux
    const char* extensions[] = {"VK_KHR_xlib_surface", "VK_KHR_surface"};
#elif _WIN32
    const char* extensions[] = {"VK_KHR_win32_surface", "VK_KHR_surface"};
#endif

    VkInstanceCreateInfo instCreateInfo;
    instCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instCreateInfo.pNext = nullptr;
    instCreateInfo.flags = 0;
    instCreateInfo.enabledExtensionCount = 2;
    instCreateInfo.enabledLayerCount = layers.size();
    instCreateInfo.pApplicationInfo = &appInfo;
    instCreateInfo.ppEnabledExtensionNames = extensions;
    instCreateInfo.ppEnabledLayerNames = layers.data();

    VkInstance instance;

    vkCreateInstance(&instCreateInfo, nullptr, &instance);

    uint32_t amountOfPhysicalDevices;

    vkEnumeratePhysicalDevices(instance, &amountOfPhysicalDevices, nullptr);
    VkPhysicalDevice *physicalDevices = new VkPhysicalDevice[amountOfPhysicalDevices];
    vkEnumeratePhysicalDevices(instance, &amountOfPhysicalDevices, physicalDevices);

    for(int i = 0; i < amountOfPhysicalDevices; i++){
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(physicalDevices[i], &props);
        std::cout << "api version : " << props.apiVersion << "\ndriver version : " << props.driverVersion
                  << "\nvendor ID : " << props.vendorID << "\ndevice ID : " << props.deviceID << "\ndevice type : "
                  << props.deviceType << "\ndevice name : " << props.deviceName << "\npipeline cache UUID : "
                  << props.pipelineCacheUUID << "\n\n";
    }

    VkSurfaceKHR  surface;

#ifdef __linux
    VkXlibSurfaceCreateInfoKHR xlibCreateInfo;
    xlibCreateInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    xlibCreateInfo.pNext = nullptr;
    xlibCreateInfo.flags = 0;
    xlibCreateInfo.dpy = display;
    xlibCreateInfo.window = window;

    vkCreateXlibSurfaceKHR(instance, &xlibCreateInfo, nullptr, &surface);

    XEvent e;
    while(true){
        XNextEvent(display, &e);
        if (e.type == Expose) {
            XFillRectangle(display, window, DefaultGC(display, screenID), 20, 20, 10, 10);
            XDrawString(display, window, DefaultGC(display, screenID), 10, 50, "hello world", 11);
        }
        if (e.type == KeyPress)
            break;
    }
    XCloseDisplay(display);
#endif

    return 0;
}
