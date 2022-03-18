#include "SVSwapchain.hpp"
#include <iostream>

void SVSwapchain::init(VkDevice &logicalDevice, VkPhysicalDevice *physicalDevice, VkSurfaceKHR &surface,
                       VkRect2D displayDimensions, VkFormat displayFormat, VkColorSpaceKHR colorSpace) {
    VkSurfaceCapabilitiesKHR surfaceCapa;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice[0], surface, &surfaceCapa);

    uint32_t surfaceFormatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice[0], surface, &surfaceFormatCount, nullptr);
    VkSurfaceFormatKHR* surfaceFormats = new VkSurfaceFormatKHR[surfaceFormatCount];
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice[0], surface, &surfaceFormatCount, surfaceFormats);

    for(int i = 0; i < surfaceFormatCount; i++){
        std::cout << "format :" << surfaceFormats[i].format << " color space :" << surfaceFormats[i].colorSpace << std::endl;
    }

    uint32_t imageCount = surfaceCapa.maxImageCount < 3 ? surfaceCapa.maxImageCount : 3;
    imageCount = surfaceCapa.minImageCount > 3 ? surfaceCapa.minImageCount : imageCount;

    VkImageUsageFlags imageUsage = surfaceCapa.supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;


    uint32_t amountOfPresentModes;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice[0], surface, &amountOfPresentModes, nullptr);
    VkPresentModeKHR *presentModes = new VkPresentModeKHR[amountOfPresentModes];
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice[0], surface, &amountOfPresentModes, presentModes);
    VkPresentModeKHR chosenPresentMode = presentModes[0];


    for(int i = 0; i < amountOfPresentModes; i++){
        switch (presentModes[i])
        {
            case VK_PRESENT_MODE_MAILBOX_KHR:
                chosenPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
                break;
            case VK_PRESENT_MODE_FIFO_RELAXED_KHR:
                chosenPresentMode = !(chosenPresentMode == VK_PRESENT_MODE_MAILBOX_KHR) ? VK_PRESENT_MODE_FIFO_RELAXED_KHR : chosenPresentMode;
                break;
            case VK_PRESENT_MODE_FIFO_KHR:
                chosenPresentMode =
                        !(chosenPresentMode == VK_PRESENT_MODE_MAILBOX_KHR || chosenPresentMode == VK_PRESENT_MODE_FIFO_RELAXED_KHR) ?
                        VK_PRESENT_MODE_FIFO_KHR : chosenPresentMode;
                break;
            default:
                chosenPresentMode =
                        !(chosenPresentMode == VK_PRESENT_MODE_MAILBOX_KHR || chosenPresentMode == VK_PRESENT_MODE_FIFO_RELAXED_KHR || chosenPresentMode == VK_PRESENT_MODE_FIFO_KHR) ?
                        presentModes[i] : chosenPresentMode;
                break;
        }
    }

    VkSwapchainCreateInfoKHR swapchainCreateInfo;
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.pNext = nullptr;
    swapchainCreateInfo.flags = 0;
    swapchainCreateInfo.surface = surface;
    swapchainCreateInfo.minImageCount = imageCount;
    swapchainCreateInfo.imageFormat = displayFormat;
    swapchainCreateInfo.imageColorSpace = colorSpace;
    swapchainCreateInfo.imageExtent = displayDimensions.extent;
    swapchainCreateInfo.imageArrayLayers = 1;
    swapchainCreateInfo.imageUsage = imageUsage;
    swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchainCreateInfo.queueFamilyIndexCount = 0;
    swapchainCreateInfo.pQueueFamilyIndices = nullptr;
    swapchainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCreateInfo.presentMode = chosenPresentMode;
    swapchainCreateInfo.clipped = VK_TRUE;
    swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

    vkCreateSwapchainKHR(logicalDevice, &swapchainCreateInfo, nullptr, &_swapchain);


    delete[] presentModes;
}
