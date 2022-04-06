#pragma once

#include <optional>
#include <vulkan/vulkan.h>

using std::optional;

struct QueueFamilyIndices {
    optional<uint32_t> graphicsFamily;
    optional<uint32_t> presentFamily;

    [[nodiscard]] bool isComplete() const;
};

namespace Renderer {
    void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, VkPhysicalDevice *physicalDevice);

    void createLogicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkDevice *device,
                             VkQueue *graphicsQueue, VkQueue *presentQueue);

    bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);

    static bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

    uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
}