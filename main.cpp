#include <iostream>
#include <vulkan/vulkan.hpp>

int main() {
    VkApplicationInfo appInfo;
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr;
    appInfo.apiVersion = VK_API_VERSION_1_2;
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
    appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 0);
    appInfo.pApplicationName = "Vulkan Renderer";
    appInfo.pEngineName = "Vulkan Engine";

    std::vector<const char*> layers = {"VK_LAYER_KHRONOS_validation"};

    VkInstanceCreateInfo instCreateInfo;
    instCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instCreateInfo.pNext = nullptr;
    instCreateInfo.flags = 0;
    instCreateInfo.enabledExtensionCount = 0;
    instCreateInfo.enabledLayerCount = layers.size();
    instCreateInfo.pApplicationInfo = &appInfo;
    instCreateInfo.ppEnabledExtensionNames = nullptr;
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

    return 0;
}
