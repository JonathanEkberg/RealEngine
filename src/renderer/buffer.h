#pragma once

#include "device.h"

#include <vulkan/vulkan.hpp>

namespace Renderer {
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDevice device,
                      VkPhysicalDevice physicalDevice, VkBuffer &buffer, VkDeviceMemory &bufferMemory);

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDevice device,
                    VkCommandPool commandPool, VkQueue graphicsQueue);
}