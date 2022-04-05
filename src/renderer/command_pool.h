#pragma once

#include "device.h"

namespace Renderer {
    void createCommandPool(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
                           VkCommandPool *commandPool);
}