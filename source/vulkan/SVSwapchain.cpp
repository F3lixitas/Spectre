#include "SVSwapchain.hpp"
#include <iostream>

SVSwapchain::SVSwapchain(VkDevice* device) : _device(device){

}

void SVSwapchain::beginRenderPass(VkCommandBuffer& commandBuffer, VkFramebuffer& framebuffer, VkExtent2D size){
    VkClearValue clearValue = {0.0f, 0.0f, 0.0f, 1.0f};

    VkRenderPassBeginInfo renderPassBeginInfo;
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = nullptr;
    renderPassBeginInfo.renderPass = _renderPass;
    renderPassBeginInfo.renderArea.offset = {0, 0};
    renderPassBeginInfo.renderArea.extent = size;
    renderPassBeginInfo.framebuffer = framebuffer;
    renderPassBeginInfo.clearValueCount = 1;
    renderPassBeginInfo.pClearValues = &clearValue;

    vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void SVSwapchain::createRenderPass(){
    VkAttachmentDescription attachmentDescription{};
    attachmentDescription.flags             = 0;
    attachmentDescription.format            = VK_FORMAT_B8G8R8A8_SRGB;
    attachmentDescription.samples           = VK_SAMPLE_COUNT_1_BIT;
    attachmentDescription.loadOp            = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachmentDescription.storeOp           = VK_ATTACHMENT_STORE_OP_STORE;
    attachmentDescription.stencilLoadOp     = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachmentDescription.stencilStoreOp    = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachmentDescription.initialLayout     = VK_IMAGE_LAYOUT_UNDEFINED;
    attachmentDescription.finalLayout       = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference attachmentReference{};
    attachmentReference.attachment  = 0;
    attachmentReference.layout      = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpassDescription{};
    subpassDescription.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments    = &attachmentReference;

    VkSubpassDependency subpassDependency;
    subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpassDependency.dstSubpass        = 0;
    subpassDependency.srcStageMask      = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.dstStageMask      = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.srcAccessMask     = 0;
    subpassDependency.dstAccessMask     = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    subpassDependency.dependencyFlags   = 0;

    VkRenderPassCreateInfo renderPassCreateInfo;
    renderPassCreateInfo.sType              = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.pNext              = nullptr;
    renderPassCreateInfo.flags              = 0;
    renderPassCreateInfo.attachmentCount    = 1;
    renderPassCreateInfo.pAttachments       = &attachmentDescription;
    renderPassCreateInfo.subpassCount       = 1;
    renderPassCreateInfo.pSubpasses         = &subpassDescription;
    renderPassCreateInfo.dependencyCount    = 1;
    renderPassCreateInfo.pDependencies      = &subpassDependency;

    vkCreateRenderPass(*_device, &renderPassCreateInfo, nullptr, &_renderPass);
}

void SVSwapchain::init(VkPhysicalDevice *physicalDevice, VkSurfaceKHR &surface,
                       VkRect2D displayDimensions, VkFormat displayFormat, VkColorSpaceKHR colorSpace) {
    VkSurfaceCapabilitiesKHR surfaceCapa;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*physicalDevice, surface, &surfaceCapa);

    uint32_t surfaceFormatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(*physicalDevice, surface, &surfaceFormatCount, nullptr);
    VkSurfaceFormatKHR* surfaceFormats = new VkSurfaceFormatKHR[surfaceFormatCount];
    vkGetPhysicalDeviceSurfaceFormatsKHR(*physicalDevice, surface, &surfaceFormatCount, surfaceFormats);

    for(int i = 0; i < surfaceFormatCount; i++){
        std::cout << "format :" << surfaceFormats[i].format << " color space :" << surfaceFormats[i].colorSpace << std::endl;
    }

    uint32_t imageCount = surfaceCapa.maxImageCount < 3 ? surfaceCapa.maxImageCount : 3;
    imageCount = surfaceCapa.minImageCount > 3 ? surfaceCapa.minImageCount : imageCount;

    VkImageUsageFlags imageUsage = surfaceCapa.supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;


    uint32_t amountOfPresentModes;
    vkGetPhysicalDeviceSurfacePresentModesKHR(*physicalDevice, surface, &amountOfPresentModes, nullptr);
    VkPresentModeKHR *presentModes = new VkPresentModeKHR[amountOfPresentModes];
    vkGetPhysicalDeviceSurfacePresentModesKHR(*physicalDevice, surface, &amountOfPresentModes, presentModes);
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

    vkCreateSwapchainKHR(*_device, &swapchainCreateInfo, nullptr, &_swapchain);

    delete[] presentModes;

    createRenderPass();
}
