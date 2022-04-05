#pragma once

#include "context.h"

namespace Renderer {
    void createIndexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool,
                           VkQueue graphicsQueue, VkBuffer *indexBuffer, VkDeviceMemory *indexBufferMemory);
}