#include "SVWidget.hpp"

void SVWidget::loadSurface(VkInstance &instance, VkSurfaceKHR &surface) {
#if defined __linux__ || defined __APPLE__

    VkXcbSurfaceCreateInfoKHR xcbCreateInfo;
    xcbCreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    xcbCreateInfo.pNext = nullptr;
    xcbCreateInfo.flags = 0;
    xcbCreateInfo.connection = _connection;
    xcbCreateInfo.window = _window;

    vkCreateXcbSurfaceKHR(instance, &xcbCreateInfo, nullptr, &surface);

#elif defined _WIN32

    VkWin32SurfaceCreateInfoKHR win32CreateInfo;
    win32CreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    win32CreateInfo.pNext = nullptr;
    win32CreateInfo.flags = 0;
    win32CreateInfo.hwnd = _widgetHandle;
    win32CreateInfo.hinstance = nullptr;

    vkCreateWin32SurfaceKHR(instance, &win32CreateInfo, nullptr, &surface);

#endif
}

