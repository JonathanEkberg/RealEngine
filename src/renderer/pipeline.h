#pragma once

#include <vulkan/vulkan.h>

namespace Renderer {
    void createGraphicsPipeline(VkDevice device, VkRenderPass renderPass, VkExtent2D swapChainExtent,
                                VkPipeline &graphicsPipeline, VkPipelineLayout &pipelineLayout);
}
