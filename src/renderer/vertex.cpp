#include "vertex.h"
#include "constants.h"
#include "device.h"
#include "buffer.h"

#include <stdexcept>

void Renderer::createVertexBuffer(Renderer::Context *ctx) {
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer{};
    VkDeviceMemory stagingBufferMemory;
    createBuffer(ctx, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                 stagingBufferMemory);

    void *data;
    vkMapMemory(ctx->device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t) bufferSize);
    vkUnmapMemory(ctx->device, stagingBufferMemory);

    createBuffer(ctx, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, ctx->vertexBuffer, ctx->vertexBufferMemory);

    Renderer::copyBuffer(ctx, stagingBuffer, ctx->vertexBuffer, bufferSize);

    vkDestroyBuffer(ctx->device, stagingBuffer, nullptr);
    vkFreeMemory(ctx->device, stagingBufferMemory, nullptr);
}

VkVertexInputBindingDescription Renderer::Vertex::getBindingDescription() {
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 2> Renderer::Vertex::getAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

    // Position types
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, pos);

    // Color types
    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    return attributeDescriptions;
}
