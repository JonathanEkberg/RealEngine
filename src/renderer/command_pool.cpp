#include "command_pool.h"

void Renderer::createCommandPool(Renderer::Context *ctx) {
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(ctx->physicalDevice, ctx->surface);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(ctx->device, &poolInfo, nullptr, &ctx->commandPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create command pool!");
    }
}
