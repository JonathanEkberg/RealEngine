#pragma once

#include <vulkan/vulkan.h>

namespace Renderer {
    struct CreateVertexBufferInfo {
        VkDevice device;
        VkPhysicalDevice physicalDevice;
        VkCommandPool commandPool;
        VkQueue graphicsQueue;
        VkBuffer &vertexBuffer;
        VkDeviceMemory &pVertexBufferMemory;
    };

    void createVertexBuffer(CreateVertexBufferInfo &info);
}
