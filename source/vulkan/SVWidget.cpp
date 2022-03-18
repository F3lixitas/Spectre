#include "SVWidget.hpp"

void SVWidget::loadSurface(VkInstance &instance, VkSurfaceKHR &surface) {
    //VkXlibSurfaceCreateInfoKHR xlibCreateInfo;
    //xlibCreateInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    //xlibCreateInfo.pNext = nullptr;
    //xlibCreateInfo.flags = 0;
    //xlibCreateInfo.dpy = _display;
    //xlibCreateInfo.window = _window;
//
    //vkCreateXlibSurfaceKHR(instance, &xlibCreateInfo, nullptr, &surface);

    VkXcbSurfaceCreateInfoKHR xcbCreateInfo;
    xcbCreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    xcbCreateInfo.pNext = nullptr;
    xcbCreateInfo.flags = 0;
    xcbCreateInfo.connection = _connection;
    xcbCreateInfo.window = _window;

    vkCreateXcbSurfaceKHR(instance, &xcbCreateInfo, nullptr, &surface);
}

