#include "buffer.h"
#include "vertex_buffer.h"
#include "../constants.h"

void Renderer::createVertexBuffer(CreateVertexBufferInfo &info) {
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer{};
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, info.device,
                 info.physicalDevice, stagingBuffer, stagingBufferMemory);

    void *data;
    vkMapMemory(info.device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t) bufferSize);
    vkUnmapMemory(info.device, stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, info.device, info.physicalDevice, info.vertexBuffer,
                 info.pVertexBufferMemory);

    Renderer::copyBuffer(stagingBuffer, info.vertexBuffer, bufferSize, info.device, info.commandPool,
                         info.graphicsQueue);

    vkDestroyBuffer(info.device, stagingBuffer, nullptr);
    vkFreeMemory(info.device, stagingBufferMemory, nullptr);
}

