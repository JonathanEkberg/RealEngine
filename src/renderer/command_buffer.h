#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace Renderer {
    void createCommandBuffers(VkDevice device, VkCommandPool commandPool,
                              std::vector<VkCommandBuffer> &commandBuffers);

    struct RecordCommandBufferInfo {
        uint32_t imageIndex;
        VkPipeline graphicsPipeline;
        VkCommandBuffer &commandBuffer;
        VkBuffer vertexBuffer;
        VkBuffer indexBuffer;
        VkRenderPass renderPass;
        VkExtent2D swapChainExtent;
        std::vector<VkFramebuffer> swapChainFrameBuffers;
    };

    void recordCommandBuffer(RecordCommandBufferInfo info);
}