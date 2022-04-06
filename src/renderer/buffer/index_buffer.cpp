#include "index_buffer.h"
#include "../constants.h"
#include "buffer.h"

#include <vulkan/vulkan.h>

#define ARRAY_SIZE(x) sizeof(x[0]) * x.size()

void Renderer::createIndexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool,
                                 VkQueue graphicsQueue, VkBuffer *indexBuffer, VkDeviceMemory *indexBufferMemory) {
    VkDeviceSize bufferSize = ARRAY_SIZE(indices);

    VkBuffer stagingBuffer{};
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, device,
                 physicalDevice, stagingBuffer, stagingBufferMemory);

    void *data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t) bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    Renderer::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                           VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, device, physicalDevice, *indexBuffer,
                           *indexBufferMemory);

    Renderer::copyBuffer(stagingBuffer, *indexBuffer, bufferSize, device, commandPool, graphicsQueue);
    //    copyBuffer(ctx, stagingBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}
