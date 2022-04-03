#include "index_buffer.h"
#include "constants.h"
#include "buffer.h"

#include <vulkan/vulkan.h>

#define ARRAY_SIZE(x) sizeof(x[0]) * x.size()

void Renderer::createIndexBuffer(Context *ctx) {
    VkDeviceSize bufferSize = ARRAY_SIZE(indices);

    VkBuffer stagingBuffer{};
    VkDeviceMemory stagingBufferMemory;
    createBuffer(ctx, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                 stagingBufferMemory);

    void *data;
    vkMapMemory(ctx->device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t) bufferSize);
    vkUnmapMemory(ctx->device, stagingBufferMemory);

    createBuffer(ctx, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, ctx->indexBuffer, ctx->indexBufferMemory);

    Renderer::copyBuffer(ctx, stagingBuffer, ctx->indexBuffer, bufferSize);

    vkDestroyBuffer(ctx->device, stagingBuffer, nullptr);
    vkFreeMemory(ctx->device, stagingBufferMemory, nullptr);
}
