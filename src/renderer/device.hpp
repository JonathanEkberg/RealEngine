#pragma once

#include "context.hpp"

#include <optional>

using std::optional;

struct QueueFamilyIndices {
    optional<uint32_t> graphicsFamily;
    optional<uint32_t> presentFamily;

    [[nodiscard]] bool isComplete() const;
};

namespace Renderer {
    void pickPhysicalDevice(Context *ctx);

    void createLogicalDevice(Context *ctx);

    bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);

    static bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
}