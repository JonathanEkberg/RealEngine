#pragma once

#include "context.h"

namespace Renderer {
    void createCommandBuffers(VkDevice device, VkCommandPool commandPool,
                              std::vector<VkCommandBuffer> *commandBuffers);

    void recordCommandBuffer(uint32_t imageIndex, VkPipeline graphicsPipeline, VkCommandBuffer commandBuffer,
                             VkBuffer vertexBuffer, VkBuffer indexBuffer, VkRenderPass renderPass,
                             VkExtent2D swapChainExtent, std::vector<VkFramebuffer> swapChainFrameBuffers);
}