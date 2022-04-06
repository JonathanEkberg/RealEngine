#pragma once


#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include <array>

namespace Renderer {
    struct Vertex {
        glm::vec2 pos;
        glm::vec3 color;

        static VkVertexInputBindingDescription getBindingDescription();

        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
    };

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
